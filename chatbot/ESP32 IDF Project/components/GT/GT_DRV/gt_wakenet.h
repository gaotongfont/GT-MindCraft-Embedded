/**
 * @file gt_wakenet.h
 * @author Feyoung
 * @brief Use the sd card to load the wake up model
 * @version 0.1
 * @date 2025-02-14
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */
#ifndef _GT_WAKENET_H_
#define _GT_WAKENET_H_

#ifdef __cplusplus
extern "C" {
#endif

/* include --------------------------------------------------------------*/
#include "audio_idf_version.h"
#include "esp_system.h"


/* define ---------------------------------------------------------------*/

#if CONFIG_MODEL_IN_SDCARD && CONFIG_USE_WAKENET && (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0))
    #ifndef GT_USE_WAKENET
        #define GT_USE_WAKENET      1
    #endif
#else
    #ifndef GT_USE_WAKENET
        #define GT_USE_WAKENET      0
    #endif
#endif


/* typedef --------------------------------------------------------------*/
/**
 * @brief The wakeup handler callback
 * @param user_data The user data
 */
typedef int (* gt_wakeup_handler_t)(void * user_data);


/* macros ---------------------------------------------------------------*/



/* class ----------------------------------------------------------------*/



/* global functions / API interface -------------------------------------*/
#if GT_USE_WAKENET
/**
 * @brief audio_board_sdcard_init() must be initialized before
 *      calling this function.
 */
void gt_wakenet_init(void);

/**
 * @brief Set the callback function after the recognition is successful
 *
 * @param handler_cb
 * @param user_data
 */
void gt_wakenet_register_wakeup_handler_cb(gt_wakeup_handler_t handler_cb, void * user_data);

/**
 * @brief After VAD detects the speech, the audio data is recorded to sdcard
 *
 * @param buffer
 * @param bytes_length
 * @return int 0: success, other: fail
 */
int gt_wakenet_recording_audio(int16_t * buffer, int bytes_length);

/**
 * @brief Pre-record the last audio data to the cache
 *
 * @param buffer
 * @param bytes_length
 */
void gt_wakenet_prerecording_prev_audio_data(int16_t * buffer, int bytes_length);

/**
 * @brief The sd card records the last audio and performs wake-up word recognition
 *
 * @param buffer
 * @param bytes_length
 * @return int 0: success, other: fail
 */
int gt_wakenet_ready_to_detect(int16_t * buffer, int bytes_length);

void gt_wakenet_resume(void);
void gt_wakenet_suspend(void);
bool gt_wakenet_is_running(void);

#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!_GT_WAKENET_H_
