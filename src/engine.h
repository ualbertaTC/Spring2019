#pragma once

#include "entity.h"

#include <vector>

class Engine
{
    Entity* m_RootEntity;
    std::vector<BaseSystem*> m_Systems;    
public:

};

