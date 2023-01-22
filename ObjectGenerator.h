#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>
#include "CollidedObject.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

class ObjectGenerator
{
public:

	static int getRandomXPosition()
	{
		int positionsX[4] = { 135, 259, 386, 512 };

		int index = rand() % 4;

		while (index == lastPosition)
		{
			index = rand() % 4;
		}
		lastPosition = index;

		return positionsX[index];

	}

	static int lastPosition;

private:

};