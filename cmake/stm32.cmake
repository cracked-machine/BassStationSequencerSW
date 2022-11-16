message("\n** Configuring STM32 Environment **")
message("** Selected Kit: ${CMAKE_C_COMPILER} **\n")

set(CMAKE_SYSTEM_NAME		Linux)
set(CMAKE_SYSTEM_VERSION	1)
set(CMAKE_SYSTEM_PROCESSOR	arm-eabi)

# binutils language settings
# compilers are set by the select kit in vscode (CMAKE_C_COMPILER/CMAKE_CXX_COMPILER)
set(CMAKE_OBJCOPY			arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP			arm-none-eabi-objdump)
set(CMAKE_SIZE			    ${SIZE})

set(TARGET              STM32G0B1KETxN)
set(ARCH			    armv6-m)
set(CORE			    cortex-m0plus)
set(ARM_ASM             mthumb)
set(LINKER_SCRIPT       ${CMAKE_SOURCE_DIR}/stm32cube_workspace/G0B1KET6N/STM32G0B1KETXN_FLASH.ld)
set(BUILD_NAME          build.elf)
set(HEX_NAME            build.hex)
set(MAP_NAME            build.map)

# common build settings
set(STACK_USAGE "-fstack-usage -Wstack-usage=10000")
set(OPTIMZATIONS "-ffunction-sections -Wl,--gc-sections -fdata-sections -fsection-anchors")
set(WARNING_FLAGS "-Wall -Werror -Wextra -Wdouble-promotion -Wformat=2 -Wformat-overflow -Wundef -Wformat-truncation -Wfloat-equal -Wshadow")
set(COMMON_FLAGS "-march=${ARCH} -mcpu=${CORE} -${ARM_ASM} -mfloat-abi=soft -ffreestanding -fno-builtin -fno-exceptions")
set(CMAKE_EXE_LINKER_FLAGS  "-mthumb -mcpu=${CORE} -specs=nosys.specs -Wl,-Map=${MAP_NAME} -fno-exceptions -T${LINKER_SCRIPT}" CACHE INTERNAL "exe link flags")

# C compiler settings
set(C_FLAGS "--specs=nano.specs")
set(CMAKE_C_FLAGS "${C_FLAGS} ${COMMON_FLAGS} ${STACK_USAGE} ${WARNING_FLAGS} ${OPTIMZATIONS} " CACHE INTERNAL "c compiler flags")
enable_language(C)

# c++ compiler settings
set(CMSIS_COMPAT "-Wno-volatile")
set(CPP_FLAGS "-fno-rtti -fno-exceptions -fno-use-cxa-atexit -fno-threadsafe-statics -ftemplate-backtrace-limit=0 ${CMSIS_COMPAT}")
set(CMAKE_CXX_FLAGS "${CPP_FLAGS} ${COMMON_FLAGS} ${STACK_USAGE} ${WARNING_FLAGS} ${OPTIMZATIONS}" CACHE INTERNAL "cpp compiler flags")
enable_language(CXX)

## Assembler compiler settings
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -MP -MD -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
enable_language(ASM)

# Required to include STM32 LL framework
add_compile_definitions(${TARGET} USE_FULL_LL_DRIVER)

# used to include stm32g0xx.h dependency (and maybe stm32g0xx LL)
add_compile_definitions(
    ${TARGET} STM32G0B1xx
)

message("CMAKE_CXX_FLAGS_DEBUG:" ${CMAKE_CXX_FLAGS_DEBUG})
message("CMAKE_CXX_FLAGS_RELEASE:" ${CMAKE_CXX_FLAGS_RELEASE})
message("CMAKE_CXX_FLAGS_MINSIZEREL:" ${CMAKE_CXX_FLAGS_MINSIZEREL})
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO:" ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

