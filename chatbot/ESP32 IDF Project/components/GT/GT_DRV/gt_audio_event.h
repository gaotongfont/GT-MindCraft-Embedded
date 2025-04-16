#ifndef GT_AUDIO_EVENT
#define GT_AUDIO_EVENT

#include "gt_pipeline_play.h"
#include "gt_websocket.h"

#define AUDIO_STOP (1<<0)
#define AUDIO_RUNNING (1<<1)
#define AUDIO_GENERATING (1<<2)
#define AUDIO_NATURAL_STOP (1<<3)
#define AUDIO_GENERATE_ERROR (1<<4)

typedef enum{
    ALL_EVENT_STOP,
    ALL_EVENT_GENERATING,
    ALL_EVENT_RUNNING,
    SUBTITLES_EVENT_STOP_ONLY
}AUDIO_EVENT_STATUS;




void audio_event_task();

void set_audio_event_status(AUDIO_EVENT_STATUS value);

AUDIO_EVENT_STATUS get_audio_event_status();

#endif 