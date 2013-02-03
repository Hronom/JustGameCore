#include "GraphicSystem.h"

#include <QDebug>

namespace JGC
{
    GraphicSystem* GraphicSystem::mInstance = 0;

    bool GraphicSystem::initialize(ISystemsListener *xMainListener, Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile)
    {
        mInstance = new GraphicSystem(xMainListener, xOgreCfg, xPluginsCfg, xResourcesCfg, xOgreLogFile, xMyGUILogFile);
        return mInstance->init();
    }

    void GraphicSystem::shutdown()
    {
        qDebug()<<"Shutdown:"<<"GraphicSystem";
        delete mInstance;
        mInstance = 0;
    }

    GraphicSystem* GraphicSystem::instance()
    {
        return mInstance;
    }

    GraphicSystem::GraphicSystem(ISystemsListener *xMainListener, Ogre::String xOgreCfg, Ogre::String xPluginsCfg, Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile)
    {
        mMainListener = xMainListener;

        mOgreCfg = xOgreCfg;
        mPluginsCfg = xPluginsCfg;
        mResourcesCfg = xResourcesCfg;
        mOgreLogFile = xOgreLogFile;
        mMyGUILogFile = xMyGUILogFile;
    }

    GraphicSystem::~GraphicSystem()
    {
        //----------------------------------------------------
        // 10 удаление
        //----------------------------------------------------
        // MyGUI
        mMyGUI->shutdown();
        delete mMyGUI;
        mMyGUI = 0;

        mOgrePlatform->shutdown();
        delete mOgrePlatform;
        mOgrePlatform = 0;

        Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWindow, this);

        // Ogre
        delete mRoot;
        mRoot = 0;
    }

    bool GraphicSystem::init()
    {
        //-----------------------------------------------------
        // 1 Создание рута
        //-----------------------------------------------------
        mRoot = new Ogre::Root(mPluginsCfg, mOgreCfg, mOgreLogFile);

        //-----------------------------------------------------
        // 2 Настройка приложение и создание окна
        //-----------------------------------------------------
        if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
        {
            delete mRoot;
            return false;
        }

        mRenderWindow = mRoot->initialise(true, "JustGame");

        // Set default mipmap level (note: some APIs ignore this)
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

        //-----------------------------------------------------
        // 3 Загрузка ресурсов из конфига
        //-----------------------------------------------------
        Ogre::ConfigFile xConfigFile;
        xConfigFile.load(mResourcesCfg);

        // Проходим по всем секция в файле
        Ogre::ConfigFile::SectionIterator xSection = xConfigFile.getSectionIterator();

        Ogre::String xSecName, xTypeName, xArchName;
        while(xSection.hasMoreElements())
        {
            xSecName = xSection.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = xSection.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for(i = settings->begin(); i != settings->end(); ++i)
            {
                xTypeName = i->first;
                xArchName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                            xArchName, xTypeName, xSecName);
            }
        }

        // Грузим все ресурсы
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        //-----------------------------------------------------
        // 7 Инициализация MyGUI
        //-----------------------------------------------------
        mOgrePlatform = new MyGUI::OgrePlatform();
        mOgrePlatform->initialise(mRenderWindow, 0, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mMyGUILogFile);
        mMyGUI = new MyGUI::Gui();
        mMyGUI->initialise();

        //-----------------------------------------------------
        // 8 Добавление слушателя событий
        //-----------------------------------------------------
        Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
        mRoot->addFrameListener(this);

        mActiveSceneManager = 0;

        return true;
    }

    bool GraphicSystem::frameStarted(const  Ogre::FrameEvent& evt)
    {
        return mMainListener->frameStarted(evt.timeSinceLastFrame);
    }

    bool GraphicSystem::frameEnded(const Ogre::FrameEvent& evt)
    {
        return mMainListener->frameEnded(evt.timeSinceLastFrame);
    }

    void GraphicSystem::windowResized(Ogre::RenderWindow* xRenderWindow)
    {
        mMainListener->windowResized(xRenderWindow->getWidth(), xRenderWindow->getHeight());
    }

    void GraphicSystem::windowClosed(Ogre::RenderWindow* xRenderWindow)
    {
        mMainListener->windowClosed();
    }

    void GraphicSystem::injectUpdate()
    {
        Ogre::WindowEventUtilities::messagePump();
        mRoot->renderOneFrame();
    }

    void GraphicSystem::injectMouseMoved( const OIS::MouseEvent &arg )
    {
        MyGUI::InputManager::getInstancePtr()->injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
    }

    void GraphicSystem::injectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
    {
        MyGUI::InputManager::getInstancePtr()->injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    }

    void GraphicSystem::injectMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
    {
        MyGUI::InputManager::getInstancePtr()->injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    }

    void GraphicSystem::injectKeyPressed( const OIS::KeyEvent &arg )
    {
        MyGUI::InputManager::getInstancePtr()->injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
    }

    void GraphicSystem::injectKeyReleased( const OIS::KeyEvent &arg )
    {
        MyGUI::InputManager::getInstancePtr()->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    }

    size_t GraphicSystem::getWinHandle()
    {
        size_t xWinHandle = 0;
        mRenderWindow->getCustomAttribute("WINDOW", &xWinHandle);

        return xWinHandle;
    }

    unsigned int GraphicSystem::getWinWidth()
    {
        return mRenderWindow->getWidth();
    }

    unsigned int GraphicSystem::getWinHeight()
    {
        return mRenderWindow->getHeight();
    }

    float GraphicSystem::getLastFPS()
    {
        return mRenderWindow->getLastFPS();
    }

    void GraphicSystem::createSceneManager(QString xSceneManagerName)
    {
        //-----------------------------------------------------
        // 4 Создание менеджера сцен
        //
        //		ST_GENERIC = octree
        //		ST_EXTERIOR_CLOSE = simple terrain
        //		ST_EXTERIOR_FAR = nature terrain (depreciated)
        //		ST_EXTERIOR_REAL_FAR = paging landscape
        //		ST_INTERIOR = Quake3 BSP
        //-----------------------------------------------------
        Ogre::SceneManager *xSceneManager;
        xSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, xSceneManagerName.toStdString());
        xSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

        mSceneManagers.insert(xSceneManagerName, xSceneManager);

        //-----------------------------------------------------
        // 5 Создание камеры
        //-----------------------------------------------------
        Ogre::Camera *xCamera;
        xCamera = xSceneManager->createCamera("MainCamera");
        xCamera->setPosition(Ogre::Vector3(0,0,100));
        xCamera->lookAt(Ogre::Vector3(0,0,0));
        xCamera->setNearClipDistance(5);
    }

    void GraphicSystem::deleteSceneManager(QString xSceneManagerName)
    {
        if(mSceneManagers.contains(xSceneManagerName))
        {
            Ogre::SceneManager *xSceneManager;
            xSceneManager = mSceneManagers.take(xSceneManagerName);
            mRoot->destroySceneManager(xSceneManager);
        }
    }

    Ogre::SceneManager* GraphicSystem::getSceneManager(QString xSceneManagerName)
    {
        if(mSceneManagers.contains(xSceneManagerName))
            return mSceneManagers.value(xSceneManagerName);
        else
            return 0;
    }

    void GraphicSystem::setActiveSceneManager(QString xSceneManagerName)
    {
        if(mSceneManagers.contains(xSceneManagerName))
        {
            if(mActiveSceneManager != 0)
                mActiveSceneManager->setDisplaySceneNodes(false);

            mActiveSceneManager = mSceneManagers.value(xSceneManagerName);

            //-----------------------------------------------------
            // Создание вьюпорта
            //-----------------------------------------------------
            mRenderWindow->removeAllViewports();
            Ogre::Camera *xCamera = mActiveSceneManager->getCamera("MainCamera"); //The Camera
            Ogre::Viewport* xViewport = mRenderWindow->addViewport(xCamera);
            xViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

            xCamera->setAspectRatio(Ogre::Real(xViewport->getActualWidth()) / Ogre::Real(xViewport->getActualHeight()));

            //-----------------------------------------------------
            // Обновление данных MyGUI
            //-----------------------------------------------------
            mOgrePlatform->getRenderManagerPtr()->setSceneManager(mActiveSceneManager);
            mOgrePlatform->getRenderManagerPtr()->setActiveViewport(0);
        }
    }

    Ogre::SceneManager* GraphicSystem::getActiveSceneManager()
    {
        return mActiveSceneManager;
    }

    Ogre::Camera* GraphicSystem::getCamera(QString xSceneManagerName)
    {
        if(mSceneManagers.contains(xSceneManagerName))
            return mSceneManagers.value(xSceneManagerName)->getCamera("MainCamera");
        else
            return 0;
    }

    MyGUI::Gui* GraphicSystem::getGui()
    {
        return mMyGUI;
    }

    Ogre::Vector3 GraphicSystem::getMouseWorldCoord(Ogre::Real xDistance)
    {
        MyGUI::IntPoint xMousePosition = MyGUI::InputManager::getInstancePtr()->getMousePosition();
        MyGUI::IntSize xSize = MyGUI::RenderManager::getInstance().getViewSize();
        Ogre::Ray xRay = mActiveSceneManager->getCamera("MainCamera")->getCameraToViewportRay(xMousePosition.left / float(xSize.width), xMousePosition.top / float(xSize.height));
        return xRay.getPoint(xDistance);
    }
}
