#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>

#define CONIOEX
#include "conioex.h"

#define MAX_RESOURCE (32)

std::string filename_Sound[MAX_RESOURCE];
int handle_Sound[MAX_RESOURCE] = {};

namespace Resource
{
	void Init()
	{
		for (int i = 0; i < MAX_RESOURCE; i++)
		{
			handle_Sound[i] = 0;
		}
	}

	void Playsound(char* filename)
	{
		bool found = false;
		int handle = 0;

		for (int i = 0; i < MAX_RESOURCE; i++)
		{
			if (filename_Sound[i].compare(filename) == 0)
			{
				handle = handle_Sound[i];
				found = true;
				break;
			}
		}

		if (found == false)
		{
			for (int i = 0; i < MAX_RESOURCE; i++)
			{
				if (handle_Sound[i] == 0)
				{
					int handleBuf = 0;
					try
					{
						handleBuf = opensound((char*)filename);
					}
					catch (...) {}
					filename_Sound[i] = filename;
					handle_Sound[i] = handleBuf;
					handle = handleBuf;
					break;
				}
			}
		}

		playsound(handle, 0);
	}
}
