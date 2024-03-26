#pragma once

#include "VECS/comps/Velocity.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/core/vecs.hpp"

struct PhysicsSystem : VECS::internal::System
{
	void Update(float dt)
	{
		for (VECS::internal::Entity en : mEntities)
		{
			Velocity &velocity = en.GetComponent<Velocity>();
			Transform &transform = en.GetComponent<Transform>();
			transform.x += velocity.vx * dt;
			transform.y += velocity.vy * dt;
		}
	}
};
