#ifndef IGAMEOBJECTSLISTENER_H
#define IGAMEOBJECTSLISTENER_H

#include <Ogre.h>

class IGameObjectsListener
{
public:
	virtual void addBullet(short xObjectCollideWith, Ogre::Vector2 xPos, Ogre::Vector2 xDestination) = 0;

};

#endif