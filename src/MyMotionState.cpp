#include "MyMotionState.h"

namespace JGC
{
	MyMotionState::MyMotionState(const btTransform &xInitialpos, Ogre::SceneNode *xNode)
	{
		mSceneNode = xNode;
		mPos = xInitialpos;
	}

	MyMotionState::~MyMotionState()
	{

	}

	void MyMotionState::setNode(Ogre::SceneNode *xNode)
	{
		mSceneNode = xNode;
	}

	void MyMotionState::getWorldTransform(btTransform &xWorldTrans) const 
	{
		xWorldTrans = mPos;
	}

	void MyMotionState::setWorldTransform(const btTransform &xWorldTrans) 
	{
		if(NULL == mSceneNode) return; // silently return before we set a node
		btQuaternion xRot = xWorldTrans.getRotation();
		mSceneNode->setOrientation(xRot.w(), xRot.x(), xRot.y(), xRot.z());
		btVector3 xPos = xWorldTrans.getOrigin();
		mSceneNode->setPosition(xPos.x(), xPos.y(), xPos.z());
	}
}
