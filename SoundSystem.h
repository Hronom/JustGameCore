#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "ISystemsListener.h"
#include "SoundSource.h"
#include "SoundListener.h"

#include <al.h>
#include <alc.h>
#include <alut.h>

namespace JGC
{
	class MainSystem;
	namespace Sound
	{
		class SoundSystem
		{
			friend class MainSystem;
			struct SndInfo
			{
				ALuint ID;
				std::string Filename;
				ALsizei Rate;
				ALenum Format;
			};

		private:
			static SoundSystem* mInstance;

			ISystemsListener *mMainListener;

			ALCdevice *mALCdevice;
			ALCcontext *mALCcontext;

			SoundListener *mSoundListener;

			std::map<ALuint, SndInfo> mBuffers;
			std::list<SoundSource*> mSoundSources;

		public:
			static void initialize(ISystemsListener *xMainListener);
			static void shutdown();
			static SoundSystem* instance();

		private:
			SoundSystem(ISystemsListener *xMainListener);
			~SoundSystem();
			bool loadWavFile(const std::string &xFilename, ALuint &xSourceID);
			ALboolean CheckALCError();
			ALboolean CheckALError();

		public:
			SoundListener* getSoundListener();
			SoundSource* createSoundSource(float xPosX, float xPosY, float xPosZ, const std::string &xFilename, bool xLooped);
			void destroySoundSource(SoundSource* xSoundNode);
		};
	}
}

#endif

