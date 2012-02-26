#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "ISystemsListener.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

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

			btBroadphaseInterface* mBroadphase;
			btDefaultCollisionConfiguration* mCollisionConfiguration;
			btCollisionDispatcher* mDispatcher;
			btSequentialImpulseConstraintSolver* mSolver;
			btDiscreteDynamicsWorld* mDynamicsWorld;

		public:
			static void initialize(ISystemsListener *xMainListener, Ogre::SceneManager *xSceneManager);
			static void shutdown();
			static PhysicsSystem* instance();

		private: 
			PhysicsSystem(ISystemsListener *xMainListener);
			~PhysicsSystem();
			void init(Ogre::SceneManager *xSceneManager);
			void needUpdate(const Ogre::FrameEvent& evt);
			void setDebugDrawer(btIDebugDraw* xBtIDebugDraw);

		public:
			btDiscreteDynamicsWorld* getDynamicsWorld();
		};
	}
}

#endif