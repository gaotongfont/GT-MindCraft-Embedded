/**
 * @file gt_fs.c
 * @author yongg
 * @brief file system implementation for hal layer devices.
 * @version 0.1
 * @date 2022-07-05 19:15:15
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "gt_fs.h"
#include "../others/gt_color.h"
#include "../others/gt_log.h"
#include "ctype.h"
#include "string.h"

/* private define -------------------------------------------------------*/



/* private typedef ------------------------------------------------------*/

typedef struct _gt_vf_type_label_s {
    gt_fs_type_et type;
    gt_fs_label_et label;
}_gt_vf_type_label_st;

typedef struct _gt_fs_dsc_s {
    gt_fs_drv_st * drv;
    gt_fs_fp_st * fp;
}_gt_fs_dsc_st;

/* static variables -----------------------------------------------------*/

/**
 * @brief default drive label
 */
static GT_ATTRIBUTE_RAM_DATA const _gt_vf_type_label_st _drive_label[] = {
#if GT_USE_MODE_FLASH
    { GT_FS_TYPE_FLASH, GT_FS_LABEL_FLASH },
#endif
#if GT_USE_MODE_SD
    { GT_FS_TYPE_SD,    GT_FS_LABEL_SD    },
#endif
#if GT_USE_MODE_SRC
    { GT_FS_TYPE_ARRAY, GT_FS_LABEL_ARRAY },
#endif
};


/* macros ---------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/

/**
 * @brief get file system type
 *
 * @param index
 * @return gt_fs_type_et
 */
static GT_ATTRIBUTE_RAM_TEXT inline gt_fs_type_et _gt_fs_get_type(gt_size_t index) {
    return _drive_label[index].type;
}

/**
 * @brief get file system label
 *
 * @param index
 * @return gt_fs_label_et
 */
static GT_ATTRIBUTE_RAM_TEXT inline gt_fs_label_et _gt_fs_get_label(gt_size_t index) {
    return _drive_label[index].label;
}

static GT_ATTRIBUTE_RAM_TEXT inline gt_fs_drv_st * _get_default_drv_by(gt_fs_type_et type) {
#if GT_USE_MODE_FLASH
    if (GT_FS_TYPE_FLASH == type) {
        return gt_vf_get_drv();
    }
#endif

#if GT_USE_MODE_SD
    if (GT_FS_TYPE_SD == type) {
        return NULL;
    }
#endif

#if GT_USE_MODE_SRC
    if (GT_FS_TYPE_ARRAY == type) {
        return gt_src_get_drv();
    }
#endif
    return NULL;
}

/**
 * @brief get file system driver, according to letters
 *          @ref _drive_label array
 *
 * @param path full path
 * @return gt_fs_drv_st* the driver of file system
 */
static GT_ATTRIBUTE_RAM_TEXT inline gt_fs_drv_st * _get_default_drv(char const * path) {
    gt_fs_type_et type = gt_fs_get_src_type(path);

    return _get_default_drv_by(type);
}


/* global functions / API interface -------------------------------------*/

const char * gt_fs_get_ext(const char * path)
{
    size_t i;
    for (i = strlen(path); i > 0; i--) {
        if ('.' == path[i]) {
            return &path[i + 1];
        }
        else if ('/' == path[i] || '\\' == path[i] || ':' == path[i]) {
            return "";
        }
    }
    return "";
}

gt_fs_type_et gt_fs_get_src_type(const void * src)
{
    gt_fs_type_et ret = GT_FS_TYPE_UNKNOWN;
    char drive_label = tolower(*(uint8_t * )src);
    gt_size_t len = sizeof(_drive_label) / sizeof(_gt_vf_type_label_st);

    while (len--) {
        if (_gt_fs_get_label(len) == drive_label) {
            ret = _gt_fs_get_type(len);
            break;
        }
    }
    return ret;
}

gt_fs_fp_st * gt_fs_open(const char * path, gt_fs_mode_et mode)
{
    gt_fs_drv_st * drv = _get_default_drv(path);

    if (!drv || !drv->open_cb) {
        return NULL;
    }
    return (gt_fs_fp_st * )drv->open_cb(drv, (char *)path, mode);
}

gt_fs_fp_st * gt_fs_open_raw(const char * raw_data, uint32_t raw_len, gt_fs_mode_et mode)
{
    gt_fs_drv_st * drv = gt_src_get_drv();

    if (!drv || !drv->open_cb_raw) {
        return NULL;
    }
    return (gt_fs_fp_st * )drv->open_cb_raw(drv, (uint8_t *)raw_data, raw_len, mode);
}

#if GT_USE_FILE_HEADER
gt_fs_fp_st * gt_fs_fh_open(gt_file_header_param_st const * const fh_param, gt_fs_mode_et mode)
{
    gt_fs_drv_st * drv = gt_vf_get_drv();
    if (!drv || !drv->fh_open_cb) {
        return NULL;
    }
    gt_fs_fp_st * fp = drv->fh_open_cb(drv, fh_param, mode);

    return fp;
}
#endif

#if GT_USE_DIRECT_ADDR
gt_fs_fp_st * gt_fs_direct_addr_open(gt_addr_t addr, gt_fs_mode_et mode)
{
    gt_fs_drv_st * drv = gt_vf_get_drv();
    if (!drv || !drv->direct_addr_open_cb) {
        return NULL;
    }
    gt_fs_fp_st * fp = drv->direct_addr_open_cb(drv, addr, mode);
    return fp;
}
#endif


#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
gt_fs_fp_st * gt_fs_custom_size_addr_open(gt_direct_addr_custom_size_st * dac, gt_fs_mode_et mode)
{
    gt_fs_drv_st * drv = gt_vf_get_drv();
    if (!drv || !drv->custom_size_addr_open_cb) {
        return NULL;
    }
    gt_fs_fp_st * fp = drv->custom_size_addr_open_cb(drv, dac, mode);
}
#endif


gt_fs_res_et gt_fs_read(gt_fs_fp_st * fp, uint8_t * out, uint32_t size, uint32_t * ret_len)
{
    gt_fs_drv_st * drv = fp->drv;
    if (!fp || !drv->read_cb) {
        return GT_FS_RES_FAIL;
    }

    return drv->read_cb(drv, fp, out, size, ret_len);
}

gt_fs_res_et gt_fs_seek(gt_fs_fp_st * fp, uint32_t offset, gt_fs_whence_et whence)
{
    gt_fs_drv_st * drv = fp->drv;
    if (!fp || !drv->seek_cb) {
        return GT_FS_RES_FAIL;
    }

    return drv->seek_cb(drv, fp, offset, whence);
}

gt_fs_res_et gt_fs_tell(gt_fs_fp_st * fp, uint32_t * pos)
{
    gt_fs_drv_st * drv = fp->drv;
    if (!fp || !drv->tell_cb) {
        return GT_FS_RES_FAIL;
    }

    return drv->tell_cb(drv, fp, pos);
}

gt_fs_res_et gt_fs_write(gt_fs_fp_st * fp, uint8_t * buffer, uint32_t len, uint32_t * ret_len)
{
    gt_fs_drv_st * drv = fp->drv;
    if (!fp || !drv->write_cb) {
        return GT_FS_RES_FAIL;
    }

    return drv->write_cb(drv, fp, buffer, len, ret_len);
}

gt_fs_res_et gt_fs_read_img_offset(gt_fs_fp_st * fp, uint8_t * res, uint32_t offset, uint32_t len)
{
    gt_fs_drv_st * drv = fp->drv;
    if (!fp) {
        return GT_FS_RES_FAIL;
    }
    uint32_t ret_len = 0;

    if (!fp || !drv->seek_cb || !drv->read_cb) {
        return GT_FS_RES_FAIL;
    }
    drv->seek_cb(drv, fp, offset, GT_FS_SEEK_SET);
    drv->read_cb(drv, fp, res, len, &ret_len);

    return GT_FS_RES_OK;
}

gt_fs_res_et gt_fs_read_img_wh(const char * path, uint16_t * w, uint16_t * h)
{
    if( !path ){
        return GT_FS_RES_FAIL;
    }
    gt_fs_fp_st * fp = gt_fs_open(path, GT_FS_MODE_RD);
    if (!fp) {
        return GT_FS_RES_FAIL;
    }
    *w = fp->msg.pic.w;
    *h = fp->msg.pic.h;

    gt_fs_close(fp);

    return GT_FS_RES_OK;
}

#if GT_USE_FILE_HEADER
gt_fs_res_et gt_fs_fh_read_img_wh(gt_file_header_param_st * fh, uint16_t * w, uint16_t * h)
{
    GT_CHECK_BACK_VAL(fh, GT_FS_RES_FAIL);
    if (fh->idx < 0) {
        return GT_FS_RES_FAIL;
    }
    gt_fs_fp_st * fp = gt_fs_fh_open(fh, GT_FS_MODE_RD);
    if (NULL == fp) {
        return GT_FS_RES_FAIL;
    }

    *w = fp->msg.pic.w;
    *h = fp->msg.pic.h;

    gt_fs_close(fp);

    return GT_FS_RES_OK;
}
#endif

#if GT_USE_DIRECT_ADDR
gt_fs_res_et gt_fs_direct_addr_read_img_wh(gt_addr_t addr, uint16_t * w, uint16_t * h)
{
    if (gt_hal_is_invalid_addr(addr)) {
        return GT_FS_RES_FAIL;
    }
    gt_fs_fp_st * fp = gt_fs_direct_addr_open(addr, GT_FS_MODE_RD);
    if (NULL == fp) {
        return GT_FS_RES_FAIL;
    }

    *w = fp->msg.pic.w;
    *h = fp->msg.pic.h;

    gt_fs_close(fp);

    return GT_FS_RES_OK;
}
#endif

#if GT_USE_DIRECT_ADDR_CUSTOM_SIZE
gt_fs_res_et gt_fs_direct_addr_custom_size_read_img_wh(gt_direct_addr_custom_size_st * custom_addr, uint16_t * w, uint16_t * h)
{
    if (gt_hal_is_invalid_custom_size_addr(custom_addr)) {
        return GT_FS_RES_FAIL;
    }
    gt_fs_fp_st * fp = gt_fs_custom_size_addr_open(custom_addr, GT_FS_MODE_RD);
    if (NULL == fp) {
        return GT_FS_RES_FAIL;
    }

    *w = fp->msg.pic.w;
    *h = fp->msg.pic.h;

    gt_fs_close(fp);

    return GT_FS_RES_OK;
}
#endif


uint32_t gt_fs_read_direct_physical(gt_addr_t addr, uint32_t len, uint8_t * data)
{
    gt_fs_drv_st * drv = gt_vf_get_drv();
    if (!drv) {
        return 0;
    }
    uint8_t data_write[5] = {GT_HAL_FLASH_READ_CMD, (uint8_t)(addr>>16), (uint8_t)(addr>>8), (uint8_t)addr };
    uint8_t len_write = 4;

    if (addr > 0xffffff) {
        data_write[1] = (uint8_t)(addr >> 24);
        data_write[2] = (uint8_t)(addr >> 16);
        data_write[3] = (uint8_t)(addr >> 8);
        data_write[4] = (uint8_t)addr;
        ++len_write;
    }

    /* start read */
#ifdef _GT_PORT_SIMULATOR_ENVS
    // TODO change to call back function,  do not force to use direct read
    extern unsigned long r_dat_bat(unsigned long address, unsigned long DataLen, unsigned char * pBuff);
    return r_dat_bat(addr, len, data);
#else
    return drv->rw_cb(data_write, len_write, data, len);
#endif
}

void gt_fs_close(gt_fs_fp_st * fp)
{
    GT_CHECK_BACK(fp);
    if (fp->drv->close_cb) {
        fp->drv->close_cb(fp->drv, fp);
    }
    fp = NULL;
}

#if GT_USE_FS_NAME_BY_INDEX
char const * const gt_fs_get_name_by_index(gt_fs_type_et file_type, uint16_t index_of_list)
{
    gt_fs_drv_st * drv = _get_default_drv_by(file_type);
    GT_CHECK_BACK_VAL(drv, NULL);
    GT_CHECK_BACK_VAL(drv->get_name_by_cb, NULL);
    return drv->get_name_by_cb(index_of_list);
}
#endif

/* end ------------------------------------------------------------------*/
