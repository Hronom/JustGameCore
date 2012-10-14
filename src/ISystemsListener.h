#ifndef ISYSTEMSLISTENER_H
#define ISYSTEMSLISTENER_H

#include "Ogre.h"
#include "OgreFrameListener.h"
#include <OIS/OIS.h>

namespace JGC
{
	class ISystemsListener
	{
	public:
		ISystemsListener() {};
		virtual ~ISystemsListener() {};

		virtual bool frameStarted(const float& xTimeSinceLastFrame) = 0;
		virtual bool frameEnded(const float& xTimeSinceLastFrame) = 0;
		virtual void windowResized(unsigned int xNewWidth, unsigned int xNewHeight) = 0;
		virtual void windowClosed() = 0;
		virtual	void injectMouseMoved(const OIS::MouseEvent& e) = 0;
		virtual void injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) = 0;
		virtual void injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) = 0;
		virtual	void injectKeyPressed(const OIS::KeyEvent& e) = 0;
		virtual void injectKeyReleased(const OIS::KeyEvent& e) = 0;
	};
}

#endif
