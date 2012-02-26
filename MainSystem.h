#ifndef MAINSYSTEM_H
#define MAINSYSTEM_H

#include <string>

#include "ISystemsListener.h"
#include "IState.h"
#include "PhysicsDebugDrawer.h"

namespace JGC
{
	class MainSystem : public ISystemsListener
	{
	private:
		static MainSystem* mInstance;

		PhysicsDebugDrawer *mPhysicsDebugDrawer;

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

		void needSwitchToState(std::string xStateName, bool xShowLoadScreen = false);
		void stateLoadProgress(int xProgressValue, std::string xText);
		void needShutdown();

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