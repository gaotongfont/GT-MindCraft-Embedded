#include "gt_ui.h"
#include "gt_font_config.h"


/** keyboard */
gt_obj_st * keyboard = NULL;
static gt_obj_st * VolumeSettings = NULL;
static gt_obj_st * lab1 = NULL;
static gt_obj_st * btn1 = NULL;
static gt_obj_st * a = NULL;
static gt_obj_st * b = NULL;
static gt_obj_st * c = NULL;
static gt_obj_st * d = NULL;
static gt_obj_st * OK = NULL;
static gt_obj_st * e = NULL;
static gt_obj_st * f = NULL;
static gt_obj_st * g = NULL;
static gt_obj_st * h = NULL;
static gt_obj_st * Delete = NULL;
static gt_obj_st * i = NULL;
static gt_obj_st * j = NULL;
static gt_obj_st * k = NULL;
static gt_obj_st * l = NULL;
static gt_obj_st * aA = NULL;
static gt_obj_st * m = NULL;
static gt_obj_st * n = NULL;
static gt_obj_st * o = NULL;
static gt_obj_st * oCopy = NULL;
static gt_obj_st * Numbersymbolkey = NULL;
static gt_obj_st * q = NULL;
static gt_obj_st * r = NULL;
static gt_obj_st * s = NULL;
static gt_obj_st * t = NULL;

static void screen_home_0_cb(gt_event_st * e) {
	gt_disp_stack_go_back(1);
}

gt_obj_st * gt_init_keyboard(void)
{
	keyboard = gt_obj_create(NULL);
	gt_obj_add_event_cb(keyboard, screen_home_0_cb, GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT, NULL);
	gt_screen_set_bgcolor(keyboard, gt_color_hex(0x000000));


	

	/** VolumeSettings */
	/** 音量设置 */
	VolumeSettings = gt_label_create(keyboard);
	gt_obj_set_pos(VolumeSettings, 10, 6);
	gt_obj_set_size(VolumeSettings, 41, 39);
	gt_label_set_font_color(VolumeSettings, gt_color_hex(0xc0c0c0));
	gt_label_set_font_family(VolumeSettings, gray_black_20);
	gt_label_set_font_cjk(VolumeSettings, 0);
	gt_label_set_font_align(VolumeSettings, GT_ALIGN_LEFT_MID);
	gt_label_set_text(VolumeSettings, "WiFi");

	

	/** lab1 */
	lab1 = gt_label_create(keyboard);
	gt_obj_set_pos(lab1, 51, 7);
	gt_obj_set_size(lab1, 114, 33);
	gt_label_set_font_color(lab1, gt_color_hex(0x94A5B3));
	gt_label_set_font_family(lab1, gray_black_16);
	gt_label_set_font_cjk(lab1, 0);
	gt_label_set_font_align(lab1, GT_ALIGN_LEFT_MID);
	gt_label_set_text(lab1, "仅支持2.4G");

	

	/** btn1 */
	btn1 = gt_btn_create(keyboard);
	gt_obj_set_pos(btn1, 3, 42);
	gt_obj_set_size(btn1, 232, 44);
	gt_btn_set_font_color(btn1, gt_color_hex(0xc0c0c0));
	gt_btn_set_font_family(btn1, gray_black_16);
	gt_btn_set_font_cjk(btn1, 0);
	gt_btn_set_font_align(btn1, GT_ALIGN_LEFT_MID);
	gt_btn_set_text(btn1, " 输入密码");
	gt_btn_set_color_background(btn1, gt_color_hex(0x181B22));
	gt_btn_set_color_pressed(btn1, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(btn1, gt_color_hex(0x000000));
	gt_btn_set_radius(btn1, 8);

	

	/** a */
	a = gt_btn_create(keyboard);
	gt_obj_set_pos(a, 4, 92);
	gt_obj_set_size(a, 44, 44);
	gt_btn_set_font_color(a, gt_color_hex(0xffffff));
	gt_btn_set_font_family(a, gray_black_16);
	gt_btn_set_font_cjk(a, 0);
	gt_btn_set_font_align(a, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(a, "a");
	gt_btn_set_color_background(a, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(a, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(a, gt_color_hex(0x000000));
	gt_btn_set_radius(a, 8);

	

	/** b */
	b = gt_btn_create(keyboard);
	gt_obj_set_pos(b, 50, 92);
	gt_obj_set_size(b, 44, 44);
	gt_btn_set_font_color(b, gt_color_hex(0xffffff));
	gt_btn_set_font_family(b, gray_black_16);
	gt_btn_set_font_cjk(b, 0);
	gt_btn_set_font_align(b, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(b, "b");
	gt_btn_set_color_background(b, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(b, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(b, gt_color_hex(0x000000));
	gt_btn_set_radius(b, 8);

	

	/** c */
	c = gt_btn_create(keyboard);
	gt_obj_set_pos(c, 97, 92);
	gt_obj_set_size(c, 44, 44);
	gt_btn_set_font_color(c, gt_color_hex(0xffffff));
	gt_btn_set_font_family(c, gray_black_16);
	gt_btn_set_font_cjk(c, 0);
	gt_btn_set_font_align(c, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(c, "c");
	gt_btn_set_color_background(c, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(c, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(c, gt_color_hex(0x000000));
	gt_btn_set_radius(c, 8);

	

	/** d */
	d = gt_btn_create(keyboard);
	gt_obj_set_pos(d, 144, 92);
	gt_obj_set_size(d, 44, 44);
	gt_btn_set_font_color(d, gt_color_hex(0xffffff));
	gt_btn_set_font_family(d, gray_black_16);
	gt_btn_set_font_cjk(d, 0);
	gt_btn_set_font_align(d, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(d, "d");
	gt_btn_set_color_background(d, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(d, gt_color_hex(0x00a8ff));
	gt_btn_set_font_color_pressed(d, gt_color_hex(0x000000));
	gt_btn_set_radius(d, 8);

	

	/** OK */
	/** 确认键 */
	OK = gt_imgbtn_create(keyboard);
	gt_obj_set_pos(OK, 191, 92);
	gt_obj_set_size(OK, 44, 44);
	gt_imgbtn_set_src(OK, "f:img_3110_44x44.png");
	gt_imgbtn_set_src_press(OK, "f:img_3116_44x44.png");
	gt_imgbtn_add_state_item(OK, "f:img_3117_44x44.png");

	

	/** e */
	e = gt_btn_create(keyboard);
	gt_obj_set_pos(e, 4, 139);
	gt_obj_set_size(e, 44, 44);
	gt_btn_set_font_color(e, gt_color_hex(0xffffff));
	gt_btn_set_font_family(e, gray_black_16);
	gt_btn_set_font_cjk(e, 0);
	gt_btn_set_font_align(e, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(e, "e");
	gt_btn_set_color_background(e, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(e, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(e, gt_color_hex(0x000000));
	gt_btn_set_radius(e, 8);

	

	/** f */
	f = gt_btn_create(keyboard);
	gt_obj_set_pos(f, 50, 139);
	gt_obj_set_size(f, 44, 44);
	gt_btn_set_font_color(f, gt_color_hex(0xffffff));
	gt_btn_set_font_family(f, gray_black_16);
	gt_btn_set_font_cjk(f, 0);
	gt_btn_set_font_align(f, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(f, "f");
	gt_btn_set_color_background(f, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(f, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(f, gt_color_hex(0x000000));
	gt_btn_set_radius(f, 8);

	

	/** g */
	g = gt_btn_create(keyboard);
	gt_obj_set_pos(g, 97, 139);
	gt_obj_set_size(g, 44, 44);
	gt_btn_set_font_color(g, gt_color_hex(0xffffff));
	gt_btn_set_font_family(g, gray_black_16);
	gt_btn_set_font_cjk(g, 0);
	gt_btn_set_font_align(g, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(g, "g");
	gt_btn_set_color_background(g, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(g, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(g, gt_color_hex(0x000000));
	gt_btn_set_radius(g, 8);

	

	/** h */
	h = gt_btn_create(keyboard);
	gt_obj_set_pos(h, 144, 140);
	gt_obj_set_size(h, 44, 44);
	gt_btn_set_font_color(h, gt_color_hex(0xffffff));
	gt_btn_set_font_family(h, gray_black_16);
	gt_btn_set_font_cjk(h, 0);
	gt_btn_set_font_align(h, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(h, "h");
	gt_btn_set_color_background(h, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(h, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(h, gt_color_hex(0x000000));
	gt_btn_set_radius(h, 8);

	

	/** Delete */
	/** 删除键 */
	Delete = gt_imgbtn_create(keyboard);
	gt_obj_set_pos(Delete, 191, 140);
	gt_obj_set_size(Delete, 44, 44);
	gt_imgbtn_set_src(Delete, "f:img_3111_44x44.png");
	gt_imgbtn_add_state_item(Delete, "f:img_3118_44x44.png");

	

	/** i */
	i = gt_btn_create(keyboard);
	gt_obj_set_pos(i, 4, 186);
	gt_obj_set_size(i, 44, 44);
	gt_btn_set_font_color(i, gt_color_hex(0xffffff));
	gt_btn_set_font_family(i, gray_black_16);
	gt_btn_set_font_cjk(i, 0);
	gt_btn_set_font_align(i, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(i, "i");
	gt_btn_set_color_background(i, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(i, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(i, gt_color_hex(0x000000));
	gt_btn_set_radius(i, 8);

	

	/** j */
	j = gt_btn_create(keyboard);
	gt_obj_set_pos(j, 51, 187);
	gt_obj_set_size(j, 44, 44);
	gt_btn_set_font_color(j, gt_color_hex(0xffffff));
	gt_btn_set_font_family(j, gray_black_16);
	gt_btn_set_font_cjk(j, 0);
	gt_btn_set_font_align(j, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(j, "j");
	gt_btn_set_color_background(j, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(j, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(j, gt_color_hex(0x000000));
	gt_btn_set_radius(j, 8);

	

	/** k */
	k = gt_btn_create(keyboard);
	gt_obj_set_pos(k, 98, 187);
	gt_obj_set_size(k, 44, 44);
	gt_btn_set_font_color(k, gt_color_hex(0xffffff));
	gt_btn_set_font_family(k, gray_black_16);
	gt_btn_set_font_cjk(k, 0);
	gt_btn_set_font_align(k, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(k, "k");
	gt_btn_set_color_background(k, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(k, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(k, gt_color_hex(0x000000));
	gt_btn_set_radius(k, 8);

	

	/** l */
	l = gt_btn_create(keyboard);
	gt_obj_set_pos(l, 145, 188);
	gt_obj_set_size(l, 44, 44);
	gt_btn_set_font_color(l, gt_color_hex(0xffffff));
	gt_btn_set_font_family(l, gray_black_16);
	gt_btn_set_font_cjk(l, 0);
	gt_btn_set_font_align(l, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(l, "l");
	gt_btn_set_color_background(l, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(l, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(l, gt_color_hex(0x000000));
	gt_btn_set_radius(l, 8);

	

	/** aA */
	/** 大小写切换键 */
	aA = gt_imgbtn_create(keyboard);
	gt_obj_set_pos(aA, 192, 186);
	gt_obj_set_size(aA, 44, 44);
	gt_imgbtn_set_src(aA, "f:img_3109_44x44.png");
	gt_imgbtn_set_src_press(aA, "f:img_3112_44x44.png");
	gt_imgbtn_add_state_item(aA, "f:img_3114_44x44.png");

	

	/** m */
	m = gt_btn_create(keyboard);
	gt_obj_set_pos(m, 4, 233);
	gt_obj_set_size(m, 44, 44);
	gt_btn_set_font_color(m, gt_color_hex(0xffffff));
	gt_btn_set_font_family(m, gray_black_16);
	gt_btn_set_font_cjk(m, 0);
	gt_btn_set_font_align(m, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(m, "m");
	gt_btn_set_color_background(m, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(m, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(m, gt_color_hex(0x000000));
	gt_btn_set_radius(m, 8);

	

	/** n */
	n = gt_btn_create(keyboard);
	gt_obj_set_pos(n, 51, 233);
	gt_obj_set_size(n, 44, 44);
	gt_btn_set_font_color(n, gt_color_hex(0xffffff));
	gt_btn_set_font_family(n, gray_black_16);
	gt_btn_set_font_cjk(n, 0);
	gt_btn_set_font_align(n, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(n, "n");
	gt_btn_set_color_background(n, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(n, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(n, gt_color_hex(0x000000));
	gt_btn_set_radius(n, 8);

	

	/** o */
	o = gt_btn_create(keyboard);
	gt_obj_set_pos(o, 98, 234);
	gt_obj_set_size(o, 44, 44);
	gt_btn_set_font_color(o, gt_color_hex(0xffffff));
	gt_btn_set_font_family(o, gray_black_16);
	gt_btn_set_font_cjk(o, 0);
	gt_btn_set_font_align(o, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(o, "o");
	gt_btn_set_color_background(o, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(o, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(o, gt_color_hex(0x000000));
	gt_btn_set_radius(o, 8);

	

	/** oCopy */
	oCopy = gt_btn_create(keyboard);
	gt_obj_set_pos(oCopy, 146, 235);
	gt_obj_set_size(oCopy, 44, 44);
	gt_btn_set_font_color(oCopy, gt_color_hex(0xffffff));
	gt_btn_set_font_family(oCopy, gray_black_16);
	gt_btn_set_font_cjk(oCopy, 0);
	gt_btn_set_font_align(oCopy, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(oCopy, "p");
	gt_btn_set_color_background(oCopy, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(oCopy, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(oCopy, gt_color_hex(0x000000));
	gt_btn_set_radius(oCopy, 8);

	

	/** Numbersymbolkey */
	/** 数字符号键 */
	Numbersymbolkey = gt_imgbtn_create(keyboard);
	gt_obj_set_pos(Numbersymbolkey, 193, 236);
	gt_obj_set_size(Numbersymbolkey, 44, 44);
	gt_imgbtn_set_src(Numbersymbolkey, "f:img_3113_44x44.png");
	gt_imgbtn_set_src_press(Numbersymbolkey, "f:img_3115_44x44.png");

	

	/** q */
	q = gt_btn_create(keyboard);
	gt_obj_set_pos(q, 4, 280);
	gt_obj_set_size(q, 44, 44);
	gt_btn_set_font_color(q, gt_color_hex(0xffffff));
	gt_btn_set_font_family(q, gray_black_16);
	gt_btn_set_font_cjk(q, 0);
	gt_btn_set_font_align(q, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(q, "q");
	gt_btn_set_color_background(q, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(q, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(q, gt_color_hex(0x000000));
	gt_btn_set_radius(q, 8);

	

	/** r */
	r = gt_btn_create(keyboard);
	gt_obj_set_pos(r, 52, 280);
	gt_obj_set_size(r, 44, 44);
	gt_btn_set_font_color(r, gt_color_hex(0xffffff));
	gt_btn_set_font_family(r, gray_black_16);
	gt_btn_set_font_cjk(r, 0);
	gt_btn_set_font_align(r, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(r, "r");
	gt_btn_set_color_background(r, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(r, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(r, gt_color_hex(0x000000));
	gt_btn_set_radius(r, 8);

	

	/** s */
	s = gt_btn_create(keyboard);
	gt_obj_set_pos(s, 99, 281);
	gt_obj_set_size(s, 44, 44);
	gt_btn_set_font_color(s, gt_color_hex(0xffffff));
	gt_btn_set_font_family(s, gray_black_16);
	gt_btn_set_font_cjk(s, 0);
	gt_btn_set_font_align(s, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(s, "s");
	gt_btn_set_color_background(s, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(s, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(s, gt_color_hex(0x000000));
	gt_btn_set_radius(s, 8);

	

	/** t */
	t = gt_btn_create(keyboard);
	gt_obj_set_pos(t, 146, 282);
	gt_obj_set_size(t, 44, 44);
	gt_btn_set_font_color(t, gt_color_hex(0xffffff));
	gt_btn_set_font_family(t, gray_black_16);
	gt_btn_set_font_cjk(t, 0);
	gt_btn_set_font_align(t, GT_ALIGN_CENTER_MID);
	gt_btn_set_text(t, "t");
	gt_btn_set_color_background(t, gt_color_hex(0x30384A));
	gt_btn_set_color_pressed(t, gt_color_hex(0x1A589F));
	gt_btn_set_font_color_pressed(t, gt_color_hex(0x000000));
	gt_btn_set_radius(t, 8);



	return keyboard;
}

