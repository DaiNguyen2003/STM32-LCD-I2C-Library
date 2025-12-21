/**
  ******************************************************************************
  * @file           : lcd_i2c.c
  * @brief          : LCD1602 I2C library implementation
  ******************************************************************************
  */

#include "lcd_i2c.h"

/* Private variables */
static I2C_HandleTypeDef *lcd_i2c;
static uint8_t backlight_state = LCD_BACKLIGHT;

/* Private function prototypes */
static void LCD_SendCmd(uint8_t cmd);
static void LCD_SendData(uint8_t data);
static void LCD_Write(uint8_t data, uint8_t mode);
static void LCD_Write4Bits(uint8_t value);
static void LCD_EnablePulse(uint8_t value);
static void LCD_I2C_Write(uint8_t data);

/**
 * @brief Write byte to I2C
 */
static void LCD_I2C_Write(uint8_t data)
{
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
}

/**
 * @brief Send enable pulse to LCD
 */
static void LCD_EnablePulse(uint8_t value)
{
    LCD_I2C_Write(value | LCD_EN);  // EN = 1
    HAL_Delay(1);
    LCD_I2C_Write(value & ~LCD_EN); // EN = 0
    HAL_Delay(1);
}

/**
 * @brief Write 4 bits to LCD
 */
static void LCD_Write4Bits(uint8_t value)
{
    LCD_I2C_Write(value | backlight_state);
    LCD_EnablePulse(value | backlight_state);
}

/**
 * @brief Write byte to LCD in 4-bit mode
 */
static void LCD_Write(uint8_t data, uint8_t mode)
{
    uint8_t high_nibble = (data & 0xF0) | mode;
    uint8_t low_nibble = ((data << 4) & 0xF0) | mode;
    
    LCD_Write4Bits(high_nibble);
    LCD_Write4Bits(low_nibble);
}

/**
 * @brief Send command to LCD
 */
static void LCD_SendCmd(uint8_t cmd)
{
    LCD_Write(cmd, 0);  // RS = 0 for command
}

/**
 * @brief Send data to LCD
 */
static void LCD_SendData(uint8_t data)
{
    LCD_Write(data, LCD_RS);  // RS = 1 for data
}

/**
 * @brief Initialize LCD
 */
void LCD_Init(I2C_HandleTypeDef *hi2c)
{
    lcd_i2c = hi2c;
    
    // Wait for LCD to power up
    HAL_Delay(50);
    
    // Initialize in 4-bit mode
    // Send 0x30 three times to ensure 4-bit mode
    LCD_Write4Bits(0x30);
    HAL_Delay(5);
    LCD_Write4Bits(0x30);
    HAL_Delay(1);
    LCD_Write4Bits(0x30);
    HAL_Delay(1);
    
    // Set to 4-bit mode
    LCD_Write4Bits(0x20);
    HAL_Delay(1);
    
    // Function set: 4-bit, 2 lines, 5x8 font
    LCD_SendCmd(LCD_CMD_FUNCTION_SET);
    
    // Display on, cursor off, blink off
    LCD_SendCmd(LCD_CMD_DISPLAY_ON);
    
    // Clear display
    LCD_Clear();
    
    // Entry mode: increment cursor, no shift
    LCD_SendCmd(LCD_CMD_ENTRY_MODE);
}

/**
 * @brief Clear LCD display
 */
void LCD_Clear(void)
{
    LCD_SendCmd(LCD_CMD_CLEAR);
    HAL_Delay(2);  // Clear command needs extra time
}

/**
 * @brief Set cursor position
 */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;
    
    if (row == 0) {
        address = 0x00 + col;
    } else {
        address = 0x40 + col;
    }
    
    LCD_SendCmd(LCD_CMD_SET_DDRAM | address);
}

/**
 * @brief Send string to LCD
 */
void LCD_SendString(char *str)
{
    while (*str) {
        LCD_SendData(*str++);
    }
}

/**
 * @brief Send single character to LCD
 */
void LCD_SendChar(char ch)
{
    LCD_SendData(ch);
}

/**
 * @brief Control backlight
 */
void LCD_Backlight(uint8_t state)
{
    if (state) {
        backlight_state = LCD_BACKLIGHT;
    } else {
        backlight_state = 0;
    }
    LCD_I2C_Write(backlight_state);
}
