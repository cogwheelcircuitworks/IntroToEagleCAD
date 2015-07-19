
#include "EagleCadClassBlinky.h" // pull in class definitions
#include "Arduino.h" // pull in regular Arduino cruft

/*
 Demonstration software for the ATTiny-based board that gets built in the Eagle CAD Class
 To use this library create a sketch that looks like this:

#include <EagleCADClassBlinky.h> // <---do an 'sketch->include library' to add this line

CadBlinky cadblinky; // instantiate a blinky objecxt

void setup() {
  cadblinky.Setup(); // initial blinky setup
}

void loop() {
  cadblinky.Loop(); // run repeatedly forever
}

 */
const unsigned char CadBlinky::num_leds = CADBLINKY_NUMLEDS; // number of leds around the face
const unsigned char CadBlinky::led2pin[] = {3,4}; // Led numbers to pin numbers for digitalWrite()
const unsigned char CadBlinky::num_slots = CADBLINKY_PWM_SLOTS; // Define periods for software pwm brightness control
bool CadBlinky::pwm_slots[CADBLINKY_PWM_SLOTS][CADBLINKY_NUMLEDS]; // where pwm brightness is defined per-led
unsigned char CadBlinky::blink_pattern = CADBLINKY_MODE_0; // A blink_pattern is a blink pattern
unsigned int CadBlinky::blink_pattern_iterations = 0; // how many times a pattern has repeated
int CadBlinky::loop_rate = CADBLINKY_LOOP_RATE; // basic speed set by this
// gets incremented each visit. 
// things change only when !(loop_counter % loop_rate )
unsigned int CadBlinky::loop_counter = 0;  

void CadBlinky::Setup(void)
{
  // called once 
  unsigned char i,j;
  for (j = 0; j < num_leds; j++) {
    pinMode(led2pin[j], OUTPUT); // make LED pins outputs
  }

  for (i = 0; i < num_slots; i++) {
    for (j = 0; j < num_leds; j++) {
      pwm_slots[i][j] = false; // turn leds off to start
    }
  }

}



void CadBlinky::Loop(void)
{
  // called repeatedly from top loop() in main sketch
  switch(blink_pattern) {
    // each mode will decide when to increment blink_pattern which will fire next case
    case CADBLINKY_MODE_0:
      Mode_0();
      // if this blink_pattern has been going on long enough, go to next blink_pattern
      if (blink_pattern_iterations == 1) { // bumped inside the Mode*() functions
          blink_pattern_iterations = 0;
          blink_pattern++;
      }
      break;
    case CADBLINKY_MODE_1:
      Mode_1();
      // if this blink_pattern has been going on long enough, go to next blink_pattern
      if (blink_pattern_iterations == 1) { // bumped inside the Mode*() functions
          blink_pattern_iterations = 0;
          blink_pattern++;
      }
      break;

    case CADBLINKY_MODE_2:
      Mode_2(); 
      static unsigned int last_blink_pattern_iteration_val;
      static char direction = -3;

      if (last_blink_pattern_iteration_val != blink_pattern_iterations) {
          // if Mode*() thinks it's gone through one entire iteration ..
          last_blink_pattern_iteration_val = blink_pattern_iterations; // reset to detect next one..
          loop_rate += direction; // manipulate speed of the pattern up or down

          if (loop_rate < 0) { // if we are going as fast as we can..
            loop_rate -= direction; 
            direction = 3; // then reverse direction..
          }

          if (loop_rate >= CADBLINKY_LOOP_RATE)  {
            // if we've gotten back to default speed
            // then reset everthing and go on to the next patter
            direction = -3; 
            loop_rate = CADBLINKY_LOOP_RATE;
            blink_pattern_iterations = 0;
            blink_pattern++;
          }
      }
      break;

    case CADBLINKY_MODE_3:
      // we don't do anything yet. Just go back to the initial pattern
      blink_pattern = 0;
      break;
  }


  PwmChores();

}

void CadBlinky::Mode_0() {
  // ramp up/down
  static char bri = 0;
  static char direction = 1; // -1 or 1

  loop_counter++;
  if (!(loop_counter % loop_rate)) {
    unsigned char i,j;

    // increment decrement brightness each time
    bri += direction;
    if (bri == num_slots)
      direction = -1;

    if (bri == 0) {
      blink_pattern_iterations++;
      direction = 1;
    }

    SetBrightness(CADBLINKY_LED_1,bri);
    SetBrightness(CADBLINKY_LED_2,bri);

  }
}

void CadBlinky::Mode_1() {
  // alternate up/down 
  static char bri = 0;
  static char direction = 1; // -1 or 1
  static char led_no = CADBLINKY_LED_1;
  static char led_iterations  = 0 ;

  if (!(loop_counter++ % loop_rate)) {
    unsigned char i,j;

    // increment decrement brightness each time
    bri += direction;

    SetBrightness(led_no,bri);

    if (bri == num_slots)
      direction = -1;


    if (bri == 0) {
      direction = 1;
      led_no = (led_no == CADBLINKY_LED_1 ? CADBLINKY_LED_2 : CADBLINKY_LED_1);
      led_iterations++;
    }
    if (led_iterations == 2) {
      led_iterations = 0;
      blink_pattern_iterations++;
    }


  }
}

void CadBlinky::Mode_2() {
  // manipulate the alternate up/down by modulating rate 
  Mode_1();
}

void CadBlinky::Mode_3() {
  // manipulate the alternate up/down by modulating rate 
  Mode_1();
}

void CadBlinky::SetBrightness(unsigned char led_no, unsigned char led_bri) {
  unsigned char i,j;
  // change brightness by filling in timeslots which get emitted by PwmChores();
  for (i = 0; i < num_slots; i++) {
      if (i < led_bri)
        pwm_slots[i][led_no] = true;
      else 
        pwm_slots[i][led_no] = false;
     
  }

}

void CadBlinky::PwmChores(void) {
  // take care of pwm'ing the LEDs
  // there are num_slots opportunitys for the led to be on or off
  // turning all slots to 1 will be full brightness
  // turning all slots to 0 will be off
  unsigned char i,j;
  for (i = 0; i < num_slots; i++) {
    for (j = 0; j < num_leds; j++) {
      if (pwm_slots[i][j])
        digitalWrite(led2pin[j], CADBLINKY_LED_ON);
      else
        digitalWrite(led2pin[j], CADBLINKY_LED_OFF);
    }
  }
}

