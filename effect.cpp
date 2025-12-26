#include <iostream>


#define CONIOEX
#include "conioex.h"
#include "effect.h"

struct EffectObject
{
	bool use = false;
	EffectType effectType = None;
	int x = -1;
	int y = -1;
	int size_x = -1;
	int size_y = -1;
	EffectColors color = E_WHITE;
	unsigned char nowEffectFrame = 0;
	unsigned char maxEffectFrame = 0;
	DWORD lastRenderTime = 0;
};

EffectObject effectObject[16];

//フラッシュエフェクト
bool effect_flash[EFFECT_NOTESEFFECT_FRAME][EFFECT_FLASH_Y][EFFECT_FLASH_X] =
{
	{
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 1, 0, 0, 0,},
		{0, 0, 0, 1, 0, 0, 0,},
		{0, 0, 0, 1, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
	},

	{
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 1, 0, 0 ,0,},
		{0, 0, 1, 1, 1, 0 ,0,},
		{0, 1, 1, 1, 1, 1 ,0,},
		{0, 0, 1, 1, 1, 0 ,0,},
		{0, 0, 0, 1, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
	},

	{
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 1, 1, 1, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
	},

	{
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 1, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
	},

	{
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
		{0, 0, 0, 0, 0, 0 ,0,},
	},
};

bool effect_right_slash[EFFECT_NOTESEFFECT_FRAME][EFFECT_RIGHTSLASH_Y][EFFECT_RIGHTSLASH_X] =
{
	{
		{0, 0, 0, 0, 1, 1, 0,},
		{0, 0, 0, 0, 0, 1, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
	},

	{
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 1, 0,},
		{0, 0, 0, 0, 1, 1, 0,},
		{0, 0, 0, 0, 1, 1, 0,},
		{0, 0, 0, 0, 1, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
	},

	{
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 1, 1, 0,},
		{0, 0, 0, 1, 1, 0, 0,},
		{0, 0, 1, 1, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
	},

	{
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 1, 1, 0, 0, 0,},
		{0, 1, 1, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
	},

	{
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
		{0, 0, 0, 0, 0, 0, 0,},
	},
};

void GenerateEffect(EffectType effectType, int x, int y, EffectColors effectColor)
{
	for (int i = 0; i < sizeof(effectObject) / sizeof(EffectObject); i++)
	{
		if (effectObject[i].use == false)
		{
			effectObject[i].use = true;
			effectObject[i].effectType = effectType;
			effectObject[i].x = x;
			effectObject[i].y = y;
			effectObject[i].color = effectColor;
			effectObject[i].lastRenderTime = timeGetTime();
			switch (effectType)
			{
			case Flash:
				effectObject[i].maxEffectFrame = EFFECT_NOTESEFFECT_FRAME;
				effectObject[i].size_x = EFFECT_FLASH_X;
				effectObject[i].size_y = EFFECT_FLASH_Y;
				break;
			case Slash_Left:
				break;
			case Slash_Right:
				effectObject[i].maxEffectFrame = EFFECT_NOTESEFFECT_FRAME;
				effectObject[i].size_x = EFFECT_RIGHTSLASH_X;
				effectObject[i].size_y = EFFECT_RIGHTSLASH_Y;
				break;
			case None:
				break;
			default:
				break;
			}
			break;
		}
	}
}

void DrawEffect(int bpm)
{
	for (int i = 0; i < sizeof(effectObject) / sizeof(EffectObject); i++)
	{
		if (timeGetTime() > effectObject[i].lastRenderTime + ((int)(60.0f / bpm * 125.0f)))
		{
			if (effectObject[i].nowEffectFrame >= effectObject[i].maxEffectFrame)
			{
				effectObject[i].use = false;
				effectObject[i].effectType = None;
				effectObject[i].x = -1;
				effectObject[i].y = -1;
				effectObject[i].size_x = -1;
				effectObject[i].size_y = -1;
				effectObject[i].color = E_WHITE;
				effectObject[i].nowEffectFrame = 0;
				effectObject[i].maxEffectFrame = 0;
			}
			else
			{
				textcolor(effectObject[i].color);
				for (int y = 0; y < effectObject[i].size_y; y++)
				{
					for (int x = 0; x < effectObject[i].size_x; x++)
					{
						gotoxy(effectObject[i].x + (x * 2), effectObject[i].y + y);
						switch (effectObject[i].effectType)
						{
						case Flash:
							if (effect_flash[effectObject[i].nowEffectFrame][y][x])
								std::cout << "■";
							else
								std::cout << "  ";
							break;
						case Slash_Right:
							if (effect_right_slash[effectObject[i].nowEffectFrame][y][x])
								std::cout << "■";
							else
								std::cout << "  ";
							break;
						case Slash_Left:
							if (effect_flash[effectObject[i].nowEffectFrame][y][x])
								std::cout << "■";
							else
								std::cout << "  ";
							break;
						default:
							break;
						}
					}
				}
			}

			effectObject[i].nowEffectFrame++;
			effectObject[i].lastRenderTime = timeGetTime();
		}
	}
}