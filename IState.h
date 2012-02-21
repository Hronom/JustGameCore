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

		virtual void needUpdate(const Ogre::FrameEvent& evt) {};
		virtual	void mouseMoved(const OIS::MouseEvent& e) {};
		virtual void mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {};
		virtual void mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {};
		virtual	void keyPressed(const OIS::KeyEvent& e) {};
		virtual void keyReleased(const OIS::KeyEvent& e) {};
	};
}

#endif

