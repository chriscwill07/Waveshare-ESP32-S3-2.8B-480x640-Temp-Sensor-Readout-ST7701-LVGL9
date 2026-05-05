#pragma once

#include <lvgl.h>
#include "lv_conf.h"
#include <esp_heap_caps.h>
#include "Display_ST7701.h"

#define LCD_WIDTH     ESP_PANEL_LCD_WIDTH
#define LCD_HEIGHT    ESP_PANEL_LCD_HEIGHT
#define BYTE_PER_PIXEL  2 



void initLVGL(void);
void loopLVGL(void);

