#include "Arduino.h"

#define SENSORS 20
#define FIRST_SENSOR_IN_USE 4
#define LAST_SENSOR_IN_USE 13
#define SENSOR_PERIOD_MS 10
#define LASER_SENSOR_DELAY 1000
#define FAIL 0
#define COVER 1
#define SEE 2

// Photo transistor sensors
// PT202C
// pins 1, 11 and 21 are ground
uint8_t Sensor[SENSORS] = { 26, // 2
27,		// 3
28,		// 4
29,		// 5
30,		// 6
31,		// 7
32,		// 8
33,		// 9
34,		// 10
36,		// 12
35,		// 13
38,		// 14
37,		// 15
40,		// 16
39,		// 17
42,		// 18
41,		// 19
44,		// 20
43,		// 22
45,		// 23
};

uint8_t Laser[SENSORS] = { 2,	// 2
3,		// 3
4,		// 4
A14,	// 5
A15,	// 6
17,		// 7
16,		// 8
19,		// 9
18,		// 10
20,		// 12
21,		// 13
24,		// 14
22,		// 15
23,		// 16
A12,	// 17
A13,	// 18
A10,	// 19
A11,	// 20
A9,		// 22
A8,		// 23
};

byte sensorState[SENSORS];

long sensorTime = millis();

void setupSensors() {
	for (int i = 0; i<SENSORS - 1; i++)
		pinMode(Sensor[i], INPUT);
	for (int i = 0; i<SENSORS - 1; i++)
		pinMode(Laser[i], OUTPUT);
}

void checkSensors(bool running) {
	// Is it time to check the sensors?
	long _currentTime = millis();
	if (_currentTime < sensorTime + SENSOR_PERIOD_MS)
		return;
	else
		sensorTime = _currentTime;
	if (!running)
		return;

	// Check all the sensors that are in use
	for (byte i = FIRST_SENSOR_IN_USE; i < LAST_SENSOR_IN_USE+1; i++) {
		if (digitalRead(Sensor[i]) == LOW) {
			// Too much light
			sensorState[i] = FAIL;
		}
		else {
			// Default is input is covered
			sensorState[i] = COVER;
		}
		digitalWrite(Laser[i], HIGH);
		for (int del = 0; del < LASER_SENSOR_DELAY; del++) {
		}
		if (digitalRead(Sensor[i]) == LOW)
			sensorState[i] = SEE;
		digitalWrite(Laser[i], LOW);
	};
	delay(0);
}
