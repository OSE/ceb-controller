// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your left
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

// TODO: install and test primary cylinder sensor
// TODO: auger?

// TODO: reintroduce and test pause button
// TODO: replace delay_or_pressure with just pressure
// TODO: fix naming of pressure sensor
// TODO: combine movements in automatic code that can happen simultaneously (if
//   that still works with pressure sensor movement ...
//   this isn't super easy and will obfuscate the code somewhat
//   ALSO: steps 6&7 could potentially be combined
// TODO: remap pins 0 and 1 so that serial can be used while they are connected

#include "CEBManual.h"

// can't use serial line with pins 0 or 1 ...
const int serial = 1;

void _digitalWrite(int pin, int out) {
  if(invert_output) {
    digitalWrite(pin, !out);
  } else {
    digitalWrite(pin, out);
  }
}

void setup() {
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

unsigned int semiautomatic_loop_state = 0;
int auto_loop_state = 0;
int reset_state = 0;

void loop_manual() {
    manual();
}

void manual() {
    _digitalWrite(primary_up,      !digitalRead(manual_up));
    _digitalWrite(primary_down,    !digitalRead(manual_down));
    _digitalWrite(secondary_left,  !digitalRead(manual_left));
    _digitalWrite(secondary_right, !digitalRead(manual_right));
    _digitalWrite(shaker,          LOW);
}

void loop() {
  //manual();
  //sreturn;
  // check_pause();

  // determine which mode we are in
  if(digitalRead(mode_pin) ^ invert_mode_pin) {
    auto_loop_state = 0;
    reset_state = 0;
    
    manual();
    // semiautomatic_loop();
    if(serial) {
      Serial.println("manual");
    }
  } else {
    semiautomatic_loop_state = 0;
    
    auto_loop();
    if(serial) {
      //Serial.println("auto");
    }
  }
}

void check_pause() {
  // check the pause pin to see if we should pause whats going on.
  // if we should pause, stay here in this function until we can unpause
  if(digitalRead(pause_pin)) {
    if(serial) {
      //Serial.println('paused');
    }
    
    // save pinout state
    int primary_up_state      = digitalRead(primary_up);
    int primary_down_state    = digitalRead(primary_down);
    int secondary_left_state  = digitalRead(secondary_left_state);
    int secondary_right_state = digitalRead(secondary_right_state);
    int shaker_state          = digitalRead(shaker_state);
  
    while(digitalRead(pause_pin)) { }
    
    // restore pinout state
    _digitalWrite(primary_up,      primary_up_state);
    _digitalWrite(primary_down,    primary_down_state);
    _digitalWrite(secondary_left,  secondary_left_state);
    _digitalWrite(secondary_right, secondary_right_state);
    _digitalWrite(shaker,          shaker_state);
  }
}

void manual_control(int output_pin, int input_pin) {
  if(digitalRead(input_pin)) {
    _digitalWrite(output_pin, LOW);
  } else {
    _digitalWrite(output_pin, HIGH);
  }
}

void all_off() {
  _digitalWrite(secondary_left,  LOW);
  _digitalWrite(secondary_right, LOW);
  _digitalWrite(primary_up,      LOW);
  _digitalWrite(primary_down,    LOW);
  _digitalWrite(shaker,          LOW);
}

unsigned long last_sensor_pressure = 0;
void semiautomatic_loop() {
  if(serial) {
    //Serial.print("sensor_primary: ");
    //Serial.println(analogRead(sensor_primary));
    
    //Serial.print("sensor_secondary: ");
    //Serial.println(analogRead(sensor_secondary));
  }
  
  if(digitalRead(sensor_pressure)) {
    if(millis() - last_sensor_pressure < 500) {
      return;
    }
  }
  
  switch(semiautomatic_loop_state) {
    case 0:      
      if (!digitalRead(manual_left)) {
        _digitalWrite(secondary_left,  HIGH);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    LOW);    
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 1;
      } else if (!digitalRead(manual_right)) {
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, HIGH);
        _digitalWrite(primary_up,      LOW);
        _digitalWrite(primary_down,    LOW);    
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 2;
      } else if (!digitalRead(manual_up)) {
        _digitalWrite(secondary_left,  LOW);
        _digitalWrite(secondary_right, LOW);
        _digitalWrite(primary_up,      HIGH);
        _digitalWrite(primary_down,    LOW);
        _digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 3;
      } else if(!digitalRead(manual_down)) {
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
  if(serial) {
    /*Serial.print("down: ");
    Serial.println(digitalRead(manual_down));
  
    Serial.print("up: ");
    Serial.println(digitalRead(manual_up));
  
    Serial.print("left: ");
    Serial.println(digitalRead(manual_left));
  
    Serial.print("right: ");
    Serial.println(digitalRead(manual_right));
  
    Serial.print("shaker: ");
    Serial.println(digitalRead(manual_shaker));
    */
    
    //Serial.print("sensor_pressure: ");
    //Serial.println(digitalRead(sensor_pressure));
    /*
    Serial.print("sensor_pressure: ");
    Serial.println(digitalRead(sensor_pressure));*/
  }

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

int move_until_state = 0;
boolean move_until(int pin, fnptr until, int params, boolean pressure){
  // returns true if parent should move on to the next state
  
  switch(move_until_state) {
    case 0:
      _digitalWrite(pin, HIGH);
      move_until_state += 1;
      return false;
    case 1:
      _digitalWrite(pin, HIGH);
      if(until(params) || (pressure && digitalRead(sensor_pressure))) {
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
  if(!digitalRead(sensor_pin)) {
    sensor_state = 0;
    return true;
  }
  return false;
}

const int threshold = 80;
boolean until_sensor(int sensor_pin) {
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

int delay_state = 0;
int delay_in_progress = 0;
unsigned long delay_start = 0;
boolean _delay(int t) {
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
  return false;
}

unsigned long begin_right = 0;
unsigned long end_right = 1000;

boolean reset() {
  // reset machine to primary all of the way down, and secondary all the way left
  switch(reset_state) {
    case 0:
      all_off();
      reset_state = 1;
      delay(100);
    case 1:
      _digitalWrite(primary_down, HIGH);
      if(digitalRead(sensor_pressure)) {
        reset_state = 2;
        _digitalWrite(primary_down, LOW);
        
        // don't go all of the way up in case it holds the secondary drawer
        // stuck
        _digitalWrite(primary_up, HIGH);
        delay(100);
        _digitalWrite(primary_up, LOW);
        
        // wait for pressure to reset
        delay(100);
      }
      break;
    case 2:
      _digitalWrite(secondary_right, HIGH);
      if(digitalRead(sensor_pressure)) {
        reset_state = 3;
        _digitalWrite(secondary_right, LOW);
      }
      break;
    case 3:
      //begin_right = millis();
      reset_state = 4;
    case 4:
      _digitalWrite(secondary_left, HIGH);
      if(digitalRead(sensor_pressure)) {
        reset_state = 5;
        _digitalWrite(secondary_left, LOW);
      }
      break;
    case 5:
      end_right = millis();
      reset_state = 6;
    case 6:
      // done resetting
      return true;
  }
  return false;
}

const int shaker_on = LOW;
unsigned long loop_start = 0;
unsigned long now;
unsigned long begin_down = 0;
unsigned long end_down = 0;
double knob_primary_setting = 0;
void auto_loop() {
  // wait for press to be moved to pre-loading position - secondary extended, 
  // primary extended
  // in order to support a pause button, save the step that we are on 
  if(serial) {
    // Serial.println("auto_state");
    Serial.println(auto_loop_state);
  }
  
  if(!reset()) {
    return;
  }
  
  switch(auto_loop_state) {
    case 0:
      // turn off everything
      _digitalWrite(secondary_left,  LOW);
      _digitalWrite(secondary_right, LOW);
      _digitalWrite(primary_up,      LOW);
      _digitalWrite(primary_down,    LOW);
      
      // turn on shaker
      //_digitalWrite(shaker, shaker_on);
      // TODO: low or no delay here
      // delay(500);
      auto_loop_state += 1;

      // if we make it all of the way through the loop in less than 100ms
      // there is probably something wrong
      //now = millis();
      //if(now - loop_start < 100) {
      //  auto_loop_state = 99;
      //}
      //loop_start = now;
      
      begin_down = millis();
      
      break;
    case 1:
      // lower primary cylinder to sensor
      //_digitalWrite(shaker, shaker_on);
      // TODO : wait for primary magnet
      // can happen along side step 9, though only after a small delay ...
      auto_loop_state += move_until(primary_down, &never, 0, true);
      break;
    case 2:
      //auto_loop_state += _delay(1000);  
      end_down = millis();
      auto_loop_state += 1;
    case 3:
      //Serial.println(end_down - begin_down);
      //if(end_down - begin_down < 300) {
      //  auto_loop_state = 99;
      //  return;
      //}
      // with a sensor, we could do away with this extra time spent going
      // all of the way down, and then back up again.
      
      // knob_primary_setting = 0.39;
      
      // this will need tweaked depending on your knob.  This works for
      // log fade.  you could remove the pow to make this work well
      // on a normal linear fade.
      knob_primary_setting = pow(0.1 + analogRead(knob_primary) / 1700.0, 0.7);
      // Serial.println(knob_primary_setting);
      
      auto_loop_state += move_until(primary_up, &_delay, (end_down - begin_down) * knob_primary_setting, true);
      break;
    case 4:
      // retract secondary cylinder to center via sensor (to become ready for
      // compression)
      //_digitalWrite(shaker, shaker_on);
      //auto_loop_state += move_until(secondary_left, &until_sensor, sensor_secondary, true);
      auto_loop_state += move_until(secondary_left, &_delay, (end_right - begin_right) * 0.38, true);
      break;
    case 5:
      // turn off shaker
      //_digitalWrite(shaker, LOW);
      auto_loop_state += 1;
      break;
    case 6:
      // compress block
      // extend primary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      // for now, just wait for pressure (might want simple delay here instead
      // if actually compressing the brick never gets to sensor pressure)
      auto_loop_state += move_until(primary_up, &never, 0, true);
      //auto_loop_state += move_until(primary_up, &_delay, (end_down - begin_down) * 1.5, false);
      break;
    case 7:
      // reduce pressure on block
      // slightly retract primary cylinder to ready ejection
      //_digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_down, &_delay, 100, true);
      break;
    case 8:
      // allow brick to be ejected out of compression chamber
      // retract secondary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      auto_loop_state += move_until(secondary_left, &never, 0, true);
      break;
    case 9:
      // eject block out of compression chamber
      // extend primary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_up, &never, 0, true);
      begin_right = millis();
      break;
    case 10:
      // push block into queue and ready for loading new soil
      // extend secondary cylinder for time or to pressure
      //_digitalWrite(shaker, LOW);
      // NOTE: could technically start drawing the primary cylinder down
      // at this point too.  With all set points on timers though, we can't
      // do that.  I'm also not sure if it would actually save time since there
      // is only so many GPM the engine can deliver
      auto_loop_state += move_until(secondary_right, &never, 0, true);
      break;
    case 11:
      end_right = millis();
      //Serial.println(end_right - begin_right);
      //if(end_right - begin_right < 300) {
      //  auto_loop_state = 99;
      //  return;
      //}
      auto_loop_state += 1;
    case 99:
      // an error has occurred, do nothing
      all_off();
      break;
    default:
      auto_loop_state = 0;
      return;
  }
}
