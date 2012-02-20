#ifndef MAINSYSTEM_H
#define MAINSYSTEM_H

#include <string>

#include "ISystemsListener.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "StatesSystem.h"
#include "IState.h"

class MainSystem : public ISystemsListener, public ICore
{
private:
	GraphicSystem *mGraphicSystem;
	PhysicsSystem *mPhysicsSystem;
	InputSystem *mInputSystem;
	StatesSystem *mStatesSystem;

	bool mNeedShutdown;
	bool mStateLoad;


	std::string mCurrentStateName;
	bool mShowLoadScreen;

public:
	MainSystem(Ogre::String xOgreCfg = "Ogre.cfg",
		Ogre::String xPluginsCfg = "Plugins.cfg",
		Ogre::String xResourcesCfg = "Resources.cfg",
		Ogre::String xOgreLogFile = "Ogre.log",
		Ogre::String xMyGUILogFile = "MyGUI.log");
	virtual ~MainSystem();
	bool init();
	void run();

	void setLoadState(ILoadScreen *xLoadState);
	void addNormalState(std::string xStateName, IState *xState);

	//-------------------------------------------------------------
	// ISystemsListener
	//-------------------------------------------------------------
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);
	virtual void windowResized(unsigned int xNewWidth, unsigned int xNewHeight);
	virtual void windowClosed();
	virtual	void mouseMoved(const OIS::MouseEvent& e);
	virtual void mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	virtual	void keyPressed(const OIS::KeyEvent& e);
	virtual void keyReleased(const OIS::KeyEvent& e);

	virtual void stateStartLoad();
	virtual void stateEndLoad();

	//-------------------------------------------------------------
	// ICore
	//-------------------------------------------------------------
	virtual void needSwitchToState(std::string xStateName, bool xShowLoadScreen = false);
	virtual void stateLoadProgress(int xProgressValue, std::string xText);
	virtual void needShutdown();

	virtual Ogre::SceneManager* getSceneManager();
	virtual Ogre::Camera* getCamera();
	virtual	MyGUI::Gui* getGui();
	virtual OgreBulletDynamics::DynamicsWorld* getDynamicsWorld();
};

#endif