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
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "rom/ets_sys.h"


/* Private Variables ------------------------------------------------------------*/
static spi_device_handle_t spi_device_handle = {0};



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
IC74165_SetGPIO_OUT(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_OUTPUT);
}

static void
IC74165_SetGPIO_IN(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_INPUT);
}

static void
IC74165_PlatformInit(void)
{
#if (IC74165_CLKINH_ENABLE)
  IC74165_SetGPIO_OUT(IC74165_CLKINH_GPIO);
#endif
  IC74165_SetGPIO_OUT(IC74165_CLK_GPIO);
  IC74165_SetGPIO_OUT(IC74165_SHLD_GPIO);
  IC74165_SetGPIO_IN(IC74165_QH_GPIO);
}

static void
IC74165_PlatformDeInit(void)
{
#if (IC74165_CLKINH_ENABLE)
  gpio_reset_pin(IC74165_CLKINH_GPIO);
#endif
  gpio_reset_pin(IC74165_CLK_GPIO);
  gpio_reset_pin(IC74165_SHLD_GPIO);
  gpio_reset_pin(IC74165_QH_GPIO);
}

#if (IC74165_CLKINH_ENABLE)
static void
IC74165_ClkInhWrite(uint8_t Level)
{
  gpio_set_level(IC74165_CLKINH_GPIO, Level);
}
#endif

static uint8_t
IC74165_QhRead(void)
{
  return gpio_get_level(IC74165_QH_GPIO);
}

static void
IC74165_ClkWrite(uint8_t Level)
{
  gpio_set_level(IC74165_CLK_GPIO, Level);
}

static void
IC74165_ShLdWrite(uint8_t Level)
{
  gpio_set_level(IC74165_SHLD_GPIO, Level);
}

static void
IC74165_DelayUs(uint8_t Delay)
{
  ets_delay_us(Delay);
}

static void
IC74165_PlatformInit_SPI(void)
{
  const spi_bus_config_t spi_bus_config =
  {
    .mosi_io_num = IC74165_SHLD_GPIO,
    .miso_io_num = IC74165_QH_GPIO,
    .sclk_io_num = IC74165_CLK_GPIO,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 10,
    .flags = 0,
    .intr_flags = 0
  };
  spi_bus_initialize(IC74165_SPI_NUM, &spi_bus_config, 0);

  const spi_device_interface_config_t spi_device_interface_config =
  {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 1,
    .duty_cycle_pos = 0,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = IC74165_SPI_CLK, // Hz
    .input_delay_ns = 0,
    .spics_io_num = -1,
    .flags = 0,
    .queue_size = 1,
    .pre_cb = (void *)0,
    .post_cb = (void *)0
  };
  spi_bus_add_device(IC74165_SPI_NUM, &spi_device_interface_config, &spi_device_handle);

#if (IC74165_CLKINH_ENABLE)
  IC74165_SetGPIO_OUT(IC74165_CLKINH_GPIO);
#endif
}

static void
IC74165_PlatformDeInit_SPI(void)
{
#if (IC74165_CLKINH_ENABLE)
  gpio_reset_pin(IC74165_CLKINH_GPIO);
#endif
  spi_bus_remove_device(spi_device_handle);
  spi_bus_free(IC74165_SPI_NUM);
}

static void
IC74165_SPI_SendReceive(uint8_t *SendData,
                        uint8_t *ReceiveData,
                        uint8_t Len)
{
  spi_transaction_t spi_transaction = {0};
  uint8_t TxBuff[SOC_SPI_MAXIMUM_BUFFER_SIZE] = {0xFF};

  spi_transaction.flags = 0;
  spi_transaction.length = SOC_SPI_MAXIMUM_BUFFER_SIZE * 8;
  spi_transaction.rxlength = 0;

  while (Len > SOC_SPI_MAXIMUM_BUFFER_SIZE)
  {
    spi_transaction.tx_buffer = SendData ? SendData : TxBuff;
    spi_transaction.rx_buffer = ReceiveData;
    if (spi_device_polling_transmit(spi_device_handle, &spi_transaction) != ESP_OK)
      return;

    Len -= SOC_SPI_MAXIMUM_BUFFER_SIZE;
    if (ReceiveData)
      ReceiveData += SOC_SPI_MAXIMUM_BUFFER_SIZE;
    if (SendData)
      SendData += SOC_SPI_MAXIMUM_BUFFER_SIZE;
  }

  if (Len)
  {
    spi_transaction.length = Len * 8;
    spi_transaction.rxlength = 0;
    spi_transaction.tx_buffer = SendData ? SendData : TxBuff;
    spi_transaction.rx_buffer = ReceiveData;
    if (spi_device_polling_transmit(spi_device_handle, &spi_transaction) != ESP_OK)
      return;
  }

  return;
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

/**
 * @brief  Initialize platform dependent layer to communicate with 74165 using
 *         SPI.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
IC74165_Platform_Init_SPI(IC74165_Handler_t *Handler)
{
  IC74165_PLATFORM_SET_COMMUNICATION(Handler, IC74165_COMMUNICATION_SPI);
  IC74165_PLATFORM_LINK_INIT(Handler, IC74165_PlatformInit_SPI);
  IC74165_PLATFORM_LINK_DEINIT(Handler, IC74165_PlatformDeInit_SPI);
#if (IC74165_CLKINH_ENABLE)
  IC74165_PLATFORM_LINK_CLKINHWRITE(Handler, IC74165_ClkInhWrite);
#endif
  IC74165_PLATFORM_LINK_SPI_SENDRECEIVE(Handler, IC74165_SPI_SendReceive);
}

