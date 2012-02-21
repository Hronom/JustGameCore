#include "MainSystem.h"
using namespace JGC;

MainSystem* MainSystem::mInstance = 0;

MainSystem::MainSystem(Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile)
{
	mGraphicSystem = new Graphic::GraphicSystem(this, xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
	mPhysicsSystem = new Physics::PhysicsSystem(this);
	mSoundSystem = new Sound::SoundSystem(this);
	mInputSystem = new Input::InputSystem(this);
	mStatesSystem = new States::StatesSystem(this);

	mNeedShutdown = false;
	mStateLoad = false;

	mCurrentStateName = "";
	mShowLoadScreen = false;

	mGraphicSystem->init();
	mPhysicsSystem->init(mGraphicSystem->getSceneManager());
	mSoundSystem->init();
	mInputSystem->init(mGraphicSystem->getWinHandle(), mGraphicSystem->getWinWidth(), mGraphicSystem->getWinHeight());
	mStatesSystem->init();
}

MainSystem::~MainSystem()
{
	delete mStatesSystem;
	delete mInputSystem;
	delete mSoundSystem;
	delete mPhysicsSystem;
	delete mGraphicSystem;
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
	mGraphicSystem->start();
}

void  MainSystem::setLoadState(ILoadScreen *xLoadState)
{
	mStatesSystem->setLoadState(xLoadState);
}

void MainSystem::addNormalState(std::string xStateName, IState *xState)
{
	mStatesSystem->addNormalState(xStateName, xState);
}

void MainSystem::needSwitchToState(std::string xStateName, bool xShowLoadScreen)
{
	mCurrentStateName = xStateName;
	mShowLoadScreen = xShowLoadScreen;
}

void MainSystem::stateLoadProgress(int xProgressValue, std::string xText)
{
	mStatesSystem->injectStateLoadProgress(xProgressValue, xText);
	mGraphicSystem->needSingleUpdate();
}

void MainSystem::needShutdown()
{
	mNeedShutdown = true;
}

Ogre::SceneManager* MainSystem::getSceneManager()
{
	return mGraphicSystem->getSceneManager();
}

Ogre::Camera* MainSystem::getCamera()
{
	return mGraphicSystem->getCamera();
}

MyGUI::Gui* MainSystem::getGui()
{
	return mGraphicSystem->getGui();
}

OgreBulletDynamics::DynamicsWorld* MainSystem::getDynamicsWorld()
{
	return mPhysicsSystem->getDynamicsWorld();
}

//-------------------------------------------------------------
// ISystemsListener
//-------------------------------------------------------------
bool MainSystem::frameStarted(const Ogre::FrameEvent& evt)
{
	if(mStateLoad != true)
	{
		if(mStatesSystem->getCurrentStateName() != mCurrentStateName)
		{
			mStatesSystem->switchToState(mCurrentStateName, mShowLoadScreen);
		}
		else
		{
			mPhysicsSystem->needUpdate(evt);
			mInputSystem->needUpdate();
			mStatesSystem->needUpdate(evt);
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
	mInputSystem->injectWindowResized(xNewWidth, xNewHeight);
}

void MainSystem::windowClosed()
{
	mNeedShutdown = true;
}

void MainSystem::mouseMoved(const OIS::MouseEvent& e)
{
	mGraphicSystem->injectMouseMoved(e);
	mStatesSystem->injectMouseMoved(e);
}

void MainSystem::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	mGraphicSystem->injectMousePressed(e, id);
	mStatesSystem->injectMousePressed(e, id);
}

void MainSystem::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	mGraphicSystem->injectMouseReleased(e, id);
	mStatesSystem->injectMouseReleased(e, id);
}

void MainSystem::keyPressed(const OIS::KeyEvent& e)
{
	mGraphicSystem->injectKeyPressed(e);
	mStatesSystem->injectKeyPressed(e);
}

void MainSystem::keyReleased(const OIS::KeyEvent& e)
{
	mGraphicSystem->injectKeyReleased(e);
	mStatesSystem->injectKeyReleased(e);
}

void MainSystem::stateStartLoad()
{
	mStateLoad = true;
}

void MainSystem::stateEndLoad()
{
	mStateLoad = false;
}