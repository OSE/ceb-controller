// Define pins by plugin of hydraulics
#define mainup 5
#define maindown 6
#define secin 10
#define secout 11
// Define pins 5, 6, 10, and 11 according to their hydraulic
// function on the CEB press - moving main cylinder up and down,
// and moving secondary cylinder in and out. 'In' is defined as
// the direction of brick ejection motion out of the machine.

// Define base motion time as 1000 milliseconds.
unsigned long time = 1000;
// Define scaling factors that determine all motion of cylinders,
// based on known cylinder diameters and known motion distances.
float factor1 = 1;
float factor2 = 1;
float factor3 = 1;
float factor4 = 1;
float factor5 = 1;
float factor6 = 1;
float factor7 = 1;
float factor8 = 1;
float factor9 = 1;
float factor10 = 1;

// delay() function below happens to require unsigned longs

// The setup() content between {} runs once only
// when the sketch starts.

void setup()   {
  // initialize pins 5, 6, 10, and 11 as outputs:
  pinMode(mainup, OUTPUT);
  pinMode(maindown, OUTPUT);
  pinMode(secin, OUTPUT);
  pinMode(secout, OUTPUT);
  // Initialization should involve this series of steps:
  // 1. a quick drop of the main cylinder (to release pressure on drawer)
  // 2. motion of secondary cylinder all the way out (to expose any bricks)
  // 3. motion of main cylinder all the way up (to eject any bricks from hopper)
  // 4. motion of the secondary cylinder all the way in (to eject any brick from machine)
  // The result is all bricks cleared from machine - ready to start pressing cycles.
  digitalWrite(maindown, HIGH);   // start moving main cylinder down
  delay(time*2);                  // move it 6 seconds
  digitalWrite(maindown, LOW);    // stop moving
  digitalWrite(secin, HIGH);      // Start moving secondary cylinder in
  delay(time*2);                  // ...
  digitalWrite(secin, LOW);
}

// The loop() method runs over and over again,
// as long as the Arduino has power.

void loop()
{
  // Order of motion starts with main cylinder down and
  // secondary cylinder all the way in.
  // Motion pattern is to:
  // 1. Move secondary cylinder to pressing position
  digitalWrite(secout, HIGH);   // start moving
  delay(time*factor1);          // for a given duration
  digitalWrite(secout, LOW);    // and stop moving
  // 2. Start compression half way up.
  digitalWrite(mainup, HIGH);
  delay(time*factor2);
  digitalWrite(mainup, LOW);
  // 3.
  digitalWrite(secout, HIGH);
  delay(time);
  digitalWrite(secout, LOW);
}

