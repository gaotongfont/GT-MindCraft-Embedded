/**
 * @file gt_wordart.h
 * @author Yang
 * @brief
 * @version 0.1
 * @date 2023-07-21 14:10:24
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */
#ifndef _GT_WORDART_H_
#define _GT_WORDART_H_

#ifdef __cplusplus
extern "C" {
#endif

/* include --------------------------------------------------------------*/
#include "gt_obj.h"
#include "gt_obj_class.h"
#include "stdarg.h"
#include "stdlib.h"
#include "gt_conf_widgets.h"

#if GT_CFG_ENABLE_WORDART

#if GT_USE_FILE_HEADER
#include "../hal/gt_hal_file_header.h"
#endif

#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
#include "../hal/gt_hal_fs.h"
#endif

/* define ---------------------------------------------------------------*/



/* typedef --------------------------------------------------------------*/



/* macros ---------------------------------------------------------------*/



/* global functions / API interface -------------------------------------*/
gt_obj_st * gt_wordart_create(gt_obj_st * parent);

/**
 * @brief Example Initialize the maximum number of items
 *
 * @param wordart obj
 * @param total maximum number
 * @return valid number count of items
 */
uint32_t gt_wordart_items_init(gt_obj_st* wordart, uint32_t total);

/**
 * @brief add item
 *
 * @param wordart obj
 * @param coding custom unicode
 * @param src image path
 */
void gt_wordart_add_item(gt_obj_st* wordart, uint32_t coding, char * src);

#if GT_USE_FILE_HEADER
/**
 * @brief add item by file header
 *
 * @param wordart
 * @param encode
 * @param fh idx -1[defalut]: Disabled file header, using img path to open file; >=0: index number, valid value
 *              < 0: invalid value.
 *           package_idx 0[defalut]: The index of element within item, as the first element
 */
void gt_wordart_add_item_by_file_header(gt_obj_st * wordart, uint32_t encode, gt_file_header_param_st const * fh);
#endif

#if GT_USE_DIRECT_ADDR
/**
 * @brief add item by direct address
 *
 * @param wordart
 * @param encode
 * @param addr
 */
void gt_wordart_add_item_by_direct_addr(gt_obj_st * wordart, uint32_t encode, gt_addr_t addr);
#endif

#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
/**
 * @brief add item by custom size direct address
 *
 * @param wordart
 * @param encode
 * @param dac custom size direct address
 */
void gt_wordart_add_item_by_custom_size_addr(gt_obj_st * wordart, uint32_t encode, gt_direct_addr_custom_size_st * dac);
#endif

void gt_wordart_remove_all_items(gt_obj_st * wordart);
uint32_t gt_wordart_get_items_count(gt_obj_st * wordart);

/**
 * @brief Set the text, which display by wordart image character
 *
 * @param wordart
 * @param fmt
 */
void gt_wordart_set_text(gt_obj_st * wordart, const char * fmt, ...);

void gt_wordart_set_text_by_len(gt_obj_st * wordart, const char * text, uint16_t len);

char * gt_wordart_get_text(gt_obj_st * wordart);

void gt_wordart_set_space(gt_obj_st * wordart, uint8_t space_x, uint8_t space_y);

void gt_wordart_set_number(gt_obj_st * wordart, double number,
                            uint8_t integer_len, uint8_t decimal_len,
                            bool is_float);
#endif /* GT_CFG_ENABLE_WORDART */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //!_GT_WORDART_H_

/* end of file ----------------------------------------------------------*/


