#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "ISystemsListener.h"

#include "OgreBulletDynamicsRigidBody.h"
#include "Ogre.h"

namespace JGC
{
	class MainSystem;
	namespace Physics
	{
		class PhysicsSystem
		{
			friend class MainSystem;
		private:
			static PhysicsSystem* mInstance;

			ISystemsListener *mMainListener;

			OgreBulletDynamics::DynamicsWorld *mDynamicsWorld;   // OgreBullet World
			OgreBulletCollisions::DebugDrawer *mDebugDrawer;

		public:
			static void initialize(ISystemsListener *xMainListener, Ogre::SceneManager *xSceneManager);
			static void shutdown();
			static PhysicsSystem* instance();

		private: 
			PhysicsSystem(ISystemsListener *xMainListener);
			~PhysicsSystem();
			void init(Ogre::SceneManager *xSceneManager);
			void needUpdate(const Ogre::FrameEvent& evt);
		public:
			OgreBulletDynamics::DynamicsWorld* getDynamicsWorld();
		};
	}
}

#endif