#include "main.h"

/* The linker omits the toolchain CRT; startup still runs libc init arrays. */
void _init(void) {}

void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *i2c)
{
    if (i2c->Instance != I2C1)
    {
        return;
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    gpio.Mode = GPIO_MODE_AF_OD;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);
    __HAL_RCC_I2C1_CLK_ENABLE();
}
