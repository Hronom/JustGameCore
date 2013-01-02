#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "ISystemsListener.h"

#include <btBulletDynamicsCommon.h>

#include <QHash>
#include <QString>
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

        btDiscreteDynamicsWorld* mActiveDynamicsWorld;
        QHash<QString, btDiscreteDynamicsWorld*> mDynamicsWorlds;

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

        void createDynamicsWorld(QString xDynamicsWorldName);
        void deleteDynamicsWorld(QString xDynamicsWorldName);
        btDiscreteDynamicsWorld* getDynamicsWorld(QString xDynamicsWorldName);

        void setActiveDynamicsWorld(QString xDynamicsWorldName);
        btDiscreteDynamicsWorld* getActiveDynamicsWorld();

        QVector<QPair<const btCollisionObject *, const btCollisionObject *> > getCollidedObjects();
	};
}

#endif
