#include "Arduino.h"

#define SENSOR_PERIOD_MS 5
#define SENSOR_RECORD_STEPS
#define LASER_SENSOR_DELAY_US 20

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



long sensorTime = millis();

void initSensors() {
	for (int i = 0; i<SENSORS - 1; i++)
		pinMode(Sensor[i], INPUT);
	for (int i = 0; i<SENSORS - 1; i++)
		pinMode(Laser[i], OUTPUT);

	return;
}

void checkSensors(bool running, bool startRecording) {
	long _currentTime = millis();
	if (!running)	// Is it time to check the sensors?
		return;

	if (mode == MODE_MUTE)
	{
		for (byte i = FIRST_SENSOR_IN_USE; i < LAST_SENSOR_IN_USE + 1; i++)
			digitalWrite(Laser[i], LOW);
		return;
	}

	if (_currentTime < sensorTime + SENSOR_PERIOD_MS)
		return;
	else
		sensorTime = _currentTime;

	// Check all the sensors that are in use
	for (byte i = FIRST_SENSOR_IN_USE; i < LAST_SENSOR_IN_USE+1; i++) {
		digitalWrite(Laser[i], LOW);
		if (digitalRead(Sensor[i]) == LOW) {
			// Too much light
			sensorStates[i] = SENSOR_FAIL;
			//Serial.print("f");
		}
		else 
		{
			// Default is input is covered
			sensorStates[i] = SENSOR_COVERED;
			digitalWrite(Laser[i], HIGH);
			delayMicroseconds(LASER_SENSOR_DELAY_US);
			if (digitalRead(Sensor[i]) == LOW) 
			{
				sensorStates[i] = SENSOR_SEES;
				//Serial.print("S");
			}
			else
			{
				//Serial.print("c");
			}
			digitalWrite(Laser[i], LOW);
		}
	};
	//Serial.println("");
}
