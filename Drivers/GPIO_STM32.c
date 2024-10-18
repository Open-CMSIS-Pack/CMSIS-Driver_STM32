/*
 * Copyright (c) 2024 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * $Date:       10. October 2024
 * $Revision:   V1.1
 *
 * Project:     GPIO Driver for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

/*! \page gpio_stm32 GPIO

# Revision History

- Version 1.1
  - Added support for GPIO ports M, N, O and P
- Version 1.0
  - Initial release

# Requirements

This driver requires the STM32 device specific **HAL** and **STM32CubeMX** (CubeMX) initialization code generator.
The driver instance is mapped to hardware as shown in the table below:

  CMSIS Driver Instance | STM32 Hardware Resource
  :---------------------|:-----------------------
  Driver_GPIO0          | PORTA .. PORTZ

# Pin mapping

  Pin Id          | STM32 Hardware Resource
  :---------------|:-----------------------
    0 ..  15      | PORTA 0..15
   16 ..  31      | PORTB 0..15
   32 ..  47      | PORTC 0..15
   48 ..  63      | PORTD 0..15
   64 ..  79      | PORTE 0..15
   80 ..  95      | PORTF 0..15
   96 .. 111      | PORTG 0..15
  112 .. 127      | PORTH 0..15
  128 .. 143      | PORTI 0..15
  144 .. 159      | PORTJ 0..15
  160 .. 175      | PORTK 0..15
  176 .. 191      | PORTM 0..15
  192 .. 207      | PORTN 0..15
  208 .. 223      | PORTO 0..15
  224 .. 239      | PORTP 0..15
  240 .. 255      | PORTZ 0..15

# Deviations

This driver has the following deviations from the CMSIS-Driver specification:

__Conceptual__ deviations:
  - CubeMX generated initialization code (function MX_GPIO_Init) already configures
    the peripheral. Power, clocks, pins, and interrupts are enabled after execution
    of initialization that executes in `main.c`.

# CubeMX Configuration

> **Important**
>
> Pins requiring edge detection must be configured as such in CubeMX.

This driver requires the following configuration in CubeMX:

  - **pins** used for **edge detection**:
    - pin function selected as **GPIO_EXTIn**.
    - for **GPIO mode** selected **External Interrupt Mode** any edge trigger detection (Rising or Falling or Rising/Falling).
    - for **GPIO Pull-up/Pull-down** selected **No pull-up and no pull-down**.
  - **pins** not used for **edge detection**:
    - pin function selected as **GPIO_Input**.
    - for **GPIO mode** selected **Input mode**.
    - for **GPIO Pull-up/Pull-down** selected **No pull-up and no pull-down**.
  - **interrupts**: enabled appropriate **EXTI line interrupt** and **IRQ handlers** that **Call HAL handlers**.

> **Notes**
>
> - configuration information in the file **MX_Device.h** file is based on CubeMX configuration.
> - due to hardware limitations only one pin at the same port position can be configured with edge detection functionality
>   (for example, for pins PC5 and PG5 only one can be selected for edge detection at one point in time).

## Example

### Pinout & Configuration tab

  1. In the **Pinout view** window click on a pin and select it's functionality:
       Pin      | Functionality
       :--------|:--------------------:
       PG5      | **GPIO_EXTI5**
     \n

  2. Under **Categories**: **System Core** select **GPIO**:

     __PG5 Configuration__:
       - GPIO Settings:
           Pin Name | Signal on Pin | GPIO output..| GPIO mode                                 | GPIO Pull-up/Pull..| User Label
           :--------|:-------------:|:------------:|:-----------------------------------------:|:------------------:|:----------:
           PG5      | n/a           | n/a          | External Interrupt Mode with Rising edge..| No pull-up and no..| .
         \n

  3. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
           NVIC Interrupt Table              | Enabled     | Preemption Priority | Sub Priority
           :---------------------------------|:-----------:|:-------------------:|:------------:
           EXTI line[9:5] interrupts         | **checked** | 0                   | 0
         \n

       - Code generation:
           Enabled interrupt table           | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
           :---------------------------------|:-----------:|:-------------------:|:----------------:|:----------------:
           EXTI line[9:5] interrupts         | unchecked   | checked             | checked          | checked
         \n
*/

/*! \cond */

#include "GPIO_STM32.h"

#include "RTE_Components.h"
#include  CMSIS_device_header

// HAL Callback prototypes
extern void HAL_GPIO_EXTI_Callback         (uint16_t GPIO_Pin);
extern void HAL_GPIO_EXTI_Rising_Callback  (uint16_t GPIO_Pin);
extern void HAL_GPIO_EXTI_Falling_Callback (uint16_t GPIO_Pin);

// Compile-time configuration **************************************************

// Configuration depending on the local macros

// Compile-time configuration (that can be externally overridden if necessary)
// Maximum number of ports
#ifndef GPIO_MAX_PORTS_NUM
#define GPIO_MAX_PORTS_NUM      (16U)
#endif

// Maximum number of pins
#ifndef GPIO_MAX_PINS_NUM
#define GPIO_MAX_PINS_NUM       ((GPIO_MAX_PORTS_NUM)*16U)
#endif

// *****************************************************************************

// Macros
// Macro for section for RW info
#ifdef  GPIO_SECTION_NAME
#define GPIO_SECTION_(name)     __attribute__((section(name)))
#define GPIO_SECTION            GPIO_SECTION_(GPIO_SECTION_NAME)
#else
#define GPIO_SECTION
#endif

// Pin configuration
typedef struct {
  uint8_t                       cb_registered : 1;      // Callback registered flag
  uint8_t                       dir           : 1;      // Direction
  uint8_t                       output_mode   : 1;      // Output mode
  uint8_t                       pull_resistor : 2;      // Pull resistor
  uint8_t                       trigger       : 2;      // Trigger
  uint8_t                       reserved      : 1;      // Reserved (for padding and future use)
} PinConfig_t;

// Run-time information (RW)
typedef struct {
  ARM_GPIO_SignalEvent_t        cb_event[16U];          // Event callback
  uint8_t                       cb_event_pin[16U];      // Pin identifier for registered event
  PinConfig_t                   pin_config[GPIO_MAX_PINS_NUM];      // Pin configuration
} RW_Info_t;

// Information definitions
static  RW_Info_t gpio0_rw_info GPIO_SECTION;

// GPIO Peripheral addresses
static GPIO_TypeDef * const gpio_addr[GPIO_MAX_PORTS_NUM] = {
#ifdef GPIOA
  GPIOA
#else
  NULL
#endif
#ifdef GPIOB
, GPIOB
#else
, NULL
#endif
#ifdef GPIOC
, GPIOC
#else
, NULL
#endif
#ifdef GPIOD
, GPIOD
#else
, NULL
#endif
#ifdef GPIOE
, GPIOE
#else
, NULL
#endif
#ifdef GPIOF
, GPIOF
#else
, NULL
#endif
#ifdef GPIOG
, GPIOG
#else
, NULL
#endif
#ifdef GPIOH
, GPIOH
#else
, NULL
#endif
#ifdef GPIOI
, GPIOI
#else
, NULL
#endif
#ifdef GPIOJ
, GPIOJ
#else
, NULL
#endif
#ifdef GPIOK
, GPIOK
#else
, NULL
#endif
#ifdef GPIOM
, GPIOM
#else
, NULL
#endif
#ifdef GPION
, GPION
#else
, NULL
#endif
#ifdef GPIOO
, GPIOO
#else
, NULL
#endif
#ifdef GPIOP
, GPIOP
#else
, NULL
#endif
#ifdef GPIOZ
, GPIOZ
#else
, NULL
#endif
};

// Local functions prototypes
static int32_t  GPIO_PinConfigure    (ARM_GPIO_Pin_t pin, PinConfig_t pin_config);
static int32_t  GPIO_Setup           (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
static int32_t  GPIO_SetDirection    (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);
static int32_t  GPIO_SetOutputMode   (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);
static int32_t  GPIO_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);
static int32_t  GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);
static void     GPIO_SetOutput       (ARM_GPIO_Pin_t pin, uint32_t val);
static uint32_t GPIO_GetInput        (ARM_GPIO_Pin_t pin);

// Auxiliary functions

/**
  \fn          int32_t GPIO_PinConfigure (ARM_GPIO_Pin_t pin, PinConfig_t pin_config)
  \brief       Configure the pin.
  \param[in]   pin        Pin identifier
  \param[in]   pin_config Pin configuration
  \return      \ref execution_status
*/
static int32_t GPIO_PinConfigure (ARM_GPIO_Pin_t pin, PinConfig_t pin_config) {
  GPIO_InitTypeDef gpio_init;
  GPIO_TypeDef    *ptr_gpio;
  uint32_t         pin_mask;
  uint32_t         mode;
  uint32_t         pull;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return ARM_GPIO_ERROR_PIN;
  }

  ptr_gpio = gpio_addr[pin >> 4U];              // Pointer to port peripheral

  if (ptr_gpio == NULL) {                       // If port address is invalid
    return ARM_DRIVER_ERROR;
  }

  pin_mask = 1U << (pin & 0x0FU);               // Pin mask on port

  // Determine required Mode
  mode = GPIO_MODE_INPUT;
  if (pin_config.dir == ARM_GPIO_INPUT) {       // If Input mode
    switch (pin_config.trigger) {
      case ARM_GPIO_TRIGGER_NONE:               // Input mode
        mode = GPIO_MODE_INPUT;
        break;
      case ARM_GPIO_TRIGGER_RISING_EDGE:        // Input mode with rising-edge detection
        mode = GPIO_MODE_IT_RISING;
        break;
      case ARM_GPIO_TRIGGER_FALLING_EDGE:       // Input mode with falling-edge detection
        mode = GPIO_MODE_IT_FALLING;
        break;
      case ARM_GPIO_TRIGGER_EITHER_EDGE:        // Input mode with either edge detection
        mode = GPIO_MODE_IT_RISING_FALLING;
        break;
    }
  } else {                                      // If Output mode
    switch (pin_config.pull_resistor) {
      case ARM_GPIO_PUSH_PULL:                  // Output mode with push-pull mode
        mode = GPIO_MODE_OUTPUT_PP;
        break;
      case ARM_GPIO_OPEN_DRAIN:                 // Output mode with open-drain mode
        mode = GPIO_MODE_IT_RISING;
        break;
    }
  }

  // Determine Pull setting
  pull = GPIO_NOPULL;
  switch (pin_config.pull_resistor) {
    case ARM_GPIO_PULL_NONE:                    // No pull-up or pull-down
      pull = GPIO_NOPULL;
      break;
    case ARM_GPIO_PULL_UP:                      // Pull-up
      pull = GPIO_PULLUP;
      break;
    case ARM_GPIO_PULL_DOWN:                    // Pull-down
      pull = GPIO_PULLDOWN;
      break;
  }

  // Populate GPIO_InitTypeDef structure with necessary settings
  gpio_init.Pin       = pin_mask;
  gpio_init.Mode      = mode;
  gpio_init.Pull      = pull;
#ifdef GPIO_SPEED_FREQ_VERY_HIGH
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
#else
  gpio_init.Speed     = GPIO_SPEED_FREQ_HIGH;
#endif
  gpio_init.Alternate = 0U;

  HAL_GPIO_Init(ptr_gpio, &gpio_init);          // Configure the pin

  return ARM_DRIVER_OK;
}

// Driver functions ************************************************************

/**
  \fn          int32_t ARM_GPIO_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
  \brief       Setup GPIO Interface.
  \param[in]   pin  GPIO Pin
  \param[in]   cb_event  Pointer to \ref ARM_GPIO_SignalEvent
  \return      \ref execution_status
*/
static int32_t GPIO_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event) {
  PinConfig_t   pin_config;
  GPIO_TypeDef *ptr_gpio;
  uint32_t      pin_pos;
  uint32_t      pin_mask;
  int32_t       ret;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return ARM_GPIO_ERROR_PIN;
  }

  ptr_gpio = gpio_addr[pin >> 4U];              // Pointer to port peripheral

  if (ptr_gpio == NULL) {                       // If port address is unknown
    return ARM_GPIO_ERROR_PIN;
  }

  pin_pos  = pin & 0x0FU;                       // Pin position on port (0 .. 15)
  pin_mask = 1U << pin_pos;                     // Pin mask on port

  if ((cb_event != NULL)                           &&
      (gpio0_rw_info.cb_event_pin[pin_pos] != 0U)  &&
      (gpio0_rw_info.cb_event_pin[pin_pos] != pin)) {
    // If callback cannot be registered because it is already allocated for another pin on the same position
    return ARM_DRIVER_ERROR;
  }

  // Set default configuration for a pin
  pin_config.dir           = ARM_GPIO_INPUT;
  pin_config.output_mode   = ARM_GPIO_PUSH_PULL;
  pin_config.pull_resistor = ARM_GPIO_PULL_NONE;
  pin_config.trigger       = ARM_GPIO_TRIGGER_NONE;

  // Deinitialize Pin to disable edge detection
  HAL_GPIO_DeInit(ptr_gpio, pin_mask);

  // By default reset pin state
  HAL_GPIO_WritePin(ptr_gpio, (uint16_t)pin_mask, GPIO_PIN_RESET);

  ret = GPIO_PinConfigure(pin, pin_config);     // Configure the pin

  if (ret == ARM_DRIVER_OK) {
    // Register or deregister callback
    if (cb_event != NULL) {                     // If callback is to be registered
      gpio0_rw_info.cb_event    [pin_pos] = cb_event;
      gpio0_rw_info.cb_event_pin[pin_pos] = pin & 0xFFU;
      pin_config.cb_registered = 1U;
    } else {                                    // If callback is to be deregistered
      gpio0_rw_info.cb_event    [pin_pos] = NULL;
      gpio0_rw_info.cb_event_pin[pin_pos] = 0U;
      pin_config.cb_registered = 0U;
    }

    gpio0_rw_info.pin_config[pin] = pin_config; // Update pin configuration information
  }

  return ret;
}

/**
  \fn          int32_t ARM_GPIO_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
  \brief       Set GPIO Direction.
  \param[in]   pin  GPIO Pin
  \param[in]   direction  \ref ARM_GPIO_DIRECTION
  \return      \ref execution_status
*/
static int32_t GPIO_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction) {
  PinConfig_t pin_config;
  int32_t     ret;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return ARM_GPIO_ERROR_PIN;
  }

  pin_config = gpio0_rw_info.pin_config[pin];   // Load pin configuration

  pin_config.dir = direction;                   // Set direction

  ret = GPIO_PinConfigure(pin, pin_config);     // Configure the pin

  if (ret == ARM_DRIVER_OK) {
    gpio0_rw_info.pin_config[pin] = pin_config; // Update pin configuration information
  }

  return ret;
}

/**
  \fn          int32_t ARM_GPIO_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
  \brief       Set GPIO Output Mode.
  \param[in]   pin  GPIO Pin
  \param[in]   mode  \ref ARM_GPIO_OUTPUT_MODE
  \return      \ref execution_status
*/
static int32_t GPIO_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode) {
  PinConfig_t pin_config;
  int32_t     ret;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return ARM_GPIO_ERROR_PIN;
  }

  pin_config = gpio0_rw_info.pin_config[pin];   // Load pin configuration

  pin_config.output_mode = mode;                // Set output mode

  ret = GPIO_PinConfigure(pin, pin_config);     // Configure the pin

  if (ret == ARM_DRIVER_OK) {
    gpio0_rw_info.pin_config[pin] = pin_config; // Update pin configuration information
  }

  return ret;
}

/**
  \fn          int32_t ARM_GPIO_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
  \brief       Set GPIO Pull Resistor.
  \param[in]   pin  GPIO Pin
  \param[in]   resistor  \ref ARM_GPIO_PULL_RESISTOR
  \return      \ref execution_status
*/
static int32_t GPIO_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor) {
  PinConfig_t pin_config;
  int32_t     ret;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return ARM_GPIO_ERROR_PIN;
  }

  pin_config = gpio0_rw_info.pin_config[pin];   // Load pin configuration

  pin_config.pull_resistor = resistor;          // Set pull resistor mode

  ret = GPIO_PinConfigure(pin, pin_config);     // Configure the pin

  if (ret == ARM_DRIVER_OK) {
    gpio0_rw_info.pin_config[pin] = pin_config; // Update pin configuration information
  }

  return ret;
}

/**
  \fn          int32_t ARM_GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
  \brief       Set GPIO Event Trigger.
  \param[in]   pin  GPIO Pin
  \param[in]   trigger  \ref ARM_GPIO_EVENT_TRIGGER
  \return      \ref execution_status
*/
static int32_t GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger) {
  PinConfig_t   pin_config;
  GPIO_TypeDef *ptr_gpio;
  uint32_t      pin_mask;
  int32_t       ret;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return ARM_GPIO_ERROR_PIN;
  }

  ptr_gpio = gpio_addr[pin >> 4U];              // Pointer to port peripheral

  if (ptr_gpio == NULL) {                       // If port address is unknown
    return ARM_GPIO_ERROR_PIN;
  }

  pin_mask = 1U << (pin & 0x0FU);               // Pin mask on port

  pin_config = gpio0_rw_info.pin_config[pin];   // Load pin configuration

  pin_config.trigger = trigger;                 // Set trigger

  HAL_GPIO_DeInit(ptr_gpio, pin_mask);          // Deinitialize Pin to disable edge detection

  ret = GPIO_PinConfigure(pin, pin_config);     // Configure the pin

  if (ret == ARM_DRIVER_OK) {
    gpio0_rw_info.pin_config[pin] = pin_config; // Update pin configuration information
  }

  return ret;
}

/**
  \fn          void ARM_GPIO_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val)
  \brief       Set GPIO Output Level.
  \param[in]   pin  GPIO Pin
  \param[in]   val  GPIO Pin Level (0 or 1)
*/
static void GPIO_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val) {
  GPIO_TypeDef *ptr_gpio;
  uint32_t      pin_mask;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return;
  }

  ptr_gpio = gpio_addr[pin >> 4U];              // Pointer to port peripheral

  if (ptr_gpio == NULL) {                       // If port address is unknown
    return;
  }

  pin_mask = 1U << (pin & 0x0FU);               // Pin mask on port

  if (val == 0U) {
    HAL_GPIO_WritePin(ptr_gpio, (uint16_t)pin_mask, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(ptr_gpio, (uint16_t)pin_mask, GPIO_PIN_SET);
  }
}

/**
  \fn          uint32_t ARM_GPIO_GetInput (ARM_GPIO_Pin_t pin)
  \brief       Get GPIO Input Level.
  \param[in]   pin  GPIO Pin
  \return      GPIO Pin Level (0 or 1)
*/
static uint32_t GPIO_GetInput (ARM_GPIO_Pin_t pin) {
  GPIO_TypeDef *ptr_gpio;
  uint32_t      pin_val;
  uint32_t      pin_mask;

  if (pin >= GPIO_MAX_PINS_NUM) {               // If pin id is out-of-bounds
    return 0U;
  }

  ptr_gpio = gpio_addr[pin >> 4U];              // Pointer to port peripheral

  if (ptr_gpio == NULL) {                       // If port address is unknown
    return 0U;
  }

  pin_mask = 1U << (pin & 0x0FU);               // Pin mask on port

  pin_val = 0U;
  if (HAL_GPIO_ReadPin(ptr_gpio, (uint16_t)pin_mask) != GPIO_PIN_RESET) {
    pin_val = 1U;
  }

  return pin_val;
}

// HAL callback functions ******************************************************

/**
  \fn          void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
  \brief       EXTI line detection callback.
  \param[in]   GPIO_Pin Specifies the port pin connected to corresponding EXTI line
*/
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin) {
  uint32_t pin_pos;
  uint32_t pin;

  pin_pos = POSITION_VAL(GPIO_Pin);
  pin     = gpio0_rw_info.cb_event_pin[pin_pos];

  switch (gpio0_rw_info.pin_config[pin].trigger) {
    case ARM_GPIO_TRIGGER_RISING_EDGE:
      gpio0_rw_info.cb_event[pin_pos](gpio0_rw_info.cb_event_pin[pin_pos], ARM_GPIO_EVENT_RISING_EDGE);
      break;
    case ARM_GPIO_TRIGGER_FALLING_EDGE:
      gpio0_rw_info.cb_event[pin_pos](gpio0_rw_info.cb_event_pin[pin_pos], ARM_GPIO_EVENT_FALLING_EDGE);
      break;
    case ARM_GPIO_TRIGGER_EITHER_EDGE:
      gpio0_rw_info.cb_event[pin_pos](gpio0_rw_info.cb_event_pin[pin_pos], ARM_GPIO_EVENT_EITHER_EDGE);
      break;
    default:
      break;
  }
}

/**
  \fn          void HAL_GPIO_EXTI_Rising_Callback (uint16_t GPIO_Pin)
  \brief       EXTI line rising edge detection callback.
  \param[in]   GPIO_Pin Specifies the port pin connected to corresponding EXTI line
*/
void HAL_GPIO_EXTI_Rising_Callback (uint16_t GPIO_Pin) {
  uint32_t pin_pos;

  pin_pos = POSITION_VAL(GPIO_Pin);

  if (gpio0_rw_info.cb_event[pin_pos] != NULL) {
    gpio0_rw_info.cb_event[pin_pos](gpio0_rw_info.cb_event_pin[pin_pos], ARM_GPIO_EVENT_RISING_EDGE);
  }
}

/**
  \fn          void HAL_GPIO_EXTI_Falling_Callback (uint16_t GPIO_Pin)
  \brief       EXTI line falling edge detection callback.
  \param[in]   GPIO_Pin Specifies the port pin connected to corresponding EXTI line
*/
void HAL_GPIO_EXTI_Falling_Callback (uint16_t GPIO_Pin) {
  uint32_t pin_pos;

  pin_pos = POSITION_VAL(GPIO_Pin);

  if (gpio0_rw_info.cb_event[pin_pos] != NULL) {
    gpio0_rw_info.cb_event[pin_pos](gpio0_rw_info.cb_event_pin[pin_pos], ARM_GPIO_TRIGGER_FALLING_EDGE);
  }
}

// Global driver structures ****************************************************

ARM_DRIVER_GPIO Driver_GPIO0 = {
  GPIO_Setup,
  GPIO_SetDirection,
  GPIO_SetOutputMode,
  GPIO_SetPullResistor,
  GPIO_SetEventTrigger,
  GPIO_SetOutput,
  GPIO_GetInput
};

/*! \endcond */
