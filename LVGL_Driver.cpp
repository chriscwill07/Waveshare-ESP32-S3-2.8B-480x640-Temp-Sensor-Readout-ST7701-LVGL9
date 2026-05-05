///complete 4-24-26 12:13

#include "LVGL_Driver.h"

lv_color_t *buf1 = NULL;
lv_color_t *buf2 = NULL;


void flushDisplay(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
    LCD_addWindow(area->x1, area->y1, area->x2, area->y2, color_p);
    lv_display_flush_ready(disp);
}


void initLVGL(void)
{
    lv_init();
    lv_tick_set_cb(xTaskGetTickCount);

    // --- Allocate buffers in PSRAM ---
    size_t buf_size_pixels = LCD_WIDTH * LCD_HEIGHT;
    size_t buf_size_bytes = buf_size_pixels * sizeof(lv_color_t);

    lv_color_t *buf1 = (lv_color_t*)heap_caps_malloc(buf_size_bytes, MALLOC_CAP_SPIRAM);
    lv_color_t *buf2 = (lv_color_t*)heap_caps_malloc(buf_size_bytes, MALLOC_CAP_SPIRAM);

    if (!buf1 || !buf2) {
        LV_LOG_ERROR("PSRAM allocation failed");
        // Optional: fallback to smaller internal RAM buffers
        return;
    }

    // --- Create display and assign buffers ---
    lv_display_t *disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);
    lv_display_set_buffers(disp, buf1, buf2, buf_size_bytes, LV_DISPLAY_RENDER_MODE_FULL);

    // --- Configure display ---
    lv_display_set_physical_resolution(disp, LCD_WIDTH, LCD_HEIGHT);
    lv_display_set_flush_cb(disp, flushDisplay);
    //lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);  // Uncomment if needed

}

void loopLVGL(void) {
  lv_timer_handler();

  vTaskDelay(pdMS_TO_TICKS(5));
}
