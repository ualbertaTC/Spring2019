#pragma once
#include "systemBase.h"

class PhysicsSystem : public BaseSystem
{
public:
    virtual void Tick(float DeltaTime) override = 0;
};
