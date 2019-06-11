#pragma once 
#include <bullet/btBulletDynamicsCommon.h>

#include "componentPhysics.h"
#include "systemPhysicsBullet.h"

class BulletPhysicsComponent : public PhysicsComponent
{
protected:
    friend BulletPhysicsSystem;
    btRigidBody* m_Body;
public:
    BulletPhysicsComponent(btRigidBody* Body);
    virtual glm::vec3 GetLocation() const;
};
