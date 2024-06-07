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
 * $Date:       7. June 2024
 * $Revision:   V3.0
 *
 * Project:     SPI Driver header for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

#ifndef  SPI_STM32_H__
#define  SPI_STM32_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "Driver_SPI.h"
#include "MX_Device.h"

// Global driver structures ***************************************************

#ifdef  MX_SPI1
extern  ARM_DRIVER_SPI Driver_SPI1;
#endif
#ifdef  MX_SPI2
extern  ARM_DRIVER_SPI Driver_SPI2;
#endif
#ifdef  MX_SPI3
extern  ARM_DRIVER_SPI Driver_SPI3;
#endif
#ifdef  MX_SPI4
extern  ARM_DRIVER_SPI Driver_SPI4;
#endif
#ifdef  MX_SPI5
extern  ARM_DRIVER_SPI Driver_SPI5;
#endif
#ifdef  MX_SPI6
extern  ARM_DRIVER_SPI Driver_SPI6;
#endif
#ifdef  MX_SPI7
extern  ARM_DRIVER_SPI Driver_SPI7;
#endif
#ifdef  MX_SPI8
extern  ARM_DRIVER_SPI Driver_SPI8;
#endif

#ifdef  __cplusplus
}
#endif

#endif /* SPI_STM32_H__ */
