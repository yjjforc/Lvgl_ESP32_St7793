# Lvgl_ESP32_St7793
基于https://github.com/littlevgl/esp32_ili9341.git lvgl+esp32+9341   稍微修改支持st7793芯片驱动400*240


1.将dip_spi,st7793 文件放入 drv文件中替换
2.将main文件替换根目录main文件
3 更改（lv_conf.h）屏幕分辨率
#define LV_HOR_RES_MAX          (400)
#define LV_VER_RES_MAX          (240)
