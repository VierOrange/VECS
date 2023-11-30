#include "component.h"

template<typename T>
void ComponentArray<T>::AddComponent(Entity entity, T comp)
{
	assert((mEntityToIndex.find(entity)==mEntityToIndex.end())&&"entity already has this component.");
	mEntityToIndex.insert({entity,mEntityCount});
	mIndexToEntity.insert({mEntityCount,entity})
	mData[mEntityCount] = comp;
	++mEntityCount;
}
template<typename T>
void ComponentArray<T>::RemoveComponent(Entity entity)
{
	assert((mEntityToIndex.find(entity)!=mEntityToIndex.end())&&"entity does not has this component.");
	Entity lastEntity = mIndexToEntity[--mEntityCount];
	if(entity == lastEntity)
	{
		mIndexToEntity.erase(mEntityCount);
	}else
	{
		size_t entityIndex = mEntityToIndex[entity];
		mData[entityIndex] = mData[mEntityCount];
		mEntityToIndex[lastEntity] = entityIndex;
		mIndexToEntity[entityIndex] = lastEntity;
	}
	mEntityToIndex.erase(entity);
}
template<typename T>
void ComponentArray<T>::RemoveEntity(Entity entity)
{
	if(mEntityToIndex.find(entity)!=mEntityToIndex.end())
		RemoveComponent(entity);
}

template<typename T>
T& ComponentArray<T>::GetData(Entity entity)
{
	return mData[entity];	
}

template<typename T>
IComponentArray* ComponentManager::GetComponentArray()
{
	char const * pCompTypeName = typeid(T).name();
	assert((mComponentArrays.find(pCompTypeName)!=mComponentArrays.end())&&"component not be registered.");
	return mComponentArrays[pCompTypeName];
}

template<typename T>
void ComponentManager::RegisterComponent()
{
	char const * pCompTypeName = typeid(T).name();
	assert((mComponentArrays.find(compTypeName)==mComponentArrays.end())&&"component already be registered.");
	IComponentArray* pComponentArray = new ComponentArray<T>;
	++mComponentTypeCount;
	mComponentArrays.insert({pCompTypeName,pComponentArray});
	mComponentTypes.insert({pCompTypeName,mComponentTypeCount});
}

template<typename T>
void ComponentManager::AddComponent(Entity entity,T comp)
{
	GetComponentArray<T>()->AddComponent(entity,comp);	
}

template<typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	GetComponentArray<T>()->RemoveComponent(entity);
}

void ComponentManager::RemoveEntity(Entity entity)
{
	for(auto compArray : mComponentArrays)
		compArray.second->RemoveEntity(entity);
}

template<typename T>
ComponentType ComponentManager::GetComponentType()
{
	char const * pCompTypeName = typeid(T).name();
	assert((mComponentArrays.find(pCompTypeName)!=mComponentArrays.end())&&"component not be registered.");

	return mComponentTypes[pCompTypeName]; 
}

template<typename T>
T& ComponentManager::GetComponent(Entity entity)
{
	return GetComponentArray<T>()->GetData(entity);
}
