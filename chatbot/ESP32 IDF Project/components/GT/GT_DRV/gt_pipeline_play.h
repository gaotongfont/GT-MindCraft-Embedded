#ifndef GT_PIPELINE_H
#define GT_PIPELINE_H

#ifdef __cplusplus
extern "C"{
#endif


/* include --------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "esp_log.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "http_stream.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "board.h"
#include "esp_audio.h"
/* define ---------------------------------------------------------------*/

typedef struct gt_pipeline {
    audio_pipeline_handle_t pipeline;
    audio_element_handle_t http_stream_reader;
    audio_element_handle_t i2s_stream_writer;
    audio_element_handle_t mp3_decoder;
    audio_event_iface_msg_t* msg;
    audio_event_iface_handle_t evt;
    ringbuf_handle_t ringbuf;
    esp_audio_handle_t player;
    bool isStartListen;
}gt_pipeline_st;

void gt_pipeline_init(gt_pipeline_st* gt_pipeline_obj);

gt_pipeline_st* gt_pipeline_single();

int gt_audio_pipeline_run(gt_pipeline_st* gt_pipeline_obj, const char* uri);

audio_event_iface_msg_t* get_iface_msg(gt_pipeline_st* gt_pipeline_obj);

audio_event_iface_handle_t get_evt(gt_pipeline_st* gt_pipeline_obj);

audio_element_handle_t get_mp3_decoder(gt_pipeline_st* gt_pipeline_obj);

audio_element_handle_t get_i2s_stream_writer(gt_pipeline_st* gt_pipeline_obj);

audio_element_handle_t get_http_stream_reader(gt_pipeline_st* gt_pipeline_obj);

int gt_audio_pipeline_stop(gt_pipeline_st* gt_pipeline_obj);

int gt_audio_pipeline_resume(gt_pipeline_st* gt_pipeline_obj);

int gt_audio_pipeline_reset_items_state(gt_pipeline_st* gt_pipeline_obj);

int gt_audio_event_iface_discard (gt_pipeline_st* gt_pipeline_obj);

void set_startListen(gt_pipeline_st* gt_pipeline_obj, bool value);

bool get_startListen();

#ifdef __cplusplus
}
#endif


#endif // !GT_PIPELINE_H