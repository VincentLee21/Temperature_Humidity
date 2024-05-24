
#include "Arduino.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include "BME280I2C.h"

#include "DEV_Debug.h"
#include "DEV_Config.h"

#ifdef ESP_IDF_VERSION_MAJOR // IDF 4+
#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/rtc.h"
#else
#error Target CONFIG_IDF_TARGET is not supported
#endif
#else // ESP32 Before IDF 4.0
#include "rom/rtc.h"
#endif

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

#define GFXFF 1

#define FF17 &FreeSans9pt7b
#define FF18 &FreeSans12pt7b
#define FF19 &FreeSans18pt7b
#define FF20 &FreeSans24pt7b

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);
volatile bool bUpdate = false;
// Ticker ticker;

float temp(NAN), hum(NAN), pres(NAN);
BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_Pa);

BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280I2C bme(settings);    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

void TaskRead_BME280(void *pvParameters);

// void tcr10s() {
//   // Serial.printf("SRAM fee size: %d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
//   Debug("Total heap: %d\n", ESP.getHeapSize());
//   Debug("Free heap: %d\n", ESP.getFreeHeap());
//   Debug("Total PSRAM: %d\n", ESP.getPsramSize());
//   Debug("Free PSRAM: %d\n\n\n", ESP.getFreePsram());
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 115200);
  Debug("\n\n\nESP32-S3 RoundTFT 1.28\n");

  // ticker.attach( 10, tcr10s);

  if( psramInit()) {
    Debug("PSRAM is correctly initialized\n");
  } else {
    Debug("PSRAM not available\n");
  }

  Debug( "Init LCD\n");
  // pinMode(2, OUTPUT);
  // digitalWrite(2, HIGH);
  ledcSetup(0, 500, 8);
  ledcAttachPin(2, 0);
  ledcWrite(0, 255);

  if (DEV_Module_Init() != 0)
    Serial.println("GPIO Init Fail!");
  else
    Serial.println("GPIO Init successful!");

  while( !bme.begin()) {
    Print( "BME280 device not found\n");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Debug("Found BME280 sensor! Success.\n");
       break;
     case BME280::ChipModel_BMP280:
       Debug("Found BMP280 sensor! No Humidity available.\n");
       break;
     default:
       Debug("Found UNKNOWN sensor! Error!\n");
  }

  // Setup LCD
  tft.init();
  tft.setRotation(0);
  img.createSprite(240, 240);
  img.setTextColor(TFT_BLACK, TFT_WHITE, true);
  img.fillScreen(TFT_WHITE);
  // img.pushSprite(0, 0);
  Debug( "Init LCD done\n");

  img.fillRect( 0, 0, 100, 100, TFT_GREEN);

  xTaskCreate(
    TaskRead_BME280
    ,  "Task IMU" // A name just for humans
    ,  4096        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,  0 // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,  1  // Priority
    ,  NULL // Task handle is not used here - simply pass NULL
    );

  img.pushSprite(0, 0);
}

void TaskRead_BME280(void *pvParameters) {
  // uint32_t iDelayUS = *((uint32_t*) pvParameters);

  while( 1) {
    bme.read(pres, temp, hum, tempUnit, presUnit);
    bUpdate = true;
    usleep(1000000); // 1s
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = 20, y, dy = 30;
  if( bUpdate) {
    img.fillScreen(TFT_WHITE);
    img.setFreeFont(FF19);

    y = 80;
    String str = String("T. ") + String( temp, 1) + String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F');
    img.drawString(str, x, y, GFXFF);
    y+=dy;
    str = String("H. ") + String( hum, 1) + String("RH");
    img.drawString(str, x, y, GFXFF);
    y+=dy;
    str = String("P. ") + String(pres*0.01, 1) + String("mb");
    img.drawString(str, x, y, GFXFF);
    y+=dy;

    bUpdate = false;
    img.pushSprite(0, 0);
  }

  usleep(100);
}