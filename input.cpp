#include <iostream>

#define	CONIOEX
#include "input.h"

KEYCONFIG Key[] = 
{
	{KEY_RED, PK_F},
	{KEY_GREEN, PK_SP},
	{KEY_BLUE, PK_J},

	{KEY_RIGHT, PK_RIGHT},
	{KEY_LEFT, PK_LEFT},
	{KEY_UP, PK_UP},
	{KEY_DOWN, PK_DOWN},
	{KEY_SELECT, PK_ENTER},


	{KEY_PAUSE, PK_ESC},
};

REALTIMEINPUT InputKeyData[KEY_NUM];
KEYINPUT InputFrameKeyData[KEY_NUM];
bool isPressedNotesKey;

void	InitKey()
{
	memset(&InputKeyData[0], 0, sizeof(InputKeyData));
	memset(&InputFrameKeyData[0], 0, sizeof(InputFrameKeyData));
	isPressedNotesKey = false;
}

void UpdateKey()
{
	for (int i = 0; i < sizeof(Key) / sizeof(KEYCONFIG); i++)
	{
		InputKeyData[Key[i].GameKey].Press = inport(Key[i].KeyCode);
		if (InputKeyData[Key[i].GameKey].Press != false)
		{
			if (InputKeyData[Key[i].GameKey].Release == true)
			{
				InputKeyData[Key[i].GameKey].PressedTime = timeGetTime();
				InputKeyData[Key[i].GameKey].Release = false;
			}
		}
		else
		{
			InputKeyData[Key[i].GameKey].Release = true;
		}
	}
}

void UpdateFrameKey()
{
	for (int i = 0; i < sizeof(Key) / sizeof(KEYCONFIG); i++)
	{
		InputFrameKeyData[Key[i].GameKey].PressOld = InputFrameKeyData[Key[i].GameKey].Press;

		InputFrameKeyData[Key[i].GameKey].Press = InputKeyData[Key[i].GameKey].Press;

		InputFrameKeyData[Key[i].GameKey].Trigger =
			(InputFrameKeyData[Key[i].GameKey].PressOld ^
				InputFrameKeyData[Key[i].GameKey].Press) &
			InputFrameKeyData[Key[i].GameKey].Press;

		InputFrameKeyData[Key[i].GameKey].Release =
			(InputFrameKeyData[Key[i].GameKey].PressOld ^
				InputFrameKeyData[Key[i].GameKey].Press) &
			(~InputFrameKeyData[Key[i].GameKey].Press);
	}
}

int	GetKeyDown(GAMEKEYNAME k)
{
	return	InputFrameKeyData[k].Trigger;
}

int	GetKeyRelease(GAMEKEYNAME k)
{
	return InputFrameKeyData[k].Release;
}

int	GetKey(GAMEKEYNAME k)
{
	return InputFrameKeyData[k].Press;
}

int	GetKeyDownLastTime(GAMEKEYNAME k)
{
	return InputKeyData[k].PressedTime;
}