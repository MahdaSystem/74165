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
#include "main.h"



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

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


static void
IC74165_PlatformInit(void)
{
#if (IC74165_CLKINH_ENABLE)
  IC74165_SetGPIO_OUT(IC74165_CLKINH_GPIO, IC74165_CLKINH_PIN);
#endif
  IC74165_SetGPIO_OUT(IC74165_CLK_GPIO, IC74165_CLK_PIN);
  IC74165_SetGPIO_OUT(IC74165_SHLD_GPIO, IC74165_SHLD_PIN);
  IC74165_SetGPIO_IN(IC74165_QH_GPIO, IC74165_QH_PIN);
}

static void
IC74165_PlatformDeInit(void)
{
}

#if (IC74165_CLKINH_ENABLE)
static void
IC74165_ClkInhWrite(uint8_t Level)
{
  HAL_GPIO_WritePin(IC74165_CLKINH_GPIO, IC74165_CLKINH_PIN, Level);
}
#endif

static uint8_t
IC74165_QhRead(void)
{
  return HAL_GPIO_ReadPin(IC74165_QH_GPIO, IC74165_QH_PIN);
}

static void
IC74165_ClkWrite(uint8_t Level)
{
  HAL_GPIO_WritePin(IC74165_CLK_GPIO, IC74165_CLK_PIN, Level);
}

static void
IC74165_ShLdWrite(uint8_t Level)
{
  HAL_GPIO_WritePin(IC74165_SHLD_GPIO, IC74165_SHLD_PIN, Level);
}

static void
IC74165_DelayUs(uint8_t Delay)
{
  for (uint32_t DelayCounter = 0; DelayCounter < 100 * Delay; DelayCounter++)
    DelayCounter = DelayCounter;
}

/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */
 
/**
 * @brief  Initialize platform dependent layer to communicate with 74165 using
 *         GPIO.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
IC74165_Platform_Init(IC74165_Handler_t *Handler)
{
  IC74165_PLATFORM_SET_COMMUNICATION(Handler, IC74165_COMMUNICATION_GPIO);
  IC74165_PLATFORM_LINK_INIT(Handler, IC74165_PlatformInit);
  IC74165_PLATFORM_LINK_DEINIT(Handler, IC74165_PlatformDeInit);
#if (IC74165_CLKINH_ENABLE)
  IC74165_PLATFORM_LINK_CLKINHWRITE(Handler, IC74165_ClkInhWrite);
#endif
  IC74165_PLATFORM_LINK_GPIO_CLKWRITE(Handler, IC74165_ClkWrite);
  IC74165_PLATFORM_LINK_GPIO_SHLDWRITE(Handler, IC74165_ShLdWrite);
  IC74165_PLATFORM_LINK_GPIO_QHREAD(Handler, IC74165_QhRead);
  IC74165_PLATFORM_LINK_GPIO_DELAYUS(Handler, IC74165_DelayUs);
}
