#pragma once

#include "Resources.h"

#include <SDL.h>

#define m2p 15.0f
#define p2m (1.0f/15.0f)

static int u_sdlToB2(int yValue)
{
	return -1 * (yValue - D_SCREEN_HEIGHT);
}

static int u_b2ToSdl(int yValue)
{
	return -1 * (yValue - D_SCREEN_HEIGHT);	
}

/*
sdl2b2 and b22sdl do the exact same thing. cry if u must but do so quiet
*/

static SDL_Rect u_SdlRectToB2(const SDL_Rect& sdlRect)
{
	SDL_Rect b2Rect;
	b2Rect.x = sdlRect.x + (sdlRect.w / 2);
	b2Rect.y = u_sdlToB2(sdlRect.y - (sdlRect.h / 2));

	b2Rect.w = sdlRect.w;
	b2Rect.h = sdlRect.h;

	return b2Rect;
}

static SDL_Rect u_B2RectToSdl(const SDL_Rect& b2Rect)
{
	SDL_Rect sdlRect;
	sdlRect.x = b2Rect.x - (b2Rect.w / 2);
	sdlRect.y = u_b2ToSdl(b2Rect.y + (b2Rect.h / 2));

	sdlRect.w = b2Rect.w;
	sdlRect.h = b2Rect.h;

	return sdlRect;
}