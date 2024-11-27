#include "gt_ui.h"


static const gt_scr_list_st gt_scr_list[] = {
	{GT_ID_SCREEN_HOME, gt_init_screen_home},
	{GT_ID_SCREEN_RECORDING, gt_init_screen_recording},
	{GT_ID_SCREEN_SETUP, gt_init_screen_setup},
	{GT_ID_SCREEN_SUBTITLE, gt_init_screen_subtitle},
	{GT_ID_SCREEN_JS, gt_init_screen_js},
	{GT_ID_SCREEN_XG, gt_init_screen_xg},
	{GT_ID_SCREEN_YS, gt_init_screen_ys},
	{GT_ID_SCREEN_NC, gt_init_screen_nc},
	{GT_ID_SCREEN_FAIL, gt_init_screen_fail},
	{GT_ID_SCREEN_GL, gt_init_screen_gl},
	{GT_ID_SCREEN_JZ, gt_init_screen_jz},
	{GT_ID_MAIN_INTERFACE, gt_init_Main_Interface},
	{GT_ID_SETTING_LIST, gt_init_Setting_List},
	{GT_ID_VOLUMEANDBRIGHTNESSLAB1, gt_init_Volumeandbrightnesslab1},
	{GT_ID_KEYBOARD, gt_init_keyboard},
	{GT_ID_FORGET_THE_PASSWORD, gt_init_forget_the_password},
	{GT_ID_CONNECTION_FAILED, gt_init_Connection_failed},
	{GT_ID_WI_FI_LIST, gt_init_Wi_Fi_List},
	{GT_ID_HISTORY_PAGE, gt_init_History_page},
	{GT_ID_CLEAR_PAGE, gt_init_Clear_page},
};

void gt_ui_init(void) {


	gt_scr_stack_register_id_list(gt_scr_list, sizeof(gt_scr_list) / sizeof(gt_scr_list[0]), 20);
	gt_scr_stack_set_home_scr_id(GT_ID_MAIN_INTERFACE, false);
	gt_disp_stack_load_scr(GT_ID_MAIN_INTERFACE);
}

