#include "gt_ui.h"
#include "gt_font_config.h"


/** Clear_page */
gt_obj_st * Clear_page = NULL;
static gt_obj_st * imgbtn1 = NULL;
static gt_obj_st * Historybt = NULL;
static gt_obj_st * emptybt = NULL;
static gt_obj_st * stupbt = NULL;

static void screen_subtitle_0_cb(gt_event_st * e) {
	gt_disp_stack_go_back(1);
}

	
gt_obj_st * _Clear_page_dialog1_init() {
		

	gt_obj_st * dialog1 = NULL;
	gt_obj_st * lab6 = NULL;
	gt_obj_st * btn1 = NULL;
	gt_obj_st * btn2 = NULL;
	

	/** dialog1 */
	/** 清空历史记录 */
	dialog1 = gt_dialog_create(true);
	gt_obj_set_pos(dialog1, 0, 86);
	gt_obj_set_size(dialog1, 240, 159);
	gt_dialog_set_bgcolor(dialog1, gt_color_hex(0x181B22));
	gt_dialog_set_border_color(dialog1, gt_color_hex(0xc7c7c7));
	gt_dialog_set_border_width(dialog1, 0);
	gt_dialog_set_border_radius(dialog1, 20);
	

	/** lab6 */
	lab6 = gt_label_create(dialog1);
	gt_obj_set_pos(lab6, 23, 128);
	gt_obj_set_size(lab6, 199, 33);
	gt_label_set_font_color(lab6, gt_color_hex(0xffffff));
	gt_label_set_font_family(lab6, gray_black_20);
	gt_label_set_font_cjk(lab6, 0);
	gt_label_set_font_align(lab6, GT_ALIGN_CENTER_MID);
	gt_label_set_text(lab6, "是否清空历史记录？");
	

	/** btn1 */
	/** 是 */
	btn1 = gt_btn_create(dialog1);
	gt_obj_set_pos(btn1, 18, 191);
	gt_obj_set_size(btn1, 74, 37);
	gt_btn_set_font_color(btn1, gt_color_hex(0xffffff));
	gt_btn_set_font_family(btn1, gray_black_16);
	gt_btn_set_font_cjk(btn1, 0);
	gt_btn_set_font_align(btn1, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(btn1, "是");
	gt_btn_set_color_background(btn1, gt_color_hex(0x3895FF));
	gt_btn_set_color_pressed(btn1, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn1, gt_color_hex(0x000000));
	gt_btn_set_radius(btn1, 20);
	

	/** btn2 */
	/** 否 */
	btn2 = gt_btn_create(dialog1);
	gt_obj_set_pos(btn2, 133, 191);
	gt_obj_set_size(btn2, 74, 37);
	gt_btn_set_font_color(btn2, gt_color_hex(0xffffff));
	gt_btn_set_font_family(btn2, gray_black_16);
	gt_btn_set_font_cjk(btn2, 0);
	gt_btn_set_font_align(btn2, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(btn2, "否");
	gt_btn_set_color_background(btn2, gt_color_hex(0xFF5733));
	gt_btn_set_color_pressed(btn2, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn2, gt_color_hex(0x000000));
	gt_btn_set_radius(btn2, 20);




	gt_dialog_show(dialog1);
	
	return dialog1;
}

static void imgbtn1_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_HOME, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void Historybt_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_HISTORY_PAGE, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void emptybt_0_cb(gt_event_st * e) {
		_Clear_page_dialog1_init();
}

static void stupbt_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_SETUP, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

gt_obj_st * gt_init_Clear_page(void)
{
	Clear_page = gt_obj_create(NULL);
	gt_obj_add_event_cb(Clear_page, screen_subtitle_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(Clear_page, gt_color_hex(0x000000));


	

	/** imgbtn1 */
	imgbtn1 = gt_imgbtn_create(Clear_page);
	gt_obj_set_pos(imgbtn1, 13, 10);
	gt_obj_set_size(imgbtn1, 16, 16);
	gt_imgbtn_set_src(imgbtn1, ".:img_fh_16x16.png");
	gt_obj_add_event_cb(imgbtn1, imgbtn1_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** Historybt */
	/** 历史记录 */
	Historybt = gt_imgbtn_create(Clear_page);
	gt_obj_set_pos(Historybt, 64, 8);
	gt_obj_set_size(Historybt, 22, 19);
	gt_imgbtn_set_src(Historybt, ".:img_History_22x19.png");
	gt_imgbtn_add_state_item(Historybt, ".:img_History2_22x19.png");
	gt_obj_add_event_cb(Historybt, Historybt_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** emptybt */
	/** 清空 */
	emptybt = gt_imgbtn_create(Clear_page);
	gt_obj_set_pos(emptybt, 129, 9);
	gt_obj_set_size(emptybt, 18, 18);
	gt_imgbtn_set_src(emptybt, ".:img_empty_18x18.png");
	gt_imgbtn_add_state_item(emptybt, ".:img_empty2_18x18.png");
	gt_obj_add_event_cb(emptybt, emptybt_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** stupbt */
	/** 设置 */
	stupbt = gt_img_create(Clear_page);
	gt_obj_set_pos(stupbt, 189, 7);
	gt_obj_set_size(stupbt, 24, 24);
	gt_img_set_src(stupbt, ".:img_Set_up_24x24.png");
	gt_obj_add_event_cb(stupbt, stupbt_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);


	return Clear_page;
}

