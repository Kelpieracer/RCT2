#include <IRremote.h>
#include "arduino.h"

#define IR_0 0xFF6897
#define IR_1 0xFF30CF
#define IR_2 0xFF18E7
#define IR_3 0xFF7A85
#define IR_4 0xFF10EF
#define IR_5 0xFF38C7
#define IR_6 0xFF5AA5
#define IR_7 0xFF42BD
#define IR_8 0xFF4AB5
#define IR_9 0xFF52AD

#define IR_PWR 0xFFA25D
#define IR_MODE 0xFF629D
#define IR_MUTE 0xFFE21D
#define IR_PAUSE 0xFF22DD
#define IR_REV 0xFF02FD
#define IR_FF 0xFFC23D
#define IR_EQ 0xFFE01F
#define IR_MINUS 0xFFA857
#define IR_PLUS 0xFF906F
#define IR_RANDOM 0xFF9867
#define IR_USD 0xFFB04F

const int RECV_PIN = 46;
IRrecv irrecv(RECV_PIN);
decode_results results;

void initIR()
{
	pinMode(13, OUTPUT);
	irrecv.enableIRIn();
	irrecv.blink13(true);
}

void checkIR()
{
	if (irrecv.decode(&results)) {
		//Serial.println(results.value, HEX);
		switch (results.value)
		{
		case IR_USD:
			changeMode(MODE_TEST);
			break;
		case IR_PAUSE:
			changeMode(MODE_2ON2OFF);
			break;
		case IR_PWR:
			changeMode(MODE_RC);
			break;
		case IR_MUTE:
			changeMode(MODE_MUTE);
			break;
		default:
			break;
		}
		irrecv.resume(); // Receive the next value
	}
}