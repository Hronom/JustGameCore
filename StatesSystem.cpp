#include "StatesSystem.h"
using namespace JGC;
using namespace JGC::States;

StatesSystem* StatesSystem::mInstance = 0;

bool StatesSystem::initialize(ISystemsListener *xMainListener)
{
	mInstance = new StatesSystem(xMainListener);
	return mInstance->init();
}

void StatesSystem::shutdown()
{
	delete mInstance;
	mInstance = 0;
}

StatesSystem* StatesSystem::instance()
{
	return mInstance;
}

StatesSystem::StatesSystem(ISystemsListener *xMainListener)
{
	mMainListener = xMainListener;

	mLoadState = 0;
	mCurrentState = 0;

	mCurrentStateName = "";
	mNeedStateName = "";
	mShowLoadScreen = false;
	mStateSwitching = false;
	mBeginStateSwitch = false;
}

StatesSystem::~StatesSystem()
{
	if(mLoadState != 0)
	{
		delete mLoadState;
		mLoadState = 0;
	}

	std::map<std::string, IState*>::iterator xElement;
	xElement = mStatesMap.begin();
	while(xElement != mStatesMap.end())
	{
		delete xElement->second;
		++xElement;
	}
	mStatesMap.clear();
}

bool StatesSystem::init()
{
	return true;
}

void StatesSystem::injectUpdate(const float& xTimeSinceLastFrame)
{
	if(mBeginStateSwitch == true) beginStateSwitch();
	if(mCurrentState != 0 && mStateSwitching == false) mCurrentState->injectUpdate(xTimeSinceLastFrame);
}

void StatesSystem::injectMouseMoved(const OIS::MouseEvent& e)
{
	if(mCurrentState != 0) mCurrentState->injectMouseMoved(e);
}

void StatesSystem::injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	if(mCurrentState != 0) mCurrentState->injectMousePressed(e, id);
}

void StatesSystem::injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	if(mCurrentState != 0) mCurrentState->injectMouseReleased(e, id);
}

void StatesSystem::injectKeyPressed(const OIS::KeyEvent& e)
{
	if(mCurrentState != 0) mCurrentState->injectKeyPressed(e);
}

void StatesSystem::injectKeyReleased(const OIS::KeyEvent& e)
{
	if(mCurrentState != 0) mCurrentState->injectKeyReleased(e);
}

void StatesSystem::injectStateLoadProgress(int xProgressValue, std::string xText)
{
	if(mLoadState != 0) mLoadState->setProgress(xProgressValue, xText);
}

void StatesSystem::beginStateSwitch()
{
	mBeginStateSwitch = false;

	if(mStatesMap.count(mNeedStateName) > 0)
	{
		if(mCurrentState != 0)
		{
			mCurrentState->exit();
			mCurrentState = 0;
		}

		if(mShowLoadScreen == true && mLoadState != 0)
		{
			mLoadState->show();
			mCurrentState = mStatesMap[mNeedStateName];
			mCurrentState->prepareState();
			mLoadState->hide();
			
			mCurrentState->enter();
		}
		else
		{
			mCurrentState = mStatesMap[mNeedStateName];
			mCurrentState->prepareState();
			mCurrentState->enter();
		}

		mCurrentStateName = mNeedStateName;
	}

	mStateSwitching = false;
}

void StatesSystem::needSwitchToState(std::string xStateName, bool xShowLoadScreen)
{
	mNeedStateName = xStateName;
	mShowLoadScreen = xShowLoadScreen;
	mStateSwitching = true;
	mBeginStateSwitch = true;
}

void StatesSystem::setLoadState(ILoadScreen *xLoadState)
{
	mLoadState = xLoadState;
}

void StatesSystem::addNormalState(std::string xStateName, IState *xState)
{
	mStatesMap[xStateName] = xState;
}

std::string StatesSystem::getCurrentStateName()
{
	return mCurrentStateName;
}
