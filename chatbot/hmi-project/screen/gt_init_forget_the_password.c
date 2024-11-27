#include "gt_ui.h"
#include "gt_font_config.h"


/** forget_the_password */
gt_obj_st * forget_the_password = NULL;
static gt_obj_st * VolumeSettings = NULL;
static gt_obj_st * wifiicon = NULL;
static gt_obj_st * Cancel = NULL;
static gt_obj_st * OK = NULL;

static void screen_home_0_cb(gt_event_st * e) {
	
}

gt_obj_st * gt_init_forget_the_password(void)
{
	forget_the_password = gt_obj_create(NULL);

	gt_screen_set_bgcolor(forget_the_password, gt_color_hex(0x000000));


	

	/** VolumeSettings */
	/** 音量设置 */
	VolumeSettings = gt_label_create(forget_the_password);
	gt_obj_set_pos(VolumeSettings, 39, 151);
	gt_obj_set_size(VolumeSettings, 158, 27);
	gt_label_set_font_color(VolumeSettings, gt_color_hex(0xffffff));
	gt_label_set_font_family(VolumeSettings, gray_black_20);
	gt_label_set_font_cjk(VolumeSettings, 0);
	gt_label_set_font_align(VolumeSettings, GT_ALIGN_CENTER_MID);
	gt_label_set_text(VolumeSettings, "是否忘记密码？");

	

	/** wifiicon */
	/** 图标 */
	wifiicon = gt_img_create(forget_the_password);
	gt_obj_set_pos(wifiicon, 74, 50);
	gt_obj_set_size(wifiicon, 82, 82);
	gt_img_set_src(wifiicon, "f:img_3106_82x82.png");

	

	/** Cancel */
	/** 取消键 */
	Cancel = gt_btn_create(forget_the_password);
	gt_obj_set_pos(Cancel, 27, 208);
	gt_obj_set_size(Cancel, 80, 40);
	gt_btn_set_font_color(Cancel, gt_color_hex(0xffffff));
	gt_btn_set_font_family(Cancel, gray_black_16);
	gt_btn_set_font_cjk(Cancel, 0);
	gt_btn_set_font_align(Cancel, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(Cancel, "取消");
	gt_btn_set_color_background(Cancel, gt_color_hex(0x0075FD));
	gt_btn_set_color_pressed(Cancel, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(Cancel, gt_color_hex(0x000000));
	gt_btn_set_radius(Cancel, 20);

	

	/** OK */
	/** 确认键 */
	OK = gt_btn_create(forget_the_password);
	gt_obj_set_pos(OK, 132, 208);
	gt_obj_set_size(OK, 80, 40);
	gt_btn_set_font_color(OK, gt_color_hex(0xffffff));
	gt_btn_set_font_family(OK, gray_black_16);
	gt_btn_set_font_cjk(OK, 0);
	gt_btn_set_font_align(OK, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(OK, "确认");
	gt_btn_set_color_background(OK, gt_color_hex(0xFF5733));
	gt_btn_set_color_pressed(OK, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(OK, gt_color_hex(0x000000));
	gt_btn_set_radius(OK, 20);



	return forget_the_password;
}

