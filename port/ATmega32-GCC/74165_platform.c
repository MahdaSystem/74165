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
#include <avr/io.h>
#define F_CPU IC74165_AVR_CLK
#include <util/delay.h>



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
IC74165_PlatformInit(void)
{
#if (IC74165_CLKINH_ENABLE)
  IC74165_CLKINH_DDR |= (1<<IC74165_CLKINH_NUM);
#endif
  IC74165_CLK_DDR |= (1<<IC74165_CLK_NUM);
  IC74165_SHLD_DDR |= (1<<IC74165_SHLD_NUM);
  IC74165_QH_DDR &= ~(1<<IC74165_QH_NUM);
  IC74165_QH_PORT &= ~(1<<IC74165_QH_NUM);
}

static void
IC74165_PlatformDeInit(void)
{
#if (IC74165_CLKINH_ENABLE)
  IC74165_CLKINH_DDR &= ~(1<<IC74165_CLKINH_NUM);
  IC74165_CLKINH_PORT &= ~(1<<IC74165_CLKINH_NUM);
#endif
  IC74165_CLK_DDR &= ~(1<<IC74165_CLK_NUM);
  IC74165_CLK_PORT &= ~(1<<IC74165_CLK_NUM);
  IC74165_SHLD_DDR &= ~(1<<IC74165_SHLD_NUM);
  IC74165_SHLD_PORT &= ~(1<<IC74165_SHLD_NUM);
  IC74165_QH_DDR &= ~(1<<IC74165_QH_NUM);
  IC74165_QH_PORT &= ~(1<<IC74165_QH_NUM);
}

#if (IC74165_CLKINH_ENABLE)
static void
IC74165_ClkInhWrite(uint8_t Level)
{
  if (Level)
    IC74165_CLKINH_PORT |= (1<<IC74165_CLKINH_NUM);
  else
    IC74165_CLKINH_PORT &= ~(1<<IC74165_CLKINH_NUM);
}
#endif

static uint8_t
IC74165_QhRead(void)
{
  return (IC74165_QH_PIN & (1 << IC74165_QH_NUM)) ? 1 : 0;
}

static void
IC74165_ClkWrite(uint8_t Level)
{
  if (Level)
    IC74165_CLK_PORT |= (1<<IC74165_CLK_NUM);
  else
    IC74165_CLK_PORT &= ~(1<<IC74165_CLK_NUM);
}

static void
IC74165_ShLdWrite(uint8_t Level)
{
  if (Level)
    IC74165_SHLD_PORT |= (1<<IC74165_SHLD_NUM);
  else
    IC74165_SHLD_PORT &= ~(1<<IC74165_SHLD_NUM);
}

static void
IC74165_DelayUs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_us(1);
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
