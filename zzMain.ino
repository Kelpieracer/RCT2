	void loop() {
		checkSensors(true);
		refreshDisplay();
		checkBuzzer();
		checkIR();
		showUI();
	}


