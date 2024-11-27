#include "gt_ui.h"
#include "gt_font_config.h"


/** Main_Interface */
gt_obj_st * Main_Interface = NULL;
static gt_obj_st * AIRobots = NULL;
static gt_obj_st * setup = NULL;
static gt_obj_st * imgbtn1 = NULL;
static gt_obj_st * imgbtn2 = NULL;

static void screen_home_0_cb(gt_event_st * e) {
	
}

static void AIRobots_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SCREEN_HOME, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

static void setup_0_cb(gt_event_st * e) {
	gt_disp_stack_load_scr_anim(GT_ID_SETTING_LIST, GT_SCR_ANIM_TYPE_NONE, 500, 0, true);
}

gt_obj_st * gt_init_Main_Interface(void)
{
	Main_Interface = gt_obj_create(NULL);

	gt_screen_set_bgcolor(Main_Interface, gt_color_hex(0x000000));


	

	/** AIRobots */
	/** AI机器人 */
	AIRobots = gt_imgbtn_create(Main_Interface);
	gt_obj_set_pos(AIRobots, 13, 34);
	gt_obj_set_size(AIRobots, 101, 124);
	gt_imgbtn_set_src(AIRobots, ".:img_3096_101x124.png");
	gt_obj_add_event_cb(AIRobots, AIRobots_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** setup */
	/** 设置 */
	setup = gt_imgbtn_create(Main_Interface);
	gt_obj_set_pos(setup, 126, 35);
	gt_obj_set_size(setup, 105, 124);
	gt_imgbtn_set_src(setup, ".:img_3097_105x124.png");
	gt_obj_add_event_cb(setup, setup_0_cb, GT_EVENT_TYPE_INPUT_PRESSED, NULL);
	

	/** imgbtn1 */
	imgbtn1 = gt_imgbtn_create(Main_Interface);
	gt_obj_set_pos(imgbtn1, 177, 4);
	gt_obj_set_size(imgbtn1, 16, 12);
	gt_imgbtn_set_src(imgbtn1, ".:img_WIFIsignal1_16x12.png");
	gt_imgbtn_add_state_item(imgbtn1, ".:img_WIFIsignal1_16x12.png");
	gt_imgbtn_add_state_item(imgbtn1, ".:img_WIFIsignal2_16x12.png");
	gt_imgbtn_add_state_item(imgbtn1, ".:img_WIFIsignal3_16x12.png");
	gt_imgbtn_add_state_item(imgbtn1, ".:img_WIFIsignal4_16x12.png");
	gt_imgbtn_add_state_item(imgbtn1, ".:img_WIFIsignal5_16x12.png");

	

	/** imgbtn2 */
	imgbtn2 = gt_imgbtn_create(Main_Interface);
	gt_obj_set_pos(imgbtn2, 206, 4);
	gt_obj_set_size(imgbtn2, 22, 12);
	gt_imgbtn_set_src(imgbtn2, ".:img_Battery_22x12.png");



	return Main_Interface;
}

