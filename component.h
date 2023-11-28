#include "ecs.h"
#include <unordered_map>

class IComponentArray
{
public:
    virtual ~IComponentArray();
};

template <typename T>
class ComponentArray:IComponentArray
{
    std::unordered_map<Entity, size_t> entityToIndex{};
    std::unordered_map<size_t, Entity> indexToEntity{};
    std::array<T>_data(MAX_ENTITIES);
    size_t entityCount;
public:
    void addData(Entity entity, T component);
    void removeData(Entity entity);
    T& getData(Entity entity);
};

class ComponentManager
{
    std::unordered_map<char const *,IComponentArray *> ComponentArrays{};
    std::unordered_map<char const *,ComponentType> ComponentTypes{};
    size_t ComponentTypesCount;
    template<typename T>
    T* GetComponentArray();
public:
    template<typename T>
    ComponentType GetComponentType();
    template<typename T>
    T* GetComponent(Entity entity);
    template<typename T>
    void RegisterComponent();
    template<typename T>
    void AddComponent(Entity entity);
    template<typename T>
    void RemoveComponent(Entity entity);
    void RemoveEntity(Entity entity);
};