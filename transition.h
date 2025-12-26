#ifndef _TRANSITION_H_
#define _TRANSITION_H_

enum TransitionColors
{
	T_BLACK,			/* 黒		*/
	T_BLUE,				/* 濃い青	*/
	T_GREEN,			/* 緑		*/
	T_CYAN,				/* 青緑		*/
	T_RED,				/* 濃い赤	*/
	T_MAGENTA,			/* 紫		*/
	T_BROWN,			/* 濃い黄	*/
	T_LIGHTGRAY,		/* 25%灰色	*/
	T_DARKGRAY,			/* 50%灰色	*/
	T_LIGHTBLUE,		/* 青		*/
	T_LIGHTGREEN,		/* 明るい緑	*/
	T_LIGHTCYAN,		/* 水色		*/
	T_LIGHTRED,			/* 赤		*/
	T_LIGHTMAGENTA,		/* ピンク	*/
	T_YELLOW,			/* 黄		*/
	T_WHITE				/* 白		*/
};

void TransitionStart(TransitionColors color);
void DrawTransition();

extern bool transition;

#endif // !_TRANSITION_H_