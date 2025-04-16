#include "gt_gui_task.h"

extern QueueHandle_t mYxQueue2;
extern QueueHandle_t mYxQueue3;

QueueHandle_t gui_task_queue = NULL;
SemaphoreHandle_t scr_id_mutex = NULL;
static const char *TAG = "gui_task";

#if (WEBSOCKET_HTTP_SWITCH == 0)//USE_HTTP_STREAM
void gt_gui_task(void *pvParameters)
{
    ESP_LOGI(TAG, "--------------------------gt_gui_task\n");
    int received_msg = 0;
    ReceivedAnswerData* receive_evt = NULL;
    while(1){
        if (mYxQueue3 != NULL && xQueueReceive(mYxQueue3, &receive_evt, 1) == pdPASS) {
            if (receive_evt != NULL/*received_msg >= 0 && received_msg < 6*//*ESP_OK == received_msg*/) {
                ESP_LOGI(TAG,"<<<<<---------------receive_evt->is_first_response: %d\n", receive_evt->is_first_response);
                if (receive_evt->is_first_response == true) 
                {
                    gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
                    ESP_LOGI(TAG,">>---------------screen_id: %d\n",screen_id);
                    if (screen_id != GT_ID_SCREEN_SUBTITLE)
                    {
                        gt_disp_stack_load_scr_anim(GT_ID_SCREEN_SUBTITLE, GT_SCR_ANIM_TYPE_NONE, 50, 0, true);
                    }

                }
                xQueueSend(audio_uri_queue, &receive_evt, portMAX_DELAY);
                receive_evt = NULL;
            } else {
                //切换语音识别失败时的ui
                gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
                ESP_LOGI(TAG,">>1---------------screen_id: %d\n",screen_id);
                if ( screen_id == GT_ID_SCREEN_HOME )
                {
                    identification_failed_ui();
                } else if ( screen_id == GT_ID_SCREEN_SUBTITLE ){
                    identifying_failed_ui_in_subtitle();
                }
            }
        }
        if (xQueueReceive(mYxQueue2, &received_msg, 1) == pdPASS) {
            ESP_LOGI(TAG, "mYxQueue2-------------------received_msg = %d\n", received_msg);
            if (ESP_FAIL == received_msg) {
                //切换语音识别失败时的ui
                gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
                ESP_LOGI(TAG,">>2---------------screen_id: %d\n",screen_id);
                if ( screen_id == GT_ID_SCREEN_HOME )
                {
                    identification_failed_ui();
                } else if ( screen_id == GT_ID_SCREEN_SUBTITLE ){
                    identifying_failed_ui_in_subtitle();
                }
            }
        }
        gt_task_handler();
        // vTaskDelay(1);
    }
}
#elif (WEBSOCKET_HTTP_SWITCH == 1) //!USE_HTTP_STREAM
void gt_gui_task(void *pvParameters)
{
    ESP_LOGI(TAG, "--------------------------gt_gui_task\n");
    int received_msg;
    while(1){
        if (xQueueReceive(mYxQueue2, &received_msg, portMAX_DELAY) == pdPASS) {
            ESP_LOGI(TAG, "mYxQueue2-------------------received_msg = %d\n", received_msg);
            if (ESP_OK == received_msg) {
                //启动音频播放器
                gt_audio_player_start(cb_data.answer->tts_audio);
                //跳转到字幕界面
                gt_disp_stack_load_scr_anim(GT_ID_SCREEN_SUBTITLE, GT_SCR_ANIM_TYPE_NONE, 50, 0, true);
            } else {
                //切换语音识别失败时的ui
                gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
                ESP_LOGI(TAG,">>3---------------screen_id: %d\n",screen_id);
                if ( screen_id == GT_ID_SCREEN_HOME )
                {
                    identification_failed_ui();
                } else if ( screen_id == GT_ID_SCREEN_SUBTITLE ){
                    identifying_failed_ui_in_subtitle();
                }
            }
        }
        gt_task_handler();
        // vTaskDelay(1);
    }
}

#elif (WEBSOCKET_HTTP_SWITCH == 2)
void gt_gui_task(void *pvParameters)
{
    // ESP_LOGI(TAG, "--------------------------gt_gui_task\n");
    int received_msg = 0, queue_res = 0;
    GT_PROTOCOL* gui_info = NULL;
    ReceivedAnswerData* receive_evt = NULL;
    bool is_first_response = true;
    gt_wifi_icon_status_et wifi_icon_status = WIFI_NO_CONNECTED;
    WIFI_ITEM_INFO_S* wifi_items_info_temp = NULL;
    scr_id_mutex = xSemaphoreCreateMutex();
    gui_task_queue = xQueueCreate(4, sizeof(GT_PROTOCOL*));
    while(1)
    {
        queue_res = xQueueReceive(gui_task_queue, &gui_info, 1);
       
        if(queue_res == pdPASS)
        {
            ESP_LOGI(TAG, "gui_info->head_type = %d", gui_info->head_type);
            switch(gui_info->head_type)
            {
            case REC_WEB_DATA:
            ESP_LOGI(TAG, "REC_WEB_DATA");
            receive_evt = (ReceivedAnswerData*)(gui_info->data);
            if (receive_evt->socket_status != 3001 && receive_evt->socket_status != 4006)
            {
                gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
                ESP_LOGE(TAG,">>---------------screen_id: %d\n",screen_id);
                if(receive_evt->is_web_first_response == true)
                {
                    receive_evt->is_web_first_response = false;
                    if (screen_id != GT_ID_SCREEN_SUBTITLE)
                    {
                        ESP_LOGE(TAG,">>---------------screen_id 2: %d\n",screen_id);
                        gt_disp_stack_load_scr_anim(GT_ID_SCREEN_SUBTITLE, GT_SCR_ANIM_TYPE_NONE, 50, 0, true);
                    }
                }
                screen_id = gt_scr_stack_get_current_id();
                if(screen_id == GT_ID_SCREEN_SUBTITLE)
                {
                    ESP_LOGE(TAG,">>---------------screen_id 3: %d\n",screen_id);
                    xSemaphoreTake(audio_event_mutex, portMAX_DELAY);
                    update_subtitles(receive_evt);
                    xSemaphoreGive(audio_event_mutex);
                }
                // audio_free(receive_evt->socket_type);
                // receive_evt->socket_type = NULL;
   
                // audio_free(receive_evt->emotion_value);
                // receive_evt->emotion_value = NULL;
                // audio_free(receive_evt->llm_response);
                // receive_evt->llm_response = NULL;
            } 
            else //切换语音识别失败时的ui
            {
                gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
                ESP_LOGI(TAG,">>1---------------screen_id: %d\n",screen_id);
                if ( screen_id == GT_ID_SCREEN_HOME )
                {
                    identification_failed_ui();
                } 
                else if ( screen_id == GT_ID_SCREEN_SUBTITLE )
                {
                    identifying_failed_ui_in_subtitle();
                }
            }
            break;
            
            case SCAN_LIST:  //show wifi scan list       // set_wifi_status_icon(data->wifi_icon);
            WIFI_ITEM_INFO_S* data = (WIFI_ITEM_INFO_S*)(gui_info->data);
            ESP_LOGI(TAG, "SCAN LIST");
            xSemaphoreTake(scr_id_mutex, portMAX_DELAY);
            if( gt_scr_stack_get_current_id() == GT_ID_WIFI_LIST)
            {
                
                if(data->wifi_items == NULL)
                {
                    printf("data->wifi_items == NULL");
                    redraw_wifi_list(wifi_items_info_temp->wifi_items, wifi_items_info_temp->scan_count);
                    audio_free(data);
                    data = NULL;
                }
                else
                {
                    close_wifi_scan_dialog();
                    change_wifi_connect_tip(data->wifi_tip);
                    redraw_wifi_list(data->wifi_items, data->scan_count);
                    wifi_items_info_temp = data;
                }
                
            }
            xSemaphoreGive(scr_id_mutex);
            audio_free(gui_info);
            gui_info = NULL;
            break;

            case LOAD_SCR:
            LOAD_SCREEN* load_data = (LOAD_SCREEN*)(gui_info->data);
            gt_disp_stack_load_scr_anim(load_data->scr_id, load_data->scr_anim_type, load_data->time, load_data->delay, load_data->del_pre_scr);
            audio_free(load_data);
            audio_free(gui_info);
            gui_info = NULL;
            load_data = NULL;
            break;

            case LOAD_MAIN_SCR:
            WIFI_ITEM_INFO_S* data_temp = (WIFI_ITEM_INFO_S*)(gui_info->data);
            xSemaphoreTake(scr_id_mutex, portMAX_DELAY);
            if( gt_scr_stack_get_current_id() == GT_ID_MAIN_INTERFACE)
            {
                if(data_temp == NULL)
                {
                    if(wifi_items_info_temp != NULL)
                    {
                        set_wifi_status_icon(wifi_items_info_temp->wifi_icon);
                    }
                    
                }
                else
                {
                    wifi_items_info_temp = data_temp;
                    set_wifi_status_icon(wifi_items_info_temp->wifi_icon);
                }
            }
            else
            {
                if(data_temp != NULL)
                {
                    wifi_items_info_temp = data_temp;
                }
            }
            audio_free(gui_info);
            gui_info = NULL;
            xSemaphoreGive(scr_id_mutex);
            break;

            case SET_AUDIO_VOL:
            gt_set_vol(*((uint16_t*)(gui_info->data)));
            audio_free(gui_info);
            gui_info = NULL;

            break;
            case WEB_SERVER_DIALOG:
            serve_disconnect_dialog();
            audio_free(gui_info);
            gui_info = NULL;
            break;
            case WIFI_SCAN_DIALOG:
            break;
            default:
            break;
            }
    
        }
        gt_task_handler();
   
    }
    ESP_LOGE(TAG, "gt_gui_task finish");
}
#endif //WEBSOCKET_HTTP_SWITCH