#pragma once
#include "common.hpp"

#include <unordered_map>
#include <typeinfo>
#include <memory>
#include <iostream>


class IComponentArray
{
public:
    virtual ~IComponentArray()=default;
	virtual void RemoveEntity(Entity entity)=0;
	virtual void PrintStorageInfo()=0;
};

template <typename T>
class ComponentArray:public IComponentArray
{
    std::unordered_map<Entity, size_t> mEntityToIndex{};
    std::unordered_map<size_t, Entity> mIndexToEntity{};
    std::array<T,MAX_ENTITIES>mData{};
    size_t mEntityCount{0};
public:
	void AddComponent(Entity entity,T comp);
	void RemoveComponent(Entity entity);
	void RemoveEntity(Entity entity)override;
	void PrintStorageInfo()override;
	T& GetData(Entity entity);
};

class ComponentManager
{
    std::unordered_map<char const *, IComponentArray *> mComponentArrays{};
    std::unordered_map<char const *, ComponentType> mComponentTypes{};
    size_t mComponentTypeCount{0};

    template<typename T>
    ComponentArray<T>* GetComponentArray();

public:
	void PrintComponents();

    template<typename T>
    void RegisterComponent();

    template<typename T>
    void AddComponent(Entity entity,T comp);

    template<typename T>
    void RemoveComponent(Entity entity);

    void RemoveEntity(Entity entity);

    template<typename T>
    ComponentType GetComponentType();

    template<typename T>
    T& GetComponent(Entity entity);
};

template<typename T>
void ComponentArray<T>::AddComponent(Entity entity, T comp)
{
	assert((mEntityToIndex.find(entity)==mEntityToIndex.end())&&"entity already has this component.");
	mEntityToIndex.insert({entity,mEntityCount});
	mIndexToEntity.insert({mEntityCount,entity});
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
void ComponentArray<T>::PrintStorageInfo()
{
	for(auto const & en:mEntityToIndex)
		std::cout<<"["<<en.second<<"]:"<<"Entity "<<en.first<<std::endl;
}

template<typename T>
T& ComponentArray<T>::GetData(Entity entity)
{
	return mData[entity];	
}

///////////////////////////////////////////////////////////////

void ComponentManager::PrintComponents()
{
	for(auto const & compArray:mComponentArrays)
	{
		std::cout<<compArray.first+1<<":\n";
		compArray.second->PrintStorageInfo();
	}
}

template<typename T>
ComponentArray<T>* ComponentManager::GetComponentArray()
{
	char const * pCompTypeName = typeid(T).name();
	assert((mComponentArrays.find(pCompTypeName)!=mComponentArrays.end())&&"component not be registered.");
	return reinterpret_cast<ComponentArray<T>*>(mComponentArrays[pCompTypeName]);
}
template<typename T>
void ComponentManager::RegisterComponent()
{
	char const * pCompTypeName = typeid(T).name();
	assert((mComponentArrays.find(pCompTypeName)==mComponentArrays.end())&&"component already be registered.");
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

