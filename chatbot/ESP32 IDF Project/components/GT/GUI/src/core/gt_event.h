/**
 * @file gt_event.h
 * @author yongg
 * @brief Object implementation for the event handler
 * @version 0.1
 * @date 2022-05-11 14:57:40
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */
#ifndef _GT_EVENT_TYPE_H_
#define _GT_EVENT_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* include --------------------------------------------------------------*/
#include "../gt_conf.h"
#include "gt_event.h"
#include "../others/gt_types.h"

/* define ---------------------------------------------------------------*/



/* typedef --------------------------------------------------------------*/
/**
 * @brief The event type enum definition
 */
typedef enum gt_event_type_e {
    /* =================== Core defined here begin =================== */
    GT_EVENT_TYPE_NONE = 0,

    /* Input device events */
    _GT_EVENT_TYPE_INPUT_BEGIN,

    GT_EVENT_TYPE_INPUT_RELEASED = _GT_EVENT_TYPE_INPUT_BEGIN,     // release event
    GT_EVENT_TYPE_INPUT_PRESSING,                                  // pressing, not scroll
    GT_EVENT_TYPE_INPUT_PRESSED,                                   // pressed
    GT_EVENT_TYPE_INPUT_LONG_PRESSED,
    GT_EVENT_TYPE_INPUT_PRESS_LOST,    // move off and lost focus handler
    GT_EVENT_TYPE_INPUT_SCROLL_START,
    GT_EVENT_TYPE_INPUT_SCROLL,
    GT_EVENT_TYPE_INPUT_SCROLL_END,
    GT_EVENT_TYPE_INPUT_KEY,                // keypad pressed event
    GT_EVENT_TYPE_INPUT_KEY_PRESSING,       // keypad pressing event
    GT_EVENT_TYPE_INPUT_KEY_RELEASED,       // keypad released event
    GT_EVENT_TYPE_INPUT_KEY_LONG_PRESSED,   // keypad long pressed event
    GT_EVENT_TYPE_INPUT_FOCUSED,
    GT_EVENT_TYPE_INPUT_SCROLL_UP,
    GT_EVENT_TYPE_INPUT_SCROLL_DOWN,
    GT_EVENT_TYPE_INPUT_SCROLL_LEFT,
    GT_EVENT_TYPE_INPUT_SCROLL_RIGHT,
    GT_EVENT_TYPE_INPUT_HIDED,          // Need to hide layer top widgets
    GT_EVENT_TYPE_INPUT_HOME_GESTURE_TOP,
    GT_EVENT_TYPE_INPUT_HOME_GESTURE_BOTTOM,
    GT_EVENT_TYPE_INPUT_HOME_GESTURE_LEFT,
    GT_EVENT_TYPE_INPUT_HOME_GESTURE_RIGHT,
    GT_EVENT_TYPE_INPUT_PLAY_START,
    GT_EVENT_TYPE_INPUT_PLAY_END,
    _GT_EVENT_TYPE_INPUT_END,

    /* update value */
    GT_EVENT_TYPE_UPDATE_VALUE = _GT_EVENT_TYPE_INPUT_END,   // value update, need to redraw
    GT_EVENT_TYPE_UPDATE_STYLE,

    /* Drawing events */
    GT_EVENT_TYPE_DRAW_START,    // need to start draw widget
    GT_EVENT_TYPE_DRAW_END,      // draw widget ok
    GT_EVENT_TYPE_DRAW_REDRAW,
    GT_EVENT_TYPE_SCREEN_LOAD_BEFORE,       // [Called by core inside] screen loading to draw before
    GT_EVENT_TYPE_SCREEN_BEFORE_GO_BACK,    // [Called by core inside] screen before go back

    /* CHANGE events */
    GT_EVENT_TYPE_CHANGE_CHILD_REMOVE,     // [Called by core inside] remove child but not delete child
    GT_EVENT_TYPE_CHANGE_CHILD_REMOVED,    // [Called by core inside] remove child finish
    GT_EVENT_TYPE_CHANGE_CHILD_ADD,        // [Called by core inside] add child
    GT_EVENT_TYPE_CHANGE_CHILD_DELETE,     // [Called by core inside] delete child
    GT_EVENT_TYPE_CHANGE_CHILD_DELETED,    // [Called by core inside] delete child finish
    GT_EVENT_TYPE_CHANGE_DELETED,          // [Called by core inside] delete self finish

    /* scroll boundary events */
    GT_EVENT_TYPE_PULL_TO_REFRESH,         // [Called by core inside] scroll to top, pull to refresh event
    GT_EVENT_TYPE_LOAD_MORE,               // [Called by core inside] scroll to bottom, load more event

    /* notify event */
    GT_EVENT_TYPE_NOTIFY_CLOSE,     // notify widget to close

    /* =================== Core defined here end =================== */

    /* =================== User custom defined here begin =================== */



    /* =================== User custom defined here end =================== */

    _GT_EVENT_TYPE_TOTAL_COUNT,    // count event number

    GT_EVENT_TYPE_ALL = 0xFFFF,
}gt_event_type_et;

typedef struct _gt_event_s {
    struct _gt_event_s * prev;
    struct gt_obj_s * target;   /** Bubble notify temp target */
    struct gt_obj_s * origin;   /** origin control */
    void * user_data;
    void * param;               /** gt_event_send() param */
    gt_event_type_et code_type;
    uint32_t key_val;           /** key value, using by keypad indev mode */
}gt_event_st;

/**
 * @brief The event handler callback function pointer
 *
 * @param e An object describing the content of the event
 */
typedef void (* gt_event_cb_t)(gt_event_st * e);

/* macros ---------------------------------------------------------------*/



/* global functions / API interface -------------------------------------*/
#if GT_USE_SCREEN_ANIM
/**
 * @brief Disabled when the screen is animating.
 *
 * @param enabled
 */
void gt_event_set_enabled(bool enabled);

/**
 * @brief Disabled when the screen is animating.
 *
 * @return true
 * @return false
 */
bool gt_event_is_enabled(void);
#endif  /** GT_USE_SCREEN_ANIM */

/**
 * @brief Adding an user event to an object
 *
 * @param obj Which object need to add event callback
 * @param event The callback function
 * @param filter The event type enum @ref gt_event_type_e
 * @param user_data memory free by user [default or unused: NULL]
 */
void gt_obj_add_event_cb(struct gt_obj_s * obj, gt_event_cb_t event, gt_event_type_et filter, void * user_data);

/**
 * @brief Using user's event() to replace the core event_cb() callback handler,
 *      [Warn] The core event_cb() event type inline handler will be ignored.
 *
 * @param obj
 * @param event The callback function
 * @param filter
 * @param user_data
 */
void gt_obj_add_replace_core_event_cb(struct gt_obj_s * obj, gt_event_cb_t event, gt_event_type_et filter, void * user_data);

/**
 * @brief Check if the event callback is already added
 *
 * @param obj
 * @param event
 * @param filter
 * @return true The event callback is already added
 * @return false The event callback is not added
 */
bool gt_event_has_the_same_cb(struct gt_obj_s * obj, gt_event_cb_t event, gt_event_type_et filter);

/**
 * @brief Get the user event count
 *
 * @param obj
 * @return uint16_t
 */
uint16_t gt_obj_get_user_event_count(struct gt_obj_s * obj);

/**
 * @brief [Safe] Remove the event callback
 *
 * @param obj
 */
void gt_obj_remove_all_event_cb(struct gt_obj_s * obj);

/**
 * @brief [Unsafe] Remove all event callback immediately
 *
 * @param obj
 */
void gt_obj_remove_all_event_cb_immediately(struct gt_obj_s * obj);

/**
 * @brief Send an event code to the object kernel.
 *
 * @param parent Object which want to be notified
 * @param event The event code to send
 * @param parms User data [default or unused: NULL]
 * @return gt_res_t The result status @ref gt_res_t
 */
gt_res_t gt_event_send(struct gt_obj_s * parent, gt_event_type_et event, void * parms);

/**
 * @brief Send an event to the child object, excluding the parent object.
 *
 * @param parent object
 * @param event
 * @param parms
 * @return gt_res_t
 */
gt_res_t gt_event_send_to_childs(struct gt_obj_s * parent, gt_event_type_et event, void * parms);

/**
 * @brief
 *
 * @param widget_id register widget id by gt_obj_register_id()
 * @param event
 * @param parms
 * @return gt_res_t
 */
gt_res_t gt_event_send_by_id(gt_id_t widget_id, gt_event_type_et event, void * parms);

#if GT_USE_LAYER_TOP
/**
 * @brief Adding an event to the global, can be called by any time, any screen or widget
 *
 * @param event
 * @param filter
 * @param user_data
 */
void gt_global_add_event_cb(gt_event_cb_t event, gt_event_type_et filter, void * user_data);

/**
 * @brief Check global user event if the event callback is already added
 *
 * @param event
 * @param filter
 * @return true The event callback is already added
 * @return false The event callback is not added
 */
bool gt_global_event_has_the_same_cb(gt_event_cb_t event, gt_event_type_et filter);

/**
 * @brief Get the global event count
 *
 * @return uint16_t
 */
uint16_t gt_global_get_event_count(void);

/**
 * @brief [Safe] Remove global event callback, not the widget object
 */
void gt_global_remove_all_event_cb(void);

/**
 * @brief [Unsafe] Remove global all event callback immediately, not
 *      the widget object
 */
void gt_global_remove_all_event_cb_immediately(void);

/**
 * @brief Send an event code to the global register user event.
 *
 * @param event The event code to send
 * @param parms User data [default or unused: NULL]
 * @return gt_res_t The result status @ref gt_res_t
 */
gt_res_t gt_global_event_send(gt_event_type_et event, void * parms);

/**
 * @brief [Warn, Called by indev] Send an event code to the global register user event.
 *      Not for the user to call.
 *
 * @param org The orgin object
 * @param event The event code to send
 * @param parms User data [default or unused: NULL]
 * @return gt_res_t The result status @ref gt_res_t
 */
gt_res_t _gt_global_core_indev_event_send(struct gt_obj_s * org, gt_event_type_et event, void * parms);

#endif  /** GT_USE_LAYER_TOP */

/**
 * @brief Get the code value for the event
 *
 * @param e The structure of the event
 * @return gt_event_type_et get newly code value @ref gt_event_type_e
 */
gt_event_type_et gt_event_get_code(gt_event_st * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!_GT_EVENT_TYPE_H_
