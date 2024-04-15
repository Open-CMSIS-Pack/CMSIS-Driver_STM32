/******************************************************************************
 * @file     GPIO_STM32.h
 * @brief    GPIO Driver header for STMicroelectronics STM32 devices
 * @version  V1.0
 * @date     15. April 2024
 ******************************************************************************/
/*
 * Copyright (c) 2024 Arm Limited (or its affiliates).
 * All rights reserved.
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
 */

#ifndef  GPIO_STM32_H__
#define  GPIO_STM32_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "Driver_GPIO.h"
#include "MX_Device.h"

// Global driver structures ***************************************************

extern  ARM_DRIVER_GPIO Driver_GPIO0;

#ifdef  __cplusplus
}
#endif

#endif /* GPIO_STM32_H__ */
