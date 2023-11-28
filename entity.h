#include "ecs.h"

#include <queue>
#include <array>

class EntityManager
{
    std::queue<Entity> Entities{};
    std::array<Signature,MAX_ENTITIES> Signatures{};
public:
    EntityManager();
    Entity AddEntity();
    void RemoveEntity(Entity entity);
    void SetSignature(Entity entity,Signature signature);
    Signature GetSignature(Entity entity);
};


