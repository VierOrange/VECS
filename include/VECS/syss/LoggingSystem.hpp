#pragma once
#include <cstdio>

#include "VECS/comps/Controllable.hpp"
#include "VECS/comps/Render.hpp"
#include "VECS/comps/Velocity.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/core/vecs.hpp"

struct LoggingSystem : VECS::internal::System
{
	float interval;
	void Update(float dt)
	{
		for (VECS::internal::Entity en : mEntities)
		{
			interval += dt;
			if (interval > 1)
			{
				interval			 = 0;
				Transform &transform = en.GetComponent<Transform>();
			Velocity &velocity = en.GetComponent<Velocity>();
				std::printf("x: %f\ty: %f\tvx: %d\tvy: %d\n",
							transform.x,
							transform.y,
							velocity.vx,
							velocity.vy);
			}
		}
	}
};
