#pragma once
#include "systemBase.h"

class RenderSystem : public BaseSystem
{

public:
    virtual void Tick(float DeltaTime) override;
};
