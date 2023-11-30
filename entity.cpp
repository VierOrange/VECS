#include "entity.h"

EntityManager::EntityManager()
{
	for(Entity en = 0;en<MAX_ENTITIES;++en)
		Entities.emplace(en);
}
Entity EntityManager::AddEntity()
{
	assert((LiveEntitiesCount>=MAX_ENTITIES)&&"invalid entity.");
	
	Entity newEntity = Entities.font();
	Entities.pop();
	++LiveEntitiesCount;
	return newEntity;
}
void EntityManager::RemoveEntity(Entity entity)
{
	assert((entity<MAX_ENTITIES)&&"invalid entity.");

	Entities.emplace(entity);	
	Signatures[entity].reset();
	--LiveEntitiesCount;
}
void EntityManager::SetSignature(Entity entity,Signature signature)
{
	assert((entity<MAX_ENTITIES)&&"invalid entity.");

	Signatures[entity] = signature;
}
Signature EntityManager::GetSignature(Entity entity)
{
	assert((entity<MAX_ENTITIES)&&"invalid entity.");

	return Signatures[entity];
}
