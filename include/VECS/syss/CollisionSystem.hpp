#pragma once
#include "VECS/comps/Collider.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/core/vecs.hpp"

struct CollisionSystem : VECS::internal::System
{
  private:
	bool testCollision(float dy1,
					   float dy2,
					   float dx1,
					   float dx2,
					   float sy1,
					   float sy2,
					   float sx1,
					   float sx2)
	{
		if (dy2 < sy1 || dy1 > sy2)
			return false;
		if (dx2 < sx1 || dx1 > sx2)
			return false;
		return true;
	}

  public:
	void Update()
	{
		std::set<VECS::internal::Entity> dynamicEntities;
		for (VECS::internal::Entity en : mEntities)
		{
			Collider &collider = en.GetComponent<Collider>();
			if (collider.ct == DYNAMIC)
				dynamicEntities.insert(en);
		}

		for (VECS::internal::Entity den : dynamicEntities)
		{
			Collider &dcoll	 = den.GetComponent<Collider>();
			Transform &dtran = den.GetComponent<Transform>();
			float dx1		 = dtran.x;
			float dx2		 = dtran.x + dcoll.cw;
			float dy1		 = dtran.y;
			float dy2		 = dtran.y + dcoll.ch;

			for (VECS::internal::Entity en : mEntities)
			{
				Collider &coll	= en.GetComponent<Collider>();
				Transform &tran = en.GetComponent<Transform>();
				float sx1		= tran.x;
				float sx2		= tran.x + coll.cw;
				float sy1		= tran.y;
				float sy2		= tran.y + coll.ch;
				if (testCollision(dx1, dx2, dy1, dy2, sx1, sx2, sy1, sy2) &&
					den != en)
				{
					++dcoll.cn;
					++coll.cn;
				}
			}
		}
	}
};
