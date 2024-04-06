# SHT3x Library
74165 8-bit serial shift register driver.
- Support chaining of multiple 74165

## Hardware Support
It is easy to port this library to any platform. But now it is ready for use in:
- STM32 (HAL)
- ESP32 (esp-idf)
- AVR (ATmega32)

## How To Use
1. Add `74165.h` and `74165.c` files to your project.  It is optional to use `74165_platform.h` and `74165_platform.c` files (open and config `74165_platform.h` file).
2. Initialize platform-dependent part of handler.
4. Call `IC74165_Init()`.
5. Call other functions and enjoy.

## Example
<details>
<summary>Using 74165_platform files</summary>

```c
#include <stdio.h>
#include "74165.h"
#include "74165_platform.h"

void app_main(void)
{
  IC74165_Handler_t Handler = {0};

  printf(TAG, "74165 Driver Example\r\n");

  IC74165_Platform_Init(&Handler);
  IC74165_Init(&Handler, 2);

  while (1)
  {
    uint8_t Data[2] = {0};
    IC74165_ReadAll(&Handler, Data);
    printf("Value: 0x%02hhX, 0x%02hhX\r\n", Data[0], Data[1]);
  }

  IC74165_DeInit(&Handler);
}
```
</details>


<details>
<summary>Without using 74165_platform files (esp-idf)</summary>

```c
#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "74165.h"

#define IC74165_CLK_GPIO      GPIO_NUM_18
#define IC74165_SHLD_GPIO     GPIO_NUM_23
#define IC74165_QH_GPIO       GPIO_NUM_22

void
IC74165_SetGPIO_OUT(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_OUTPUT);
}

void
IC74165_SetGPIO_IN(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_INPUT);
}

void
IC74165_PlatformInit(void)
{
  IC74165_SetGPIO_OUT(IC74165_CLK_GPIO);
  IC74165_SetGPIO_OUT(IC74165_SHLD_GPIO);
  IC74165_SetGPIO_IN(IC74165_QH_GPIO);
}

void
IC74165_PlatformDeInit(void)
{
  gpio_reset_pin(IC74165_CLK_GPIO);
  gpio_reset_pin(IC74165_SHLD_GPIO);
  gpio_reset_pin(IC74165_QH_GPIO);
}

uint8_t
IC74165_QhRead(void)
{
  return gpio_get_level(IC74165_QH_GPIO);
}

void
IC74165_ClkWrite(uint8_t Level)
{
  gpio_set_level(IC74165_CLK_GPIO, Level);
}

void
IC74165_ShLdWrite(uint8_t Level)
{
  gpio_set_level(IC74165_SHLD_GPIO, Level);
}

void
IC74165_DelayUs(uint8_t Delay)
{
  ets_delay_us(Delay);
}


int main(void)
{
  IC74165_Handler_t Handler = {0};

  printf(TAG, "74165 Driver Example\r\n");

  IC74165_PLATFORM_SET_COMMUNICATION(&Handler, IC74165_COMMUNICATION_GPIO);
  IC74165_PLATFORM_LINK_INIT(&Handler, IC74165_PlatformInit);
  IC74165_PLATFORM_LINK_DEINIT(&Handler, IC74165_PlatformDeInit);
  IC74165_PLATFORM_LINK_GPIO_CLKWRITE(&Handler, IC74165_ClkWrite);
  IC74165_PLATFORM_LINK_GPIO_SHLDWRITE(&Handler, IC74165_ShLdWrite);
  IC74165_PLATFORM_LINK_GPIO_QHREAD(&Handler, IC74165_QhRead);
  IC74165_PLATFORM_LINK_GPIO_DELAYUS(&Handler, IC74165_DelayUs);

  IC74165_Init(&Handler, 2);

  while (1)
  {
    uint8_t Data[2] = {0};
    IC74165_ReadAll(&Handler, Data);
    printf("Value: 0x%02hhX, 0x%02hhX\r\n", Data[0], Data[1]);
  }

  IC74165_DeInit(&Handler);
  return 0;
}
```
</details>
