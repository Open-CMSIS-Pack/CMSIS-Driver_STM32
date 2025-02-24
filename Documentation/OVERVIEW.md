# Introduction

The **CMSIS-Driver interface to STM32Cube HAL** are API Shim interfaces for the STM32Cube HAL provided by STMicroelectronics. The Drivers are accessible by CMSIS-Pack enabled software development tools, for example µVision 5.40, VS Code Arm CMSIS Solution extension, and the CMSIS-Toolbox.

User applications and middleware components use [CMSIS-Driver](https://arm-software.github.io/CMSIS_6/latest/Driver/index.html) to achieve better code reuse and simplify integration in various ecosystems. CMSIS-Driver are generic and independent of a specific RTOS making it reusable across a wide range of supported microcontroller devices.

STM32 microcontrollers already provide powerful drivers, however with a different API interface, call STM32Cube HAL. With this **Driver API Shim Interface** the CMSIS standards can be used by the software ecosystem and application developers.

The diagram below outlines the software architecture that is used for example by [Reference Applications](https://open-cmsis-pack.github.io/cmsis-toolbox/ReferenceApplications/) and the [MDK middleware examples](https://github.com/ARM-software/MDK-Middleware/tree/main/Examples). Refer to the [CMSIS-Toolbox](https://open-cmsis-pack.github.io/cmsis-toolbox/) documentation [Reference Applications - Header File Structure](https://open-cmsis-pack.github.io/cmsis-toolbox/ReferenceApplications/#header-file-structure) for a description of the various components in this diagram. The Driver API Shim Interface is configured using [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) (CubeMX) that generates via the CMSIS tool integration the file `MX_Device.h`.

![Software Architecture](./SW-Architecture.png "Software Architecture")

## Available CMSIS-Drivers

```yml
  - component: CMSIS-Driver:Ethernet MAC     # Ethernet MAC Driver for STM32 devices
  - component: CMSIS-Driver:GPIO             # GPIO Driver for STM32 devices
  - component: CMSIS-Driver:I2C              # I2C Driver for STM32 devices
  - component: CMSIS-Driver:MCI              # MCI Driver for STM32 devices
  - component: CMSIS-Driver:SPI              # SPI Driver for STM32 devices
  - component: CMSIS-Driver:USART            # USART Driver for STM32 devices
  - component: CMSIS-Driver:USB Device       # USB Device Driver for STM32 devices
  - component: CMSIS-Driver:USB Host         # USB Host Driver for STM32 devices
```

### Supported STM32 Device Families

The Driver API Shim implementations are using the STM32Cube HAL interface and therefore support a wide range of STM32 device families. Refer to the STM32 Device Family Packs for further details as some STM32Cube HAL implementations have inconsistencies.

Please feel free to raise an [issue on GitHub](https://github.com/Open-CMSIS-Pack/CMSIS-Driver_STM32/issues)
to report misbehavior (i.e. bugs) or start discussions about enhancements. This
is your best way to interact directly with the maintenance team and the community.

### Driver Validation

The Driver API Shim implementations are validated using the [CMSIS-Driver_Validation](https://github.com/ARM-software/CMSIS-Driver_Validation) tests.

## Usage

Add the following packs and components to your project.

```yml
  packs:
    - pack: Keil::STM32U5xx_DFP@^3.0.0        # choose the DFP that matches your device
    - pack: ARM::CMSIS@^6.0.0
    - pack: ARM::CMSIS-Driver_STM32

  components:
    - component: CMSIS:CORE                   # CMSIS-Core component is required
    - component: Device:CubeMX                # Component that connects to CubeMX    
    - component: CMSIS Driver:USART           # Add Driver API shim interface
```

The drivers require configuration with [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html). The configuration settings are exemplified in the [Driver API Shim Documentation](https://open-cmsis-pack.github.io/CMSIS-Driver_STM32/latest/driver.html), but depend on the actual device that is used.

For additional information refer to:

- [CMSIS-Toolbox - Configure STM32 Devices with CubeMX](https://open-cmsis-pack.github.io/cmsis-toolbox/CubeMX/) for usage information of STM32CubeMX with CMSIS projects. In µVision start CubeMX in dialog [Manage Run-Time Environment](https://developer.arm.com/documentation/101407/0540/Creating-Applications/Software-Components/Managing-Run-Time-Environment) from the component `Device:CubeMX`.

- [CMSIS-Driver - Theory of Operation](https://arm-software.github.io/CMSIS_6/latest/Driver/theoryOperation.html) describes the usage of CMSIS-Drivers in your application code via an [access struct](https://arm-software.github.io/CMSIS_6/latest/Driver/theoryOperation.html#AccessStruct). The driver instances available depend on the CubeMX device configuration as explained in the [Driver API Shim Documentation](https://open-cmsis-pack.github.io/CMSIS-Driver_STM32/latest/driver.html).
