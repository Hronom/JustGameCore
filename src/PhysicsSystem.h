#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "ISystemsListener.h"

#include <btBulletDynamicsCommon.h>
#include <QVector>
#include <QPair>

namespace JGC
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
        static void initialize(ISystemsListener *xMainListener);
		static void shutdown();
		static PhysicsSystem* instance();

	private: 
		PhysicsSystem(ISystemsListener *xMainListener);
		~PhysicsSystem();
        void init();

	public:
		void injectUpdate(const float& xTimeSinceLastFrame);
		void setDebugDrawer(btIDebugDraw* xBtIDebugDraw);
		btDiscreteDynamicsWorld* getDynamicsWorld();
        QVector<QPair<const btCollisionObject *, const btCollisionObject *> > getCollidedObjects();
	};
}

#endif
