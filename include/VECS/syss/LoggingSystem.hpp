#pragma once
#include <cstdio>

#include "VECS/comps/Controllable.hpp"
#include "VECS/comps/Render.hpp"
#include "VECS/core/vecs.hpp"

struct LoggingSystem : VECS::internal::System
{
	void Update()
	{
		for (VECS::internal::Entity en : mEntities)
		{
			Render &render	  = en.GetComponent<Render>();
			Controllable &con = en.GetComponent<Controllable>();
			std::printf("Render: %d %d\n",render.type ,render.index );
			std::printf("Controllable: %d %d\n",con.type ,con.isControlled );
		}
	}
};
