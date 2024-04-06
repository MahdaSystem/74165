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

/* Includes ---------------------------------------------------------------------*/
#include "74165.h"
#include <stdio.h>
#include <string.h>


/**
 ==================================================================================
                          ##### Private Functions #####                            
 ==================================================================================
 */

static inline IC74165_Result_t
IC74165_Load(IC74165_Handler_t *Handler)
{
  if (Handler->Platform.Communication == IC74165_COMMUNICATION_GPIO)
  {
    Handler->Platform.GPIO.ShLdWrite(0);
    Handler->Platform.GPIO.DelayUs(1);
    Handler->Platform.GPIO.ShLdWrite(1);
    Handler->Platform.GPIO.DelayUs(1);
  }
  else if (Handler->Platform.Communication == IC74165_COMMUNICATION_SPI)
  {
    uint8_t Buffer = 0;
    Handler->Platform.SPI.SendReceive(&Buffer, &Buffer, 1);
  }
  return IC74165_OK;
}

static IC74165_Result_t
IC74165_ShiftIn(IC74165_Handler_t *Handler, uint8_t *Data, uint8_t Count)
{
  if (Handler->Platform.ClkInhWrite)
    Handler->Platform.ClkInhWrite(0);

  if (Handler->Platform.Communication == IC74165_COMMUNICATION_GPIO)
  {
    for (uint8_t i = 0; i < Count; i++)
    {
      uint8_t Buffer = 0;
      for (int8_t j = 7; j >= 0; j--)
      {
        Buffer |= (Handler->Platform.GPIO.QhRead() << j);
        Handler->Platform.GPIO.ClkWrite(1);
        Handler->Platform.GPIO.DelayUs(1);
        Handler->Platform.GPIO.ClkWrite(0);
        Handler->Platform.GPIO.DelayUs(1);
      }

      if (Data != NULL)
        Data[i] = Buffer;
    }
  }
  else if (Handler->Platform.Communication == IC74165_COMMUNICATION_SPI)
  {
    if (Data != NULL)
      memset(Data, 0xFF, Count);
    Handler->Platform.SPI.SendReceive(Data, Data, Count);
  }

  if (Handler->Platform.ClkInhWrite)
    Handler->Platform.ClkInhWrite(1);

  return IC74165_OK;
}



/**
 ==================================================================================
                           ##### Public Functions #####                            
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
IC74165_Init(IC74165_Handler_t *Handler, uint8_t ChainLen)
{
  if (Handler->Platform.Communication == IC74165_COMMUNICATION_GPIO)
  {
    if (Handler->Platform.GPIO.ClkWrite == NULL ||
        Handler->Platform.GPIO.ShLdWrite == NULL ||
        Handler->Platform.GPIO.QhRead == NULL ||
        Handler->Platform.GPIO.DelayUs == NULL)
      return IC74165_FAIL;
  }
  else if (Handler->Platform.Communication == IC74165_COMMUNICATION_SPI)
  {
    if (Handler->Platform.SPI.SendReceive == NULL)
      return IC74165_FAIL;
  }

  if (Handler->Platform.Init)
    Handler->Platform.Init();

  if (ChainLen == 0)
    ChainLen = 1;

  Handler->ChainLen = ChainLen;

  return IC74165_OK;
}


/**
 * @brief  De-Initialization function.
 * @param  Handler: Pointer to handler
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_DeInit(IC74165_Handler_t *Handler)
{
  if (Handler->Platform.DeInit)
    Handler->Platform.DeInit();
  Handler->ChainLen = 0;
  return IC74165_OK;
}


/**
 * @brief  Read chain.
 * @param  Handler: Pointer to handler
 * @note   In case of using SPI, this function will not work and you should use
 *         IC74165_ReadAll() function.
 * @param  Data: Pointer to a buffer to store data
 * @param  Pos: Start position in chain
 * @param  Count: Number of bytes to read from chain
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_Read(IC74165_Handler_t *Handler, uint8_t *Data,
             uint8_t Pos, uint8_t Count)
{
  if (Handler->ChainLen == 0)
    return IC74165_FAIL;

  if (Count + Pos > Handler->ChainLen)
    Count = Handler->ChainLen - Pos;

  IC74165_Load(Handler);

  if (IC74165_ShiftIn(Handler, NULL, Pos) != IC74165_OK)
    return IC74165_FAIL;

  if (IC74165_ShiftIn(Handler, Data, Count) != IC74165_OK)
    return IC74165_FAIL;

  return IC74165_OK;
}


/**
 * @brief  Read all chained devices.
 * @param  Handler: Pointer to handler
 * @param  Data: Pointer to a buffer to store data
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_ReadAll(IC74165_Handler_t *Handler, uint8_t *Data)
{
  if (Handler->ChainLen == 0)
    return IC74165_FAIL;

  IC74165_Load(Handler);

  if (IC74165_ShiftIn(Handler, Data, Handler->ChainLen) != IC74165_OK)
    return IC74165_FAIL;

  return IC74165_OK;
}


/**
 * @brief  Read a shift register in the chain.
 * @note   In case of using SPI, this function will not work and you should use
 *         IC74165_ReadAll() function.
 * @param  Handler: Pointer to handler
 * @param  Data: Pointer to a buffer to store data
 * @param  Pos: The position in the chain
 * @retval IC74165_Result_t
 *         - IC74165_OK: Operation was successful.
 *         - IC74165_FAIL: Operation was not successful.
 */
IC74165_Result_t
IC74165_ReadOne(IC74165_Handler_t *Handler, uint8_t *Data,
                uint8_t Pos)
{
  return IC74165_Read(Handler, Data, Pos, 1);
}
