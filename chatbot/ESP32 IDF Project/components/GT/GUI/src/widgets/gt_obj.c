/**
 * @file gt_obj.c
 * @author yongg
 * @brief
 * @version 0.1
 * @date 2022-05-11 15:03:47
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "stddef.h"
#include "gt_obj.h"
#include "gt_obj_class.h"
#include "../core/gt_obj_pos.h"
#include "../core/gt_timer.h"
#include "../core/gt_disp.h"
#include "../core/gt_mem.h"
#include "../core/gt_style.h"
#include "../core/gt_disp.h"
#include "../core/gt_obj_scroll.h"
#include "../core/gt_graph_base.h"
#include "../core/gt_draw.h"
#include "../hal/gt_hal_disp.h"
#include "../others/gt_log.h"
#include "../others/gt_gc.h"

#if GT_CFG_ENABLE_KEYPAD
#include "./gt_keypad.h"
#endif

/* private define -------------------------------------------------------*/
#define OBJ_TYPE    GT_TYPE_OBJ
#define MY_CLASS    &gt_obj_class

#define OBJ_TYPE_SCREEN     GT_TYPE_SCREEN
#define MY_CLASS_SCREEN     &gt_screen_class

/* private typedef ------------------------------------------------------*/



/* static variables -----------------------------------------------------*/
static void _init_cb(gt_obj_st * obj);
static void _obj_event_cb(gt_obj_st * obj, struct _gt_event_s * e);
static void _screen_event_cb(gt_obj_st * obj, struct _gt_event_s * e);

static GT_ATTRIBUTE_RAM_DATA const gt_obj_class_st gt_obj_class = {
    ._init_cb      = _init_cb,
    ._deinit_cb    = (_gt_deinit_cb_t)NULL,
    ._event_cb     = _obj_event_cb,
    .type          = OBJ_TYPE,
    .size_style    = sizeof(gt_obj_st)
};

static GT_ATTRIBUTE_RAM_DATA const gt_obj_class_st gt_screen_class = {
    ._init_cb      = (_gt_init_cb_t)NULL,
    ._deinit_cb    = (_gt_deinit_cb_t)NULL,
    ._event_cb     = _screen_event_cb,
    .type          = OBJ_TYPE_SCREEN,
    .size_style    = sizeof(gt_obj_st)
};

/* macros ---------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/
static void _init_cb(gt_obj_st * obj) {
    gt_area_st real_area = {0};
    gt_attr_rect_st rect_attr;
    gt_graph_init_rect_attr(&rect_attr);
    rect_attr.reg.is_fill = true;
    rect_attr.radius = obj->radius;

    if (gt_obj_get_mask_effect(obj) && GT_STATE_PRESSED == gt_obj_get_state(obj)) {
        rect_attr.bg_opa         = GT_OPA_100;
        rect_attr.border_color  = gt_color_hex(0x308ac5);
        rect_attr.fg_color      = gt_color_mix(gt_color_hex(0x308ac5), obj->bgcolor, 0x44);
        rect_attr.bg_color      = gt_color_mix(gt_color_hex(0x308ac5), obj->bgcolor, 0x44);
    } else if (gt_obj_is_show_bg(obj)) {
        rect_attr.bg_opa         = obj->opa;
        rect_attr.bg_color      = obj->bgcolor;
    } else {
        draw_focus(obj);
        return;
    }
    draw_bg(obj->draw_ctx, &rect_attr, &obj->area);
    draw_focus(obj);
}

static GT_ATTRIBUTE_RAM_TEXT void _screen_scroll(gt_obj_st * obj) {
    if (false == gt_obj_is_type(obj, OBJ_TYPE_SCREEN)) {
        return;
    }
    gt_area_abs_st * max_area_p = gt_disp_get_area_max();
    if (gt_disp_get_res_hor(NULL) == max_area_p->right &&
        gt_disp_get_res_ver(NULL) == max_area_p->bottom &&
        0 == max_area_p->left && 0 == max_area_p->top) {
        /** Full screen can display all widget, do not refresh full screen again */
        gt_area_st * area = gt_disp_get_area_act();
        area->x = 0;
        area->y = 0;
        obj->process_attr.scroll.x = 0;
        obj->process_attr.scroll.y = 0;
        return;
    }

    if (-obj->process_attr.scroll.x < max_area_p->left) {
        obj->process_attr.scroll.x = -max_area_p->left;
    }
    else if (-obj->process_attr.scroll.x + obj->area.w > max_area_p->right) {
        obj->process_attr.scroll.x = -max_area_p->right + obj->area.w;
    }
    if (-obj->process_attr.scroll.y < max_area_p->top) {
        obj->process_attr.scroll.y = -max_area_p->top;
    }else if (-obj->process_attr.scroll.y + obj->area.h > max_area_p->bottom) {
        obj->process_attr.scroll.y = -max_area_p->bottom + obj->area.h;
    }
    gt_disp_scroll_area_act(-obj->process_attr.scroll.x, -obj->process_attr.scroll.y);
}

static GT_ATTRIBUTE_RAM_TEXT void _masked_effect_handler(gt_obj_st * obj) {
    if (false == gt_obj_get_mask_effect(obj)) {
        return;
    }
    gt_disp_invalid_area(obj);
}

static GT_ATTRIBUTE_RAM_TEXT void _unmasked_effect_handler(gt_obj_st * obj) {
    if (false == gt_obj_get_mask_effect(obj)) {
        return;
    }
    gt_disp_invalid_area(obj);
}

static GT_ATTRIBUTE_RAM_TEXT void _change_state_by_trigger_mode(gt_obj_st * obj, bool is_pressed) {
    if (gt_obj_get_trigger_mode(obj)) {
        if (false == is_pressed) {
            gt_obj_set_state(obj, (gt_state_et)!gt_obj_get_state(obj));
        }
        return;
    }
    gt_obj_set_state(obj, is_pressed ? GT_STATE_PRESSED : GT_STATE_NONE);
}

#if GT_CFG_ENABLE_KEYPAD
static GT_ATTRIBUTE_RAM_TEXT void _auto_hide_keypad(gt_obj_st * scr) {
    gt_obj_st * kp = NULL;
    for (uint16_t i = 0; i < scr->cnt_child; ++i) {
        kp = scr->child[i];
        if (gt_obj_is_type(kp, GT_TYPE_KEYPAD)) {
            gt_keypad_hide(kp);
            gt_disp_invalid_area(scr);
        }
    }
}
#endif

static void _obj_event_cb(gt_obj_st * obj, struct _gt_event_s * e) {
    gt_event_type_et code_val = gt_event_get_code(e);

    switch(code_val) {
        case GT_EVENT_TYPE_DRAW_START: {
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_END, NULL);
            break;
        }
        case GT_EVENT_TYPE_UPDATE_STYLE: {
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_START, NULL);
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL: {
            _gt_obj_scroll_internal(obj, 0);
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL_END: {
            _change_state_by_trigger_mode(obj, false);
            break;
        }
        case GT_EVENT_TYPE_INPUT_PRESSED: {
            _change_state_by_trigger_mode(obj, true);
            _masked_effect_handler(obj);
            break;
        }
        case GT_EVENT_TYPE_INPUT_RELEASED: {
            _change_state_by_trigger_mode(obj, false);
            _unmasked_effect_handler(obj);
            break;
        }
        case GT_EVENT_TYPE_INPUT_PRESS_LOST: {
            _change_state_by_trigger_mode(obj, false);
            _unmasked_effect_handler(obj);
            break;
        }
        default:
            break;
    }
}

static void _screen_event_cb(gt_obj_st * obj, struct _gt_event_s * e) {
    gt_event_type_et code_val = gt_event_get_code(e);

    switch(code_val) {
        case GT_EVENT_TYPE_DRAW_START: {
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_END, NULL);
            break;
        }
        case GT_EVENT_TYPE_UPDATE_STYLE: {
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_START, NULL);
            break;
        }
        case GT_EVENT_TYPE_INPUT_KEY_RELEASED:
        case GT_EVENT_TYPE_INPUT_RELEASED: {
#if GT_CFG_ENABLE_KEYPAD
            _auto_hide_keypad(obj);
#endif
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL: {
            _screen_scroll(obj);
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL_UP: {
            /** scroll 3/4 screen */
            gt_obj_scroll_to_y(obj, (gt_disp_get_res_ver(NULL) * 3) >> 2, GT_ANIM_ON);
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL_DOWN: {
            gt_obj_scroll_to_y(obj, (-gt_disp_get_res_ver(NULL) * 3) >> 2, GT_ANIM_ON);
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL_LEFT: {
            gt_obj_scroll_to_x(obj, (gt_disp_get_res_hor(NULL) * 3) >> 2, GT_ANIM_ON);
            break;
        }
        case GT_EVENT_TYPE_INPUT_SCROLL_RIGHT: {
            gt_obj_scroll_to_x(obj, (-gt_disp_get_res_hor(NULL) * 3) >> 2, GT_ANIM_ON);
            break;
        }
        default:
            break;
    }
}

/**
 * @brief destroy the object by core timer handler,
 * Free the memory of the object and its children.
 *
 * @param timer
 */
static GT_ATTRIBUTE_RAM_TEXT void _gt_obj_destroy_handler_cb(struct _gt_timer_s * timer) {
    gt_obj_st * obj = (gt_obj_st * )_gt_timer_get_user_data(timer);
    GT_CHECK_BACK(obj);
    gt_event_st * ptr = _GT_GC_GET_ROOT(_gt_event_node_header_ll);
    while(ptr) {
        if (ptr->target == obj) {
            GT_LOGW(GT_LOG_TAG_GUI, "Disables the removal of the control itself within event list.");
            _gt_timer_set_repeat_count(timer, 1);   // retry to destroy object
            return ;
        }
        ptr = ptr->prev;
    }

    if (obj->using_sta) {
        GT_LOGW("obj", "Try to delete an object %p that is in use.", obj);
        _gt_timer_set_repeat_count(timer, 1);   // retry to destroy object
        return;
    }

    // release children memory and remove itself from parent children array
#if GT_USE_DISPLAY_PREF_DESTROY
    uint32_t start = gt_tick_get();
#endif

    _gt_obj_class_destroy(obj);
    _gt_timer_set_user_data(timer, NULL);

#if GT_USE_DISPLAY_PREF_DESTROY
    GT_LOGI(GT_LOG_TAG_MEM, "destroy obj: %p, used: %d ms", obj, gt_tick_get() - start);
#endif
}

static GT_ATTRIBUTE_RAM_TEXT gt_obj_st * _find_obj_recursive_by_id(gt_obj_st * self, gt_id_t widget_id) {
    gt_obj_st * target = NULL;

    if (widget_id == self->id) {
        return self;
    }

    for (gt_size_t i = 0, cnt = self->cnt_child; i < cnt; i++) {
        target = _find_obj_recursive_by_id(self->child[i], widget_id);
        if (target) {
            return target;
        }
    }
    return target;
}

/* global functions / API interface -------------------------------------*/

gt_obj_st * gt_obj_create(gt_obj_st * parent)
{
    gt_obj_st * obj = gt_obj_class_create(parent ? MY_CLASS : MY_CLASS_SCREEN, parent);
    GT_CHECK_BACK_VAL(obj, NULL);

    obj->fixed = false;
    obj->focus_dis = GT_DISABLED;
    return obj;
}

gt_obj_st * gt_obj_get_parent(gt_obj_st * obj)
{
    GT_CHECK_BACK_VAL(obj, NULL);
    return obj->parent;
}

gt_obj_st * gt_obj_change_parent(gt_obj_st * obj, gt_obj_st * to)
{
    return _gt_obj_class_change_parent(obj, to);
}

gt_obj_st * gt_obj_find_by_id(gt_id_t widget_id)
{
    gt_disp_st * disp = gt_disp_get_default();
    gt_obj_st * target = NULL;

    gt_size_t i = 0, cnt = disp->cnt_scr;

#if GT_USE_LAYER_TOP
    if (disp->layer_top && disp->layer_top->cnt_child) {
        target = _find_obj_recursive_by_id(disp->layer_top, widget_id);
        if (target) {
            return target;
        }
    }
#endif

    for (; i < cnt; i++) {
        target = _find_obj_recursive_by_id(disp->screens[i], widget_id);
        if (target) {
            break;
        }
    }
    return target;
}

void gt_obj_register_id(gt_obj_st * obj, gt_id_t id)
{
    GT_CHECK_BACK(obj);
    obj->id = id;
}

gt_id_t gt_obj_get_id(gt_obj_st * obj)
{
    if (NULL == obj) {
        return -1;
    }
    return obj->id;
}

bool gt_obj_is_child(gt_obj_st * obj, gt_obj_st * parent)
{
    if (GT_TYPE_UNKNOWN == gt_obj_class_get_type(obj)) { return false; }
    gt_obj_st * p = obj->parent;
    while (p) {
        if (p == parent) {
            return true;
        }
        p = p->parent;
    }
    return p == parent ? true : false;
}

uint16_t gt_obj_get_child_index(gt_obj_st * parent, gt_obj_st * obj)
{
    uint16_t ret = 0xFFFF;  /** Fail: 0xFFFF */
    GT_CHECK_BACK_VAL(parent, ret);
    GT_CHECK_BACK_VAL(obj, ret);
    for (uint16_t i = 0; i < parent->cnt_child; ++i) {
        if (parent->child[i] == obj) {
            return i;
        }
    }
    return ret;
}


gt_obj_st * gt_obj_search_child_by_type(gt_obj_st * parent, gt_obj_type_et type)
{
    gt_obj_st * ch = NULL;
    GT_CHECK_BACK_VAL(parent, NULL);
    for (uint16_t i = 0; i < parent->cnt_child; ++i) {
        if (gt_obj_is_type(parent->child[i], type)) {
            ch = parent->child[i];
            break;
        }
    }
    return ch;
}

uint16_t gt_obj_search_childs_count_by_type(gt_obj_st * parent, gt_obj_type_et type)
{
    uint16_t count = 0;
    GT_CHECK_BACK_VAL(parent, 0);
    for (uint16_t i = 0; i < parent->cnt_child; ++i) {
        if (gt_obj_is_type(parent->child[i], type)) {
            ++count;
        }
    }
    return count;
}

gt_obj_st * gt_obj_within_which_scr(gt_obj_st * obj)
{
    GT_CHECK_BACK_VAL(obj, NULL);
    gt_obj_st * p = obj->parent;
    while (p) {
        if (gt_obj_is_type(p, OBJ_TYPE_SCREEN)) {
            break;
        }
        p = p->parent;
    }
    return p;
}

void gt_obj_destroy(gt_obj_st * obj)
{
    // GT_LOGV(GT_LOG_TAG_GUI, "Want to destroy obj: 0x%p", obj);
    if (!obj) { return ; }
    obj->delate = true;
    gt_obj_child_set_prop(obj, GT_OBJ_PROP_TYPE_DELATE, true);

    // create a temp timer once task to destroy the object
    _gt_timer_st * tmp_timer = _gt_timer_create(_gt_obj_destroy_handler_cb, GT_TASK_PERIOD_TIME_DESTROY, obj);
    _gt_timer_set_repeat_count(tmp_timer, 1);
}

void gt_obj_destroy_immediately(gt_obj_st * obj)
{
    _gt_obj_class_destroy(obj);
}

void gt_screen_set_bgcolor(gt_obj_st * obj, gt_color_t color)
{
    if (false == gt_obj_is_type(obj, OBJ_TYPE_SCREEN)) {
        return;
    }
    obj->bgcolor = color;
}

gt_color_t gt_screen_get_bgcolor(gt_obj_st * obj)
{
    if (false == gt_obj_is_type(obj, OBJ_TYPE_SCREEN)) {
        return gt_color_make(0, 0, 0);
    }
    return obj->bgcolor;
}

void gt_obj_set_bgcolor(gt_obj_st * obj, gt_color_t color)
{
    if (false == gt_obj_is_type(obj, GT_TYPE_TOTAL)) {
        return ;
    }
    obj->bgcolor = color;
}

gt_color_t gt_obj_get_bgcolor(gt_obj_st * obj)
{
    if (false == gt_obj_is_type(obj, GT_TYPE_TOTAL)) {
        return gt_color_make(0, 0, 0);
    }
    return obj->bgcolor;
}

gt_size_t gt_obj_get_limit_right(gt_obj_st * obj)
{
    gt_obj_st * child = NULL;
    gt_size_t max_x = -0x7fff;
    gt_size_t temp = 0, min_x = 0x7fff;
    GT_CHECK_BACK_VAL(obj, 0);

    for (gt_size_t i = 0, cnt = obj->cnt_child; i < cnt; i++) {
        child = obj->child[i];
        if (child->area.x < min_x) {
            min_x = child->area.x;
        }
        temp = child->area.x + child->area.w;
        if (temp > max_x) {
            max_x = temp;
        }
    }
    return min_x - max_x + obj->area.w;
}

gt_size_t gt_obj_get_limit_bottom(gt_obj_st * obj)
{
    gt_obj_st * child = NULL;
    gt_size_t max_y = -0x7fff;
    gt_size_t temp = 0, min_y = 0x7fff;
    GT_CHECK_BACK_VAL(obj, 0);

    for (gt_size_t i = 0, cnt = obj->cnt_child; i < cnt; i++) {
        child = obj->child[i];
        if (child->area.y < min_y) {
            min_y = child->area.y;
        }
        temp = child->area.y + child->area.h;
        if (temp > max_y) {
            max_y = temp;
        }
    }
    return min_y - max_y + obj->area.h;
}

gt_point_st gt_obj_get_childs_max_size(gt_obj_st * parent)
{
    gt_obj_st * child = NULL;
    gt_point_st ret_size = { .x = 0, .y = 0 };
    gt_point_st min = { .x = 0x7fff, .y = 0x7fff };
    gt_point_st max = { .x = -0x7fff, .y = -0x7fff };
    GT_CHECK_BACK_VAL(parent, ret_size);

    for (gt_size_t i = parent->cnt_child - 1; i >= 0; --i) {
        child = parent->child[i];
        if (child->area.x < min.x) { min.x = child->area.x; }
        if (child->area.y < min.y) { min.y = child->area.y; }
        if (child->area.x + child->area.w > max.x) { max.x = child->area.x + child->area.w; }
        if (child->area.y + child->area.h > max.y) { max.y = child->area.y + child->area.h; }
    }
    ret_size.x = max.x - min.x;
    ret_size.y = max.y - min.y;
    return ret_size;
}

gt_size_t gt_obj_get_childs_max_width(gt_obj_st * parent)
{
    gt_point_st size = gt_obj_get_childs_max_size(parent);
    return size.x;
}

gt_size_t gt_obj_get_childs_max_height(gt_obj_st * parent)
{
    gt_point_st size = gt_obj_get_childs_max_size(parent);
    return size.y;
}

void gt_obj_show_bg(gt_obj_st * obj, bool show)
{
    if (false == gt_obj_is_type(obj, GT_TYPE_TOTAL)) {
        return;
    }
    obj->show_bg = show;
}

bool gt_obj_is_show_bg(gt_obj_st * obj)
{
    if (false == gt_obj_is_type(obj, GT_TYPE_TOTAL)) {
        return false;
    }
    return obj->show_bg;
}

/* end ------------------------------------------------------------------*/
