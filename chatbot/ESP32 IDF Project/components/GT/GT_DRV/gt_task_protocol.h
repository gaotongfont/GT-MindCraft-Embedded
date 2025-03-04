#ifndef GT_TASK_PROTOCOL_H
#define GT_TASK_PROTOCOL_H

#include "wifi_event.h"

extern QueueHandle_t gui_task_queue;

typedef enum{
    WIFI_DISCONNECT_EVENT = 0,
    WIFI_CONNECT_EVENT,
    WIFI_SCAN_EVENT,
    REC_WEB_DATA,
    SCAN_LIST,
    LOAD_SCR,
    LOAD_MAIN_SCR
}EXCU_EVENT_TYPE;

typedef struct{
    EXCU_EVENT_TYPE head_type;
    void* data;
}GT_PROTOCOL;


typedef struct{
    int scr_id;
    int scr_anim_type;
    uint32_t time;
    uint32_t delay;
    bool del_pre_scr;
}LOAD_SCREEN;

#endif //