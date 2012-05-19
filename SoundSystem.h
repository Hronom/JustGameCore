#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "ISystemsListener.h"
#include "SoundSource.h"
#include "SoundListener.h"

#include <al.h>
#include <alc.h>

namespace JGC
{
	class SoundSystem
	{
		struct SoundInfo
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

		std::map<ALuint, SoundInfo> mBuffers;
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

#endif

