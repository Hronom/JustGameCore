#include "MainSystem.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "InputSystem.h"
#include "EntitySystem/EntitySystem.h"
#include "PhysicsDebugDrawer.h"

namespace JGC
{
	MainSystem* MainSystem::mInstance = 0;

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
		while(mNeedShutdown == false)
		{
			InputSystem::instance()->injectUpdate();
			PhysicsSystem::instance()->injectUpdate(mTimeSinceLastFrame);
            EntitySystem::instance()->injectUpdate(mTimeSinceLastFrame);
			GraphicSystem::instance()->injectUpdate();
		}
	}

	void MainSystem::needShutdown()
	{
		mNeedShutdown = true;
	}

	MainSystem::MainSystem(Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile)
	{
		mPhysicsDebugDrawer = 0;
		mTimeSinceLastFrame = 0;
		mNeedShutdown = false;

		GraphicSystem::initialize(this, xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
		PhysicsSystem::initialize(this, GraphicSystem::instance()->getSceneManager());
		SoundSystem::initialize(this);
		InputSystem::initialize(this, 
			GraphicSystem::instance()->getWinHandle(),
			GraphicSystem::instance()->getWinWidth(), 
			GraphicSystem::instance()->getWinHeight());
        EntitySystem::initialize();

		/*mPhysicsDebugDrawer = new PhysicsDebugDrawer(GraphicSystem::instance()->getSceneManager());
		PhysicsSystem::instance()->setDebugDrawer(mPhysicsDebugDrawer);*/
	}

	MainSystem::~MainSystem()
	{
		//delete mPhysicsDebugDrawer;
        EntitySystem::shutdown();
		InputSystem::shutdown();
		SoundSystem::shutdown();
		PhysicsSystem::shutdown();
		GraphicSystem::shutdown();
	}

	//-------------------------------------------------------------
	// ISystemsListener
	//-------------------------------------------------------------
	bool MainSystem::frameStarted(const float& xTimeSinceLastFrame)
	{
		//mPhysicsDebugDrawer->frameStarted(xTimeSinceLastFrame);
		return true;
	}

	bool MainSystem::frameEnded(const float& xTimeSinceLastFrame)
	{
		mTimeSinceLastFrame = xTimeSinceLastFrame;
		//mPhysicsDebugDrawer->frameEnded(xTimeSinceLastFrame);
		return true;
	}

	void MainSystem::windowResized(unsigned int xNewWidth, unsigned int xNewHeight)
	{
		InputSystem::instance()->injectWindowResized(xNewWidth, xNewHeight);
	}

	void MainSystem::windowClosed()
	{
		mNeedShutdown = true;
	}

	void MainSystem::injectMouseMoved(const OIS::MouseEvent& e)
	{
		GraphicSystem::instance()->injectMouseMoved(e);
	}

	void MainSystem::injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		GraphicSystem::instance()->injectMousePressed(e, id);
	}

	void MainSystem::injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		GraphicSystem::instance()->injectMouseReleased(e, id);
	}

	void MainSystem::injectKeyPressed(const OIS::KeyEvent& e)
	{
		GraphicSystem::instance()->injectKeyPressed(e);
	}

	void MainSystem::injectKeyReleased(const OIS::KeyEvent& e)
	{
		GraphicSystem::instance()->injectKeyReleased(e);
	}
}
