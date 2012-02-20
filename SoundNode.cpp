#include "SoundNode.h"

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
	/*std::string Ext =  (xFilename).data();
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
	SndInfo    buffer;
	// ������ ������ � ������
	ALenum    format;
	// ��������� �� ������ ������ �����
	ALvoid    *data;
	// ������ ����� �������
	ALsizei    size;
	// ������� ����� � ������
	ALsizei    freq;
	// ������������� ������������ ���������������
	ALboolean  loop;
	// ������������� ������
	ALuint    BufID = 0;

	// ��������� SndInfo �������
	buffer.Filename = xFilename;
	// ����, � ��� �� ��� ������������� ������ � ������ ������?
	for (std::map<ALuint, SndInfo>::iterator i = mBuffers.begin(); i != mBuffers.end(); i++)
	{
		if (i->second.Filename == xFilename) BufID = i->first;
	}

	// ���� ���� ��������� �������
	if (!BufID)
	{
		// ������ �����
		alGenBuffers(1, &buffer.ID);
		if (!CheckALError()) return false;
		// ��������� ������ �� wav �����
		alutLoadWAVFile((ALbyte *)xFilename.data(), &format, &data,
			&size, &freq, &loop);
		if (!CheckALError()) return false;

		buffer.Format      = format;
		buffer.Rate      = freq;
		// ��������� ����� �������
		alBufferData(buffer.ID, format, data, size, freq);
		if (!CheckALError()) return false;
		// ��������� ���� �� �������������
		alutUnloadWAV(format, data, size, freq);
		if (!CheckALError()) return false;

		// ��������� ���� ����� � ������
		mBuffers[buffer.ID] = buffer;
	}
	else 
		buffer = mBuffers[BufID];

	// ����������� ����� � ����������
	alSourcei (mSourceID, AL_BUFFER, buffer.ID);

	return true;
}

ALboolean SoundNode::CheckALCError()
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

ALboolean SoundNode::CheckALError()
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
