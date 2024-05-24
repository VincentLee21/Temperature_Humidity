#include "DEV_Config.h"


// ------------------------------------------------------------------------------------
// function:  I2C0 R/W functions
// ------------------------------------------------------------------------------------
void DEV_I2C0_Write_Byte(uint8_t addr, uint8_t reg, uint8_t Value)
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(Value);
  Wire.endTransmission();
}

void DEV_I2C0_Write_nByte(uint8_t addr,uint8_t *pData, uint32_t Len)
{
  Wire.beginTransmission(addr);
  Wire.write(pData,Len);
  Wire.endTransmission();
}

void DEV_I2C0_Write_Register(uint8_t addr, uint8_t reg, uint16_t value)
{
  uint8_t tmpi[3];
  tmpi[0] = reg;
  tmpi[1] = (value >> 8) & 0xFF;
  tmpi[2] = value & 0xFF;
  Wire.beginTransmission(addr);
  Wire.write(tmpi,3);
  Wire.endTransmission();
}

uint8_t DEV_I2C0_Read_Byte(uint8_t addr, uint8_t reg)
{
  uint8_t value;

  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, (byte)1);
  value = Wire.read();

  return value;
}

void DEV_I2C0_Read_nByte(uint8_t addr, uint8_t reg, uint8_t *pData, uint32_t Len)
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, Len);

  uint8_t i = 0;
  for(i = 0; i < Len; i++) {
    pData[i] =  Wire.read();
  }
  Wire.endTransmission();
}

void DEV_I2C0_Read_Register(uint8_t addr, uint8_t reg, uint16_t *value)
{
  uint8_t tmpi[2];

  Wire.beginTransmission(addr);
  Wire.write(reg);
  // Wire.endTransmission();
  Wire.requestFrom(addr, 2);

  uint8_t i = 0;
  for(i = 0; i < 2; i++) {
    tmpi[i] =  Wire.read();
  }
  Wire.endTransmission();
  *value = (((uint16_t)tmpi[0] << 8) | (uint16_t)tmpi[1]);
}

// ------------------------------------------------------------------------------------
// function:  I2C1 R/W functions
// ------------------------------------------------------------------------------------
void DEV_I2C1_Write_Byte(uint8_t addr, uint8_t reg, uint8_t Value)
{
  Wire1.beginTransmission(addr);
  Wire1.write(reg);
  Wire1.write(Value);
  Wire1.endTransmission();
}

void DEV_I2C1_Write_nByte(uint8_t addr,uint8_t *pData, uint32_t Len)
{
  Wire1.beginTransmission(addr);
  Wire1.write(pData,Len);
  Wire1.endTransmission();
}

void DEV_I2C1_Write_Register(uint8_t addr, uint8_t reg, uint16_t value)
{
  uint8_t tmpi[3];
  tmpi[0] = reg;
  tmpi[1] = (value >> 8) & 0xFF;
  tmpi[2] = value & 0xFF;
  Wire1.beginTransmission(addr);
  Wire1.write(tmpi,3);
  Wire1.endTransmission();
}

uint8_t DEV_I2C1_Read_Byte(uint8_t addr, uint8_t reg)
{
  uint8_t value;

  Wire1.beginTransmission(addr);
  Wire1.write((byte)reg);
  Wire1.endTransmission();

  Wire1.requestFrom(addr, (byte)1);
  value = Wire1.read();

  return value;
}

void DEV_I2C1_Read_nByte(uint8_t addr, uint8_t reg, uint8_t *pData, uint32_t Len)
{
  Wire1.beginTransmission(addr);
  Wire1.write(reg);
  Wire1.endTransmission();

  Wire1.requestFrom(addr, Len);

  uint8_t i = 0;
  for(i = 0; i < Len; i++) {
    pData[i] =  Wire1.read();
  }
  Wire1.endTransmission();
}

void DEV_I2C1_Read_Register(uint8_t addr, uint8_t reg, uint16_t *value)
{
  uint8_t tmpi[2];

  Wire1.beginTransmission(addr);
  Wire1.write(reg);
  // Wire1.endTransmission();
  Wire1.requestFrom(addr, 2);

  uint8_t i = 0;
  for(i = 0; i < 2; i++) {
    tmpi[i] =  Wire1.read();
  }
  Wire1.endTransmission();
  *value = (((uint16_t)tmpi[0] << 8) | (uint16_t)tmpi[1]);
}

// ------------------------------------------------------------------------------------
// function:  Get ADC reading
// ------------------------------------------------------------------------------------
float DEV_ADC_Read(void)
{
  return analogReadMilliVolts(BAT_ADC_PIN) * 0.001;
}

// ------------------------------------------------------------------------------------
// function:  Set PWM
// ------------------------------------------------------------------------------------
#ifdef __GPIO_PWM__
void DEV_GPIO_PWM( int16_t pin, int16_t pwm_channel, int16_t frequency, int16_t resolution) {
  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(pin, pwm_channel);
}

void DEV_GPIO_PWM_dutycycle( int16_t pwm_channel, int16_t dutycycle) {
  ledcWrite(pwm_channel, dutycycle);
}
#else
void DEV_GPIO_PWM(int16_t pin, uint8_t Value)
{
  if (Value > 255)
    return;
  analogWrite(pin, Value);
}
#endif

// ------------------------------------------------------------------------------------
// function:  Set GPIO in/out mode
// ------------------------------------------------------------------------------------
void DEV_GPIO_Mode(uint16_t Pin, uint16_t Mode)
{
  pinMode(Pin, (Mode == 0) ? INPUT : OUTPUT);
}

// ------------------------------------------------------------------------------------
// function:  Set GPIO in w/i pullup mode
// ------------------------------------------------------------------------------------
void DEV_GPIO_IN_PULLUP(uint16_t Pin)
{
  pinMode(Pin,INPUT_PULLUP);
}

// ------------------------------------------------------------------------------------
// function:  GPIO R/W function
// ------------------------------------------------------------------------------------
void DEV_Digital_Write(uint16_t Pin, uint8_t Value)
{
  digitalWrite(Pin, Value);
}

uint8_t DEV_Digital_Read(uint16_t Pin)
{
  return digitalRead(Pin);
}

// ------------------------------------------------------------------------------------
// function:  Initial GPIO & PWM pins
// ------------------------------------------------------------------------------------
void DEV_GPIO_Init(void)
{
  // ADC
  DEV_GPIO_Mode(BAT_ADC_PIN,0);

  // PWM Config
  #ifdef __GPIO_PWM__
  DEV_GPIO_PWM( LCD_BL_PIN, 0, 500, 8); // 500Hz, 8bit resolution
  DEV_GPIO_PWM_dutycycle( 0, 240); // Dutycycle 240/255
  #else
  DEV_GPIO_PWM( LCD_BL_PIN, 250); // Dutycycle 250/255
  #endif
}


// ------------------------------------------------------------------------------------
// function:  Module Initialize, Initial GPIO & I2C inferface
// ------------------------------------------------------------------------------------
uint8_t DEV_Module_Init(void)
{
  Serial.begin(115200);
  usleep(100000);
  // GPIO Config
  DEV_GPIO_Init();

  // I2C Config
  Wire.setPins(DEV_SDA0_PIN, DEV_SCL0_PIN);
  Wire.setClock(400000);
  Wire.begin();

  Wire1.setPins(DEV_SDA1_PIN, DEV_SCL1_PIN);
  Wire1.setClock(400000);
  Wire1.begin();
  Print("DEV_Module_Init OK \r\n");
  return 0;
}

// ------------------------------------------------------------------------------------
// function:  Module exits, restore IO settings
// ------------------------------------------------------------------------------------
void DEV_Module_Exit(void)
{
  Wire.end();
  Wire1.end();
}
