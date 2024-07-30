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
 * $Date:       20. June 2024
 * $Revision:   V3.0
 *
 * Project:     MCI Driver for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

/*! \page mci_stm32 MCI

# Revision History

- Version 3.0
  - Initial release

# Requirements

This driver requires the STM32 device specific **HAL** and **STM32CubeMX** (CubeMX) initialization code generator.
The driver instance is mapped to hardware as shown in the table below:

  CMSIS Driver Instance | STM32 Hardware Resource
  :---------------------|:-----------------------
  Driver_MCI1           | SDMMC1 or SDIO
  Driver_MCI2           | SDMMC2

# CubeMX Configuration

This driver requires the following configuration in CubeMX:

  - When using SDMMC peripheral:
    - **clock**: **SDMMC** peripheral clock.
    - **peripheral**: **SDMMC** peripheral configured in **SD** or **MMC** mode.
    - **pins**: **CMD**, **CK**, **D0** - **D3** and for 8-bit MMC optionally **D4** - **D7**.
    - **interrupts**:
      - enabled **SDMMC global interrupt** with **Generate Enable in Init** and without **Generate IRQ handler**

  - When using SDIO peripheral:
    - **clock**: **SDIO** peripheral clock.
    - **peripheral**: **SDIO** peripheral configured in **SD** or **MMC** mode.
    - **pins**: **CMD**, **CK**, **D0** - **D3** and for 8-bit MMC optionally **D4** - **D7**.
    - **DMA**: **DMA** stream configuration.
    - **interrupts**:
      - enabled **SDIO global interrupt** with **Generate Enable in Init** and without **Generate IRQ handler**
      - enabled **SDIO_RX** and **SDIO_TX** DMA Requests that **Call HAL handlers**.

  - Optionally Card Detect or Write Protect pin may be configured:
    - Chose any general purpose pin as input and add **User Label**:
      - MemoryCard_1_CD to add Card Detect pin for SDMMC1 or SDIO
      - MemoryCard_1_WP to add Write Protect pin for SDMMC1
    - Similar for SDMMC2, add **User Label** as below:
      - MemoryCard_2_CD
      - MemoryCard_2_WP

> **Notes**
>
> - configuration information in the **MX_Device.h** file is based on CubeMX configuration.
> - **DMA** configuration can differ between devices series so configure DMA **as required by the used device**.
> - for **DMA** usage on devices with cache, ensure that data buffers for Transmit and Receive functions
>   are in **non-cacheable memory**, or ensure that memory for transmit is updated (**cache clean**) before Transmit function
>   is called and that memory containing received data is updated after the reception finishes (**cache invalidate**).
> - some DMA controllers can only access specific memories, so ensure that proper memory is used for the buffers
>   according to the DMA requirement.

## Example

### Pinout & Configuration tab

  1. Under **Categories**: **Connectivity** select **SDMMC1**:

     __Mode__:
       - Mode: **SD 4 bits Wide bus**

     __Configuration__:
       - Parameter Settings: not required
       - User Constants: not required

       - NVIC Settings:
           NVIC Interrupt Table    | Enabled     | Preemption Priority | Sub Priority
           :-----------------------|:-----------:|:-------------------:|:------------:
           SDMMC1 global interrupt | **checked** | 0                   | 0
         \n

       - GPIO Settings:
           Pin Name | Signal on Pin | GPIO output..| GPIO mode                     | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
           :--------|:-------------:|:------------:|:-----------------------------:|:------------------:|:------------:|:---------:|:----------:
           PC8      | SDMMC1_D0     | n/a          | Alternate Function Push Pull  | Pull-up            | Very High    | n/a       | .
           PC9      | SDMMC1_D1     | n/a          | Alternate Function Push Pull  | Pull-up            | Very High    | n/a       | .
           PC10     | SDMMC1_D2     | n/a          | Alternate Function Push Pull  | Pull-up            | Very High    | n/a       | .
           PC11     | SDMMC1_D3     | n/a          | Alternate Function Push Pull  | Pull-up            | Very High    | n/a       | .
           PC12     | SDMMC1_CK     | n/a          | Alternate Function Push Pull  | Pull-up            | Very High    | n/a       | .
           PD2      | SDMMC1_CMD    | n/a          | Alternate Function Push Pull  | Pull-up            | Very High    | n/a       | .

         - Use the **Pinout view** window to reconfigure pins when custom pin selection is required

  2. Under **Categories**: **System Core** select **NVIC**:

     __Configuration__:
       - NVIC:
           NVIC Interrupt Table    | Enabled     | Preemption Priority | Sub Priority
           :-----------------------|:-----------:|:-------------------:|:------------:
           SDMMC1 global interrupt |   checked   | 0                   | 0
         \n

       - Code generation:
           Enabled interrupt table | Select for..| Generate Enable in..| Generate IRQ h.. | Call HAL handler
           :-----------------------|:-----------:|:-------------------:|:----------------:|:----------------:
           SDMMC1 global interrupt | unchecked   | checked             | **unchecked**    | checked
         \n

  3. In the **Pinout view** window click on a Card Detect/Write Protect pin and select it's functionality:
       - Functionality:
           Pin      | GPIO mode
           :--------|:--------------------:
           PI8      | **GPIO_Input**
           PI0      | **GPIO_Input**
           \n

  4. Under **Categories**: **System Core** select **GPIO**:

     __Configuration__:
       - GPIO:
           Pin Name | Signal on Pin | GPIO output..| GPIO mode  | GPIO Pull-up/Pull..| Maximum out..| Fast Mode | User Label
           :--------|:-------------:|:------------:|:----------:|:------------------:|:------------:|:---------:|:---------------:
           PI8      | n/a           | n/a          | Input mode | Pull-up            | n/a          | n/a       | MemoryCard_1_CD
           PI0      | n/a           | n/a          | Input mode | Pull-up            | n/a          | n/a       | MemoryCard_1_WP

### Clock Configuration tab

  1. Configure **To SDMMC1,2 (MHz)**: **240**
     - Usually clock can be configured to **max** frequency.

### Project Manager tab

  1. Under **Advanced Settings**:

     __Generated Function Calls__:
       Generate Code | Function Name               | Peripheral Inst..| Do not generate ..| Visibility (Static)
       :-------------|:---------------------------:|:----------------:|:-----------------:|:-------------------:
       checked       | MX_SDMMC1_SD_Init           | SDMMC1           | **checked**       | checked
*/

/*! \cond */

#include <string.h>
#include "MCI_STM32.h"

#if defined(MX_SDMMC1) || defined(MX_SDMMC2)
  #include "MCI_STM32_SDMMC.h"
#elif defined(MX_SDIO)
  #include "MCI_STM32_SDIO.h"
#else
  #error "MCI driver requires SDMMC or SDIO peripheral configured in STM32CubeMX!"
#endif

/* Driver Version */
#define ARM_MCI_DRV_VERSION             ARM_DRIVER_VERSION_MAJOR_MINOR(3,0)


/**
  \brief MCI1: Define Card Detect pin active state
*/
#if !defined(MemoryCard_1_CD_Pin_Active)
  #define MemoryCard_1_CD_Pin_Active    GPIO_PIN_RESET
#endif

/**
  \brief MCI1: Define Write Protect pin active state
*/
#if !defined(MemoryCard_1_WP_Pin_Active)
  #define MemoryCard_1_WP_Pin_Active    GPIO_PIN_SET
#endif

/**
  \brief MCI1: Define Card Power pin active state
*/
#if !defined(MemoryCard_1_PWR_Pin_Active)
  #define MemoryCard_1_PWR_Pin_Active   GPIO_PIN_RESET
#endif

/**
  \brief MCI2: Define Card Detect pin active state
*/
#if !defined(MemoryCard_2_CD_Pin_Active)
  #define MemoryCard_2_CD_Pin_Active    GPIO_PIN_RESET
#endif

/**
  \brief MCI2: Define Write Protect pin active state
*/
#if !defined(MemoryCard_2_WP_Pin_Active)
  #define MemoryCard_2_WP_Pin_Active    GPIO_PIN_SET
#endif

/**
  \brief MCI2: Define Card Power pin active state
*/
#if !defined(MemoryCard_2_PWR_Pin_Active)
  #define MemoryCard_2_PWR_Pin_Active   GPIO_PIN_SET
#endif

/* Macro for section for RW info */
#ifdef  MCI_SECTION_NAME
#define MCIn_SECTION_(name,n)   __attribute__((section(name #n)))
#define MCIn_SECTION(n)         MCIn_SECTION_(MCI_SECTION_NAME,n)
#else
#define MCIn_SECTION(n)
#endif

/* Macro for declaring functions (for instances) */
#define FUNCS_DECLARE(n)                                                                                                        \
static ARM_MCI_CAPABILITIES MCI##n##_GetCapabilities (void);                                                                    \
static int32_t              MCI##n##_Initialize      (ARM_MCI_SignalEvent_t cb_event);                                          \
static int32_t              MCI##n##_Uninitialize    (void);                                                                    \
static int32_t              MCI##n##_PowerControl    (ARM_POWER_STATE state);                                                   \
static int32_t              MCI##n##_CardPower       (uint32_t voltage);                                                        \
static int32_t              MCI##n##_ReadCD          (void);                                                                    \
static int32_t              MCI##n##_ReadWP          (void);                                                                    \
static int32_t              MCI##n##_SendCommand     (uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response);          \
static int32_t              MCI##n##_SetupTransfer   (uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode); \
static int32_t              MCI##n##_AbortTransfer   (void);                                                                    \
static int32_t              MCI##n##_Control         (uint32_t control, uint32_t arg);                                          \
static ARM_MCI_STATUS       MCI##n##_GetStatus       (void);                                                                    \
       void                 MCI##n##_IRQ_HANDLER     (void);

/* Macro for defining functions (for instances) */
#define FUNCS_DEFINE(n)                                                                                                                                                                                   \
static ARM_MCI_CAPABILITIES MCI##n##_GetCapabilities (void)                                                                    { return GetCapabilities (&MCI##n);                                      } \
static int32_t              MCI##n##_Initialize      (ARM_MCI_SignalEvent_t cb_event)                                          { return Initialize      (cb_event, &MCI##n);                            } \
static int32_t              MCI##n##_Uninitialize    (void)                                                                    { return Uninitialize    (&MCI##n);                                      } \
static int32_t              MCI##n##_PowerControl    (ARM_POWER_STATE state)                                                   { return PowerControl    (state, &MCI##n);                               } \
static int32_t              MCI##n##_CardPower       (uint32_t voltage)                                                        { return CardPower       (voltage, &MCI##n);                             } \
static int32_t              MCI##n##_ReadCD          (void)                                                                    { return ReadCD          (&MCI##n);                                      } \
static int32_t              MCI##n##_ReadWP          (void)                                                                    { return ReadWP          (&MCI##n);                                      } \
static int32_t              MCI##n##_SendCommand     (uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response)          { return SendCommand     (cmd, arg, flags, response, &MCI##n);           } \
static int32_t              MCI##n##_SetupTransfer   (uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode) { return SetupTransfer   (data, block_count, block_size, mode, &MCI##n); } \
static int32_t              MCI##n##_AbortTransfer   (void)                                                                    { return AbortTransfer   (&MCI##n);                                      } \
static int32_t              MCI##n##_Control         (uint32_t control, uint32_t arg)                                          { return Control         (control, arg, &MCI##n);                        } \
static ARM_MCI_STATUS       MCI##n##_GetStatus       (void)                                                                    { return GetStatus       (&MCI##n);                                      } \
       void                 MCI##n##_IRQ_HANDLER     (void)                                                                    { return MCI_IRQHandler  (&MCI##n);                                      }

/* Macro for defining driver structures (for instances) */
#define MCI_DRIVER(n)            \
ARM_DRIVER_MCI Driver_MCI##n = { \
  GetVersion,                    \
  MCI##n##_GetCapabilities,      \
  MCI##n##_Initialize,           \
  MCI##n##_Uninitialize,         \
  MCI##n##_PowerControl,         \
  MCI##n##_CardPower,            \
  MCI##n##_ReadCD,               \
  MCI##n##_ReadWP,               \
  MCI##n##_SendCommand,          \
  MCI##n##_SetupTransfer,        \
  MCI##n##_AbortTransfer,        \
  MCI##n##_Control,              \
  MCI##n##_GetStatus             \
};

#if MCI1_ENABLE

/* MCI1 Information (Run-Time) */
static MCIn_SECTION(0) MCI_INFO MCI1_Info = {
  NULL,
  {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
  NULL,
  0U,
  0U,
  0U
};

#if defined(MCI_DMA_EXT)
extern DMA_HandleTypeDef hdma_sdmmc1_rx;
extern DMA_HandleTypeDef hdma_sdmmc1_tx;

static void MCI1_RX_DMA_Complete(DMA_HandleTypeDef *hdma);

static MCI_DMA MCI1_RxDma = {
  &hdma_sdmmc1_rx,
  &MCI1_RX_DMA_Complete
};

static MCI_DMA MCI1_TxDma = {
  &hdma_sdmmc1_tx,
  NULL
};
#endif

/* MCI1 Resources */
static MCI_RESOURCES MCI1 = {
  MCI1_HAL_MSPINIT,
  MCI1_HAL_MSPDEINIT,
  &MCI1_HANDLE,
  MCI1_REG_BLOCK,
  #if (MCI1_CFG_PIN_CD != 0U)
  { MemoryCard_1_CD_GPIO_Port, MemoryCard_1_CD_Pin, MemoryCard_1_CD_Pin_Active },
  #else
  { NULL, 0U, 0U },
  #endif
  #if (MCI1_CFG_PIN_WP != 0U)
  { MemoryCard_1_WP_GPIO_Port, MemoryCard_1_WP_Pin, MemoryCard_1_WP_Pin_Active },
  #else
  { NULL, 0U, 0U },
  #endif
  #if (MCI1_CFG_PIN_PWR != 0U)
  { MemoryCard_1_PWR_GPIO_Port, MemoryCard_1_PWR_Pin, MemoryCard_1_PWR_Pin_Active },
  #else
  { NULL, 0U, 0U },
  #endif
  #if defined(MCI_DMA_EXT)
  MCI1_RxDma,
  MCI1_TxDma,
  #endif
  MCI1_CFG,
  &MCI1_Info
};

#if defined(MCI_DMA_EXT)
/* DMA RX complete callback */
static void MCI1_RX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  RX_DMA_Complete (hdma, &MCI1);
}
#endif

#endif /* MCI1_ENABLE */

#if MCI2_ENABLE

/* MCI2 Information (Run-Time) */
static MCIn_SECTION(0) MCI_INFO MCI2_Info = {
  NULL,
  {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
  NULL,
  0U,
  0U,
  0U
};

#if defined(MCI_DMA_EXT)
extern DMA_HandleTypeDef hdma_sdmmc2_rx;
extern DMA_HandleTypeDef hdma_sdmmc2_tx;

static void MCI2_RX_DMA_Complete(DMA_HandleTypeDef *hdma);

static MCI_DMA MCI2_RxDma = {
  &hdma_sdmmc2_rx,
  &MCI2_RX_DMA_Complete
};

static MCI_DMA MCI2_TxDma = {
  &hdma_sdmmc2_tx,
  NULL
};
#endif

/* MCI2 Resources */
static MCI_RESOURCES MCI2 = {
  MCI2_HAL_MSPINIT,
  MCI2_HAL_MSPDEINIT,
  &MCI2_HANDLE,
  MCI2_REG_BLOCK,
  #if (MCI2_CFG_PIN_CD != 0U)
  { MemoryCard_2_CD_GPIO_Port, MemoryCard_2_CD_Pin, MemoryCard_2_CD_Pin_Active },
  #else
  { NULL, 0U, 0U },
  #endif
  #if (MCI2_CFG_PIN_WP != 0U)
  { MemoryCard_2_WP_GPIO_Port, MemoryCard_2_WP_Pin, MemoryCard_2_WP_Pin_Active },
  #else
  { NULL, 0U, 0U },
  #endif
  #if (MCI2_CFG_PIN_PWR != 0U)
  { MemoryCard_2_PWR_GPIO_Port, MemoryCard_2_PWR_Pin, MemoryCard_2_PWR_Pin_Active },
  #else
  { NULL, 0U, 0U },
  #endif
  #if defined(MCI_DMA_EXT)
  MCI2_RxDma,
  MCI2_TxDma,
  #endif
  MCI2_CFG,
  &MCI2_Info
};

#if defined(MCI_DMA_EXT)
/* DMA RX complete callback */
static void MCI2_RX_DMA_Complete(DMA_HandleTypeDef *hdma) {
  RX_DMA_Complete (hdma, &MCI2);
}
#endif

#endif /* MCI2_ENABLE */


/**
  \fn          void Assign_SDMMC_Instance (uint32_t set, MCI_RESOURCES *mci)
  \brief       Set/Reset instance variable inside HAL handle structure
*/
static void Assign_SDMMC_Instance (uint32_t set, MCI_RESOURCES *mci) {
#if (MCI1_ENABLE && (MCI1_HANDLE_TYPE == 0)) || \
    (MCI2_ENABLE && (MCI2_HANDLE_TYPE == 0))
  SD_HandleTypeDef *h_sd;
#endif
#if (MCI1_ENABLE && (MCI1_HANDLE_TYPE == 1)) || \
    (MCI2_ENABLE && (MCI2_HANDLE_TYPE == 1))
  MMC_HandleTypeDef *h_mmc;
#endif

#if MCI1_ENABLE
  if (mci == &MCI1) {
    #if (MCI1_HANDLE_TYPE == 0)
      h_sd = (SD_HandleTypeDef *)mci->h;
      /* Instance is the corresponding peripheral register interface */
      if (set == 0) {
        h_sd->Instance = NULL;
      } else {
        h_sd->Instance = mci->reg;
      }
    #else
      h_mmc = (MMC_HandleTypeDef *)mci->h;
      /* Instance is the corresponding peripheral register interface */
      if (set == 0) {
        h_mmc->Instance = NULL;
      } else {
        h_mmc->Instance = mci->reg;
      }
    #endif
  }
#endif

#if MCI2_ENABLE
  if (mci == &MCI2) {
    #if (MCI2_HANDLE_TYPE == 0)
      h_sd = (SD_HandleTypeDef *)mci->h;
      /* Instance is the corresponding peripheral register interface */
      if (set == 0) {
        h_sd->Instance = NULL;
      } else {
        h_sd->Instance = mci->reg;
      }
    #else
      h_mmc = (MMC_HandleTypeDef *)mci->h;
      /* Instance is the corresponding peripheral register interface */
      if (set == 0) {
        h_mmc->Instance = NULL;
      } else {
        h_mmc->Instance = mci->reg;
      }
    #endif
  }
#endif
}


/**
  \fn            int32_t MCI_ReadCD (uint32_t instance)
  \brief         Read Card Detect (CD) state.
  \param[in]     instance  MCI driver instance number (1:Driver_MCI1, 2:Driver_MCI2)
  \return        1:card detected, 0:card not detected, -1: functionality not available
  \note This function can be reimplemented in user application
*/
__WEAK int32_t MCI_ReadCD (uint32_t instance) {
  MCI_RESOURCES *mci = NULL;
  int32_t rval = 0;

#if MCI1_ENABLE
  if (instance == 1U) { mci = &MCI1; }
#endif
#if MCI2_ENABLE
  if (instance == 2U) { mci = &MCI2; }
#endif

  /* Read CD (Card Detect) Pin */
  if ((mci != NULL ) && (mci->io_cd.port != NULL)) {
    /* Note: io->active holds MemoryCard_CD_Pin_Active definition */
    if (HAL_GPIO_ReadPin(mci->io_cd.port, mci->io_cd.pin) == mci->io_cd.active) {
      /* Card Detect switch is active */
      rval = 1;
    }
  } else {
    rval = -1;
  }

  return (rval);
}

/**
  \fn            int32_t MCI_ReadWP (uint32_t instance)
  \brief         Read Write Protect (WP) state.
  \param[in]     instance  MCI driver instance number (1:Driver_MCI1, 2:Driver_MCI2)
  \return        1:write protected, 0:not write protected, -1: functionality not available
  \note This function can be reimplemented in user application
*/
__WEAK int32_t MCI_ReadWP (uint32_t instance) {
  MCI_RESOURCES *mci = NULL;
  int32_t rval = 0;

#if MCI1_ENABLE
  if (instance == 1U) { mci = &MCI1; }
#endif
#if MCI2_ENABLE
  if (instance == 2U) { mci = &MCI2; }
#endif

  if ((mci != NULL) && (mci->io_wp.port != NULL)) {
    /* Note: io->active holds MemoryCard_WP_Pin_Active definition */
    if (HAL_GPIO_ReadPin (mci->io_wp.port, mci->io_wp.pin) == mci->io_wp.active) {
      /* Write protect switch is active */
      rval = 1;
    }
  } else {
    rval = -1;
  }

  return (rval);
}

/**
  \fn            int32_t MCI_CardPower (uint32_t instance, uint32_t voltage)
  \brief         Set Card Power to the specified voltage level.
  \param[in]     instance  MCI driver instance number (1:Driver_MCI1, 2:Driver_MCI2)
  \param[in]     voltage   voltage specified with ARM_MCI_POWER_Vxx define
  \return        1:voltage set, 0:voltage not set, -1: functionality not available
  \note This function can be reimplemented in user application
*/
__WEAK int32_t MCI_CardPower (uint32_t instance, uint32_t voltage) {
  MCI_RESOURCES *mci = NULL;
  int32_t rval;

#if MCI1_ENABLE
  if (instance == 1U) { mci = &MCI1; }
#endif
#if MCI2_ENABLE
  if (instance == 2U) { mci = &MCI2; }
#endif

  if ((mci == NULL) || (mci->io_pwr.port == NULL)) {
    return -1;
  }

  /* Note: io->active holds MemoryCard_PWR_Pin_Active definition */
  if ((voltage & ARM_MCI_POWER_VDD_3V3) || (voltage & ARM_MCI_POWER_VCCQ_3V3)) {
    HAL_GPIO_WritePin (mci->io_pwr.port, mci->io_pwr.pin, mci->io_pwr.active);

    rval = 1;
  }
  else if ((voltage & ARM_MCI_POWER_VDD_1V8) || (voltage & ARM_MCI_POWER_VCCQ_1V8)) {
    HAL_GPIO_WritePin (mci->io_pwr.port, mci->io_pwr.pin, !mci->io_pwr.active);

    rval = 1;
  }
  else if ((voltage & ARM_MCI_POWER_VDD_OFF) || (voltage & ARM_MCI_POWER_VCCQ_OFF)) {
    rval = 1;
  }
  else {
    rval = 0;
  }

  return (rval);
}


/**
  \fn          ARM_DRV_VERSION GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION GetVersion (void) {
  const ARM_DRIVER_VERSION version = { ARM_MCI_API_VERSION,
                                       ARM_MCI_DRV_VERSION };
  return version;
}


/**
  \fn          ARM_MCI_CAPABILITIES MCI_GetCapabilities (MCI_RESOURCES *mci)
  \brief       Get driver capabilities.
  \return      \ref ARM_MCI_CAPABILITIES
*/
static ARM_MCI_CAPABILITIES GetCapabilities (MCI_RESOURCES *mci) {
  ARM_MCI_CAPABILITIES cap;
  uint32_t instance = 0U;

  memset((void *)&cap, 0U, sizeof(ARM_MCI_CAPABILITIES));
#if MCI1_ENABLE
  if (mci->reg == MCI1_REG_BLOCK) {
    instance = 1U;
  }
#endif
#if MCI2_ENABLE
  if (mci->reg == MCI2_REG_BLOCK) {
    instance = 2U;
  }
#endif
  /* Check whether Card Detect and Write Protect pin detection is available */
  if (MCI_ReadCD(instance) != -1) {
    cap.cd_state = 1U;
  }
  if (MCI_ReadWP(instance) != -1) {
    cap.wp_state = 1U;
  }
  /* Check whether 4-bit and 8-bit data bus is available */
  if (mci->cfg & MCI_CFG_BUS_WIDTH_4) {
    cap.data_width_4 = 1U;
  }
  if (mci->cfg & MCI_CFG_BUS_WIDTH_8) {
    cap.data_width_8 = 1U;
  }
  /* Check whether Card Power pin is available */
  if (MCI_CardPower(instance, ARM_MCI_POWER_VDD_OFF) != -1) {
    cap.vdd      = 1U;
    cap.vdd_1v8  = 1U;
    cap.vccq     = 1U;
    cap.vccq_1v8 = 1U;
  }
  /* Set controller specific capabilities */
  cap.high_speed     = 1U;
  cap.sdio_interrupt = 1U;
  cap.read_wait      = 1U;

  return (cap);
}


/**
  \fn            int32_t Initialize (ARM_MCI_SignalEvent_t cb_event, MCI_RESOURCES *mci)
  \brief         Initialize the Memory Card Interface
  \param[in]     cb_event  Pointer to \ref ARM_MCI_SignalEvent
  \return        \ref execution_status
*/
static int32_t Initialize (ARM_MCI_SignalEvent_t cb_event, MCI_RESOURCES *mci) {

  if (mci->info->flags & MCI_INIT) { return ARM_DRIVER_OK; }

  /* Set SDMMC instance pointer */
  Assign_SDMMC_Instance (1U, mci);

  /* Clear control structure */
  memset ((void *)mci->info, 0, sizeof (MCI_INFO));

  MCI_Init_Peripheral(mci);

  mci->info->cb_event = cb_event;
  mci->info->flags    = MCI_INIT;

  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t Uninitialize (MCI_RESOURCES *mci)
  \brief         De-initialize Memory Card Interface.
  \return        \ref execution_status
*/
static int32_t Uninitialize (MCI_RESOURCES *mci) {

  mci->info->flags = 0U;

  /* Reset SDMMC instance pointer */
  Assign_SDMMC_Instance (0U, mci);

  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t PowerControl (ARM_POWER_STATE state, MCI_RESOURCES *mci)
  \brief         Control Memory Card Interface Power.
  \param[in]     state   Power state \ref ARM_POWER_STATE
  \return        \ref execution_status
*/
static int32_t PowerControl (ARM_POWER_STATE state, MCI_RESOURCES *mci) {
  int32_t status;

  if ((state != ARM_POWER_OFF)  &&
      (state != ARM_POWER_FULL) &&
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  status = ARM_DRIVER_OK;

  switch (state) {
    case ARM_POWER_OFF:
      /* Reset/Dereset SDMMC peripheral */
      MCI_Reset_Peripheral(mci);

      /* Call HAL SD/MMC MSPDeInit function */
      mci->msp_deinit(mci->h);

      /* Clear status */
      mci->info->status.command_active   = 0U;
      mci->info->status.command_timeout  = 0U;
      mci->info->status.command_error    = 0U;
      mci->info->status.transfer_active  = 0U;
      mci->info->status.transfer_timeout = 0U;
      mci->info->status.transfer_error   = 0U;
      mci->info->status.sdio_interrupt   = 0U;
      mci->info->status.ccs              = 0U;

      mci->info->flags &= ~MCI_POWER;
      break;

    case ARM_POWER_FULL:
      if ((mci->info->flags & MCI_INIT)  == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((mci->info->flags & MCI_POWER) != 0U) {
        return ARM_DRIVER_OK;
      }

      /* Call HAL SD/MMC MSPInit function */
      mci->msp_init(mci->h);

      /* Get SDMMC kernel clock frequency */
      mci->info->ker_clk = MCI_Get_PeriphCLKFreq(mci);

      /* Clear response variable */
      mci->info->response = NULL;

      /* Enable MCI peripheral interrupts */
      MCI_Enable_Interrupt(mci, MCI_IT_CCRCFAIL  |
                                MCI_IT_DCRCFAIL  |
                                MCI_IT_CTIMEOUT  |
                                MCI_IT_DTIMEOUT  |
                                MCI_IT_CMDREND   |
                                MCI_IT_CMDSENT   |
                                MCI_IT_DATAEND   |
                                MCI_IT_BUSYD0END);

      /* Set max data timeout */
      MCI_Set_DTimeout(mci, 0xFFFFFFFF);

      /* Set transceiver polarity */
      if (mci->cfg & MCI_CFG_DIR_POLARITY) {
        MCI_Set_DirIOPolarity_High(mci);
      } else {
        MCI_Set_DirIOPolarity_Low(mci);
      }

      /* Set maximum clock divider */
      MCI_Set_ClockDivider(mci, 0x3FFU);

      /* Enable clock to the card */
      MCI_Enable_ClockOutput(mci);

      mci->info->flags |= MCI_POWER;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return status;
}


/**
  \fn            int32_t CardPower (uint32_t voltage, MCI_RESOURCES *mci)
  \brief         Set Memory Card supply voltage.
  \param[in]     voltage  Memory Card supply voltage
  \return        \ref execution_status
*/
static int32_t CardPower (uint32_t voltage, MCI_RESOURCES *mci) {
  uint32_t instance = 0U;
  int32_t val;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

#if MCI1_ENABLE
  if (mci->reg == MCI1_REG_BLOCK) { instance = 1U; }
#endif
#if MCI2_ENABLE
  if (mci->reg == MCI2_REG_BLOCK) { instance = 2U; }
#endif

  val = MCI_CardPower(instance, voltage);

  return (val);
}


/**
  \fn            int32_t ReadCD (MCI_RESOURCES *mci)
  \brief         Read Card Detect (CD) state.
  \return        1:card detected, 0:card not detected, or error
*/
static int32_t ReadCD (MCI_RESOURCES *mci) {
  uint32_t instance = 0U;
  int32_t val;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

#if MCI1_ENABLE
  if (mci->reg == MCI1_REG_BLOCK) { instance = 1U; }
#endif
#if MCI2_ENABLE
  if (mci->reg == MCI2_REG_BLOCK) { instance = 2U; }
#endif

  val = MCI_ReadCD(instance);

  return (val);
}


/**
  \fn            int32_t ReadWP (MCI_RESOURCES *mci)
  \brief         Read Write Protect (WP) state.
  \return        1:write protected, 0:not write protected, or error
*/
static int32_t ReadWP (MCI_RESOURCES *mci) {
  uint32_t instance = 0U;
  int32_t val;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

#if MCI1_ENABLE
  if (mci->reg == MCI1_REG_BLOCK) { instance = 1U; }
#endif
#if MCI2_ENABLE
  if (mci->reg == MCI2_REG_BLOCK) { instance = 2U; }
#endif

  val = MCI_ReadWP(instance);

  return (val);
}


/**
  \fn            int32_t SendCommand (uint32_t  cmd,
                                      uint32_t  arg,
                                      uint32_t  flags,
                                      uint32_t *response,
                                      MCI_RESOURCES *mci)
  \brief         Send Command to card and get the response.
  \param[in]     cmd       Memory Card command
  \param[in]     arg       Command argument
  \param[in]     flags     Command flags
  \param[out]    response  Pointer to buffer for response
  \return        \ref execution_status
*/
static int32_t SendCommand (uint32_t cmd, uint32_t arg, uint32_t flags, uint32_t *response, MCI_RESOURCES *mci) {
  uint32_t i;
  uint32_t cflags;

  if (((flags & MCI_RESPONSE_EXPECTED_Msk) != 0U) && (response == NULL)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if ((mci->info->flags & MCI_SETUP) == 0U) {
    return ARM_DRIVER_ERROR;
  }
  if (mci->info->status.command_active) {
    return ARM_DRIVER_ERROR_BUSY;
  }
  mci->info->status.command_active   = 1U;
  mci->info->status.command_timeout  = 0U;
  mci->info->status.command_error    = 0U;
  mci->info->status.transfer_timeout = 0U;
  mci->info->status.transfer_error   = 0U;
  mci->info->status.ccs              = 0U;

  if (flags & ARM_MCI_CARD_INITIALIZE) {
    MCI_Disable_ClockPowerSave(mci);

    /* Determine peripheral output frequency */
    i = mci->info->ker_clk / MCI_Get_ClockDivider(mci);

    /* Determine ratio between CPU and peripheral output frequency */
    i = SystemCoreClock / i;

    for (i *= 74; i; i--) {
      __NOP(); /* Wait for at least 74 cycles */
    }

    MCI_Enable_ClockPowerSave(mci);
  }

  if (cmd == 11U) {
    /* Voltage switch procedure enable */
    MCI_Start_VSwitch(mci);
  }

  mci->info->response = response;
  mci->info->flags   &= ~(MCI_RESP_CRC | MCI_RESP_LONG);

  switch (flags & ARM_MCI_RESPONSE_Msk) {
    case ARM_MCI_RESPONSE_NONE:
      /* No response expected (wait CMDSENT) */
      cflags = MCI_RESPONSE_NO;
      break;

    case ARM_MCI_RESPONSE_SHORT:
    case ARM_MCI_RESPONSE_SHORT_BUSY:
      /* Short response expected (wait CMDREND or CCRCFAIL) */
      cflags = MCI_RESPONSE_SHORT;
      break;

    case ARM_MCI_RESPONSE_LONG:
      mci->info->flags |= MCI_RESP_LONG;
      /* Long response expected (wait CMDREND or CCRCFAIL) */
      cflags = MCI_RESPONSE_LONG;
      break;

    default:
      return ARM_DRIVER_ERROR;
  }
  if (flags & ARM_MCI_RESPONSE_CRC) {
    mci->info->flags |= MCI_RESP_CRC;
  }
  if (flags & ARM_MCI_TRANSFER_DATA) {
    mci->info->flags |= MCI_DATA_XFER;
  }

  /* Clear all interrupt flags */
  MCI_Clear_Interrupt(mci, MCI_IT_CLR_Msk);

  /* Start command transfer */
  MCI_Start_CTransfer(mci, cmd, arg, cflags);

  return ARM_DRIVER_OK;
}


/**
  \fn            int32_t SetupTransfer (uint8_t *data,
                                        uint32_t block_count,
                                        uint32_t block_size,
                                        uint32_t mode,
                                        MCI_RESOURCES *mci)
  \brief         Setup read or write transfer operation.
  \param[in,out] data         Pointer to data block(s) to be written or read
  \param[in]     block_count  Number of blocks
  \param[in]     block_size   Size of a block in bytes
  \param[in]     mode         Transfer mode
  \return        \ref execution_status
*/
static int32_t SetupTransfer (uint8_t *data, uint32_t block_count, uint32_t block_size, uint32_t mode, MCI_RESOURCES *mci) {
  uint32_t xfer_flags;

  if ((data == NULL) || (block_count == 0U) || (block_size == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if ((mci->info->flags & MCI_SETUP) == 0U) {
    return ARM_DRIVER_ERROR;
  }
  if (mci->info->status.transfer_active) {
    return ARM_DRIVER_ERROR_BUSY;
  }
  if (block_size > MCI_MAX_BLOCK_SIZE) {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  xfer_flags = 0U;

  if ((mode & ARM_MCI_TRANSFER_WRITE) == 0) {
    /* Direction: From card to controller */
    mci->info->flags |= MCI_DATA_READ;
    xfer_flags |= MCI_DTRANSFER_MODE_READ;
  }
  else {
    /* Direction: From controller to card */
    mci->info->flags &= ~MCI_DATA_READ;
    xfer_flags |= MCI_DTRANSFER_MODE_WRITE;
  }

  if (mode & ARM_MCI_TRANSFER_STREAM) {
    /* Stream or SDIO multibyte data transfer enable */
    xfer_flags |= MCI_DTRANSFER_MODE_STREAM;
  }
  else {
    /* Block data transfer enable */
    xfer_flags |= MCI_DTRANSFER_MODE_BLOCK;
  }

  MCI_Setup_DTransfer(mci, data, block_count, block_size);

  mci->info->xfer_flags = xfer_flags;

  return (ARM_DRIVER_OK);
}


/**
  \fn            int32_t AbortTransfer (MCI_RESOURCES *mci)
  \brief         Abort current read/write data transfer.
  \return        \ref execution_status
*/
static int32_t AbortTransfer (MCI_RESOURCES *mci) {
  int32_t  status;
  uint32_t mask;

  if ((mci->info->flags & MCI_SETUP) == 0U) { return ARM_DRIVER_ERROR; }

  status = ARM_DRIVER_OK;

  /* Disable interrupts */
  mask = MCI_Get_InterruptMask(mci);
  MCI_Disable_Interrupt(mci, mask);

  status = MCI_Abort_DTransfer(mci);

  mci->info->status.command_active  = 0U;
  mci->info->status.transfer_active = 0U;
  mci->info->status.sdio_interrupt  = 0U;
  mci->info->status.ccs             = 0U;

  /* Clear pending interrupts */
  MCI_Clear_Interrupt(mci, MCI_IT_CLR_Msk);

  /* Enable interrupts */
  MCI_Enable_Interrupt(mci, mask);

  return status;
}


/**
  \fn            int32_t Control (uint32_t control, uint32_t arg, MCI_RESOURCES *mci)
  \brief         Control MCI Interface.
  \param[in]     control  Operation
  \param[in]     arg      Argument of operation (optional)
  \return        \ref execution_status
*/
static int32_t Control (uint32_t control, uint32_t arg, MCI_RESOURCES *mci) {
  uint32_t clkdiv, bps;

  if ((mci->info->flags & MCI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

  switch (control) {
    case ARM_MCI_BUS_SPEED:
      /* Determine clock divider and set bus speed */
      clkdiv = ((mci->info->ker_clk - 1U) / arg) + 1U;
      clkdiv = clkdiv & 0x3FFU;

      /* Set new clock divider */
      MCI_Set_ClockDivider(mci, clkdiv);

      /* Calculate actual clock (in bps) */
      bps = mci->info->ker_clk / MCI_Get_ClockDivider(mci);

      /* Bus speed configured */
      mci->info->flags |= MCI_SETUP;
      return ((int32_t)bps);

    case ARM_MCI_BUS_SPEED_MODE:
      switch (arg) {
        case ARM_MCI_BUS_DEFAULT_SPEED:
          /* Speed mode up to 25MHz */
          if (MCI_Set_BusSpeedMode_DS(mci) == ARM_DRIVER_OK) {
            break;
          }
        case ARM_MCI_BUS_HIGH_SPEED:
          /* Speed mode up to 50MHz */
          if (MCI_Set_BusSpeedMode_HS(mci) == ARM_DRIVER_OK) {
            break;
          }
        case ARM_MCI_BUS_UHS_SDR12:
          /* SDR12:  up to  25MHz,  12.5MB/s: UHS-I 1.8V signaling */
          if (MCI_Set_BusSpeedMode_SDR12(mci) == ARM_DRIVER_OK) {
            break;
          }
        case ARM_MCI_BUS_UHS_SDR25:
          /* SDR25:  up to  50MHz,  25  MB/s: UHS-I 1.8V signaling */
          if (MCI_Set_BusSpeedMode_SDR25(mci) == ARM_DRIVER_OK) {
            break;
          }
        case ARM_MCI_BUS_UHS_SDR50:
          /* SDR50:  up to 100MHz,  50  MB/s: UHS-I 1.8V signaling */
          if (MCI_Set_BusSpeedMode_SDR50(mci) == ARM_DRIVER_OK) {
            break;
          }
        case ARM_MCI_BUS_UHS_SDR104:
          /* SDR104: up to 208MHz, 104  MB/s: UHS-I 1.8V signaling */
          if (MCI_Set_BusSpeedMode_SDR104(mci) == ARM_DRIVER_OK) {
            break;
          }
        case ARM_MCI_BUS_UHS_DDR50:
          /* DDR50:  up to  50MHz,  50  MB/s: UHS-I 1.8V signaling */
          if (MCI_Set_BusSpeedMode_DDR50(mci) == ARM_DRIVER_OK) {
            break;
          }

        default: return ARM_DRIVER_ERROR_UNSUPPORTED;
      }
      break;

    case ARM_MCI_BUS_CMD_MODE:
      switch (arg) {
        case ARM_MCI_BUS_CMD_OPEN_DRAIN:
          /* Configure command line in open-drain mode */
          break;
        case ARM_MCI_BUS_CMD_PUSH_PULL:
          /* Configure command line in push-pull mode */
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }
      break;

    case ARM_MCI_BUS_DATA_WIDTH:
      /* Set data bus width */
      switch (arg) {
        case ARM_MCI_BUS_DATA_WIDTH_1:
          MCI_Set_BusWidth(mci, MCI_BUS_WIDTH_1);
          break;
        case ARM_MCI_BUS_DATA_WIDTH_4:
          MCI_Set_BusWidth(mci, MCI_BUS_WIDTH_4);
          break;
        case ARM_MCI_BUS_DATA_WIDTH_8:
          MCI_Set_BusWidth(mci, MCI_BUS_WIDTH_8);
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }
      break;

    case ARM_MCI_CONTROL_CLOCK_IDLE:
      if (arg) {
        /* Clock generation enabled when idle */
        MCI_Disable_ClockPowerSave(mci);
      }
      else {
        /* Clock generation disabled when idle */
        MCI_Enable_ClockPowerSave(mci);
      }
      break;

    case ARM_MCI_DATA_TIMEOUT:
      MCI_Set_DTimeout(mci, arg);
      break;

    case ARM_MCI_MONITOR_SDIO_INTERRUPT:
      mci->info->status.sdio_interrupt = 0U;
      MCI_Enable_Interrupt(mci, MCI_IT_SDIOIT);
      break;

    case ARM_MCI_CONTROL_READ_WAIT:
      if (arg) {
        /* Assert read wait */
        mci->info->flags |= MCI_READ_WAIT;
      }
      else {
        /* Clear read wait */
        mci->info->flags &= ~MCI_READ_WAIT;
        MCI_Stop_ReadWait(mci);
      }
      break;

    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}


/**
  \fn            ARM_MCI_STATUS GetStatus (MCI_RESOURCES *mci)
  \brief         Get MCI status.
  \return        MCI status \ref ARM_MCI_STATUS
*/
static ARM_MCI_STATUS GetStatus (MCI_RESOURCES *mci) {
  return mci->info->status;
}


/* SDMMC interrupt handler */
static void MCI_IRQHandler (MCI_RESOURCES *mci) {
  uint32_t sta, icr, event, mask;

  event = 0U;
  icr   = 0U;

  /* Read SDMMC interrupt status */
  sta = MCI_Get_InterruptStatus(mci);

  if (sta & MCI_IT_ERR_Msk) {
    /* Check error interrupts */
    if (sta & MCI_IT_CCRCFAIL) {
      icr |= MCI_IT_CCRCFAIL;
      /* Command response CRC check failed */
      if (mci->info->flags & MCI_RESP_CRC) {
        mci->info->status.command_error = 1U;

        event |= ARM_MCI_EVENT_COMMAND_ERROR;
      }
      else {
        /* Ignore CRC error and read the response */
        sta |= MCI_IT_CMDREND;
      }
    }
    if (sta & MCI_IT_DCRCFAIL) {
      icr |= MCI_IT_DCRCFAIL;
      /* Data block CRC check failed */
      mci->info->status.transfer_error = 1U;

      event |= ARM_MCI_EVENT_TRANSFER_ERROR;
    }
    if (sta & MCI_IT_CTIMEOUT) {
      icr |= MCI_IT_CTIMEOUT;
      /* Command response timeout */
      mci->info->status.command_timeout = 1U;

      event |= ARM_MCI_EVENT_COMMAND_TIMEOUT;
    }
    if (sta & MCI_IT_DTIMEOUT) {
      icr |= MCI_IT_DTIMEOUT;
      /* Data timeout */
      mci->info->status.transfer_timeout = 1U;

      event |= ARM_MCI_EVENT_TRANSFER_TIMEOUT;
    }
  }

  if (sta & MCI_IT_CMDREND) {
    /* Command response received */
    icr |= MCI_IT_CMDREND;

    if (!(sta & MCI_IT_BUSYD0)) {
      /* BusyD0 signal not active */
      event |= ARM_MCI_EVENT_COMMAND_COMPLETE;
    }
  }
  if (sta & MCI_IT_BUSYD0END) {
    icr |= MCI_IT_BUSYD0END;
    /* BusyD0 signal deactivated */
    event |= ARM_MCI_EVENT_COMMAND_COMPLETE;
  }

  if (event & ARM_MCI_EVENT_COMMAND_COMPLETE) {
    if (mci->info->response) {
      /* Read response registers */
      if (mci->info->flags & MCI_RESP_LONG) {
        MCI_Read_RespLong(mci, mci->info->response);
      }
      else {
        MCI_Read_RespShort(mci, mci->info->response);
      }
    }
    if (mci->info->flags & MCI_DATA_XFER) {
      mci->info->flags &= ~MCI_DATA_XFER;

      if (mci->info->flags & MCI_READ_WAIT) {
        MCI_Start_ReadWait(mci);
      }

      /* Start data transfer */
      MCI_Start_DTransfer(mci, mci->info->xfer_flags);

      mci->info->status.transfer_active = 1U;
    }
  }
  if (sta & MCI_IT_CMDSENT) {
    icr |= MCI_IT_CMDSENT;
    /* Command sent (no response required) */
    event |= ARM_MCI_EVENT_COMMAND_COMPLETE;
  }
  if (sta & MCI_IT_DATAEND) {
    icr |= MCI_IT_DATAEND;
    #if !defined(MCI_DMA_EXT)
    /* Data end (DCOUNT is zero) */
    event |= ARM_MCI_EVENT_TRANSFER_COMPLETE;
    #endif
  }
  if (sta & MCI_IT_DBCKEND) {
    icr |= MCI_IT_DBCKEND;
    /* Data block sent/received (CRC check passed) */
    #if defined(MCI_DMA_EXT)
    if ((mci->info->flags & MCI_DATA_READ) == 0) {
      /* Write transfer */
      event |= ARM_MCI_EVENT_TRANSFER_COMPLETE;
    }
    #endif
    
  }
  if (sta & MCI_IT_SDIOIT) {
    icr |= MCI_IT_SDIOIT;
    /* Disable interrupt (must be re-enabled using Control) */
    MCI_Disable_Interrupt(mci, MCI_IT_SDIOIT);

    event |= ARM_MCI_EVENT_SDIO_INTERRUPT;
  }

  /* Clear processed interrupts */
  MCI_Clear_Interrupt(mci, icr);

  if (event) {
    /* Check for transfer events */
    mask = ARM_MCI_EVENT_TRANSFER_ERROR   |
           ARM_MCI_EVENT_TRANSFER_TIMEOUT |
           ARM_MCI_EVENT_TRANSFER_COMPLETE;
    if (event & mask) {
      mci->info->status.transfer_active = 0U;

      if (mci->info->cb_event) {
        if (event & ARM_MCI_EVENT_TRANSFER_ERROR) {
          (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_ERROR);
        }
        else if (event & ARM_MCI_EVENT_TRANSFER_TIMEOUT) {
          (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_TIMEOUT);
        }
        else {
          (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_COMPLETE);
        }
      }
    }
    /* Check for command events */
    mask = ARM_MCI_EVENT_COMMAND_ERROR   |
           ARM_MCI_EVENT_COMMAND_TIMEOUT |
           ARM_MCI_EVENT_COMMAND_COMPLETE;
    if (event & mask) {
      mci->info->status.command_active = 0U;

      if (mci->info->cb_event) {
        if (event & ARM_MCI_EVENT_COMMAND_ERROR) {
          (mci->info->cb_event)(ARM_MCI_EVENT_COMMAND_ERROR);
        }
        else if (event & ARM_MCI_EVENT_COMMAND_TIMEOUT) {
          (mci->info->cb_event)(ARM_MCI_EVENT_COMMAND_TIMEOUT);
        }
        else {
          (mci->info->cb_event)(ARM_MCI_EVENT_COMMAND_COMPLETE);
        }
      }
    }
    /* Check for SDIO INT event */
    if (event & ARM_MCI_EVENT_SDIO_INTERRUPT) {
      mci->info->status.sdio_interrupt = 1U;

      if (mci->info->cb_event) {
        (mci->info->cb_event)(ARM_MCI_EVENT_SDIO_INTERRUPT);
      }
    }
  }
}

// Local driver functions definitions (for instances)
#if MCI1_ENABLE
FUNCS_DECLARE(1)
#endif
#if MCI2_ENABLE
FUNCS_DECLARE(2)
#endif

#if MCI1_ENABLE
FUNCS_DEFINE(1)
#endif
#if MCI2_ENABLE
FUNCS_DEFINE(2)
#endif


// Global driver structures ****************************************************

#if MCI1_ENABLE
MCI_DRIVER(1)
#endif

#if MCI2_ENABLE
MCI_DRIVER(2)
#endif

/*! \endcond */
