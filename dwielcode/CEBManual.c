// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your left
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

#include "CEBManual.h"

#define DIECIMILA 1
#ifdef TEENSY
const int blink_bin        = 13;

// sensor pins
const int sensor_primary   = 16;
const int sensor_secondary = 17;

// output pins
const int primary_down    = 15;
const int primary_up      = 14;
const int secondary_left  = 4;
const int secondary_right = 5;
const int shaker          = 9;

// button pins
const int manual_down   = 18;
const int manual_up     = 19;
const int manual_left   = 20;
const int manual_right  = 21;
const int manual_shaker = 13;
#endif

#ifdef DIECIMILA
const int blink_bin        = 11;

// sensor pins
const int sensor_primary   = 16;
const int sensor_secondary = 17;

// output pins
const int primary_down    = 15;
const int primary_up      = 14;
const int secondary_left  = 4;
const int secondary_right = 5;
const int shaker          = 9;

// button pins
const int manual_down   = 18;
const int manual_up     = 19;
const int manual_left   = 20;
const int manual_right  = 21;
const int manual_shaker = 13;
#endif

void setup() {
  pinMode(sensor_primary,   INPUT);
  pinMode(sensor_secondary, INPUT);

  pinMode(primary_down,    OUTPUT);
  pinMode(primary_up,      OUTPUT);
  pinMode(secondary_left,  OUTPUT);
  pinMode(secondary_right, OUTPUT);
  pinMode(shaker,          OUTPUT);
  
  pinMode(manual_down,   INPUT_PULLUP);
  pinMode(manual_up,     INPUT_PULLUP);
  pinMode(manual_left,   INPUT_PULLUP);
  pinMode(manual_right,  INPUT_PULLUP);
  pinMode(manual_shaker, INPUT_PULLUP);
}

void manual_control(int output_pin, int input_pin) {
  if(digitalRead(input_pin)) {
    digitalWrite(output_pin, LOW);
  } else {
    digitalWrite(output_pin, HIGH);
  }
}

// blinking helps know that the control board is powered
const unsigned int blink_speed = 30000;
void loop() {
  unsigned int shaker_state = 0;
  unsigned int i = 0;
  unsigned int shaker_timer = 0;
  unsigned int last_shaker_toggle_state = 0;
  unsigned int v = 0;
  
  while(1) {
    // blinker
    if(i < blink_speed) {
      digitalWrite(11, HIGH);
    } else {
      digitalWrite(11, LOW);
    }

    if(i >= blink_speed*2) {
      i = 0;
    } 
    i += 1;

    /*    
    // primary
    v = analogRead(sensor_primary);
    if(v < 10) {
      //digitalWrite(11, HIGH);
      digitalWrite(primary_down, HIGH);
    } else {
      //digitalWrite(11, LOW);
      digitalWrite(primary_down, LOW);
    }

    // secondary
    v = analogRead(sensor_secondary);
    if(v < 10) {
      //digitalWrite(11, HIGH);
      digitalWrite(secondary_right, HIGH);
    } else {
      //digitalWrite(11, LOW);
      digitalWrite(secondary_right, LOW);
    }
        
    digitalWrite(primary_down,    !digitalRead(manual_down));
    digitalWrite(primary_up,      !digitalRead(manual_up));
    digitalWrite(secondary_left,  !digitalRead(manual_left));
    digitalWrite(secondary_right, !digitalRead(manual_right));
    
    // the shaker_timer is used to sample the toggle input to avoid 
    // switch bouncing to oscilate the shaker state
    if(shaker_timer > 10000) {
      shaker_timer = 0;
      
      // sample the manual_shaker state
      unsigned int current_shaker_toggle_state = !digitalRead(manual_shaker);
      
      // if the current state is on and the last state was off, toggle the
      // state of the shaker output
      if(current_shaker_toggle_state && !last_shaker_toggle_state) {
        shaker_state = !shaker_state;
        digitalWrite(shaker, shaker_state);
      }
      
      last_shaker_toggle_state = current_shaker_toggle_state;
    } else {
      shaker_timer += 1;
    }
    */
  }
}

void move_until(int pin, fnptr until, int params){
  digitalWrite(pin, HIGH);
  until(params);
  digitalWrite(pin, LOW);
}

void until_sensor(int sensor_pin) {
  // TODO
  while(1) {
    // turn off sensors
    // wait for reset
    // turn on sensors
    // wait for new state
    // read sensor
    // if sensor activated, return
  }
}

void _delay(int t) {
  delay(t);
}

void other() {
  // wait for press to be moved to pre-loading position - secondary extended, 
  // primary extended
  
  // start cycling
  while(1) {
    // turn on shaker
    digitalWrite(shaker, HIGH);
    
    // lower primary cylinder to sensor
    move_until(primary_down, &until_sensor, sensor_primary);
    
    // wait for chamber to fill
    delay(500);
    
    // retract secondary cylinder to center via sensor
    move_until(secondary_left, &until_sensor, sensor_secondary);
    
    // turn off shaker
    digitalWrite(shaker, LOW);
    
    // compress block
    // extend primary cylinder for time or to pressure
    move_until(primary_up,      &_delay, 500);
    
    // reduce pressure on block
    // slightly retract primary cylinder to ready ejection
    move_until(primary_down,    &_delay, 50);
    
    // allow brick to be ejected out of compression chamber
    // retract secondary cylinder for time or to pressure
    move_until(secondary_right, &_delay, 500);
    
    // eject block out of compression chamber
    // extend primary cylinder for time or to pressure
    move_until(primary_up,      &_delay, 200);
    
    // push block into queue and ready for loading new soil
    // extend secondary cylinder for time or to pressure
    move_until(secondary_right, &_delay, 500);
  }
}
