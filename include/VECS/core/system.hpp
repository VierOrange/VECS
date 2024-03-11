#pragma once
#include "common.hpp"

#include <any>
#include <cassert>
#include <memory>
#include <set>
#include <unordered_map>

struct System
{
	std::set<Entity> mEntities{};
	virtual void Update();
};

class SystemManager
{
	std::unordered_map<char const *, Signature> mSignatures{};
	std::unordered_map<char const *, std::shared_ptr<System>> mSystems{};

  public:
	template <typename T>
	std::shared_ptr<T> RegisterSystem();

	template <typename T>
	void SetSignature(Signature signature);

	void RemoveEntity(Entity entity);

	void EntitySignatureChanged(Entity entity, Signature signature);
};

template <typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
	char const *typeName = typeid(T).name();
	assert(mSystems.find(typeName) == mSystems.end() &&
		   "system registered twice.");
	auto system = std::make_shared<T>();
	mSystems.insert(typeName, system);
	return system;
}

template <typename T>
void SystemManager::SetSignature(Signature signature)
{

	char const *typeName = typeid(T).name();
	assert(mSystems.find(typeName) != mSystems.end() &&
		   "unknown system.");
	mSignatures[typeName]=signature;
}

inline void SystemManager::RemoveEntity(Entity entity)
{
		for(auto const & pair:mSystems)
		{
			pair.second->mEntities.erase(entity);	
		}
}
inline void SystemManager::EntitySignatureChanged(Entity entity,Signature signature)
{
		for(auto const & pair:mSignatures)
		{
			if((signature & pair.second)==signature)		
					mSystems[pair.first]->mEntities.insert(entity);
			else
					mSystems[pair.first]->mEntities.erase(entity);
		}
}
