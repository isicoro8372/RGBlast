#include <iostream>
#include <iomanip>

#include "main.h"
#include "message.h"
#include "input.h"
#include "gameplay.h"
#include "effect.h"
#include "resource.h"
#include "transition.h"

#define FRAMERATE (60)
#define RANKING_MAX (50)
#define NAME_MAX (32)
#define SCENE_TRANSITION_TIME (4000)

#define TITLE_X (78)
#define TITLE_Y (24)

enum Scenes
{
	TITLE,
	GAMEPLAY,
	RESULT,
	RANKING,
};

int fpsCount;
int lastExecuteTime;
int lastFrameTime;
DWORD currentTime;
int frameCount;
int framerate;
int framerateMultiplier;

Scenes nowScene;
DWORD sceneStartTime = 0;
bool save = false;
int rankOffset = 1;
bool end = false;

static Messages title;

HANDLE hInputHandle = GetStdHandle(STD_INPUT_HANDLE);
DWORD inputDefalt;

struct Ranking
{
	char name[NAME_MAX];
	int score;
};

Ranking ranking[RANKING_MAX];

char titleText[TITLE_Y][TITLE_X] =
{
	{0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,},
	{0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,0,0,},
	{2,2,2,2,2,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,0,0,2,2,2,0,},
	{0,0,2,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,2,0,0,0,2,},
	{0,0,2,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,2,0,0,0,0,},
	{0,0,2,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,2,0,0,2,2,},
	{0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,},
	{0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,},
	{2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,},
	{2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,},
	{2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,},
	{2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,},
	{2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,},
	{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,},
	{2,2,2,0,0,0,0,0,3,3,3,0,0,0,3,3,3,3,0,0,0,3,3,0,0,0,3,3,3,3,0,0,0,0,0,0,2,0,0,},
	{2,0,0,0,0,0,0,0,3,0,0,3,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,2,0,0,},
	{2,0,0,0,0,0,0,0,3,3,3,0,0,0,3,3,3,0,0,0,3,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,2,0,0,},
	{2,2,2,2,0,0,0,0,3,0,0,3,0,0,3,0,0,0,0,0,3,3,3,3,0,0,0,0,3,0,0,0,0,0,0,0,2,0,0,},
	{0,0,0,0,0,0,0,0,3,0,0,3,0,0,3,0,0,0,0,0,3,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,3,3,3,0,0,0,3,3,3,3,0,0,3,0,0,3,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,},
};

void WindowConsoleInitialize(void)
{
	// Set console window position
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 130, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	// Set console window size
	SMALL_RECT windowSize = { 0, 0, 80, 25 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
	// Set console buffer size
	// ウィンドウサイズは80, 25で固定すること
	COORD coord;
	coord.X = 80;
	coord.Y = 25;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	// Set console font style and size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	// ↓フォントサイズのみ変更可↓
	// ただしこの値を上限としたほうがいい
	// フォントに合わせてウィンドウが広がり画面からはみ出る可能性があるので
	cfi.dwFontSize.X = 20;//横サイズ
	cfi.dwFontSize.Y = 40;//縦サイズ
	// ↑フォントサイズのみ変更可↑
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"MS Gothic");// Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

int main()
{
	Init();

	do
	{
		currentTime = timeGetTime();

		Update();

	} while (!end);

	return 0;
}

void Init()
{
	setcaption((char*)"Rythm Got the Beat.");

	WindowConsoleInitialize();
	textattr(0xF0);
	clrscr();
	setcursortype(NOCURSOR);
	gotoxy(0, 0);

	timeBeginPeriod(1);

	lastExecuteTime =
	lastFrameTime = timeGetTime();
	currentTime =
	frameCount = 0;
	framerate = FRAMERATE;

	InitKey();
	Resource::Init();

	for (int i = 0; i < RANKING_MAX; i++)
	{
		ranking[i].name[0] = '\0';
		ranking[i].score = INT_MIN;
	}
	
	FILE* rankingFile;
	rankingFile = fopen("ranking.bin", "rb+");
	if (!rankingFile)
	{
		rankingFile = fopen("ranking.bin", "wb+");
		fwrite(ranking, sizeof(Ranking), RANKING_MAX, rankingFile);
	}
	else
	{
		fread(ranking, sizeof(Ranking), RANKING_MAX, rankingFile);
	}
	fclose(rankingFile);

	GetConsoleMode(hInputHandle, &inputDefalt);
	SetConsoleMode(hInputHandle, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS);

	framerate = FRAMERATE;

	nowScene = TITLE;
	sceneStartTime = 0;
	rankOffset = 1;
}

void Update()
{
	UpdateKey();

#ifdef _DEBUG
	if ((currentTime - lastFrameTime) >= 500)
	{
		fpsCount = frameCount * 1000 / (currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		frameCount = 0;
	}
#endif // !_DEBUG

	if ((currentTime - lastExecuteTime) >= (1000 / framerate))
	{
		lastExecuteTime = currentTime;
#ifdef _DEBUG
		DispDebug();
#endif
		DWORD nowTime = timeGetTime();

		frameCount++;
		UpdateFrameKey();
		DrawTransition();

		////ポーズメニュー
		//if (GetKeyDown(KEY_PAUSE) && nowScene != TITLE)
		//{
		//	pause = !pause;
		//	if (!pause)
		//	{
		//		pauseMenu.Init();
		//	}
		//}
		//if (pause && nowScene != TITLE) 
		//{
		//	pauseMenu.SelectScreen(1, Messages::DOUBLE);
		//	switch (pauseMenu.selected)
		//	{
		//	case -1:
		//		break;
		//	case 0:
		//		pause = false;
		//		break;
		//	case 1:
		//		pause = false;
		//		GameplayInit();
		//		break;
		//	case 2:
		//		pause = false;
		//		clrscr();
		//		nowScene = TITLE;
		//		break;
		//	}
		//	if (pauseMenu.selected != -1)
		//	{
		//		pauseMenu.Init();
		//	}
		//}
		//else
		//{
		
		//ゲーム処理
		//タイトル画面
		if (nowScene == TITLE)
		{
			if (!transition)
			{
				if (sceneStartTime == 0)
				{
					for (int y = 0; y < TITLE_Y; y++)
					{
						gotoxy(3, y + 2);
						for (int x = 0; x < TITLE_X; x++)
						{
							switch (titleText[y][x])
							{
							case 0:
								textbackground(WHITE);
								break;
							case 1:
								textbackground(RED);
								break;
							case 2:
								textbackground(GREEN);
								break;
							case 3:
								textbackground(BLUE);
								break;
							}
							std::cout << "  ";
						}
					}

					textbackground(WHITE);

					sceneStartTime = nowTime;
				}
				//gotoxy(24, 6);
				//textcolor(RED);
				//std::cout << "Rythm ";
				//textcolor(GREEN);
				//std::cout << "Got ";
				//textcolor(BLACK);
				//std::cout << "the ";
				//textcolor(BLUE);
				//std::cout << "Beat";

				gotoxy(34, 10);
				title.SelectScreen(0, Messages::Double, Messages::M_DARKGRAY, Messages::M_BLACK, (char*)"Resource/gamestart.wav", Messages::Blinking, 2000);
				switch (title.selected)
				{
				case 0:
					TransitionStart(T_BLACK);
					GameplayInit();
					save = false;
					nowScene = GAMEPLAY;
					break;
				case 1:
					TransitionStart(T_LIGHTCYAN);
					nowScene = RANKING;
					break;
				case 2:
					end = true;
					break;
				default:
					break;
				}
				if (title.selected != -1)
				{
					title.Init();
				}
			}
		}

		//ゲームプレイ画面
		else if (nowScene == GAMEPLAY)
		{
			if (sceneStartTime == 0)
			{
				sceneStartTime = nowTime;
			}
			if (!transition)
			{
				int nextScene = (Scenes)Gameloop();
				if ((Scenes)nextScene != GAMEPLAY)
				{
					sceneStartTime = 0;
					nowScene = (Scenes)nextScene;
				}
			}
		}

		//リザルト画面
		else if (nowScene == RESULT)
		{
			if (sceneStartTime == 0) 
			{
				sceneStartTime = nowTime;
			}
			if (!transition)
			{
				gotoxy(30, 13);
				textcolor(BLACK);
				std::cout << "Score : " << std::setw(10) << score;

				//ランキング処理
				if(!save)
				{
					Ranking bufold = { "Noname", -1 };
					Ranking bufnew = { "Noname", -1 };
					int rank = -1;
					for (int i = 0; i < RANKING_MAX; i++)
					{
						if (ranking[i].score < score)
						{
							if (rank == -1)
							{
								rank = i;
							}

							bufold = ranking[i];
							ranking[i] = bufnew;
							bufnew = bufold;
						}
					}

					if (rank >= 0)
					{
						gotoxy(35, 14);
						std::cout << "順位" << std::setw(2) << rank + 1 << "位";
						SetConsoleMode(hInputHandle, inputDefalt);

						while (1)
						{
							gotoxy(20, 17);
							clreol();
							textcolor(BLACK);
							std::cout << "名前を入力 > ";
							bool foundTC = false;
							char namebuf[NAME_MAX];
							std::cin >> namebuf;
							for (int i = 0; i < NAME_MAX; i++)
							{
								if (namebuf[i] == '\0')
								{
									foundTC = true;
									break;
								}
							}
							if (!foundTC)
							{
								textcolor(RED);
								gotoxy(28, 18);
								std::cout << "名前が長すぎます!";
							}
							else
							{
								ranking[rank].score = score;
								strcpy(ranking[rank].name, namebuf);
								break;
							}
						}
						SetConsoleMode(hInputHandle, ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS);

						//ファイルに書き込み
						{
							FILE* rankingFile;
							rankingFile = fopen("ranking.bin", "wb+");
							fwrite(ranking, sizeof(Ranking), RANKING_MAX, rankingFile);
							fclose(rankingFile);
							save = true;
						}
					}
				}
				if (sceneStartTime + SCENE_TRANSITION_TIME < nowTime)
				{
					TransitionStart(T_LIGHTCYAN);
					sceneStartTime = 0;
					nowScene = RANKING;
				}
			}
		}

		//ランキング画面
		else if (nowScene == RANKING)
		{
			if (sceneStartTime == 0)
			{
				rankOffset = 1;
				sceneStartTime = nowTime;
			}
			if (!transition)
			{

				if (GetKeyDown(KEY_UP))
				{
					//clrscr();
					rankOffset--;
					if (rankOffset <= 0)
					{
						rankOffset = 1;
					}
				}
				if (GetKeyDown(KEY_DOWN))
				{
					//clrscr();
					rankOffset++;
					if (rankOffset > RANKING_MAX - 24)
					{
						rankOffset = RANKING_MAX - 24;
					}
				}

				for (int i = 0; i < 25; i++)
				{
					gotoxy(3, 1 + i);
					if (((rankOffset + i - 1) / 10 + 1) % 2 == 0)
					{
						textcolor(DARKGRAY);
					}
					else
					{
						textcolor(BLACK);
					}

					switch (i)
					{
					case 0:
						textcolor(YELLOW);
						break;
					case 1:
						textcolor(WHITE);
						break;
					case 2:
						textcolor(BROWN);
						break;
					}
					if (i < 3)
					{
						std::cout << std::setw(2) << i + 1 << "          " << std::setw(40) << ranking[i].name << "          " << std::setw(10) << ranking[i].score;
					}
					else
					{
						std::cout << std::setw(2) << i + rankOffset << "          " << std::setw(40) << ranking[i + rankOffset - 1].name << "          " << std::setw(10) << ranking[i + rankOffset - 1].score;
					}
				}

				if (GetKeyDown(KEY_PAUSE) || GetKeyDown(KEY_SELECT))
				{
					TransitionStart(T_WHITE);
					sceneStartTime = 0;
					nowScene = TITLE;
				}

				gotoxy(3, 26);
				textcolor(BLACK);
				std::cout << "(矢印キー上下でスクロール)                             ESCかENTERでタイトルへ";
			}
		}
		//}
	}

}

//void DrawTitle()


#ifdef _DEBUG
void DispDebug()
{
	gotoxy(1, 25);
	std::cout << "FPS:"<< fpsCount;
	gotoxy(1, 26);
	std::cout << GetKeyDownLastTime(KEY_SELECT);
}
#endif
