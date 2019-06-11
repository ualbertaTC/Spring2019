#pragma once
#include "systemBase.h"

class Entity;
class BaseSystem;
class BaseComponent
{
protected:
    friend BaseSystem;
    Entity *m_Parent;
public:
    Entity* GetEntity() const
    {
        return m_Parent;
    }
    virtual void Tick(float DeltaTime) {}; 
};

