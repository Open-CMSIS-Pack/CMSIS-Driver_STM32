# Comments about STM32 HAL

# General comments

1. the **MX_..._Init** function calls **HAL_..._Init** function thus already puts peripheral in operating mode
   (enables clocks and interrupts)
   - there is no way to set default peripheral parameters (also peripheral address (handle)) without starting the peripheral operation
2. pin configuration is not separate from power configuration (clocks and interrupts) they are all
   configured in the **HAL_..._MspInit** and **HAL_..._MspDeInit** functions
   - this prevents any low-power mode configuration
3. there is no way to **reset** the peripheral
   - this prevents recovery of malfunctioning peripheral as well as any clean-up
4. in general drivers do not have interrupts enabled until a transfer operation is started meaning some types of
   events like data lost cannot be detected until transfer operation is started
5. **MX_..._Init** function naming is not consistent across device series or peripheral instances
   - for example **MX_..._Init** for USB can be: **MX_USB_PCD_Init** or **MX_USB_OTG_FS_PCD_Init**
   - for example **MX_..._Init** for USART can be: **MX_UART4_Init** or **MX_USART3_UART_Init**
6. peripheral **handle** naming is not consistent across device series
   - for example USB **handle** can be: **hpcd_USB_FS**, **hpcd_USB_DRD_FS** or **hpcd_USB_OTG_FS**
7. some peripherals have **additional configuration** not handled by HAL but with **additional (Ex) functions**
   - for example such functions for I2C are: **HAL_I2CEx_ConfigAnalogFilter** and **HAL_I2CEx_ConfigDigitalFilter**
   - for example such functions for USART are: **HAL_UARTEx_SetTxFifoThreshold**, **HAL_UARTEx_SetRxFifoThreshold**,
     **HAL_UARTEx_EnableFifoMode** and  **HAL_UARTEx_DisableFifoMode**
   - for example such functions for USB Device are: **HAL_PCDEx_SetTxFiFo**, **HAL_PCDEx_SetRxFiFo** or **HAL_PCDEx_PMAConfig**
8. retrieval of **peripheral clock** is inconsistently implemented across device series (`defect`)
   - [example for missing clock information for RCC_PERIPHCLK_I2C1 on STM32H7](https://github.com/STMicroelectronics/stm32h7xx_hal_driver/blob/fec141ce999da655a48e1a15db83a72d564a1312/Src/stm32h7xx_hal_rcc_ex.c#L1881)

# Peripheral specific comments

## Ethernet MAC

1.  no way to **retrieve size of received Ethernet frame**
2.  no way to **enable/disable the transmitter or receiver**
3.  no support for MAC address filter
4.  no way to flush data (Rx/Tx) buffer
5.  no support for sleep mode (wait for Magic packet)
6.  no way to disable VLAN filter
7.  no support for Precision Time Protocol (PTP)

## GPIO

1.  no way to **retrieve pin configuration** only HAL_GPIO_Init function for setting the configuration exists
2.  inconsistent callbacks across device series for edge detection: HAL_GPIO_EXTI_Callback or HAL_GPIO_EXTI_Rising_Callback and HAL_GPIO_EXTI_Falling_Callback
3.  edge detection can only be disabled by HAL_GPIO_DeInit function

## I2C

1.  **analog/digital filter not configurable with HAL_I2C_Init** but through HAL Ex HAL_I2CEx_ConfigAnalogFilter and HAL_I2CEx_ConfigDigitalFilter functions
2.  **HAL_I2C_Init does not preserve filter settings** done by HAL_I2CEx_ConfigAnalogFilter and HAL_I2CEx_ConfigDigitalFilter functions
3.  no way to **get bus speed**
4.  no way to **set bus speed**
5.  no way to execute **bus clear** operation
6.  no way to **get number of transferred bytes**
7.  Transmit operation **clears error information relating to reception** (`defect`)
    - [example on STM32H7](https://github.com/STMicroelectronics/stm32h7xx_hal_driver/blob/2266e33c0b1ed9ebc9485f6a4c9862023e0e5b82/Src/stm32h7xx_hal_i2c.c#L1127)
8.  Slave transfer has to be started from HAL_I2C_AddrCallback
9.  maximum transfers supported by Transmit and Receive functions is limited to 65535 bytes

## MCI

1. not HAL based driver, only abstracted register access

## SPI

1.  no way to **get bus speed**
2.  no way to **set bus speed**
3.  no support for **default transmit value during reception**
4.  no way to **get number of transferred bytes**
5.  Transmit operation **clears error information relating to reception** (`defect`)
    - [example on STM32H7](https://github.com/STMicroelectronics/stm32h7xx_hal_driver/blob/2266e33c0b1ed9ebc9485f6a4c9862023e0e5b82/Src/stm32h7xx_hal_spi.c#L853)
6.  maximum transfers supported by Transmit and Receive functions is limited to 65535 bytes
7.  no way to control internal Slave Select input level (SSI) value
8.  Slave Select line operation is defined at compile-time and is not expected to change at run-time
9.  no way of driving user selected GPIO as software Slave Select

## USART (only for UART mode)

1.  no way to **generate continuous Tx break** signal
2.  no way to **get number of transmitted or received bytes**
3.  Transmit operation **clears error information relating to reception**  (`defect`)
    - [example on STM32H7](https://github.com/STMicroelectronics/stm32h7xx_hal_driver/blob/2266e33c0b1ed9ebc9485f6a4c9862023e0e5b82/Src/stm32h7xx_hal_uart.c#L1309)
4.  maximum number of data items supported by Transmit and Receive functions is limited to 65535 items
5.  no support for manual modem lines handling (driving or status retrieval)

## USB Device

1.  **Endpoint FIFOs not configurable with HAL_PCD_Init** but through HAL Ex HAL_PCDEx_SetTxFiFo, HAL_PCDEx_SetRxFiFo or HAL_PCDEx_PMAConfig functions
2.  no callback for **Set Address request**
3.  **Endpoint 0 does not handle transfers larger than maximum packet**
4.  **aborting transfer on Interrupt OUT Endpoint** is not working (at least on some device series) (`defect`)
    - [example of Interrupt OUT endpoint abort timing-out on STM32H7](https://github.com/STMicroelectronics/stm32h7xx_hal_driver/blob/2266e33c0b1ed9ebc9485f6a4c9862023e0e5b82/Src/stm32h7xx_ll_usb.c#L958)
5.  some drivers do not call **HAL_PCD_DataOutStageCallback** when **zero-length packet is received**
    - [example on STM32H5](https://github.com/STMicroelectronics/stm32h5xx_hal_driver/blob/afcafe6d4f21a18d898400705addd9c94fba8660/Src/stm32h5xx_hal_pcd.c#L1793)

## USB Host

1.  no support for **Port suspend** and **resume**
2.  no support for **over-current detection**
3.  no support for **periodic transfer period handling**
4.  no support for **disabling Host Channel** (only **HAL_HCD_HC_Init** function exists)
5.  no support for **toggle control/reset**
6.  **transfers of max packet size packets on Control Host Channel mishandles toggle bit**
7.  no support for **transfers of sizes larger then max packet size**
8.  **HAL_HCD_HC_GetXferCount** returns correct size only for **IN transfers**
9.  no support for **aborting of transfer**
