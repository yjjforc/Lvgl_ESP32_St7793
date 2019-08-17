/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "lvgl/lvgl.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "esp_freertos_hooks.h"
#include "LED_PWM.h"

#include "drv/disp_spi.h"
#include "drv/st7793.h"
#include "drv/tp_spi.h"
#include "drv/xpt2046.h"

#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#define Main_LOG           "Main_LOG"


static void IRAM_ATTR lv_tick_task(void);

void app_main()
{
	lv_init();

	disp_spi_init();
	st7793_init();
    LED_PWM_Init();
	tp_spi_init();
    xpt2046_init();
    static lv_color_t buf1[DISP_BUF_SIZE+1];
    static lv_color_t buf2[DISP_BUF_SIZE+1];
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, &buf1[1], &buf2[1], DISP_BUF_SIZE);


	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = st7793_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);
    disp_drv.rotated=0;
    
    ESP_LOGI(Main_LOG, "buf1 is %d",(int)&buf1);
    ESP_LOGI(Main_LOG, "buf2 is %d",(int)&buf2);
    // Set TOUCH_SUPPORT on drv\component.mk to 1 if
    // your board have touch support
#if ENABLE_TOUCH_INPUTTAG
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = xpt2046_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#endif

	esp_register_freertos_tick_hook(lv_tick_task);

	demo_create();

        while(1) {
        	vTaskDelay(20);
        	lv_task_handler();
        }
}

static void IRAM_ATTR lv_tick_task(void)
{
	lv_tick_inc(portTICK_RATE_MS);
}
