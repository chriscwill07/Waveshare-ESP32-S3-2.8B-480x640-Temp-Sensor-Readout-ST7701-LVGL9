//4-27-26 10:55pm Need to remove white border around main container 

#define LV_CONF_INCLUDE_SIMPLE
#include <Arduino.h>
#include <lvgl.h>
#include "LVGL_Driver.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define DS18B20_PIN 16  // change if needed

// DS18B20
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

//Screens 
lv_obj_t *main_scr;

// LVGL UI objects
static lv_obj_t *label_temp;
static lv_obj_t *label_gauge;

// Color variables (initialized at runtime)
static lv_color_t COLOR_GREEN;
static lv_color_t COLOR_ORANGE;
static lv_color_t COLOR_RED;

// Helper: convert 0xRRGGBB to lv_color_t using available LVGL API
static lv_color_t hex_to_lv_color(uint32_t hex)
{
#if defined(LV_COLOR_HEX)
  return LV_COLOR_HEX(hex);
#elif defined(LV_COLOR_MAKE)
  return LV_COLOR_MAKE((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
#elif defined(lv_color_make)
  return lv_color_make((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
#else
  // Fallback: best-effort store lower 16 bits (for typical 16-bit builds)
  lv_color_t c;
  c.full = (uint16_t)(hex & 0xFFFF);
  return c;
#endif
}

// Forward declaration
static void ui_create();
static void update_temperature_timer(lv_timer_t *timer);

//Custom Fonts stored in src/display
LV_FONT_DECLARE(tempfont200);
#define font200 &tempfont200

/********** Image Creation **********/
#ifdef __cplusplus
extern "C" {
#endif

//Declaring images for use 480px wide created with LVGL Online Image Converter 
extern const lv_image_dsc_t landcruiser480;
extern const lv_image_dsc_t engineicon;

#ifdef __cplusplus
}
#endif

// ---------------------- Driver functions ----------------------
void Driver_Init() {
  I2C_Init();
  TCA9554PWR_Init(0x00);   
  Set_EXIO(EXIO_PIN8,Low);
  Backlight_Init(); 

  Serial.println("drivers initialised");
}

void Driver_Loop(void *parameter)
{
    while (1)
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}


// ---------------------- UI helpers ----------------------
static void set_color_for_fahrenheit(float f)
{
  if (f <= 200.0f) {
    lv_obj_set_style_text_color(label_temp, COLOR_GREEN, 0);
    
  } else if (f <= 210.0f) { 
    lv_obj_set_style_text_color(label_temp, COLOR_ORANGE, 0); //need to fix this to be orange at correct temp 
    
  } else if (f >=210.1f) {
    lv_obj_set_style_text_color(label_temp, COLOR_RED, 0); //red at correct temp 
    
  }
}

static void show_disconnected()
{
  lv_label_set_text(label_temp, "--.-");
  lv_obj_set_style_text_color(label_temp, COLOR_RED, 0);

}

//*******************BUZZER for temp over 210°F******************************//



// LVGL timer callback: read sensor, update UI
static void update_temperature_timer(lv_timer_t *timer)
{
  (void)timer;
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC == DEVICE_DISCONNECTED_C) {
    show_disconnected();
    return;
  }

  float tempF = tempC * 9.0f / 5.0f + 32.0f;
  char buf[32];
  snprintf(buf, sizeof(buf), "%.1f", tempF);

  lv_label_set_text(label_temp, buf);
  set_color_for_fahrenheit(tempF);
}


/******************Create Basic UI********************/
static void ui_create()
{
    // Main container
lv_obj_t *main_cont = lv_obj_create(lv_scr_act());
lv_obj_set_size(main_cont, 480, 640);
lv_obj_set_style_bg_color(main_cont, lv_color_black(), 0);
lv_obj_set_style_bg_opa(main_cont, LV_OPA_COVER, 0);
lv_obj_set_style_clip_corner(main_cont, false, 0);
lv_obj_set_style_radius(main_cont, 0, 0);
lv_obj_center(main_cont);
const int border_px = 6;
lv_obj_set_style_border_width(main_cont, border_px, 0);
lv_obj_set_style_border_color(main_cont, lv_color_black(), 0);
lv_obj_set_style_border_opa(main_cont, LV_OPA_COVER, 0);


    // Create temp readout label as child of main_cont
    label_temp = lv_label_create(main_cont);  // Use the global/static label_temp
    lv_label_set_text(label_temp, "--.-");
    lv_obj_set_style_text_font(label_temp, font200, 0);
    lv_obj_set_style_text_align(label_temp, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_temp, LV_ALIGN_CENTER, -10, 0); //-10 moves to the right some and -60 moves up (portrait mode)

/**
    // Create engine temp label 
     label_gauge = lv_label_create(main_cont);
     lv_obj_set_width(label_gauge, 640);  // assuming this was meant to be label_gauge
     lv_obj_set_style_text_font(label_gauge, &lv_font_montserrat_48, 0);
     lv_obj_set_style_text_color(label_gauge, lv_color_white(), 0);  // 👈 Add this line
     lv_obj_set_style_text_align(label_gauge, LV_TEXT_ALIGN_CENTER, 0);
     lv_label_set_text(label_gauge, "Engine Temp");
     lv_obj_align(label_gauge, LV_ALIGN_CENTER, 0, 120);

      const int line_thickness = 6;

      //Top line
      lv_obj_t * top = lv_obj_create(main_cont);
      lv_obj_set_size(top, lv_obj_get_width(main_cont), line_thickness);
      lv_obj_set_style_bg_color(top, lv_color_white(), 0);
      lv_obj_set_style_border_width(top, 0, 0); // no border on the line
      lv_obj_align(top, LV_ALIGN_TOP_MID, 0, 0);

      //Bottom line
       lv_obj_t * bottom = lv_obj_create(main_cont);
       lv_obj_set_size(bottom, lv_obj_get_width(main_cont), line_thickness);
       lv_obj_set_style_bg_color(bottom, lv_color_white(), 0);
       lv_obj_set_style_border_width(bottom, 0, 0);
       lv_obj_align(bottom, LV_ALIGN_BOTTOM_MID, 0, 0);

      //Give children space if needed
      //lv_obj_set_style_pad_top(main_cont, line_thickness, 0);
      //lv_obj_set_style_pad_bottom(main_cont, line_thickness, 0);**/
}

/***Create: Landcrusier Logo and Engine Icon***/
void create_image() {
    // Landcruiser Logo 
    lv_obj_t * img = lv_img_create(lv_scr_act());   // lv_img_create in LVGL9
    // set the object's source to the descriptor (pass the address)
    lv_img_set_src(img, &landcruiser480);
    // position
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 100);
    lv_image_set_scale(img, 225);

     // EngineIcon
    lv_obj_t * img2 = lv_img_create(lv_scr_act());   // lv_img_create in LVGL9
    // set the object's source to the descriptor (pass the address)
    lv_img_set_src(img2, &engineicon);
    // position
    lv_obj_align(img2, LV_ALIGN_CENTER, 0, 180);
    lv_image_set_scale(img2, 175);
}

// ---------------------- Arduino setup / loop ----------------------


void setup() { 

  // Init color constants using helper that works across LVGL versions
  COLOR_GREEN  = hex_to_lv_color(0x00C853);
  COLOR_ORANGE = hex_to_lv_color(0xFF9800);
  COLOR_RED    = hex_to_lv_color(0xFF1800);

  // Initialize drivers and display
  Driver_Init();
  LCD_Init();    // initialize display (as in your original)
  initLVGL();   // initialize LVGL (and register display driver)

  // Start DS18B20
  sensors.begin();

  Serial.begin(115200);


  // Create the temperature UI (assumes LVGL is initialized)
  ui_create();
  create_image();

  // Create LVGL timer to update the temperature every 1000 ms
  lv_timer_create(update_temperature_timer, 1000, NULL);
  
}



void loop() 
  {
  loopLVGL(); // existing LVGL handling in your framework
  delay(5);
  }





