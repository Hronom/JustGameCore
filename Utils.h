#ifndef UTILS_H
#define UTILS_H

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

namespace JGC
{
	btVector3 toBulletVector3(Ogre::Vector3 xVector3);
	btQuaternion toBulletQuaternion(Ogre::Quaternion xQuaternion);
}

#endif