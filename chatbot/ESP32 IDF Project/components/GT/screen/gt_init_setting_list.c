#include "gt_ui.h"
#include "gt_font_config.h"

static const char *TAG = "SCREEN_SETTING_LIST";

extern SemaphoreHandle_t is_scanning_sem;
/** setting_list */
gt_obj_st * setting_list = NULL;
static gt_obj_st * WiFiSettings = NULL;
static gt_obj_st * img1 = NULL;
static gt_obj_st * Volumeandbrightness = NULL;
static gt_obj_st * img2 = NULL;

static void screen_home_0_cb(gt_event_st * e) 
{
	gt_disp_stack_go_back(1);
    xSemaphoreTake(scr_id_mutex, portMAX_DELAY);
	GT_PROTOCOL* gt_pro = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
	memset(gt_pro, 0, sizeof(GT_PROTOCOL));
	gt_pro->head_type = LOAD_MAIN_SCR;
	gt_pro->data = NULL;
	xQueueSend(gui_task_queue, &gt_pro, portMAX_DELAY);
	xSemaphoreGive(scr_id_mutex);
}

static void WiFiSettings_0_cb(gt_event_st * e) 
{
    ESP_LOGI(TAG,"--------is_auto_connected_end = %d\n", is_auto_connected_end);
	gt_disp_stack_load_scr_anim(GT_ID_WIFI_LIST, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
	wifi_scanning_dialog();
	GT_PROTOCOL* gt_pro = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
	gt_pro->head_type = WIFI_SCAN_EVENT;
	xQueueSend(wifi_task_queue, &gt_pro, portMAX_DELAY);
}

static void Volumeandbrightness_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_VOLUME_AND_BRIGHTNESS, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

gt_obj_st * gt_init_setting_list(void)
{
	setting_list = gt_obj_create(NULL);
	gt_obj_add_event_cb(setting_list, screen_home_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(setting_list, gt_color_hex(0x000000));

	/** WiFiSettings */
	/** WiFi设置 */
	WiFiSettings = gt_btn_create(setting_list);
	gt_obj_set_pos(WiFiSettings, 15, 26);
	gt_obj_set_size(WiFiSettings, 205, 52);
	gt_btn_set_font_color(WiFiSettings, gt_color_hex(0xffffff));
	gt_btn_set_font_family(WiFiSettings, gray_black_20);
	gt_btn_set_font_cjk(WiFiSettings, 0);
	gt_btn_set_font_align(WiFiSettings, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(WiFiSettings, "     WiFi");
	gt_btn_set_color_background(WiFiSettings, gt_color_hex(0x181b22));
	gt_btn_set_color_pressed(WiFiSettings, gt_color_hex(0x1682F7));
	gt_btn_set_font_color_pressed(WiFiSettings, gt_color_hex(0xffffff));
	gt_btn_set_radius(WiFiSettings, 8);
	gt_obj_add_event_cb(WiFiSettings, WiFiSettings_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);


	/** img1 */
	img1 = gt_img_create(setting_list);
	gt_obj_set_pos(img1, 24, 33);
	gt_obj_set_size(img1, 36, 36);
	gt_img_set_src(img1, "f:img_Wi_Fisettings_36x36.png");



	/** Volumeandbrightness */
	/** 音量与亮度 */
	Volumeandbrightness = gt_btn_create(setting_list);
	gt_obj_set_pos(Volumeandbrightness, 15, 86);
	gt_obj_set_size(Volumeandbrightness, 205, 52);
	gt_btn_set_font_color(Volumeandbrightness, gt_color_hex(0xffffff));
	gt_btn_set_font_family(Volumeandbrightness, gray_black_20);
	gt_btn_set_font_cjk(Volumeandbrightness, 0);
	gt_btn_set_font_align(Volumeandbrightness, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(Volumeandbrightness, "  音量与亮度");
	gt_btn_set_color_background(Volumeandbrightness, gt_color_hex(0x181B22));
	gt_btn_set_color_pressed(Volumeandbrightness, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(Volumeandbrightness, gt_color_hex(0x000000));
	gt_btn_set_radius(Volumeandbrightness, 8);
	gt_obj_add_event_cb(Volumeandbrightness, Volumeandbrightness_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);


	/** img2 */
	img2 = gt_img_create(setting_list);
	gt_obj_set_pos(img2, 24, 92);
	gt_obj_set_size(img2, 36, 36);
	gt_img_set_src(img2, "f:img_VolumeBrightness_36x36.png");



	return setting_list;
}

