#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#define COLORS (3)
#define TIMING_TEXT_FADEOUT (500)

void GameplayInit();
int Gameloop();
void DispUI();
void DispDebug_gameplay();

extern int score;
extern int bpm;

#endif // !_GAMEPLAY_H_
