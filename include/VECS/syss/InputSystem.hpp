#pragma once

#include "SDL_events.h"
#include "VECS/core/vecs.hpp"
#include <SDL2/SDL.h>

struct InputSystem : VECS::internal::System
{
  private:
	SDL_Event e;

  public:
	void Update();
};
inline void InputSystem::Update()
{
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			VECS::Exit = true;
			break;
		default:
			break;
		}
	}
}
