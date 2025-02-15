/**
 * @file gt_hal_fs.c
 * @author Feyoung
 * @brief
 * @version 0.1
 * @date 2022-08-30 20:29:41
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "./gt_hal_fs.h"
#include "../core/gt_mem.h"
#include "../others/gt_log.h"


/* private define -------------------------------------------------------*/



/* private typedef ------------------------------------------------------*/



/* static variables -----------------------------------------------------*/



/* macros ---------------------------------------------------------------*/



/* class ----------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/



/* global functions / API interface -------------------------------------*/

gt_fs_fp_st * _gt_hal_fp_init(void)
{
    gt_fs_fp_st * fp = gt_mem_malloc(sizeof(gt_fs_fp_st));
    if (!fp) {
        GT_LOGE(GT_LOG_TAG_GUI, "malloc fp fail");
        return fp;
    }
    gt_memset_0(fp, sizeof(gt_fs_fp_st));
    return fp;
}

#if GT_USE_DIRECT_ADDR
void gt_hal_direct_addr_init(gt_addr_t * info)
{
    *info = GT_ADDR_INVALID;
}

bool gt_hal_is_invalid_addr(gt_addr_t addr)
{
    return addr == GT_ADDR_INVALID;
}
#endif  /** GT_USE_DIRECT_ADDR */

#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
void gt_hal_custom_size_addr_init(gt_direct_addr_custom_size_st * custom_addr)
{
    if (NULL == custom_addr) {
        return ;
    }
    custom_addr->addr = GT_ADDR_INVALID;
    custom_addr->w = 0;
    custom_addr->h = 0;
    custom_addr->is_alpha = 0;
}

bool gt_hal_is_invalid_custom_size_addr(gt_direct_addr_custom_size_st * custom_addr)
{
    return custom_addr->addr == GT_ADDR_INVALID;
}
#endif  /** GT_USE_DIRECT_ADDR_CUSTOM_SIZE */

/* end ------------------------------------------------------------------*/
