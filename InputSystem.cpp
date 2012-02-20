#include "InputSystem.h"


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
	// 7 ���������� OIS ��� ��������� ������� 
	//----------------------------------------------------

	//��������� ���������, ����� � ���� ��� �������
	mInputManager = OIS::InputManager::createInputSystem(xWinHandle);
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));

	//������� OIS � �������� ������ ����
	const OIS::MouseState &xMouseState = mMouse->getMouseState();
	xMouseState.width = xWinWidth;
	xMouseState.height = xWinHeight;

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
}

void InputSystem::needUpdate()
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
	mMainListener->mouseMoved(e);

	return true; 
}

bool InputSystem::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{ 
	mMainListener->mousePressed(e, id);

	return true; 
}

bool InputSystem::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{ 
	mMainListener->mouseReleased(e, id);

	return true; 
}

bool InputSystem::keyPressed(const OIS::KeyEvent& e)
{ 
	mMainListener->keyPressed(e);

	return true;
}

bool InputSystem::keyReleased(const OIS::KeyEvent& e)
{ 
	mMainListener->keyReleased(e);

	return true; 
}