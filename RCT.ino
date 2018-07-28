/*
    Name:       RCT.ino
    Created:	01-Jul-18 10:40:31
    Author:     Tapio Mäntysalo
*/
//#include <ir_Lego_PF_BitStreamEncoder.h>
//#include <IRremoteInt.h>
//#include <boarddefs.h>
#include "timer.h"
#include <IRremote.h>

#define SENSORS 20
#define FIRST_SENSOR_IN_USE 0
#define LAST_SENSOR_IN_USE 17
#define SENSOR_FAIL 0
#define SENSOR_COVERED 1
#define SENSOR_SEES 2

#define MODE_TEST 0
#define MODE_2ON2OFF 1
#define MODE_RUNNING 2
#define MODE_MUTE 3

static char mode = MODE_RUNNING;

byte sensorStates[SENSORS];