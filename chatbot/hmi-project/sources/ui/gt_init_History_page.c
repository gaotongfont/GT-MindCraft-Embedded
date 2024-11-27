#include "gt_ui.h"
#include "gt_font_config.h"


/** History_page */
gt_obj_st * History_page = NULL;
static gt_obj_st * imgbtn1 = NULL;
static gt_obj_st * Historybt = NULL;
static gt_obj_st * emptybt = NULL;
static gt_obj_st * stupbt = NULL;
static gt_obj_st * btn1 = NULL;
static gt_obj_st * btn2 = NULL;
static gt_obj_st * btn1Copy = NULL;

static void screen_subtitle_0_cb(gt_event_st * e) {
	gt_disp_stack_go_back(1);
}

static void imgbtn1_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_HOME, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void Historybt_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_HISTORY_PAGE, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void emptybt_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_CLEAR_PAGE, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void stupbt_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_SETUP, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

gt_obj_st * gt_init_History_page(void)
{
	History_page = gt_obj_create(NULL);
	gt_obj_add_event_cb(History_page, screen_subtitle_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(History_page, gt_color_hex(0x000000));


	

	/** imgbtn1 */
	imgbtn1 = gt_imgbtn_create(History_page);
	gt_obj_set_pos(imgbtn1, 13, 10);
	gt_obj_set_size(imgbtn1, 16, 16);
	gt_imgbtn_set_src(imgbtn1, ".:img_fh_16x16.png");
	gt_obj_add_event_cb(imgbtn1, imgbtn1_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** Historybt */
	/** 历史记录 */
	Historybt = gt_imgbtn_create(History_page);
	gt_obj_set_pos(Historybt, 64, 8);
	gt_obj_set_size(Historybt, 22, 19);
	gt_imgbtn_set_src(Historybt, ".:img_History_22x19.png");
	gt_imgbtn_set_src_press(Historybt, ".:img_History2_22x19.png");
	gt_imgbtn_add_state_item(Historybt, ".:img_History2_22x19.png");
	gt_obj_add_event_cb(Historybt, Historybt_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** emptybt */
	/** 清空 */
	emptybt = gt_imgbtn_create(History_page);
	gt_obj_set_pos(emptybt, 129, 9);
	gt_obj_set_size(emptybt, 18, 18);
	gt_imgbtn_set_src(emptybt, ".:img_empty_18x18.png");
	gt_imgbtn_add_state_item(emptybt, ".:img_empty2_18x18.png");
	gt_obj_add_event_cb(emptybt, emptybt_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** stupbt */
	/** 设置 */
	stupbt = gt_img_create(History_page);
	gt_obj_set_pos(stupbt, 188, 8);
	gt_obj_set_size(stupbt, 24, 24);
	gt_img_set_src(stupbt, ".:img_Set_up_24x24.png");
	gt_obj_add_event_cb(stupbt, stupbt_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** btn1 */
	btn1 = gt_btn_create(History_page);
	gt_obj_set_pos(btn1, 39, 50);
	gt_obj_set_size(btn1, 187, 45);
	gt_btn_set_font_color(btn1, gt_color_hex(0xffffff));
	gt_btn_set_font_family(btn1, gray_black_16);
	gt_btn_set_font_cjk(btn1, 0);
	gt_btn_set_font_align(btn1, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(btn1, "你好，你是谁？");
	gt_btn_set_color_background(btn1, gt_color_hex(0x1B8BFC));
	gt_btn_set_color_pressed(btn1, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn1, gt_color_hex(0x000000));
	gt_btn_set_radius(btn1, 20);

	

	/** btn2 */
	btn2 = gt_btn_create(History_page);
	gt_obj_set_pos(btn2, 13, 109);
	gt_obj_set_size(btn2, 207, 133);
	gt_btn_set_font_color(btn2, gt_color_hex(0xffffff));
	gt_btn_set_font_family(btn2, gray_black_16);
	gt_btn_set_font_cjk(btn2, 0);
	gt_btn_set_font_align(btn2, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(btn2, "你好，小朋友。 我是住在芯片王国里的一个机器人。 你可以叫我老师。我能够帮助你学习新知识。");
	gt_btn_set_color_background(btn2, gt_color_hex(0x1656CC));
	gt_btn_set_color_pressed(btn2, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn2, gt_color_hex(0x000000));
	gt_btn_set_radius(btn2, 20);

	

	/** btn1Copy */
	btn1Copy = gt_btn_create(History_page);
	gt_obj_set_pos(btn1Copy, 21, 257);
	gt_obj_set_size(btn1Copy, 201, 45);
	gt_btn_set_font_color(btn1Copy, gt_color_hex(0xffffff));
	gt_btn_set_font_family(btn1Copy, gray_black_16);
	gt_btn_set_font_cjk(btn1Copy, 0);
	gt_btn_set_font_align(btn1Copy, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(btn1Copy, "黑洞形成的原理是什么？");
	gt_btn_set_color_background(btn1Copy, gt_color_hex(0x1B8BFC));
	gt_btn_set_color_pressed(btn1Copy, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn1Copy, gt_color_hex(0x000000));
	gt_btn_set_radius(btn1Copy, 20);



	return History_page;
}

