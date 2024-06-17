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

#ifndef MCI_STM32_SDIO_H_
#define MCI_STM32_SDIO_H_

#include "MCI_STM32.h"

#include "RTE_Components.h"
#include CMSIS_device_header

#include "main.h"

/* MCI1: Define 4-bit data bus width */
#if defined(MX_SDIO_D0_Pin) && defined(MX_SDIO_D1_Pin) && defined(MX_SDIO_D2_Pin) && defined(MX_SDIO_D3_Pin)
  #define MCI1_CFG_BUS_WIDTH_4          MCI_CFG_BUS_WIDTH_4
#else
  #define MCI1_CFG_BUS_WIDTH_4          0U
#endif

/* MCI1: Define 8-bit data bus width */
#if defined(MX_SDIO_D0_Pin) && defined(MX_SDIO_D1_Pin) && defined(MX_SDIO_D2_Pin) && defined(MX_SDIO_D3_Pin) && \
    defined(MX_SDIO_D4_Pin) && defined(MX_SDIO_D5_Pin) && defined(MX_SDIO_D6_Pin) && defined(MX_SDIO_D7_Pin)
  #define MCI1_CFG_BUS_WIDTH_8          MCI_CFG_BUS_WIDTH_8
#else
  #define MCI1_CFG_BUS_WIDTH_8          0U
#endif

/* MCI1: Define Card Detect pin existence */
#if defined(MemoryCard_1_CD_Pin)
  #define MCI1_CFG_PIN_CD               MCI_CFG_PIN_CD
#else
  #define MCI1_CFG_PIN_CD               0U
#endif

/* MCI1: Define Write Protect pin existence */
#if defined(MemoryCard_1_WP_Pin)
  #define MCI1_CFG_PIN_WP              MCI_CFG_PIN_WP
#else
  #define MCI1_CFG_PIN_WP               0U
#endif

/* MCI1: Enable or disable instance */
#if defined(MX_SDIO)
  #define MCI1_ENABLE                   1U
#else
  #define MCI1_ENABLE                   0U
#endif

#define MCI1_CFG                        (MCI1_CFG_PIN_CD      | \
                                         MCI1_CFG_PIN_WP      | \
                                         MCI1_CFG_BUS_WIDTH_4 | \
                                         MCI1_CFG_BUS_WIDTH_8)
#if MCI1_ENABLE
#define MCI1_REG_BLOCK                  SDIO
#define MCI1_IRQ_HANDLER                SDIO_IRQHandler
#if !defined(MemoryCard_MMC1)
#define MCI1_HAL_MSPINIT                (HAL_MspFunc_t)HAL_SD_MspInit
#define MCI1_HAL_MSPDEINIT              (HAL_MspFunc_t)HAL_SD_MspDeInit
#define MCI1_HANDLE                     hsd
extern SD_HandleTypeDef                 hsd;
#else
#define MCI1_HAL_MSPINIT                (HAL_MspFunc_t)HAL_MMC_MspInit
#define MCI1_HAL_MSPDEINIT              (HAL_MspFunc_t)HAL_MMC_MspDeInit
#define MCI1_HANDLE                     mmc
extern MMC_HandleTypeDef                mmc;
#endif
/* DMA handler prototypes */
extern DMA_HandleTypeDef                hdma_sdio_rx;
extern DMA_HandleTypeDef                hdma_sdio_tx;
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
  SDIO_TypeDef  *reg;                   /* Peripheral register interface      */
  MCI_IO         io_cd;                 /* I/O config: card detect            */
  MCI_IO         io_wp;                 /* I/O config: write protect line     */
  MCI_IO         io_pwr;                /* I/O config: card power select      */
  uint32_t       cfg;                   /* Configuration flags                */
  MCI_INFO      *info;                  /* Run-Time information               */
} const MCI_RESOURCES;


/* SDIO Adapter Clock definition */
#define SDIOCLK                         (48000000U)

/* Bus Speed Mode definitions */
#define MCI_BUS_SPEED_MODE_DS           (0)
#define MCI_BUS_SPEED_MODE_HS           (1)
#define MCI_BUS_SPEED_MODE_SDR12        (2)
#define MCI_BUS_SPEED_MODE_SDR25        (3)
#define MCI_BUS_SPEED_MODE_SDR50        (4)
#define MCI_BUS_SPEED_MODE_SDR104       (5)
#define MCI_BUS_SPEED_MODE_DDR50        (6)

/* Bus Width definitions */
#define MCI_BUS_WIDTH_1                 (0)
#define MCI_BUS_WIDTH_4                 (SDIO_CLKCR_WIDBUS_0)
#define MCI_BUS_WIDTH_8                 (SDIO_CLKCR_WIDBUS_0 | SDIO_CLKCR_WIDBUS_1)

/* Data Transfer Mode definitions */
#define MCI_DTRANSFER_MODE_BLOCK        (0)
#define MCI_DTRANSFER_MODE_STREAM       (SDIO_DCTRL_DTMODE)
#define MCI_DTRANSFER_MODE_READ         (SDIO_DCTRL_DTDIR)
#define MCI_DTRANSFER_MODE_WRITE        (0)


#define MCI_DIR_IO_OUT_LOW              (0)
#define MCI_DIR_IO_OUT_HIGH             (0)

#define MCI_RESPONSE_NO                 (0)
#define MCI_RESPONSE_SHORT              (SDIO_CMD_WAITRESP_0)
#define MCI_RESPONSE_LONG               (SDIO_CMD_WAITRESP_1 | SDIO_CMD_WAITRESP_0)

#define MCI_IT_CCRCFAIL                 (SDIO_STA_CCRCFAIL)
#define MCI_IT_DCRCFAIL                 (SDIO_STA_DCRCFAIL)
#define MCI_IT_CTIMEOUT                 (SDIO_STA_CTIMEOUT)
#define MCI_IT_DTIMEOUT                 (SDIO_STA_DTIMEOUT)
#define MCI_IT_TXUNDERR                 (SDIO_STA_TXUNDERR)
#define MCI_IT_RXOVERR                  (SDIO_STA_RXOVERR)
#define MCI_IT_CMDREND                  (SDIO_STA_CMDREND)
#define MCI_IT_CMDSENT                  (SDIO_STA_CMDSENT)
#define MCI_IT_DATAEND                  (SDIO_STA_DATAEND)
#define MCI_IT_DHOLD                    (0)
#define MCI_IT_DBCKEND                  (SDIO_STA_DBCKEND)
#define MCI_IT_DABORT                   (0)
#define MCI_IT_DPSMACT                  (SDIO_STA_DPSMACT)
#define MCI_IT_CMDACT                   (SDIO_STA_CPSMACT)
#define MCI_IT_TXFIFOHE                 (SDIO_STA_TXFIFOHE)
#define MCI_IT_RXFIFOHF                 (SDIO_STA_RXFIFOHF)
#define MCI_IT_TXFIFOF                  (SDIO_STA_TXFIFOF)
#define MCI_IT_RXFIFOF                  (SDIO_STA_RXFIFOF)
#define MCI_IT_TXFIFOE                  (SDIO_STA_TXFIFOE)
#define MCI_IT_RXFIFOE                  (SDIO_STA_RXFIFOE)
#define MCI_IT_BUSYD0                   (0)
#define MCI_IT_BUSYD0END                (0)
#define MCI_IT_SDIOIT                   (SDIO_STA_SDIOIT)
#define MCI_IT_ACKFAIL                  (0)
#define MCI_IT_ACKTIMEOUT               (0)
#define MCI_IT_VSWEND                   (0)
#define MCI_IT_CKSTOP                   (0)
#define MCI_IT_IDMATE                   (0)
#define MCI_IT_IDMABTC                  (0)

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
                                         MCI_IT_DBCKEND    | \
                                         MCI_IT_SDIOIT)

/* Error interrupt mask */
#define MCI_IT_ERR_Msk                  (MCI_IT_CCRCFAIL   | \
                                         MCI_IT_DCRCFAIL   | \
                                         MCI_IT_CTIMEOUT   | \
                                         MCI_IT_DTIMEOUT)

/* Maximum Data Block Size */
#define MCI_MAX_BLOCK_SIZE              (16384U)


static MCI_RESOURCES *arg;

/* Rx DMA Callback */
void RX_DMA_Complete(struct __DMA_HandleTypeDef *hdma) {
  MCI_RESOURCES *mci = arg;
  (void)hdma;

  mci->info->status.transfer_active = 0U;

  if (mci->info->cb_event) {
    (mci->info->cb_event)(ARM_MCI_EVENT_TRANSFER_COMPLETE);
  }
}

/**
  \brief Initialize MCI peripheral
*/
__STATIC_INLINE void MCI_Init_Peripheral (MCI_RESOURCES *mci) {
  (void)mci;

  arg = mci;

  /* Set DMA callback function */
  hdma_sdio_rx.XferCpltCallback  = &RX_DMA_Complete;
}

/**
  \brief Reset MCI peripheral
*/
__STATIC_INLINE void MCI_Reset_Peripheral (MCI_RESOURCES *mci) {
  (void)mci;

  /* Reset/Dereset SDIO peripheral */
  if (mci->reg == SDIO) {
    __HAL_RCC_SDIO_FORCE_RESET();
    __NOP(); __NOP(); __NOP(); __NOP();
    __HAL_RCC_SDIO_RELEASE_RESET();
  }
}

/**
  \brief Get MCI peripheral input clock
*/
__STATIC_INLINE uint32_t MCI_Get_PeriphCLKFreq(MCI_RESOURCES *mci) {
  (void)mci;

  return (SDIOCLK);
}

/**
  \brief Set the clock divider that generates the output clock
*/
__STATIC_INLINE void MCI_Set_ClockDivider (MCI_RESOURCES *mci, uint32_t divider) {
  mci->reg->CLKCR &= ~SDIO_CLKCR_CLKDIV;
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
  mci->reg->POWER |= SDIO_POWER_PWRCTRL;
}

/**
  \brief Enable MCI peripheral output clock power save
*/
__STATIC_INLINE void MCI_Enable_ClockPowerSave (MCI_RESOURCES *mci) {
  mci->reg->CLKCR |=  SDIO_CLKCR_PWRSAV;
}

/**
  \brief Disable MCI peripheral output clock power save
*/
__STATIC_INLINE void MCI_Disable_ClockPowerSave (MCI_RESOURCES *mci) {
  mci->reg->CLKCR &= ~SDIO_CLKCR_PWRSAV;
}

/**
  \brief Set the bus speed mode
*/
__STATIC_INLINE int32_t MCI_Set_BusSpeedMode(MCI_RESOURCES *mci, uint32_t bus_speed_mode) {
  if (bus_speed_mode == MCI_BUS_SPEED_MODE_DS) {
    mci->reg->CLKCR &= ~SDIO_CLKCR_NEGEDGE;
  }
  else if (bus_speed_mode == MCI_BUS_SPEED_MODE_HS) {
    /* Speed mode up to 50MHz */
    /* Errata: configuration with the NEGEDGE bit set should not be used. */
    return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  else {
    /* This peripheral doesn't have this functionality */
    return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \brief Set bus width
  \param[in]  bus_width  bus width (see MCI_BUS_WIDTH_*)
*/
__STATIC_INLINE void MCI_Set_BusWidth(MCI_RESOURCES *mci, uint32_t bus_width) {
  mci->reg->CLKCR &= ~SDIO_CLKCR_WIDBUS_Msk;
  mci->reg->CLKCR |= bus_width;
}

/**
  \brief Set the data timeout
*/
__STATIC_INLINE void MCI_Set_DTimeout(MCI_RESOURCES *mci, uint32_t periods) {
  mci->reg->DTIMER = periods;
}

/**
  \brief Set the direction signals polarity
*/
__STATIC_INLINE void MCI_Set_DirIOPolarity(MCI_RESOURCES *mci, uint32_t polarity) {
  (void)mci;
  (void)polarity;
  /* This peripheral doesn't have this functionality */
}

/**
  \brief Start voltage switch procedure
*/
__STATIC_INLINE void MCI_Start_VSwitch (MCI_RESOURCES *mci) {
  (void)mci;
  /* This peripheral doesn't have this functionality */
}

/**
  \brief Start Read Wait
*/
__STATIC_INLINE void MCI_Start_ReadWait(MCI_RESOURCES *mci) {
  mci->reg->DCTRL |= SDIO_DCTRL_RWSTART;
}

/**
  \brief Stop Read Wait
*/
__STATIC_INLINE void MCI_Stop_ReadWait(MCI_RESOURCES *mci) {
  mci->reg->DCTRL |= SDIO_DCTRL_RWSTOP;
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
__STATIC_INLINE void MCI_Start_CTransfer(MCI_RESOURCES *mci,uint32_t cmd, uint32_t arg, uint32_t flag) {
  mci->reg->ARG = arg;
  mci->reg->CMD = (flag)          |
                  (cmd & 0xFFU)   |
                  SDIO_CMD_CPSMEN;
}

/**
  \brief Setup data transfer
*/
__STATIC_INLINE int32_t MCI_Setup_DTransfer(MCI_RESOURCES *mci, uint8_t *data, uint32_t b_count, uint32_t b_size) {
  uint32_t sz;
  uint32_t cnt = b_count * b_size;

  mci->reg->DLEN = cnt;

  if (b_size == 512U) { sz = 9U; }
  else {
    for (sz = 0U; sz < 14U; sz++) {
      if (b_size & (1UL << sz)) {
        break;
      }
    }
  }

  if (mci->info->flags & MCI_DATA_READ) {
    /* Enable RX DMA stream */
    if (HAL_DMA_Start_IT (&hdma_sdio_rx, (uint32_t)&(SDIO->FIFO), (uint32_t)data, cnt) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }
  else {
    /* Enable TX DMA stream */
    if (HAL_DMA_Start_IT (&hdma_sdio_tx, (uint32_t)data, (uint32_t)&(SDIO->FIFO), cnt) != HAL_OK) {
      return ARM_DRIVER_ERROR;
    }
  }

  mci->reg->DCTRL = (mci->reg->DCTRL & ~(SDIO_DCTRL_DBLOCKSIZE | SDIO_DCTRL_DTEN)) | (sz << 4);

  return ARM_DRIVER_OK;
}

/**
  \brief Start data transfer
*/
__STATIC_INLINE void MCI_Start_DTransfer(MCI_RESOURCES *mci, uint32_t dtransfer_mode) {

  mci->reg->DCTRL = (mci->reg->DCTRL & ~(SDIO_DCTRL_DTMODE | SDIO_DCTRL_DTDIR)) | dtransfer_mode   |
                                                                                  SDIO_DCTRL_DMAEN |
                                                                                  SDIO_DCTRL_DTEN;
}

/**
  \brief Abort data transfer
*/
__STATIC_INLINE int32_t MCI_Abort_DTransfer (MCI_RESOURCES *mci) {
  int32_t status = ARM_DRIVER_OK;

  /* Disable DMA and clear data transfer bit */
  mci->reg->DCTRL &= ~(SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);

  if (HAL_DMA_Abort (&hdma_sdio_rx) != HAL_OK) {
    status = ARM_DRIVER_ERROR;
  }
  if (HAL_DMA_Abort (&hdma_sdio_tx) != HAL_OK) {
    status = ARM_DRIVER_ERROR;
  }

  /* Clear SDIO FIFO */
  while (SDIO->FIFOCNT) {
    SDIO->FIFO;
  }

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

#endif /* MCI_STM32_SDIO_H_ */
