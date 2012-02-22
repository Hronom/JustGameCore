#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include <al.h>
#include <alc.h>
#include <alut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

namespace JGC
{
	namespace Sound
	{
		class SoundSource
		{
		private:
			ALuint mSourceID;
			bool mLooped;
			bool mReady;
			std::string mErrorMessage;

		public:
			SoundSource();
			virtual ~SoundSource();

			void play();
			void stop();
			void move(float xNewX, float xNewY, float xNewZ);
			bool isReady();
			std::string getErrorMessage();

		private:
			void init(ALuint xSourceID, bool xLooped);
			void setError(std::string xErrorMessage, bool xReady);

			friend class SoundSystem;
		};
	}
}

#endif