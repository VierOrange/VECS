#pragma once
#include "common.h"

#include <set>
#include <unordered_map>

struct System
{
    std::set<Entity> RuledEnities{};
    virtual void Update();
};

class SystemManager
{
    std::unordered_map<char const *,Signature> systemSignatures{};
    std::unordered_map<char const *,System*> systemPointers{};
public:
    
    template<typename T>
    T* RegisterSystem();

    template<typename T>
    void SetSignature(Signature signature);
    
    void RemoveEntity(Entity entity);
    
    void SignatureChanged(Entity entity,Signature signature);
};
