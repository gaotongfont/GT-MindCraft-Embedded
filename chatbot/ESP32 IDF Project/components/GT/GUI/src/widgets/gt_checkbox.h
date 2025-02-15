/**
 * @file gt_checkbox.h
 * @author yongg
 * @brief checkbox element
 * @version 0.1
 * @date 2022-07-18 14:11:44
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */
#ifndef _GT_CHECKBOX_H_
#define _GT_CHECKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* include --------------------------------------------------------------*/
#include "gt_conf_widgets.h"

#if GT_CFG_ENABLE_CHECKBOX
#include "gt_obj.h"
#include "gt_obj_class.h"
#include "stdarg.h"
#include "stdlib.h"
#include "../font/gt_font.h"

/* define ---------------------------------------------------------------*/



/* typedef --------------------------------------------------------------*/



/* macros ---------------------------------------------------------------*/



/* global functions / API interface -------------------------------------*/

/**
 * @brief create checkbox obj
 *
 * @param parent checkbox's parent obj
 * @return gt_obj_st* checkbox obj ptr
 */
gt_obj_st * gt_checkbox_create(gt_obj_st * parent);

/**
 * @brief set the checkbox text
 *
 * @param checkbox checkbox obj
 * @param fmt text format
 */
void gt_checkbox_set_text(gt_obj_st * checkbox, const char * fmt, ...);

void gt_checkbox_set_text_by_len(gt_obj_st * checkbox, const char * text, uint16_t len);

char * gt_checkbox_get_text(gt_obj_st * checkbox);

void gt_checkbox_set_font_color(gt_obj_st * checkbox, gt_color_t color);
#if (defined(GT_FONT_FAMILY_OLD_ENABLE) && (GT_FONT_FAMILY_OLD_ENABLE == 1))
void gt_checkbox_set_font_family_cn(gt_obj_st * checkbox, gt_family_t family);
void gt_checkbox_set_font_family_en(gt_obj_st * checkbox, gt_family_t family);
void gt_checkbox_set_font_family_fl(gt_obj_st * checkbox, gt_family_t family);
void gt_checkbox_set_font_family_numb(gt_obj_st * checkbox, gt_family_t family);
#else
void gt_checkbox_set_font_family(gt_obj_st * checkbox, gt_family_t family);
void gt_checkbox_set_font_cjk(gt_obj_st* checkbox, gt_font_cjk_et cjk);
#endif
void gt_checkbox_set_font_size(gt_obj_st * checkbox, uint8_t size);
void gt_checkbox_set_font_gray(gt_obj_st * checkbox, uint8_t gray);
void gt_checkbox_set_font_thick_en(gt_obj_st * checkbox, uint8_t thick);
void gt_checkbox_set_font_thick_cn(gt_obj_st * checkbox, uint8_t thick);
void gt_checkbox_set_font_encoding(gt_obj_st * checkbox, gt_encoding_et encoding);
void gt_checkbox_set_font_style(gt_obj_st * checkbox, gt_font_style_et font_style);
void gt_checkbox_set_space(gt_obj_st * checkbox, uint8_t space_x, uint8_t space_y);

void gt_checkbox_set_font_point_offset_x(gt_obj_st * checkbox , gt_size_t x);
void gt_checkbox_set_font_point_offset_y(gt_obj_st * checkbox , gt_size_t y);
void gt_checkbox_set_font_point_offset(gt_obj_st * checkbox , gt_size_t x , gt_size_t y);



#endif  /** GT_CFG_ENABLE_CHECKBOX */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!_GT_CHECKBOX_H_
