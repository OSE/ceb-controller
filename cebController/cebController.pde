// Notes: Orientation is defined by machine user
// facing the control panel. 3 magnets (sensor actuators) are used
// for main cylinder, 3 magnets for soil drawer, and 2 magnets are
// are used for soil grate shaker. Copyright April 2010, Creative
// Commons CC-BY-SA-compatible Open Source Ecology (OSE) License,
// by Marcin Jakubowski, Ph.D., for OSE. This code and other
// documentation is available at the Open+Pario project management
// site under the CEB Project, under Documents - Liberator Beta 2.0
// Control Code, http:// openpario.net/projects/OSE

// Complete code including simultaneous motion of 2 cylinders:

int val; // main cylinder
int val2; // secondary cylinder
int val3; // shaker
int toggle1; // main cylinder toggle
int toggle2; // secondary cylinder toggle
unsigned long startcounter1;
unsigned long counter1;
unsigned long startcounter2;
unsigned long counter2;
unsigned long counter;

//                   Initialization
// -----------------------------------------------------

void setup(){
  Serial.begin(9600);
  pinMode(19,INPUT);// Digital 19 is Analog 5.
  pinMode(18,INPUT);// Digital 18 is Analog 4.
  pinMode(17,INPUT);// Digital 17 is Analog 3.
  pinMode(3,OUTPUT);// Pins 3 and 6 are up and down motion,
  pinMode(6,OUTPUT);// respectively.
  pinMode(9,OUTPUT);// Pins 9 and 10 are right and left,
  pinMode(10,OUTPUT);// respectively.
  pinMode(11,OUTPUT);// Hopper shaker motor.
  counter1=0;
  counter2=0;
  counter=0;
  val=analogRead(5);// Read the sensor.
  val2=analogRead(4);
  Serial.println("Main cylinder state:");
  Serial.println(val);
  Serial.println("Drawer cylinder state:");
  Serial.println(val2);

  // 3=up
  // 6=down
  // 9=right
  // 10=left
  // analog5=main cylinder
  // analog4=secondary cylinder

  // 1. Move main cylinder down 1/2 sec.
  digitalWrite(6,HIGH);
  delay(500);
  digitalWrite(6,LOW);

  // 2. Move secondary cylinder left 1/2 sec.
  digitalWrite(10,HIGH);
  delay(500);
  digitalWrite(10,LOW);

  // 3. Zero main cylinder.
  while (analogRead(5) < 500){
    digitalWrite(6, HIGH);
  }
  digitalWrite(6, LOW);

  // 4. Zero secondary cylinder.
  while(analogRead(4) < 500){
    digitalWrite(10, HIGH);
  }
  digitalWrite(10, LOW);

  // 5. Eject soil from chamber with main cylinder. (Go up to mid,
  // then go up to top.)
  while (analogRead(5) > 500){
    digitalWrite(3, HIGH);
  }
  while (analogRead(5) < 500){
  }
  digitalWrite(3, LOW);

  // 6. Push soil out of way with secondary cylinder by moving right
  // and stop in closed compression chamber position.
  while(analogRead(4)>500){
    digitalWrite(9, HIGH);
  }
  digitalWrite(9, LOW);

  // 7. Initialize secondary cylinder.
  while(analogRead(4)<500){
    digitalWrite(10, HIGH);
  }
  digitalWrite(10, LOW);

  // 8. Calibrate main cylinder (move from top to bottom and measure t)
  // Move down to -, measuring down motion time T1
  startcounter1=millis();
  digitalWrite(6, HIGH);
  while (analogRead(5)>500){}
  counter1=millis()-startcounter1;

  // 9. Move down to +, measuring T2
  startcounter2=millis();
  while (analogRead(5)<500){}
  counter2=millis()-startcounter2;
  digitalWrite(6, LOW);

  // 10. Add T1 and T2 to get total time of down travel.
  counter=counter1+counter2;

  // 11. Move main cylinder to terminal position.
  while (analogRead(5)>500){
    digitalWrite(3, HIGH);
  }
  while (analogRead(5)<500){}
  digitalWrite(3, LOW);

  // 12. Wait 5 seconds. If stopped and not zeroed, cycle controller.
  delay(5000);

  // At this point, secondary cylinder is in initial position and main
  // cylinder is in terminal position.
}


//                       Main Loop
// -----------------------------------------------------

void loop(){
  digitalWrite(9, HIGH);
  delay(200);
  digitalWrite(6, HIGH);
  toggle1=0;
  toggle2=0;

  // C2 motion from left to middle.
  while(analogRead(4)>500){
    if (analogRead(5)<500){
      toggle1=1;
    }
    if (toggle1=1 && analogRead(5)>500){
      digitalWrite(6, LOW);
    }
  }

  // C2 motion from middle to right.
  while(analogRead(4)<500){
    if (analogRead(5)<500){
      toggle1=1;
    }
    if (toggle1=1 && analogRead(5)>500){
      digitalWrite(6, LOW);
    }
  }

  // Stop C2.
  digitalWrite(9, LOW);

  // Complete down motion of C1. This allows for continuation of
  // motion. This works if motion is completed already.
  for (int x=0; x<1 ; x=x+0){
    if (analogRead(5)<500){
      toggle1=1;
    }
    if (analogRead(5)>500 && toggle1==1){
      break;
    }
    }
  digitalWrite(6, LOW);// Stops C1.
  delay(200);// Last nuggets drop. Soil loading cycle complete.

  // Close drawer by moving C2 left.
  while(analogRead(4)>500){
    digitalWrite(10, HIGH);
  }
  digitalWrite(10, LOW);

  // Press.
  while (analogRead(5)>500){
    digitalWrite(3, HIGH);
  }
  digitalWrite(3, LOW);

  // Release.
  digitalWrite(6, HIGH);
  delay(counter/32);
  digitalWrite(6, LOW);

  // Open drawer by moving C2 left.
  while(analogRead(4)<500){
    digitalWrite(10, HIGH);
  }
  digitalWrite(10, LOW);

  // Eject.
  while (analogRead(5)<500){
    digitalWrite(3, HIGH);
  }
  digitalWrite(3, LOW);
}

