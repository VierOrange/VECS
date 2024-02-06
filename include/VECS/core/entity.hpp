#pragma once
#include "common.hpp"

#include <array>
#include <queue>

class EntityManager
{
	std::queue<Entity> Entities{};
	std::array<Signature, MAX_ENTITIES> Signatures{};
	size_t LiveEntitiesCount{0};

  public:
	EntityManager()
	{
		for (Entity en = 0; en < MAX_ENTITIES; ++en)
			Entities.emplace(en);
	}
	Entity AddEntity()
	{
		assert((LiveEntitiesCount < MAX_ENTITIES) && "invalid entity.");

		Entity newEntity = Entities.front();
		Entities.pop();
		++LiveEntitiesCount;
		return newEntity;
	}
	void RemoveEntity(Entity entity)
	{
		assert((entity < MAX_ENTITIES) && "invalid entity.");

		Entities.emplace(entity);
		Signatures[entity].reset();
		--LiveEntitiesCount;
	}
	void SetSignature(Entity entity, Signature signature)
	{
		assert((entity < MAX_ENTITIES) && "invalid entity.");

		Signatures[entity] = signature;
	}
	Signature GetSignature(Entity entity)
	{
		assert((entity < MAX_ENTITIES) && "invalid entity.");

		return Signatures[entity];
	}
};
