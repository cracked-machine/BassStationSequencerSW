// MIT License

// Copyright (c) 2022 Chris Sutton

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "mainapp.hpp"
#include <adp5587.hpp>
#include <file_manager.hpp>
#include <sequence_manager.hpp>
#include <timer_manager.hpp>

#ifdef __cplusplus
extern "C"
{
#endif

void _close(void)
{
}
void _lseek(void)
{

}
void _read(void)
{
}
void _write(void)
{
}

#define ENABLE_FATFS 0

  void error_handler()
  {
    while (true)
    {
    }
  }

  void mainapp()
  {

    // initialise the timer used for system wide microsecond timeout
    if (stm32::TimerManager::initialise(TIM6) == false)
    {
      error_handler();
    }

#if ENABLE_FATFS
    // setup fatfs support for uSDCard
    fatfs::DiskioProtocolSPI fatfs_spi_interface(SPI2,
                                                 std::make_pair(GPIOB, GPIO_BSRR_BS8), // sck  - PB8
                                                 std::make_pair(GPIOB, GPIO_BSRR_BS7), // mosi - PB7
                                                 std::make_pair(GPIOD, GPIO_BSRR_BS3), // miso - PD3
                                                 std::make_pair(GPIOD, GPIO_BSRR_BS2), // cs  	- PD2
                                                 RCC_APBENR1_SPI2EN);
#endif

    // bass_station::FileManager spi_fm(fatfs_spi_interface);

    // Timer peripheral for sequencer manager rotary encoder control
    TIM_TypeDef *sequencer_encoder_timer = TIM1;

    // SPI peripheral for SSD1306 display driver serial communication
    ssd1306::DriverSerialInterface<STM32G0_ISR> ssd1306_spi_interface(SPI1,
                                                                      std::make_pair(GPIOA, GPIO_BSRR_BS0), // PA0 - DC
                                                                      std::make_pair(GPIOA, GPIO_BSRR_BS3), // PA3 - Reset
                                                                      STM32G0_ISR::dma1_ch2);

    // I2C peripheral for keypad manager serial communication
    I2C_TypeDef *ad5587_keypad_i2c = I2C3;

    // keypad debounce timer
    TIM_TypeDef *general_purpose_debounce_timer = TIM17;

    // I2C periperhal for synth output control switch serial communication
    I2C_TypeDef *adg2188_control_sw_i2c = I2C2;

    // std::pair<GPIO_TypeDef*, uint16_t> test (GPIOB, GPIO_BSRR_BS9);

    // SPI peripheral for TLC5955 LED driver serial communication
    tlc5955::DriverSerialInterface tlc5955_spi_interface(SPI2,
                                                         std::make_pair(GPIOB, GPIO_BSRR_BS9), // latch port+pin
                                                         std::make_pair(GPIOB, GPIO_BSRR_BS7), // mosi port+pin
                                                         std::make_pair(GPIOB, GPIO_BSRR_BS8), // sck port+pin
                                                         std::make_pair(TIM4, TIM_CCER_CC1E),  // gsclk timer+channel
                                                         RCC_IOPENR_GPIOBEN,                   // for enabling GPIOB clock
                                                         RCC_APBENR1_SPI2EN                    // for enabling SPI2 clock
    );

    // The USART and Timer used to send the MIDI heartbeat
    midi_stm32::DeviceInterface<STM32G0_ISR> midi_usart_interface(USART5, STM32G0_ISR::usart5);

    // initialise the sequencer
    // auto timer_isr_pair = std::make_pair(*TIM3, STM32G0_ISR::tim3);
    bass_station::SequenceManager sequencer(std::make_pair(TIM3, STM32G0_ISR::tim3), // Timer peripheral for sequencer manager tempo control
                                            sequencer_encoder_timer,
                                            ssd1306_spi_interface,
                                            ad5587_keypad_i2c,
                                            general_purpose_debounce_timer,
                                            adg2188_control_sw_i2c,
                                            tlc5955_spi_interface,
                                            midi_usart_interface);

    sequencer.main_loop();
    // we should never get past here
  }

#ifdef __cplusplus
}
#endif
