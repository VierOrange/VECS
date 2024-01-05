#pragma once
#include "common.h"

#include <queue>
#include <array>
#include <algorithm>

class EntityManager
{
    std::queue<Entity> Entities{};
    std::array<Signature,MAX_ENTITIES> Signatures{};
	size_t LiveEntitiesCount{0}; 
public:
    EntityManager();
    Entity AddEntity();
    void RemoveEntity(Entity entity);
    void SetSignature(Entity entity,Signature signature);
    Signature GetSignature(Entity entity);
};


