#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <iostream>

FILE* musicResource[];

namespace Resource
{
	void Playsound(char* filename);
	void Init();
}

void SoundPlayer(char* filename);

#endif // !_RESOURCE_H_