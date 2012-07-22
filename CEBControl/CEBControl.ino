// Creative Commons CC-BY-SA-compatible Open Source Ecology (OSE) License
// Orientation: when you face the machine, the ejection side is to your left
// Code for initialization and brick production loop with motion of the
//   primary cylinder, secondary cylinder, and shaker motor

int primary_toggle;

int releasefactor;
unsigned long startcounter1;
unsigned long counter1;
unsigned long startcounter2;
unsigned long counter2;
unsigned long counterdown;
unsigned long counterup;
float timingratio;
long compressiontime;
long releasetime;

const int sensor_primary = 14;
const int sensor_secondary = 15;
const int primary_down = 3;
const int primary_up = 6;
const int secondary_left = 9;
const int secondary_right = 10;
const int shaker = 11;

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor_primary, INPUT);
  pinMode(sensor_secondary, INPUT);

  pinMode(primary_down, OUTPUT);
  pinMode(primary_up, OUTPUT);
  pinMode(secondary_left, OUTPUT);
  pinMode(secondary_right, OUTPUT);
  pinMode(shaker, OUTPUT);
  
  counter1    = 0;
  counter2    = 0;
  counterdown = 0;
  counterup   = 0;
  
  releasefactor = 16;

  Serial.println("Initialization Activated");
  
  // Move primary cylinder down for 0.5 seconds
  Serial.println("Primary Moving Down for Delay");
  digitalWrite(primary_down, HIGH);
  delay(1000);
  digitalWrite(primary_down, LOW);
  
  // Move secondary cylinder right for 0.5 seconds
  Serial.println("Secondary Moving Right for Delay");
  digitalWrite(secondary_right, HIGH);
  delay(1000);
  digitalWrite(secondary_left, LOW);
  
  // Move primary cylinder up to the top-most position.
  Serial.println("Primary Moving Up Max");
  digitalWrite(primary_up, HIGH);
  while(analogRead(0) > 80){}
  while(analogRead(0) < 80){}
  digitalWrite(primary_up, LOW);
  
  // Move secondary cylinder left to the left-most position. Wait 0.5 seconds.
  Serial.println("Secondary Moving Left Max");
  digitalWrite(secondary_left, HIGH);
  while(analogRead(1) > 80){}
  while(analogRead(1) < 80){}
  digitalWrite(secondary_left, LOW);
  Serial.println("Delay 0.5s");
  delay(500);
  
  // Move secondary cylinder right to the right-most position.
  Serial.println("Secondary Moving Right Max");
  digitalWrite(secondary_right, HIGH);
  while(analogRead(1) > 80){}
  while(analogRead(1) < 80){}
  digitalWrite(secondary_right, LOW);
  
  // Record the time taken (T1) while moving primary cylinder down to the middle position.
  Serial.println("Primary Moving Down Middle Observed");
  startcounter1=millis();
  digitalWrite(primary_down, HIGH);
  while(analogRead(0) > 80){}
  counter1=millis()-startcounter1;
  Serial.println(counter1);
  
  // Record the time taken (T2) while moving primary cylinder down to the bottom-most position. Wait 1 second.
  Serial.println("Primary Moving Down Max Observed");
  startcounter2=millis();
  while (analogRead(0)<80){}
  counter2=millis()-startcounter2;
  digitalWrite(primary_down, LOW);
  Serial.println(counter2);
  Serial.println("Delay 1s");
  delay(1000);
  
  // Add T1 and T2. The sum is the total time taken for the primary cylinder 
  // to move from the top-most position to the bottom-most position.
  Serial.println("Summing Primary Down Durations");
  counterdown=counter1+counter2;
  Serial.println(counterdown);
  
  Serial.println("Referencing Release Factor Constant");
  Serial.println(releasefactor);
  
  Serial.println("Calculating Release Time - Dividing Counterdown By Release Factor");
  long(releasetime)=float(counterdown)*int(releasefactor);
  Serial.println(releasetime);
  
  // Record the time taken (T3) while moving primary cylinder up to the middle position.
  Serial.println("Primary Moving Up Middle Observed");
  startcounter1=millis();
  digitalWrite(primary_up, HIGH);
  while (analogRead(0)>80){}
  counter1=millis()-startcounter1;
  Serial.println(counter1);
  
  // Record the time taken (T4) while moving primary cylinder up to the top-most position.
  Serial.println("Primary Moving Up Max Observed");
  startcounter2=millis();
  while (analogRead(0)<80){}
  counter2=millis()-startcounter2;
  digitalWrite(primary_up, LOW);
  Serial.println(counter2);
  
  // Add T3 and T4. The sum is the total time taken for the primary cylinder to move from
  // the bottom-most position to the top-most position.
  Serial.println("Summing Primary Up Durations");
  counterup=counter1+counter2;
  Serial.println(counterup);
  
  // Establish the timing ratio (R1) as [T3] / [T3 + T4].
  Serial.println("Calculating Timing Ratio - Dividing Duration of Primary Up Middle by Duration of Primary Up Middle and Max");
  timingratio=float(counter1)/float(counterup);
  Serial.println(timingratio);
  
  Serial.println("Calculating Compression Time - Multiplying the Timing Ratio by the Primary Up Sum");
  compressiontime=float(timingratio)*float(counterup);
  Serial.println(compressiontime);
  
  // At this point, the primary cylinder is in the top-most position.
  // At this point, the secondary cylinder is in the right-most position.
  
  Serial.println("Initialization Complete");
}

void loop(){
  Serial.println("Production Loop Initiated");  
    
  // Move secondary cylinder left. After the secondary cylinder clears the press-foot,
  // move primary cylinder down.
  Serial.println("Secondary Moving Left");
  digitalWrite(secondary_left, HIGH);
  while(analogRead(1)>80){}
  Serial.println("Primary Moving Down");
  digitalWrite(primary_down, HIGH);
  primary_toggle=0;
  
  // While the secondary cylinder moves left to the left-most position, stop the primary
  // cylinder if it reached its bottom-most position.
  Serial.println("While Secondary Approaches Left to Max, Stop Primary if Primary Maxed");
  while(analogRead(1)<80){
    if (analogRead(0)<80){
     primary_toggle=1;
    }
    if (primary_toggle==1 && analogRead(0)>80){
      digitalWrite(primary_down, LOW);
    }
  }
  digitalWrite(secondary_left, LOW);
  
  // While the secondary cylinder is at the left-most position, stop the primary cylinder 
  // if it reached its bottom-most position and break the while loop.
  Serial.println("While Secondary is Stopped, [Stop Primary and Break] when Primary Maxed");
  
  while(1){
    if (analogRead(0)<80){
      primary_toggle=1;
    }
    if (primary_toggle==1 && analogRead(0)>80){
      digitalWrite(primary_down, LOW);
      break;
    }
  }
  Serial.println("Synchronous Loop Broken");
  
  // Start shaker. After a shaking delay, stop shaker.
  Serial.println("Shaker Moving for Delay");
  digitalWrite(shaker, HIGH);
  Serial.println("Delay 2s");
  delay(2000);
  digitalWrite(shaker, LOW);
  
  // Move secondary cylinder to the middle position.
  Serial.println("Secondary Moving Left to Middle");
  digitalWrite(secondary_right, HIGH);
  while(analogRead(1)>80){}
  digitalWrite(secondary_right, LOW);
  
  // Move the primary cylinder up for a period of time equal to the product of T3 and R1.
  Serial.println("Primary Moving Up for Delay");
  digitalWrite(primary_up, HIGH);
  Serial.println("Delay Compression Time");
  delay(long(compressiontime));
  digitalWrite(primary_up, LOW);
  
  // Move primary cylinder down for a period of time equal to the fraction of [T1 + T2] / [release].
  Serial.println("Primary Moving Down for Delay");
  digitalWrite(primary_down, HIGH);
  Serial.println("Delay Release Time");
  delay(long(releasetime));
  digitalWrite(primary_down, LOW);
  
  // Move secondary cylinder right to the right-most position.
  Serial.println("Secondary Moving Right to Max");
  digitalWrite(secondary_right, HIGH);
  while(analogRead(1)<80){}
  digitalWrite(secondary_right, LOW);
  
  // Move primary cylinder up to the top-most position.
  Serial.println("Primary Moving Up to Max");
  digitalWrite(primary_up, HIGH);
  while(analogRead(0)>80){}
  while(analogRead(0)<80){}
  digitalWrite(primary_up, LOW);
  
  Serial.println("Production Loop Completed");
}

