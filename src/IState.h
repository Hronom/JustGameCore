#ifndef ISTATE_H
#define ISTATE_H

#include <OgreFrameListener.h>
#include <OIS/OIS.h>
#include <MyGUI.h>

namespace JGC
{
	class IState
	{
	public:
		IState() {};
		virtual ~IState() {};

		virtual void prepareState() = 0;
		virtual void enter() = 0;
		virtual void exit() = 0;

		virtual void injectUpdate(const float& xTimeSinceLastFrame) {};
		virtual	void injectMouseMoved(const OIS::MouseEvent& e) {};
		virtual void injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {};
		virtual void injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {};
		virtual	void injectKeyPressed(const OIS::KeyEvent& e) {};
		virtual void injectKeyReleased(const OIS::KeyEvent& e) {};
	};
}

#endif

