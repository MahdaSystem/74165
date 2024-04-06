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
 * @brief  Data type of library functions result
 */
typedef enum IC74165_Result_e
{
  IC74165_OK      = 0,
  IC74165_FAIL    = -1,
} IC74165_Result_t;

/**
 * @brief  Communication type
 */
typedef enum IC74165_Communication_e
{
  IC74165_COMMUNICATION_GPIO  = 0,
  IC74165_COMMUNICATION_SPI   = 1,
} IC74165_Communication_t;


/**
 * @brief  Function type for Initialize/Deinitialize the platform dependent layer.
 */
typedef void (*IC74165_Platform_InitDeinit_t)(void);

/**
 * @brief  Function type for Set/Reset GPIO pin.
 * @param  Level: 0 for reset, 1 for set
 */
typedef void (*IC74165_Platform_SetLevelGPIO_t)(uint8_t Level);

/**
 * @brief  Function type for get GPIO pin level.
 * @retval 
 *         - 0: Low level
 *         - 1: High level
 */
typedef uint8_t (*IC74165_Platform_GetLevelGPIO_t)(void);

/**
 * @brief  Function type for delay.
 * @param  Delay: Delay duration
 */
typedef void (*IC74165_Platform_Delay_t)(uint8_t Delay);

/**
 * @brief  Function type for Send/Receive data to/from the slave through SPI.
 * @param  SendData: Pointer to data to send
 * @param  ReceiveData: Pointer to data to receive
 * @param  Len: data len in Bytes
 * @note   If SendData is NULL, the function must receive data.
 * @note   If ReceiveData is NULL, the function must send data.
 */
typedef void (*IC74165_Platform_SPI_SendReceive_t)(uint8_t *SendData,
                                                   uint8_t *ReceiveData,
                                                   uint8_t Len);

/**
 * @brief  Platform dependent layer data type
 * @note   It is optional to initialize this functions:
 *         - Init
 *         - DeInit
 *         - ClkInhWrite
 * @note   If using GPIO, user must initialize this this functions before using library:
 *         - ClkWrite
 *         - ShLdWrite
 *         - QhRead
 *         - DelayUs
 * @note   If using SPI, user must initialize this this functions before using library:
 *         - SendReceive
 *         - SetLevelCS
 * @note   In case of using SPI, the MOSI, MISO and CS pins must be connected to SH/LD,
 *         Qh and CLK-INH pins of 74165.
 */
typedef struct IC74165_Platform_s
{
  // Communication type
  IC74165_Communication_t Communication;

  // Initialize platform dependent layer
  IC74165_Platform_InitDeinit_t Init;
  // De-initialize platform dependent layer
  IC74165_Platform_InitDeinit_t DeInit;

  // Set level of the GPIO that connected to CLK-INH PIN of 74165
  IC74165_Platform_SetLevelGPIO_t ClkInhWrite;

  // Platform dependent layer for SPI or GPIO
  union
  {
    struct
    {
      // Set level of the GPIO that connected to CLK PIN of 74165
      IC74165_Platform_SetLevelGPIO_t ClkWrite;
      // Set level of the GPIO that connected to SH/LD PIN of 74165
      IC74165_Platform_SetLevelGPIO_t ShLdWrite;
      // Read the GPIO that connected to Qh PIN of 74165
      IC74165_Platform_GetLevelGPIO_t QhRead;
      // Delay (us)
      IC74165_Platform_Delay_t DelayUs;
    } GPIO;

    struct
    {
      // Send and Receive data through SPI
      IC74165_Platform_SPI_SendReceive_t SendReceive;
    } SPI;
  };
} IC74165_Platform_t;


/**
 * @brief  Handler data type
 */
typedef struct IC74165_Handler_s
{
  uint8_t ChainLen;

  // Platform dependent layer
  IC74165_Platform_t Platform;
} IC74165_Handler_t;


/* Exported Macros --------------------------------------------------------------*/
/**
 * @brief  Link platform dependent layer communication type
 * @param  HANDLER: Pointer to handler
 * @param  COM: Communication type
 *         - IC74165_COMMUNICATION_SPI
 *         - IC74165_COMMUNICATION_UART
 */
#define IC74165_PLATFORM_SET_COMMUNICATION(HANDLER, COM) \
  (HANDLER)->Platform.Communication = COM


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_INIT(HANDLER, FUNC) \
  (HANDLER)->Platform.Init = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_DEINIT(HANDLER, FUNC) \
  (HANDLER)->Platform.DeInit = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_CLKINHWRITE(HANDLER, FUNC) \
  (HANDLER)->Platform.ClkInhWrite = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_GPIO_CLKWRITE(HANDLER, FUNC) \
  (HANDLER)->Platform.GPIO.ClkWrite = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_GPIO_SHLDWRITE(HANDLER, FUNC) \
  (HANDLER)->Platform.GPIO.ShLdWrite = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_GPIO_QHREAD(HANDLER, FUNC) \
  (HANDLER)->Platform.GPIO.QhRead = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_GPIO_DELAYUS(HANDLER, FUNC) \
  (HANDLER)->Platform.GPIO.DelayUs = FUNC


/**
 * @brief  Link platform dependent layer functions to handler
 * @param  HANDLER: Pointer to handler
 * @param  FUNC: Function name
 */
#define IC74165_PLATFORM_LINK_SPI_SENDRECEIVE(HANDLER, FUNC) \
  (HANDLER)->Platform.SPI.SendReceive = FUNC



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
                uint8_t Pos);



#ifdef __cplusplus
}
#endif

#endif //! __74165_H__
