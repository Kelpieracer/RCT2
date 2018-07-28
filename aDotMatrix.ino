
//http://embed.plnkr.co/3VUsekP3jC5xwSIQDVHx/preview

#include "timer.h"

#define DISPLAY_PERIOD_MS 20

#define COL_1 A4
#define COL_2 7
#define COL_3 8
#define COL_4 A1
#define COL_5 10
#define COL_6 A2
#define COL_7 A6
#define COL_8 A7

#define ROW_1 9
#define ROW_2 6
#define ROW_3 11
#define ROW_4 5
#define ROW_5 A3
#define ROW_6 12
#define ROW_7 A5
#define ROW_8 A0

/*

1588BS component Top
i/o	use		pin pin use	i/o
5	col4	1	9	row8	A7
6	col2	2	10	row7	A6
7	row2	3	11	col7	A5
8	row3	4	12	row1	A4
9	col1	5	13	col5	A3
10	row5	6	14	row4	A1
11	col3	7	15	row6	A2
12	col6	8	16	col8	A0

Pin	Col	Row

*/

const byte rows[] = {
	ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte cols[] = {
	COL_1, COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

// The display buffer
// It's prefilled with a smiling face (1 = ON, 0 = OFF)
// https://xantorohara.github.io/led-matrix-editor/

byte* screenBuffer = A;

byte* testModeScreenBuffer;



static long _displayTime = millis();
void refreshDisplay() {
	// Is it time to check the sensors?
	long _currentTime = millis();
	if (_currentTime < _displayTime + DISPLAY_PERIOD_MS)
		return;
	else
		_displayTime = _currentTime;

	drawScreen(screenBuffer);

	if (mode == MODE_TEST)
		testModeScreenBuffer = getRawSensorData(_currentTime % (DISPLAY_PERIOD_MS / 2), sensorStates);
}

void  drawScreen(byte buffer2[]) {


	// Turn on each row in series
	for (byte i = 0; i < 8; i++) {
		setColumns(buffer2[i]); // Set columns for this specific row

		digitalWrite(rows[i], HIGH);
		delay(2); // Set this to 50 or 100 if you want to see the multiplexing effect!
		digitalWrite(rows[i], LOW);

	}
}


void setColumns(byte b) {
	digitalWrite(COL_1, (~b >> 0) & 0x01); // Get the 1st bit: 10000000
	digitalWrite(COL_2, (~b >> 1) & 0x01); // Get the 2nd bit: 01000000
	digitalWrite(COL_3, (~b >> 2) & 0x01); // Get the 3rd bit: 00100000
	digitalWrite(COL_4, (~b >> 3) & 0x01); // Get the 4th bit: 00010000
	digitalWrite(COL_5, (~b >> 4) & 0x01); // Get the 5th bit: 00001000
	digitalWrite(COL_6, (~b >> 5) & 0x01); // Get the 6th bit: 00000100
	digitalWrite(COL_7, (~b >> 6) & 0x01); // Get the 7th bit: 00000010
	digitalWrite(COL_8, (~b >> 7) & 0x01); // Get the 8th bit: 00000001

										   // If the polarity of your matrix is the opposite of mine
										   // remove all the '~' above.
}

TimerClass _blinkObj;
byte bitWise[8] = { B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000 };

byte* getRawSensorData(byte blink, byte sensors[]) 
{
	static byte testModeScreenBuffer[8] = { 0,0,0,0,0,0,0,0 };
	//for (int i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
	//	Serial.print(sensors[i]);
	//Serial.println("");
	for (byte sensor = 0; sensor < 8; sensor++)
	{
		checkSensor(&sensors[sensor], &testModeScreenBuffer[0], 0, sensor);
	}
	for (byte sensor = 8; sensor < 16; sensor++)
	{
		checkSensor(&sensors[sensor], &testModeScreenBuffer[1], 1, sensor);
	}
	for (byte sensor = 16; sensor < 24; sensor++)
	{
		checkSensor(&sensors[sensor], &testModeScreenBuffer[2], 2, sensor);
	}
	return testModeScreenBuffer;
	//Serial.print(output[0], BIN);
	//Serial.print(" - ");
	//Serial.print(output[1], BIN);
	//Serial.println("");
}

byte checkSensor(byte *sensor, byte *output, byte row, byte sensorIdx)
{
	if (sensorIdx < FIRST_SENSOR_IN_USE || sensorIdx > LAST_SENSOR_IN_USE)
		return;
	if (*sensor == SENSOR_COVERED)
		*output |= bitWise[sensorIdx % 8];
	else if (*sensor == SENSOR_FAIL)
		*output |= bitWise[sensorIdx % 8] * (millis() / 500) % 2;
}