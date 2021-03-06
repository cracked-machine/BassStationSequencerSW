## Requirements

This project uses:

- `VSCode`. This is the main IDE.
- `gcc` compiler. 
    - For STM32G0B1 target: [`GNU Arm Embedded Toolchain 10.3-2021.10`](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
    - For x86_64 Linux CTests: `GCC 10.3.0 x86_64-linux-gnu`
- `cmake v3.22` to build the project. it is recommened to use VSCode with the `CMake tools` extension.
- `SEGGER JLink` to debug the device. You will need to purchase [JLink probe from SEGGER](https://www.segger.com/products/debug-probes/j-link/).  You also need to install [JLink SW and doc pack](https://www.segger.com/downloads/jlink/)
- `SEGGER RTT` to view printf output. JLinkRTTViewerExe is included in the JLink SW pack.
- `STM32CubeIDE v1.6.1` to generate the startup file, linker script and the LL related library files. _This IDE is not used to compile the project._

## Installing the project

This project uses the `cpp_ssd1306` and `cpp_tlc5955` submodule projects.

After cloning, cd into the root directory and run `git submodule update --init --recursive`

## Updating the STM32CubeIDE generated code

STM32CubeIDE uses an Eclipse IDE that has some quirks(!)

To open the project

- open the workspace in the `stm32_workspace` directory
- Use `File > Import... > General > Existing project into Workspace` to import the project and update the paths. You should now see the `G0B1KET6N` project in the STM32CubeIDE Project Explorer.
- Double-click the `G0B1KET6N.ioc` file to open the Device Config Tool.
- After making device changes, use the generate icon in the toolbar to update the project code.
