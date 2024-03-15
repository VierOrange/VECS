#pragma once
#include "VECS/comps/Health.hpp"
#include "VECS/core/vecs.hpp"

struct HurtingSystem : VECS::internal::System
{
	void Update()
	{
		for (VECS::internal::Entity en : mEntities)
		{
			Health &health = en.GetComponent<Health>();
			if (health.HP > 0)
			{
				health.HP -= 10;
			}
		}
	}
};
