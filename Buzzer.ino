#define BUZZER_PIN 48
#define _BUZZ_MORSE_MS 70

static int BuzzBip[] = { 1, 0 };
int BuzzA[] = { 1, 1, 5, 0 };
int BuzzB[] = { 5, 1, 1, 1, 1, 1, 1, 0 };
int Buzz2on2of[] = { 1, 1, 1, 1, 1, 1, 5, 1, 5, 1, 0 };
int BuzzRun[] = { 1, 1, 5, 1, 1, 1, 0 };
int BuzzTest[] = { 5, 5, 1, 5, 1, 1, 1, 1, 1, 5, 5, 0 };
int BuzzMute[] = { 5, 1, 5, 0 };
int BuzzFail[] = { 10, 0 };

static long _buzzerTime_ms = millis();
static int* _buzzPtr = &BuzzBip[1];

void initBuzzer()
{
	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, LOW);
}

void testBuzzer()
{
	digitalWrite(BUZZER_PIN, HIGH);
	delay(100);
	digitalWrite(BUZZER_PIN, LOW);
}


void playBuzzer(int duration_ms)
{
	_buzzerTime_ms = millis() + duration_ms;
	digitalWrite(BUZZER_PIN, HIGH);
}

void playBuzzChar(int* ptr)
{
	digitalWrite(BUZZER_PIN, HIGH);
	_buzzerTime_ms = millis() + ((*ptr) * _BUZZ_MORSE_MS);
	_buzzPtr = ptr;
}

void checkBuzzer()
{
	if (*_buzzPtr == 0)
		return;

	long _currentTime = millis();
	if (_currentTime < _buzzerTime_ms)
		return;
	else
	{
		_buzzPtr++;
		if (*_buzzPtr == 0)
		{
			digitalWrite(BUZZER_PIN, LOW);
			return;
		}
		digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
		_buzzerTime_ms = _currentTime + (*_buzzPtr) * _BUZZ_MORSE_MS;
	}
}