#include <Utils.h>

btVector3 JGC::toBtVector3(const Ogre::Vector3 &xVector3)
{
	return btVector3(xVector3.x, xVector3.y, xVector3.z);
}

Ogre::Vector3 JGC::toOgreVector3(const btVector3& xVector3)
{
	return Ogre::Vector3(xVector3.x(), xVector3.y(), xVector3.z());
}

btQuaternion JGC::toBtQuaternion(const Ogre::Quaternion &xQuaternion)
{
	return btQuaternion(xQuaternion.x, xQuaternion.y, xQuaternion.z, xQuaternion.w);
}

Ogre::Quaternion JGC::toOgreQuaternion(const btQuaternion &xQuaternion)
{
	return Ogre::Quaternion(xQuaternion.w(), xQuaternion.x(), xQuaternion.y(), xQuaternion.z());
}

Ogre::Vector2 JGC::toOgreVector2Z(const Ogre::Vector3 &xVector3)
{
	return Ogre::Vector2 (xVector3.x, xVector3.y);
}