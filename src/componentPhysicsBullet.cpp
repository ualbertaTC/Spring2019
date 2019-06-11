#include "componentPhysicsBullet.h"

#include "utils.h"

BulletPhysicsComponent::BulletPhysicsComponent(btRigidBody* Body) : m_Body(Body)
{}

glm::vec3 BulletPhysicsComponent::GetLocation() const
{
    return bt2glm(m_Body->getCenterOfMassPosition());
}

