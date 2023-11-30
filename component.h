#include "ecs.h"

#include <unordered_map>
#include <typeinfo>

class IComponentArray
{
public:
    virtual ~IComponentArray();
};

template <typename T>
class ComponentArray:IComponentArray
{
    std::unordered_map<Entity, size_t> mEntityToIndex{};
    std::unordered_map<size_t, Entity> mIndexToEntity{};
    std::array<T,MAX_ENTITIES>mData{};
    size_t mEntityCount{0};
public:
	void AddComponent(Entity entity,T comp);
	void RemoveComponent(Entity entity);
	void RemoveEntity(Entity entity);
	T& GetData(Entity entity);
};

class ComponentManager
{
    std::unordered_map<char const *, IComponentArray *> mComponentArrays{};
    std::unordered_map<char const *, ComponentType> mComponentTypes{};
    size_t mComponentTypeCount{0};

    template<typename T>
    IComponentArray* GetComponentArray();

public:

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
