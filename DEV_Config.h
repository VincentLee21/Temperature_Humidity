#ifndef __DEV_CONFIG_H__
#define __DEV_CONFIG_H__

#include "stdio.h"

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "DEV_Debug.h"

#define __GPIO_PWM__

#define LCD_BL_PIN          2

#define DEV_SDA0_PIN        6
#define DEV_SCL0_PIN        7
#define DEV_SDA1_PIN        15
#define DEV_SCL1_PIN        16

#define BAT_ADC_PIN         1

//--------------------
// Function definition
//--------------------

void DEV_Digital_Write(uint16_t Pin, uint8_t Value);
uint8_t DEV_Digital_Read(uint16_t Pin);
float DEV_ADC_Read(void);

void DEV_GPIO_Mode(uint16_t Pin, uint16_t Mode);
void DEV_GPIO_IN_PULLUP(uint16_t Pin);

#ifdef __GPIO_PWM__
void DEV_GPIO_PWM( int16_t pin, int16_t pwm_channel, int16_t frequency, int16_t resolution);
void DEV_GPIO_PWM_dutycycle( int16_t pwm_channel, int16_t dutycycle);
#else
void DEV_GPIO_PWM(int16_t pin, uint8_t Value);
#endif

void DEV_I2C0_Write_Byte(uint8_t addr, uint8_t reg, uint8_t Value);
void DEV_I2C0_Write_nByte(uint8_t addr, uint8_t *pData, uint32_t Len);
void DEV_I2C0_Write_Register(uint8_t addr,uint8_t reg, uint16_t value);

uint8_t DEV_I2C0_Read_Byte(uint8_t addr, uint8_t reg);
void DEV_I2C0_Read_nByte(uint8_t addr, uint8_t reg, uint8_t *pData, uint32_t Len);
void DEV_I2C0_Read_Register(uint8_t addr,uint8_t reg, uint16_t *value);

void DEV_I2C1_Write_Byte(uint8_t addr, uint8_t reg, uint8_t Value);
void DEV_I2C1_Write_nByte(uint8_t addr, uint8_t *pData, uint32_t Len);
void DEV_I2C1_Write_Register(uint8_t addr,uint8_t reg, uint16_t value);

uint8_t DEV_I2C1_Read_Byte(uint8_t addr, uint8_t reg);
void DEV_I2C1_Read_nByte(uint8_t addr, uint8_t reg, uint8_t *pData, uint32_t Len);
void DEV_I2C1_Read_Register(uint8_t addr,uint8_t reg, uint16_t *value);


uint8_t DEV_Module_Init(void);

#endif
