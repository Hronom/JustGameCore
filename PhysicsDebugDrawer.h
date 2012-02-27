#ifndef PHYSICSDEBUGDRAWER_H
#define PHYSICSDEBUGDRAWER_H

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

namespace JGC
{
	class PhysicsDebugDrawer: public btIDebugDraw, public Ogre::FrameListener
	{
	private:
		bool mFrameStarted;
		struct ContactPoint
		{
			Ogre::Vector3 xFrom;
			Ogre::Vector3 xTo;
			Ogre::ColourValue xColor;
			size_t xDieTime;
		};

		DebugDrawModes mDebugModes;
		Ogre::ManualObject *mLines;
		Ogre::ManualObject *mTriangles;
		std::vector<ContactPoint> *mContactPoints;
		std::vector<ContactPoint> mContactPoints1;
		std::vector<ContactPoint> mContactPoints2;

	public:
		PhysicsDebugDrawer(Ogre::SceneManager *xSceneManager);
		~PhysicsDebugDrawer();
		virtual void drawLine(const btVector3 &xFrom, const btVector3 &xTo, const btVector3 &xColor);
		virtual void drawTriangle(const btVector3 &xDot1, const btVector3 &xDot2, const btVector3 &xDot3, const btVector3 &xColor, btScalar xAlpha);
		virtual void drawContactPoint(const btVector3 &xPointOnB, const btVector3 &xNormalOnB, btScalar xDistance, int xLifeTime, const btVector3 &xColor);
		virtual void reportErrorWarning(const char *xWarningString);
		virtual void draw3dText(const btVector3 &xLocation, const char *xTextString);
		virtual void setDebugMode(int xDebugMode);
		virtual int  getDebugMode() const;

	public:
		bool frameStarted(const float& xTimeSinceLastFrame);
		bool frameEnded(const float& xTimeSinceLastFrame);
	};
}

#endif