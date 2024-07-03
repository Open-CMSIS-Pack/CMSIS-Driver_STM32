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
 */

#ifndef MCI_STM32_SDMMC_H_
#define MCI_STM32_SDMMC_H_

#include "MCI_STM32.h"

#include "RTE_Components.h"
#include CMSIS_device_header

#include "main.h"

/* SDMMC peripheral version that requires external DMA streams (no dedicated DMA) */
#if defined(MX_SDMMC1_RX_DMA_Instance) && defined(MX_SDMMC1_TX_DMA_Instance) || \
    defined(MX_SDMMC2_RX_DMA_Instance) && defined(MX_SDMMC2_TX_DMA_Instance)
  #define MCI_SDMMC_V1
  #define MCI_DMA_EXT
#endif

/* MCI1: Define 4-bit data bus width */
#if defined(MX_SDMMC1_D0_Pin) && defined(MX_SDMMC1_D1_Pin) && defined(MX_SDMMC1_D2_Pin) && defined(MX_SDMMC1_D3_Pin)
  #define MCI1_CFG_BUS_WIDTH_4          MCI_CFG_BUS_WIDTH_4
#else
  #define MCI1_CFG_BUS_WIDTH_4          0U
#endif

/* MCI2: Define 4-bit data bus width */
#if defined(MX_SDMMC2_D0_Pin) && defined(MX_SDMMC2_D1_Pin) && defined(MX_SDMMC2_D2_Pin) && defined(MX_SDMMC2_D3_Pin)
  #define MCI2_CFG_BUS_WIDTH_4          MCI_CFG_BUS_WIDTH_4
#else
  #define MCI2_CFG_BUS_WIDTH_4          0U
#endif

/* MCI1: Define 8-bit data bus width */
#if defined(MX_SDMMC1_D0_Pin) && defined(MX_SDMMC1_D1_Pin) && defined(MX_SDMMC1_D2_Pin) && defined(MX_SDMMC1_D3_Pin) && \
    defined(MX_SDMMC1_D4_Pin) && defined(MX_SDMMC1_D5_Pin) && defined(MX_SDMMC1_D6_Pin) && defined(MX_SDMMC1_D7_Pin)
  #define MCI1_CFG_BUS_WIDTH_8          MCI_CFG_BUS_WIDTH_8
#else
  #define MCI1_CFG_BUS_WIDTH_8          0U
#endif

/* MCI2: Define 8-bit data bus width */
#if defined(MX_SDMMC2_D0_Pin) && defined(MX_SDMMC2_D1_Pin) && defined(MX_SDMMC2_D2_Pin) && defined(MX_SDMMC2_D3_Pin) && \
    defined(MX_SDMMC2_D4_Pin) && defined(MX_SDMMC2_D5_Pin) && defined(MX_SDMMC2_D6_Pin) && defined(MX_SDMMC2_D7_Pin)
  #define MCI2_CFG_BUS_WIDTH_8          MCI_CFG_BUS_WIDTH_8
#else
  #define MCI2_CFG_BUS_WIDTH_8          0U
#endif

/* MCI1: Define direction pin existence */
#if defined(MX_SDMMC1_CDIR_Pin)
  #define MCI1_CFG_DIR_PINS             MCI_CFG_DIR_PINS
#else
  #define MCI1_CFG_DIR_PINS             0U
#endif

/* MCI2: Define direction pin existence */
#if defined(MX_SDMMC2_CDIR_Pin)
  #define MCI2_CFG_DIR_PINS             MCI_CFG_DIR_PINS
#else
  #define MCI2_CFG_DIR_PINS             0U
#endif

/* MCI1: Define Card Detect pin existence */
#if defined(MemoryCard_1_CD_Pin)
  #define MCI1_CFG_PIN_CD               MCI_CFG_PIN_CD
#else
  #define MCI1_CFG_PIN_CD               0U
#endif

/* MCI2: Define Card Detect pin existence */
#if defined(MemoryCard_2_CD_Pin)
  #define MCI2_CFG_PIN_CD               MCI_CFG_PIN_CD
#else
  #define MCI2_CFG_PIN_CD               0U
#endif

/* MCI1: Define Write Protect pin existence */
#if defined(MemoryCard_1_WP_Pin)
  #define MCI1_CFG_PIN_WP               MCI_CFG_PIN_WP
#else
  #define MCI1_CFG_PIN_WP               0U
#endif

/* MCI2: Define Write Protect pin existence */
#if defined(MemoryCard_2_WP_Pin)
  #define MCI2_CFG_PIN_WP               MCI_CFG_PIN_WP
#else
  #define MCI2_CFG_PIN_WP               0U
#endif

/* MCI1: Define Card Power Select pin existence */
#if defined(MemoryCard_1_PWR_Pin)
  #define MCI1_CFG_PIN_PWR              MCI_CFG_PIN_PWR
#else
  #define MCI1_CFG_PIN_PWR              0U
#endif

/* MCI2: Define Card Power Select pin existence */
#if defined(MemoryCard_2_PWR_Pin)
  #define MCI2_CFG_PIN_PWR              MCI_CFG_PIN_PWR
#else
  #define MCI2_CFG_PIN_PWR              0U
#endif

/* MCI1: Define polarity of the direction (DIR) signals */
#if !defined(MemoryCard_1_DirPolarity_Invert)
  #define MCI1_CFG_DIR_POLARITY         MCI_CFG_DIR_POLARITY
#else
  #define MCI1_CFG_DIR_POLARITY         0U
#endif

/* MCI2: Define polarity of the direction (DIR) signals */
#if !defined(MemoryCard_2_DirPolarity_Invert)
  #define MCI2_CFG_DIR_POLARITY         MCI_CFG_DIR_POLARITY
#else
  #define MCI2_CFG_DIR_POLARITY         0U
#endif

/* MCI1: Enable or disable instance */
#if defined(MX_SDMMC1)
  #define MCI1_ENABLE                   1U
#else
  #define MCI1_ENABLE                   0U
#endif

/* MCI2: Enable or disable instance */
#if defined(MX_SDMMC2)
  #define MCI2_ENABLE                   1U
#else
  #define MCI2_ENABLE                   0U
#endif

#define MCI1_CFG                        (MCI1_CFG_PIN_CD      | \
                                         MCI1_CFG_PIN_WP      | \
                                         MCI1_CFG_PIN_PWR     | \
                                         MCI1_CFG_BUS_WIDTH_4 | \
                                         MCI1_CFG_BUS_WIDTH_8 | \
                                         MCI1_CFG_DIR_PINS    | \
                                         MCI1_CFG_DIR_POLARITY)

#define MCI2_CFG                        (MCI2_CFG_PIN_CD      | \
                                         MCI2_CFG_PIN_WP      | \
                                         MCI2_CFG_PIN_PWR     | \
                                         MCI2_CFG_BUS_WIDTH_4 | \
                                         MCI2_CFG_BUS_WIDTH_8 | \
                                         MCI2_CFG_DIR_PINS    | \
                                         MCI2_CFG_DIR_POLARITY)

#if MCI1_ENABLE
#define MCI1_REG_BLOCK                  SDMMC1
#define MCI1_IRQ_HANDLER                SDMMC1_IRQHandler
#if !defined(MemoryCard_1_MMC)
#define MCI1_HAL_MSPINIT                (HAL_MspFunc_t)HAL_SD_MspInit
#define MCI1_HAL_MSPDEINIT              (HAL_MspFunc_t)HAL_SD_MspDeInit
#define MCI1_HANDLE                     hsd1
extern SD_HandleTypeDef                 hsd1;
#else
#define MCI1_HAL_MSPINIT                (HAL_MspFunc_t)HAL_MMC_MspInit
#define MCI1_HAL_MSPDEINIT              (HAL_MspFunc_t)HAL_MMC_MspDeInit
#define MCI1_HANDLE                     hmmc1
extern MMC_HandleTypeDef                hmmc1;
#endif
#endif

#if MCI2_ENABLE
#define MCI2_REG_BLOCK                  SDMMC2
#define MCI2_IRQ_HANDLER                SDMMC2_IRQHandler
#if !defined(MemoryCard_2_MMC)
#define MCI2_HAL_MSPINIT                (HAL_MspFunc_t)HAL_SD_MspInit
#define MCI2_HAL_MSPDEINIT              (HAL_MspFunc_t)HAL_SD_MspDeInit
#define MCI2_HANDLE                     hsd2
extern SD_HandleTypeDef                 hsd2;
#else
#define MCI2_HAL_MSPINIT                (HAL_MspFunc_t)HAL_MMC_MspInit
#define MCI2_HAL_MSPDEINIT              (HAL_MspFunc_t)HAL_MMC_MspDeInit
#define MCI2_HANDLE                     hmmc2
extern MMC_HandleTypeDef                hmmc2;
#endif
#endif

/* Driver configuration flag definitions */
#define MCI_CFG_PIN_CD         (1UL << 0) /* Card Detect pin presence      */
#define MCI_CFG_PIN_WP         (1UL << 1) /* Write Protect pin presence    */
#define MCI_CFG_PIN_PWR        (1UL << 2) /* Card Power pin presence       */
#define MCI_CFG_BUS_WIDTH_4    (1UL << 3) /* 4-bit data bus presence       */
#define MCI_CFG_BUS_WIDTH_8    (1UL << 4) /* 8-bit data bus presence       */
#define MCI_CFG_DIR_PINS       (1UL << 5) /* Directional I/O presence      */
#define MCI_CFG_DIR_POLARITY   (1UL << 6) /* Directional I/O polarity      */

/* Driver flag definitions */
#define MCI_INIT      ((uint32_t)0x0001)  /* MCI initialized               */
#define MCI_POWER     ((uint32_t)0x0002)  /* MCI powered on                */
#define MCI_SETUP     ((uint32_t)0x0004)  /* MCI configured                */
#define MCI_RESP_LONG ((uint32_t)0x0008)  /* Long response expected        */
#define MCI_RESP_CRC  ((uint32_t)0x0010)  /* Check response CRC error      */
#define MCI_DATA_XFER ((uint32_t)0x0020)  /* Transfer data                 */
#define MCI_DATA_READ ((uint32_t)0x0040)  /* Read transfer                 */
#define MCI_READ_WAIT ((uint32_t)0x0080)  /* Read wait operation start     */

#define MCI_RESPONSE_EXPECTED_Msk       (ARM_MCI_RESPONSE_SHORT      | \
                                         ARM_MCI_RESPONSE_SHORT_BUSY | \
                                         ARM_MCI_RESPONSE_LONG)

#if defined(MCI_DMA_EXT)
/* DMA Callback function */
typedef void (*DMA_Callback_t) (DMA_HandleTypeDef *hdma);

/* DMA Information Definition */
typedef struct {
  DMA_HandleTypeDef *h;                 /* DMA handle                         */
  DMA_Callback_t     cb_complete;       /* DMA complete callback              */
} const MCI_DMA;
#endif

/* MCI Input/Output Definition */
typedef const struct {
  GPIO_TypeDef         *port;           /* IO port                            */
  uint16_t              pin;            /* IO pin                             */
  uint16_t              active;         /* IO active state                    */
} MCI_IO;

/* MCI Driver State Definition */
typedef struct {
  ARM_MCI_SignalEvent_t     cb_event;   /* Driver event callback function     */
  ARM_MCI_STATUS volatile   status;     /* Driver status                      */
  uint32_t                 *response;   /* Pointer to response buffer         */
  uint32_t                  ker_clk;    /* SDMMC kernel clock frequency       */
  uint32_t volatile         flags;      /* Driver state flags                 */
  uint32_t                  xfer_flags; /* Transfer flags */
} MCI_INFO;

typedef void (*HAL_MspFunc_t) (void *h);

/* MCI Resources Definition */
typedef struct {
  HAL_MspFunc_t  msp_init;
  HAL_MspFunc_t  msp_deinit;
  void          *h;                     /* SD_HandleTypeDef/MMC_HandleTypeDef */
  SDMMC_TypeDef *reg;                   /* Peripheral register interface      */
  MCI_IO         io_cd;                 /* I/O config: card detect            */
  MCI_IO         io_wp;                 /* I/O config: write protect line     */
  MCI_IO         io_pwr;                /* I/O config: card power select      */
#if defined(MCI_DMA_EXT)
  MCI_DMA        rx_dma;                /* DMA configuration for receive      */
  MCI_DMA        tx_dma;                /* DMA configuration for transmit     */
#endif
  uint32_t       cfg;                   /* Configuration flags                */
  MCI_INFO      *info;                  /* Run-Time information               */
} const MCI_RESOURCES;


/* Bus Width definitions */
#define MCI_BUS_WIDTH_1                 (0)
#define MCI_BUS_WIDTH_4                 (SDMMC_CLKCR_WIDBUS_0)
#define MCI_BUS_WIDTH_8                 (SDMMC_CLKCR_WIDBUS_0 | SDMMC_CLKCR_WIDBUS_1)

/* Data Transfer Mode definitions */
#define MCI_DTRANSFER_MODE_BLOCK        (0)
#define MCI_DTRANSFER_MODE_STREAM       (SDMMC_DCTRL_DTMODE)
#define MCI_DTRANSFER_MODE_READ         (SDMMC_DCTRL_DTDIR)
#define MCI_DTRANSFER_MODE_WRITE        (0)

#define MCI_RESPONSE_NO                 (0)
#define MCI_RESPONSE_SHORT              (SDMMC_CMD_WAITRESP_0)
#define MCI_RESPONSE_LONG               (SDMMC_CMD_WAITRESP_1 | SDMMC_CMD_WAITRESP_0)

#define MCI_IT_CCRCFAIL                 (SDMMC_STA_CCRCFAIL)
#define MCI_IT_DCRCFAIL                 (SDMMC_STA_DCRCFAIL)
#define MCI_IT_CTIMEOUT                 (SDMMC_STA_CTIMEOUT)
#define MCI_IT_DTIMEOUT                 (SDMMC_STA_DTIMEOUT)
#define MCI_IT_TXUNDERR                 (SDMMC_STA_TXUNDERR)
#define MCI_IT_RXOVERR                  (SDMMC_STA_RXOVERR)
#define MCI_IT_CMDREND                  (SDMMC_STA_CMDREND)
#define MCI_IT_CMDSENT                  (SDMMC_STA_CMDSENT)
#define MCI_IT_DATAEND                  (SDMMC_STA_DATAEND)
#define MCI_IT_DBCKEND                  (SDMMC_STA_DBCKEND)
#define MCI_IT_DPSMACT                  (SDMMC_STA_DPSMACT)
#define MCI_IT_CMDACT                   (SDMMC_STA_CPSMACT)
#define MCI_IT_TXFIFOHE                 (SDMMC_STA_TXFIFOHE)
#define MCI_IT_RXFIFOHF                 (SDMMC_STA_RXFIFOHF)
#define MCI_IT_TXFIFOF                  (SDMMC_STA_TXFIFOF)
#define MCI_IT_RXFIFOF                  (SDMMC_STA_RXFIFOF)
#define MCI_IT_TXFIFOE                  (SDMMC_STA_TXFIFOE)
#define MCI_IT_RXFIFOE                  (SDMMC_STA_RXFIFOE)
#define MCI_IT_SDIOIT                   (SDMMC_STA_SDIOIT)

#if defined(MCI_SDMMC_V1)
#define MCI_IT_DABORT                   (0)
#define MCI_IT_DHOLD                    (0)
#define MCI_IT_BUSYD0END                (0)
#define MCI_IT_ACKFAIL                  (0)
#define MCI_IT_ACKTIMEOUT               (0)
#define MCI_IT_VSWEND                   (0)
#define MCI_IT_CKSTOP                   (0)
#define MCI_IT_BUSYD0                   (0)
#define MCI_IT_IDMATE                   (0)
#define MCI_IT_IDMABTC                  (0)
#else
#define MCI_IT_DABORT                   (SDMMC_STA_DABORT)
#define MCI_IT_DHOLD                    (SDMMC_STA_DHOLD)
#define MCI_IT_BUSYD0END                (SDMMC_STA_BUSYD0END)
#define MCI_IT_ACKFAIL                  (SDMMC_STA_ACKFAIL)
#define MCI_IT_ACKTIMEOUT               (SDMMC_STA_ACKTIMEOUT)
#define MCI_IT_VSWEND                   (SDMMC_STA_VSWEND)
#define MCI_IT_CKSTOP                   (SDMMC_STA_CKSTOP)
#define MCI_IT_BUSYD0                   (SDMMC_STA_BUSYD0)
#define MCI_IT_IDMATE                   (SDMMC_STA_IDMATE)
#define MCI_IT_IDMABTC                  (SDMMC_STA_IDMABTC)
#endif

/* Interrupt clear mask */
#define MCI_IT_CLR_Msk                  (MCI_IT_CCRCFAIL   | \
                                         MCI_IT_DCRCFAIL   | \
                                         MCI_IT_CTIMEOUT   | \
                                         MCI_IT_DTIMEOUT   | \
                                         MCI_IT_TXUNDERR   | \
                                         MCI_IT_RXOVERR    | \
                                         MCI_IT_CMDREND    | \
                                         MCI_IT_CMDSENT    | \
                                         MCI_IT_DATAEND    | \
                                         MCI_IT_DHOLD      | \
                                         MCI_IT_DBCKEND    | \
                                         MCI_IT_DABORT     | \
                                         MCI_IT_TXFIFOHE   | \
                                         MCI_IT_RXFIFOHF   | \
                                         MCI_IT_RXFIFOF    | \
                                         MCI_IT_TXFIFOE    | \
                                         MCI_IT_BUSYD0END  | \
                                         MCI_IT_SDIOIT     | \
                                         MCI_IT_ACKFAIL    | \
                                         MCI_IT_ACKTIMEOUT | \
                                         MCI_IT_VSWEND     | \
                                         MCI_IT_CKSTOP     | \
                                         MCI_IT_IDMABTC)

/* Error interrupt mask */
#define MCI_IT_ERR_Msk                  (MCI_IT_CCRCFAIL   | \
                                         MCI_IT_DCRCFAIL   | \
                                         MCI_IT_CTIMEOUT   | \
                                         MCI_IT_DTIMEOUT   | \
                                         MCI_IT_ACKFAIL    | \
                                         MCI_IT_ACKTIMEOUT | \
                                         MCI_IT_IDMATE)

/* Maximum Data Block Size */
#define MCI_MAX_BLOCK_SIZE              (16384U)

#if defined(MCI_DMA_EXT)
/**
  \brief Common DMA receive complete callback
*/
static void RX_DMA_Complete (DMA_HandleTypeDef *hdma, MCI_RESOURCES *mci) {
  (void)hdma;

  mci->info->status.transfer_active = 0U;

  if (mci->info->cb_event) {
    (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_COMPLETE);
  }
}
#endif

/**
  \brief Initialize MCI peripheral
*/
__STATIC_INLINE void MCI_Init_Peripheral (MCI_RESOURCES *mci) {
  (void)mci;

#if defined(MCI_DMA_EXT)
  /* Set DMA callback function */
  mci->rx_dma.h->XferCpltCallback = mci->rx_dma.cb_complete;
#endif
}

/**
  \brief Reset MCI peripheral
*/
__STATIC_INLINE void MCI_Reset_Peripheral (MCI_RESOURCES *mci) {
#if defined(SDIO)
  /* Reset/Dereset SDIO peripheral */
  if (mci->reg == SDIO) {
    __HAL_RCC_SDIO_FORCE_RESET();
    __NOP(); __NOP(); __NOP(); __NOP();
    __HAL_RCC_SDIO_RELEASE_RESET();
  }
#elif (defined(SDMMC1) || defined(SDMMC2))
  /* Reset/Dereset SDMMC peripheral */
  if (mci->reg == SDMMC1) {
    __HAL_RCC_SDMMC1_FORCE_RESET();
    __NOP(); __NOP(); __NOP(); __NOP();
    __HAL_RCC_SDMMC1_RELEASE_RESET();
  }
#if defined(SDMMC2)
  else {
    __HAL_RCC_SDMMC2_FORCE_RESET();
    __NOP(); __NOP(); __NOP(); __NOP();
    __HAL_RCC_SDMMC2_RELEASE_RESET();
  }
#endif
#endif
}

/**
  \brief Get MCI peripheral input clock
*/
__STATIC_INLINE uint32_t MCI_Get_PeriphCLKFreq(MCI_RESOURCES *mci) {
  (void)mci;

#if defined(RCC_PERIPHCLK_SDMMC)
  return HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SDMMC);
#else
  return HAL_RCC_GetHCLKFreq();
#endif
}

/**
  \brief Set the clock divider that generates the output clock
*/
__STATIC_INLINE void MCI_Set_ClockDivider (MCI_RESOURCES *mci, uint32_t divider) {
  mci->reg->CLKCR &= ~SDMMC_CLKCR_CLKDIV;
  mci->reg->CLKCR |= (divider) >> 1U;
}

/**
  \brief Get the clock divider that generates the output clock
*/
__STATIC_INLINE uint32_t MCI_Get_ClockDivider (MCI_RESOURCES *mci) {
  return ((mci->reg->CLKCR & 0x3FFU) << 1U);
}

/**
  \brief Enable MCI peripheral output clock
*/
__STATIC_INLINE void MCI_Enable_ClockOutput (MCI_RESOURCES *mci) {
  mci->reg->POWER |= SDMMC_POWER_PWRCTRL;
}

/**
  \brief Enable MCI peripheral output clock power save
*/
__STATIC_INLINE void MCI_Enable_ClockPowerSave (MCI_RESOURCES *mci) {
  mci->reg->CLKCR |=  SDMMC_CLKCR_PWRSAV;
}

/**
  \brief Disable MCI peripheral output clock power save
*/
__STATIC_INLINE void MCI_Disable_ClockPowerSave (MCI_RESOURCES *mci) {
  mci->reg->CLKCR &= ~SDMMC_CLKCR_PWRSAV;
}

/**
  \brief Set the bus speed mode: default speed
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_DS(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  mci->reg->CLKCR &= ~(SDMMC_CLKCR_NEGEDGE);
#else
  mci->reg->CLKCR &= ~(SDMMC_CLKCR_DDR | SDMMC_CLKCR_BUSSPEED);
#endif
  return ARM_DRIVER_OK;
}

/**
  \brief Set the bus speed mode: high speed
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_HS(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  (void)mci;
#else
  mci->reg->CLKCR &= ~(SDMMC_CLKCR_DDR | SDMMC_CLKCR_BUSSPEED);
#endif
  return ARM_DRIVER_OK;
}

/**
  \brief Set the bus speed mode: SDR12
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_SDR12(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  (void)mci;
  /* Not available */
  return ARM_DRIVER_ERROR;
#else
  mci->reg->CLKCR &= ~(SDMMC_CLKCR_DDR | SDMMC_CLKCR_BUSSPEED);
  return ARM_DRIVER_OK;
#endif
}

/**
  \brief Set the bus speed mode: SDR25
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_SDR25(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  (void)mci;
  /* Not available */
  return ARM_DRIVER_ERROR;
#else
  mci->reg->CLKCR &= ~(SDMMC_CLKCR_DDR | SDMMC_CLKCR_BUSSPEED);
  return ARM_DRIVER_OK;
#endif
}

/**
  \brief Set the bus speed mode: SDR50
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_SDR50(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  (void)mci;
  /* Not available */
  return ARM_DRIVER_ERROR;
#else
  mci->reg->CLKCR = (mci->reg->CLKCR & ~SDMMC_CLKCR_DDR) | SDMMC_CLKCR_BUSSPEED;
  return ARM_DRIVER_OK;
#endif
}

/**
  \brief Set the bus speed mode: SDR104
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_SDR104(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  (void)mci;
  /* Not available */
  return ARM_DRIVER_ERROR;
#else
  mci->reg->CLKCR = (mci->reg->CLKCR & ~SDMMC_CLKCR_DDR) | SDMMC_CLKCR_BUSSPEED;
  return ARM_DRIVER_OK;
#endif
}

/**
  \brief Set the bus speed mode: DDR50
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode_DDR50(MCI_RESOURCES *mci) {

#if defined(MCI_SDMMC_V1)
  (void)mci;
  /* Not available */
  return ARM_DRIVER_ERROR;
#else
  mci->reg->CLKCR |= (SDMMC_CLKCR_DDR | SDMMC_CLKCR_BUSSPEED);
  return ARM_DRIVER_OK;
#endif
}

/**
  \brief Set bus width
  \param[in]  width  bus width (MCI_BUSW_1 | MCI_BUSW_4, MCI_BUSW_8)
*/
__STATIC_INLINE void MCI_Set_BusWidth(MCI_RESOURCES *mci, uint32_t bus_width) {
  mci->reg->CLKCR &= ~SDMMC_CLKCR_WIDBUS_Msk;
  mci->reg->CLKCR |= bus_width;
}

/**
  \brief Set the data timeout
*/
__STATIC_INLINE void MCI_Set_DTimeout(MCI_RESOURCES *mci, uint32_t periods) {
  mci->reg->DTIMER = periods;
}

/**
  \brief Set the direction signals polarity to high
*/
__STATIC_INLINE void MCI_Set_DirIOPolarity_High(MCI_RESOURCES *mci) {
  #if defined(MCI_SDMMC_V1)
  (void)mci;
  #else
  mci->reg->POWER |= SDMMC_POWER_DIRPOL;
  #endif
}

/**
  \brief Set the direction signals polarity to low
*/
__STATIC_INLINE void MCI_Set_DirIOPolarity_Low(MCI_RESOURCES *mci) {
  #if defined(MCI_SDMMC_V1)
  (void)mci;
  #else
  mci->reg->POWER &= ~SDMMC_POWER_DIRPOL;
  #endif
}

/**
  \brief Start voltage switch procedure
*/
__STATIC_INLINE void MCI_Start_VSwitch(MCI_RESOURCES *mci) {
  #if defined(MCI_SDMMC_V1)
  (void)mci;
  #else
  mci->reg->POWER |= SDMMC_POWER_VSWITCHEN;
  #endif
}

/**
  \brief Start Read Wait
*/
__STATIC_INLINE void MCI_Start_ReadWait(MCI_RESOURCES *mci) {
  mci->reg->DCTRL |= SDMMC_DCTRL_RWSTART;
}

/**
  \brief Stop Read Wait
*/
__STATIC_INLINE void MCI_Stop_ReadWait(MCI_RESOURCES *mci) {
  mci->reg->DCTRL |= SDMMC_DCTRL_RWSTOP;
}

/**
  \brief Enable MCI interrupt.
*/
__STATIC_INLINE void MCI_Enable_Interrupt(MCI_RESOURCES *mci, uint32_t interrupt) {
  mci->reg->MASK |= interrupt;
}

/**
  \brief Disable MCI interrupt.
*/
__STATIC_INLINE void MCI_Disable_Interrupt(MCI_RESOURCES *mci, uint32_t interrupt) {
  mci->reg->MASK &= ~(interrupt);
}

/**
  \brief Clear the MCI interrupt
*/
__STATIC_INLINE void MCI_Clear_Interrupt(MCI_RESOURCES *mci, uint32_t interrupt) {
  mci->reg->ICR = interrupt;
}

/**
  \brief Get enabled interrupts bit mask
*/
__STATIC_INLINE uint32_t MCI_Get_InterruptMask(MCI_RESOURCES *mci) {
  return (mci->reg->MASK);
}

/**
  \brief Return bit mask of the MCI interrupts that have occured
  \param[in]  instance    pointer to SD/MMC peripheral base
*/
__STATIC_INLINE uint32_t MCI_Get_InterruptStatus(MCI_RESOURCES *mci) {
  return (mci->reg->STA);
}

/**
  \brief Start command transfer
*/
__STATIC_INLINE void MCI_Start_CTransfer(MCI_RESOURCES *mci, uint32_t cmd, uint32_t arg, uint32_t flag) {
  mci->reg->ARG = arg;
  mci->reg->CMD = (flag)          |
                  (cmd & 0xFFU)   |
                  SDMMC_CMD_CPSMEN;
}

/**
  \brief Setup data transfer
*/
__STATIC_INLINE int32_t MCI_Setup_DTransfer(MCI_RESOURCES *mci, uint8_t *data, uint32_t b_count, uint32_t b_size) {
  uint32_t sz;
  uint32_t cnt = b_count * b_size;

  #if !defined(MCI_SDMMC_V1)
    #if defined(SDMMC_IDMABASER_IDMABASER)
    mci->reg->IDMABASER = (uint32_t)data;
    #else
    mci->reg->IDMABASE0 = (uint32_t)data;
    #endif
    mci->reg->IDMACTRL  = SDMMC_IDMA_IDMAEN;
  #endif

  mci->reg->DLEN = cnt;
  
  if (b_size == 512U) { sz = 9U; }
  else {
    for (sz = 0U; sz < 14U; sz++) {
      if (b_size & (1UL << sz)) {
        break;
      }
    }
  }

  #if defined(MCI_SDMMC_V1)
  if (mci->info->flags & MCI_DATA_READ) {
    /* Enable RX DMA stream */
    if (HAL_DMA_Start_IT(mci->rx_dma.h, (uint32_t)&(mci->reg->FIFO), (uint32_t)data, cnt) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }
  else {
    /* Enable TX DMA stream */
    if (HAL_DMA_Start_IT(mci->tx_dma.h, (uint32_t)data, (uint32_t)&(mci->reg->FIFO), cnt) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }
  mci->reg->DCTRL = (mci->reg->DCTRL & ~(SDMMC_DCTRL_DBLOCKSIZE | SDMMC_DCTRL_DTEN)) | (sz << 4);
  #else
  mci->reg->DCTRL = (mci->reg->DCTRL & ~(SDMMC_DCTRL_DBLOCKSIZE)) | (sz << 4);
  #endif

  return ARM_DRIVER_OK;
}

/**
  \brief Start data transfer
*/
__STATIC_INLINE void MCI_Start_DTransfer(MCI_RESOURCES *mci, uint32_t dtransfer_mode) {

  mci->reg->DCTRL = (mci->reg->DCTRL & ~(SDMMC_DCTRL_DTMODE | SDMMC_DCTRL_DTDIR)) | dtransfer_mode    |
                                                                                    #if defined(MCI_SDMMC_V1)
                                                                                    SDMMC_DCTRL_DMAEN |
                                                                                    #endif
                                                                                    SDMMC_DCTRL_DTEN;
}

/**
  \brief Abort data transfer
*/
__STATIC_INLINE int32_t MCI_Abort_DTransfer (MCI_RESOURCES *mci) {
  int32_t status = ARM_DRIVER_OK;

  #if defined(MCI_SDMMC_V1)
  /* Disable DMA and clear data transfer bit */
  mci->reg->DCTRL &= ~(SDMMC_DCTRL_DMAEN | SDMMC_DCTRL_DTEN);

  if (HAL_DMA_Abort(mci->rx_dma.h) != HAL_OK) {
    status = ARM_DRIVER_ERROR;
  }
  if (HAL_DMA_Abort(mci->tx_dma.h) != HAL_OK) {
    status = ARM_DRIVER_ERROR;
  }
  #else
  /* Disable IDMA */
  mci->reg->IDMACTRL &= ~SDMMC_IDMA_IDMAEN;
  /* Flush FIFO */
  mci->reg->DCTRL |= SDMMC_DCTRL_FIFORST;
  /* Clear data transfer bit */
  mci->reg->DCTRL &= ~(SDMMC_DCTRL_DTEN);
  #endif

  return (status);
}

/**
  \brief Read short response
*/
__STATIC_INLINE void MCI_Read_RespShort(MCI_RESOURCES *mci, uint32_t *buf) {
  buf[0] = mci->reg->RESP1;
}

/**
  \brief Read long response
*/
__STATIC_INLINE void MCI_Read_RespLong(MCI_RESOURCES *mci, uint32_t *buf) {
  buf[0] = mci->reg->RESP4;
  buf[1] = mci->reg->RESP3;
  buf[2] = mci->reg->RESP2;
  buf[3] = mci->reg->RESP1;
}

#endif /* MCI_STM32_SDMMC_H_ */
