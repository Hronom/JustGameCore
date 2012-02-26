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

		void stateLoadProgress(int xProgressValue, std::string xText);
		void needShutdown();

	private:
		//-------------------------------------------------------------
		// ISystemsListener
		//-------------------------------------------------------------
		virtual bool frameStarted(const float& xTimeSinceLastFrame);
		virtual bool frameEnded(const float& xTimeSinceLastFrame);
		virtual void windowResized(unsigned int xNewWidth, unsigned int xNewHeight);
		virtual void windowClosed();
		virtual	void injectMouseMoved(const OIS::MouseEvent& e);
		virtual void injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		virtual void injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		virtual	void injectKeyPressed(const OIS::KeyEvent& e);
		virtual void injectKeyReleased(const OIS::KeyEvent& e);
	};
}

#endif