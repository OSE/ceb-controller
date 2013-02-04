/* LED Blink, Teensyduino Tutorial #1
   http://www.pjrc.com/teensy/tutorial.html
 
   This example code is in the public domain.
*/

const int ledPin = 11;   // Teensy has LED on 11, Teensy++ on 6
const int ledPin2 = 6;

// the setup() method runs once, when the sketch starts

void setup() {
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {
  digitalWrite(ledPin, HIGH);   // set the LED on
  digitalWrite(ledPin2, HIGH);  // set the LED on
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  digitalWrite(ledPin2, LOW);   // set the LED off
  delay(1000);                  // wait for a second
}

