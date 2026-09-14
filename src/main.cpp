/*******************************************************************************
 * LVGL Widgets
 * This is a widgets demo for LVGL - Light and Versatile Graphics Library
 * import from: https://github.com/lvgl/lv_demos.git
 *
 * Dependent libraries:
 * LVGL: https://github.com/lvgl/lvgl.git
 *
 * LVGL Configuration file:
 * Copy your_arduino_path/libraries/lvgl/lv_conf_template.h
 * to your_arduino_path/libraries/lv_conf.h
 *
 * In lv_conf.h around line 15, enable config file:
 * #if 1 // Set it to "1" to enable content
 *
 * Then find and set:
 * #define LV_COLOR_DEPTH     16
 * #define LV_TICK_CUSTOM     1
 *
 * For SPI/parallel 8 display set color swap can be faster, parallel 16/RGB screen don't swap!
 * #define LV_COLOR_16_SWAP   1 // for SPI and parallel 8
 * #define LV_COLOR_16_SWAP   0 // for parallel 16 and RGB
 *
 * Enable LVGL Demo Widgets
 * #define LV_USE_DEMO_WIDGETS 1
 ******************************************************************************/
#include "ui.h"
// #define DIRECT_MODE // Uncomment to enable full frame buffer

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

// #define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */


#define GFX_BL 32
Arduino_DataBus *bus = new Arduino_ESP32SPI(2, 15, 18, 23, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_ILI9342        (bus, 4, 1 /* rotation */, true /* IPS */);
#define CANVAS


#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/*******************************************************************************
 * Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.hpp"

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static uint32_t bufSize;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
#ifndef DIRECT_MODE
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif
#endif // #ifndef DIRECT_MODE

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

#include "touch.cpp"
#include <esp32Servo.h>
#include "sensor.h"

// ------------ ประกาศสร้างตัวแปรทั้งหมด ------------
const int pingPin = 5; 
int inPin = 13;        
int ledPin = 25;       
int digitalPin = 33;   
int val = 0;
int buzzer = 14;       
int digitalPin2 = 16;
int servoPin = 26;     
// ---------------------------------------------------

Servo doorServo;

const int OPEN_ANGLE   = 0;
const int CLOSED_ANGLE = 90;
bool isDoorOpen = true; 

// ----------------- ตั้งค่าระยะห่างของรถ -----------------
const int DISTANCE_THRESHOLD = 20; 
// ----------------------------------------------------

// ----------------- ระบบรหัสผ่าน 3 แบบและเวลา -----------------
const int numPasswords = 3;
String savedPasswords[numPasswords] = {"6717780"}; 

unsigned long doorOpenTime = 0; 
const unsigned long autoCloseDelay = 3000; 
// --------------------------------------------------------
// ฟังก์ชันนี้จะทำงานเมื่อมีการพิมพ์รหัสและกดปุ่ม ENTER บนจอ
// ฟังก์ชันควบคุมปุ่มตัวเลข
void ui_password_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code != LV_EVENT_VALUE_CHANGED)
        return;

    lv_obj_t *btnmatrix = lv_event_get_target(e);

    // หาว่ากดปุ่มไหน
    uint16_t id = lv_btnmatrix_get_selected_btn(btnmatrix);

    // อ่านข้อความของปุ่มที่กด
    const char *btnText = lv_btnmatrix_get_btn_text(btnmatrix, id);

    if (btnText == NULL)
        return;

    Serial.print("Button: ");
    Serial.println(btnText);

    // -------------------------
    // ถ้ากดตัวเลข
    // -------------------------
    if ((btnText[0] >= '0' && btnText[0] <= '9') && btnText[1] == '\0')
    {
        // ดึงข้อความปัจจุบันในช่อง
        const char *oldText = lv_textarea_get_text(objects.enterpass);

        String newText = String(oldText) + String(btnText);

        // ใส่ตัวเลขลง TextArea
        lv_textarea_set_text(objects.enterpass, newText.c_str());
    }

    // -------------------------
    // ถ้ากด CLEAR
    // -------------------------
    else if (strcmp(btnText, "CLEAR") == 0)
    {
        lv_textarea_set_text(objects.enterpass, "");

        Serial.println("Password Cleared");
    }

    // -------------------------
    // ถ้ากด ENTER
    // -------------------------
    else if (strcmp(btnText, "ENTER") == 0)
    {
        const char *txt = lv_textarea_get_text(objects.enterpass);

        String inputCode = String(txt);

        Serial.print("Entered Code: ");
        Serial.println(inputCode);

        // ตรวจสอบรหัส
        bool isPasswordCorrect = false;

        for (int i = 0; i < numPasswords; i++)
        {
            if (inputCode == savedPasswords[i])
            {
                isPasswordCorrect = true;
                break;
            }
        }

        // -------------------------
        // รหัสถูก
        // -------------------------
        if (isPasswordCorrect)
        {
            Serial.println("UI: Password Correct!");

            // เปิดประตู
            openDoor();
            doorOpenTime = millis();

            // ล้างช่องรหัส
            lv_textarea_set_text(objects.enterpass, "");

            // เปลี่ยนไปหน้าถัดไป
            loadScreen(SCREEN_ID_SLIDE2);
        }

        // -------------------------
        // รหัสผิด
        // -------------------------
        else
        {
            Serial.println("UI: Wrong Password!");

            // ล้างรหัสเพื่อกรอกใหม่
            lv_textarea_set_text(objects.enterpass, "");
        }
    }
}

void setup()
{
    Serial.begin(115200);

    setupSensors();

    pinMode(digitalPin2, INPUT);

    Serial.println("Arduino_GFX LVGL Widgets example");
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->setRotation(4);
  gfx->fillScreen(RGB565_BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

 
  
  // Init touch device
  touch_init(gfx->width(), gfx->height(), gfx->getRotation());

  lv_init();

  screenWidth = gfx->width();
  screenHeight = gfx->height();


  
#ifdef DIRECT_MODE
  bufSize = screenWidth * screenHeight;
#else
  bufSize = screenWidth * 40;
#endif

#ifdef ESP32
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * bufSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
  {
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * bufSize, MALLOC_CAP_8BIT);
  }
#else
  disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * bufSize);
#endif
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, bufSize);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
#ifdef DIRECT_MODE
    disp_drv.direct_mode = true;
#endif
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);


//    gfx->fillScreen(RED);
//    gfx->flush();
//    //delay(1000);
//    gfx->fillScreen(GREEN);
//    gfx->flush();
//    //delay(1000);
//    gfx->fillScreen(BLUE);
//    gfx->flush();
//    //delay(1000);
    
ui_init();

Serial.println("Setup done");

// รับ Event จาก ButtonMatrix
lv_obj_add_event_cb(
    objects.botton,
    ui_password_event_cb,
    LV_EVENT_VALUE_CHANGED,
    NULL
);
  }
}

void loop()
{
  // 1 & 2. การอ่านค่าระยะทางจากเซ็นเซอร์และอัพเดทเวลารถขวาง
  long currentDistance = readDistance();

  // 3. อ่านค่าเซ็นเซอร์เคลื่อนไหว ควบคุมไฟ LED
  handleMotionSensor();

  // 4. ส่วนตรวจสอบรหัสผ่าน (ส่งค่าระยะทางไปเช็คเงื่อนไขด้วย)
  checkPasswordSystem(currentDistance);
  
  // 5. ตรวจสอบการปิดประตูอัตโนมัติ
  checkAutoClose();  
  lv_timer_handler(); /* let the GUI do its work */

  // อ่านค่าจากพิน IR ทั้ง 2 ตัว
  int irValue = digitalRead(digitalPin); 
  int irValue2 = digitalRead(digitalPin2); 

  // แสดงค่าออกทาง Serial Monitor เพื่อตรวจสอบ
  Serial.print("Distance: ");
  Serial.print(currentDistance);
  Serial.print(" | digitalPin (Garage 1): ");
  Serial.print(irValue);
  Serial.print(" | digitalPin2 (Garage 2): ");
  Serial.println(irValue2);

  // ถ้าเจอรถ (IR เป็น HIGH) หรือถ้าระยะอยู่ในเกณฑ์ (Garage 1)
  if (irValue == 1 ) {
      lv_obj_clear_flag(objects.ledg, LV_OBJ_FLAG_HIDDEN);  // โชว์ไฟแดง (ledg)
      lv_obj_add_flag(objects.ledr, LV_OBJ_FLAG_HIDDEN);    // ซ่อนไฟเขียว (ledr)
  } 
  // ถ้าไม่มีรถ (IR เป็น LOW)
  else {
      lv_obj_add_flag(objects.ledg, LV_OBJ_FLAG_HIDDEN);    // ซ่อนไฟแดง
      lv_obj_clear_flag(objects.ledr, LV_OBJ_FLAG_HIDDEN);  // โชว์ไฟเขียว
  }

  // ถ้าเจอรถ (IR เป็น HIGH) หรือถ้าระยะอยู่ในเกณฑ์ (Garage 2)
  if (irValue2 == 1) {
      lv_obj_clear_flag(objects.ledgreen, LV_OBJ_FLAG_HIDDEN);  // โชว์ไฟแดง 
      lv_obj_add_flag(objects.ledred, LV_OBJ_FLAG_HIDDEN);      // ซ่อนไฟเขียว 
  } 
  // ถ้าไม่มีรถ (IR เป็น LOW)
  else {
      lv_obj_add_flag(objects.ledgreen, LV_OBJ_FLAG_HIDDEN);    // ซ่อนไฟแดง
      lv_obj_clear_flag(objects.ledred, LV_OBJ_FLAG_HIDDEN);    // โชว์ไฟเขียว
  }

#ifdef DIRECT_MODE
#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(0, 0, (uint16_t *)disp_draw_buf, screenWidth, screenHeight);
#else
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)disp_draw_buf, screenWidth, screenHeight);
#endif
#endif // #ifdef DIRECT_MODE

#ifdef CANVAS
  gfx->flush();
#endif

  delay(1000);
}