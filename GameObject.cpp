#include "GameObject.h"

#include "ICore.h"
#include "IGameObjectsListener.h"

GameObject::GameObject(ICore *xCore, IGameObjectsListener *xGameObjectsListener, Ogre::String xObjectName, short xObjectCollideWith)
{
	mCore = xCore;
	mGameObjectsListener = xGameObjectsListener;

	mObjectName = xObjectName;
	mObjectCollideWith = xObjectCollideWith;
	mHealthCount = 0;
	mMakeDamage = 0;
	mMoveSpeed = 0;
	mMoveDirection = Ogre::Vector3::ZERO;
	mDestinationDot = Ogre::Vector2::ZERO;

	// Graph
	mEntity = 0;
	mSceneNode = 0;
	
	// Phys
	mCollisionShape = 0;
	mRigidBody = 0;

	mShoot = false;
	mShootDelay = 0;
	mTimeSinceLastShot = mShootDelay;
	mNeedDelete = false;
}

GameObject::~GameObject()
{
	if(mRigidBody != 0)
	{
		mCore->getDynamicsWorld()->getBulletDynamicsWorld()->removeRigidBody(mRigidBody->getBulletRigidBody());
		delete mRigidBody->getBulletRigidBody()->getMotionState();
		delete mRigidBody->getBulletRigidBody();
	}

	if(mCollisionShape != 0)
		delete mCollisionShape;

	if(mEntity != 0) 
		mCore->getSceneManager()->destroyEntity(mEntity);

	if(mSceneNode != 0)
	{
		mSceneNode->removeAndDestroyAllChildren();
		mCore->getSceneManager()->destroySceneNode(mSceneNode);
	}
}

void GameObject::update(const Ogre::FrameEvent& evt)
{

}

Ogre::String GameObject::getObjectName()
{
	return mObjectName; 
}

int GameObject::getCurrentHealth()
{
	return mHealthCount;
}

int GameObject::getDamage()
{
	return mMakeDamage;
}

void GameObject::makeDamage(int xDamage)
{
	mHealthCount -= xDamage;
}

Ogre::Vector2 GameObject::getCurrentPos()
{
	Ogre::Vector3 xVector3Pos;
	Ogre::Vector2 xVector2Pos;

	xVector3Pos = mSceneNode->getPosition();

	xVector2Pos.x = xVector3Pos.x;
	xVector2Pos.y = xVector3Pos.y;

	return xVector2Pos;
}

bool GameObject::isNeedDelete()
{
	return mNeedDelete;
}

void GameObject::moveUp(bool doMove)
{
	if(doMove == true)
		mMoveDirection.x = 1;
	else 
		mMoveDirection.x = 0;
}

void GameObject::moveDown(bool doMove)
{
	if(doMove == true)
		mMoveDirection.x = -1;
	else 
		mMoveDirection.x = 0;
}

void GameObject::moveRight(bool doMove)
{
	if(doMove == true)
		mMoveDirection.y = -1;
	else 
		mMoveDirection.y = 0;
}

void GameObject::moveLeft(bool doMove)
{
	if(doMove == true)
		mMoveDirection.y = 1;
	else 
		mMoveDirection.y = 0;
}

void GameObject::rotateTo(Ogre::Vector2 xDestinationDot)
{
	mDestinationDot = xDestinationDot;
}

void GameObject::shoot(bool doShoot)
{
	mShoot = doShoot;
}
