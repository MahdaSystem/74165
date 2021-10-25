/**
 **********************************************************************************
 * @file   74165_platform.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  A sample Platform dependent layer for 74165 Driver
 **********************************************************************************
 *
 * Copyright (c) 2021 Hossein.M (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */
  
/* Includes ---------------------------------------------------------------------*/
#include "74165_platform.h"

#if defined(IC74165_PLATFORM_AVR)
#include <avr/io.h>
#define F_CPU IC74165_AVR_CLK
#include <util/delay.h>
#elif defined(IC74165_PLATFORM_STM32)
#include "main.h"
#elif defined(IC74165_PLATFORM_ESP32_IDF)
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#endif



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

#if defined(IC74165_PLATFORM_STM32)
void IC74165_SetGPIO_OUT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
									
void IC74165_SetGPIO_IN(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
#elif defined(IC74165_PLATFORM_ESP32_IDF)
void IC74165_SetGPIO_OUT(uint64_t GPIO_Pad)
{
  gpio_config_t IC74165_GPIO_CONF;
  IC74165_GPIO_CONF.pin_bit_mask = GPIO_Pad;
  IC74165_GPIO_CONF.mode = GPIO_MODE_OUTPUT;
  IC74165_GPIO_CONF.pull_up_en = GPIO_PULLUP_DISABLE;
  IC74165_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;
  IC74165_GPIO_CONF.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&IC74165_GPIO_CONF);
}

void IC74165_SetGPIO_IN(uint64_t GPIO_Pad)
{
  gpio_config_t IC74165_GPIO_CONF;
  IC74165_GPIO_CONF.pin_bit_mask = GPIO_Pad;
  IC74165_GPIO_CONF.mode = GPIO_MODE_DEF_INPUT;
  IC74165_GPIO_CONF.pull_up_en = GPIO_PULLUP_DISABLE;
  IC74165_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;
  IC74165_GPIO_CONF.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&IC74165_GPIO_CONF);
}
#endif

static void
IC74165_PlatformInit(void)
{
#if defined(IC74165_PLATFORM_AVR)
  IC74165_CLK_DDR |= (1<<IC74165_CLK_NUM);
  IC74165_SHLD_DDR |= (1<<IC74165_SHLD_NUM);
  IC74165_SO_DDR &= ~(1<<IC74165_SO_NUM);
#elif defined(IC74165_PLATFORM_STM32)
  IC74165_SetGPIO_OUT(IC74165_CLK_GPIO, IC74165_CLK_PIN);
  IC74165_SetGPIO_OUT(IC74165_SHLD_GPIO, IC74165_SHLD_PIN);
  IC74165_SetGPIO_IN(IC74165_SO_GPIO, IC74165_SO_PIN);
#elif defined(IC74165_PLATFORM_ESP32_IDF)
  uint64_t GPIO_Pad = 0;
  GPIO_Pad = (1 << IC74165_CLK_GPIO) |
             (1 << IC74165_SHLD_GPIO);
  IC74165_SetGPIO_OUT(GPIO_Pad);

  GPIO_Pad = (1 << IC74165_SO_GPIO);
  IC74165_SetGPIO_IN(GPIO_Pad);
#endif
}

static void
IC74165_PlatformDeInit(void)
{
#if defined(IC74165_PLATFORM_AVR)
  IC74165_CLK_DDR &= ~(1<<IC74165_CLK_NUM);
  IC74165_CLK_PORT &= ~(1<<IC74165_CLK_NUM);
  IC74165_SHLD_DDR &= ~(1<<IC74165_SHLD_NUM);
  IC74165_SHLD_PORT &= ~(1<<IC74165_SHLD_NUM);
  IC74165_SO_DDR &= ~(1<<IC74165_SO_NUM);
  IC74165_SO_PORT &= ~(1<<IC74165_SO_NUM);
#elif defined(IC74165_PLATFORM_STM32)

#elif defined(IC74165_PLATFORM_ESP32_IDF)
  gpio_reset_pin(IC74165_CLK_GPIO);
  gpio_reset_pin(IC74165_SHLD_GPIO);
  gpio_reset_pin(IC74165_SO_GPIO);
#endif
}

static uint8_t
IC74165_SoRead(void)
{
  uint8_t Result = 1;
#if defined(IC74165_PLATFORM_AVR)
  Result = (IC74165_SO_PIN & (1 << IC74165_SO_NUM)) ? 1 : 0;
#elif defined(IC74165_PLATFORM_STM32)
  Result = HAL_GPIO_ReadPin(IC74165_SO_GPIO, IC74165_SO_PIN);
#elif defined(IC74165_PLATFORM_ESP32_IDF)
  Result = gpio_get_level(IC74165_SO_GPIO);
#endif
  return Result;
}

static void
IC74165_ClkWrite(uint8_t Level)
{
#if defined(IC74165_PLATFORM_AVR)
  if (Level)
    IC74165_CLK_PORT |= (1<<IC74165_CLK_NUM);
  else
    IC74165_CLK_PORT &= ~(1<<IC74165_CLK_NUM);
#elif defined(IC74165_PLATFORM_STM32)
  HAL_GPIO_WritePin(IC74165_CLK_GPIO, IC74165_CLK_PIN, Level);
#elif defined(IC74165_PLATFORM_ESP32_IDF)
  gpio_set_level(IC74165_CLK_GPIO, Level);
#endif
}

static void
IC74165_ShLdWrite(uint8_t Level)
{
#if defined(IC74165_PLATFORM_AVR)
  if (Level)
    IC74165_SHLD_PORT |= (1<<IC74165_SHLD_NUM);
  else
    IC74165_SHLD_PORT &= ~(1<<IC74165_SHLD_NUM);
#elif defined(IC74165_PLATFORM_STM32)
  HAL_GPIO_WritePin(IC74165_SHLD_GPIO, IC74165_SHLD_PIN, Level);
#elif defined(IC74165_PLATFORM_ESP32_IDF)
  gpio_set_level(IC74165_SHLD_GPIO, Level);
#endif
}

static void
IC74165_DelayUs(uint8_t Delay)
{
#if defined(IC74165_PLATFORM_AVR)
  for (; Delay; --Delay)
    _delay_us(1);
#elif defined(IC74165_PLATFORM_STM32)
  for (uint32_t DelayCounter = 0; DelayCounter < 100 * Delay; DelayCounter++)
    DelayCounter = DelayCounter;
#elif defined(IC74165_PLATFORM_ESP32_IDF)
  ets_delay_us(Delay);
#endif
}

/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */
 
/**
 * @brief  Initialize platfor device to communicate IC74165.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
IC74165_Platform_Init(IC74165_Handler_t *Handler)
{
  Handler->PlatformInit = IC74165_PlatformInit;
  Handler->PlatformDeInit = IC74165_PlatformDeInit;
  Handler->ClkWrite = IC74165_ClkWrite;
  Handler->ShLdWrite = IC74165_ShLdWrite;
  Handler->SoRead = IC74165_SoRead;
  Handler->DelayUs = IC74165_DelayUs;
}
