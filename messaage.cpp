#include <iostream>
#include <time.h>
#include <conio.h>
#include <vector>

#define CONIOEX
#include "conioex.h"

#include "input.h"
#include "message.h"
#include "transition.h"
#include "resource.h"

//===============================
//�I�����`��֐�
//===============================
void Messages::DrawSelectScreenText(MessageColors color, MessageColors selectedcolor, effectType effectType)
{
	DWORD nowTime = timeGetTime();
	for (int i = 0; i < text[message].size(); i++)
	{
		textcolor((COLORS)color);
		if (i == nowselect)
		{
			textcolor((COLORS)selectedcolor);
			if (effect)
			{
				switch (effectType)
				{
				case Messages::Blinking:
					if (nowTime / 125 % 2)
					{
						lastEffectTime = nowTime;
						CONSOLE_SCREEN_BUFFER_INFO csbi;
						HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
						if (GetConsoleScreenBufferInfo(handle, &csbi))
						{
							if ((csbi.wAttributes & 0x00) == selectedcolor)
							{
								textcolor((COLORS)color);
							}
						}
					}
					break;
				}
			}
		}

		if (type == Single)
		{
			gotoxy(text[message][i].x, text[message][i].y);
		}
		else if (type == Double)
		{
			gotoxy(text[message][i].x - 2, text[message][i].y);
			if (i == nowselect)
			{
				std::cout << "> ";
			}
			else
			{
				std::cout << "  ";
			}
		}

		std::cout << text[message][i].text;

		if (i == nowselect)
		{
			std::cout << " <";
		}
		else
		{
			std::cout << "  ";
		}
	}
}

//===============================
//�I�����\���֐�
// 
//����		int messageNumber
//			���b�Z�[�W�̎Q�Ɣԍ�
//			message.h�Őݒ�
//			selectType selecttype
//			�I�����̕\���^�C�v		
// 
//�߂�l	int
//			�I�����̌���
//===============================
int Messages::SelectScreen(int messageNumber, selectType selectType, MessageColors textcolor, MessageColors selectedcolor, char* sound, effectType effectType, DWORD time)

{
	message = messageNumber;
	type = selectType;
	DWORD nowTime = timeGetTime();

	Messages::DrawSelectScreenText(textcolor, selectedcolor, effectType);

	if (!effect)
	{
		if (GetKeyDown(KEY_UP))
		{
			nowselect--;
			if (nowselect < 0)
			{
				nowselect = text[message].size() - 1;
			}
		}
		if (GetKeyDown(KEY_DOWN))
		{
			nowselect++;
			if (nowselect > text[message].size() - 1)
			{
				nowselect = 0;
			}
		}

		if (GetKeyDown(KEY_SELECT))
		{
			Resource::Playsound(sound);
			selectedTime = nowTime;
			effect = true;
#ifdef _DEBUG
			gotoxy(1, 2);
			std::cout << nowselect;
#endif // _DEBUG
		}
	}

	if (selectedTime + time < nowTime && effect)
	{
		selected = nowselect;
	}

	return selected;
}

//===============================
//�I�����������֐�
//===============================
void Messages::Init()
{
	textcolor(WHITE);
	for (int i = 0; i < text[message].size(); i++)
	{
		if (type == Double)
		{
			gotoxy(text[message][i].x - 2, text[message][i].y);
			std::cout << "  ";
		}

		std::string buf = text[message][i].text;
		for (int i = 0; i < buf.size() + 2; i++)
		{
			std::cout << " ";
		}
	}

	nowselect = 0;
	selected = -1;
	message = -1;
	selectedcolor = 0x0E;
	type = selectType::None;
	lastEffectTime = 0;
	effect = false;
	selectedTime = 0;
}