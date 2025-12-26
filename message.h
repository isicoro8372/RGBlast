#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <vector>
#include <string>
#include <sstream>

//===============================
//メッセージ表示クラス(static化して使うこと)
//関数		SelectScreen(int messageNum);
// 
//			DrawSelectScreenText(int messageNumber, int selected_)
//			選択式メッセージの内容
//			message.hで定義
//			int selected_
//			選択中の選択肢
//===============================
class Messages
{
public:

	enum MessageColors
	{
		M_BLACK,			/* 黒		*/
		M_BLUE,				/* 濃い青	*/
		M_GREEN,			/* 緑		*/
		M_CYAN,				/* 青緑		*/
		M_RED,				/* 濃い赤	*/
		M_MAGENTA,			/* 紫		*/
		M_BROWN,			/* 濃い黄	*/
		M_LIGHTGRAY,		/* 25%灰色	*/
		M_DARKGRAY,			/* 50%灰色	*/
		M_LIGHTBLUE,		/* 青		*/
		M_LIGHTGREEN,		/* 明るい緑	*/
		M_LIGHTCYAN,		/* 水色		*/
		M_LIGHTRED,			/* 赤		*/
		M_LIGHTMAGENTA,		/* ピンク	*/
		M_YELLOW,			/* 黄		*/
		M_WHITE				/* 白		*/
	};

	enum selectType
	{
		Single,
		Double,

		None = -1,
	};

	enum effectType
	{
		Blinking,

		Ignore = -1
	};

	int selected = -1;
	int message = -1;
	int selectedcolor = 0x0E;
	selectType type = None;

	void DrawSelectScreenText(MessageColors color, MessageColors selectedcolor, effectType effectType);
	int SelectScreen(int messageNumber, selectType selectType, MessageColors textcolor, MessageColors selectedcolor, char* sound, effectType effect, DWORD time);
	void Init();

private:
	int nowselect = 0;
	DWORD lastEffectTime = 0;
	DWORD selectedTime = 0;

	bool pressed_enter = false;
	bool effect = false;

	struct messages
	{
		std::string text;
		int x;
		int y;
		int textattr;
	};

	//メッセージ定義
	std::vector<std::vector<messages>> text =
	{
		{{"スタート", 38, 14, 0x0F}, {"ランキング", 37, 15, 0x0F}, {"ゲーム終了", 37, 17, 0x0C}}, //タイトル
		{{"再開", 40, 11, 0x0F}, {"やり直す", 38, 12, 0x0F}, {"タイトルへ", 37, 14, 0x0C}}, //ポーズメニュー
		{},
	};
};

#endif
