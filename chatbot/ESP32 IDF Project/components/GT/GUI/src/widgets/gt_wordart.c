/**
 * @file gt_wordart.c
 * @author Yang
 * @brief
 * @version 0.1
 * @date 2023-07-21 14:10:32
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "gt_wordart.h"
#include "../core/gt_mem.h"
#include "../others/gt_log.h"
#include "string.h"
#include "../core/gt_graph_base.h"
#include "../core/gt_obj_pos.h"
#include "../font/gt_font.h"
#include "../others/gt_assert.h"
#include "../core/gt_draw.h"
#include "../core/gt_disp.h"

/* private define -------------------------------------------------------*/
#define OBJ_TYPE    GT_TYPE_WORDART
#define MY_CLASS    &gt_word_art_class

#if GT_CFG_ENABLE_WORDART
/* private typedef ------------------------------------------------------*/
typedef struct _gt_word_art_item_s {
    uint32_t encode;
    char * img;
#if GT_USE_FILE_HEADER
    gt_file_header_param_st fh;
#endif
#if GT_USE_DIRECT_ADDR
    gt_addr_t addr;
#endif
#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
    gt_direct_addr_custom_size_st custom_addr;
#endif
}_gt_word_art_item_st;

typedef struct _gt_wor_dart_s {
    gt_obj_st obj;
    _gt_word_art_item_st * items;
    uint32_t total;
    uint32_t item_number;
    char * text;
    uint8_t space_x;
    uint8_t space_y;
}_gt_wor_dart_st;


/* static prototypes ----------------------------------------------------*/



/* static variables -----------------------------------------------------*/
static void _init_cb(gt_obj_st * obj);
static void _deinit_cb(gt_obj_st * obj);
static void _event_cb(struct gt_obj_s * obj, gt_event_st * e);

static GT_ATTRIBUTE_RAM_DATA const gt_obj_class_st gt_word_art_class = {
    ._init_cb      = _init_cb,
    ._deinit_cb    = _deinit_cb,
    ._event_cb     = _event_cb,
    .type          = OBJ_TYPE,
    .size_style    = sizeof(_gt_wor_dart_st)
};


/* macros ---------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/
static int _gt_wordart_items_find(_gt_wor_dart_st * style, uint32_t encode) {
    for (uint32_t i = 0; i < style->item_number; i++) {
        if(encode == style->items[i].encode){
            return i;
        }
    }
    return -1;
}

/**
 * @brief obj init wordart widget call back
 *
 * @param obj
 */
static void _init_cb(gt_obj_st * obj) {
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )obj;
    if(0 == obj->area.w || 0 == obj->area.h){
        obj->area.w = 5;
        obj->area.h = 5;
    }

    gt_area_st area = {
        .x = obj->area.x,
        .y = obj->area.y,
        .w = obj->area.w,
        .h = obj->area.h
    };

    if(NULL == style->items || 0 == style->item_number){
        return ;
    }

    gt_attr_rect_st dsc = {
        .bg_opa = obj->opa,
    };

    uint32_t len = (NULL == style->text) ? 0 : strlen(style->text);
    uint32_t idx = 0, encode = 0;
    int tmp = 0;

    while(idx < len) {
        tmp = _gt_utf8_to_unicode((uint8_t * )&style->text[idx], &encode);
        if (0 == tmp) {
            return ;
        }
        idx += tmp;
        tmp = _gt_wordart_items_find(style, encode);
        if (tmp < 0) {
            area.x += area.w + style->space_x;
            continue;
        }
        dsc.bg_img_src = style->items[tmp].img;
#if GT_USE_FILE_HEADER
        dsc.file_header = gt_file_header_param_check_valid(&style->items[tmp].fh);
#endif

#if GT_USE_DIRECT_ADDR
        dsc.addr = style->items[tmp].addr;
#endif

#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
        dsc.custom_addr = &style->items[tmp].custom_addr;
#endif

        draw_bg_img(obj->draw_ctx, &dsc, &area);
        area.x += area.w + style->space_x;
    }
}

static void _free_items(gt_obj_st * obj)
{
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )obj;
    if(NULL != style->items){
        for(int32_t i = style->item_number; i >= 0; i--) {
            if(NULL == style->items[i].img){
                continue;
            }
            gt_mem_free(style->items[i].img);
            style->items[i].img = NULL;
        }
        style->item_number = 0;

        gt_mem_free(style->items);
        style->items = NULL;
        style->total = 0;
    }
}

/**
 * @brief obj deinit call back
 *
 * @param obj
 */
static void _deinit_cb(gt_obj_st * obj) {
    GT_LOGV(GT_LOG_TAG_GUI, "wordart deinit");
    if (NULL == obj) {
        return ;
    }

    _free_items(obj);

    _gt_wor_dart_st * style_p = (_gt_wor_dart_st * )obj;
    if(NULL != style_p->text){
        gt_mem_free(style_p->text);
        style_p->text = NULL;
    }
}

/**
 * @brief obj event handler call back
 *
 * @param obj
 * @param e event
 */
static void _event_cb(struct gt_obj_s * obj, gt_event_st * e) {
    gt_event_type_et code_val = gt_event_get_code(e);
    switch(code_val) {
        case GT_EVENT_TYPE_DRAW_START:
            gt_disp_invalid_area(obj);
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_END, NULL);
            break;

        case GT_EVENT_TYPE_UPDATE_VALUE:
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_START, NULL);
            break;

        case GT_EVENT_TYPE_INPUT_PRESSING:   /* add clicking style and process clicking event */
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_START, NULL);
            break;

        case GT_EVENT_TYPE_INPUT_RELEASED: /* click event finish */
            gt_event_send(obj, GT_EVENT_TYPE_DRAW_START, NULL);
            break;

        default:
            break;
    }
}


/* global functions / API interface -------------------------------------*/
/**
 * @brief create a wordart obj
 *
 * @param parent wordart's parent element
 * @return gt_obj_st * wordart obj
 */
gt_obj_st * gt_wordart_create(gt_obj_st * parent)
{
    gt_obj_st * obj = gt_obj_class_create(MY_CLASS, parent);
    return obj;
}


uint32_t gt_wordart_items_init(gt_obj_st * wordart, uint32_t total)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return 0;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;

    if (NULL == style->items) {
        style->item_number = 0;
        style->items = (_gt_word_art_item_st * )gt_mem_malloc(total * sizeof(_gt_word_art_item_st));
    } else {
        style->items = (_gt_word_art_item_st * )gt_mem_realloc(style->items, total * sizeof(_gt_word_art_item_st));
    }
    style->total = total;

    if (NULL == style->items) {
        style->total = 0;
    }
    if (style->total < style->item_number) {
        style->item_number = style->total;
    }
    return style->total;
}

void gt_wordart_add_item(gt_obj_st * wordart, uint32_t encode, char * src)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    if (NULL == src) {
        return ;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    if(NULL == style->items || style->item_number >= style->total){
        return ;
    }

    uint32_t len = src ? strlen(src) : 0;
    style->items[style->item_number].img = gt_mem_malloc(len + 1);
    if(NULL == style->items[style->item_number].img){
        return ;
    }
    gt_memcpy(style->items[style->item_number].img, src, len);
    style->items[style->item_number].img[len] = '\0';
    style->items[style->item_number].encode = encode;
#if GT_USE_FILE_HEADER
    gt_file_header_param_init(&style->items[style->item_number].fh);
#endif
#if GT_USE_DIRECT_ADDR
    gt_hal_direct_addr_init(&style->items[style->item_number].addr);
#endif
#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
    gt_hal_custom_size_addr_init(&style->items[style->item_number].custom_addr);
#endif
    ++style->item_number;
}

#if GT_USE_FILE_HEADER
void gt_wordart_add_item_by_file_header(gt_obj_st * wordart, uint32_t encode, gt_file_header_param_st const * fh)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    if (NULL == fh) {
        return ;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    if (NULL == style->items || style->item_number >= style->total) {
        return ;
    }
    style->items[style->item_number].img = NULL;
    style->items[style->item_number].fh = *fh;
    style->items[style->item_number].encode = encode;
#if GT_USE_DIRECT_ADDR
    gt_hal_direct_addr_init(&style->items[style->item_number].addr);
#endif
#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
    gt_hal_custom_size_addr_init(&style->items[style->item_number].custom_addr);
#endif
    ++style->item_number;
}
#endif

#if GT_USE_DIRECT_ADDR
void gt_wordart_add_item_by_direct_addr(gt_obj_st * wordart, uint32_t encode, gt_addr_t addr)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    if (gt_hal_is_invalid_addr(addr)) {
        return;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    if (NULL == style->items || style->item_number >= style->total) {
        return ;
    }
    style->items[style->item_number].img = NULL;
    style->items[style->item_number].addr = addr;
    style->items[style->item_number].encode = encode;
#if GT_USE_FILE_HEADER
    gt_file_header_param_init(&style->items[style->item_number].fh);
#endif
    ++style->item_number;
}
#endif

#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
void gt_wordart_add_item_by_custom_size_addr(gt_obj_st * wordart, uint32_t encode, gt_direct_addr_custom_size_st * dac)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    if (gt_hal_is_invalid_custom_size_addr(dac)) {
        return;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    if (NULL == style->items || style->item_number >= style->total) {
        return ;
    }
    style->items[style->item_number].img = NULL;
    style->items[style->item_number].encode = encode;
    style->items[style->item_number].custom_addr = *dac;
#if GT_USE_FILE_HEADER
    gt_file_header_param_init(&style->items[style->item_number].fh);
#endif
#if GT_USE_DIRECT_ADDR
    gt_hal_direct_addr_init(&style->items[style->item_number].addr);
#endif
    ++style->item_number;
}
#endif
void gt_wordart_remove_all_items(gt_obj_st * wordart)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    _free_items(wordart);
}

uint32_t gt_wordart_get_items_count(gt_obj_st * wordart)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return 0;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    return style->item_number;
}

void gt_wordart_set_text(gt_obj_st * wordart, const char * fmt, ...)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    char buffer[8] = {0};
    va_list args;
    va_list args2;

    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;

    va_start(args, fmt);
    va_copy(args2, args);
    uint16_t size = (NULL == fmt) ? 0 : (vsnprintf(buffer, sizeof(buffer), fmt, args) + 1);
    va_end(args);
    if (!size) {
        goto free_lb;
    }

    if (NULL == style->text) {
        style->text = gt_mem_malloc(size);
    } else if (size != strlen(style->text) + 1) {
        style->text = gt_mem_realloc(style->text, size);
    }
    if (NULL == style->text) {
        goto free_lb;
    }
    gt_memset(style->text, 0, size);
    va_start(args2, fmt);
    vsnprintf(style->text, size, fmt, args2);
    va_end(args2);

    gt_event_send(wordart, GT_EVENT_TYPE_DRAW_START, NULL);

    return ;

free_lb:
    va_end(args2);
}

void gt_wordart_set_text_by_len(gt_obj_st * wordart, const char * text, uint16_t len)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    if (NULL == style->text) {
        style->text = gt_mem_malloc(len + 1);
    } else if (len != strlen(style->text)) {
        style->text = gt_mem_realloc(style->text, len + 1);
    }
    if (NULL == style->text) {
        return;
    }
    gt_memcpy(style->text, text, len);
    style->text[len + 1] = '\0';
    gt_event_send(wordart, GT_EVENT_TYPE_DRAW_START, NULL);
}

char * gt_wordart_get_text(gt_obj_st * wordart)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return NULL;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    return style->text;
}

void gt_wordart_set_space(gt_obj_st * wordart, uint8_t space_x, uint8_t space_y)
{
    if (false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;
    style->space_x = space_x;
    style->space_y = space_y;
}

void gt_wordart_set_number(gt_obj_st * wordart, double number,
                            uint8_t integer_len, uint8_t decimal_len,
                            bool is_float)
{
    if( false == gt_obj_is_type(wordart, OBJ_TYPE)) {
        return ;
    }
    _gt_wor_dart_st * style = (_gt_wor_dart_st * )wordart;

    char buffer[16] = {0};
    uint8_t idx = 0;
    uint8_t append_len = decimal_len ? decimal_len + 1 : decimal_len;

    buffer[idx++] = '%';

    if(is_float){
        char tmp_str[50] = {0};
        sprintf(buffer + idx, "%d.%dlf", integer_len + append_len, decimal_len);
        sprintf(tmp_str, buffer, number);
        char* d_dot = strchr(tmp_str, '.');
        if(d_dot){
            if((d_dot - tmp_str) > integer_len){
                gt_wordart_set_text(wordart, &tmp_str[(d_dot - tmp_str) - integer_len]);
            }
            else{
                gt_wordart_set_text(wordart, tmp_str);
            }
        }
        else{
            uint8_t len = strlen(tmp_str);
            gt_wordart_set_text(wordart, &tmp_str[len - integer_len]);
        }
    }
    else{
        if(!decimal_len){
            gt_wordart_set_text(wordart, "%d", number);
            return ;
        }

        uint64_t n_power = 1;
        for(uint8_t i = 0; i < decimal_len; i++){
            n_power *= 10;
        }

        number = number / n_power;

        sprintf(buffer + idx, "%d.%dlf", integer_len, decimal_len);

        gt_wordart_set_text(wordart, buffer, number);
    }

}

#endif /* GT_CFG_ENABLE_WORDART */
/* end of file ----------------------------------------------------------*/


