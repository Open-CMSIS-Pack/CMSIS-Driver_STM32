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
 * $Revision:   3.0
 *
 * Project:     MCI Driver for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

#ifndef MCI_STM32_H_
#define MCI_STM32_H_

#include "Driver_MCI.h"
#include "MX_Device.h"

#ifdef  __cplusplus
extern  "C"
{
#endif

// Global driver structures ****************************************************

#if defined(MX_SDMMC1) || defined(MX_SDIO)
extern ARM_DRIVER_MCI Driver_MCI1;
#endif
#if defined(MX_SDMMC2)
extern ARM_DRIVER_MCI Driver_MCI2;
#endif

#ifdef  __cplusplus
}
#endif

#endif /* MCI_STM32_H__ */
