#define UI_CHANGE_SHOW_TIME 2000

static long _UITime_ms = millis();
static bool _newUI = false;
byte* screenPtr = CLEAR;
byte runningScreenBuffer[] = { 0,0,0,0,0,0,0,0 };
extern byte* screenBuffer;
extern byte* testModeScreenBuffer;

void changeMode(int newMode)
{
	 
	mode = newMode;
	switch (newMode)
	{
	case MODE_2ON2OFF:
		playBuzzChar(Buzz2on2of);
		screenBuffer = _2;;
		_UITime_ms = millis() + UI_CHANGE_SHOW_TIME;
		_newUI = true;
		break;
	case MODE_MUTE:
		playBuzzChar(BuzzMute);
		screenBuffer = M;
		_UITime_ms = millis() + UI_CHANGE_SHOW_TIME;
		_newUI = true;
		break;
	case MODE_RUNNING:
		playBuzzChar(BuzzRun);
		screenBuffer = R;
		_UITime_ms = millis() + UI_CHANGE_SHOW_TIME;
		_newUI = true;
		break;
	case MODE_TEST:
		playBuzzChar(BuzzTest);
		screenBuffer = T;
		_UITime_ms = millis() + UI_CHANGE_SHOW_TIME;
		_newUI = true;
		break;
	}
}


void showUI()
{
	if (_newUI)
	{
		if (millis() > _UITime_ms)
		{
			_newUI = false;
		}
		else
		{
			return;
		}
	}

	switch (mode)
	{
	case MODE_2ON2OFF:
		screenBuffer = _2;
		break;
	case MODE_MUTE:
		screenBuffer = CLEAR;
		break;
	case MODE_RUNNING:
		screenBuffer = runningScreenBuffer;
		break;
	case MODE_TEST:
		screenBuffer = testModeScreenBuffer;
		break;
	}
}
