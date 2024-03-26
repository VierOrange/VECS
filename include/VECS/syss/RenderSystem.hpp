#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/_real_SDL_config.h>
#include <cstdio>
#include <stdio.h>
#include <vector>

#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"

#include "VECS/comps/Render.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/core/vecs.hpp"
enum Textures
{
	RED,
	WHITE,
	GOLD,
	SILVER,
	DIRT,
	STONE,
};
enum RenderType
{
	TILE,
	PLAYER,
	UI
};
inline auto cmp = [](VECS::internal::Entity a, VECS::internal::Entity b) {
	return a.GetComponent<Render>().type < b.GetComponent<Render>().type;
};
struct RenderSystem : VECS::internal::System
{
  private:
	int mWindowsWidth;
	int mWindowsHeight;
	SDL_Window *pWindow;
	SDL_Renderer *pRender;
	SDL_Texture *pTexture;
	SDL_Surface *pSurface;

	SDL_Surface *loadSurface(std::string path);
	SDL_Texture *loadTexture(std::string path);
	void setRenderDrawTexture(int texture);
	void renderEntity(VECS::internal::Entity en);

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
inline SDL_Texture *RenderSystem::loadTexture(std::string path)
{

	std::string fullPath	   = VECS::AssetsPath + path;
	SDL_Texture *loadedTexture = NULL;
	SDL_Surface *loadedSurface = IMG_Load(fullPath.c_str());
	if (loadedSurface == NULL)
	{
		std::printf("SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		loadedTexture = SDL_CreateTextureFromSurface(pRender, loadedSurface);
		if (loadedTexture == NULL)
		{
			std::printf("SDL_Error: %s\n", SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return loadedTexture;
}
inline void RenderSystem::setRenderDrawTexture(int texture)
{
	switch (texture)
	{
	case GOLD:
		SDL_SetRenderDrawColor(pRender, 0xFF, 0xDF, 0x00, 0xFF);
		break;
	case SILVER:
		SDL_SetRenderDrawColor(pRender, 0xC1, 0xBF, 0xBD, 0xFF);
		break;
	case DIRT:
		SDL_SetRenderDrawColor(pRender, 0x9B, 0x76, 0x53, 0xFF);
		break;
	case STONE:
		SDL_SetRenderDrawColor(pRender, 0x88, 0x8C, 0x8D, 0xFF);
		break;
	default:
		SDL_SetRenderDrawColor(pRender, 0x88, 0x8C, 0x8D, 0xFF);
		break;
	}
}
inline void RenderSystem::renderEntity(VECS::internal::Entity en)
{
	Render &render		 = en.GetComponent<Render>();
	Transform &transform = en.GetComponent<Transform>();

	SDL_Rect fillRect = {static_cast<int>(transform.x),
						 static_cast<int>(transform.y),
						 render.w,
						 render.h};
	setRenderDrawTexture(render.texture);
	SDL_RenderFillRect(pRender, &fillRect);
}
inline RenderSystem::RenderSystem()
{
	int mWindowsWidth  = 640;
	int mWindowsHeight = 480;

	pWindow	 = NULL;
	pRender	 = NULL;
	pTexture = NULL;
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
			pRender = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
			if (pRender == NULL)
			{
				std::printf("SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_SetRenderDrawColor(pRender, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if ((IMG_Init(imgFlags) & imgFlags) == 0)
				{
					std::printf("SDL_Error: %s\n", SDL_GetError());
				}
			}
		}
	}
}
inline RenderSystem::~RenderSystem()
{
	SDL_DestroyTexture(pTexture);
	SDL_DestroyRenderer(pRender);
	SDL_DestroyWindow(pWindow);

	pTexture = NULL;
	pRender	 = NULL;
	pWindow	 = NULL;

	IMG_Quit();
	SDL_Quit();
}
inline void RenderSystem::Update()
{
	SDL_SetRenderDrawColor(pRender, 0x8C, 0x59, 0x15, 0xFF);
	SDL_RenderClear(pRender);
	std::set<VECS::internal::Entity, decltype(cmp)> zBuffer;
	for (VECS::internal::Entity en : mEntities)
	{
		Render &render = en.GetComponent<Render>();
		switch (render.type)
		{
		case PLAYER:
			zBuffer.insert(en);
			break;
		default:
			renderEntity(en);
			break;
		}
	}
	for (VECS::internal::Entity en : zBuffer){
			renderEntity(en);
	}
	SDL_RenderPresent(pRender);
}
