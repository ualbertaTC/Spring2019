#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "componentBase.h"

class BaseComponent;
class Entity
{
    std::vector<std::unique_ptr<BaseComponent>> m_Components;

public:
    void AddComponent(std::unique_ptr<BaseComponent> Component);
    
    template <typename T>
    T* GetComponent()
    {
        for (auto&& Component : m_Components)
        {
            auto ret = dynamic_cast<T*>(Component.get());
            if (ret != nullptr)
            {
                return ret;
            }
        }
        return nullptr;
    }

    glm::vec3 GetLocation();
    virtual void OnCollision(Entity* Other);


};
