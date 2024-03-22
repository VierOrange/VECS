#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "VECS/comps/Controllable.hpp"
#include "VECS/comps/Render.hpp"
#include "VECS/core/vecs.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

struct InputSystem : VECS::internal::System
{
  private:
	SDL_Event e;
	void ControlDefault(VECS::internal::Entity en);

  public:
	void Update();
};
inline void InputSystem::ControlDefault(VECS::internal::Entity en)
{
	Render &render = en.GetComponent<Render>();
	switch (e.key.keysym.sym)
	{
	case SDLK_UP:
		render.index = 1;
		break;
	case SDLK_DOWN:
		render.index = 2;
		break;
	case SDLK_LEFT:
		render.index = 3;
		break;
	case SDLK_RIGHT:
		render.index = 4;
		break;
	default:
		render.index = 0;
		break;
	}
}
inline void InputSystem::Update()
{
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			VECS::Exit = true;
			break;
		case SDL_KEYDOWN:
			for (VECS::internal::Entity en : mEntities)
			{
				Controllable &con = en.GetComponent<Controllable>();
				if (con.isControlled)
				{
					switch (con.type)
					{
					default:
						ControlDefault(en);
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}
}
