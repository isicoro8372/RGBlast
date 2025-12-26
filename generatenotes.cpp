#include <iostream>
#include <time.h>

#define CONIOEX
#include "conioex.h"
#include "gameplay.h"
#include "generatenotes.h"

#define GENERATE_PERCENTAGE (70)

unsigned char templateNotes[] =
{ 
	{0b01000000},
	{0b00000100},
	{0b00010000},
	{0b00000001},

	{0b10000000},
	{0b00001000},
	{0b00100000},
	{0b00000010}, // 裏拍
};


void CreateNotes(int* colors, int* pointer)
{
	//初期化
	for (int i = 0; i < 8; i++)
	{
		pointer[i] = -1;
	}
	int buf = (0b00000000);

	int difficulty = bpm / 60;
	if (difficulty > 4)
	{
		difficulty = 4;
	}

	//抽選
	for (int i = 0; i < difficulty + 3; i++) //ノーツ生成
	{
		if (rand() % 100 < GENERATE_PERCENTAGE)
		{
			buf = buf | templateNotes[i];
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (buf & ((1 << i))) // i番目のビットが立っているか
		{
			pointer[i] = colors[rand() % COLORS];

			switch (pointer[i])
			{
			case 0:
				pointer[i] = RED;
				break;
			case 1:
				pointer[i] = GREEN;
				break;
			case 2:
				pointer[i] = BLUE;
				break;
			}

		}
	}
 }
