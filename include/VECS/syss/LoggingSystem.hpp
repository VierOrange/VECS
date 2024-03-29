#pragma once
#include <cstdio>

#include "VECS/comps/Controllable.hpp"
#include "VECS/comps/Render.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/comps/Velocity.hpp"
#include "VECS/core/vecs.hpp"

struct LoggingSystem : VECS::internal::System
{
	float mInterval;
	int mFramesCount;
	void Update(float dt)
	{
		mInterval += dt;
		++mFramesCount;
		if (mInterval > 1)
		{
			mInterval = 0;
			std::printf("[FPS]: %d\t[Entity Count]: %d\n",
						mFramesCount,
						int(mEntities.size()));
			mFramesCount = 0;
		}
		//for (VECS::internal::Entity en : mEntities)
		//{
		//}
	}
};
