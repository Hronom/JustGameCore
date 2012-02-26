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
	mPhysicsDebugDrawer = 0;
	mNeedShutdown = false;

	Graphic::GraphicSystem::initialize(this, xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
	Physics::PhysicsSystem::initialize(this, Graphic::GraphicSystem::instance()->getSceneManager());
	Sound::SoundSystem::initialize(this);
	Input::InputSystem::initialize(this, 
		Graphic::GraphicSystem::instance()->getWinHandle(),
		Graphic::GraphicSystem::instance()->getWinWidth(), 
		Graphic::GraphicSystem::instance()->getWinHeight());
	States::StatesSystem::initialize(this);

	/*mPhysicsDebugDrawer = new PhysicsDebugDrawer(Graphic::GraphicSystem::instance()->getSceneManager());
	mPhysicsDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	Physics::PhysicsSystem::instance()->setDebugDrawer(mPhysicsDebugDrawer);*/
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
bool MainSystem::frameStarted(const float& xTimeSinceLastFrame)
{
	if(States::StatesSystem::instance()->isStateSwitching() == true)
	{
		States::StatesSystem::instance()->injectUpdate(xTimeSinceLastFrame);
	}
	else
	{
		Physics::PhysicsSystem::instance()->injectUpdate(xTimeSinceLastFrame);
		//mPhysicsDebugDrawer->frameStarted(xTimeSinceLastFrame);
		Input::InputSystem::instance()->injectUpdate();
		States::StatesSystem::instance()->injectUpdate(xTimeSinceLastFrame);
	}

	return !mNeedShutdown;
}

bool MainSystem::frameEnded(const float& xTimeSinceLastFrame)
{
	//mPhysicsDebugDrawer->frameEnded(xTimeSinceLastFrame);

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

void MainSystem::injectMouseMoved(const OIS::MouseEvent& e)
{
	Graphic::GraphicSystem::instance()->injectMouseMoved(e);
	States::StatesSystem::instance()->injectMouseMoved(e);
}

void MainSystem::injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	Graphic::GraphicSystem::instance()->injectMousePressed(e, id);
	States::StatesSystem::instance()->injectMousePressed(e, id);
}

void MainSystem::injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	Graphic::GraphicSystem::instance()->injectMouseReleased(e, id);
	States::StatesSystem::instance()->injectMouseReleased(e, id);
}

void MainSystem::injectKeyPressed(const OIS::KeyEvent& e)
{
	Graphic::GraphicSystem::instance()->injectKeyPressed(e);
	States::StatesSystem::instance()->injectKeyPressed(e);
}

void MainSystem::injectKeyReleased(const OIS::KeyEvent& e)
{
	Graphic::GraphicSystem::instance()->injectKeyReleased(e);
	States::StatesSystem::instance()->injectKeyReleased(e);
}