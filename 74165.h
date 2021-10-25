/**
 **********************************************************************************
 * @file   74165.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  74165 chip driver
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
#ifndef __74165_H__
#define __74165_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>


/* Exported Data Types ----------------------------------------------------------*/
/**
 * @brief  Handler data type
 * @note   User must initialize this this functions before using library:
 *         - PlatformInit
 *         - PlatformDeInit
 *         - ClkWrite
 *         - ShLdWrite
 *         - SoRead
 *         - DelayUs
 */
typedef struct IC74165_Handler_s
{
  // Initialize the platform-dependent layer
  void (*PlatformInit)(void);
  // Uninitialize the platform-dependent layer
  void (*PlatformDeInit)(void);

  // Set level of the GPIO that connected to CLK PIN of 74165
  void (*ClkWrite)(uint8_t);

  // Set level of the GPIO that connected to SH/LD PIN of 74165
  void (*ShLdWrite)(uint8_t);

  // Read the GPIO that connected to SO PIN of 74165
  uint8_t (*SoRead)(void);

  // Delay (us)
  void (*DelayUs)(uint8_t);

  uint8_t ChainLen;
} IC74165_Handler_t;


/**
 * @brief  Data type of library functions result
 */
typedef enum IC74165_Result_e
{
  IC74165_OK      = 0,
  IC74165_FAIL    = -1,
} IC74165_Result_t;



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Initialization function.
 * @param  Handler: Pointer to handler
 * @param  ChainLen: Number of chained 74165
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_Init(IC74165_Handler_t *Handler, uint8_t ChainLen);


/**
 * @brief  De-Initialization function.
 * @param  Handler: Pointer to handler
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_DeInit(IC74165_Handler_t *Handler);


/**
 * @brief  Read chain.
 * @param  Handler: Pointer to handler
 * @param  Data: Pointer to a buffer to store data
 * @param  Pos: Start position in chain
 * @param  Count: Number of bytes to read from chain
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_Read(IC74165_Handler_t *Handler, uint8_t *Data,
             uint8_t Pos, uint8_t Count);


/**
 * @brief  Read all chained devices.
 * @param  Handler: Pointer to handler
 * @param  Data: Pointer to a buffer to store data
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_ReadAll(IC74165_Handler_t *Handler, uint8_t *Data);


/**
 * @brief  Read a shift register in the chain.
 * @param  Handler: Pointer to handler
 * @param  Data: Pointer to a buffer to store data
 * @param  Pos: The position in the chain
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_ReadOne(IC74165_Handler_t *Handler, uint8_t *Data,
                uint8_t Pos);



#ifdef __cplusplus
}
#endif

#endif //! __74165_H__
