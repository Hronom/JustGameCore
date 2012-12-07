#include "SoundSystem.h"

#include <QDebug>

namespace JGC
{
	SoundSystem* SoundSystem::mInstance = 0;

	void SoundSystem::initialize(ISystemsListener *xMainListener)
	{
		mInstance = new SoundSystem(xMainListener);
	}

	void SoundSystem::shutdown()
	{
        qDebug()<<"Shutdown:"<<"SoundSystem";
		delete mInstance;
		mInstance = 0;
	}

	SoundSystem* SoundSystem::instance()
	{
		return mInstance;
	}

	SoundSystem::SoundSystem(ISystemsListener *xMainListener)
	{
		mMainListener = xMainListener;
		mSoundListener = 0;

		// ��������� �������� �� ��������� ����������
		mALCdevice = alcOpenDevice("");
		// �������� �� ������
		if (!mALCdevice)
		{
			//ERRMSG("Default sound device not present");
			return;
		}
		// ������� �������� ����������
		mALCcontext = alcCreateContext(mALCdevice, 0);
		if (!CheckALCError()) return;

		// ������ �������� �������
		alcMakeContextCurrent(mALCcontext);

		mSoundListener = new SoundListener(0.0, 0.0, 0.0);
	}

	SoundSystem::~SoundSystem()
	{
		std::list<SoundSource*>::iterator xElement;
		xElement = mSoundSources.begin();
		while(xElement != mSoundSources.end())
		{
			delete (*xElement);
			++xElement;
		}
		mSoundSources.clear();

		if(mSoundListener != 0)
		{
			delete mSoundListener;
			mSoundListener = 0;
		}

		// ������� ��� �������
		for (std::map<ALuint, SoundInfo>::iterator i = mBuffers.begin(); i != mBuffers.end(); i++)
			alDeleteBuffers(1, &i->second.ID);
		// ��������� ������� ��������
		alcMakeContextCurrent(0);
		// ���������� ��������
		alcDestroyContext(mALCcontext);
		// ��������� �������� ����������
		alcCloseDevice(mALCdevice);
	}

	bool SoundSystem::loadWavFile(const std::string &xFilename, ALuint &xSourceID)
	{
        // ��������� ���������� ��������������
        SoundInfo xBuffer;
        // ������ ������ � ������
        ALenum xFormat;
        // ��������� �� ������ ������ �����
        ALvoid *xData;
        // ������ ����� �������
        ALsizei xSize;
        // ������� ����� � ������
        ALsizei xFreq;
        // ������������� ������������ ���������������
        ALboolean xLoop;
        // ������������� ������
        ALuint xBufID = 0;

        // ��������� SoundInfo �������
        xBuffer.Filename = xFilename;
        // ����, � ��� �� ��� ������������� ������ � ������ ������?
        for (std::map<ALuint, SoundInfo>::iterator i = mBuffers.begin(); i != mBuffers.end(); i++)
        {
            if (i->second.Filename == xFilename) xBufID = i->first;
        }

        // ���� ���� ��������� �������
        if (!xBufID)
        {
            // ������ �����
            alGenBuffers(1, &xBuffer.ID);
            if (!CheckALError()) return false;
            // ��������� ������ �� wav �����
            alutLoadWAVFile((ALbyte *)xFilename.data(), &xFormat, &xData,	&xSize, &xFreq, &xLoop);
            if (!CheckALError()) return false;

            xBuffer.Format = xFormat;
            xBuffer.Rate = xFreq;
            // ��������� ����� �������
            alBufferData(xBuffer.ID, xFormat, xData, xSize, xFreq);
            if (!CheckALError()) return false;
            // ��������� ���� �� �������������
            alutUnloadWAV(xFormat, xData, xSize, xFreq);
            if (!CheckALError()) return false;

            // ��������� ���� ����� � ������
            mBuffers[xBuffer.ID] = xBuffer;
        }
        else
            xBuffer = mBuffers[xBufID];

        // ����������� ����� � ����������
        alSourcei (xSourceID, AL_BUFFER, xBuffer.ID);

		return true;
	}

	ALboolean SoundSystem::CheckALCError()
	{
		ALenum ErrCode;
		std::string Err = "ALC error: ";
		if ((ErrCode = alcGetError(mALCdevice)) != ALC_NO_ERROR)
		{
			Err += (char *)alcGetString(mALCdevice, ErrCode);
			//ERRMSG(Err.data());
			return AL_FALSE;
		}
		return AL_TRUE;
	}

	ALboolean SoundSystem::CheckALError()
	{
		ALenum ErrCode;
		std::string Err = "OpenAL error: ";
		if ((ErrCode = alGetError()) != AL_NO_ERROR)
		{
			Err += (char *)alGetString(ErrCode);
			//ERRMSG(Err.data());
			return AL_FALSE;
		}
		return AL_TRUE;
	}

	SoundListener* SoundSystem::getSoundListener()
	{
		return mSoundListener;
	}

	SoundSource* SoundSystem::createSoundSource(float xPosX, float xPosY, float xPosZ, const std::string &xFilename, bool xLooped)
	{
		SoundSource *xSoundNode = new SoundSource();
		mSoundSources.push_back(xSoundNode);

		ALuint xSourceID;
		// ��������� ���� �� �������
		std::ifstream a(xFilename.c_str());
		if (!a.is_open())
		{
			xSoundNode->setError("Cant open file", false);
			return xSoundNode;
		}
		a.close();

		ALfloat xVel[3];
		ALfloat xPos[3] = { xPosX, xPosY, xPosZ };

		// ������� �������� ��������������� ������ �����
		alGenSources(1, &xSourceID);
		if (!CheckALError())
		{
			xSoundNode->setError("ALError", false);
			return xSoundNode;
		}

		alSourcef (xSourceID, AL_PITCH, 1.0f);
		alSourcef (xSourceID, AL_GAIN, 1.0f);
		alSourcefv(xSourceID, AL_POSITION, xPos);
		alSourcefv(xSourceID, AL_VELOCITY, xVel);
		alSourcei (xSourceID, AL_LOOPING, xLooped);

		loadWavFile(xFilename, xSourceID);

		xSoundNode->init(xSourceID, xLooped);
		return xSoundNode;
	}

	void SoundSystem::destroySoundSource(SoundSource* xSoundNode)
	{
		mSoundSources.remove(xSoundNode);

		delete xSoundNode;
	}
}
