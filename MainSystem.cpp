#include "MainSystem.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "InputSystem.h"
#include "StatesSystem.h"
using namespace JGC;

MainSystem* MainSystem::mInstance = 0;

MainSystem::MainSystem(Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile)
{
	//mGraphicSystem = new Graphic::GraphicSystem(this, xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
	//mPhysicsSystem = new Physics::PhysicsSystem(this);
	//mSoundSystem = new Sound::SoundSystem(this);
	//mInputSystem = new Input::InputSystem(this);
	//mStatesSystem = new States::StatesSystem(this);

	mNeedShutdown = false;
	mStateLoad = false;

	mCurrentStateName = "";
	mShowLoadScreen = false;

	Graphic::GraphicSystem::initialize(this, xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
	Physics::PhysicsSystem::initialize(this, Graphic::GraphicSystem::instance()->getSceneManager());
	Sound::SoundSystem::initialize(this);
	Input::InputSystem::initialize(this, 
		Graphic::GraphicSystem::instance()->getWinHandle(),
		Graphic::GraphicSystem::instance()->getWinWidth(), 
		Graphic::GraphicSystem::instance()->getWinHeight());
	States::StatesSystem::initialize(this);
}

MainSystem::~MainSystem()
{
	States::StatesSystem::shutdown();
	Input::InputSystem::shutdown();
	Sound::SoundSystem::shutdown();
	Physics::PhysicsSystem::shutdown();
	Graphic::GraphicSystem::shutdown();
}

void MainSystem::initialize(Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile)
{
	mInstance = new MainSystem(xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
}

void MainSystem::shutdown()
{
	delete mInstance;
	mInstance = 0;
}

MainSystem* MainSystem::instance()
{
	return mInstance;
}

void MainSystem::run()
{
	Graphic::GraphicSystem::instance()->start();
}

void MainSystem::needSwitchToState(std::string xStateName, bool xShowLoadScreen)
{
	mCurrentStateName = xStateName;
	mShowLoadScreen = xShowLoadScreen;
}

void MainSystem::stateLoadProgress(int xProgressValue, std::string xText)
{
	States::StatesSystem::instance()->injectStateLoadProgress(xProgressValue, xText);
	Graphic::GraphicSystem::instance()->needSingleUpdate();
}

void MainSystem::needShutdown()
{
	mNeedShutdown = true;
}

//-------------------------------------------------------------
// ISystemsListener
//-------------------------------------------------------------
bool MainSystem::frameStarted(const Ogre::FrameEvent& evt)
{
	if(mStateLoad != true)
	{
		if(States::StatesSystem::instance()->getCurrentStateName() != mCurrentStateName)
		{
			States::StatesSystem::instance()->switchToState(mCurrentStateName, mShowLoadScreen);
		}
		else
		{
			Physics::PhysicsSystem::instance()->needUpdate(evt);
			Input::InputSystem::instance()->needUpdate();
			States::StatesSystem::instance()->needUpdate(evt);
		}
	}

	return !mNeedShutdown;
}

bool MainSystem::frameEnded(const Ogre::FrameEvent& evt)
{
	return !mNeedShutdown;
}

void MainSystem::windowResized(unsigned int xNewWidth, unsigned int xNewHeight)
{
	Input::InputSystem::instance()->injectWindowResized(xNewWidth, xNewHeight);
}

void MainSystem::windowClosed()
{
	mNeedShutdown = true;
}

void MainSystem::mouseMoved(const OIS::MouseEvent& e)
{
	Graphic::GraphicSystem::instance()->injectMouseMoved(e);
	States::StatesSystem::instance()->injectMouseMoved(e);
}

void MainSystem::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	Graphic::GraphicSystem::instance()->injectMousePressed(e, id);
	States::StatesSystem::instance()->injectMousePressed(e, id);
}

void MainSystem::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	Graphic::GraphicSystem::instance()->injectMouseReleased(e, id);
	States::StatesSystem::instance()->injectMouseReleased(e, id);
}

void MainSystem::keyPressed(const OIS::KeyEvent& e)
{
	Graphic::GraphicSystem::instance()->injectKeyPressed(e);
	States::StatesSystem::instance()->injectKeyPressed(e);
}

void MainSystem::keyReleased(const OIS::KeyEvent& e)
{
	Graphic::GraphicSystem::instance()->injectKeyReleased(e);
	States::StatesSystem::instance()->injectKeyReleased(e);
}

void MainSystem::stateStartLoad()
{
	mStateLoad = true;
}

void MainSystem::stateEndLoad()
{
	mStateLoad = false;
}