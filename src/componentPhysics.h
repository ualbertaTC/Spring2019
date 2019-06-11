#pragma once 

#include "componentBase.h"
#include <glm/glm.hpp>
#include "systemPhysics.h"

class PhysicsComponent : public BaseComponent
{
protected:
    friend PhysicsSystem;
public:
    virtual glm::vec3 GetLocation() const = 0;

};
