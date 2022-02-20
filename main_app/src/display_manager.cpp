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

#include <display_manager.hpp>

namespace bass_station 
{

DisplayManager::DisplayManager(ssd1306::DriverSerialInterface<stm32::isr::InterruptTypeStm32g0> &display_spi_interface) 
: m_oled(ssd1306::Driver<stm32::isr::InterruptTypeStm32g0>(display_spi_interface, ssd1306::Driver<stm32::isr::InterruptTypeStm32g0>::SPIDMA::enabled))
{
    // init SSD1306 IC display driver
	m_oled.power_on_sequence();
    
}

void DisplayManager::set_display_line(DisplayLine line, std::string &msg)
{
    switch(line)
    {
        case DisplayLine::LINE_ONE:
            m_display_line1 = msg;
            break;
        case DisplayLine::LINE_TWO:
            m_display_line2 = msg;
            break;
        case DisplayLine::LINE_THREE:
            m_display_line3 = msg;
            break;        
        case DisplayLine::LINE_FOUR:
            m_display_line4 = msg;
            break;
        case DisplayLine::LINE_FIVE:
            m_display_line5 = msg;
            break;        
        case DisplayLine::LINE_SIX:
            m_display_line6 = msg;
            break;        
    }
    
}

void DisplayManager::update_oled()
{
	m_oled.write(m_display_line1, m_font, 0, 0, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
    m_oled.write(m_display_line2, m_font, 0, 10, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
    m_oled.write(m_display_line3, m_font, 0, 20, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
    m_oled.write(m_display_line4, m_font, 0, 30, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
    m_oled.write(m_display_line5, m_font, 0, 40, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
    m_oled.write(m_display_line6, m_font, 0, 50, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
}


} // namespace bass_station 