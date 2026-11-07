/**
  ******************************************************************************
  * @file           : lcd_i2c.h
  * @brief          : Header file for LCD1602 I2C library
  ******************************************************************************
  */

#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#include "stm32f1xx_hal.h"

/* LCD I2C Address - Change this if your module has different address */
#ifndef LCD_I2C_ADDR
#define LCD_I2C_ADDR    (0x27U << 1)  // Shifted for HAL I2C
#endif

/* LCD Commands */
#define LCD_CMD_CLEAR           0x01
#define LCD_CMD_HOME            0x02
#define LCD_CMD_ENTRY_MODE      0x06
#define LCD_CMD_DISPLAY_ON      0x0C
#define LCD_CMD_DISPLAY_OFF     0x08
#define LCD_CMD_FUNCTION_SET    0x28  // 4-bit, 2 lines, 5x8 font
#define LCD_CMD_SET_DDRAM       0x80

/* LCD Control Bits */
#define LCD_RS                  0x01  // Register Select
#define LCD_RW                  0x02  // Read/Write
#define LCD_EN                  0x04  // Enable
#define LCD_BACKLIGHT           0x08  // Backlight

/* Function Prototypes */

/**
 * @brief Initialize LCD with I2C
 * @param hi2c Pointer to I2C handle
 */
void LCD_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Clear LCD display
 */
void LCD_Clear(void);

/**
 * @brief Set cursor position
 * @param row Row number (0 or 1)
 * @param col Column number (0-15)
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Send a string to LCD
 * @param str Null-terminated string
 */
void LCD_SendString(char *str);

/**
 * @brief Send a single character to LCD
 * @param ch Character to send
 */
void LCD_SendChar(char ch);

/**
 * @brief Turn on/off backlight
 * @param state 1 = ON, 0 = OFF
 */
void LCD_Backlight(uint8_t state);

#endif /* __LCD_I2C_H */
