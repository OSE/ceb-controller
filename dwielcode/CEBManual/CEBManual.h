#include "Arduino.h"

typedef boolean (*fnptr)(int);

#define DIECIMILA 1
#ifdef TEENSY
const int blink_pin        = 11;

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
const int manual_down   = 18;
const int manual_up     = 19;
const int manual_left   = 20;
const int manual_right  = 21;
const int manual_shaker = 13;
#endif

#ifdef DIECIMILA
const boolean invert_output = true;

// const int blink_pin        = ;
const int pause_pin        = 12;
const int mode_pin         = 7;
const boolean invert_mode_pin = true;

// sensor pins
const int sensor_primary   = A4;
const int sensor_secondary = A5;
const int sensor_pressure  = A3;
const int knob_primary     = A2;

// output pins
const int primary_down    = 2;
const int primary_up      = 3;
const int secondary_left  = 4;
const int secondary_right = 5;
const int shaker          = 6;

// button pins
const int manual_down   = 8;
const int manual_up     = 9;
const int manual_left   = 10;
const int manual_right  = 11;
const int manual_shaker = 13;
#endif

