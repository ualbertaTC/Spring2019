#include "systemBase.h"

void BaseSystem::RegisterComponent(BaseComponent* Component)
{
    m_Components.push_back(Component);
}


