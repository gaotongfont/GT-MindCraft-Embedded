/**
 * @file gt_qrcode.c
 * @author yongg
 * @brief
 * @version 0.1
 * @date 2022-08-24 16:37:09
 * @copyright Copyright (c) 2014-present, Company Genitop. Co., Ltd.
 */

/* include --------------------------------------------------------------*/
#include "gt_qrcode.h"

#if GT_CFG_ENABLE_QRCODE && defined(__GT_QRCODE_EN_)
#include "../core/gt_mem.h"
#include "../others/gt_log.h"
#include "../core/gt_draw.h"
#include "string.h"
#include "../core/gt_graph_base.h"
#include "../core/gt_obj_pos.h"
#include "../core/gt_disp.h"
#include "../font/gt_font.h"
#include "../others/gt_assert.h"

/* private define -------------------------------------------------------*/
#define OBJ_TYPE    GT_TYPE_QRCODE
#define MY_CLASS    &gt_qr_code_class


// buff size
#define _QRDataBuf_SIZE  (4072)//(4072)
#define _QRCodeDataBuf_SIZE   _QRDataBuf_SIZE  // (3706)

/* private typedef ------------------------------------------------------*/
typedef struct _gt_qr_code_s {
    gt_obj_st obj;
    uint16_t str_len;
    uint8_t * str;
    uint8_t ec_level;
    uint8_t mask_patt;
    gt_family_t version;
    gt_color_t fg_color;
}_gt_qr_code_st;


/* static variables -----------------------------------------------------*/
static void _init_cb(gt_obj_st * obj);
static void _deinit_cb(gt_obj_st * obj);
static void _event_cb(struct gt_obj_s * obj, gt_event_st * e);

static GT_ATTRIBUTE_RAM_DATA const gt_obj_class_st gt_qr_code_class = {
    ._init_cb      = _init_cb,
    ._deinit_cb    = _deinit_cb,
    ._event_cb     = _event_cb,
    .type          = OBJ_TYPE,
    .size_style    = sizeof(_gt_qr_code_st)
};


/* macros ---------------------------------------------------------------*/



/* static functions -----------------------------------------------------*/

static void _Data_ReMap(unsigned char* getdate,unsigned char* putdata, unsigned char width, unsigned char high, unsigned char style)
{
    unsigned int i,j,wbyte;
    unsigned char i_8;
    wbyte = (width+7)/8;
    if( style == 0 )	//竖置横排 转 横置横排 ( Y-->W )
    {
    for( i = 0; i < high; i++ )
        for( j = 0; j < width; j++ )
        {
            i_8 = i/8;
            if((*(putdata+i_8*width+j)&(0x01<<(i%8))) > 0)
                getdate[wbyte*i+j/8] |= (0x80>>(j%8));
            else
                getdate[wbyte*i+j/8] &= (~(0x80>>(j%8)));
        }
    }
    else if( style == 1 )	//竖置横排 (W--> Y)
    {
        for( i = 0; i < high; i++ )
            for( j = 0; j < width; j++ )
            {
            i_8 = i/8;
            if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
                getdate[i_8*width+j] |= (0x01<<(i%8));
            else
                getdate[i_8*width+j] &= (~(0x01<<(i%8)));
            }
    }
}

/**
 * @brief obj init obj widget call back
 *
 * @param obj
 */
static void _init_cb(gt_obj_st * obj) {
    _gt_qr_code_st * style = (_gt_qr_code_st * )obj;
    uint8_t *qr_data_buff = NULL;
    uint8_t *qr_code_buff = NULL;

    qr_data_buff = gt_mem_malloc(_QRDataBuf_SIZE);
    qr_code_buff = gt_mem_malloc(_QRCodeDataBuf_SIZE);
    if(!gt_qrcode_init(style->version , qr_data_buff , qr_code_buff))
    {
        GT_LOGE(GT_LOG_TAG_GUI , "qrcode init err!");
        goto err_ret;
    }

    gt_memset_0(qr_data_buff , _QRDataBuf_SIZE);
    gt_memset_0(qr_code_buff , _QRCodeDataBuf_SIZE);

    if(!gt_qrcode_get(Mode_Unicode , style->ec_level , style->mask_patt , style->str , style->str_len))
    {
        GT_LOGE(GT_LOG_TAG_GUI , "get qrcode data err!");
        goto err_ret;
    }

    /* base shape */
    gt_attr_rect_st rect_attr;
    gt_graph_init_rect_attr(&rect_attr);
    rect_attr.reg.is_fill   = 1;
    rect_attr.radius        = 0;
    rect_attr.bg_opa        = GT_OPA_100;
    rect_attr.border_width  = 0;
    rect_attr.fg_color      = gt_color_white();
    rect_attr.border_color  = gt_color_black();

    gt_area_st area;

    area.w = 4 * style->version + 17;
    area.h = ((area.w + 7) >> 3) << 3;
    obj->area.w = area.w + 8;
    obj->area.h = obj->area.w;

    if(  ((area.h * area.h) >> 3 ) > _QRCodeDataBuf_SIZE )
    {
        GT_LOGE(GT_LOG_TAG_GUI , "get qrcode buff size is small!");
        goto err_ret;
    }

    gt_memset_0(qr_code_buff , _QRCodeDataBuf_SIZE);
    _Data_ReMap(qr_code_buff,qr_data_buff, area.w, area.h, 0 );

    //
    gt_mem_free(qr_data_buff);
    qr_data_buff = NULL;

    gt_area_st box_area = gt_area_reduce(obj->area , 2);
    // draw background
    rect_attr.bg_color      = obj->bgcolor;
    draw_bg(obj->draw_ctx, &rect_attr, &box_area);

    area.w = ((area.w + 7) >> 3) << 3;
    // GT_LOGD( GT_LOG_TAG_GUI,"qrcode w = %d , h = %d" , area.w , area.h);
    // draw the qr code
    rect_attr.bg_color      = style->fg_color;
    rect_attr.gray = 1;
    rect_attr.data_gray = qr_code_buff;
    area.x = box_area.x+2;
    area.y = box_area.y+2;
    // GT_LOGD(GT_LOG_TAG_UI , "barcode gray = %d" , rect_attr.gray);
    // 1:base shape
    draw_bg(obj->draw_ctx, &rect_attr, &area);

    // focus
    draw_focus(obj);

err_ret:
    if(NULL != qr_data_buff)
    {
        gt_mem_free(qr_data_buff);
        qr_data_buff = NULL;
    }
    if(NULL != qr_code_buff)
    {
        gt_mem_free(qr_code_buff);
        qr_code_buff = NULL;
    }
}

/**
 * @brief obj deinit call back
 *
 * @param obj
 */
static void _deinit_cb(gt_obj_st * obj) {
    GT_LOGV(GT_LOG_TAG_GUI, "start deinit_cb");
    /* code */
}


/**
 * @brief obj event handler call back
 *
 * @param obj
 * @param e event
 */
static void _event_cb(struct gt_obj_s * obj, gt_event_st * e) {
    gt_event_type_et code_val = gt_event_get_code(e);
    if (GT_EVENT_TYPE_DRAW_START == code_val) {
        gt_disp_invalid_area(obj);
        gt_event_send(obj, GT_EVENT_TYPE_DRAW_END, NULL);
    }
}

/* global functions / API interface -------------------------------------*/

/**
 * @brief create a qr_code obj
 *
 * @param parent qr_code's parent element
 * @return gt_obj_st* qr_code obj
 */
gt_obj_st * gt_qrcode_create(gt_obj_st * parent)
{
    gt_obj_st * obj = gt_obj_class_create(MY_CLASS, parent);
    if (NULL == obj) {
        return obj;
    }

    _gt_qr_code_st * style = (_gt_qr_code_st * )obj;
    const char * default_str = "https://www.hmi.gaotongfont.cn/";
    uint16_t len = strlen(default_str);

    style->str = gt_mem_malloc(len + 1);
    if (style->str) {
        gt_memcpy(style->str, default_str, len + 1);
        style->str_len = len;
    }
    style->version = GT_FAMILY_QRCODE_VERSION_10;
    style->ec_level = ECLevel_H;
    style->mask_patt = MaskPattern4;
    obj->bgcolor = gt_color_white();
    style->fg_color = gt_color_black();
    return obj;
}


void gt_qrcode_set_version(gt_obj_st * qr_code , gt_qr_code_version_em version)
{
    if (false == gt_obj_is_type(qr_code, OBJ_TYPE)) {
        return ;
    }
    _gt_qr_code_st * style = (_gt_qr_code_st * )qr_code;
    style->version = version;
}

void gt_qrcode_set_str(gt_obj_st * qr_code , char* str)
{
    if (false == gt_obj_is_type(qr_code, OBJ_TYPE)) {
        return ;
    }
    _gt_qr_code_st * style = (_gt_qr_code_st * )qr_code;
    if( NULL != style->str) {
        gt_mem_free(style->str);
    }
    uint16_t size = str == NULL ? 0 : strlen(str);
    style->str = gt_mem_malloc(size + 1);
    if (style->str) {
        gt_memcpy(style->str, str, size);
        style->str[size] = '\0';
    }
    // gt_event_send(barcode, GT_EVENT_TYPE_DRAW_START, NULL);
}

void gt_qrcode_set_background(gt_obj_st * qr_code , gt_color_t color)
{
    if (false == gt_obj_is_type(qr_code, OBJ_TYPE)) {
        return ;
    }
    gt_obj_set_bgcolor(qr_code, color);
}

void gt_qrcode_set_forecolor(gt_obj_st * qr_code , gt_color_t color)
{
    if (false == gt_obj_is_type(qr_code, OBJ_TYPE)) {
        return ;
    }
    _gt_qr_code_st * style = (_gt_qr_code_st * )qr_code;
    style->fg_color = color;
}

#endif  /** GT_CFG_ENABLE_QRCODE */
/* end ------------------------------------------------------------------*/
