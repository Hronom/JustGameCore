#ifndef MAINSYSTEM_H
#define MAINSYSTEM_H

#include <string>

#include "ISystemsListener.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "InputSystem.h"
#include "StatesSystem.h"
#include "IState.h"

namespace JGC
{
	class MainSystem : public ISystemsListener
	{
	private:
		static MainSystem* mInstance;

		Graphic::GraphicSystem *mGraphicSystem;
		Physics::PhysicsSystem *mPhysicsSystem;
		//Sound::SoundSystem *mSoundSystem;
		Input::InputSystem *mInputSystem;
		States::StatesSystem *mStatesSystem;

		bool mNeedShutdown;
		bool mStateLoad;

		std::string mCurrentStateName;
		bool mShowLoadScreen;

	private:
		MainSystem(Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile);
		~MainSystem();

	public:
		//singleton
		static void initialize(Ogre::String xOgreCfg = "Ogre.cfg",
			Ogre::String xPluginsCfg = "Plugins.cfg",
			Ogre::String xResourcesCfg = "Resources.cfg",
			Ogre::String xOgreLogFile = "Ogre.log",
			Ogre::String xMyGUILogFile = "MyGUI.log");
		static void shutdown();
		static MainSystem* instance();
		
		//normal
		void run();
		void setLoadState(ILoadScreen *xLoadState);
		void addNormalState(std::string xStateName, IState *xState);

		void needSwitchToState(std::string xStateName, bool xShowLoadScreen = false);
		void stateLoadProgress(int xProgressValue, std::string xText);
		void needShutdown();

		Ogre::SceneManager* getSceneManager();
		Ogre::Camera* getCamera();
		MyGUI::Gui* getGui();
		OgreBulletDynamics::DynamicsWorld* getDynamicsWorld();

	private:
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
	};
}

#endif