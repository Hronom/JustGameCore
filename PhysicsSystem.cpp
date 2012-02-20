#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(ISystemsListener *xMainListener)
{
	mMainListener = xMainListener;
}

PhysicsSystem::~PhysicsSystem()
{
	delete mDynamicsWorld->getDebugDrawer();
	mDynamicsWorld->setDebugDrawer(0);

	delete mDynamicsWorld;
	mDynamicsWorld = 0;
}

void PhysicsSystem::init(Ogre::SceneManager *xSceneManager)
{
	// Start Bullet
	mDynamicsWorld = new OgreBulletDynamics::DynamicsWorld(xSceneManager,
	Ogre::AxisAlignedBox(Ogre::Vector3(-10000, -10000, -10000),Ogre::Vector3(10000,  10000,  10000)), Ogre::Vector3(0,0,0));
	// add Debug info display tool
	mDebugDrawer = new OgreBulletCollisions::DebugDrawer();
	mDebugDrawer->setDrawWireframe(false);   // we want to see the Bullet containers
	mDynamicsWorld->setDebugDrawer(mDebugDrawer);
	mDynamicsWorld->setShowDebugShapes(false);      // enable it if you want to see the Bullet containers
	Ogre::SceneNode *xNode = xSceneManager->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	xNode->attachObject(static_cast <Ogre::SimpleRenderable*> (mDebugDrawer));
}

void PhysicsSystem::needUpdate(const Ogre::FrameEvent& evt)
{
	mDynamicsWorld->stepSimulation(evt.timeSinceLastFrame);	// update Bullet Physics animation
}

OgreBulletDynamics::DynamicsWorld* PhysicsSystem::getDynamicsWorld()
{
	return mDynamicsWorld;
}