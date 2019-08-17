/**
* @file st7793.h
 *
 */

#ifndef ST7793_H
#define ST7793_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define DISP_BUF_SIZE (LV_HOR_RES_MAX * 40)
//#define ST7793_DC   21
#define ST7793_RST  21
#define ST7793_BCKL 18

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void st7793_init(void);
void st7793_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*st7793_H*/
