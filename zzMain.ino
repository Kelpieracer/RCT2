	void loop() {
		for (int i = 0; i < SENSORS; i++) {
			digitalWrite(Laser[i], HIGH);
			delay(0);
			bool _light = digitalRead(Sensor[i]);
			digitalWrite(Laser[i], LOW);
			//Serial.print(_light ? "H " : "L ");
		};
		delay(0);
		//Serial.println();
		//digitalWrite(rows[1], LOW);
		//digitalWrite(cols[0], HIGH);
		//digitalWrite(cols[1], HIGH);
		//digitalWrite(cols[2], HIGH);
		//digitalWrite(cols[3], HIGH);
		//digitalWrite(cols[4], HIGH);
		//digitalWrite(cols[5], HIGH);
		//digitalWrite(cols[6], HIGH);
		//digitalWrite(cols[7], HIGH);
		refreshDisplay();
	}


