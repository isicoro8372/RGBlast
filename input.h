#ifndef _INPUT_H_
#define _INPUT_H_

#include	"main.h"

enum GAMEKEYNAME
{
	KEY_RED,
	KEY_GREEN,
	KEY_BLUE,

	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,

	KEY_SELECT,

	KEY_PAUSE,

	KEY_NUM,
};

struct	KEYINPUT
{
	int	Trigger;
	int	Release;
	int	Press;
	int	PressOld;
};

struct	REALTIMEINPUT
{
	int	Press;
	int	Release;
	DWORD PressedTime;
};


struct	KEYCONFIG
{
	GAMEKEYNAME	GameKey;
	int	KeyCode;
};

void InitKey();
void UpdateKey();
void UpdateFrameKey();

int	GetKeyDown(GAMEKEYNAME k);
int	GetKey(GAMEKEYNAME k);
int	GetKeyRelease(GAMEKEYNAME k);
int	GetKeyDownLastTime(GAMEKEYNAME k);

#endif // !_INPUT_H_

/*
KEYCONFIG型
定義したキーの名前(GAMEKEYNAME)とそれに対応するinport処理用の定数を保存

KEYINPUT型
入力されたキーの情報を格納

KEYINPUT型配列を、定義したキーの名前(GAMEKEYNAME)の長さで作成し、enumの番号に対応したキーをinportで判定。
KEYCONFIG型の配列でどのキーが対応するかを決める。
*/