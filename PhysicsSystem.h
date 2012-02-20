#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "ISystemsListener.h"

#include "OgreBulletDynamicsRigidBody.h"
#include "Ogre.h"

class PhysicsSystem
{
private:
	ISystemsListener *mMainListener;

	OgreBulletDynamics::DynamicsWorld *mDynamicsWorld;   // OgreBullet World
	OgreBulletCollisions::DebugDrawer *mDebugDrawer;

public: 
	PhysicsSystem(ISystemsListener *xMainListener);
	~PhysicsSystem();

	void init(Ogre::SceneManager *xSceneManager);

	void needUpdate(const Ogre::FrameEvent& evt);

	OgreBulletDynamics::DynamicsWorld* getDynamicsWorld();
};

#endif
