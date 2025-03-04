#include "wifi_event.h"

static const char *TAG = "WIFI_EVENT";
QueueHandle_t wifi_task_queue = NULL;

typedef struct{
    GT_WIFI_STATUS wifi_status;
    WIFI_ITEM_INFO_S* wifi_items_c;
}GT_WIFI_DATA; //wifi 事件   线程内部使用

static GT_WIFI_DATA* wifi_data = NULL;


static GT_PROTOCOL* gt_connect_wifi_in_list(uint8_t selected_idx, WIFI_ITEM_INFO* scan_wifi_list)
{
    if (scan_wifi_list[selected_idx].name == NULL) 
    {
        ESP_LOGE(TAG, "Invalid SSID in scan_wifi_list");
        return NULL;
    }
    ESP_LOGI(TAG, "---------------scan_wifi_list[selected_idx].name: %s\n", scan_wifi_list[selected_idx].name);

    wifi_ap_record_t ap_info;
    esp_err_t ret;
    ret = esp_wifi_sta_get_ap_info(&ap_info);
    
    GT_PROTOCOL* gt_pro = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
    gt_pro->head_type = LOAD_SCR;
    LOAD_SCREEN* load_src = (LOAD_SCREEN*)audio_malloc(sizeof(LOAD_SCREEN));
    load_src->scr_anim_type = GT_SCR_ANIM_TYPE_NONE;
    load_src->time = 500;
    load_src->delay = 0;
    load_src->del_pre_scr = true;
    load_src->scr_id = GT_ID_KEYBOARD;
    gt_pro->data = load_src;
    return gt_pro;

}

static GT_WIFI_STATUS gt_poweron_connect_wifi(GT_WIFI_DATA* wifi_data_2)
{
    wifi_config_t last_wifi_config = get_current_wifi_config();
    ESP_LOGI(TAG,"ifi_config.sta.ssid = %s\n", (const char *)last_wifi_config.sta.ssid);
    ESP_LOGI(TAG,"ifi_config.sta.password = %s\n", (const char *)last_wifi_config.sta.password);
    if (strcmp((char *)last_wifi_config.sta.ssid, "") != 0 && strcmp((char *)last_wifi_config.sta.password, "")!= 0) 
    {
        wifi_data_2->wifi_status = wifi_sta_connect((char *)last_wifi_config.sta.ssid, (char *)last_wifi_config.sta.password);
        gt_scr_id_t screen_id = gt_scr_stack_get_current_id();
        if (screen_id == GT_ID_MAIN_INTERFACE)
        {
            update_wifi_icon(); //wifi图标
        }
    }
    return  wifi_data_2->wifi_status;
}

void wifi_scan_task(void* wifi_data)
{
    ESP_LOGE(TAG, "wifi scan task");
    wifi_ap_record_t ap_info;
    GT_WIFI_DATA* wifi_data_ptr = (GT_WIFI_DATA*)wifi_data;
    wifi_data_ptr->wifi_items_c->scan_count = wifi_scan(wifi_data_ptr->wifi_items_c->wifi_items);
    esp_err_t ret = esp_wifi_sta_get_ap_info(&ap_info);
    ESP_LOGE(TAG, "wifi_scan_task ret = %d", ret);
    reset_wifi_status(wifi_data_ptr->wifi_items_c->wifi_items, wifi_data_ptr->wifi_items_c->scan_count);
    GT_PROTOCOL* wifi_temp = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
    wifi_temp->head_type =  SCAN_LIST;
    wifi_temp->data = wifi_data_ptr->wifi_items_c;
    
    if (ret == ESP_OK) 
    {
        // primary：AP的主要信道号，1.primary=0:未连接到 AP; 2.primary!=0:已连接到 AP，信道号有效。通过检查 primary 字段判断是否已连接
        if (ap_info.primary != 0) 
        {
            wifi_config_t last_wifi_config = get_current_wifi_config();
            for(int i = 0; i < wifi_data_ptr->wifi_items_c->scan_count; i++)
            {
               if(strcmp(wifi_data_ptr->wifi_items_c->wifi_items[i].name, (char *)last_wifi_config.sta.ssid) == 0)
               {
                    wifi_data_ptr->wifi_items_c->wifi_tip = 0x01;
                    wifi_data_ptr->wifi_items_c->wifi_icon = WIFI_SIGNAL_4;
                    wifi_data_ptr->wifi_items_c->wifi_items[i].isConnected = true;
                    break;
               }
            }
        }
    }   
    xQueueSend(gui_task_queue, &wifi_temp, portMAX_DELAY);
    wifi_data_ptr->wifi_status = GT_WIFI_SCAN_FINISH; 
    vTaskDelete(NULL);
}


void gt_wifi_task(void* wifi_param)
{
    GT_PROTOCOL* wifi_protocol = NULL;
    GT_WIFI_PROTOCOL* get_wifi_data = NULL;
    wifi_task_queue = xQueueCreate(4, sizeof(GT_PROTOCOL*));
    
    // LOAD_SCREEN* load_src = (LOAD_SCREEN*)audio_malloc(sizeof(LOAD_SCREEN));
    // memset(load_src, 0, sizeof(LOAD_SCREEN));

    wifi_data = (GT_WIFI_DATA*)audio_malloc(sizeof(GT_WIFI_DATA));
    memset(wifi_data, 0, sizeof(GT_WIFI_DATA));
    
    wifi_data->wifi_items_c = (WIFI_ITEM_INFO_S*)audio_malloc(sizeof(WIFI_ITEM_INFO_S));
    memset(wifi_data->wifi_items_c, 0, sizeof(WIFI_ITEM_INFO_S));

    wifi_data->wifi_items_c->wifi_items = (WIFI_ITEM_INFO*)audio_malloc(DEFAULT_SCAN_LIST_SIZE * sizeof(WIFI_ITEM_INFO));
    memset(wifi_data->wifi_items_c->wifi_items, 0, DEFAULT_SCAN_LIST_SIZE * sizeof(WIFI_ITEM_INFO));
    
    wifi_data->wifi_status = gt_poweron_connect_wifi(wifi_data);//连接上一次的wifi  自动连接上次wifi的功能要改，从flash拿账号和密码
    
    if(wifi_data->wifi_status == GT_WIFI_CONNECTED)
    {
        gt_websocket_client_start();
    }

    while(1)
    {
        int res = xQueueReceive(wifi_task_queue, &wifi_protocol, portMAX_DELAY);
        
        if(res == pdPASS)
        {
            ESP_LOGI(TAG, "wifi_protocol->head_type = %d", wifi_protocol->head_type);
            switch(wifi_protocol->head_type)
            {
            case WIFI_DISCONNECT_EVENT:
            if(esp_wifi_disconnect() == ESP_OK)
            {
                wifi_data->wifi_status = GT_WIFI_DISCONNECTED;
                reset_wifi_status(wifi_data->wifi_items_c->wifi_items, wifi_data->wifi_items_c->scan_count);//重置wifi连接状态，更改wifi状态label
            }
            break;
            case WIFI_CONNECT_EVENT:
            get_wifi_data = (GT_WIFI_PROTOCOL*)(wifi_protocol->data);
            ESP_LOGE(TAG, "get_wifi_data->send_obj = %d\r\n",get_wifi_data->send_obj);
            if(wifi_data->wifi_status != GT_WIFI_RUNNING)
            {
                if(get_wifi_data->send_obj == GT_KEY_BOARD)
                {
                    wifi_data->wifi_status = GT_WIFI_RUNNING;
                    reset_wifi_status(wifi_data->wifi_items_c->wifi_items, wifi_data->wifi_items_c->scan_count);
                    wifi_data->wifi_items_c->wifi_items[get_wifi_data->list_item_index].password = (char*)audio_malloc(strlen(get_wifi_data->password)+1);
                    memset(wifi_data->wifi_items_c->wifi_items[get_wifi_data->list_item_index].password, 0, strlen(get_wifi_data->password) + 1);
                    strcpy(wifi_data->wifi_items_c->wifi_items[get_wifi_data->list_item_index].password, get_wifi_data->password);
                    wifi_data->wifi_status = wifi_sta_connect(wifi_data->wifi_items_c->wifi_items[get_wifi_data->list_item_index].name, get_wifi_data->password);
                    
                    ESP_LOGE(TAG, "wifi_data->connect_status = %d\r\n",wifi_data->wifi_status);
                    if( wifi_data->wifi_status == GT_WIFI_CONNECTED)
                    {
                        ESP_LOGI(TAG, "wifi connect successfull ===========");
                        wifi_data->wifi_items_c->wifi_items[get_wifi_data->list_item_index].isConnected = true;
                        wifi_data->wifi_items_c->wifi_tip = 0x01;
                        wifi_data->wifi_items_c->wifi_icon = WIFI_SIGNAL_4;
                        if(gt_websocket_client_state() == false)
                        {
                            gt_websocket_client_start();
                        }
                    }
                    else
                    {
                        ESP_LOGI(TAG, "wifi connect fail ===========");
                        wifi_data->wifi_items_c->wifi_tip = 0x04;
                        wifi_data->wifi_items_c->wifi_icon = WIFI_NO_CONNECTED;
                    }
                    GT_PROTOCOL* gt_pro_temp = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
                    gt_pro_temp->head_type =  SCAN_LIST;
                    gt_pro_temp->data = wifi_data->wifi_items_c;
                    xQueueSend(gui_task_queue, &gt_pro_temp, portMAX_DELAY);
                }
                else if(get_wifi_data->send_obj == GT_WIFI_LIST) //wifi list screen send signal
                {       
                    GT_PROTOCOL* gt_pro_temp = gt_connect_wifi_in_list(get_wifi_data->list_item_index, wifi_data->wifi_items_c->wifi_items);
                    if(gt_pro_temp != NULL)
                    {
                        xQueueSend(gui_task_queue, &gt_pro_temp, portMAX_DELAY);
                    }
                }
            }
            audio_free(get_wifi_data);
            audio_free(wifi_protocol);
            get_wifi_data = NULL;
            wifi_protocol = NULL;
            break;
            case WIFI_SCAN_EVENT:
            if(wifi_data->wifi_status != GT_WIFI_RUNNING)
            {
                wifi_data->wifi_status = GT_WIFI_RUNNING;
                audio_free(wifi_protocol);
                wifi_protocol = NULL;
                xTaskCreate(wifi_scan_task, "wifi_scan_task", 5*1024, wifi_data, 6, NULL); //防止从夫点击
            }
            break;
            default:
            break;
            }
        }
        vTaskDelay(100);
    }

}

int gt_wifi_connect_status()
{
    return wifi_data->wifi_status;
}




