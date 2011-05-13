// Notes: Orientation is defined by machine user
// facing the control panel. 3 magnets (sensor actuators) are used
// for main cylinder, 3 magnets for soil drawer, and 2 magnets are
// are used for soil grate shaker. Copyright April 2010, Creative
// Commons CC-BY-SA-compatible Open Source Ecology (OSE) License,
// by Marcin Jakubowski, Ph.D., for OSE. This code and other
// documentation is available at the Open+Pario project management
// site under the CEB Project, under Documents - Liberator Beta 2.0
// Control Code, http://openpario.net/projects/OSE

// Complete code including simultaneous motion of 2 cylinders:


// #define directives for pins
// -----------------------------------------------------
// Analog Inputs
#define mainCylinder  5
#define secCylinder   4
// Digital Outputs
#define UP      3
#define DOWN    6
#define LEFT    10
#define RIGHT   9
#define HOPPER  11


int main_val;    // main cylinder
int sec_val;     // secondary cylinder
int shaker_val;  // shaker
int main_toggle; // main cylinder toggle
int sec_toggle;  // secondary cylinder toggle
unsigned long start_time;
unsigned long motion_time;

//                   Initialization
// -----------------------------------------------------

void setup(){
  Serial.begin(9600);

  pinMode(mainCylinder + 14, INPUT); // Digital input channels are analog + 14.
  pinMode(secCylinder  + 14, INPUT);
  pinMode(17,     INPUT); // Digital 17 is Analog 3.
  pinMode(UP,     OUTPUT);// Pins 3 and 6 are up and down motion,
  pinMode(DOWN,   OUTPUT);// respectively.
  pinMode(RIGHT,  OUTPUT);// Pins 9 and 10 are right and left,
  pinMode(LEFT,   OUTPUT);// respectively.
  pinMode(HOPPER, OUTPUT);// Hopper shaker motor.
  motion_time  = 0;

  // Read the sensors & print to serial console.
  main_val = analogRead(mainCylinder);
  sec_val  = analogRead(secCylinder);
  Serial.println("Main cylinder state:");
  Serial.println(main_val);
  Serial.println("Drawer cylinder state:");
  Serial.println(sec_val);

  // 1. Move main cylinder down 1/2 sec.
  digitalWrite(DOWN, HIGH);
  delay(500);
  digitalWrite(DOWN, LOW);

  // 2. Move secondary cylinder left 1/2 sec.
  digitalWrite(LEFT, HIGH);
  delay(500);
  digitalWrite(LEFT, LOW);

  // 3. Zero main cylinder.
  while (analogRead(mainCylinder) < 500){
    digitalWrite(DOWN, HIGH);
  }
  digitalWrite(DOWN, LOW);

  // 4. Zero secondary cylinder.
  while(analogRead(secCylinder) < 500){
    digitalWrite(LEFT, HIGH);
  }
  digitalWrite(LEFT, LOW);

  // 5. Eject soil from chamber with main cylinder. (Go up to mid,
  // then go up to top.)
  while (analogRead(mainCylinder) > 500){
    digitalWrite(UP,  HIGH);
  }
  while (analogRead(mainCylinder) < 500){
  }
  digitalWrite(UP,  LOW);

  // 6. Push soil out of way with secondary cylinder by moving right
  // and stop in closed compression chamber position.
  while(analogRead(secCylinder) > 500){
    digitalWrite(RIGHT,  HIGH);
  }
  digitalWrite(RIGHT,  LOW);

  // 7. Initialize secondary cylinder.
  while(analogRead(secCylinder) < 500){
    digitalWrite(LEFT,  HIGH);
  }
  digitalWrite(LEFT,  LOW);

  // 8. Calibrate main cylinder (move from top to bottom and measure T in 2 stages)
  // Move down to -,  measuring down motion time T1
  start_time = millis();
  digitalWrite(DOWN,  HIGH);
  while (analogRead(mainCylinder) > 500){}
  motion_time += millis() - start_time;

  // 9. Move down to +,  measuring down motion time T2
  start_time = millis();
  while (analogRead(mainCylinder) < 500){}
  motion_time += millis() - start_time;
  digitalWrite(DOWN,  LOW);

  // 10. Move main cylinder to terminal position.
  while (analogRead(mainCylinder) > 500){
    digitalWrite(UP,  HIGH);
  }
  while (analogRead(mainCylinder) < 500){}
  digitalWrite(UP,  LOW);

  // 11. Wait 5 seconds. If stopped and not zeroed,  cycle controller.
  delay(5000);

  // At this point,  secondary cylinder is in initial position and main
  // cylinder is in terminal position.
}


//                       Main Loop
// -----------------------------------------------------

void loop(){
  digitalWrite(RIGHT,  HIGH);
  delay(200);
  digitalWrite(DOWN,  HIGH);
  main_toggle = 0;
  sec_toggle = 0;

  // C2 motion from left to middle.
  while(analogRead(secCylinder) > 500){
    if (analogRead(mainCylinder) < 500){
      main_toggle = 1;
    }
    if (main_toggle = 1 && analogRead(mainCylinder) > 500){
      digitalWrite(DOWN,  LOW);
    }
  }

  // C2 motion from middle to right.
  while(analogRead(secCylinder) < 500){
    if (analogRead(mainCylinder) < 500){
      main_toggle = 1;
    }
    if (main_toggle = 1 && analogRead(mainCylinder) > 500){
      digitalWrite(DOWN,  LOW);
    }
  }

  // Stop C2.
  digitalWrite(RIGHT,  LOW);

  // Complete down motion of C1. This allows for continuation of
  // motion. This works if motion is completed already.
  while(true){
    if (analogRead(mainCylinder) < 500){
      main_toggle = 1;
    }
    if (analogRead(mainCylinder) > 500 && main_toggle == 1){
      break;
    }
  }

  digitalWrite(DOWN,  LOW);// Stops C1.
  delay(200); // Last nuggets drop. Soil loading cycle complete.

  // Close drawer by moving C2 left.
  while(analogRead(secCylinder) > 500){
    digitalWrite(LEFT,  HIGH);
  }
  digitalWrite(LEFT,  LOW);

  // Press.
  while (analogRead(mainCylinder) > 500){
    digitalWrite(UP,  HIGH);
  }
  digitalWrite(UP,  LOW);

  // Release.
  digitalWrite(DOWN,  HIGH);
  delay(motion_time / 32);
  digitalWrite(DOWN,  LOW);

  // Open drawer by moving C2 left.
  while(analogRead(secCylinder) < 500){
    digitalWrite(LEFT,  HIGH);
  }
  digitalWrite(LEFT,  LOW);

  // Eject.
  while (analogRead(mainCylinder) < 500){
    digitalWrite(UP,  HIGH);
  }
  digitalWrite(UP,  LOW);
}

