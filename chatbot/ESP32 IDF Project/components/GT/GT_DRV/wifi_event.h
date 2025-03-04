#ifndef WIFI_EVENT_H
#define WIFI_EVENT_H

#include "gt_ui.h"
#include "gt_websocket.h"

void wifi_scan_task(void* wifi_event_obj);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

int gt_wifi_connect_status();

void gt_wifi_task(void* wifi_param);

#endif // !WIFI_EVENT_H