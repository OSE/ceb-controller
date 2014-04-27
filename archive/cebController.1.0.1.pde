// State Machine for CEB Press
// Open Source Ecology / Factor-E Farm
// Benjamin Gatti - 10-5-2009
// This code is provided free to use and modify under the GPL

// Constants Defined
int State = 0;
float TFactor = .6;
boolean Run = 1;
BaseTime = 4000
// End of Constants Defined

// Definition of Positions and Sequence Timing
#define STATEMIN  1
#define STATEDOWN  1 //Drop the press all the way down
int TD = 3000;
#define STATELOAD  2 //Wait/Jiggle for chamber to fill
int TL = 250;
#define STATECLOSE  3 //Move Drawer to closed position - (Move for TC millis)
int TC = 660;
#define STATEPRESS  4  //Raise the press
int TP = 5000;
#define STATERELEASE  5  //Drop the Press slightly (TR millis)
int TR = 120;
#define STATEOUT  6  //Move Drawer to open position - fully Out
int TO = 2000;
#define STATEUP  7  //Raise the press all the way Up - lifting the Brick above the eject trough
int TU = 600;
#define STATEEJECT  8  //Move Drawer - full In
int TE = 500;
#define STATEMAX  STATEEJECT
//End of Definition of Positions and Sequence Timing

// Definition of Output Pin Assignment on Arduino
int solPressUp = 5;
int solPressDown = 6;
int solDrawerIn = 10;
int solDrawerOut = 11;
int Timer = 0; //clock for current state
int ledPin = 13;                // LED connected to digital pin 13
// End of Definition of Output Pin Assignment on Arduino

// Setup
void setup()                    // run once, when the sketch starts
{
  shutdown();
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  pinMode(solPressUp, OUTPUT);      // sets the digital pin as output
  pinMode(solPressDown, OUTPUT);      // sets the digital pin as output
  pinMode(solDrawerIn, OUTPUT);      // sets the digital pin as output
  pinMode(solDrawerOut, OUTPUT);      // sets the digital pin as output

  Run = 1;
  Serial.begin (19200);
  Serial.println("Started");
  State = STATEMIN;
  shutdown();
  Timer = 100;
}
// End Setup

// Shutdown
void shutdown(){
  digitalWrite(solPressUp,LOW);
  digitalWrite(solPressDown,LOW);
  digitalWrite(solDrawerIn,LOW);
  digitalWrite(solDrawerOut,LOW);
}
//End Shutdown

// Actual Pressing Steps
void Transition()
{
  shutdown();
  //  Serial.writeln (State,DEC)
  switch (State)    {
  case STATEDOWN:  //Drop the press all the way down
    {
      Timer = TD * TFactor;
      digitalWrite(solPressDown,HIGH);
      Serial.println("Down");
      break;
    }
  case STATELOAD: //Wait/Jiggle for chamber to fill
    {
      Timer = TL * TFactor;
      Serial.println("Load");
      break;
    }
  case STATECLOSE:  //Move Drawer to closed position - (Move for TC millis)
    {
      Timer = TC * TFactor;
      digitalWrite(solDrawerIn,HIGH);
      Serial.println("Close");
      break;
    }
  case STATEPRESS: //Raise the press
    {
      Timer = TP * TFactor;
      digitalWrite(solPressUp,HIGH);
      Serial.println("Press");
      break;
    }
  case STATERELEASE: //Drop the Press slightly (TR millis)
    {
      Timer = TR * TFactor;
      digitalWrite(solPressDown,HIGH);
      Serial.println("Release");
      break;
    }
  case STATEOUT:   //Move Drawer to open position - fully Out
    {
      Timer = TO * TFactor;
      digitalWrite(solDrawerOut,HIGH);
      Serial.println("Out");
      break;
    }
  case STATEUP:  //Raise the press all the way Up - lifting the Brick above the eject trough
    {
      Timer = TU * TFactor;
      digitalWrite(solPressUp,HIGH);
      Serial.println("Up");
      break;
    }
  case STATEEJECT:   //Move Drawer - full In
    {
      Timer = TE * TFactor;
      digitalWrite(solDrawerIn,HIGH);
      Serial.println("Eject");
      break;
    }
  default:
    Run =0;
    Serial.println("Error on State Case Shut Down");
    break;
  }
}
// End Actual Pressing Steps


void loop()                     // run over and over again
{
  if(Run==1)
  {
    if (Timer>0){
      delay(1);
      Timer--;
    }
    else
    {
      State++;
      if (State > STATEMAX) {
        State=STATEMIN;
      }
      Transition();
    }
  }
  else
  {
    digitalWrite(solPressUp,LOW);
    digitalWrite(solPressDown,LOW);
    digitalWrite(solDrawerIn,LOW);
    digitalWrite(solDrawerOut,LOW);
  }


  //  digitalWrite(ledPin, HIGH);   // sets the LED on
  //  digitalWrite(Chnl0, HIGH);   // sets the LED on
  //  delay(500);                  // waits for a second
  //  digitalWrite(ledPin, LOW);   // sets the LED on
  //  digitalWrite(Chnl0, LOW);   // sets the LED on
  //  delay(500);                  // waits for a second



}

