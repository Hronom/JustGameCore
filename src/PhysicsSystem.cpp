#include "PhysicsSystem.h"


namespace JGC
{
	PhysicsSystem* PhysicsSystem::mInstance = 0;

	void PhysicsSystem::initialize(ISystemsListener *xMainListener, Ogre::SceneManager *xSceneManager)
	{
		mInstance = new PhysicsSystem(xMainListener);
		mInstance->init(xSceneManager);
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

	void PhysicsSystem::init(Ogre::SceneManager *xSceneManager)
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
}