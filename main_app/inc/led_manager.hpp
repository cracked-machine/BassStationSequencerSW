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

#ifndef __LEDMANAGER_HPP__
#define __LEDMANAGER_HPP__

#include <step.hpp>
#include <tlc5955.hpp>

namespace bass_station
{

// @brief manages the tlc5955 driver
class LedManager
{
public:
  // @brief toggle the LAT pin after each full write to the chip common register
  enum class LatchOption
  {
    enable,
    disable
  };

  // @brief Construct a new Sequencer Led Manager object
  explicit LedManager(tlc5955::DriverSerialInterface &serial_interface);

  void reinit_driver(tlc5955::Driver::DisplayFunction display          = tlc5955::Driver::DisplayFunction::display_repeat_off,
                     tlc5955::Driver::TimingFunction timing            = tlc5955::Driver::TimingFunction::timing_reset_off,
                     tlc5955::Driver::RefreshFunction refresh          = tlc5955::Driver::RefreshFunction::auto_refresh_off,
                     tlc5955::Driver::PwmFunction pwm                  = tlc5955::Driver::PwmFunction::normal_pwm,
                     tlc5955::Driver::ShortDetectFunction short_detect = tlc5955::Driver::ShortDetectFunction::threshold_90_percent,
                     std::array<uint8_t, 3> global_brightness          = {{0x1, 0x1, 0x1}},
                     std::array<uint8_t, 3> max_current                = {{0x1, 0x1, 0x1}},
                     uint8_t global_dot_correction                     = 0x1F);

  // @brief Sets a single LED at specific index position and row
  // @param led_position index position within row: 0-15
  // @param row The sequencer row: SequencerRow::upper or SequencerRow::lower
  // @param greyscale_pwm Constrast of LED: 0-65535
  // @param colour Preset colour: LedColour
  // @param latch_option LatchOption
  void set_one_led_at(
      uint16_t led_position, const SequencerRow &row, uint16_t greyscale_pwm, const tlc5955::LedColour &colour, const LatchOption &latch_option);

  // @brief Update all LEDs (both rows) using step/sequence map data structure
  // @tparam LED_NUMBER The size of the sequence array (always 32 in this version)
  // @param step_sequence The array of step objects that make up the full sequence
  template <std::size_t LED_NUMBER>
  void set_both_rows_with_step_sequence_mapping(
      noarch::containers::StaticMap<adp5587::Driver<STM32G0_ISR>::KeyEventIndex, Step, LED_NUMBER> &step_sequence);

  // @brief Run a simple demo that runs boths rows 0->15 then 15->0, for red, green and blue.
  // @param pwm_value The constrast for the iteration
  // @param delay_ms The delay between each iteration. Affects the speed of the demo
  template <std::size_t LED_NUMBER>
  void run_led_sweep(noarch::containers::StaticMap<adp5587::Driver<STM32G0_ISR>::KeyEventIndex, Step, LED_NUMBER> &sequence_map,
                     tlc5955::LedColour colour,
                     uint32_t delay_ms);

  // @brief Convenience function to set the all key leds on to a single colour
  // @param greyscale_pwm
  // @param colour
  void set_all_leds_both_rows(uint16_t greyscale_pwm, const tlc5955::LedColour &colour);

private:
  // @brief The TLC5955 driver instance
  tlc5955::Driver m_tlc5955_driver;
};

template <std::size_t LED_NUMBER>
void LedManager::set_both_rows_with_step_sequence_mapping(
    noarch::containers::StaticMap<adp5587::Driver<STM32G0_ISR>::KeyEventIndex, Step, LED_NUMBER> &sequence_map)
{
  // get the start, mid, end iterators for this input map
  auto start_pos = sequence_map.data.begin();
  auto mid_pos   = sequence_map.data.begin() + sequence_map.data.size() / 2;
  auto end_pos   = sequence_map.data.end();

  m_tlc5955_driver.clear_register();

  // set the TLC5955 register data for the upper row keys
  std::for_each(mid_pos,
                end_pos,
                [this, &sequence_map](const std::pair<adp5587::Driver<STM32G0_ISR>::KeyEventIndex, Step> &data_pair)
                {
                  Step current_step = data_pair.second;
                  if (current_step.m_state == StepState::ON)
                  {
                    // remap the logical array positions to the physical PCB wiring
                    m_tlc5955_driver.set_position_and_colour(current_step.m_tlc5955_pin_index, current_step.m_colour);
                  }
                });

  m_tlc5955_driver.send_first_bit(tlc5955::Driver::DataLatchType::data);
  m_tlc5955_driver.send_spi_bytes(tlc5955::Driver::LatchPinOption::no_latch);

  // clear buffer so that the upper row data that was just sent, does not contaminatate the lower row data we are
  // about to send
  m_tlc5955_driver.clear_register();

  // set the TLC5955 register data for the lower row keys
  std::for_each(start_pos,
                mid_pos,
                [this, &sequence_map](const std::pair<adp5587::Driver<STM32G0_ISR>::KeyEventIndex, Step> &data_pair)
                {
                  Step current_step = data_pair.second;
                  if (current_step.m_state == StepState::ON)
                  {
                    // remap the logical array positions to the physical PCB wiring
                    m_tlc5955_driver.set_position_and_colour(current_step.m_tlc5955_pin_index, current_step.m_colour);
                  }
                });

  // send the lower row data with latch
  m_tlc5955_driver.send_first_bit(tlc5955::Driver::DataLatchType::data);
  m_tlc5955_driver.send_spi_bytes(tlc5955::Driver::LatchPinOption::latch_after_send);
}

/// @brief Turn on/off each sequencer LED in turn, then repeat for next colour
/// @tparam LED_NUMBER
/// @param sequence_map
/// @param pwm_value
/// @param delay_ms
template <std::size_t LED_NUMBER>
void LedManager::run_led_sweep(noarch::containers::StaticMap<adp5587::Driver<STM32G0_ISR>::KeyEventIndex, Step, LED_NUMBER> &sequence_map
                               [[maybe_unused]],
                               tlc5955::LedColour colour,
                               uint32_t delay_ms)
{

  // incremental sweep
  size_t position_offset = 16;
  size_t lower_idx       = sequence_map.data.max_size() - position_offset;
  for (size_t upper_idx = sequence_map.data.max_size() - 1; upper_idx > position_offset; --upper_idx)
  {

    --lower_idx;
    Step current_upper_step = sequence_map.data[upper_idx].second;
    Step current_lower_step = sequence_map.data[lower_idx].second;

    set_one_led_at(current_upper_step.m_tlc5955_pin_index, bass_station::SequencerRow::upper, 65353, colour, LatchOption::disable);
    set_one_led_at(current_lower_step.m_tlc5955_pin_index, bass_station::SequencerRow::lower, 65353, colour, LatchOption::enable);
    stm32::delay_millisecond(delay_ms);
  }

  // decremental sweep
  lower_idx = (sequence_map.data.max_size() / 2) + position_offset;
  for (size_t upper_idx = position_offset; upper_idx < sequence_map.data.max_size() - 1; ++upper_idx)
  {
    if (lower_idx < position_offset)
    {
      lower_idx++;
    }
    else
    {
      lower_idx = 0;
    }
    Step current_upper_step = sequence_map.data[upper_idx].second;
    Step current_lower_step = sequence_map.data[lower_idx].second;

    set_one_led_at(current_upper_step.m_tlc5955_pin_index, bass_station::SequencerRow::upper, 65353, colour, LatchOption::disable);
    set_one_led_at(current_lower_step.m_tlc5955_pin_index, bass_station::SequencerRow::lower, 65353, colour, LatchOption::enable);
    stm32::delay_millisecond(delay_ms);
  }
}

} // namespace bass_station

#endif // __LEDMANAGER_HPP__