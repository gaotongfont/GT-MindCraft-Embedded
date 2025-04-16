/**
 * @file gt_recording.c
 * @author Yang (your@email.com)
 * @brief
 * @version 0.1
 * @date 2024-08-27 09:59:36
 * @copyright Copyright (c) 2014-2024, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
// #include "gt_recording.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "board.h"
#include "esp_log.h"
// #include "fatfs_stream.h"
#include "i2s_stream.h"
// #include "wav_encoder.h"
#include "audio_mem.h"
#include "gt_audio_storage.h"

#include "gt_wakenet.h"
#if GT_USE_WAKENET
#include "esp_vad.h"
#define VAD_SAMPLE_RATE_HZ      16000
#define VAD_FRAME_LENGTH_MS     30
#endif

/* private define -------------------------------------------------------*/
#define GT_RING_BUFFER_SIZE         (2 * 1024)

#define GT_AUDIO_MEMORY_I2C_RATE    (16000)

/* private typedef ------------------------------------------------------*/
typedef struct {
    uint8_t lock_recording : 1;
    uint8_t use_keyword_wakeup : 1;
    uint8_t recording_state : 3;        /** @ref gt_recording_state_em */
    uint8_t reserved : 3;
}_audio_storage_st;

/* static prototypes ----------------------------------------------------*/



/* static variables -----------------------------------------------------*/
static const char *TAG = "GT_AUDIO_STORAGE";

static _audio_storage_st audio_storage = {
    .lock_recording = false,
    .recording_state = GT_RECORDING_STATE_NONE,
#if GT_USE_WAKENET
    .use_keyword_wakeup = true,
#endif
};

static audio_element_handle_t gt_i2s_stream_reader = NULL;
static ringbuf_handle_t gt_ringbuf01 = NULL;

/* macros ---------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/

static void get_pcm_data(void* arg) {
    int bytes_read = 0, chunk_size = GT_RING_BUFFER_SIZE;
    short* temp_buffer = (short*)audio_malloc(chunk_size);  // 临时缓存区
    memset(temp_buffer, 0, chunk_size);

#if GT_USE_WAKENET
    vad_handle_t vad_inst = vad_create(VAD_MODE_4);
    vad_state_t vad_state = (vad_state_t)0;
#endif
    while(1) {
        bytes_read = rb_read(gt_ringbuf01, (char*)temp_buffer, chunk_size, portMAX_DELAY);
        if (bytes_read > 0) {
            // 将读取的数据存储到pcm_buffer
            // ESP_LOGI(TAG, "get_pcm_data count: %d, value: %d %d %d", bytes_read, temp_buffer[0], temp_buffer[50], temp_buffer[100]);
            switch (audio_storage.recording_state) {
                case GT_RECORDING_STATE_WAKEUP: {
#if GT_USE_WAKENET
                    vad_state = vad_process(vad_inst, (int16_t *)temp_buffer, VAD_SAMPLE_RATE_HZ, VAD_FRAME_LENGTH_MS);
                    if (vad_state == VAD_SPEECH) {
                        // ESP_LOGI(TAG, "Speech detected");
                        gt_wakenet_recording_audio((int16_t * )temp_buffer, bytes_read);
                    } else {
                        // ESP_LOGI(TAG, "No speech detected");
                        gt_wakenet_prerecording_prev_audio_data((int16_t * )temp_buffer, bytes_read);
                        gt_wakenet_ready_to_detect((int16_t * )temp_buffer, bytes_read);
                    }
#endif
                    break;
                }
                case GT_RECORDING_STATE_UPLOAD_SERVER: {
                    gt_websocket_send_data((char*)temp_buffer);
                    break;
                }
                default:
                    break;
            }
        }
        // if (audio_element_get_state(gt_i2s_stream_reader) != AEL_STATE_RUNNING) { break; }
        vTaskDelay(10);
    }
    audio_free(temp_buffer);
    temp_buffer = NULL;
    ESP_LOGI(TAG, "get_pcm_data bytes read finish!!!!!!!!!!!");
    vTaskDelete(NULL);
}

static inline gt_recording_state_em _get_default_recording_state(void) {
    ESP_LOGI(TAG, "use_keyword_wakeup: %d, %d", audio_storage.use_keyword_wakeup, GT_USE_WAKENET);
    return audio_storage.use_keyword_wakeup ? GT_RECORDING_STATE_WAKEUP : GT_RECORDING_STATE_NONE;
}

static inline void _set_recording_state(gt_recording_state_em state) {
    audio_storage.recording_state = state;
}

static inline bool _is_recording_wakeup(void) {
    return audio_storage.recording_state == GT_RECORDING_STATE_WAKEUP;
}

/* global functions / API interface -------------------------------------*/

void gt_audio_storage_init(void)
{
    if(audio_storage.lock_recording) {
        ESP_LOGW(TAG, "recording is locked!");
        return;
    }
    // 为PCM数据分配内存
    ESP_LOGI(TAG, "[1.0] Create i2s stream to read audio data from codec chip");
    int bits_per_sample = 16;  // 假设每个样本为16位
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT_WITH_PARA(I2S_NUM_0, GT_AUDIO_MEMORY_I2C_RATE, bits_per_sample, AUDIO_STREAM_READER);
    // i2s_cfg.std_cfg.clk_cfg.clk_src = I2S_CLK_SRC_DEFAULT;
    i2s_cfg.std_cfg.clk_cfg.sample_rate_hz = 16000;
    // i2s_cfg.std_cfg.clk_cfg.mclk_multiple = I2S_MCLK_MULTIPLE_256;
    i2s_cfg.std_cfg.slot_cfg.slot_mode = I2S_SLOT_MODE_MONO;
    i2s_cfg.std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_LEFT;
    // i2s_cfg.stack_in_ext = true;
    i2s_cfg.use_alc = true;
    i2s_cfg.uninstall_drv = false;
    gt_i2s_stream_reader = i2s_stream_init(&i2s_cfg);
    ESP_LOGI(TAG, "[1.1] Create a ring buffer and insert it between i2s_stream_reader and the PCM buffer");
    gt_ringbuf01 = rb_create(GT_RING_BUFFER_SIZE, 32);
    if (!gt_ringbuf01) {
        ESP_LOGE(TAG, "Failed to create ring buffer");
        return;
    }
    _set_recording_state(_get_default_recording_state());
    audio_element_set_output_ringbuf(gt_i2s_stream_reader, gt_ringbuf01);
    xTaskCreate(get_pcm_data, "get_pcm_data", 3 * 1024, NULL, 5, NULL);

    audio_storage.lock_recording = true;
}

void gt_audio_storage_deinit(void)
{
    if(!audio_storage.lock_recording) return ;

    ESP_LOGI(TAG, "[4.0] Stop elements and release resources");
    /* Release all resources */
    audio_element_deinit(gt_i2s_stream_reader);

    gt_i2s_stream_reader = NULL;

    rb_destroy(gt_ringbuf01);
    gt_ringbuf01 = NULL;

    audio_storage.lock_recording = false;
}

void gt_audio_storage_start(gt_recording_state_em state)
{
    if(!gt_i2s_stream_reader) {
        ESP_LOGW(TAG, "recording not initialized!");
        return ;
    }
    ESP_LOGI(TAG, "[2.0] Start recording!");
    _set_recording_state(state);
    audio_element_run(gt_i2s_stream_reader);
    audio_element_resume(gt_i2s_stream_reader, 0, 0);
    ESP_LOGI(TAG, "i2s_stream_reader state: %d", audio_element_get_state(gt_i2s_stream_reader));
}

void gt_audio_storage_stop(void)
{
    if(!gt_i2s_stream_reader) {
        ESP_LOGW(TAG, " not initialized!");
        return ;
    }

    _set_recording_state(_get_default_recording_state());
    if (_is_recording_wakeup()) {
        return ;
    }
    ESP_LOGI(TAG, "[3.0] Stop recording!");
    audio_element_stop(gt_i2s_stream_reader);
}




/* end of file ----------------------------------------------------------*/


