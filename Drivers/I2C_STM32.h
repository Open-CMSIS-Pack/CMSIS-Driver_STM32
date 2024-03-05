/******************************************************************************
 * @file     I2C_STM32.h
 * @brief    I2C Driver header for STMicroelectronics STM32 devices
 * @version  V3.0
 * @date     5. March 2024
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

#ifndef  I2C_STM32_H__
#define  I2C_STM32_H__

#include "Driver_I2C.h"
#include "MX_Device.h"

#ifdef  __cplusplus
extern "C"
{
#endif

// Global driver structures ***************************************************

#ifdef  MX_I2C1
extern  ARM_DRIVER_I2C Driver_I2C1;
#endif
#ifdef  MX_I2C2
extern  ARM_DRIVER_I2C Driver_I2C2;
#endif
#ifdef  MX_I2C3
extern  ARM_DRIVER_I2C Driver_I2C3;
#endif
#ifdef  MX_I2C4
extern  ARM_DRIVER_I2C Driver_I2C4;
#endif
#ifdef  MX_I2C5
extern  ARM_DRIVER_I2C Driver_I2C5;
#endif
#ifdef  MX_I2C6
extern  ARM_DRIVER_I2C Driver_I2C6;
#endif
#ifdef  MX_I2C7
extern  ARM_DRIVER_I2C Driver_I2C7;
#endif
#ifdef  MX_I2C8
extern  ARM_DRIVER_I2C Driver_I2C8;
#endif

#ifdef  __cplusplus
}
#endif

#endif /* I2C_STM32_H__ */
