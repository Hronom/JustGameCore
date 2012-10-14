#include "SoundSource.h"

namespace JGC
{
	SoundSource::SoundSource()
	{
		mReady = true;
		mErrorMessage = "";
	}

	SoundSource::~SoundSource()
	{
		alSourceStop(mSourceID);
		if (alIsSource(mSourceID)) alDeleteSources(1, &mSourceID);
	}

	void SoundSource::play()
	{
		alSourcePlay(mSourceID);
	}

	void SoundSource::stop()
	{
		alSourceStop(mSourceID);
	}

	void SoundSource::move(float xNewX, float xNewY, float xNewZ)
	{
		ALfloat xPos[3] = { xNewX, xNewY, xNewZ };
		alSourcefv(mSourceID, AL_POSITION, xPos);
	}

	bool SoundSource::isReady()
	{
		return mReady;
	}

	std::string SoundSource::getErrorMessage()
	{
		return mErrorMessage;
	}

	void SoundSource::init(ALuint xSourceID, bool xLooped)
	{
		mSourceID = xSourceID;
		mLooped = xLooped;
	}

	void SoundSource::setError(std::string xErrorMessage, bool xReady)
	{
		mErrorMessage = xErrorMessage;
		mReady = xReady;
	}
}
