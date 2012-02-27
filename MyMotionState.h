#ifndef MYMOTIONSTATE_H
#define MYMOTIONSTATE_H 

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

namespace JGC
{
	class MyMotionState : public btMotionState
	{
	protected:
		Ogre::SceneNode *mSceneNode;
		btTransform mPos;

	public:
		MyMotionState(const btTransform &xInitialpos, Ogre::SceneNode *xNode);
		virtual ~MyMotionState();
		void setNode(Ogre::SceneNode *xNode);
		virtual void getWorldTransform(btTransform &xWorldTrans) const;
		virtual void setWorldTransform(const btTransform &xWorldTrans);
	};
}

#endif