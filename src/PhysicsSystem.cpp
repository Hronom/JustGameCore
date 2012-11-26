#include "PhysicsSystem.h"


namespace JGC
{
	PhysicsSystem* PhysicsSystem::mInstance = 0;

    void PhysicsSystem::initialize(ISystemsListener *xMainListener)
	{
		mInstance = new PhysicsSystem(xMainListener);
        mInstance->init();
	}

	void PhysicsSystem::shutdown()
	{
		delete mInstance;
		mInstance = 0;
	}

	PhysicsSystem* PhysicsSystem::instance()
	{
		return mInstance;
	}

	PhysicsSystem::PhysicsSystem(ISystemsListener *xMainListener)
	{
		mMainListener = xMainListener;
	}

	PhysicsSystem::~PhysicsSystem()
	{
		delete mDynamicsWorld;
		delete mSolver;
		delete mDispatcher;
		delete mCollisionConfiguration;
		delete mBroadphase;
	}

    void PhysicsSystem::init()
	{
		// Build the broadphase
		mBroadphase = new btDbvtBroadphase();

		// Set up the collision configuration and dispatcher
		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

		// The actual physics solver
		mSolver = new btSequentialImpulseConstraintSolver;

		// The world.
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
		mDynamicsWorld->setGravity(btVector3(0,0,0));
	}

	void PhysicsSystem::injectUpdate(const float& xTimeSinceLastFrame)
	{
        mDynamicsWorld->stepSimulation(xTimeSinceLastFrame, 10);	// update Bullet Physics animation
		mDynamicsWorld->debugDrawWorld();
	}

	void PhysicsSystem::setDebugDrawer(btIDebugDraw* xBtIDebugDraw)
	{
		mDynamicsWorld->setDebugDrawer(xBtIDebugDraw);
	}

	btDiscreteDynamicsWorld* PhysicsSystem::getDynamicsWorld()
	{
		return mDynamicsWorld;
	}

    QVector<QPair<const btCollisionObject *, const btCollisionObject *> > PhysicsSystem::getCollidedObjects()
    {
        QVector<QPair<const btCollisionObject *, const btCollisionObject *> > xCollidedObjects;

        const btCollisionObject *xObjectA = 0;
        const btCollisionObject *xObjectB = 0;

        int xNumManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for(int i = 0; i < xNumManifolds; ++i)
        {
            btPersistentManifold *xContactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject *xObjectACandidate = static_cast<const btCollisionObject *>(xContactManifold->getBody0());
            const btCollisionObject *xObjectBCandidate = static_cast<const btCollisionObject *>(xContactManifold->getBody1());

            if(xObjectA == 0 && xObjectB == 0)
            {
                xObjectA = xObjectACandidate;
                xObjectB = xObjectBCandidate;
                QPair<const btCollisionObject *, const btCollisionObject *> xPair;
                xPair.first = xObjectA;
                xPair.second = xObjectB;
                xCollidedObjects.push_back(xPair);
            }
            else if( (xObjectA != xObjectACandidate && xObjectB != xObjectBCandidate) &&
                     (xObjectA != xObjectBCandidate && xObjectB != xObjectACandidate))
            {
                xObjectA = xObjectACandidate;
                xObjectB = xObjectBCandidate;
                QPair<const btCollisionObject *, const btCollisionObject *> xPair;
                xPair.first = xObjectA;
                xPair.second = xObjectB;
                xCollidedObjects.push_back(xPair);
            }
        }

        return xCollidedObjects;
    }
}
