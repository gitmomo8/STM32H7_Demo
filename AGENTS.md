# AGENTS.md - STM32H7_Demo

## Project Type
Embedded STM32H743VIT6 firmware (ARM Cortex-M7), not a typical software project.

## Toolchain
- **IDE**: Keil uVision (MDK-ARM)
- **Project file**: `MDK-ARM/STM32H7_Demo.uvprojx`
- **Config**: `.mxproject` and `STM32H7_Demo.ioc` (STM32CubeMX)

## Directory Structure
- `Core/Src/`, `Core/Inc/` - Application source code
- `Drivers/` - STM32H7xx HAL drivers
- `FreeRTOS/` - FreeRTOS kernel (if used)
- `MDK-ARM/` - Keil project artifacts, output build files

## Build
Open `STM32H7_Demo.uvprojx` in Keil uVision and build from IDE. No command-line build available.

## Coding
- Main entry: `Core/Src/main.c`
- HAL config: `Core/Inc/stm32h7xx_hal_conf.h`
- System clock: `Core/Src/system_stm32h7xx.c`

## Key Constraints
- No typical lint/test/typecheck - embedded bare-metal code
- Hardware-dependent (JTAG/SWD debug probe needed)
- Changes to `.ioc` regenerate code - edit in STM32CubeMX