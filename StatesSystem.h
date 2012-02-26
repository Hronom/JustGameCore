#ifndef STATESSYSTEM_H
#define STATESSYSTEM_H

#include "ISystemsListener.h"
#include "ILoadScreen.h"
#include "IState.h"
namespace JGC
{
	class MainSystem;
	namespace States
	{
		class StatesSystem
		{
			friend class MainSystem;
		private:
			static StatesSystem* mInstance;

			ISystemsListener *mMainListener;

			ILoadScreen *mLoadState;
			IState *mCurrentState;
			std::map<std::string, IState*> mStatesMap;

			std::string mCurrentStateName;
			std::string mNeedStateName;

			bool mShowLoadScreen;
			bool mStateSwitching;
			bool mBeginStateSwitch;

		public:
			static bool initialize(ISystemsListener *xMainListener);
			static void shutdown();
			static StatesSystem* instance();

		private:
			StatesSystem(ISystemsListener *xMainListener);
			~StatesSystem();
			bool init();
			virtual void injectUpdate(const float& xTimeSinceLastFrame);
			virtual	void injectMouseMoved(const OIS::MouseEvent& e);
			virtual void injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
			virtual void injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
			virtual	void injectKeyPressed(const OIS::KeyEvent& e);
			virtual void injectKeyReleased(const OIS::KeyEvent& e);
			virtual void injectStateLoadProgress(int xProgressValue, std::string xText);
			void beginStateSwitch();
			bool inline isStateSwitching() {return mStateSwitching;}

		public:
			void needSwitchToState(std::string xStateName, bool xShowLoadScreen = false);
			void setLoadState(ILoadScreen *xLoadState);
			void addNormalState(std::string xStateName, IState *xState);
			std::string getCurrentStateName();
		};
	}
}

#endif
