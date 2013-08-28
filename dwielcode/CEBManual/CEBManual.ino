// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your right
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

// TODO: reset actions shouldnt eject uncompressed soil
// TODO: idea: pause if secondary cylinder hits pressure before
// expected (rock).  Button near pitch forker could tell the machine
// to move the cylinder back the way it came, and try again (to break
// the rock), rather than just detecting unexpected pressure and
// moving on. Similar logic could happen at all steps and set the
// press in 'unexpected behavior pause' mode which the operator can
// resume from once they figure out what caused the problem.

#include "CEBManual.h"

// NOTE: your serial line might conflict with pins you are using for IO
const boolean serial = true;

// in some versions of the hardware, hydraulic outputs should be inverted
// with this wrapper function, we can select this with a constant set in
// the .h file
void _digitalWrite(int pin, int out) {
  if(invert_output) {
    digitalWrite(pin, !out);
  } else {
    digitalWrite(pin, out);
  }
}

// the first function called when the arduino turns on
void setup() {
  // start by setting outputs low so that floating outputs don't cause
  // the press to jerk randomly while the arduino boots
  pinMode(primary_up,      OUTPUT);
  pinMode(primary_down,    OUTPUT);
  pinMode(secondary_left,  OUTPUT);
  pinMode(secondary_right, OUTPUT);
  pinMode(shaker,          OUTPUT);

  _digitalWrite(primary_up,      LOW);
  _digitalWrite(primary_down,    LOW);
  _digitalWrite(secondary_left,  LOW);
  _digitalWrite(secondary_right, LOW);
  _digitalWrite(shaker,          LOW);
  
  pinMode(manual_down,   INPUT_PULLUP);
  pinMode(manual_up,     INPUT_PULLUP);
  pinMode(manual_left,   INPUT_PULLUP);
  pinMode(manual_right,  INPUT_PULLUP);
  pinMode(manual_shaker, INPUT_PULLUP);
  
  pinMode(pause_pin,     INPUT_PULLUP);
  pinMode(mode_pin,      INPUT_PULLUP);

  pinMode(sensor_primary,   INPUT);
  pinMode(sensor_secondary, INPUT);
  pinMode(knob_primary,     INPUT);
  pinMode(sensor_pressure,  INPUT_PULLUP);

  if(serial) {
    Serial.begin(115200);
  }
}

// these states are used by the various finate state machines
unsigned int semiautomatic_loop_state = 0;
int auto_loop_state = 0;
int reset_state = 0;

void manual() {
    // a manual mode for directly controlling the cylinders with switches
    // TODO: re-add support for toggling the shaker motor
    //       see manual_loop() further down
/*    Serial.print(!digitalRead(manual_up));
    Serial.print(!digitalRead(manual_down));
    Serial.print(!digitalRead(manual_left));
    Serial.print(!digitalRead(manual_right));
    Serial.println();
*/
    _digitalWrite(primary_up,      !digitalRead(manual_up));
    _digitalWrite(primary_down,    !digitalRead(manual_down));
    _digitalWrite(secondary_left,  !digitalRead(manual_left));
    _digitalWrite(secondary_right, !digitalRead(manual_right));
    _digitalWrite(shaker,          !digitalRead(manual_shaker));
}

void loop() {
  // determine which mode we are in
  if(digitalRead(mode_pin) ^ invert_mode_pin) {
    // manual mode resets the auto loop and reset states so that when we
    // go back into automatic mode, we start with a reset
    auto_loop_state = 0;
    reset_state = 0;
    
    manual();
    /*if(serial) {
      Serial.println("manual");
    }*/
  } else {
    auto_loop();
  }
}

void all_off() {
  // a helper which easily ensures that all output pins are low
  _digitalWrite(secondary_left,  LOW);
  _digitalWrite(secondary_right, LOW);
  _digitalWrite(primary_up,      LOW);
  _digitalWrite(primary_down,    LOW);
  _digitalWrite(shaker,          LOW);
}

unsigned long last_sensor_pressure = 0;
void semiautomatic_loop() {
  // this loop is semiautomatic in that pressing a button momentarily will
  // move the cylinder even after letting go of the button, until a magent 
  // sensor or pressure sensor is triggered.  This is no longer as useful as
  // it used to be since I no longer use magnetic sensors
  
  if(digitalRead(sensor_pressure)) {
    if(millis() - last_sensor_pressure < 500) {
      return;
    }
  }
  
  switch(semiautomatic_loop_state) {
    case 0:      
      if (!digitalRead(manual_left)) {
        Serial.println("left");
        _digitalWrite(secondary_left,  HIGH);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    LOW);    
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 1;
      } else if (!digitalRead(manual_right)) {
        Serial.println("right");
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, HIGH);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    LOW);    
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 2;
      } else if (!digitalRead(manual_up)) {
        Serial.println("up");
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      HIGH);
        _digitalWrite(primary_down,    LOW);
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 3;
      } else if(!digitalRead(manual_down)) {
        Serial.println("down");
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    HIGH);    
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 4;
      } else if(!digitalRead(manual_shaker)) {
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    LOW);    
        _digitalWrite(shaker,          LOW);
        //semiautomatic_loop_state = 5;
      } else {
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    LOW);
        _digitalWrite(shaker,          LOW);
      }
      break;
    case 1:
      //if(until_sensor(sensor_secondary) || digitalRead(sensor_pressure)) {
      if(digitalRead(sensor_pressure)) {
        semiautomatic_loop_state = 0;
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, HIGH);
        delay(50);
        _digitalWrite(secondary_right, LOW);
      }
      break;
    case 2:
      // if(until_sensor(sensor_secondary) || digitalRead(sensor_pressure)) {
      if(digitalRead(sensor_pressure)) {
        semiautomatic_loop_state = 0;
        _digitalWrite(secondary_right,  LOW);
        _digitalWrite(secondary_left, HIGH);
        delay(50);
        _digitalWrite(secondary_left, LOW);
      }
      break;
    case 3:
      if(until_sensor(sensor_primary)) {
        _digitalWrite(primary_up, LOW);
        semiautomatic_loop_state = 0;
      }
      break;
    case 4:
      if(until_sensor(sensor_primary)) {
        _digitalWrite(primary_down, LOW);
        semiautomatic_loop_state = 0;
      }
      break;
    case 5:
      break;
  }
  
  // too high pressure always go back to 0 off state
  if(digitalRead(sensor_pressure)) {
    all_off();
    semiautomatic_loop_state = 0;
    last_sensor_pressure = millis();
  }
}

// blinking helps know that the control board is powered
const unsigned int blink_speed = 30000;
unsigned int i = 0;
unsigned int shaker_state = 0;
unsigned int shaker_timer = 0;
unsigned int last_shaker_toggle_state = 0;
void manual_loop() {
  // this is an old manual loop which includes a toggle for the shaker
  
  _digitalWrite(primary_down,    !digitalRead(manual_down));
  _digitalWrite(primary_up,      !digitalRead(manual_up));
  _digitalWrite(secondary_left,  !digitalRead(manual_left));
  _digitalWrite(secondary_right, !digitalRead(manual_right));
    
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
      _digitalWrite(shaker, shaker_state);
    }
    
    last_shaker_toggle_state = current_shaker_toggle_state;
  } else {
    shaker_timer += 1;
  }
}

int delay_state = 0;
int delay_in_progress = 0;
unsigned long delay_start = 0;

int move_until_state = 0;
boolean move_until_until = false;
boolean move_until_pressure = false;
boolean move_until(int pin, fnptr until, int params, boolean pressure){
  // returns true if parent state machine should move on to the next state
  
  switch(move_until_state) {
    case 0:
      delay_state = 0;
      move_until_pressure = 0;
      move_until_until = 0;
      _digitalWrite(pin, HIGH);
      move_until_state += 1;
      return false;
    case 1:
      _digitalWrite(pin, HIGH);

      // these are stored so that the parent state machine can determine
      // if it was pressure or the until check that ended the cycle
      // see state 4 of auto_loop
      move_until_until = until(params);
      move_until_pressure = pressure && digitalRead(sensor_pressure);
      
      if(move_until_until || move_until_pressure) {
        _digitalWrite(pin, LOW);
        move_until_state += 1;
      }
      
      return false;
    case 2:
      _digitalWrite(pin, LOW);
      move_until_state += 1;

      // turn on 'opposite' direction very briefly to avoid stuck pressure
      // this currently only happens on far right secondary cylinder, but could
      // happen to any.  Perhaps there is a better fix here, its as if there
      // is still pressure in the cylinder even though the solenoid has 
      // switched off, potentially a problem with the solenoid?
      int other_pin;
      if(pin == primary_down) {
        other_pin = primary_up;
      } else if(pin == primary_up) {
        other_pin = primary_down;
      } else if(pin == secondary_left) {
        other_pin = secondary_right;
      } else if(pin == secondary_right) {
        other_pin = secondary_left;
      }

      _digitalWrite(other_pin, HIGH);
      delay(50);
      _digitalWrite(other_pin, LOW);

      // a small delay is necessary to give the pressure sensor a chance to 
      // reset.  Otherwise, the high pressure from this step will will be
      // mistaken for immediate high pressure in the next step.  This constant
      // might need to be changed for other sensors.
      delay(100);
      return false;
    default:
      move_until_state = 0;
      return true;
  }
}

int sensor_state = 0;
boolean until_switch(int sensor_pin) {
  // returns true iff a switch is pressed - pin should have a pullup resistor
  // with a switch to ground
  if(!digitalRead(sensor_pin)) {
    sensor_state = 0;
    return true;
  }
  return false;
}

const int threshold = 80;
boolean until_sensor(int sensor_pin) {
  // returns true iff the analog value on the sensor_pin goes low, then high
  // then low again.  This was used for the magnet sensors
  
  if(serial) {
    Serial.println("until sensor");
    Serial.println(sensor_state);
  }
  
  switch(sensor_state) {
    case 0:
      // wait for on (low)
      sensor_state += analogRead(sensor_pin) < threshold;
      return false;
    case 1 :
      // wait for off
      sensor_state += analogRead(sensor_pin) > threshold;
      return false;
    case 2 :
      // wait for on
      sensor_state += analogRead(sensor_pin) < threshold;
      return false;
    default :
      sensor_state = 0;
      return true;
  }
}

boolean _delay(int t) {
  // returns true if a delay of t milliseconds have completed since the first
  // call to _delay
  switch(delay_state) {
    case 0:
      delay_state += 1;
      delay_start = millis();
      return false;
    case 1:
      delay_state += (millis() - delay_start > t);
      return false;
    default :
      delay_state = 0;
      return true;
  }
}

boolean until_pressure_switch(int sensor_pin) {
  if(analogRead(sensor_pin) > 512) {
    sensor_state = 0;
    return true;
  }
  return false;
}
boolean delay_or_pressure(int t) {
    return _delay(t) || until_pressure_switch(sensor_pressure);
}
boolean never(int x) {
  // never returns true, and is passed in to move_until so that it relies only
  // on pressure to end the cycle
  return false;
}

unsigned long begin_right = 0;
unsigned long end_right = 1000;

boolean reset() {
  // reset press.
  // time the secondary cylinder travel from the left side to the
  // right side and then get ready to press a brick with whatever was
  // in the chamber prior to reset
  switch(reset_state) {
    case 0:
      all_off();
      reset_state += 1;
      break;
    case 1:
      reset_state += move_until(secondary_left, &never, 0, true);
      begin_right = millis();
      break;
    case 2:
      // for some reason this step was always getting skipped.  Cant figure
      // out why the pressure is detected so soon
      reset_state += move_until(secondary_right, &never, 0, true);
      end_right = millis();
      break;
    case 3:
      // end reset by preparing to compress a block with whatever was in the
      // chamber when the press was turned on
      auto_loop_state = 4;
      reset_state += 1;
      break;
    case 4:
      // done resetting
      return true;
  }
  return false;
}

const int shaker_on = HIGH;

void turn_shaker_on() {
  //if(digitalRead(manual_shaker)) {
  _digitalWrite(shaker, shaker_on);
  //}
}

void turn_shaker_off() {
  _digitalWrite(shaker, !shaker_on);
}

unsigned long loop_start = 0;
unsigned long now;
unsigned long begin_down = 0;
unsigned long end_down = 0;
double knob_primary_setting = 0;
double knob_secondary_setting = 0;
int jam_count = 0;
boolean is_pressure = false;
unsigned long overextend_delay = 100;
void auto_loop() {
  // This auto_loop is designed as a state machine which gives us something
  // like multithreading on the arduino, which allows us to implement a pause
  // button.
  // Every step will be considered complete if the pressure sensor
  // detects high pressure.

  /*if(serial) {
    Serial.print("auto_state");
    Serial.println(auto_loop_state);
    Serial.println(reset_state);
  }*/
  
  // wait for press to be moved to pre-loading position - secondary extended, 
  // primary extended
  if(!reset()) {
    return;
  }
  
  switch(auto_loop_state) {
    case 0:
      // turn off everything
      all_off();
      
      turn_shaker_on();
      auto_loop_state += 1;

      // if we make it all of the way through the loop in less than 100ms
      // there is probably something wrong
      // I have disabled this for now since it was causing more problems than
      // it was helping
      //now = millis();
      //if(now - loop_start < 100) {
      //  auto_loop_state = 99;
      //}
      //loop_start = now;
      
      //begin_down = millis();
      
      break;
    case 1:
      // lower primary cylinder to sensor
      turn_shaker_on();
      auto_loop_state += 1;
      break;
    case 2:
      turn_shaker_on();
      auto_loop_state += 1;
      break;
    case 3:
      turn_shaker_on();

      // knob_primary_setting = (end_right - begin_right) * 1.75;

      knob_primary_setting = 0.75 + analogRead(knob_secondary)/1024.0;
      knob_primary_setting = (end_right - begin_right) * knob_primary_setting;

      // I dont know why, but I was getting a small pressure spike right when
      // the cylinder started going down, so I changed it to only go off of timing
      // and ignore pressure, which doesn't seem ideal, but was the only quick
      // way to get it going.  It could be that a small delay or something like that
      // might help ...
      auto_loop_state += move_until(primary_down, &_delay, knob_primary_setting, false);
      break;
    case 4:
      turn_shaker_on();
      // retract secondary cylinder to center via sensor (to become ready for
      // compression)

      //_digitalWrite(shaker, shaker_on);

      // you might need to change 0.38 to something else depending on the exact
      // construction of your press.  It might be worth making this a knob
      // so that arduinos can be preloaded with the same firmware for
      // every press and changes in the field dont require firmware
      // upgrades
      knob_secondary_setting = 0.3 + 0.14*analogRead(knob_secondary)/1024.0;
      auto_loop_state += move_until(secondary_left, &_delay, (end_right - begin_right) * knob_secondary_setting, true);

      // see if there is_pressure or move_until_pressure can be
      // reliably used as a signal for a jam
      is_pressure = digitalRead(sensor_pressure);
      if(move_until_pressure) {
        auto_loop_state = 50;
        all_off();
      }
/*    
      // NOTE: it seems that this is sometimes detecting false positive
      //       jams ... not sure why yet ...
      // something to test would be:
      if(move_until_pressure) {
        // if we stopped because of pressure, it is most likely
        // because of a rock or some other unexpected obstruction in
        // the drawer shoot.  Move the drawer right to release the
        // pressure and try again.
        
        jam_count += 1;
        if(jam_count > 3) {
          jam_count = 0;
          all_off();
          auto_loop_state = 99;
        } else {
          auto_loop_state = 50;
        }
      }
      */
      break;
    case 5:
      turn_shaker_off();

      jam_count = 0;
      auto_loop_state += 1;
      break;
    case 6:
      turn_shaker_off();
      // compress block
      // extend primary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      // for now, just wait for pressure (might want simple delay here instead
      // if actually compressing the brick never gets to sensor pressure)
      auto_loop_state += move_until(primary_up, &never, 0, true);
      break;
    case 7:
      turn_shaker_off();
      // reduce pressure on block
      // slightly retract primary cylinder to ready ejection
      //_digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_down, &_delay, 100, true);
      break;
    case 8:
      turn_shaker_off();
      // allow brick to be ejected out of compression chamber
      // retract secondary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      auto_loop_state += move_until(secondary_left, &never, 0, true);
      break;
    case 9:
      turn_shaker_off();
      // eject block out of compression chamber
      // extend primary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_up, &never, 0, true);
      break;
    case 10:
      turn_shaker_off();
      // in the event that the primary cylinder extends too far up, retract
      // it down some to account for this
      // TODO: eventually this might have to be a knob (for those machines
      //       which have this problem
      if(overextend_delay == 0) {
        auto_loop_state += 1;
      } else {
        auto_loop_state += move_until(primary_down, &_delay, (end_right - begin_right) / 1000 * overextend_delay, false);
      }
      begin_right = millis();
      break;
    case 11:
      turn_shaker_off();
      // push block into queue and ready for loading new soil
      // extend secondary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      // NOTE: could technically start drawing the primary cylinder down
      // at this point too.  With all set points on timers though, we can't
      // do that since it would mess with calibration and unless it was on
      // a seperate hydraulic source, it wouldn't even save any time
      auto_loop_state += move_until(secondary_right, &never, 0, true);
      break;
    case 12:
      turn_shaker_off();
      end_right = millis();
      auto_loop_state = 0;
      break;
    case 50:
      turn_shaker_off();
      // this happens when a jam is detected in the shoot of the drawer
      if(move_until(secondary_right, &never, 0, true)) {
        // once we are back all of the way right, try going back to
        // center position
        auto_loop_state = 4;
        Serial.println("goto state 4");
        delay(250);
      }
      break;
    case 99:
      turn_shaker_off();
      // an error has occurred, do nothing forever (until power cycled)
      all_off();
      break;
    default:
      auto_loop_state = 0;
      return;
  }
}
