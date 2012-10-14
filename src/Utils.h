#ifndef UTILS_H
#define UTILS_H

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

namespace JGC
{
	namespace Utils
	{
		btVector3 toBtVector3(const Ogre::Vector3 &xVector3);
		Ogre::Vector3 toOgreVector3(const btVector3& xVector3);

		btQuaternion toBtQuaternion(const Ogre::Quaternion &xQuaternion);
		Ogre::Quaternion toOgreQuaternion(const btQuaternion &xQuaternion);

		Ogre::Vector2 toOgreVector2Z(const Ogre::Vector3 &xVector3);
	}
}

#endif