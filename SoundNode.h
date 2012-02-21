#ifndef SOUNDNODE_H
#define SOUNDNODE_H

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
		class SoundNode
		{
			typedef struct 
			{
				unsigned int  ID;
				std::string    Filename;
				unsigned int  Rate;
				unsigned int  Format;
			} SndInfo;

		private:
			ALCdevice *mALCdevice;
			// Идентификатор источника
			ALuint mSourceID;
			// Потоковый ли наш звук?
			bool mStreamed;
			ALfloat mVel[3];
			ALfloat mPos[3];
			bool mLooped;

			std::map<ALuint, SndInfo> mBuffers;

		public:
			SoundNode(ALCdevice *xALCdevice);
			virtual ~SoundNode();

			bool open(const std::string &xFilename, bool xLooped, bool xStreamed);
			bool isStreamed();
			void play();
			void close();
			void update();
			void move(float xNewX, float xNewY, float xNewZ);
			void stop();

		private:
			bool loadWavFile (const std::string &xFilename);
			ALboolean CheckALCError();
			ALboolean CheckALError();
		};
	}
}

#endif