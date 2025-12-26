#include <iostream>

#define CONIOEX
#include "conioex.h"

#include "Transition.h"

TransitionColors transitionColor = T_WHITE;
bool transition = false;
int x;

//===============================
//トランジション生成関数
// 
//引数		TransitionColors color
//			トランジションに使う色
//			conioexのCOLORSに対応
//===============================
void TransitionStart(TransitionColors color)
{
	transitionColor = color;
	transition = true;
	x = 0;
}

//===============================
//トランジション描画関数
//===============================
void DrawTransition()
{
	if (transition)
	{
		textbackground((COLORS)transitionColor);
		if (transition)
		{
			for (int y = 0; y < 26; y++)
			{
				if ((y + 2) % 2)
				{
					gotoxy(x + 1, y + 1);
				}
				else
				{
					gotoxy(81 - x, y + 1);
				}
				std::cout << " ";
			}

			x++;
			if (x > 80)
			{
				transition = false;
			}
		}
	}
}