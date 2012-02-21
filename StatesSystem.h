#ifndef STATESSYSTEM_H
#define STATESSYSTEM_H

#include "ISystemsListener.h"
#include "ILoadScreen.h"
#include "IState.h"
namespace JGC
{
	namespace States
	{
		class StatesSystem
		{
		private:
			ISystemsListener *mMainListener;

			ILoadScreen *mLoadState;
			IState *mCurrentState;
			std::map<std::string, IState*> mStatesMap;

			std::string mCurrentStateName;

		public:
			StatesSystem(ISystemsListener *xMainListener);
			~StatesSystem();

			bool init();

			virtual void needUpdate(const Ogre::FrameEvent& evt);
			virtual	void injectMouseMoved(const OIS::MouseEvent& e);
			virtual void injectMousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
			virtual void injectMouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
			virtual	void injectKeyPressed(const OIS::KeyEvent& e);
			virtual void injectKeyReleased(const OIS::KeyEvent& e);
			virtual void injectStateLoadProgress(int xProgressValue, std::string xText);

			void setLoadState(ILoadScreen *xLoadState);
			void addNormalState(std::string xStateName, IState *xState);
			void switchToState(std::string xStateName, bool xShowLoadScreen = false);
			std::string getCurrentStateName();
		};
	}
}

#endif
