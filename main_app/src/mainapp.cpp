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


#include "mainapp.hpp"
#include <ssd1306.hpp>

// Owns LedManager part
#include <sequence_manager.hpp>

#include <adp5587.hpp>
#include <adg2188.hpp>

#ifdef __cplusplus
extern "C"
{
#endif


uint8_t font_count = 0;
ssd1306::Font5x7 font2;
ssd1306::Display oled(SPI1, ssd1306::Display::SPIDMA::enabled);


void update_oled(std::string &msg)
{
	std::string font_char {font2.character_map[font_count]};
	std::string final_msg = msg + font_char;
	ssd1306::ErrorStatus res = oled.write(final_msg, font2, 2, 2, ssd1306::Colour::Black, ssd1306::Colour::White, 3, true);
	if (res != ssd1306::ErrorStatus::OK) 
	{ 
		#if defined(USE_RTT) 
			SEGGER_RTT_printf(0, "ssd1306::Display::write(): Error\n"); 
		#endif	
	}

	if (font_count < font2.character_map.size() - 1) { font_count++; }
	else { font_count = 0; }	
}



void mainapp()
{

	// setup SSD1306 IC display driver
	oled.init();
	std::string msg {"Hello "};
	
	adg2188::Driver xpoint(I2C2);
	static bass_station::SequenceManager sequencer;

    uint16_t delay_ms {400};

	while(true)
	{


		// update sequencer LEDs
		// led_manager.update_ladder_demo(pwm_value, delay);
		
		// update oled animation
		update_oled(msg);		
		// LL_mDelay(10);


		sequencer.execute_sequence(delay_ms);


	}
}

#ifdef __cplusplus
}
#endif
