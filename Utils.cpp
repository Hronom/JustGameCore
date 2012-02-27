#include <Utils.h>

namespace JGC
{
	namespace Utils
	{
		btVector3 toBtVector3(const Ogre::Vector3 &xVector3)
		{
			return btVector3(xVector3.x, xVector3.y, xVector3.z);
		}

		Ogre::Vector3 toOgreVector3(const btVector3& xVector3)
		{
			return Ogre::Vector3(xVector3.x(), xVector3.y(), xVector3.z());
		}

		btQuaternion toBtQuaternion(const Ogre::Quaternion &xQuaternion)
		{
			return btQuaternion(xQuaternion.x, xQuaternion.y, xQuaternion.z, xQuaternion.w);
		}

		Ogre::Quaternion toOgreQuaternion(const btQuaternion &xQuaternion)
		{
			return Ogre::Quaternion(xQuaternion.w(), xQuaternion.x(), xQuaternion.y(), xQuaternion.z());
		}

		Ogre::Vector2 toOgreVector2Z(const Ogre::Vector3 &xVector3)
		{
			return Ogre::Vector2 (xVector3.x, xVector3.y);
		}
	}
}