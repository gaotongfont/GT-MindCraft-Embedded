#include "gt_audio_event.h"

static const char* TAG = "AUDIO_EVENT";

extern QueueHandle_t audio_event_queue;

static AUDIO_EVENT_STATUS audio_event_status = ALL_EVENT_STOP; 

// void audio_event_task()
// {
//     BaseType_t res;
//     AUDIO_EVENT_STATUS status = ALL_EVENT_STOP;
//     int event = 0;
//     while(1)
//     {
//         res = xQueueReceive(audio_event_queue, &event, portMAX_DELAY);
//         if(res == pdPASS)
//         {
//             switch (audio_event_status)
//             {
//                 case ALL_EVENT_STOP:
//                 ESP_LOGE(TAG, "ALL_EVENT_STOP");
//                 audio_event_status = ALL_EVENT_GENERATING;
//                 break;
                
//                 case ALL_EVENT_GENERATING:
//                 ESP_LOGE(TAG, "ALL_EVENT_GENERATING");
//                 audio_event_status = ALL_EVENT_GENERATING;
//                 gt_websocket_client_stop_receive_data();
//                 break;

//                 case ALL_EVENT_RUNNING:
//                 ESP_LOGE(TAG, "ALL_EVENT_RUNNING");
//                 gt_websocket_client_stop_receive_data();
//                 gt_audio_pipeline_stop(gt_pipeline_single());
//                 audio_event_status = ALL_EVENT_STOP;
//                 break;
            
//                 case SUBTITLES_EVENT_STOP_ONLY:
//                 ESP_LOGE(TAG, "SUBTITLES_EVENT_STOP_ONLY");
//                 gt_audio_pipeline_stop(gt_pipeline_single());
//                 audio_event_status = ALL_EVENT_STOP;
//                 break;
            
//                 default:
//                 break;
//             }
//         }
//     }
// }

void audio_event_task()
{
    EventBits_t pre_bits = 0, cur_bits = 0;
    uint8_t pre_type = 0, cur_type = 0, execute_type = 0;
    while(1)
    {
        xQueueReceive(audio_event_queue, &cur_type, portMAX_DELAY);
        execute_type = cur_type | pre_type; 
        printf("execute_type = %d  cur_type = %d  pre_type = %d\r\n", execute_type, cur_type, pre_type);
        switch(execute_type)
        {
            case AUDIO_STOP:
            ESP_LOGI(TAG, "AUDIO_STOP");
            pre_type = 0;
            break;

            case AUDIO_GENERATING | AUDIO_RUNNING:
            ESP_LOGI(TAG, "AUDIO_RUNNING");
            xSemaphoreTake(audio_event_mutex, portMAX_DELAY);
            if(get_chatbot_audio_uri() != NULL)
            {
                gt_audio_pipeline_run(gt_pipeline_single(), get_chatbot_audio_uri());
            }
            pre_type = AUDIO_RUNNING;
            xSemaphoreGive(audio_event_mutex);
            break;

            case AUDIO_GENERATING:
            ESP_LOGI(TAG, "AUDIO_GENERATING");
            gt_websocket_client_create_task();
            gt_audio_storage_start(GT_RECORDING_STATE_UPLOAD_SERVER);
            pre_type = AUDIO_GENERATING;
            break;

            case AUDIO_RUNNING | AUDIO_STOP:
            ESP_LOGI(TAG, "AUDIO_RUNNING | AUDIO_STOP");
            gt_audio_pipeline_stop(gt_pipeline_single());
            gt_websocket_client_stop_receive_data();
            pre_type = 0;
            break;

            case AUDIO_GENERATING | AUDIO_NATURAL_STOP:
            ESP_LOGI(TAG, "AUDIO_GENERATING | AUDIO_GENERATE_STOP");
            pre_type = 0;
            break;

            case AUDIO_GENERATING | AUDIO_STOP:
            ESP_LOGI(TAG, "AUDIO_GENERATING | AUDIO_STOP");
            gt_websocket_client_stop_receive_data();
            pre_type = 0;
            break;

            case AUDIO_GENERATING | AUDIO_GENERATE_ERROR:
            ESP_LOGI(TAG, "AUDIO_GENERATING | AUDIO_GENERATE_ERROR");
            pre_type = 0;
            break;

            default:
            break;
        }
        
    }    
}


void set_audio_event_status(AUDIO_EVENT_STATUS value)
{
    audio_event_status = value;
}

AUDIO_EVENT_STATUS get_audio_event_status()
{
    return audio_event_status;
}