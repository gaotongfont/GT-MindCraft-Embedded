/**
 * @file gt_wakenet.c
 * @author Feyoung
 * @brief
 * @version 0.1
 * @date 2025-02-14
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "gt_wakenet.h"

#include "model_path.h"
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "esp_log.h"
#include "dl_lib_convq_queue.h"
#include "string.h"
#include <sys/time.h>
#include <sys/dirent.h>
#if GT_USE_WAKENET || (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0))
#include "esp_log_timestamp.h"
#endif
#include "gt_audio_play.h"
#include "audio_tone_uri.h"


/* private define -------------------------------------------------------*/
#ifndef _WAKENET_WR_FILE
    #define _WAKENET_WR_FILE    01
#endif

#ifndef _TEST_PCM_FILE
    #define _TEST_PCM_FILE      0
#endif

#if _TEST_PCM_FILE
    #undef _WAKENET_WR_FILE
    #define _WAKENET_WR_FILE    0
#endif

#ifndef _WAKENET_STREAM_DETECT_MODE
    #define _WAKENET_STREAM_DETECT_MODE     0
#endif

#ifndef _WAKENET_TIMEOUT_MS
    #define _WAKENET_TIMEOUT_MS     350
#endif

/* private typedef ------------------------------------------------------*/
typedef struct {
    uint8_t has_audio : 1;          /** has audio ready to detect */
    uint8_t enabled : 1;
}_wakenet_reg_st;

typedef struct {
    esp_wn_iface_t * wakenet;
    model_iface_data_t * model_data;
    int audio_chunksize;
    int16_t * buffer;

    gt_wakeup_handler_t wakeup_handler_cb;
    void * user_data;

    uint32_t timeout_begin;

    int16_t * prev_pcm_p;
    uint32_t prev_pcm_len;

    _wakenet_reg_st reg;
}_wakenet_param_st;


/* static variables -----------------------------------------------------*/
static const char * TAG = "WAKENET";
static int16_t _prev_pcm_buffer[4 * 1024] = {0};

static _wakenet_param_st _wkn = {
    .prev_pcm_p = _prev_pcm_buffer,
    .prev_pcm_len = 0,
    .reg = {
        .enabled = true,
    },
};


#if _WAKENET_WR_FILE
const char * pcm_path = "/sdcard/wake.pcm";
#endif

/* macros ---------------------------------------------------------------*/



/* class ----------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/
static int _wakeup_default_handler_cb(void * user_data) {
    ESP_LOGI(TAG, ">>> _wakeup_default_handler_cb() detect ok !!! <<<");
#if 0
    gt_audio_player_start_sync(tone_uri[TONE_TYPE_DINGDONG]);
#endif
    return 0;
}

static void _recording_final_audio_data(FILE * fp, int16_t * buffer, int bytes_length)
{
#if _WAKENET_STREAM_DETECT_MODE
    if (false == gt_wakenet_is_running()) {
        return ;
    }
    fwrite(buffer, bytes_length, 1, fp);
    ESP_LOGI(TAG, ">>> final: %d", ftell(fp));
#endif
}

#if _TEST_PCM_FILE
static void _test_sdcard_audio_file(void) {
    const char * test_pcm_p = "/sdcard/hi_3.pcm";
    FILE * fp = fopen(test_pcm_p, "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        for (int ct = 0, per = file_size / 512; ct < per; ct++) {
            fread(_wkn.buffer, _wkn.audio_chunksize, 1, fp);
            int ret = _wkn.wakenet->detect(_wkn.model_data, _wkn.buffer);
            if (ret > 0) {
                printf(">>>>>>>>>> detect %s ok !!!! <<<<<<<<\nn", test_pcm_p);
                break;
            }
        }
        fclose(fp);
        fp = NULL;
    }
    return 0;
}
#endif

/* global functions / API interface -------------------------------------*/

#if GT_USE_WAKENET
void gt_wakenet_init(void)
{
    if (_wkn.wakenet) {
        return;
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
    srmodel_list_t *models = esp_srmodel_init("/sdcard/model");
    if (NULL == models || 0 == models->num) {
        esp_srmodel_deinit(models);
        ESP_LOGE(TAG, ">>> esp_srmodel_init() failed !!! <<<");
        return;
    }
    ESP_LOGI(TAG, "models num: %d", models->num);
    char *model_name = esp_srmodel_filter(models, ESP_WN_PREFIX, NULL);
    _wkn.wakenet = (esp_wn_iface_t*)esp_wn_handle_from_name(model_name);
    _wkn.model_data = _wkn.wakenet->create(model_name, DET_MODE_95);
    /** 512 * 2 = 1024 */
    _wkn.audio_chunksize = _wkn.wakenet->get_samp_chunksize(_wkn.model_data) * sizeof(int16_t);
    gt_wakenet_register_wakeup_handler_cb(_wakeup_default_handler_cb, NULL);
    // char * wake_words = esp_srmodel_get_wake_words(models, model_name);
    // printf(">> wake words: [%s]\n", wake_words);
    esp_srmodel_deinit(models);
    _wkn.buffer = malloc(_wkn.audio_chunksize);
#if _WAKENET_WR_FILE
    FILE * fp = fopen(pcm_path, "rb");
    if (fp) {
        fclose(fp);
        fp = NULL;
        remove(pcm_path);
    }
#endif
}

void gt_wakenet_register_wakeup_handler_cb(gt_wakeup_handler_t handler_cb, void * user_data)
{
    _wkn.wakeup_handler_cb = handler_cb;
    _wkn.user_data = user_data;
}

int gt_wakenet_recording_audio(int16_t * buffer, int bytes_length)
{
    if (bytes_length <= 0)              { return 1; }
    if (NULL == _wkn.buffer)            { return 2; }
    if (NULL == _wkn.wakeup_handler_cb) { return 3; }
    if (NULL == _wkn.wakenet)           { return 4; }
    if (false == gt_wakenet_is_running()) {
        return 0;
    }

#if _WAKENET_STREAM_DETECT_MODE
    int ret = 0;
    uint32_t half_size = _wkn.audio_chunksize >> 1;
    for (int i = 0, cnt = bytes_length / half_size; i < cnt; i++) {
    #if 01
        ret = _wkn.wakenet->detect(_wkn.model_data, buffer + i * half_size);
    #else
        memcpy(_wkn.buffer, buffer + i * half_size, _wkn.audio_chunksize);
        ret = _wkn.wakenet->detect(_wkn.model_data, _wkn.buffer);
    #endif
        if (ret > 0) {
            break;
        }
    }
    if (ret > 0 && _wkn.wakeup_handler_cb) {
        _wkn.wakeup_handler_cb(_wkn.user_data);
    } else {
        ESP_LOGI(TAG, ">>> detect: %d <<<", ret);
    }
#endif
#if _WAKENET_WR_FILE
    FILE * fp = fopen(pcm_path, "ab+");
    if (fp) {
        if (false == _wkn.reg.has_audio) {
            fwrite(_wkn.prev_pcm_p, _wkn.prev_pcm_len, 1, fp);
            ESP_LOGI(TAG, ">>> prev len: %d / %d", ftell(fp), _wkn.prev_pcm_len);
        }
        fwrite(buffer, bytes_length, 1, fp);
        ESP_LOGI(TAG, ">>> len: %ld", ftell(fp));
        fclose(fp);
        fp = NULL;
    }
    _wkn.reg.has_audio = true;
    _wkn.timeout_begin = esp_log_timestamp();
#endif  /** _WAKENET_WR_FILE */
    return 0;
}

void gt_wakenet_prerecording_prev_audio_data(int16_t * buffer, int bytes_length)
{
#if _WAKENET_STREAM_DETECT_MODE
    if (NULL == _wkn.wakenet) { return ; }
    if (false == gt_wakenet_is_running()) {
        return ;
    }
    if (_wkn.reg.has_audio) {
        return;
    }
    _wkn.prev_pcm_len = bytes_length;
    memcpy(_wkn.prev_pcm_p, buffer, bytes_length);
#endif
}

int gt_wakenet_ready_to_detect(int16_t * buffer, int bytes_length)
{
#if !_WAKENET_STREAM_DETECT_MODE
    if (NULL == _wkn.wakenet) { return 0; }
    if (false == gt_wakenet_is_running()) {
        return 0;
    }
    if (false == _wkn.reg.has_audio) {
        return 1;
    }
    uint32_t timeout_release = esp_log_timestamp() - _wkn.timeout_begin;
    if (timeout_release < _WAKENET_TIMEOUT_MS) {
        return 2;
    }
    long file_size = 0;
    int ret = 0;
    // FILE * fp = fopen(pcm_path, "rb");
    FILE * fp = fopen(pcm_path, "ab+");
    if (fp) {
        _recording_final_audio_data(fp, buffer, bytes_length);
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        for (int i = 0, blocks = file_size / (_wkn.audio_chunksize >> 1); i < blocks; i++) {
            fread(_wkn.buffer, _wkn.audio_chunksize, 1, fp);
            ret = _wkn.wakenet->detect(_wkn.model_data, _wkn.buffer);
            if (ret > 0) { break; }
        }
        fclose(fp);
        fp = NULL;
        remove(pcm_path);
    }
    ESP_LOGI(TAG, "detect done! %d len: %d", ret, file_size);
    if (ret > 0 && _wkn.wakeup_handler_cb) {
        _wkn.wakeup_handler_cb(_wkn.user_data);
    }
    _wkn.reg.has_audio = false;
#endif
    return 0;
}

void gt_wakenet_resume(void)
{
    if (NULL == _wkn.wakenet) { return ; }
    _wkn.reg.enabled = true;
}

void gt_wakenet_suspend(void)
{
    if (NULL == _wkn.wakenet) { return ; }
    _wkn.reg.enabled = false;
}

bool gt_wakenet_is_running(void)
{
    if (NULL == _wkn.wakenet) { return false; }
    return _wkn.reg.enabled;
}

#endif
/* end ------------------------------------------------------------------*/
