#include "Arduino.h"

void setup() {
	Serial.begin(115200);

	delay(500);		// Delay needed here to prevent double reset

	initBuzzer();

	initSound();
	
	// Setup IR
	initIR();
	
	// Setup sensors and lasers
	initSensors();

	// Setup display
	for (int i = 0; i < 8; i++) {
		pinMode(rows[i], OUTPUT);
		digitalWrite(rows[i], HIGH);
		pinMode(cols[i], OUTPUT);
		digitalWrite(cols[i], HIGH);
	}

	playBuzzChar(BuzzA);
}

