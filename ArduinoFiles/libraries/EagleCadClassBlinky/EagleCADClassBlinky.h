/*
  CadBlinky Library Functions

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef CadBlinky_h
#define CadBlinky_h

#include "Arduino.h"

#define CADBLINKY_NUMLEDS 2
#define CADBLINKY_LED_ON false
#define CADBLINKY_LED_OFF true
#define CADBLINKY_LED_1 0
#define CADBLINKY_LED_2 1


class CadBlinky
{
  public:
    void Setup(void);
    void Loop(void);
    void PwmChores(void);
    void SetBrightness(unsigned char , unsigned char );
    void Mode_0(void);
    void Mode_1(void);
    void Mode_2(void);
    void Mode_3(void);
  private:
    static const unsigned char num_leds;  // max number of leds we are dealing with
    static const unsigned char led2pin[CADBLINKY_NUMLEDS]; // maps led #'s to pin #'s used in DigitalWrite(), etc..
#define CADBLINKY_PWM_SLOTS 16
    static const unsigned char num_slots;  // max number of leds we are dealing with
    static bool pwm_slots[CADBLINKY_PWM_SLOTS][CADBLINKY_NUMLEDS];
    static unsigned int loop_counter;
#define CADBLINKY_LOOP_RATE 20
    static int loop_rate;
#define CADBLINKY_MODE_0 0
#define CADBLINKY_MODE_1 1 
#define CADBLINKY_MODE_2 2
#define CADBLINKY_MODE_3 3
#define CADBLINKY_NUM_MODES 4
    static unsigned char blink_pattern; // which blinky blink_pattern are wen ?
    static unsigned int blink_pattern_iterations;  // how many full iterations have we done in this blink_pattern ?
}; 

#endif // CadBlinky_h
