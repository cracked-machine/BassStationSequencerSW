// MIT License

// Copyright (c) 2021 Chris Sutton

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

#ifndef __DISPLAY_MANAGER_HPP__
#define __DISPLAY_MANAGER_HPP__

#include <stm32g0_interrupt_manager.hpp>
#include <ssd1306.hpp>

namespace bass_station
{

// This class manages the SSD1306 driver for the 128x64 pixel OLED display
class DisplayManager
{
public:
    // @brief Construct a new Display Manager object
    // @param timer Used for refresh rate of the display
    DisplayManager(TIM_TypeDef *timer);
    
    void start_isr();
    
    enum class DisplayLine
    {
        LINE_ONE,
        LINE_TWO,
        LINE_THREE,
        LINE_FOUR,
        LINE_FIVE,
        LINE_SIX,
    };
    void set_display_line(DisplayLine line, std::string &msg);
private:

    std::string m_display_line1{"line1"};
    std::string m_display_line2{"line2"};
    std::string m_display_line3{"line3"};
    std::string m_display_line4{"line4"};
    std::string m_display_line5{"line5"};
    std::string m_display_line6{"line6"};

    

    std::unique_ptr<TIM_TypeDef> m_timer_device;

    uint8_t m_font_count = 0;
    ssd1306::Font5x7 m_font;
    ssd1306::Display m_oled{SPI1, ssd1306::Display::SPIDMA::enabled};

    void update_oled();
    void display_timer_isr();

#ifdef USE_RAWPTR_ISR
	struct TimerIntHandler : public stm32::isr::STM32G0InterruptManager
	{
        // @brief the parent driver class
        DisplayManager *m_display_man_ptr;
		// @brief initialise and register this handler instance with STM32G0InterruptManager
		// @param parent_driver_ptr the instance to register
		void initialise(DisplayManager *display_man_ptr)
		{
			m_display_man_ptr = display_man_ptr;
			// register this internal handler class in stm32::isr::STM32G0InterruptManager
			stm32::isr::STM32G0InterruptManager::register_handler(stm32::isr::STM32G0InterruptManager::InterruptType::tim15, this);
		}        
        // @brief The callback used by STM32G0InterruptManager
		virtual void ISR()
		{
            m_display_man_ptr->display_timer_isr();
		}        
	};
	// @brief SequenceManager's TIM16 interrupt handler member
    TimerIntHandler m_display_timer_isr_handler;
#endif // USE_RAWPTR_ISR    

};

} // namespace bass_station

#endif // __DISPLAY_MANAGER_HPP__

