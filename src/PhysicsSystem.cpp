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
        mActiveDynamicsWorld = 0;

        while(!mDynamicsWorlds.empty())
        {
            delete mDynamicsWorlds.begin().value();
            mDynamicsWorlds.erase(mDynamicsWorlds.begin());
        }

        //delete mDynamicsWorld;
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
        mActiveDynamicsWorld = 0;
	}

	void PhysicsSystem::injectUpdate(const float& xTimeSinceLastFrame)
	{
        if(mActiveDynamicsWorld != 0)
        {
            mActiveDynamicsWorld->stepSimulation(xTimeSinceLastFrame, 10);	// update Bullet Physics animation
            mActiveDynamicsWorld->debugDrawWorld();
        }
	}

	void PhysicsSystem::setDebugDrawer(btIDebugDraw* xBtIDebugDraw)
	{
        if(mActiveDynamicsWorld != 0)
        {
            mActiveDynamicsWorld->setDebugDrawer(xBtIDebugDraw);
        }
	}

    void PhysicsSystem::setActiveDynamicsWorld(QString xDynamicsWorldName)
    {
        if(mDynamicsWorlds.contains(xDynamicsWorldName))
            mActiveDynamicsWorld = mDynamicsWorlds.value(xDynamicsWorldName);
        else
            mActiveDynamicsWorld = 0;
    }

    btDiscreteDynamicsWorld* PhysicsSystem::getActiveDynamicsWorld()
    {
        if(mActiveDynamicsWorld != 0)
            return mActiveDynamicsWorld;
        else
            return 0;
    }

    void PhysicsSystem::createDynamicsWorld(QString xDynamicsWorldName)
    {
        btDiscreteDynamicsWorld* xDynamicsWorld;
        xDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
        xDynamicsWorld->setGravity(btVector3(0,0,0));
        mDynamicsWorlds.insert(xDynamicsWorldName, xDynamicsWorld);
    }

    btDiscreteDynamicsWorld* PhysicsSystem::getDynamicsWorld(QString xDynamicsWorldName)
    {
        if(mDynamicsWorlds.contains(xDynamicsWorldName))
            return mDynamicsWorlds.value(xDynamicsWorldName);
        else
            return 0;
    }

    QVector<QPair<const btCollisionObject *, const btCollisionObject *> > PhysicsSystem::getCollidedObjects()
    {
        QVector<QPair<const btCollisionObject *, const btCollisionObject *> > xCollidedObjects;

        if(mActiveDynamicsWorld != 0)
        {
            const btCollisionObject *xObjectA = 0;
            const btCollisionObject *xObjectB = 0;

            int xNumManifolds = mActiveDynamicsWorld->getDispatcher()->getNumManifolds();
            for(int i = 0; i < xNumManifolds; ++i)
            {
                btPersistentManifold *xContactManifold =  mActiveDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
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
        }

        return xCollidedObjects;
    }
}
