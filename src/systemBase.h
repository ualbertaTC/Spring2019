#pragma once
#include <vector>

class BaseComponent;
class BaseSystem
{
protected:
    std::vector<BaseComponent*> m_Components;
public:
    void RegisterComponent(BaseComponent* Component);
    virtual void Tick(float DeltaTime) = 0;

};
