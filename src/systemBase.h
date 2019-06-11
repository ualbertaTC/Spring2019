#pragma once
#include <vector>

class BaseComponent;
class BaseSystem
{
protected:
    std::vector<BaseComponent*> m_Components;
public:
    virtual void Tick(float DeltaTime) = 0;
    virtual void RegisterComponent(BaseComponent* Component);

};
