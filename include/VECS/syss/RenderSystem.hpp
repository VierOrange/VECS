#pragma once

#include <SDL2/SDL.h>
#include <SDL2/_real_SDL_config.h>
#include <cstdio>
#include <stdio.h>

#include "SDL_surface.h"
#include "SDL_video.h"
#include "VECS/core/vecs.hpp"

struct RenderSystem : VECS::internal::System
{
  private:
	int mWindowsWidth;
	int mWindowsHeight;
	SDL_Window *pWindow;
	SDL_Surface *pSurface;

  public:
	RenderSystem();
	~RenderSystem();
	void Update();
};

inline RenderSystem::RenderSystem()
{
	int mWindowsWidth  = 640;
	int mWindowsHeight = 480;

	pWindow	 = NULL;
	pSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::printf("SDL could not initialize! SDL_Error: %s\n",
					SDL_GetError());
	}
	else
	{
		pWindow = SDL_CreateWindow("VECS",
								   SDL_WINDOWPOS_UNDEFINED,
								   SDL_WINDOWPOS_UNDEFINED,
								   mWindowsWidth,
								   mWindowsHeight,
								   SDL_WINDOW_SHOWN);
		if (pWindow == NULL)
		{
			std::printf("Window could not be initialized! SDL_Error: %s\n",
						SDL_GetError());
		}
		else
		{

			pSurface = SDL_GetWindowSurface(pWindow);
			SDL_FillRect(
				pSurface, NULL, SDL_MapRGB(pSurface->format, 0xFF, 0xFF, 0xFF));
		}
	}
}
inline RenderSystem::~RenderSystem()
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}
inline void RenderSystem::Update() { SDL_UpdateWindowSurface(pWindow); }
