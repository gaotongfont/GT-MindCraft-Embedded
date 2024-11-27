#include "gt_ui.h"
#include "gt_font_config.h"


/** screen_setup */
gt_obj_st * screen_setup = NULL;
static gt_obj_st * img1 = NULL;
static gt_obj_st * lab1 = NULL;
static gt_obj_st * btn1 = NULL;
static gt_obj_st * lab2 = NULL;
static gt_obj_st * lab3 = NULL;
static gt_obj_st * btn2 = NULL;
static gt_obj_st * lab4 = NULL;
static gt_obj_st * lab5 = NULL;
static gt_obj_st * btn3 = NULL;
static gt_obj_st * imgbtn7 = NULL;
static gt_obj_st * imgbtn8 = NULL;
static gt_obj_st * imgbtn9 = NULL;
static gt_obj_st * lab6 = NULL;
static gt_obj_st * btn4 = NULL;
static gt_obj_st * imgbtn9Copy = NULL;
static gt_obj_st * txt1 = NULL;
static gt_obj_st * lab7 = NULL;
static gt_obj_st * btn5 = NULL;
static gt_obj_st * imgbtn9CopyCopy = NULL;

static void screen_setup_0_cb(gt_event_st * e) {
	gt_disp_stack_go_back(1);
}

static void img1_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_HOME, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void btn1_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_NC, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void btn2_0_cb(gt_event_st * e) {
	
}

static void btn3_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_YS, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void btn4_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_JS, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void btn5_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_XG, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

gt_obj_st * gt_init_screen_setup(void)
{
	screen_setup = gt_obj_create(NULL);
	gt_obj_add_event_cb(screen_setup, screen_setup_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(screen_setup, gt_color_hex(0x000000));


	

	/** img1 */
	img1 = gt_img_create(screen_setup);
	gt_obj_set_pos(img1, 13, 10);
	gt_obj_set_size(img1, 16, 16);
	gt_img_set_src(img1, "f:img_fh_16x16.png");
	gt_obj_add_event_cb(img1, img1_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** lab1 */
	lab1 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab1, 39, 5);
	gt_obj_set_size(lab1, 55, 26);
	gt_label_set_font_color(lab1, gt_color_hex(0x94a5b3));
	gt_label_set_font_align(lab1, GT_ALIGN_CENTER_MID);
	gt_label_set_text(lab1, "设置");

	

	/** btn1 */
	btn1 = gt_btn_create(screen_setup);
	gt_obj_set_pos(btn1, 21, 67);
	gt_obj_set_size(btn1, 200, 36);
	gt_btn_set_font_color(btn1, gt_color_hex(0x4193fb));
	gt_btn_set_font_family(btn1, gray_black_16);
	gt_btn_set_font_cjk(btn1, 0);
	gt_btn_set_font_align(btn1, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(btn1, "  智酱");
	gt_btn_set_color_background(btn1, gt_color_hex(0x181b22));
	gt_btn_set_color_pressed(btn1, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn1, gt_color_hex(0x000000));
	gt_btn_set_radius(btn1, 8);
	gt_obj_add_event_cb(btn1, btn1_0_cb, GT_EVENT_TYPE_INPUT_RELEASED, NULL);
	

	/** lab2 */
	lab2 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab2, 19, 35);
	gt_obj_set_size(lab2, 83, 23);
	gt_label_set_font_color(lab2, gt_color_hex(0x94a5b3));
	gt_label_set_font_family(lab2, gray_black_16);
	gt_label_set_font_cjk(lab2, 0);
	gt_label_set_font_align(lab2, GT_ALIGN_CENTER_MID);
	gt_label_set_text(lab2, "AI昵称：");

	

	/** lab3 */
	lab3 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab3, 17, 111);
	gt_obj_set_size(lab3, 84, 22);
	gt_label_set_font_color(lab3, gt_color_hex(0x94a5b3));
	gt_label_set_font_family(lab3, gray_black_16);
	gt_label_set_font_cjk(lab3, 0);
	gt_label_set_font_align(lab3, GT_ALIGN_CENTER_MID);
	gt_label_set_text(lab3, "角色介绍：");

	

	/** btn2 */
	btn2 = gt_btn_create(screen_setup);
	gt_obj_set_pos(btn2, 17, 233);
	gt_obj_set_size(btn2, 200, 36);
	gt_btn_set_font_color(btn2, gt_color_hex(0x4193fb));
	gt_btn_set_font_family(btn2, gray_black_16);
	gt_btn_set_font_cjk(btn2, 0);
	gt_btn_set_font_align(btn2, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(btn2, "  5");
	gt_btn_set_color_background(btn2, gt_color_hex(0x181b22));
	gt_btn_set_color_pressed(btn2, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn2, gt_color_hex(0x000000));
	gt_btn_set_radius(btn2, 6);
	gt_obj_add_event_cb(btn2, btn2_0_cb, GT_EVENT_TYPE_INPUT_RELEASED, NULL);
	

	/** lab4 */
	lab4 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab4, 19, 205);
	gt_obj_set_size(lab4, 84, 21);
	gt_label_set_font_color(lab4, gt_color_hex(0x94a5b3));
	gt_label_set_font_family(lab4, gray_black_16);
	gt_label_set_font_cjk(lab4, 0);
	gt_label_set_font_align(lab4, GT_ALIGN_CENTER_MID);
	gt_label_set_text(lab4, "用户年龄：");

	

	/** lab5 */
	lab5 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab5, 20, 277);
	gt_obj_set_size(lab5, 84, 23);
	gt_label_set_font_color(lab5, gt_color_hex(0x94a5b3));
	gt_label_set_font_family(lab5, gray_black_16);
	gt_label_set_font_cjk(lab5, 0);
	gt_label_set_font_align(lab5, GT_ALIGN_CENTER_MID);
	gt_label_set_text(lab5, "音色设定：");

	

	/** btn3 */
	btn3 = gt_btn_create(screen_setup);
	gt_obj_set_pos(btn3, 17, 308);
	gt_obj_set_size(btn3, 200, 36);
	gt_btn_set_font_color(btn3, gt_color_hex(0x4193fb));
	gt_btn_set_font_family(btn3, gray_black_16);
	gt_btn_set_font_cjk(btn3, 0);
	gt_btn_set_font_align(btn3, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(btn3, "  蔡机");
	gt_btn_set_color_background(btn3, gt_color_hex(0x181b22));
	gt_btn_set_color_pressed(btn3, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn3, gt_color_hex(0x000000));
	gt_btn_set_radius(btn3, 6);
	gt_obj_add_event_cb(btn3, btn3_0_cb, GT_EVENT_TYPE_INPUT_RELEASED, NULL);
	

	/** imgbtn7 */
	imgbtn7 = gt_imgbtn_create(screen_setup);
	gt_obj_set_pos(imgbtn7, 185, 78);
	gt_obj_set_size(imgbtn7, 27, 20);
	gt_imgbtn_set_src(imgbtn7, "f:img_down1_27x20.png");
	gt_imgbtn_add_state_item(imgbtn7, "f:img_down1_27x20.png");

	

	/** imgbtn8 */
	imgbtn8 = gt_imgbtn_create(screen_setup);
	gt_obj_set_pos(imgbtn8, 188, 243);
	gt_obj_set_size(imgbtn8, 27, 20);
	gt_imgbtn_set_src(imgbtn8, "f:img_down1_27x20.png");
	gt_imgbtn_add_state_item(imgbtn8, "f:img_up1_27x20.png");

	

	/** imgbtn9 */
	imgbtn9 = gt_imgbtn_create(screen_setup);
	gt_obj_set_pos(imgbtn9, 186, 316);
	gt_obj_set_size(imgbtn9, 27, 20);
	gt_imgbtn_set_src(imgbtn9, "f:img_down1_27x20.png");
	gt_imgbtn_add_state_item(imgbtn9, "f:img_up1_27x20.png");

	

	/** lab6 */
	lab6 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab6, 18, 355);
	gt_obj_set_size(lab6, 84, 23);
	gt_label_set_font_color(lab6, gt_color_hex(0x94a5b3));
	gt_label_set_font_family(lab6, gray_black_16);
	gt_label_set_font_cjk(lab6, 0);
	gt_label_set_font_align(lab6, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab6, "角色设定：");

	

	/** btn4 */
	btn4 = gt_btn_create(screen_setup);
	gt_obj_set_pos(btn4, 17, 381);
	gt_obj_set_size(btn4, 200, 36);
	gt_btn_set_font_color(btn4, gt_color_hex(0x4193fb));
	gt_btn_set_font_family(btn4, gray_black_16);
	gt_btn_set_font_cjk(btn4, 0);
	gt_btn_set_font_align(btn4, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(btn4, "（空）");
	gt_btn_set_color_background(btn4, gt_color_hex(0x181b22));
	gt_btn_set_color_pressed(btn4, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn4, gt_color_hex(0x000000));
	gt_btn_set_radius(btn4, 6);
	gt_obj_add_event_cb(btn4, btn4_0_cb, GT_EVENT_TYPE_INPUT_RELEASED, NULL);
	

	/** imgbtn9Copy */
	imgbtn9Copy = gt_imgbtn_create(screen_setup);
	gt_obj_set_pos(imgbtn9Copy, 185, 393);
	gt_obj_set_size(imgbtn9Copy, 27, 20);
	gt_imgbtn_set_src(imgbtn9Copy, "f:img_down1_27x20.png");
	gt_imgbtn_add_state_item(imgbtn9Copy, "f:img_up1_27x20.png");

	

	/** txt1 */
	txt1 = gt_textarea_create(screen_setup);
	gt_obj_set_pos(txt1, 17, 139);
	gt_obj_set_size(txt1, 200, 58);
	gt_textarea_set_font_color(txt1, gt_color_hex(0x0466dd));
	gt_textarea_set_font_family(txt1, gray_black_16);
	gt_textarea_set_font_cjk(txt1, 0);
	gt_textarea_set_font_align(txt1, GT_ALIGN_LEFT);
	gt_textarea_add_str(txt1, "你是住在芯片王国里的AI机器人，可以帮助用户解决各式各样的问题", 0, gt_color_hex(0x4193fb));
	gt_textarea_set_bg_color(txt1, gt_color_hex(0x181b22));

	

	/** lab7 */
	lab7 = gt_label_create(screen_setup);
	gt_obj_set_pos(lab7, 19, 427);
	gt_obj_set_size(lab7, 84, 23);
	gt_label_set_font_color(lab7, gt_color_hex(0x94a5b3));
	gt_label_set_font_family(lab7, gray_black_16);
	gt_label_set_font_cjk(lab7, 0);
	gt_label_set_font_align(lab7, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab7, "性格设定：");

	

	/** btn5 */
	btn5 = gt_btn_create(screen_setup);
	gt_obj_set_pos(btn5, 17, 452);
	gt_obj_set_size(btn5, 200, 36);
	gt_btn_set_font_color(btn5, gt_color_hex(0x4193fb));
	gt_btn_set_font_family(btn5, gray_black_16);
	gt_btn_set_font_cjk(btn5, 0);
	gt_btn_set_font_align(btn5, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(btn5, "（空）");
	gt_btn_set_color_background(btn5, gt_color_hex(0x181b22));
	gt_btn_set_color_pressed(btn5, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn5, gt_color_hex(0x000000));
	gt_btn_set_radius(btn5, 6);
	gt_obj_add_event_cb(btn5, btn5_0_cb, GT_EVENT_TYPE_INPUT_RELEASED, NULL);
	

	/** imgbtn9CopyCopy */
	imgbtn9CopyCopy = gt_imgbtn_create(screen_setup);
	gt_obj_set_pos(imgbtn9CopyCopy, 183, 459);
	gt_obj_set_size(imgbtn9CopyCopy, 27, 20);
	gt_imgbtn_set_src(imgbtn9CopyCopy, "f:img_down1_27x20.png");
	gt_imgbtn_add_state_item(imgbtn9CopyCopy, "f:img_up1_27x20.png");



	return screen_setup;
}

