cmake_minimum_required(VERSION 3.13)

# recommended compiler: gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2
# https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

# common variables
set(CMAKE_C_STANDARD    17)
set(CMAKE_CXX_STANDARD  20)

set(ARM_TRIPLET     "arm-none-eabi")
set(X86_TRIPLET     "x86_64-linux-gnu")

# determine which kit was selected by user in VSCode CMake Tools extension
# if(${CMAKE_C_COMPILER} MATCHES "(${ARM_TRIPLET})+") 
set(TARGET_TYPE ARM)
# elseif(${CMAKE_C_COMPILER} MATCHES "(${X86_TRIPLET})+") 
# set(TARGET_TYPE GNU)
# else() 
# message(FATAL_ERROR "No suitable kit found. Aborting.")
# endif()

# load the appropriate toolchain settings (stm32.cmake or linux.cmake)
# if(TARGET_TYPE STREQUAL ARM)
    include(cmake/stm32.cmake)
# elseif(TARGET_TYPE STREQUAL GNU)
    # send pixel data to stdout before its written to the buffer
    #add_compile_definitions(${TARGET} ENABLE_SSD1306_TEST_STDOUT)
    # include(cmake/linux.cmake)    
# endif()

# include(cmake/cppcheck.cmake)

# Declare the project 
project(STEP_SEQUENCER VERSION 0.1 DESCRIPTION "STEP_SEQUENCER")



# set the executable target
add_executable(${BUILD_NAME} "")

# add common sources
add_subdirectory(main_app)
add_subdirectory(extern/embedded_utils)
add_subdirectory(extern/stm32_interrupt_managers)
add_subdirectory(extern/cpp_tlc5955)               # led driver
add_subdirectory(extern/cpp_ssd1306)               # oled driver
add_subdirectory(extern/cpp_midi_stm32)
add_subdirectory(extern/cpp_adp5587) # keyscan/io expander driver              
add_subdirectory(extern/cpp_adg2188) #xpoint switch driver      

# add_compile_definitions(ENABLE_MMC_SPI)
# add_compile_definitions(ENABLE_MMC_SDIO)
add_subdirectory(extern/cpp_fatfs)

# add the stm32-specific sources for the embedded build
if(TARGET_TYPE STREQUAL ARM)
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        # add SEGGER Real-Time-Transfer
        add_subdirectory(segger_rtt)
        add_compile_definitions(USE_RTT)
    endif()
       
    # link in STM32CubeLL static lib
    add_subdirectory(stm32cube_workspace)
    target_link_libraries(${BUILD_NAME} PUBLIC STM32CubeLL)
    target_include_directories(${BUILD_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/stm32cube_workspace")

       
# add the x86-specific sources for host build
elseif(TARGET_TYPE STREQUAL GNU)
    # setup catch2 testing framework
    find_package(Catch2 3 REQUIRED)
    # add the test sources from these dirs
    add_subdirectory(main_app/tests)
    #add_subdirectory(cpp_tlc5955/tests)
    add_subdirectory(cpp_ssd1306/tests)
    # link catch2 into the x86 build
    target_link_libraries(${BUILD_NAME} PRIVATE Catch2::Catch2WithMain)
endif()

# display size info

# add_custom_target(size ALL ${CMAKE_SIZE} ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_NAME} DEPENDS ${BUILD_NAME})
# add_custom_command(TARGET ${BUILD_NAME} POST_BUILD
#     COMMAND ${SIZE} /build/${BUILD_NAME})

# objcopy the elf file as a hex file, if using STM32
if(DEFINED ${HEX_NAME})
    add_custom_target(build.bin ALL DEPENDS ${BUILD_NAME} COMMAND ${CMAKE_OBJCOPY} -O ihex ${BUILD_NAME} ${HEX_NAME})
endif()
