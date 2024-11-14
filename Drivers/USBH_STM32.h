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
 * $Date:       13. November 2024
 * $Revision:   V2.1
 *
 * Project:     USB Host Driver header for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

#ifndef USBH_STM32_H_
#define USBH_STM32_H_

#include "Driver_USBH.h"
#include "MX_Device.h"

#include "RTE_Components.h"
#include  CMSIS_device_header

#ifdef  __cplusplus
extern  "C"
{
#endif

// Configuration ***************************************************************

#if    (defined(MX_USB) && defined(MX_USB_Host_Only))
#define MX_USBH0                        1
#define MX_USBH0_HANDLE                 MX_USB_HANDLE
#endif

#if    (defined(MX_USB_OTG_FS) && defined(MX_USB_OTG_FS_Host_Only))
#define MX_USBH0                        1
#define MX_USBH0_HANDLE                 MX_USB_OTG_FS_HANDLE
#endif

#if    (defined(MX_USB_OTG_HS) && (defined(MX_USB_OTG_HS_Host_Only_FS) || \
                                   defined(MX_USB_OTG_HS_Host_FS)      || \
                                   defined(MX_USB_OTG_HS_Host_HS)))
#define MX_USBH1                        1
#define MX_USBH1_HANDLE                 MX_USB_OTG_HS_HANDLE
#endif

// Hardware-specific functions *************************************************

// This function should be provided by user code if hardware-specific driving
// for VBUS is necessary.

/**
  \fn          int32_t USBH_HW_VbusOnOff (HCD_HandleTypeDef *ptr_hhcd, bool vbus)
  \brief       Hardware-specific control of VBUS line.
  \param[in]   ptr_hhcd         Pointer to hhcd
  \param[in]   vbus
                - \b false VBUS off
                - \b true  VBUS on
  \return      0 on success, -1 on error.
*/
extern  int32_t USBH_HW_VbusOnOff (HCD_HandleTypeDef *ptr_hhcd, bool vbus);

// Global driver structures ****************************************************

#ifdef  MX_USBH0
extern  ARM_DRIVER_USBH Driver_USBH0;
#endif

#ifdef  MX_USBH1
extern  ARM_DRIVER_USBH Driver_USBH1;
#endif

#ifdef  __cplusplus
}
#endif

#endif  // USBH_STM32_H_
