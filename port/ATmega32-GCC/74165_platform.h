/**
 **********************************************************************************
 * @file   74165_platform.h
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
  
/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _74165_PLATFORM_H_
#define _74165_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include "74165.h"
#include <stdint.h>


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify IO Pins of AVR connected to 74165
 */
#define IC74165_AVR_CLK       8000000UL
#define IC74165_CLK_DDR       DDRA
#define IC74165_CLK_PORT      PORTA
#define IC74165_CLK_NUM       0
#define IC74165_SHLD_DDR      DDRA
#define IC74165_SHLD_PORT     PORTA
#define IC74165_SHLD_NUM      1
#define IC74165_QH_DDR        DDRA
#define IC74165_QH_PORT       PORTA
#define IC74165_QH_PIN        PINA
#define IC74165_QH_NUM        2
#define IC74165_CLKINH_DDR    DDRA
#define IC74165_CLKINH_PORT   PORTA
#define IC74165_CLKINH_NUM    3
#define IC74165_CLKINH_ENABLE 0



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Initialize platform dependent layer to communicate with 74165 using
 *         GPIO.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
IC74165_Platform_Init(IC74165_Handler_t *Handler);



#ifdef __cplusplus
}
#endif

#endif //! _74165_PLATFORM_H_
