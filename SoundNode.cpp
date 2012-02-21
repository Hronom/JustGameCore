#include "SoundNode.h"
using namespace JGC;
using namespace JGC::Sound;

SoundNode::SoundNode(ALCdevice *xALCdevice)
{
	mALCdevice = xALCdevice;
}

SoundNode::~SoundNode()
{
}

bool SoundNode::open(const std::string &xFilename, bool xLooped, bool xStreamed)
{
	// ��������� ���� �� �������
	std::ifstream a(xFilename.c_str());
	if (!a.is_open()) return false;
	a.close();

	mLooped = xLooped;
	//mStreamed

	// ������� �������� ��������������� ������ �����
	alGenSources(1, &mSourceID);
	if (!CheckALError()) return false;

	alSourcef (mSourceID, AL_PITCH,    0.3f);
	//alSourcef (mSourceID, AL_PITCH,    1.0f);
	alSourcef (mSourceID, AL_GAIN,    1.0f);
	alSourcefv(mSourceID, AL_POSITION,  mPos);
	alSourcefv(mSourceID, AL_VELOCITY,  mVel);
	alSourcei (mSourceID, AL_LOOPING,  mLooped);

	// Extract ext
	/*std::string Ext =  (xFilename).xData();
	if (Ext == "WAV") return loadWavFile(xFilename);
	if (Ext == "OGG") 
	{
	mStreamed = Streamed;
	return LoadOggFile(Filename, Streamed);
	}
	return false;*/

	return loadWavFile(xFilename);
}

bool SoundNode::isStreamed()
{
	return mStreamed;
}

void SoundNode::play()
{
	alSourcePlay(mSourceID);
}

void SoundNode::close()
{
	alSourceStop(mSourceID);
	if (alIsSource(mSourceID)) alDeleteSources(1, &mSourceID);
}

void SoundNode::update()
{

}

void SoundNode::move(float xNewX, float xNewY, float xNewZ)
{
	ALfloat Pos[3] = { xNewX, xNewY, xNewZ };
	alSourcefv(mSourceID, AL_POSITION, Pos);
}

void SoundNode::stop()
{
	alSourceStop(mSourceID);
}

bool SoundNode::loadWavFile(const std::string &xFilename)
{
	// ��������� ���������� ��������������
	SndInfo xBuffer;
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

	// ��������� SndInfo �������
	xBuffer.Filename = xFilename;
	// ����, � ��� �� ��� ������������� ������ � ������ ������?
	for (std::map<ALuint, SndInfo>::iterator i = mBuffers.begin(); i != mBuffers.end(); i++)
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
	alSourcei (mSourceID, AL_BUFFER, xBuffer.ID);

	return true;
}

ALboolean SoundNode::CheckALCError()
{
	ALenum xErrCode;
	std::string xErr = "ALC error: ";
	if ((xErrCode = alcGetError(mALCdevice)) != ALC_NO_ERROR)
	{
		xErr += (char *)alcGetString(mALCdevice, xErrCode);
		//ERRMSG(xErr.xData());
		return AL_FALSE;
	}
	return AL_TRUE;
}

ALboolean SoundNode::CheckALError()
{
	ALenum xErrCode;
	std::string xErr = "OpenAL error: ";
	if ((xErrCode = alGetError()) != AL_NO_ERROR)
	{
		xErr += (char *)alGetString(xErrCode);
		//ERRMSG(xErr.xData());
		return AL_FALSE;
	}
	return AL_TRUE;
}
