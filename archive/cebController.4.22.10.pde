// -- Liberator Beta v2.0
//Notes: In every WHILE loop, shut off solenoids explicitly after
//escaping from the loop. Orientation is defined by machine user
//facing the control panel. 2 magnets (sensor actuators) are used
//for main cylinder, 4 magnets for soil drawer, and 2 magnets are
//are used for soil grate shaker. Copyright April 2010, Creative
//Commons CC-BY-SA-compatible OSE License, by Marcin Jakubowski,
//Ph.D., for OSE. This code and other documentation is available
//at the Open+Pario project management site under the CEB Project,
//under Documents - Liberator Beta 2.0 Control Code,
//http://openpario.net/projects/OSE?

int val;
int val2;
unsigned long startcounter;
unsigned long counter;
unsigned long startcounter2;
unsigned long counter2;

//0. Initialization.

void setup(){
  Serial.begin(9600);
  pinMode(19,INPUT);//Digital 19 is Analog 5.
  pinMode(18,INPUT);//Digital 18 is Analog 4.
  pinMode(3,OUTPUT);//Pins 3 and 5 are up and down motion,
  pinMode(6,OUTPUT);//respectively.
  pinMode(9,OUTPUT);//Pins 9 and 10 are right and left.
  pinMode(10,OUTPUT);
  counter=0;
  counter2=0;
  val=analogRead(5);//Read the sensor.\
  val2=analogRead(4);
  Serial.println("Main cylinder state:");
  Serial.println(val);
  Serial.println("Drawer cylinder state:");
  Serial.println(val2);

//1. Move big cylinder down all way to identify bottom point (+).

while (val<500){
  digitalWrite(3, LOW);//Move main cylinder down.
  digitalWrite(6, HIGH);//Control solenoids in pairs to keep track.
  val=analogRead(5);
  Serial.println("moving down");
  Serial.println(val);
//  delay(500);
}
  digitalWrite(3, LOW);//Turn solenoids off.
  digitalWrite(6, LOW);

Serial.println("Waiting for approval...");
//  delay(4000);

//2. Move drawer left - all way to brick ejection position.


  val2=analogRead(4);
  Serial.println("Drawer State reading:");
  Serial.println(val2);
//  Serial.println("Waiting 5 seconds for next step...");
//  delay(5000);


  while(val2 < 500){
    digitalWrite(9, LOW);//Move left.
    digitalWrite(10, HIGH);//Solenoids are controlled in pairs.
    val2=analogRead(4);
    Serial.println("Moving left");
    Serial.println(val2);
    //Need to keep reading state of sensor.
    };//stop upon reaching left (magnet at LEFT induces + state)

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  val2=analogRead(4);
  Serial.println("Finished left. Drawer state:");
  Serial.println(val2);
//    Serial.println("WAITING FOR APPROVAL...");
//  delay(4000);


//3. Calibrate main cylinder/eject.

  val=analogRead(5);
  startcounter=millis();

  while(val > 500){
    digitalWrite(3, HIGH);//Start upward motion.
    digitalWrite(6, LOW);
    Serial.println("Time during motion (ms):");
    Serial.println(millis()-startcounter);
    val=analogRead(5);

    };
    digitalWrite(3, LOW);//stop motion
    digitalWrite(6, LOW);
    counter=millis()-startcounter;
    Serial.println("Time after leaving loop:");
    Serial.println(counter);
    val=analogRead(5);
    Serial.println("Finished calibrations. Main cylinder state:");
    Serial.println(val);
    Serial.println("Waiting for approval.");
 //   delay(40000);

//4. Calibrate drawer
  val=analogRead(4);
  startcounter2=millis();

  while(val2 > 500){
    digitalWrite(9, HIGH);//Start upward motion.
    digitalWrite(10, LOW);
    Serial.println("Time during motion (ms):");
    Serial.println(millis()-startcounter2);
    val2=analogRead(4);

    };
//    digitalWrite(9, LOW);//stop motion
//    digitalWrite(10, LOW);
    counter2=millis()-startcounter2;
    Serial.println("Time after leaving loop:");
    Serial.println(counter2);
    val=analogRead(4);
    Serial.println("Finished calibration 2. Drawer cylinder state:");
    Serial.println(val2);

//5. Continue moving for the calibrated duration
//  digitalWrite(9, HIGH);
//  digitalWrite(10, LOW);
  delay(counter2);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
    Serial.println("Moved into soil loading position.");

}
//**************************** END OF INITIALIZATION

void loop(){

//8. Lower main cylinder.
  val=analogRead(5);
  Serial.println("Main cylinder state:");
  Serial.println(val);
  while(val < 500){;//Sensor is low at start.
    digitalWrite(3, LOW);//Move main cylinder down.
    digitalWrite(6, HIGH);//Control solenoids in pairs to keep track.
    val=analogRead(5);
    Serial.println("moving down");
    Serial.println(val);
    };//stop upon reaching bottom (magnet at bottom induces high state)
  digitalWrite(3,LOW);
  digitalWrite(6,LOW);

//9. Close compression chamber. Needs timing.

    digitalWrite(9, LOW);//Move left.
    digitalWrite(10, HIGH);//Solenoids are controlled in pairs.
        Serial.println("Moving left");
    delay(counter2*.84);//.75 is the exact factor

    digitalWrite(9, LOW);//Stopping by timing only.
    digitalWrite(10, LOW);

    Serial.println("Finished left. Drawer state:");
  Serial.println(val2);


//10. Begin pressing cycle with main cylinder, and then release
// pressure by moving 1/2 sec down.
  digitalWrite(3, HIGH);// Start cycle, and time motion.
  digitalWrite(6, LOW);// Start from bottom cylinder position.
  delay (counter/2);//Go up half way. Time counted in milliseconds.
  Serial.println("PRESSING...UP TO TIME OF (ms):");
  Serial.println(counter/2);
  digitalWrite(3, LOW);//  Stop motion.
  digitalWrite(6, LOW);
  val=analogRead(5);
  Serial.println("Main cylinder State:");
  Serial.println(val);

//******************************************************   RELEASE CYCLE
  digitalWrite(3, LOW);//Release, by time.
  digitalWrite(6, HIGH);//
  delay (counter/100);
  Serial.println("Releasing...");
  digitalWrite(3, LOW);//  Stop motion.
  digitalWrite(6, LOW);
  val=analogRead(5);
  Serial.println("State:");
  Serial.println(val);

//11. Open compression chamber by moving drawer left.

  val2=analogRead(4);
  Serial.println("Drawer State reading:");
  Serial.println(val2);
//  Serial.println("Waiting 5 seconds for next step...");
//  delay(5000);


  while(val2 < 500){
    digitalWrite(9, LOW);//Move left.
    digitalWrite(10, HIGH);//Solenoids are controlled in pairs.
    val2=analogRead(4);
    Serial.println("Moving left");
    Serial.println(val2);
    //Need to keep reading state of sensor.
    };//stop upon reaching left (magnet at LEFT induces + state)

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  val2=analogRead(4);
  Serial.println("Finished left. Drawer state:");
  Serial.println(val2);

//12. Push brick up.

  val=analogRead(5);
  while (val > 500){;//Complete motion up to sensor.
    digitalWrite(3, HIGH);//Complete up motion.
    digitalWrite(6, LOW);
    val=analogRead(5);
    Serial.println("Pushing brick up...");
    Serial.println("Main cylinder State:");
    Serial.println(val);
  }

  digitalWrite(3, LOW);
  digitalWrite(6, LOW);


//13. Eject brick from machine by moving drawer to the right.

  val=analogRead(4);
  startcounter2=millis();

  while(val2 > 500){
    digitalWrite(9, HIGH);//Start upward motion.
    digitalWrite(10, LOW);
    Serial.println("Time during motion (ms):");
    Serial.println(millis()-startcounter2);
    val2=analogRead(4);

    };
//    digitalWrite(9, LOW);//stop motion
//    digitalWrite(10, LOW);
    counter2=millis()-startcounter2;
    Serial.println("Time after leaving loop:");
    Serial.println(counter2);
    val=analogRead(4);
    Serial.println("Finished calibration 2. Drawer cylinder state:");
    Serial.println(val2);

//14. Continue moving for the calibrated duration (into loading pos.)
//  digitalWrite(9, HIGH);
//  digitalWrite(10, LOW);
  delay(counter2);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
    Serial.println("Moved into soil loading position.");
};


=Three Sensor Control Code=
==Testing==

===Secondary Cylinder===
This code is for the secondary (drawer) cylinder. This code may be used for calibration or troubleshooting purposes. This code zeroes the drawer cylinder by moving it to the far left (direction is determined by user facing the machine control panel). This code cycles between the 3 positions. You may use this code for determining positioning replicability at various speeds of the machine.

Correct zeroing occurs in most cases. If the machine was turned off in the far-right-position, this code does not work properly. This is an artifact of the control mechanism, and cannot be avoided easily. The user needs to reset the machine if this happens. To determine if the machine is zeroed properly, observe the position of the drawer during the initial 5 second rest (after zeroing occurred). If the machine did not reset properly, reset it manually via the reset button on the Arduino shield or by cycling the power to the controller.

Beginning of code follows.
-----

//Notes: In every WHILE loop, shut off solenoids explicitly after
//escaping from the loop. Orientation is defined by machine user
//facing the control panel. 3 magnets (sensor actuators) are used
//for main cylinder, 3 magnets for soil drawer, and 2 magnets are
//are used for soil grate shaker. Copyright April 2010, Creative
//Commons CC-BY-SA-compatible Open Source Ecology (OSE) License,
//by Marcin Jakubowski, Ph.D., for OSE. This code and other
//documentation is available at the Open+Pario project management
//site under the CEB Project, under Documents - Liberator Beta 2.0
//Control Code, http://openpario.net/projects/OSE

int val;
int val2;
int val3;

unsigned long startcounter;
unsigned long counter;
unsigned long startcounter2;
unsigned long counter2;

//0. Initialization.

void setup(){
  Serial.begin(9600);
  pinMode(19,INPUT);//Digital 19 is Analog 5.
  pinMode(18,INPUT);//Digital 18 is Analog 4.
  pinMode(17,INPUT);//Digital 17 is Analog 3.
  pinMode(3,OUTPUT);//Pins 3 and 6 are up and down motion,
  pinMode(6,OUTPUT);//respectively.
  pinMode(9,OUTPUT);//Pins 9 and 10 are right and left,
  pinMode(10,OUTPUT);//respectively.
  pinMode(11,OUTPUT);//Hopper shaker motor.
  counter=0;
  counter2=0;
  val=analogRead(5);//Read the sensor.\
  val2=analogRead(4);
  Serial.println("Main cylinder state:");
  Serial.println(val);
  Serial.println("Drawer cylinder state:");
  Serial.println(val2);

//1. Move sec cylinder Left 1/2 sec
digitalWrite(10,HIGH);
delay(400);
digitalWrite(10,LOW);
//2. If +, then stop, else keep moving left until +.
while (analogRead(4) < 500){
  digitalWrite(10, HIGH);
}
digitalWrite(10, LOW);
//3. Wait 6 seconds. If stopped and not zeroed, turn controller on and off.
delay(6000);
}
//**************************** END OF INITIALIZATION

void loop(){

//L1. Move right to -
while(analogRead(4) > 500){
  digitalWrite(9, HIGH);//Move right.
  }
//L2. Continue moving right to +. Wait a second.
while(analogRead(4) < 500){
}
digitalWrite(9, LOW);
delay(1000);
//L3. Move left till -. Wait a second.
digitalWrite(10, HIGH);
while (analogRead(4) >500){
  }
digitalWrite(10, LOW);
delay(1000);
//L4. Move left till +. Wait a second.
digitalWrite(10, HIGH);
while (analogRead(4) < 500){
  }
digitalWrite(10, LOW);
delay(1000);
}

