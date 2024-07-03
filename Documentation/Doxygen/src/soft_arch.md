# Software Architecture {#soft_arch}

The diagram below outlines the software architecture that is used for example
by [Reference Applications](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/ReferenceApplications.md) and
the [MDK middleware examples](https://github.com/ARM-software/MDK-Middleware/tree/main/Examples).
Refer to the [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/README.md) 
documentation [Reference Applications - Header File Structure](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/ReferenceApplications.md#header-file-structure)
for a description of the various components in this diagram.
The Driver API Shim Interface is configured using [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) (CubeMX)
that generates via the CMSIS tool integration the file `MX_Device.h`.

![Software Architecture](./SW-Architecture.png "Software Architecture")
