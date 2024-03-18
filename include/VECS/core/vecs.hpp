#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>
#include <set>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#define MAX_ENTITIES 5000
#define MAX_COMPONENTS 32

using ID			= std::uint32_t;
using ComponentType = std::uint8_t;
using Signature		= std::bitset<MAX_COMPONENTS>;

namespace VECS
{
namespace internal
{
class World;

class Entity
{
	ID mID;

  public:
	static std::weak_ptr<World> pWorld;

	Entity() = default;
	Entity(ID id) : mID(id) {}
	operator ID() const { return mID; }

	template <typename T>
	void AddComponent(T comp);

	template <typename T>
	T &GetComponent();

	template <typename T>
	void RemoveComponent();
};

class EntityManager
{
	std::queue<Entity> mEntities{};
	std::array<Signature, MAX_ENTITIES> mSignatures{};
	size_t LiveEntitiesCount{0};

  public:
	EntityManager();
	Entity AddEntity();
	void RemoveEntity(Entity entity);
	void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);
};

class IComponentArray
{
  public:
	virtual ~IComponentArray()				 = default;
	virtual void RemoveEntity(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
	static auto entityHash()
	{
		return [](const Entity &entity) { return std::hash<ID>{}(entity); };
	}
	std::unordered_map<Entity, size_t, decltype(entityHash())> mEntityToIndex{};
	std::unordered_map<size_t, Entity> mIndexToEntity{};
	std::array<T, MAX_ENTITIES> mData{};
	size_t mEntityCount{0};

  public:
	void AddComponent(Entity entity, T comp);
	void RemoveComponent(Entity entity);
	void RemoveEntity(Entity entity) override;
	T &GetData(Entity entity);
};

class ComponentManager
{
	std::unordered_map<char const *, std::shared_ptr<IComponentArray>>
		mComponentArrays{};
	std::unordered_map<char const *, ComponentType> mComponentTypes{};
	size_t mComponentTypeCount{0};

	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray();

  public:
	template <typename T>
	void RegisterComponent();

	template <typename T>
	void AddComponent(Entity entity, T comp);

	template <typename T>
	void RemoveComponent(Entity entity);

	void RemoveEntity(Entity entity);

	template <typename T>
	ComponentType GetComponentType();

	template <typename T>
	T &GetComponent(Entity entity);
};

struct System
{
	std::set<Entity> mEntities{};
};

class SystemManager
{
	std::unordered_map<char const *, Signature> mSignatures{};
	std::unordered_map<char const *, std::shared_ptr<System>> mSystems{};

  public:
	template <typename T>
	void RegisterSystem();

	template <typename T>
	std::shared_ptr<T> GetSystem();

	template <typename T>
	void SetSignature(Signature signature);

	void RemoveEntity(Entity entity);

	void EntitySignatureChanged(Entity entity, Signature signature);
};

class World
{
	std::unique_ptr<EntityManager> pEntityManager;
	std::unique_ptr<ComponentManager> pComponentManager;
	std::unique_ptr<SystemManager> pSystemManager;

  public:
	World();
	// Register
	template <typename T>
	void RegisterComponent();
	template <typename T>
	void RegisterSystem();
	// Add
	Entity Create();
	template <typename T>
	void AddComponent(Entity entity, T component);
	// Set
	template <typename T>
	void SetSystemSignature(Signature signature);
	// Get
	template <typename T>
	T &GetComponent(Entity entity);

	template <typename T>
	std::shared_ptr<T> GetSystem();

	template <typename T>
	ComponentType GetComponentType();
	// Remove
	template <typename T>
	void RemoveComponent(Entity entity);
	void Destory(Entity entity);
};

inline EntityManager::EntityManager()
{
	for (ID id = 0; id < MAX_ENTITIES; ++id)
		mEntities.emplace(id);
}
inline Entity EntityManager::AddEntity()
{
	assert((LiveEntitiesCount < MAX_ENTITIES) && "invalid entity.");

	Entity newEntity = mEntities.front();
	mEntities.pop();
	++LiveEntitiesCount;
	return newEntity;
}
inline void EntityManager::RemoveEntity(Entity entity)
{
	assert((entity < MAX_ENTITIES) && "invalid entity.");

	mEntities.emplace(entity);
	mSignatures[entity].reset();
	--LiveEntitiesCount;
}
inline void EntityManager::SetSignature(Entity entity, Signature signature)
{
	assert((entity < MAX_ENTITIES) && "invalid entity.");

	mSignatures[entity] = signature;
}
inline Signature EntityManager::GetSignature(Entity entity)
{
	assert((entity < MAX_ENTITIES) && "invalid entity.");

	return mSignatures[entity];
}

template <typename T>
void ComponentArray<T>::AddComponent(Entity entity, T comp)
{
	assert((mEntityToIndex.find(entity) == mEntityToIndex.end()) &&
		   "entity already has this component.");
	mEntityToIndex.insert({entity, mEntityCount});
	mIndexToEntity.insert({mEntityCount, entity});
	mData[mEntityCount] = comp;
	++mEntityCount;
}
template <typename T>
void ComponentArray<T>::RemoveComponent(Entity entity)
{
	assert((mEntityToIndex.find(entity) != mEntityToIndex.end()) &&
		   "entity does not has this component.");
	Entity lastEntity = mIndexToEntity[--mEntityCount];

	if (entity == lastEntity)
	{
		mIndexToEntity.erase(mEntityCount);
	}
	else
	{
		size_t entityIndex			= mEntityToIndex[entity];
		mData[entityIndex]			= mData[mEntityCount];
		mEntityToIndex[lastEntity]	= entityIndex;
		mIndexToEntity[entityIndex] = lastEntity;
	}
	mEntityToIndex.erase(entity);
}
template <typename T>
void ComponentArray<T>::RemoveEntity(Entity entity)
{
	if (mEntityToIndex.find(entity) != mEntityToIndex.end())
		RemoveComponent(entity);
}

template <typename T>
T &ComponentArray<T>::GetData(Entity entity)
{
	return mData[entity];
}

///////////////////////////////////////////////////////////////

template <typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{
	char const *compTypeName = typeid(T).name();
	assert((mComponentArrays.find(compTypeName) != mComponentArrays.end()) &&
		   "component not be registered.");
	return std::static_pointer_cast<ComponentArray<T>>(
		mComponentArrays[compTypeName]);
}
template <typename T>
void ComponentManager::RegisterComponent()
{
	char const *compTypeName = typeid(T).name();
	assert((mComponentArrays.find(compTypeName) == mComponentArrays.end()) &&
		   "component already be registered.");
	std::shared_ptr<IComponentArray> pComponentArray =
		std::make_shared<ComponentArray<T>>();
	++mComponentTypeCount;
	mComponentArrays.insert({compTypeName, pComponentArray});
	mComponentTypes.insert({compTypeName, mComponentTypeCount});
}

template <typename T>
void ComponentManager::AddComponent(Entity entity, T comp)
{
	GetComponentArray<T>()->AddComponent(entity, comp);
}

template <typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	GetComponentArray<T>()->RemoveComponent(entity);
}

inline void ComponentManager::RemoveEntity(Entity entity)
{
	for (auto compArray : mComponentArrays)
		compArray.second->RemoveEntity(entity);
}

template <typename T>
ComponentType ComponentManager::GetComponentType()
{
	char const *compTypeName = typeid(T).name();
	assert((mComponentArrays.find(compTypeName) != mComponentArrays.end()) &&
		   "component not be registered.");

	return mComponentTypes[compTypeName];
}

template <typename T>
T &ComponentManager::GetComponent(Entity entity)
{
	return GetComponentArray<T>()->GetData(entity);
}

template <typename T>
void SystemManager::RegisterSystem()
{
	char const *sysTypeName = typeid(T).name();
	assert(mSystems.find(sysTypeName) == mSystems.end() &&
		   "system registered twice.");
	auto system = std::make_shared<T>();
	mSystems.insert({sysTypeName, system});
}
template <typename T>
std::shared_ptr<T> SystemManager::GetSystem()
{
	char const *sysTypeName = typeid(T).name();
	assert(mSystems.find(sysTypeName) != mSystems.end() &&
		   "system not registered.");
	return std::static_pointer_cast<T>(mSystems[sysTypeName]);
}

template <typename T>
void SystemManager::SetSignature(Signature signature)
{

	char const *sysTypeName = typeid(T).name();
	assert(mSystems.find(sysTypeName) != mSystems.end() && "unknown system.");
	mSignatures[sysTypeName] = signature;
}

inline void SystemManager::RemoveEntity(Entity entity)
{
	for (auto const &pair : mSystems)
	{
		pair.second->mEntities.erase(entity);
	}
}
inline void SystemManager::EntitySignatureChanged(Entity entity,
												  Signature signature)
{
	for (auto const &pair : mSignatures)
	{
		if ((signature & pair.second) == signature)
			mSystems[pair.first]->mEntities.insert(entity);
		else
			mSystems[pair.first]->mEntities.erase(entity);
	}
}

inline World::World()
{
	pEntityManager	  = std::make_unique<EntityManager>();
	pComponentManager = std::make_unique<ComponentManager>();
	pSystemManager	  = std::make_unique<SystemManager>();
}
template <typename T>
void World::RegisterComponent()
{
	pComponentManager->RegisterComponent<T>();
}

template <typename T>
void World::RegisterSystem()
{
	pSystemManager->RegisterSystem<T>();
}

template <typename T>
std::shared_ptr<T> World::GetSystem()
{
	return pSystemManager->GetSystem<T>();
}
inline Entity World::Create() { return pEntityManager->AddEntity(); }

template <typename T>
void World::AddComponent(Entity entity, T component)
{
	Signature newSignature = pEntityManager->GetSignature(entity);
	pComponentManager->AddComponent<T>(entity, component);
	newSignature.set(pComponentManager->GetComponentType<T>(), 1);
	pEntityManager->SetSignature(entity, newSignature);
	pSystemManager->EntitySignatureChanged(entity, newSignature);
}
template <typename T>
void World::SetSystemSignature(Signature signature)
{
	pSystemManager->SetSignature<T>(signature);
}
template <typename T>
T &World::GetComponent(Entity entity)
{
	return pComponentManager->GetComponent<T>(entity);
}

template <typename T>
ComponentType World::GetComponentType()
{
	return pComponentManager->GetComponentType<T>();
}
template <typename T>
void World::RemoveComponent(Entity entity)
{
	Signature newSignature = pEntityManager->GetSignature(entity);
	pComponentManager->RemoveComponent<T>(entity);
	newSignature.set(pComponentManager->GetComponentType<T>(), 0);
	pEntityManager->SetSignature(entity, newSignature);
	pSystemManager->EntitySignatureChanged(entity, newSignature);
}
inline void World::Destory(Entity entity)
{
	pEntityManager->RemoveEntity(entity);
}

inline std::weak_ptr<World> Entity::pWorld;

template <typename T>
inline void Entity::AddComponent(T comp)
{
	assert(!pWorld.expired() && "VECS not initialized");
	pWorld.lock()->AddComponent<T>(mID, comp);
}

template <typename T>
inline T &Entity::GetComponent()
{
	assert(!pWorld.expired() && "VECS not initialized");
	return pWorld.lock()->GetComponent<T>(mID);
}

template <typename T>
void Entity::RemoveComponent()
{
	assert(!pWorld.expired() && "VECS not initialized");
	pWorld.lock()->RemoveComponent<T>(mID);
}
} // namespace internal
extern std::shared_ptr<internal::World> WorldPtr;
extern bool Exit;
void Init();
void Build();
void Run();
} // namespace VECS
