#include "systemPhysicsBullet.h"
#include "componentPhysicsBullet.h"


BulletPhysicsSystem::BulletPhysicsSystem() 
{
    m_CollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    m_Dispatcher = std::make_unique<btCollisionDispatcher>(m_CollisionConfiguration.get());
    m_OverlappingPairCache = std::make_unique<btDbvtBroadphase>();
    m_Solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_DynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_Dispatcher.get(), m_OverlappingPairCache.get(), m_Solver.get(), m_CollisionConfiguration.get());

    m_DynamicsWorld->setGravity(btVector3(0,-9.81,0));
}

BulletPhysicsSystem::~BulletPhysicsSystem()
{
    // BAD TODO: components should delete themself BulletPhysicsComponent should be a unique ptr
    for (int i = m_DynamicsWorld->getNumCollisionObjects(); i > 0; i--)
    {
        btCollisionObject* obj = m_DynamicsWorld->getCollisionObjectArray()[i-1];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        m_DynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }
}

btCollisionShape* BulletPhysicsSystem::ManageShape(btCollisionShape* CollisionShape)
{
    m_CollisionShapes.push_back(std::move(std::unique_ptr<btCollisionShape>(CollisionShape)));
    return CollisionShape;
}

void BulletPhysicsSystem::Tick(float DeltaTime)
{
    m_DynamicsWorld->stepSimulation(DeltaTime, 10);
}

void BulletPhysicsSystem::RegisterComponent(BaseComponent* Component)
{
    BaseSystem::RegisterComponent(Component);
    m_DynamicsWorld->addRigidBody(static_cast<BulletPhysicsComponent*>(Component)->m_Body);
}
