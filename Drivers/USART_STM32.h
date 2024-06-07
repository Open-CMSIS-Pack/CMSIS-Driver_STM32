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
 * Project:     USART Driver header for STMicroelectronics STM32 devices
 *
 * -----------------------------------------------------------------------------
 */

#ifndef USART_STM32_H_
#define USART_STM32_H_

#include "Driver_USART.h"
#include "MX_Device.h"

#ifdef  __cplusplus
extern  "C"
{
#endif

// Configuration ***************************************************************

#if    !defined(MX_UART1)  && (defined(MX_USART1)  && defined(MX_USART1_VM_ASYNC))
#define MX_UART1                        1
#endif
#if    !defined(MX_UART2)  && (defined(MX_USART2)  && defined(MX_USART2_VM_ASYNC))
#define MX_UART2                        1
#endif
#if    !defined(MX_UART3)  && (defined(MX_USART3)  && defined(MX_USART3_VM_ASYNC))
#define MX_UART3                        1
#endif
#if    !defined(MX_UART4)  && (defined(MX_USART4)  && defined(MX_USART4_VM_ASYNC))
#define MX_UART4                        1
#endif
#if    !defined(MX_UART5)  && (defined(MX_USART5)  && defined(MX_USART5_VM_ASYNC))
#define MX_UART5                        1
#endif
#if    !defined(MX_UART6)  && (defined(MX_USART6)  && defined(MX_USART6_VM_ASYNC))
#define MX_UART6                        1
#endif
#if    !defined(MX_UART7)  && (defined(MX_USART7)  && defined(MX_USART7_VM_ASYNC))
#define MX_UART7                        1
#endif
#if    !defined(MX_UART8)  && (defined(MX_USART8)  && defined(MX_USART8_VM_ASYNC))
#define MX_UART8                        1
#endif
#if    !defined(MX_UART9)  && (defined(MX_USART9)  && defined(MX_USART9_VM_ASYNC))
#define MX_UART9                        1
#endif
#if    !defined(MX_UART10) && (defined(MX_USART10) && defined(MX_USART10_VM_ASYNC))
#define MX_UART10                       1
#endif
#if    !defined(MX_UART11) && (defined(MX_USART11) && defined(MX_USART11_VM_ASYNC))
#define MX_UART11                       1
#endif
#if    !defined(MX_UART12) && (defined(MX_USART12) && defined(MX_USART12_VM_ASYNC))
#define MX_UART12                       1
#endif
#if    !defined(MX_UART13) && (defined(MX_USART13) && defined(MX_USART13_VM_ASYNC))
#define MX_UART13                       1
#endif
#if    !defined(MX_UART14) && (defined(MX_USART14) && defined(MX_USART14_VM_ASYNC))
#define MX_UART14                       1
#endif
#if    !defined(MX_UART15) && (defined(MX_USART15) && defined(MX_USART15_VM_ASYNC))
#define MX_UART15                       1
#endif
#if     defined(MX_LPUART1)
#define MX_UART21                       1
#endif
#if     defined(MX_LPUART2)
#define MX_UART22                       1
#endif
#if     defined(MX_LPUART3)
#define MX_UART23                       1
#endif

// Global driver structures ****************************************************

#ifdef  MX_UART1
extern  ARM_DRIVER_USART Driver_USART1;
#endif
#ifdef  MX_UART2
extern  ARM_DRIVER_USART Driver_USART2;
#endif
#ifdef  MX_UART3
extern  ARM_DRIVER_USART Driver_USART3;
#endif
#ifdef  MX_UART4
extern  ARM_DRIVER_USART Driver_USART4;
#endif
#ifdef  MX_UART5
extern  ARM_DRIVER_USART Driver_USART5;
#endif
#ifdef  MX_UART6
extern  ARM_DRIVER_USART Driver_USART6;
#endif
#ifdef  MX_UART7
extern  ARM_DRIVER_USART Driver_USART7;
#endif
#ifdef  MX_UART8
extern  ARM_DRIVER_USART Driver_USART8;
#endif
#ifdef  MX_UART9
extern  ARM_DRIVER_USART Driver_USART9;
#endif
#ifdef  MX_UART10
extern  ARM_DRIVER_USART Driver_USART10;
#endif
#ifdef  MX_UART11
extern  ARM_DRIVER_USART Driver_USART11;
#endif
#ifdef  MX_UART12
extern  ARM_DRIVER_USART Driver_USART12;
#endif
#ifdef  MX_UART13
extern  ARM_DRIVER_USART Driver_USART13;
#endif
#ifdef  MX_UART14
extern  ARM_DRIVER_USART Driver_USART14;
#endif
#ifdef  MX_UART15
extern  ARM_DRIVER_USART Driver_USART15;
#endif
#ifdef  MX_UART21
extern  ARM_DRIVER_USART Driver_USART21;
#endif
#ifdef  MX_UART22
extern  ARM_DRIVER_USART Driver_USART22;
#endif
#ifdef  MX_UART23
extern  ARM_DRIVER_USART Driver_USART23;
#endif

#ifdef  __cplusplus
}
#endif

#endif  // USART_STM32_H_
