/**
 * @file gt_focus.h
 * @author Yang
 * @brief
 * @version 0.1
 * @date 2025-01-14 10:29:24
 * @copyright Copyright (c) 2014-2025, Company Genitop. Co., Ltd.
 */
#ifndef _GT_FOCUS_H_
#define _GT_FOCUS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* include --------------------------------------------------------------*/
#include "gt_conf_widgets.h"
#if GT_CFG_ENABLE_FOCUS
#include "gt_obj.h"
#include "gt_obj_class.h"
#include "stdarg.h"
#include "stdlib.h"
#include "../others/gt_color.h"
#include "../core/gt_style.h"
#include "../font/gt_font.h"



/* define ---------------------------------------------------------------*/



/* typedef --------------------------------------------------------------*/



/* macros ---------------------------------------------------------------*/



/* global functions / API interface -------------------------------------*/
gt_obj_st * gt_focus_create(gt_obj_st * parent);

void gt_focus_test();




#endif //!GT_CFG_ENABLE_FOCUS
//
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!_GT_FOCUS_H_

/* end of file ----------------------------------------------------------*/


