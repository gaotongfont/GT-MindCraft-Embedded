#ifndef _WIFI_H_
#define _WIFI_H_


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include <netdb.h>
#include "esp_log.h"
#include "audio_mem.h"
#include "gt_nvs_store.h"
// #include "gt_ui.h"


#define DEFAULT_SCAN_LIST_SIZE 8

typedef enum {
    WIFI_NO_CONNECTED = 0,
    WIFI_SIGNAL_1,
    WIFI_SIGNAL_2,
    WIFI_SIGNAL_3,
    WIFI_SIGNAL_4,
}gt_wifi_icon_status_et;
typedef enum{
    GT_WIFI_UNEXPECTED = -1,
    GT_WIFI_DISCONNECTED,
    GT_WIFI_CONNECTED,
    GT_WIFI_SCAN_FINISH,
    GT_WIFI_RUNNING
}GT_WIFI_STATUS;

typedef enum{
    GT_KEY_BOARD = 0,
    GT_WIFI_LIST
}SEND_UI_OBJ;

typedef struct {
    char *name;
    char *password;
    bool isConnected;
 }WIFI_ITEM_INFO; //wifi信息，从外部获取数据
 
typedef struct{
    int scan_count;
    uint8_t wifi_tip;
    gt_wifi_icon_status_et wifi_icon;
    WIFI_ITEM_INFO* wifi_items;
}WIFI_ITEM_INFO_S; //WIFI_INFO_S;

typedef struct{
    SEND_UI_OBJ send_obj; //key_board    wifi_list
    uint8_t list_item_index;
    char* user_name;
    char* password;
}GT_WIFI_PROTOCOL; //wifi队列协议

typedef struct _network_connet_info_t
{
    uint8_t         connet_state;    /* 网络连接状态 */
    char            ip_buf[100];     /* 分配的IP地址 */
    char            mac_buf[100];    /* 子网掩码 */
    void (*fun)(uint8_t x);          /* 函数指针 */
}network_connet_info;



extern uint16_t ap_count;
extern bool is_auto_connected_end;

wifi_config_t get_current_wifi_config();
int8_t get_current_rssi_level();

void wifi_event_init();
void wifi_init(void);
int wifi_scan(WIFI_ITEM_INFO* scan_wifi_list);
int wifi_sta_connect(char *name, char *password);

void set_current_wifi_isConnected();
void reset_wifi_status(WIFI_ITEM_INFO* scan_wifi_list, int count);
bool check_cur_wifi_is_in_wifi_list(char *name, char *password);

void set_wifilist_item_index(uint8_t value);


/**
 * @brief store wifi name and wifi password
 * 
 * @return int 
 */
uint8_t store_wifi_pwd(char* wifi_name, char* wifi_pwd);

/**
 * @brief Get the wifi name and wifi password 
 * 
 * @param wifi_name 
 * @param wifi_pwd 
 * @return uint8_t 
 */
uint8_t get_wifi_name_pwd(char* wifi_name, char* wifi_pwd);

uint8_t erase_wifi_name_pwd();

#endif // !_WIFI_H_
