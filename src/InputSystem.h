#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <OIS/OIS.h>
#include "ISystemsListener.h"

namespace JGC
{
	class InputSystem : public OIS::MouseListener, public OIS::KeyListener
	{
	private:
		static InputSystem* mInstance;

		ISystemsListener *mMainListener;

		OIS::InputManager *mInputManager;
		OIS::Keyboard *mKeyboard;
		OIS::Mouse *mMouse;

	public:
		static void initialize(ISystemsListener *xMainListener, unsigned int xWinHandle, unsigned int xWinWidth, unsigned int xWinHeight);
		static void shutdown();
		static InputSystem* instance();

	private: 
		InputSystem(ISystemsListener *xMainListener);
		~InputSystem();
		void init(unsigned int xWinHandle, unsigned int xWinWidth, unsigned int xWinHeight);

	public:
		void injectUpdate();
		void injectWindowResized(unsigned int xNewWidth, unsigned int xNewHeight);

	private:
		bool mouseMoved(const OIS::MouseEvent& e);
		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool keyPressed(const OIS::KeyEvent& e);
		bool keyReleased(const OIS::KeyEvent& e);
	};
}

#endif
