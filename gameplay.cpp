#include <iostream>
#include <time.h>
#include <iomanip>

#define CONIOEX
#include "conioex.h"

#include "gameplay.h"
#include "input.h"
#include "generatenotes.h"
#include "effect.h"
#include "resource.h"
#include "transition.h"

#define MEASURE_MAX (4)

#define TIMING_GOOD (200)
#define TIMING_GREAT (140)
#define TIMING_EXELLENT (80)

#define LIFE_MAX (3)
#define INCREASE_BPM (30)
#define INCREASE_LEVEL (3)
#define INCREASE_MEASURES (90)

enum scenePhase
{
	CountDown,
	Ref,
	Model,
	Play,
	Stop = -1,
};

enum Timing
{
	Good = TIMING_GOOD,
	Great = TIMING_GREAT,
	Exellent = TIMING_EXELLENT,
	Bad = 0,
	Miss = 1,
	Wait = -1,
};

int colortype[COLORS] = { RED, BLUE, GREEN };
int bpm = 0;

int phase = 0;
int countdown = 0;
int countdownTimer = 0;

int notesType[MEASURE_MAX * 8];
int notesOfNumber = 0;				//ノーツ数
int notesSteps = 0;			//現在のノーツ
int rawGrade = 0; 
int playMeasures = 0;				//小節設定(最大4)
int wave = 0;

DWORD phaseStartTime = 0;
DWORD touchNotesTime;
DWORD inputTime = 0;

bool isTouchNotes = false;
bool missTouchNotes = false;
Timing timingGrade = Bad;

int timingBarPos = 0;

int score = 0;
char life = LIFE_MAX;

#ifdef _DEBUG
int debug_int = 0;
#endif // _DEBUG


void GameplayInit()
{
	phase = 0;
	bpm = 60;
	countdown = 3;
	countdownTimer = 0;
	notesSteps = 0;
	playMeasures = 1;
	wave = 0;

	touchNotesTime = 0;
	inputTime = 0;

	timingBarPos = 0;

	notesOfNumber = 0;
	isTouchNotes = false;
	phaseStartTime = 0;
	timingGrade = Wait;

	score = 0;
	life = LIFE_MAX;
}

int Gameloop()
{
	DWORD nowTime = timeGetTime();

	DispUI();
	DrawEffect(bpm);

	switch (phase)
	{
	case CountDown:	//カウントダウン
	{
		if (phaseStartTime == 0)
		{
			phaseStartTime = nowTime;
			gotoxy(20, 13);
			std::cout << "  ";
			gotoxy(61, 13);
			std::cout << "  ";
		}

		if (nowTime - phaseStartTime > ((int)(60.0f / bpm * 500.0f)) * (notesSteps + 1))
		{
			if (countdown < 0)
			{
				phaseStartTime = 0;
				notesSteps = 0;
				phase = Ref;
			}

			if (notesSteps % 2 && countdown >= 0)
			{
				if (countdown > 0)
				{
					if (countdown <= 3)
					{
						gotoxy(41, 13);
						std::cout << countdown;
					}
					else if (countdown > 4)
					{
						gotoxy(37, 13);
						std::cout << "LevelUP!!";
					}
					else
					{
						gotoxy(37, 13);
						std::cout << "         ";
					}
				}
				else if (countdown == 0)
				{
					gotoxy(40, 13);
					std::cout << "GO!";
				}
				countdown--;
				Resource::Playsound((char*)"Resource/hihat.wav");
			}

			notesSteps++;
		}
		break;
	}

	case Ref: //生成
	{
		srand((unsigned int)time(0));
		for (int nowMeasure = 0; nowMeasure < MEASURE_MAX; nowMeasure++)
		{
			int generatedNotes[8];
			int* pGeneratedNotes = &generatedNotes[0];
			CreateNotes(colortype, pGeneratedNotes);
			for (int note = 0; note < 8; note++)
			{
				notesType[note + (nowMeasure * 8)] = generatedNotes[note];

				if (notesType[note + (nowMeasure * 8)] != -1 && note + (nowMeasure * 8) < playMeasures * 8)
				{
					notesOfNumber++;
				}
			}
		}

		//小節設定
		playMeasures = bpm / INCREASE_MEASURES;
		if (playMeasures == 0)
		{
			playMeasures = 1;
		}
		else if (playMeasures > MEASURE_MAX)
		{
			playMeasures = MEASURE_MAX;
		}
		phase = Model;
		break;
	}

	case Model: //手本
	{
		if (phaseStartTime == 0)
		{
			phaseStartTime = nowTime;
			notesSteps = 0;
		}
		if (nowTime - phaseStartTime > ((int)(60.0f / bpm * 500.0f)) * (notesSteps + 1))
		{
			if (nowTime - phaseStartTime > ((int)(60.0f / bpm * 500.0f)) * (notesSteps + 1))
			{
				gotoxy(40, 13);
				std::cout << "   ";
			}
			//エフェクト生成
			switch (notesType[notesSteps])
			{
			case RED:
				Resource::Playsound((char*)"Resource/beep0.wav");
				GenerateEffect(Flash, 25, 7, (EffectColors)RED);
				break;
			case GREEN:
				Resource::Playsound((char*)"Resource/beep1.wav");
				GenerateEffect(Flash, 35, 13, (EffectColors)GREEN);
				break;
			case BLUE:
				Resource::Playsound((char*)"Resource/beep2.wav");
				GenerateEffect(Flash, 45, 7, (EffectColors)BLUE);
				break;
			default:
				break;
			}

			notesSteps++;

			textcolor(WHITE);
			gotoxy(20, 13);
			std::cout << std::setw(2) << (playMeasures * 8 - notesSteps) / 2 + 1;
			gotoxy(61, 13);
			std::cout << std::setw(2) << (playMeasures * 8 - notesSteps) / 2 + 1;

			if (notesSteps >= playMeasures * 8)
			{
				phaseStartTime = 0;
				notesSteps = 0;
				phase = Play;
			}
#ifdef _DEBUG
			gotoxy(1, 21);
			std::cout << nowTime - phaseStartTime;
#endif // _DEBUG

		}
		break;
	}

	case Play: //プレイ		
	{
		//キー入力された時間の更新
		if (phaseStartTime == 0)
		{
			phaseStartTime = nowTime;
			notesSteps = 0;
			missTouchNotes = false;
			isTouchNotes = false;
			timingGrade = Wait;

			while (notesType[notesSteps] == -1 || notesSteps >= MEASURE_MAX * 8)
			{
				notesSteps++;
			}

		}

		//キー入力がされているか
		switch (notesType[notesSteps])
		{
		case RED:
			if (isTouchNotes = GetKeyDown(KEY_RED))
			{
				inputTime = GetKeyDownLastTime(KEY_RED);
			}
			if (GetKeyDown(KEY_GREEN) | GetKeyDown(KEY_BLUE))
			{
				missTouchNotes = true;
			}
			break;
		case GREEN:
			if (isTouchNotes = GetKeyDown(KEY_GREEN))
			{
				inputTime = GetKeyDownLastTime(KEY_GREEN);
			}
			if (GetKeyDown(KEY_RED) | GetKeyDown(KEY_BLUE))
			{
				missTouchNotes = true;
			}
			break;
		case BLUE:
			if (isTouchNotes = GetKeyDown(KEY_BLUE))
			{
				inputTime = GetKeyDownLastTime(KEY_BLUE);
			}
			if (GetKeyDown(KEY_GREEN) | GetKeyDown(KEY_RED))
			{
				missTouchNotes = true;
			}
			break;
		}


		//ノーツタイミングの取得
		DWORD timing = ((int)(60.0f / bpm * 500.0f * (notesSteps + 1)));

		if (nowTime - phaseStartTime > 500)
		{
			textcolor(WHITE);
			gotoxy(20, 13);
			std::cout << "GO";
			gotoxy(61, 13);
			std::cout << "GO";
		}

		//if (missTouchNotes)
		//{
		//	GameplayInit();
		//	phase = CountDown;
		//}

		//キー入力がされている場合
		if (isTouchNotes)
		{
			rawGrade = timing - (inputTime - phaseStartTime);
			int grade = rawGrade;

			if (grade < 0)
			{
				grade = -grade;
			}

			if (grade < TIMING_EXELLENT)
			{
				timingGrade = Exellent;
			}
			else if (grade < TIMING_GREAT)
			{
				timingGrade = Great;
			}
			else if (grade < TIMING_GOOD)
			{
				timingGrade = Good;
			}
			else
			{
				timingGrade = Bad;
			}

			//エフェクト生成
			if (timingGrade != Bad)
			{
				switch (notesType[notesSteps])
				{
				case RED:
					Resource::Playsound((char*)"Resource/beep0.wav");
					GenerateEffect(Slash_Right, 25, 7, (EffectColors)RED);
					break;
				case GREEN:
					Resource::Playsound((char*)"Resource/beep1.wav");
					GenerateEffect(Slash_Right, 35, 13, (EffectColors)GREEN);
					break;
				case BLUE:
					Resource::Playsound((char*)"Resource/beep2.wav");
					GenerateEffect(Slash_Right, 45, 7, (EffectColors)BLUE);
					break;
				}

				//空ノーツを飛ばす
				do
				{
					notesSteps++;
				} while (notesType[notesSteps] == -1 || notesSteps >= MEASURE_MAX * 8);
				score += TIMING_GOOD - grade;
			}
			//判定がBad
			else
			{
				switch (notesType[notesSteps])
				{
				case RED:
					Resource::Playsound((char*)"Resource/beep3.wav");
					break;
				case GREEN:
					Resource::Playsound((char*)"Resource/beep3.wav");
					break;
				case BLUE:
					Resource::Playsound((char*)"Resource/beep3.wav");
					break;
				}
				score += grade - TIMING_GOOD;
			}

			isTouchNotes = false;

#ifdef _DEBUG
			gotoxy(1, 21);
			std::cout << timing << "  ";
			gotoxy(1, 22);
			std::cout << rawGrade << "  ";
			gotoxy(1, 23);
			std::cout << grade << "  ";
			gotoxy(1, 24);
			std::cout << notesSteps;
#endif // _DEBUG

		}

		//ミス判定
		else if (nowTime > phaseStartTime + ((int)(60.0f / bpm * 500.0f * (notesSteps + 2))) || missTouchNotes || timingGrade == Bad)
		{
#ifdef _DEBUG
			debug_int++;
#endif

			inputTime = nowTime;
			timingGrade = Miss;
			if (timingGrade == Bad)
			{
				timingGrade = Bad;
			}
			if (missTouchNotes)
			{
				Resource::Playsound((char*)"Resource/beep3.wav");
			}
			else
			{
				score -= TIMING_GOOD;
			}
			missTouchNotes = false;
			life--;

			if (life <= 0)
			{
				//ゲームオーバー
				TransitionStart(T_WHITE);
				Resource::Playsound((char*)"Resource/gameover.wav");
				return 2;
			}

			//空ノーツを飛ばす
			do
			{
				notesSteps++;
			} while (notesType[notesSteps] == -1 || notesSteps >= MEASURE_MAX * 8);
		}


		//フェーズ進行判定
		if (notesSteps >= playMeasures * 8 && (phaseStartTime + ((int)(60.0f / bpm * 500.0f * playMeasures * 8))) < nowTime)
		{
			phaseStartTime = 0;
			wave++;
			while (notesSteps < playMeasures * 8)
			{
				notesSteps++;
				life--;
			}
			if (wave % INCREASE_LEVEL == 0)
			{
				bpm += INCREASE_BPM;
				countdown = 7;
				countdownTimer = 0;
				notesSteps = 0;
				phaseStartTime = 0;
				phase = CountDown;
			}
			else
			{
				phase = Ref;
			}
		}
		break;
		}
	}
#ifdef _DEBUG
	DispDebug_gameplay();
#endif // _DEBUG

	return 1;
}


void DispUI()
{
	//スコア、BPM表示
	{
		gotoxy(63, 2);
		textcolor(WHITE);
		std::cout << "Score : " << std::setw(10) << score;
		gotoxy(72, 3);
		std::cout << "BPM : " << std::setw(3) << bpm;
	}

	//ライフ表示
	{
		gotoxy(2, 2);
		textcolor(WHITE);
		std::cout << "Life ";
		for (int i = 0; i < LIFE_MAX; i++)
		{
			if (i < life)
			{
				textcolor(RED);
				std::cout << "■";
			}
			else
			{
				textcolor(LIGHTGRAY);
				std::cout << "□";
			}
		}
	}

	//ノーツ判定表記
	{
		gotoxy(26, 25);
		for (int i = -15; i < 16; i++)
		{
			float buf = i;
			if (buf < 0)
			{
				buf = -buf;
			}

			if ((float)TIMING_EXELLENT / (float)TIMING_GOOD > buf / 15.0f)
			{
				textbackground(LIGHTGREEN);
			}
			else if ((float)TIMING_GREAT / (float)TIMING_GOOD > buf / 15.0f)
			{
				textbackground(YELLOW);
			}
			else
			{
				textbackground(RED);
			}
			std::cout << " ";
		}
		textbackground(BLACK);
	}

	//タイミング表記
	{
		if (inputTime + (DWORD)TIMING_TEXT_FADEOUT > timeGetTime())
		{
			//文字表記
			gotoxy(36, 22);
			switch (timingGrade)
			{
			case Good:
				textcolor(LIGHTGRAY);
				std::cout << "   Good!   ";
				break;
			case Great:
				textcolor(LIGHTBLUE);
				std::cout << "  Great!!  ";
				break;
			case Exellent:
				textcolor(MAGENTA);
				std::cout << "Exellent!!!";
				break;
			case Bad:
				textcolor(WHITE);
				std::cout << "    Bad    ";
				break;
			case Miss:
				textcolor(WHITE);
				std::cout << "    Miss   ";
				break;
			case Wait:
				textcolor(WHITE);
				std::cout << "           ";
				break;
			}

			//記号表記
			float posx = rawGrade + TIMING_GOOD;
			if (posx < 0)
			{
				posx = 0;
			}
			else
			{
				posx *= (30.0f / ((float)TIMING_GOOD * 2));
			}

			if (timingGrade != Miss && timingBarPos != (int)posx)
			{
				gotoxy(56 - timingBarPos, 24);
				std::cout << " ";

				timingBarPos = (int)posx;
				if (timingBarPos > 30)
				{
					timingBarPos = 30;
				}
				gotoxy(56 - timingBarPos, 24);
				std::cout << "*";
			}

			gotoxy(36, 23);
			if (timingGrade == Exellent || timingGrade == Wait || timingGrade == Miss)
			{
				std::cout << "           ";
			}
			else
			{
				textcolor(LIGHTGRAY);
				if (rawGrade < 0)
				{
					std::cout << "    Late   ";
				}
				else
				{
					std::cout << "    Fast   ";
				}
			}

		}
		else
		{
			gotoxy(36, 22);
			textcolor(WHITE);
			std::cout << "           ";
			gotoxy(36, 23);
			textcolor(WHITE);
			std::cout << "           ";
		}
	}
}

#ifdef _DEBUG
void DispDebug_gameplay()
{
	gotoxy(1, 2);
	std::cout << debug_int;
	gotoxy(1, 3);
	std::cout << notesSteps;
	gotoxy(1, 4);
	std::cout << inputTime;
	gotoxy(1, 5);
	std::cout << isTouchNotes;
	gotoxy(1, 20);
	std::cout << timeGetTime() - phaseStartTime << "  ";
	gotoxy(1, 25);
	for (int i = 0; i < 32; i++)
	{
		gotoxy(i * 2 + 1 , 1);
		switch (notesType[i])
		{
		case RED:
			textcolor(RED);
			break;
		case GREEN:
			textcolor(GREEN);
			break;
		case BLUE:
			textcolor(BLUE);
			break;
		default:
			textcolor(WHITE);
			break;
		}
		std::cout << "■";
		textcolor(WHITE);
	}
}
#endif // _DEBUG


//void effects()