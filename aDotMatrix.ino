
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
byte TODOS[] = { B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111 };
byte EX[] = { B00000000,B00010000,B00010000,B00010000,B00010000,B00000000,B00010000,B00000000 };
byte A[] = { B00000000,B00011000,B00100100,B00100100,B00111100,B00100100,B00100100,B00000000 };
byte B[] = { B01111000,B01001000,B01001000,B01110000,B01001000,B01000100,B01000100,B01111100 };
byte C[] = { B00000000,B00011110,B00100000,B01000000,B01000000,B01000000,B00100000,B00011110 };
byte D[] = { B00000000,B00111000,B00100100,B00100010,B00100010,B00100100,B00111000,B00000000 };
byte E[] = { B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00111100,B00000000 };
byte F[] = { B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00100000,B00000000 };
byte G[] = { B00000000,B00111110,B00100000,B00100000,B00101110,B00100010,B00111110,B00000000 };
byte H[] = { B00000000,B00100100,B00100100,B00111100,B00100100,B00100100,B00100100,B00000000 };
byte I[] = { B00000000,B00111000,B00010000,B00010000,B00010000,B00010000,B00111000,B00000000 };
byte J[] = { B00000000,B00011100,B00001000,B00001000,B00001000,B00101000,B00111000,B00000000 };
byte K[] = { B00000000,B00100100,B00101000,B00110000,B00101000,B00100100,B00100100,B00000000 };
byte L[] = { B00000000,B00100000,B00100000,B00100000,B00100000,B00100000,B00111100,B00000000 };
byte M[] = { B00000000,B00000000,B01000100,B10101010,B10010010,B10000010,B10000010,B00000000 };
byte N[] = { B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000 };
byte O[] = { B00000000,B00111100,B01000010,B01000010,B01000010,B01000010,B00111100,B00000000 };
byte P[] = { B00000000,B00111000,B00100100,B00100100,B00111000,B00100000,B00100000,B00000000 };
byte Q[] = { B00000000,B00111100,B01000010,B01000010,B01000010,B01000110,B00111110,B00000001 };
byte R[] = { B00000000,B00111000,B00100100,B00100100,B00111000,B00100100,B00100100,B00000000 };
byte S[] = { B00000000,B00111100,B00100000,B00111100,B00000100,B00000100,B00111100,B00000000 };
byte T[] = { B00000000,B01111100,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000 };
byte U[] = { B00000000,B01000010,B01000010,B01000010,B01000010,B00100100,B00011000,B00000000 };
byte V[] = { B00000000,B00100010,B00100010,B00100010,B00010100,B00010100,B00001000,B00000000 };
byte W[] = { B00000000,B10000010,B10010010,B01010100,B01010100,B00101000,B00000000,B00000000 };
byte X[] = { B00000000,B01000010,B00100100,B00011000,B00011000,B00100100,B01000010,B00000000 };
byte Y[] = { B00000000,B01000100,B00101000,B00010000,B00010000,B00010000,B00010000,B00000000 };
byte Z[] = { B00000000,B00111100,B00000100,B00001000,B00010000,B00100000,B00111100,B00000000 };


static long _displayTime = millis();
void refreshDisplay() {
	// Is it time to check the sensors?
	long _currentTime = millis();
	if (_currentTime < _displayTime + DISPLAY_PERIOD_MS)
		return;
	else
		_displayTime = _currentTime;
	showRawSensorData(_currentTime % (DISPLAY_PERIOD_MS / 2), sensorStates);
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

void showRawSensorData(byte blink, byte sensors[]) 
{
	//for (int i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
	//	Serial.print(sensors[i]);
	//Serial.println("");
	byte output[] = { B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000 };
	for (byte sensor = 0; sensor < 8; sensor++)
	{
		checkSensor(&sensors[sensor], &output[0], 0, sensor);
	}
	for (byte sensor = 8; sensor < 16; sensor++)
	{
		checkSensor(&sensors[sensor], &output[1], 1, sensor);
	}
	for (byte sensor = 16; sensor < 24; sensor++)
	{
		checkSensor(&sensors[sensor], &output[2], 2, sensor);
	}
	drawScreen(output);
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