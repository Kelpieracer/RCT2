#include "Adafruit_Soundboard.h"

#define RC_MODE_ARMED 0
#define RC_MODE_STARTED 1
#define RC_MODE_ENDED 2
#define RC_MODE_WAIT_TO_CLEAR 3
#define RC_MODE_STARTED_BUT_IN_AIR 4

#define RC_WAIT_BEFORE_NEW_STRIDE 2000
#define RC_TOO_LONG_STRIDE_MS 5000
#define RC_NO_CONTACT_TIME_MS 500

#define RC_SCORE_EASING 2

byte rcMode = RC_MODE_ARMED;
long rcStrideStart_ms;
long rcStrideEnd_ms;

byte rcSensorCumulative[SENSORS];

byte patternScore1[] = {	B11000000, 
							B11000000 };

byte patternScore2[] = {	B11011000, 
							B11011000 };

byte patternScore3[] = {	B11011011, 
							B11011011 };

byte patternSpeed[] = {		B00000000,		// <1500
							B10000000,		// <1000
							B11000000,		// <800
							B11100000,		// <650
							B11110000,		// <500
							B11111000,		// <400
							B11111100,		// <300
							B11111110,		// <200
							B11111111 };	// <130

int rcSpeeds_ms[8] = { 1600, 1100, 800, 600, 450, 300, 200, 130 };

void drawRCScore(int score, byte* screenBuff)
{
	screenBuff[5] = 0;
	screenBuff[6] = 0;
	screenBuff[7] = 0;

	switch (score)
	{
	case 1:
		screenBuff[6] = patternScore1[0];
		screenBuff[7] = patternScore1[1];
		break;
	case 2:
		screenBuff[6] = patternScore2[0];
		screenBuff[7] = patternScore2[1];
		break;
	case 3:
		screenBuff[6] = patternScore3[0];
		screenBuff[7] = patternScore3[1];
		break;
	default:
		break;
	}
}

void drawRCSpeed(int speed_ms, byte* screenBuff)
{
	screenBuff[3] = patternSpeed[getRCSpeed(speed_ms)];
}

byte getRCSpeed(int speed_ms)
{
	byte _speed = 0;
	for (int i = 8; i > 0; i--)
	{
		if (rcSpeeds_ms[i] > speed_ms)
		{
			_speed = i;
			break;
		}
	}
	return _speed;
}

byte rcleds[] = { 0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15 };
extern byte bitWise[8];

void drawRCHits(byte* sensorData, byte* screenBuff)
{
	screenBuff[0] = 0;
	screenBuff[1] = 0;

	float rel = (float) (LAST_SENSOR_IN_USE - FIRST_SENSOR_IN_USE + 1) / 16;

	for (byte i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
	{
		byte idx = i - FIRST_SENSOR_IN_USE;
		byte dispIdx = rcleds[(int)(rel / (float)idx)];
		if (sensorData[idx] == SENSOR_COVERED)
			if (dispIdx < 8)
			{
				screenBuff[0] |= bitWise[7-dispIdx];
			}
			else
			{
				screenBuff[1] |= bitWise[(7-(dispIdx-8))];
			}
	}
}

byte getLowestHit(byte* sensorData)
{
	byte lowest;
	for (lowest = FIRST_SENSOR_IN_USE; lowest <= LAST_SENSOR_IN_USE; lowest++)
		if (sensorData[lowest] == SENSOR_COVERED)
			break;
	Serial.print("lowest:"); Serial.print(lowest);
	float rel = 1 - (float) (lowest - FIRST_SENSOR_IN_USE) / (LAST_SENSOR_IN_USE - FIRST_SENSOR_IN_USE);
	Serial.print(" / "); Serial.println(3.99 * rel);
	return 3.99 * rel;
}

byte getStrideLength(byte* sensorData)
{
	byte lowest;
	for (lowest = FIRST_SENSOR_IN_USE; lowest <= LAST_SENSOR_IN_USE; lowest++)
		if (sensorData[lowest] == SENSOR_COVERED)
			break;
	byte highest;
	for (highest = LAST_SENSOR_IN_USE; highest >= FIRST_SENSOR_IN_USE; highest--)
		if (sensorData[highest] == SENSOR_COVERED)
			break;
	float rel = (float) ((highest - lowest) - FIRST_SENSOR_IN_USE) / (LAST_SENSOR_IN_USE - FIRST_SENSOR_IN_USE);
	return 3.99 * rel;
}

byte getStrideSensors(byte* sensorData)
{
	byte sensorCount = 0;
	for (byte i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
		if (sensorData[i] == SENSOR_COVERED)
			sensorCount++;
	float rel = (float)sensorCount / (LAST_SENSOR_IN_USE - FIRST_SENSOR_IN_USE);
	return 10 * rel;
}

byte Max(byte a, byte b)
{
	return a > b ? a : b;
}

byte Min(byte a, byte b)
{
	return a < b ? a : b;
}

bool isBoardClear(byte* sensorData)
{
	for (byte i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
	{
		//Serial.print(i); Serial.print(" ");
		if (sensorData[i] == SENSOR_COVERED)
		{
			//Serial.println("now");
			return false;
		}
	}
	return true;
}

void clearRCScreen(byte* screenBuffer)
{
	for (byte i = 0; i <= 7; i++)
		screenBuffer[i] = 0;
}

void clearRCSensorCumulative()
{
	for (byte i = 0; i < SENSORS; i++)
		rcSensorCumulative[i] = SENSOR_SEES;
}

extern Adafruit_Soundboard sfx;
void cumulateRCSensor(byte* sensors)
{
	for (byte i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
	{
		if (sensors[i] == SENSOR_COVERED)
		{
			//if(rcSensorCumulative[i] != SENSOR_COVERED)
			//	if (!sfx.playTrack("CLICK1  WAV"))
			//		Serial.println("Failed to play track?");					
			rcSensorCumulative[i] = SENSOR_COVERED;
		}
	}
}

byte calculateRCScore()
{
	return _calculateRCScore(getStrideSensors(rcSensorCumulative), getLowestHit(rcSensorCumulative), getStrideLength(rcSensorCumulative), getRCSpeed(rcStrideEnd_ms - rcStrideStart_ms));
}

byte _calculateRCScore(byte sensors, byte lowestScore, byte strideLength, byte speed)
{
	byte rSensors = Min(Max(sensors, 1), 3);
	byte rLowest = Min(Max(lowestScore, 1), 3);
	byte rLength = Min(Max(strideLength, 1), 3);
	byte rSpeed = (speed / 2);
	Serial.print(rSensors); Serial.print(" ");
	Serial.print(rLowest); Serial.print(" ");
	Serial.print(rLength); Serial.print(" ");
	Serial.print(rSpeed); Serial.println(" ");
	byte _totalScore = ((rSensors * rLowest * rLength * rSpeed) * RC_SCORE_EASING) / 3;
	if (rSpeed > 0 && _totalScore == 0)
		_totalScore = 1;

	for (byte i = FIRST_SENSOR_IN_USE; i <= LAST_SENSOR_IN_USE; i++)
		Serial.print(rcSensorCumulative[i]);
	Serial.println();

	Serial.print("Sensors: ");
	Serial.print(sensors);
	Serial.print(" Lowest:");
	Serial.print(lowestScore);
	Serial.print(" Len:");
	Serial.print(strideLength);
	Serial.print(" Speed:");
	Serial.print(speed);
	Serial.print(" Dur(ms):");
	Serial.print(rcStrideEnd_ms - rcStrideStart_ms);
	Serial.print(" Total:");
	Serial.print(_totalScore);
	Serial.println("");

	return Max(Min(_totalScore, 3), 0);
}

extern byte RCScreenBuffer[];
void checkRC()
{
	switch (rcMode)
	{
	case RC_MODE_ARMED:
		if (isBoardClear(sensorStates))
		{
			break;
		}
		rcStrideStart_ms = millis();
		rcMode = RC_MODE_STARTED;
		Serial.println("RC_MODE_STARTED");
		cumulateRCSensor(sensorStates);
		break;
	case RC_MODE_STARTED:
		if (millis() - RC_TOO_LONG_STRIDE_MS > rcStrideStart_ms)
		{
			rcMode = RC_MODE_WAIT_TO_CLEAR;
			Serial.println("RC_MODE_WAIT_TO_CLEAR");
			clearRCScreen(RCScreenBuffer);
			playBuzzChar(BuzzFail);
			break;
		}
		drawRCHits(rcSensorCumulative, RCScreenBuffer);
		if (isBoardClear(sensorStates))
		{
			rcStrideEnd_ms = millis();
			rcMode = RC_MODE_STARTED_BUT_IN_AIR;
			Serial.println("RC_MODE_STARTED_BUT_IN_AIR");
			break;
		}
		cumulateRCSensor(sensorStates);
		break;
	case RC_MODE_STARTED_BUT_IN_AIR:
		if(millis() - RC_NO_CONTACT_TIME_MS > rcStrideEnd_ms)
		{
			rcMode = RC_MODE_ENDED;
			Serial.println("RC_MODE_ENDED");
			drawRCScore(calculateRCScore(), RCScreenBuffer);
			drawRCHits(rcSensorCumulative, RCScreenBuffer);
			drawRCSpeed(rcStrideEnd_ms-rcStrideStart_ms, RCScreenBuffer);
		}
		else if(!isBoardClear(sensorStates))
		{
			rcMode = RC_MODE_STARTED;
			Serial.println("RC_MODE_STARTED");
			cumulateRCSensor(sensorStates);
		}
		break;
	case RC_MODE_ENDED:
		if (millis() - RC_WAIT_BEFORE_NEW_STRIDE > rcStrideEnd_ms)
		{
			rcMode = RC_MODE_ARMED;
			Serial.println("RC_MODE_ARMED");
			clearRCSensorCumulative();
			playBuzzChar(BuzzBip);
		}
		break;
	case RC_MODE_WAIT_TO_CLEAR:
		if (isBoardClear(sensorStates))
		{
			clearRCScreen(RCScreenBuffer);
			playBuzzChar(BuzzBip);
			rcMode = RC_MODE_ARMED;
			Serial.println("RC_MODE_ARMED");
			break;
		}
		drawRCHits(sensorStates, RCScreenBuffer);
		break;
	}

	//drawRCHits(sensorStates, RCScreenBuffer);
	//drawRCScore(2, RCScreenBuffer);
	//drawRCSpeed(900, RCScreenBuffer);
}
