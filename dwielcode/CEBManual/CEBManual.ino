// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your left
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

// TODO: combine movements in automatic code that can happen simultaneously (if
//   that still works with pressure sensor movement ...
// TODO: reduce time between high pressure sensed and pressure released
// TODO: detect constant high pressure or high pressure too soon

#include "CEBManual.h"

// can't use serial line with pins 0 or 1 ...
const int serial = 0;

void setup() {
  pinMode(primary_up,      OUTPUT);
  pinMode(primary_down,    OUTPUT);
  pinMode(secondary_left,  OUTPUT);
  pinMode(secondary_right, OUTPUT);
  pinMode(shaker,          OUTPUT);

  digitalWrite(primary_up,      LOW);
  digitalWrite(primary_down,    LOW);
  digitalWrite(secondary_left,  LOW);
  digitalWrite(secondary_right, LOW);
  digitalWrite(shaker,          LOW);
  
  pinMode(manual_down,   INPUT_PULLUP);
  pinMode(manual_up,     INPUT_PULLUP);
  pinMode(manual_left,   INPUT_PULLUP);
  pinMode(manual_right,  INPUT_PULLUP);
  pinMode(manual_shaker, INPUT_PULLUP);
  
  pinMode(pause_pin,     INPUT_PULLUP);
  pinMode(mode_pin,      INPUT_PULLUP);
  pinMode(sensor_primary_switch, INPUT_PULLUP);

  pinMode(sensor_primary,   INPUT);
  pinMode(sensor_secondary, INPUT);
  pinMode(sensor_pressure,  INPUT);

  if(serial) {
    Serial.begin(115200);
  }
}

void loop() {
  // check_pause();
  
  // determine which mode we are in
  if(digitalRead(mode_pin) ^ invert_mode_pin) {
    auto_loop_state = 0;
    reset_state = 0;
    
    // manual_loop();
    semiautomatic_loop();
    if(serial) {
      //Serial.println("manual");
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
    digitalWrite(primary_up,      primary_up_state);
    digitalWrite(primary_down,    primary_down_state);
    digitalWrite(secondary_left,  secondary_left_state);
    digitalWrite(secondary_right, secondary_right_state);
    digitalWrite(shaker,          shaker_state);
  }
}

void manual_control(int output_pin, int input_pin) {
  if(digitalRead(input_pin)) {
    digitalWrite(output_pin, LOW);
  } else {
    digitalWrite(output_pin, HIGH);
  }
}

void all_off() {
  digitalWrite(secondary_left,  LOW);
  digitalWrite(secondary_right, LOW);
  digitalWrite(primary_up,      LOW);
  digitalWrite(primary_down,    LOW);
  digitalWrite(shaker,          LOW);
}

unsigned int semiautomatic_loop_state = 0;
void semiautomatic_loop() {
  if(serial) {
    Serial.print("sensor_primary: ");
    Serial.println(analogRead(sensor_primary));
    
    Serial.print("sensor_secondary: ");
    Serial.println(analogRead(sensor_secondary));
  }
  
  switch(semiautomatic_loop_state) {
    case 0:      
      if (!digitalRead(manual_left)) {
        digitalWrite(secondary_left,  HIGH);
        digitalWrite(secondary_right, LOW);
        digitalWrite(primary_up,      LOW);
        digitalWrite(primary_down,    LOW);    
        digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 1;
      } else if (!digitalRead(manual_right)) {
        digitalWrite(secondary_left,  LOW);
        digitalWrite(secondary_right, HIGH);
        digitalWrite(primary_up,      LOW);
        digitalWrite(primary_down,    LOW);    
        digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 2;
      } else if (!digitalRead(manual_up)) {
        digitalWrite(secondary_left,  LOW);
        digitalWrite(secondary_right, LOW);
        digitalWrite(primary_up,      HIGH);
        digitalWrite(primary_down,    LOW);
        digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 3;
      } else if(!digitalRead(manual_down)) {
        digitalWrite(secondary_left,  LOW);
        digitalWrite(secondary_right, LOW);
        digitalWrite(primary_up,      LOW);
        digitalWrite(primary_down,    HIGH);    
        digitalWrite(shaker,          LOW);
        semiautomatic_loop_state = 4;
      } else if(!digitalRead(manual_shaker)) {
        digitalWrite(secondary_left,  LOW);
        digitalWrite(secondary_right, LOW);
        digitalWrite(primary_up,      LOW);
        digitalWrite(primary_down,    LOW);    
        digitalWrite(shaker,          HIGH);
        semiautomatic_loop_state = 5;
      } else {
        digitalWrite(secondary_left,  LOW);
        digitalWrite(secondary_right, LOW);
        digitalWrite(primary_up,      LOW);
        digitalWrite(primary_down,    LOW);
        digitalWrite(shaker,          LOW);
      }
      break;
    case 1:
      if(until_sensor(sensor_secondary) || digitalRead(sensor_primary_switch)) {
        semiautomatic_loop_state = 0;
        digitalWrite(secondary_left,  LOW);
        digitalWrite(secondary_right, HIGH);
        delay(50);
        digitalWrite(secondary_right, LOW);
      }
      break;
    case 2:
      if(until_sensor(sensor_secondary) || digitalRead(sensor_primary_switch)) {
        semiautomatic_loop_state = 0;
        digitalWrite(secondary_right,  LOW);
        digitalWrite(secondary_left, HIGH);
        delay(50);
        digitalWrite(secondary_left, LOW);
      }
      break;
    case 3:
      if(until_sensor(sensor_primary)) {
        digitalWrite(primary_up, LOW);
        semiautomatic_loop_state = 0;
      }
      break;
    case 4:
      if(until_sensor(sensor_primary)) {
        digitalWrite(primary_down, LOW);
        semiautomatic_loop_state = 0;
      }
      break;
    case 5:
      break;
  }
  
  // too high pressure always go back to 0 off state
  if(digitalRead(sensor_primary_switch)) {
    all_off();
    semiautomatic_loop_state = 0;
  }
}

// blinking helps know that the control board is powered
const unsigned int blink_speed = 30000;
unsigned int i = 0;
unsigned int shaker_state = 0;
unsigned int shaker_timer = 0;
unsigned int last_shaker_toggle_state = 0;
void manual_loop() {
  // blinker
  //if(i < blink_speed) {
  //  digitalWrite(blink_pin, HIGH);
  //} else {
  //  digitalWrite(blink_pin, LOW);
  //}
  //
  //if(i >= blink_speed*2) {
  //  i = 0;
  //} 
  //i += 1;

  /*
  // secondary
  tmp = analogRead(sensor_secondary);
  if(tmp < 10) {
    //digitalWrite(11, HIGH);
    digitalWrite(secondary_right, HIGH);
  } else {
    //digitalWrite(11, LOW);
    digitalWrite(secondary_right, LOW);
  }
  */
  if(until_pressure_switch(sensor_pressure)) {
    digitalWrite(blink_pin, HIGH);
  } else {
    digitalWrite(blink_pin, LOW);
  }
  
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
    
    //Serial.print("sensor_primary_switch: ");
    //Serial.println(digitalRead(sensor_primary_switch));
    /*
    Serial.print("sensor_pressure: ");
    Serial.println(digitalRead(sensor_pressure));*/
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
}

int move_until_state = 0;
boolean move_until(int pin, fnptr until, int params){
  // returns true if parent should move on to the next state
  
  switch(move_until_state) {
    case 0:
      digitalWrite(pin, HIGH);
      move_until_state += 1;
      return false;
    case 1:
      digitalWrite(pin, HIGH);
      if(until(params) || digitalRead(sensor_primary_switch)) {
        digitalWrite(pin, LOW);
        move_until_state += 1;
      }
      return false;
    case 2:
      digitalWrite(pin, LOW);
      move_until_state += 1;

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

      digitalWrite(other_pin, HIGH);
      delay(50);
      digitalWrite(other_pin, LOW);

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

int reset_state = 0;
boolean reset() {
  digitalWrite(blink_pin, digitalRead(sensor_primary_switch));
  
  // reset machine to primary all of the way up, and secondary all the way left
  switch(reset_state) {
    case 0:
      all_off();
      reset_state = 1;
      delay(1000);
    case 1:
      digitalWrite(primary_up, HIGH);
      if(digitalRead(sensor_primary_switch)) {
        reset_state = 2;
        digitalWrite(primary_up, LOW);
        
        // don't go all of the way up in case it holds the secondary drawer stuck
        digitalWrite(primary_down, HIGH);
        delay(100);
        digitalWrite(primary_down, LOW);
        
        // wait for pressure to reset
        delay(1000);
      }
      break;
    case 2:
      digitalWrite(secondary_right, HIGH);
      if(digitalRead(sensor_primary_switch)) {
        reset_state = 3;
        digitalWrite(secondary_right, LOW);
      }
      break;
    case 3:
      // done resetting
      return true;
  }
  return false;
}

const int shaker_on= LOW;
int auto_loop_state = 0;
void auto_loop() {
  // wait for press to be moved to pre-loading position - secondary extended, 
  // primary extended
  // in order to support a pause button, save the step that we are on 
  if(serial) {
    Serial.println("auto_state");
    Serial.println(auto_loop_state);
  }
  
  // TODO: move cylinders into position with pressure sensor as
  // reset
  if(!reset()) {
    return;
  }
  
  switch(auto_loop_state) {
    case 0:
      // turn off everything
      digitalWrite(secondary_left,  LOW);
      digitalWrite(secondary_right, LOW);
      digitalWrite(primary_up,      LOW);
      digitalWrite(primary_down,    LOW);
      
      // turn on shaker
      //digitalWrite(shaker, shaker_on);
      // TODO: low or no delay here
      delay(500);
      auto_loop_state += 1;
      break;
    case 1:
      // lower primary cylinder to sensor
      //digitalWrite(shaker, shaker_on);
      // TODO : wait for primary magnet
      // can happen along side step 2&3
      auto_loop_state += move_until(primary_down, &never, 0);
      break;
    case 2:
      // start shaker for fill
      //digitalWrite(shaker, shaker_on);
      //delay(500);
      auto_loop_state += 1;
      break;
    case 3:
      // retract secondary cylinder to center via sensor
      //digitalWrite(shaker, shaker_on);
      auto_loop_state += move_until(secondary_left, &until_sensor, sensor_secondary);
      break;
    case 4:
      // turn off shaker
      //digitalWrite(shaker, LOW);
      auto_loop_state += 1;
      break;
    case 5:
      // compress block
      // extend primary cylinder for time or to pressure
      //digitalWrite(shaker, LOW);
      // auto_loop_state += move_until(primary_up,      &delay_or_pressure, 1500);
      // for now, just wait for pressure (might want simple delay here instead)
      auto_loop_state += move_until(primary_up, &never, 0);
      break;
    case 6:
      // reduce pressure on block
      // slightly retract primary cylinder to ready ejection
      //digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_down, &_delay, 100);
      break;
    case 7:
      // allow brick to be ejected out of compression chamber
      // retract secondary cylinder for time or to pressure
      //digitalWrite(shaker, LOW);
      //auto_loop_state += move_until(secondary_left, &delay_or_pressure, 1500);
      auto_loop_state += move_until(secondary_left, &never, 0);
      break;
    case 8:
      // eject block out of compression chamber
      // extend primary cylinder for time or to pressure
      //digitalWrite(shaker, LOW);
      // auto_loop_state += move_until(primary_up,      &delay_or_pressure, 1500);
      auto_loop_state += move_until(primary_up, &never, 0);
      break;
    case 9:
      // push block into queue and ready for loading new soil
      // extend secondary cylinder for time or to pressure
      //digitalWrite(shaker, LOW);
      // auto_loop_state += move_until(secondary_right, &delay_or_pressure, 2000);
      auto_loop_state += move_until(secondary_right, &never, 0);
      break;
    default:
      auto_loop_state = 0;
      return;
  }
}
