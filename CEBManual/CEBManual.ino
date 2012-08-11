// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your left
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

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
const int manual_down  = 18;
const int manual_up    = 19;
const int manual_left  = 20;
const int manual_right = 21;
// const int manual_shaker = xxx;

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor_primary,   INPUT);
  pinMode(sensor_secondary, INPUT);

  pinMode(primary_down,    OUTPUT);
  pinMode(primary_up,      OUTPUT);
  pinMode(secondary_left,  OUTPUT);
  pinMode(secondary_right, OUTPUT);
  pinMode(shaker,          OUTPUT);
  
  pinMode(manual_down,  INPUT_PULLUP);
  pinMode(manual_up,    INPUT_PULLUP);
  pinMode(manual_left,  INPUT_PULLUP);
  pinMode(manual_right, INPUT_PULLUP);
    
  Serial.println("Initialization Complete");
}

void manual_control(int output_pin, int input_pin) {
  if(digitalRead(input_pin)) {
    digitalWrite(output_pin, LOW);
  } else {
    digitalWrite(output_pin, HIGH);
  }
}

const unsigned int blink_speed = 30000;
void loop() {
  unsigned int i = 0;
  while(1) {
    if(i < blink_speed) {
      digitalWrite(11, HIGH);
    } else {
      digitalWrite(11, LOW);
    }
    
    if(i >= blink_speed*2) {
      i = 0;
    } 
    i += 1;
    
    digitalWrite(primary_down,    !digitalRead(manual_down));
    digitalWrite(primary_up,      !digitalRead(manual_up));
    digitalWrite(secondary_left,  !digitalRead(manual_left));
    digitalWrite(secondary_right, !digitalRead(manual_right));
  }
}

