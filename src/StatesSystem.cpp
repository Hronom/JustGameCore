#include "StatesSystem.h"
#include "GraphicSystem.h"

namespace JGC
{
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

		mLoadScreen = 0;
		mCurrentState = 0;

		mCurrentStateName = "";
		mNeedStateName = "";
		mShowLoadScreen = false;
		mNeedStateSwitch = false;
		mStateSwitching = false;
	}

	StatesSystem::~StatesSystem()
	{
		if(mLoadScreen != 0)
		{
			delete mLoadScreen;
			mLoadScreen = 0;
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
		if(mCurrentState != 0) mCurrentState->injectUpdate(xTimeSinceLastFrame);
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
		if(mLoadScreen != 0)
		{
			mLoadScreen->setProgress(xProgressValue, xText);
			GraphicSystem::instance()->injectUpdate();
		}
	}

	void StatesSystem::needSwitchToState(std::string xStateName, bool xShowLoadScreen)
	{
		if(mCurrentStateName != xStateName)
		{
			mNeedStateName = xStateName;
			mShowLoadScreen = xShowLoadScreen;
			mNeedStateSwitch = true;
		}
	}

	void StatesSystem::switchStateIfNeed()
	{
		if(mNeedStateSwitch == true)
		{
			mStateSwitching = true;

			if(mStatesMap.count(mNeedStateName) > 0)
			{
				if(mCurrentState != 0)
				{
					mCurrentState->exit();
					mCurrentState = 0;
				}

				if(mShowLoadScreen == true && mLoadScreen != 0)
				{
					mLoadScreen->show();
					mCurrentState = mStatesMap[mNeedStateName];
					mCurrentState->prepareState();
					mLoadScreen->hide();
				}
				else
				{
					mCurrentState = mStatesMap[mNeedStateName];
					mCurrentState->prepareState();
				}

				mCurrentState->enter();

				mCurrentStateName = mNeedStateName;
			}
			else
			{
				mNeedStateName = mCurrentStateName;
			}

			mNeedStateSwitch = false;			
			mStateSwitching = false;
		}
	}

	void StatesSystem::setLoadState(ILoadScreen *xLoadScreen)
	{
		mLoadScreen = xLoadScreen;
	}

	void StatesSystem::addNormalState(std::string xStateName, IState *xState)
	{
		mStatesMap[xStateName] = xState;
	}
}
