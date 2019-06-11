#pragma once
#include "systemPhysics.h"
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

class BulletPhysicsSystem : public PhysicsSystem
{
protected:
    std::unique_ptr<btDefaultCollisionConfiguration> m_CollisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_Dispatcher;
    std::unique_ptr<btBroadphaseInterface> m_OverlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_Solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_DynamicsWorld;

    std::vector<std::unique_ptr<btCollisionShape>> m_CollisionShapes;
public:
    BulletPhysicsSystem();
    ~BulletPhysicsSystem();

    virtual btCollisionShape* ManageShape(btCollisionShape* CollisionShape);
    virtual void Tick(float DeltaTime) override;
    virtual void RegisterComponent(BaseComponent* Component) override;
};
