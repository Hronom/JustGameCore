#include "StatesSystem.h"

StatesSystem::StatesSystem(ISystemsListener *xMainListener)
{
	mMainListener = xMainListener;

	mLoadState = 0;
	mCurrentState = 0;

	mCurrentStateName = "";
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

void StatesSystem::needUpdate(const Ogre::FrameEvent& evt)
{
	if(mCurrentState != 0) mCurrentState->needUpdate(evt);
}

void StatesSystem::injectMouseMoved(const OIS::MouseEvent& e)
{
	if(mCurrentState != 0) mCurrentState->mouseMoved(e);
}

void StatesSystem::injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	if(mCurrentState != 0) mCurrentState->mousePressed(e, id);
}

void StatesSystem::injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	if(mCurrentState != 0) mCurrentState->mouseReleased(e, id);
}

void StatesSystem::injectKeyPressed(const OIS::KeyEvent& e)
{
	if(mCurrentState != 0) mCurrentState->keyPressed(e);
}

void StatesSystem::injectKeyReleased(const OIS::KeyEvent& e)
{
	if(mCurrentState != 0) mCurrentState->keyReleased(e);
}

void StatesSystem::injectStateLoadProgress(int xProgressValue, std::string xText)
{
	if(mLoadState != 0) mLoadState->setProgress(xProgressValue, xText);
}

void StatesSystem::setLoadState(ILoadScreen *xLoadState)
{
	mLoadState = xLoadState;
}

void StatesSystem::addNormalState(std::string xStateName, IState *xState)
{
	mStatesMap[xStateName] = xState;
}

void StatesSystem::switchToState(std::string xStateName, bool xShowLoadScreen)
{
	mMainListener->stateStartLoad();

	if(mStatesMap.count(xStateName) > 0)
	{
		if(mCurrentState != 0)
		{
			mCurrentState->exit();
			mCurrentState = 0;
		}

		if(xShowLoadScreen == true && mLoadState != 0)
		{
			mLoadState->show();
			mCurrentState = mStatesMap[xStateName];
			mCurrentState->prepareState();
			mLoadState->hide();
			
			mCurrentState->enter();
		}
		else
		{
			mCurrentState = mStatesMap[xStateName];
			mCurrentState->prepareState();
			mCurrentState->enter();
		}

		mCurrentStateName = xStateName;
	}

	mMainListener->stateEndLoad();
}

std::string StatesSystem::getCurrentStateName()
{
	return mCurrentStateName;
}
