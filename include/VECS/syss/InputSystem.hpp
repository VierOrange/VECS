#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "VECS/comps/Controllable.hpp"
#include "VECS/comps/Render.hpp"
#include "VECS/comps/Velocity.hpp"
#include "VECS/core/vecs.hpp"
#include <SDL2/SDL.h>
#include <span>

enum ControlSet
{
	CPLAYER,
	CUI
};
struct InputSystem : VECS::internal::System
{
  private:
	SDL_Event e;
	void ControlDefault(VECS::internal::Entity en);
	void ControlPlayer(VECS::internal::Entity en);

  public:
	void Update();
};
inline void InputSystem::ControlDefault(VECS::internal::Entity en) {}
inline void InputSystem::ControlPlayer(VECS::internal::Entity en)
{
			Velocity &velocity = en.GetComponent<Velocity>();
	switch (e.key.keysym.sym)
	{
	case SDLK_UP:
		if (velocity.vx != 0)
		{
			velocity.vy = -std::abs(velocity.vx);
			velocity.vx = 0;
		}
		break;
	case SDLK_DOWN:
		if (velocity.vx != 0)
		{
			velocity.vy = std::abs(velocity.vx);
			velocity.vx = 0;
		}
		break;
	case SDLK_LEFT:
		if (velocity.vy != 0)
		{
			velocity.vx = -std::abs(velocity.vy);
			velocity.vy = 0;
		}
		break;
	case SDLK_RIGHT:
		if (velocity.vy != 0)
		{
			velocity.vx = std::abs(velocity.vy);
			velocity.vy = 0;
		}
		break;
	default:
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
					case CPLAYER:
						ControlPlayer(en);
						break;
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
