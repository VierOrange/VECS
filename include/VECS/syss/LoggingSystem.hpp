#pragma once
#include <cstdio>

#include "VECS/comps/Health.hpp"
#include "VECS/core/vecs.hpp"

struct LoggingSystem : VECS::internal::System
{
	void Update()
	{
		for (VECS::internal::Entity en : mEntities)
		{
			Health &health = en.GetComponent<Health>();
			std::printf("Health: %d/%d\n",health.HP,health.maxHP);
		}
	}
};
