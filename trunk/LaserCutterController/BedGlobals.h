#include "Arduino.h"

#ifndef __BedGlobals_h__
#define __BedGlobals_h__


#define BED_STOPPER_PIN 10

#define COILA_PIN1   6
#define COILA_PIN2   7
#define COILB_PIN1   11
#define COILB_PIN2   12

#define DELAY_COIL_MS       20
#define DELAY_COIL_MS_FAST  12
#define ONE_TURN         144

//how many steps to do a 1mm
#define MILLI_BED_STEPS  6
#define CENTI_BED_STEPS  60

//north max dist is 85mm minus a little safety
#define MAX_DIST_NORTH (85 -3)
#define MAX_STEPS_NORTH (MAX_DIST_NORTH * MILLI_BED_STEPS) 

int mBedCurrentSpeed = DELAY_COIL_MS;

int mBedPos = 0;

#endif //__BedGlobals_h__
