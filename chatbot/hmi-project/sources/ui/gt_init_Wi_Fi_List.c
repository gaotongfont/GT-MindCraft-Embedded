#include "gt_ui.h"
#include "gt_font_config.h"


/** Wi_Fi_List */
gt_obj_st * Wi_Fi_List = NULL;
static gt_obj_st * listv1 = NULL;
static gt_obj_st * lab1 = NULL;
static gt_obj_st * lab2 = NULL;

static void screen_home_0_cb(gt_event_st * e) {
	gt_disp_stack_go_back(1);
}

gt_obj_st * gt_init_Wi_Fi_List(void)
{
	Wi_Fi_List = gt_obj_create(NULL);
	gt_obj_add_event_cb(Wi_Fi_List, screen_home_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(Wi_Fi_List, gt_color_hex(0x000000));


	

	/** listv1 */
	listv1 = gt_listview_create(Wi_Fi_List);
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
	gt_listview_set_highlight_mode(listv1, 1);
	gt_listview_set_bg_color(listv1, gt_color_hex(0x000000));
	gt_listview_set_item_bg_color(listv1, gt_color_hex(0x181B22));
	gt_listview_show_item_bg(listv1, true);
	gt_listview_set_item_reduce(listv1, 3);
	gt_listview_set_item_radius(listv1, 8);
	gt_listview_set_scale(listv1, 25, 75);
	gt_listview_set_scale_triple(listv1, 20, 60, 20);
	gt_listview_set_icon_to_right(listv1, 1);
	gt_listview_set_next_row_item_count(listv1, 1);
	gt_listview_add_item_icon(listv1, ".:img_Connected_220x278.png", "  很长的名长...");
	gt_listview_set_next_row_item_count(listv1, 1);
	gt_listview_add_item_icon(listv1, ".:img_Lock_220x278.png", "  WiFi3");
	gt_listview_add_item_icon(listv1, ".:img_Lock_220x278.png", "  WiFi4");
	gt_listview_add_item_icon(listv1, ".:img_Lock_220x278.png", "  WiFi5");
	gt_listview_add_item_icon(listv1, ".:img_Lock_220x278.png", "  WiFi6");
	gt_listview_add_item_icon(listv1, ".:img_Lock_220x278.png", "  WiFi7");

	

	/** lab1 */
	lab1 = gt_label_create(Wi_Fi_List);
	gt_obj_set_pos(lab1, 10, 6);
	gt_obj_set_size(lab1, 55, 31);
	gt_label_set_font_color(lab1, gt_color_hex(0xc0c0c0));
	gt_label_set_font_family(lab1, gray_black_20);
	gt_label_set_font_cjk(lab1, 0);
	gt_label_set_font_align(lab1, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab1, "WIFi");

	

	/** lab2 */
	lab2 = gt_label_create(Wi_Fi_List);
	gt_obj_set_pos(lab2, 51, 7);
	gt_obj_set_size(lab2, 126, 29);
	gt_label_set_font_color(lab2, gt_color_hex(0x94A5B3));
	gt_label_set_font_family(lab2, gray_black_16);
	gt_label_set_font_cjk(lab2, 0);
	gt_label_set_font_align(lab2, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab2, "仅支持2.4G");



	return Wi_Fi_List;
}

