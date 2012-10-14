#ifndef STATESSYSTEM_H
#define STATESSYSTEM_H

#include "ISystemsListener.h"
#include "ILoadScreen.h"
#include "IState.h"

namespace JGC
{
	class StatesSystem
	{
		friend class MainSystem;
	private:
		static StatesSystem* mInstance;

		ISystemsListener *mMainListener;

		ILoadScreen *mLoadScreen;
		IState *mCurrentState;
		std::map<std::string, IState*> mStatesMap;

		std::string mCurrentStateName;
		std::string mNeedStateName;
		bool mShowLoadScreen;
		bool mNeedStateSwitch;
		bool mStateSwitching;

	public:
		static bool initialize(ISystemsListener *xMainListener);
		static void shutdown();
		static StatesSystem* instance();

	private:
		StatesSystem(ISystemsListener *xMainListener);
		~StatesSystem();
		bool init();

	public:
		void injectUpdate(const float& xTimeSinceLastFrame);
		void injectMouseMoved(const OIS::MouseEvent& e);
		void injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		void injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		void injectKeyPressed(const OIS::KeyEvent& e);
		void injectKeyReleased(const OIS::KeyEvent& e);
		void injectStateLoadProgress(int xProgressValue, std::string xText);

		void needSwitchToState(std::string xStateName, bool xShowLoadScreen = false);
		void switchStateIfNeed();

		void setLoadState(ILoadScreen *xLoadScreen);
		void addNormalState(std::string xStateName, IState *xState);
		
		bool inline isStateSwitching() {return mStateSwitching;}
	};
}

#endif
