#include "InputSystem.h"


namespace JGC
{
	InputSystem* InputSystem::mInstance = 0;

	void InputSystem::initialize(ISystemsListener *xMainListener, unsigned int xWinHandle, unsigned int xWinWidth, unsigned int xWinHeight)
	{
		mInstance = new InputSystem(xMainListener);
		mInstance->init(xWinHandle, xWinWidth, xWinHeight);
	}

	void InputSystem::shutdown()
	{
		delete mInstance;
		mInstance = 0;
	}

	InputSystem* InputSystem::instance()
	{
		return mInstance;
	}

	InputSystem::InputSystem(ISystemsListener *xMainListener)
	{
		mMainListener = xMainListener;
	}

	InputSystem::~InputSystem()
	{
		//OIS
		mInputManager->destroyInputObject(mMouse); 
		mMouse = 0;

		mInputManager->destroyInputObject(mKeyboard); 
		mKeyboard = 0;

		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = 0;
	}

	void InputSystem::init(unsigned int xWinHandle, unsigned int xWinWidth, unsigned int xWinHeight)
	{
		//---------------------------------------------------- 
		// Добавление OIS для перехвата событий 
		//----------------------------------------------------

		//Настройка менеджера, клавы и мыши для захвата
		mInputManager = OIS::InputManager::createInputSystem(xWinHandle);
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));

		//Говорим OIS о размерах нашего окна
		const OIS::MouseState &xMouseState = mMouse->getMouseState();
		xMouseState.width = xWinWidth;
		xMouseState.height = xWinHeight;

		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
	}

	void InputSystem::injectUpdate()
	{
		mMouse->capture();
		mKeyboard->capture();
	}

	void InputSystem::injectWindowResized(unsigned int xNewWidth, unsigned int xNewHeight)
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = xNewWidth;
		ms.height = xNewHeight;
	}

	bool InputSystem::mouseMoved(const OIS::MouseEvent& e)
	{
		mMainListener->injectMouseMoved(e);
		return true; 
	}

	bool InputSystem::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{ 
		mMainListener->injectMousePressed(e, id);
		return true; 
	}

	bool InputSystem::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{ 
		mMainListener->injectMouseReleased(e, id);
		return true; 
	}

	bool InputSystem::keyPressed(const OIS::KeyEvent& e)
	{ 
		mMainListener->injectKeyPressed(e);
		return true;
	}

	bool InputSystem::keyReleased(const OIS::KeyEvent& e)
	{ 
		mMainListener->injectKeyReleased(e);
		return true; 
	}
}