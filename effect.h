#ifndef _EFFECT_H_
#define _EFFECT_H_

#define CONIOEX
#include "conioex.h"

enum EffectType
{
	Flash,
	Slash_Right,
	Slash_Left,

	None = -1,
};

#define EFFECT_NOTESEFFECT_FRAME (5)

#define EFFECT_FLASH_X (7)
#define EFFECT_FLASH_Y (7)

#define EFFECT_RIGHTSLASH_X (7)
#define EFFECT_RIGHTSLASH_Y (7)


enum EffectColors
{
	E_BLACK,			/* çï		*/
	E_BLUE,				/* îZÇ¢ê¬	*/
	E_GREEN,			/* óŒ		*/
	E_CYAN,				/* ê¬óŒ		*/
	E_RED,				/* îZÇ¢ê‘	*/
	E_MAGENTA,			/* éá		*/
	E_BROWN,			/* îZÇ¢â©	*/
	E_LIGHTGRAY,		/* 25%äDêF	*/
	E_DARKGRAY,			/* 50%äDêF	*/
	E_LIGHTBLUE,		/* ê¬		*/
	E_LIGHTGREEN,		/* ñæÇÈÇ¢óŒ	*/
	E_LIGHTCYAN,		/* êÖêF		*/
	E_LIGHTRED,			/* ê‘		*/
	E_LIGHTMAGENTA,		/* ÉsÉìÉN	*/
	E_YELLOW,			/* â©		*/
	E_WHITE				/* îí		*/
};

void GenerateEffect(EffectType effect, int x, int y, EffectColors effectColor);
void DrawEffect(int bpm);

extern bool effect_flash[EFFECT_NOTESEFFECT_FRAME][EFFECT_FLASH_Y][EFFECT_FLASH_X];
extern bool effect_right_slash[EFFECT_NOTESEFFECT_FRAME][EFFECT_RIGHTSLASH_Y][EFFECT_RIGHTSLASH_X];

#endif // !_EFFECT_H_