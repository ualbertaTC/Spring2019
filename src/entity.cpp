#include "entity.h"
#include "componentPhysics.h"

void Entity::AddComponent(std::unique_ptr<BaseComponent> Component)
{
    m_Components.push_back(std::move(Component));
}

glm::vec3 Entity::GetLocation()
{
    return GetComponent<PhysicsComponent>()->GetLocation();
}
void Entity::OnCollision(Entity* Other)
{
}
