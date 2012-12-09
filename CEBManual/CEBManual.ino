// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your left
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

#include "CEBManual.h"

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

  Serial.begin(115200);
}

void loop() {
  // check_pause();
  
  // determine which mode we are in
  if(digitalRead(mode_pin)) {
    manual_loop();
  } else {
    auto_loop();
  }
}

void check_pause() {
  // check the pause pin to see if we should pause whats going on.
  // if we should pause, stay here in this function until we can unpause
  if(digitalRead(pause_pin)) {
    //Serial.println('paused');
    
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

// blinking helps know that the control board is powered
const unsigned int blink_speed = 30000;
unsigned int i = 0;
unsigned int shaker_state = 0;
unsigned int shaker_timer = 0;
unsigned int last_shaker_toggle_state = 0;
void manual_loop() {
  // blinker
  if(i < blink_speed) {
    digitalWrite(blink_pin, HIGH);
  } else {
    digitalWrite(blink_pin, LOW);
  }

  if(i >= blink_speed*2) {
    i = 0;
  } 
  i += 1;

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
      move_until_state += until(params);
      return false;
    case 2:
      digitalWrite(pin, LOW);
      move_until_state += 1;
      return false;
    default:
      move_until_state = 0;
      return true;
  }
}

const int threshold = 80;
int sensor_state = 0;
boolean until_sensor(int sensor_pin) {
  Serial.println("until sensor");
  Serial.println(sensor_state);
  
  // if the switch is pressed, 
  if(sensor_pin == sensor_primary) {
    if(!digitalRead(sensor_primary_switch)) {
      sensor_state = 0;
      return true;
    }
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

boolean delay_or_pressure(int t) {
    return _delay(t) || until_sensor(sensor_pressure);
}

int auto_loop_state = 0;
void auto_loop() {
  // wait for press to be moved to pre-loading position - secondary extended, 
  // primary extended
  // in order to support a pause button, save the step that we are on 
  Serial.println("auto_state");
  Serial.println(auto_loop_state);

  switch(auto_loop_state) {
    case 0:
      // turn on shaker
      digitalWrite(shaker, HIGH);
      auto_loop_state += 1;
      break;
    case 1:
      // lower primary cylinder to sensor
      digitalWrite(shaker, HIGH);
      auto_loop_state += move_until(primary_down, &until_sensor, sensor_primary);
      break;
    case 2:
      // wait for chamber to fill
      digitalWrite(shaker, HIGH);
      //delay(500);
      auto_loop_state += 1;
      break;
    case 3:
      // retract secondary cylinder to center via sensor
      digitalWrite(shaker, HIGH);
      auto_loop_state += move_until(secondary_left, &until_sensor, sensor_secondary);
      break;
    case 4:
      // turn off shaker
      digitalWrite(shaker, LOW);
      auto_loop_state += 1;
      break;
    case 5:
      // compress block
      // extend primary cylinder for time or to pressure
      digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_up,      &delay_or_pressure, 1500);
      break;
    case 6:
      // reduce pressure on block
      // slightly retract primary cylinder to ready ejection
      digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_down,    &_delay, 100);
      break;
    case 7:
      // allow brick to be ejected out of compression chamber
      // retract secondary cylinder for time or to pressure
      digitalWrite(shaker, LOW);
      auto_loop_state += move_until(secondary_right, &delay_or_pressure, 1500);
      break;
    case 8:
      // eject block out of compression chamber
      // extend primary cylinder for time or to pressure
      digitalWrite(shaker, LOW);
      auto_loop_state += move_until(primary_up,      &delay_or_pressure, 1500);
      break;
    case 9:
      // push block into queue and ready for loading new soil
      // extend secondary cylinder for time or to pressure
      digitalWrite(shaker, LOW);
      auto_loop_state += move_until(secondary_right, &delay_or_pressure, 2000);
      break;
    default:
      auto_loop_state = 0;
      return;
  }
}
