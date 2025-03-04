#include "gt_ui.h"
#include "gt_font_config.h"

static const char *TAG = "SCREEN_WIFI_LIST";


/** wifi_list */
gt_obj_st * wifi_list = NULL;
static gt_obj_st * listv1 = NULL;
static gt_obj_st * lab1 = NULL;
static gt_obj_st * lab2 = NULL;
static gt_obj_st * lab3 = NULL;

uint8_t selected_idx = 0;
static void set_item_by_wifi_list(WIFI_ITEM_INFO*  scan_wifi_list, int ap_count);

void redraw_wifi_list(WIFI_ITEM_INFO* scan_wifi_list, int ap_count)
{
    set_item_by_wifi_list(scan_wifi_list, ap_count);
}

void change_wifi_connect_tip(uint8_t flag) 
{
    const char *wifi_connect_tip[3] = {"连接中", "已连接", "未连接"};
    ESP_LOGI(TAG,"--------flag = 0x%x\n", flag);
    switch (flag)
    {
    case 0x04:
    gt_obj_set_visible(lab3, true);
    gt_label_set_text(lab3, wifi_connect_tip[0]);
	gt_label_set_font_color(lab3, gt_color_hex(0xFF7859));
    ESP_LOGI(TAG,"--------wifi_connect_tip[0] = %s\n", wifi_connect_tip[0]);
    break;
    case 0x01:
    gt_label_set_text(lab3, wifi_connect_tip[1]);
    gt_label_set_font_color(lab3, gt_color_hex(0x94A5B3));
    ESP_LOGI(TAG,"--------wifi_connect_tip[1] = %s\n", wifi_connect_tip[1]);
    break;
    case 0x00:
    gt_label_set_text(lab3, wifi_connect_tip[2]);
    gt_label_set_font_color(lab3, gt_color_hex(0x94A5B3));
    ESP_LOGI(TAG,"--------wifi_connect_tip[2] = %s\n", wifi_connect_tip[2]);
    break;
    default:
        break;
    }
    gt_disp_invalid_area(lab3);
    ESP_LOGI(TAG, "change_wifi_connect_tip");
}


static void screen_home_0_cb(gt_event_st * e) {
    xSemaphoreGive(scr_id_sem);
	gt_disp_stack_go_back(1);
    selected_idx = 0;
    xSemaphoreTake(scr_id_sem, portMAX_DELAY);
}

static void listv1_0_cb(gt_event_st * e) {
    selected_idx = gt_listview_get_idx_selected(listv1);
    if (selected_idx == 0xFF)
    {
        return;
    }

    ESP_LOGI(TAG, "---------------selected_idx: %u\n", selected_idx);
    
    GT_PROTOCOL* gt_pro = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
	gt_pro->head_type = WIFI_CONNECT_EVENT;

    GT_WIFI_PROTOCOL* wifi_data = (GT_WIFI_PROTOCOL*)audio_malloc(sizeof(GT_WIFI_PROTOCOL));
    memset(wifi_data, 0, sizeof(GT_WIFI_PROTOCOL));
    wifi_data->send_obj = GT_WIFI_LIST;
    wifi_data->list_item_index = selected_idx;
    gt_pro->data = wifi_data;
    
    xQueueSend(wifi_task_queue, &gt_pro, portMAX_DELAY);
}

typedef enum wifi_list_item_idx_type_e {
    WIFI_LIST_ITEM_IDX_STRING = 0,  //string
    WIFI_LIST_ITEM_IDX_ICON_TYPE,
    /**  --- end ---  **/
    BAIDU_NET_LIST_ITEM_IDX_TOTAL,
} wifi_list_item_idx_type_et;

static void set_item_by_wifi_list (WIFI_ITEM_INFO*  scan_wifi_list, int ap_count)
{
    gt_listview_clear_all_items(listv1);
#if 0
	gt_listview_set_scale(listv1, 20, 80);
	gt_listview_set_scale_triple(listv1, 20, 60, 20);
	gt_listview_set_icon_to_right(listv1, 1);
	gt_listview_set_next_row_item_count(listv1, 1);
	// gt_listview_add_item_icon(listv1, "f:img_Connected_220x278.png", "  很长的名长...");
	// // gt_listview_set_next_row_item_count(listv1, 1);
	// gt_listview_add_item_icon(listv1, "f:img_Lock_220x278.png", "  WiFi3");
	// gt_listview_add_item_icon(listv1, "f:img_Lock_220x278.png", "  WiFi4");
	// gt_listview_add_item_icon(listv1, "f:img_Lock_220x278.png", "  WiFi5");
	// gt_listview_add_item_icon(listv1, "f:img_Lock_220x278.png", "  WiFi6");
	// gt_listview_add_item_icon(listv1, "f:img_Lock_220x278.png", "  WiFi7");
    for (size_t i = 0; i < ap_count; i++)
    {
        gt_listview_add_item_icon(listv1, scan_wifi_list[i].isConnected ? "f:img_Connected_220x278.png": "f:img_Lock_220x278.png", scan_wifi_list[i].name);
    }
#else
    if(scan_wifi_list != NULL)
    {
        for (size_t i = 0; i < ap_count; i++)
        {
            gt_listview_custom_item_st itm = {0};
            itm.item_idx = i;
            uint16_t len = strlen(scan_wifi_list[i].name);
            if (len > 0)
            {
                itm.element_idx = WIFI_LIST_ITEM_IDX_STRING;
                itm.type = GT_LISTVIEW_ELEMENT_TYPE_LABEL;
                itm.area.x = 6;
                itm.area.y = 0;//(52 - 16)>>1;
                itm.area.w = 220 * 0.8;
                itm.area.h = 52;
                itm.src = scan_wifi_list[i].name;
                itm.src_len = len;
                gt_listview_custom_item_set_element(listv1, &itm);
            }
            if (scan_wifi_list[i].isConnected)
            {
                itm.element_idx = WIFI_LIST_ITEM_IDX_ICON_TYPE;
                itm.type = GT_LISTVIEW_ELEMENT_TYPE_IMG;
                itm.area.x = 186;
                itm.area.y = (52 - 14)>>1;
                itm.area.w = 19;
                itm.area.h = 14;
                itm.src = "f:img_Connected_220x278.png";
                gt_listview_custom_item_set_element(listv1, &itm);
            }
            else 
            {
                itm.element_idx = WIFI_LIST_ITEM_IDX_ICON_TYPE;
                itm.type = GT_LISTVIEW_ELEMENT_TYPE_IMG;
                itm.area.x = 186;
                itm.area.y = (52 - 15)>>1;
                itm.area.w = 13;
                itm.area.h = 15;
                itm.src = "f:img_Lock_220x278.png";
                gt_listview_custom_item_set_element(listv1, &itm);
            }
        }
    }
    gt_disp_invalid_area(listv1);
#endif
}


gt_obj_st * gt_init_wifi_list(void)
{
	wifi_list = gt_obj_create(NULL);
	gt_obj_add_event_cb(wifi_list, screen_home_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(wifi_list, gt_color_hex(0x000000));


	/** listv1 */
	listv1 = gt_listview_create(wifi_list);
	gt_obj_set_pos(listv1, 8, 41);
	gt_obj_set_size(listv1, 220, 278);
	gt_listview_set_font_color(listv1, gt_color_hex(0xffffff));
	gt_listview_set_font_family(listv1, gray_black_16);
	gt_listview_set_font_cjk(listv1, 0);
	gt_listview_set_font_align(listv1, GT_ALIGN_LEFT_MID);
	gt_listview_set_item_height(listv1, 52);
	gt_listview_set_border_color(listv1,gt_color_hex(0xc7c7c7));
	gt_listview_set_border_width(listv1, 0);
	gt_listview_set_septal_line(listv1, 0);
	gt_listview_set_highlight_mode(listv1, false);
	gt_listview_set_bg_color(listv1, gt_color_hex(0x000000));
	gt_listview_set_item_bg_color(listv1, gt_color_hex(0x181B22));
	gt_listview_show_item_bg(listv1, true);
	gt_listview_set_item_reduce(listv1, 0);
	gt_listview_set_item_radius(listv1, 8);
    set_item_by_wifi_list(NULL, 0);
	gt_obj_add_event_cb(listv1, listv1_0_cb, GT_EVENT_TYPE_INPUT_RELEASED, NULL);


	/** lab1 */
	lab1 = gt_label_create(wifi_list);
	gt_obj_set_pos(lab1, 10, 6);
	gt_obj_set_size(lab1, 55, 31);
	gt_label_set_font_color(lab1, gt_color_hex(0xc0c0c0));
	gt_label_set_font_family(lab1, gray_black_20);
	gt_label_set_font_cjk(lab1, 0);
	gt_label_set_font_align(lab1, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab1, "WIFi");



	/** lab2 */
	lab2 = gt_label_create(wifi_list);
	gt_obj_set_pos(lab2, 51, 7);
	gt_obj_set_size(lab2, 126, 29);
	gt_label_set_font_color(lab2, gt_color_hex(0x94A5B3));
	gt_label_set_font_family(lab2, gray_black_16);
	gt_label_set_font_cjk(lab2, 0);
	gt_label_set_font_align(lab2, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab2, "仅支持2.4G");


	/** lab3 */
	lab3 = gt_label_create(wifi_list);
	gt_obj_set_pos(lab3, 170, 7);
	gt_obj_set_size(lab3, 126, 29);
	gt_label_set_font_color(lab3, gt_color_hex(0x94A5B3));
	gt_label_set_font_family(lab3, gray_black_16);
	gt_label_set_font_cjk(lab3, 0);
	gt_label_set_font_align(lab3, GT_ALIGN_LEFT_MID);
    gt_label_set_text(lab3, "");
    gt_obj_set_visible(lab3, false);


	return wifi_list;
}

