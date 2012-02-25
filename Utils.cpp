#include <Utils.h>

btVector3 JGC::toBulletVector3(Ogre::Vector3 xVector3)
{
	return btVector3(xVector3.x, xVector3.y, xVector3.z);
}

btQuaternion JGC::toBulletQuaternion(Ogre::Quaternion xQuaternion)
{
	return btQuaternion(xQuaternion.x, xQuaternion.y, xQuaternion.z, xQuaternion.w);
}