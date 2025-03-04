#ifndef GT_AUDIO_STORAGE
#define GT_AUDIO_STORAGE


#ifdef __cplusplus
extern "C"{
#endif


/* include --------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "esp_bit_defs.h"
#include "gt_websocket.h"
#include "gt_protocol_config.h"

/* define ---------------------------------------------------------------*/
typedef enum {
    GT_RECORDING_STATE_NONE = 0,
    GT_RECORDING_STATE_WAKEUP,
    GT_RECORDING_STATE_UPLOAD_SERVER,
}gt_recording_state_em;


void gt_audio_storage_init(void);
void gt_audio_storage_deinit(void);
void gt_audio_storage_start(gt_recording_state_em state);
void gt_audio_storage_stop(void);


#ifdef __cplusplus
}
#endif


#endif // !GT_AUDIO_STORAGE