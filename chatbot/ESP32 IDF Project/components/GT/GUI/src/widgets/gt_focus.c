/**
 * @file gt_focus.c
 * @author Yang
 * @brief
 * @version 0.1
 * @date 2025-01-14 10:29:11
 * @copyright Copyright (c) 2014-2025, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "gt_focus.h"
#if GT_CFG_ENABLE_FOCUS
#include "../core/gt_mem.h"
#include "../others/gt_log.h"
#include "string.h"
#include "../core/gt_graph_base.h"
#include "../core/gt_obj_pos.h"
#include "../others/gt_assert.h"
#include "../core/gt_draw.h"
#include "../core/gt_disp.h"
#include "../others/gt_anim.h"
#include "../utils/gt_vector.h"

/* private define -------------------------------------------------------*/
#define OBJ_TYPE    GT_TYPE_FOCUS
#define MY_CLASS    &gt_focus_class


/* private typedef ------------------------------------------------------*/
typedef struct _gt_focus_s {
    gt_obj_st obj;

    _gt_vector_st* focus_vec;

}_gt_focus_st;


/* static prototypes ----------------------------------------------------*/



/* static variables -----------------------------------------------------*/
static void _init_cb(gt_obj_st * obj);
static void _deinit_cb(gt_obj_st * obj);
static void _event_cb(struct gt_obj_s * obj, gt_event_st * e);

static GT_ATTRIBUTE_RAM_DATA const gt_obj_class_st gt_focus_class = {
    ._init_cb      = (_gt_init_cb_t) NULL,
    ._deinit_cb    = _deinit_cb,
    ._event_cb     = _event_cb,
    .type          = OBJ_TYPE,
    .size_style    = sizeof(_gt_focus_st)
};

/* macros ---------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/
static void _deinit_cb(gt_obj_st * obj)
{
    _gt_focus_st * style = (_gt_focus_st *)obj;
    if(!style->focus_vec){
        _gt_vector_free(style->focus_vec);
        style->focus_vec = NULL;
    }

}

static void _event_cb(struct gt_obj_s * obj, gt_event_st * e)
{

}


/* global functions / API interface -------------------------------------*/
gt_obj_st * gt_focus_create(gt_obj_st * parent)
{
    gt_obj_st * obj = gt_obj_class_create(MY_CLASS, parent);
    if (NULL == obj) {
        return obj;
    }
    obj->focus_dis = GT_DISABLED;

    _gt_focus_st * style = (_gt_focus_st *)obj;

    style->focus_vec = _gt_vector_create(NULL, NULL);

    return obj;
}


/* end of file ----------------------------------------------------------*/
#endif /* GT_CFG_ENABLE_FOCUS */

