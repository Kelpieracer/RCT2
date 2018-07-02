#include "Arduino.h"

void setup() {
	Serial.begin(115200);
	
	// Setup IR
	pinMode(13, OUTPUT);
	irrecv.enableIRIn();
	irrecv.blink13(true);
	
	// Setup sensors and lasers
	setupSensors();

	// Setup display
	for (int i = 0; i < 8; i++) {
		pinMode(rows[i], OUTPUT);
		digitalWrite(rows[i], HIGH);
		pinMode(cols[i], OUTPUT);
		digitalWrite(cols[i], HIGH);
	}

	//digitalWrite(Laser[3], HIGH);
	//digitalWrite(Laser[4], HIGH);
	//digitalWrite(Laser[5], HIGH);
	//delay(400000);
}

