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
 * Project:     USB Device Driver header for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

#ifndef USBD_STM32_H_
#define USBD_STM32_H_

#include "Driver_USBD.h"
#include "MX_Device.h"

#ifdef  __cplusplus
extern  "C"
{
#endif

// Configuration ***************************************************************

#if     defined(MX_USB)
#define MX_USBD0                        1
#define MX_USBD0_HANDLE                 MX_USB_HANDLE
#endif

#if   ((defined(MX_USB_OTG_FS) && defined(MX_USB_OTG_FS_DEVICE))         || \
       (defined(MX_USB_OTG_FS) && defined(MX_USB_OTG_FS_Device_Only)))
#define MX_USBD0                        1
#define MX_USBD0_HANDLE                 MX_USB_OTG_FS_HANDLE
#endif

#if   ((defined(MX_USB_OTG_HS) && defined(MX_USB_OTG_HS_DEVICE))         || \
       (defined(MX_USB_OTG_HS) && defined(MX_USB_OTG_HS_Device_Only_FS)) || \
       (defined(MX_USB_OTG_HS) && defined(MX_USB_OTG_HS_Device_HS)))
#define MX_USBD1                        1
#define MX_USBD1_HANDLE                 MX_USB_OTG_HS_HANDLE
#endif

// Global driver structures ****************************************************

#ifdef  MX_USBD0
extern  ARM_DRIVER_USBD Driver_USBD0;
#endif

#ifdef  MX_USBD1
extern  ARM_DRIVER_USBD Driver_USBD1;
#endif

#ifdef  __cplusplus
}
#endif

#endif  // USBD_STM32_H_
