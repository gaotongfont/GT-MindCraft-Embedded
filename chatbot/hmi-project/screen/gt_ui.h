#ifndef __GT_UI_H__
#define __GT_UI_H__


#include "../../GT-HMI-Engine/gt.h"


extern gt_obj_st * screen_home;
extern gt_obj_st * screen_recording;
extern gt_obj_st * screen_setup;
extern gt_obj_st * screen_subtitle;
extern gt_obj_st * screen_js;
extern gt_obj_st * screen_xg;
extern gt_obj_st * screen_ys;
extern gt_obj_st * screen_nc;
extern gt_obj_st * screen_fail;
extern gt_obj_st * screen_gl;
extern gt_obj_st * screen_jz;
extern gt_obj_st * Main_Interface;
extern gt_obj_st * Setting_List;
extern gt_obj_st * Volumeandbrightnesslab1;
extern gt_obj_st * keyboard;
extern gt_obj_st * forget_the_password;
extern gt_obj_st * Connection_failed;
extern gt_obj_st * Wi_Fi_List;
extern gt_obj_st * History_page;
extern gt_obj_st * Clear_page;


void gt_ui_init(void);


gt_obj_st * gt_init_screen_home(void);
gt_obj_st * gt_init_screen_recording(void);
gt_obj_st * gt_init_screen_setup(void);
gt_obj_st * gt_init_screen_subtitle(void);
gt_obj_st * gt_init_screen_js(void);
gt_obj_st * gt_init_screen_xg(void);
gt_obj_st * gt_init_screen_ys(void);
gt_obj_st * gt_init_screen_nc(void);
gt_obj_st * gt_init_screen_fail(void);
gt_obj_st * gt_init_screen_gl(void);
gt_obj_st * gt_init_screen_jz(void);
gt_obj_st * gt_init_Main_Interface(void);
gt_obj_st * gt_init_Setting_List(void);
gt_obj_st * gt_init_Volumeandbrightnesslab1(void);
gt_obj_st * gt_init_keyboard(void);
gt_obj_st * gt_init_forget_the_password(void);
gt_obj_st * gt_init_Connection_failed(void);
gt_obj_st * gt_init_Wi_Fi_List(void);
gt_obj_st * gt_init_History_page(void);
gt_obj_st * gt_init_Clear_page(void);


typedef enum {
        GT_ID_SCREEN_HOME = 0,
        GT_ID_SCREEN_RECORDING = 1,
        GT_ID_SCREEN_SETUP = 2,
        GT_ID_SCREEN_SUBTITLE = 3,
        GT_ID_SCREEN_JS = 4,
        GT_ID_SCREEN_XG = 5,
        GT_ID_SCREEN_YS = 6,
        GT_ID_SCREEN_NC = 7,
        GT_ID_SCREEN_FAIL = 8,
        GT_ID_SCREEN_GL = 9,
        GT_ID_SCREEN_JZ = 10,
        GT_ID_MAIN_INTERFACE = 11,
        GT_ID_SETTING_LIST = 12,
        GT_ID_VOLUMEANDBRIGHTNESSLAB1 = 13,
        GT_ID_KEYBOARD = 14,
        GT_ID_FORGET_THE_PASSWORD = 15,
        GT_ID_CONNECTION_FAILED = 16,
        GT_ID_WI_FI_LIST = 17,
        GT_ID_HISTORY_PAGE = 18,
        GT_ID_CLEAR_PAGE = 19,
} gt_scr_id_et;



#endif

