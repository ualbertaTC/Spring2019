#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "componentBase.h"

class BaseComponent;
class Entity
{
    std::unordered_map<std::type_index, std::unique_ptr<BaseComponent>> m_Components;
    Entity* m_Parent;
    std::vector<Entity*> m_Children;

    // Position of some sort?
public:
    template <typename T>
    T* GetComponent() const
    {
        auto iter = m_Component.find(typeid(T));
        return iter != m_Component.end() ? oter->second.get() : nullptr;
    }
    template <typename T>
    void AddComponent(std::unique_ptr<T> Component)
    {
        m_Components[typeid(T)] = std::move(Component);
    }

    glm::vec3 GetLocation() const;
    virtual void OnCollision(Entity* Other);


};
