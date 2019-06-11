#include <iostream>
#include <memory>

#include "entity.h"
#include "componentPhysicsBullet.h"
#include "systemPhysicsBullet.h"

int main ()
{
    auto entity = new Entity();
	auto sys = new BulletPhysicsSystem();
    
    {
    btCollisionShape* colShape = new btSphereShape(btScalar(1.));
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0,50,0));
    btScalar mass(1.f);

    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
       colShape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo (mass, myMotionState, colShape, localInertia);

    auto body = new btRigidBody(rbInfo);
    auto comp = std::unique_ptr<BulletPhysicsComponent>(new BulletPhysicsComponent(body));
    sys->RegisterComponent(comp.get());
    entity->AddComponent(std::move(comp));
    }

    for (int i = 0; i < 100; i++)
    {
        sys->Tick(1);
        std::cout << entity->GetLocation().y << "\n";
    }

}
