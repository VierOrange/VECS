#pragma once
#include "VECS/comps/Collider.hpp"
#include "VECS/comps/Health.hpp"
#include "VECS/core/vecs.hpp"

struct CombatSystem : VECS::internal::System
{
	void Update()
	{
		for (VECS::internal::Entity en : mEntities)
		{
			Health &health	   = en.GetComponent<Health>();
			Collider &collider = en.GetComponent<Collider>();
			health.HP -= collider.cn;
			collider.cn = 0;
			if (health.HP <= 0)
				en.Destory();
		}
	}
};
