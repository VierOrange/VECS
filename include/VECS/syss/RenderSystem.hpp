#pragma once

#include <SDL2/SDL.h>
#include <SDL2/_real_SDL_config.h>
#include <cstdio>
#include <stdio.h>

#include "SDL_surface.h"
#include "SDL_video.h"
#include "VECS/comps/Render.hpp"
#include "VECS/core/vecs.hpp"
enum KeyPressSurfaces
{
	SURFACE_DEFAULT,
	SURFACE_KEY_UP,
	SURFACE_KEY_DOWN,
	SURFACE_KEY_LEFT,
	SURFACE_KEY_RIGHT,
	SURFACE_TOTAL,
};

struct RenderSystem : VECS::internal::System
{
  private:
	int mWindowsWidth;
	int mWindowsHeight;
	SDL_Window *pWindow;
	SDL_Surface *pSurface;
	SDL_Surface *pKeyPressSurfaces[SURFACE_TOTAL];

	SDL_Surface *loadSurface(std::string path);

  public:
	RenderSystem();
	~RenderSystem();
	void Update();
};
inline SDL_Surface *RenderSystem::loadSurface(std::string path)
{
	std::string fullPath	   = VECS::AssetsPath + path;
	SDL_Surface *loadedSurface = SDL_LoadBMP(fullPath.c_str());
	if (loadedSurface == NULL)
	{
		std::printf("%s\n", SDL_GetError());
	}
	else
	{
		loadedSurface = SDL_ConvertSurface(loadedSurface, pSurface->format, 0);
		if (loadedSurface == NULL)
		{
			std::printf("%s\n", SDL_GetError());
		}
	}
	// assert(loadedSurface != NULL && SDL_GetError());
	// assert(loadedSurface != NULL && "Loading surface failed.");
	return loadedSurface;
}

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
	pKeyPressSurfaces[SURFACE_DEFAULT]	 = loadSurface("default_surface.bmp");
	pKeyPressSurfaces[SURFACE_KEY_UP]	 = loadSurface("up_surface.bmp");
	pKeyPressSurfaces[SURFACE_KEY_DOWN]	 = loadSurface("down_surface.bmp");
	pKeyPressSurfaces[SURFACE_KEY_LEFT]	 = loadSurface("left_surface.bmp");
	pKeyPressSurfaces[SURFACE_KEY_RIGHT] = loadSurface("right_surface.bmp");
}
inline RenderSystem::~RenderSystem()
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}
inline void RenderSystem::Update()
{
//	SDL_Rect stretchRect;
//	stretchRect.x = 0;
//	stretchRect.y = 0;
//	stretchRect.w = mWindowsWidth;
//	stretchRect.h = mWindowsHeight;
	for (VECS::internal::Entity en : mEntities)
	{
		Render &render = en.GetComponent<Render>();
	//	SDL_BlitSurface(pKeyPressSurfaces[render.index], NULL, pSurface, NULL);
	SDL_BlitScaled(
			pKeyPressSurfaces[render.index], NULL, pSurface, NULL);
	}

	SDL_UpdateWindowSurface(pWindow);
}
