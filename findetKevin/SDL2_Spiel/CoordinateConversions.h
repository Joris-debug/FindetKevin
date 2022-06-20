#pragma once

#include "Resources.h"

namespace utils
{
	float sdlToB2(float yValue)
	{
		return -1 * (yValue - D_SCREEN_HEIGHT);
	}

	float b2Tosdl(float yValue)
	{
		return -1 * (yValue + D_SCREEN_HEIGHT);
	}
}