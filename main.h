#ifndef _MAIN_H_
#define _MAIN_H_

#include "windows.h"
#include "conioex.h"

void Init();
void Update();


#ifdef _DEBUG
	void DispDebug();
#endif // !_DEBUG

#endif // !_MAIN_H_