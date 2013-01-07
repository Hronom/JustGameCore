#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OGRE\OgreWindowEventUtilities.h>

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#include <OIS\OIS.h>

#include "ISystemsListener.h"

#include <QString>
#include <QHash>

namespace JGC
{
	class GraphicSystem : public Ogre::FrameListener, public Ogre::WindowEventListener
	{
	private: 
		static GraphicSystem* mInstance;

		ISystemsListener *mMainListener;

		Ogre::String mOgreCfg;
		Ogre::String mPluginsCfg;
		Ogre::String mResourcesCfg;
		Ogre::String mOgreLogFile;
		Ogre::String mMyGUILogFile;

		Ogre::Root *mRoot;
		Ogre::RenderWindow *mRenderWindow;
        Ogre::SceneManager *mActiveSceneManager;
        QHash<QString, Ogre::SceneManager *> mSceneManagers;

		MyGUI::OgrePlatform *mOgrePlatform;
		MyGUI::Gui *mMyGUI;

	public:
		static bool initialize(ISystemsListener *xMainListener,
			Ogre::String xOgreCfg,
			Ogre::String xPluginsCfg,
			Ogre::String xResourcesCfg,
			Ogre::String xOgreLogFile,
			Ogre::String xMyGUILogFile);
		static void shutdown();
		static GraphicSystem* instance();

	private: 
		GraphicSystem(ISystemsListener *xMainListener,
			Ogre::String xOgreCfg,
			Ogre::String xPluginsCfg,
			Ogre::String xResourcesCfg,
			Ogre::String xOgreLogFile,
			Ogre::String xMyGUILogFile);
		~GraphicSystem();

		bool init();
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);
		virtual void windowResized(Ogre::RenderWindow* xRenderWindow);
		virtual void windowClosed(Ogre::RenderWindow* xRenderWindow);

	public:
		void injectUpdate();
		void injectMouseMoved( const OIS::MouseEvent &arg );
		void injectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		void injectMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		void injectKeyPressed( const OIS::KeyEvent &arg );
		void injectKeyReleased( const OIS::KeyEvent &arg );

		unsigned int getWinHandle();
		unsigned int getWinWidth();
		unsigned int getWinHeight();

        void createSceneManager(QString xSceneManagerName);
        void deleteSceneManager(QString xSceneManagerName);
        Ogre::SceneManager* getSceneManager(QString xSceneManagerName);

        void setActiveSceneManager(QString xSceneManagerName);
        Ogre::SceneManager* getActiveSceneManager();

        Ogre::Camera* getCamera(QString xSceneManagerName);
		MyGUI::Gui* getGui();

        Ogre::Vector3 getMouseWorldCoord(Ogre::Real xDistance);
	};
}

#endif
