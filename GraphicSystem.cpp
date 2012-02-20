#include "GraphicSystem.h"

GraphicSystem::GraphicSystem(ISystemsListener *xMainListener, Ogre::String xOgreCfg, Ogre::String xPluginsCfg,	Ogre::String xResourcesCfg, Ogre::String xOgreLogFile, Ogre::String xMyGUILogFile) 
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
	// 10 ��������
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
	// 1 �������� ����
	//-----------------------------------------------------
	mRoot = new Ogre::Root(mPluginsCfg, mOgreCfg, mOgreLogFile);

	//-----------------------------------------------------
	// 2 ��������� ���������� � �������� ����
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
	// 3 �������� �������� �� �������
	//-----------------------------------------------------
	Ogre::ConfigFile xConfigFile;
	xConfigFile.load(mResourcesCfg);

	// �������� �� ���� ������ � �����
	Ogre::ConfigFile::SectionIterator xSection = xConfigFile.getSectionIterator();

	Ogre::String xSecName, xTypeName, xArchName;
	while (xSection.hasMoreElements())
	{
		xSecName = xSection.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = xSection.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end();   i++)
		{
			xTypeName = i->first;
			xArchName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				xArchName, xTypeName, xSecName);
		}
	}

	// ������ ��� �������
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//-----------------------------------------------------
	// 4 �������� ��������� ����
	//
	//		ST_GENERIC = octree
	//		ST_EXTERIOR_CLOSE = simple terrain
	//		ST_EXTERIOR_FAR = nature terrain (depreciated)
	//		ST_EXTERIOR_REAL_FAR = paging landscape
	//		ST_INTERIOR = Quake3 BSP
	//----------------------------------------------------- 
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC); 

	//----------------------------------------------------- 
	// 5 �������� ������
	//----------------------------------------------------- 
	mCamera = mSceneManager->createCamera("MainCamera"); 
	mCamera->setPosition(Ogre::Vector3(0,0,100));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);

	//----------------------------------------------------- 
	// 6 �������� ��������
	//----------------------------------------------------- 
	Ogre::Viewport* xViewport = mRenderWindow->addViewport(mCamera);
	xViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	mCamera->setAspectRatio(Ogre::Real(xViewport->getActualWidth()) / Ogre::Real(xViewport->getActualHeight()));

	// Set ambient light
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	//----------------------------------------------------- 
	// 7 ������������� MyGUI
	//----------------------------------------------------- 
	mOgrePlatform = new MyGUI::OgrePlatform();
	mOgrePlatform->initialise(mRenderWindow, mSceneManager, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mMyGUILogFile); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mMyGUI = new MyGUI::Gui();
	mMyGUI->initialise();

	//----------------------------------------------------- 
	// 8 ���������� ��������� �������
	//----------------------------------------------------- 
	Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRoot->addFrameListener(this);

	return true;
}

void GraphicSystem::start()
{
	//----------------------------------------------------
	// 9 ����� �������
	//----------------------------------------------------
	mRoot->startRendering(); // ���� ��������� ����� ������������� ����� false ( ��� � ���������� ��� ������� ESCAPE )
}

bool GraphicSystem::frameStarted(const  Ogre::FrameEvent& evt) 
{
	return mMainListener->frameStarted(evt);
}

bool GraphicSystem::frameEnded(const Ogre::FrameEvent& evt)
{
	return mMainListener->frameEnded(evt);
}

void GraphicSystem::windowResized(Ogre::RenderWindow* xRenderWindow)
{
	mMainListener->windowResized(xRenderWindow->getWidth(), xRenderWindow->getHeight());
}

void GraphicSystem::windowClosed(Ogre::RenderWindow* xRenderWindow)
{
	mMainListener->windowClosed();
}

void GraphicSystem::injectMouseMoved( const OIS::MouseEvent &arg )
{
	//mMyGUI->injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
	MyGUI::InputManager::getInstancePtr()->injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
}

void GraphicSystem::injectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//mMyGUI->injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	MyGUI::InputManager::getInstancePtr()->injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
}

void GraphicSystem::injectMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//mMyGUI->injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
	MyGUI::InputManager::getInstancePtr()->injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
}

void GraphicSystem::injectKeyPressed( const OIS::KeyEvent &arg )
{
	//mMyGUI->injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
	MyGUI::InputManager::getInstancePtr()->injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
}

void GraphicSystem::injectKeyReleased( const OIS::KeyEvent &arg )
{
	//mMyGUI->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
	MyGUI::InputManager::getInstancePtr()->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
}

void GraphicSystem::needSingleUpdate()
{
	Ogre::WindowEventUtilities::messagePump();
	mRoot->renderOneFrame();

	//MyGUI::InputManager::getInstancePtr()->resetMouseCaptureWidget();
	//MyGUI::InputManager::getInstancePtr()->resetKeyFocusWidget();
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

Ogre::SceneManager* GraphicSystem::getSceneManager()
{
	return mSceneManager;
}

Ogre::Camera* GraphicSystem::getCamera()
{
	return mCamera;
}

MyGUI::Gui* GraphicSystem::getGui()
{
	return mMyGUI;
}