#include "entity.h"
#include "componentPhysics.h"

glm::vec3 Entity::GetLocation()
{
    return GetComponent<PhysicsComponent>()->GetLocation();
}
void Entity::OnCollision(Entity* Other)
{
}
