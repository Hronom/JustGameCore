#ifndef ISYSTEMSLISTENER_H
#define ISYSTEMSLISTENER_H

#include "Ogre.h"
#include "OgreFrameListener.h"
#include <OIS/OIS.h>

class ISystemsListener
{
public:
	ISystemsListener() {};
	virtual ~ISystemsListener() {};

	virtual bool frameStarted(const Ogre::FrameEvent& evt) = 0;
	virtual bool frameEnded(const Ogre::FrameEvent& evt) = 0;
	virtual void windowResized(unsigned int xNewWidth, unsigned int xNewHeight) = 0;
	virtual void windowClosed() = 0;
	virtual	void mouseMoved(const OIS::MouseEvent& e) = 0;
	virtual void mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) = 0;
	virtual void mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) = 0;
	virtual	void keyPressed(const OIS::KeyEvent& e) = 0;
	virtual void keyReleased(const OIS::KeyEvent& e) = 0;

	virtual void stateStartLoad() = 0;
	virtual void stateEndLoad() = 0;
};

#endif
