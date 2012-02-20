#include "SoundSystem.h"

SoundSystem::SoundSystem(ISystemsListener *xMainListener)
{
	mMainListener = xMainListener;
}

SoundSystem::~SoundSystem()
{
	// ������� ��� �������
	/* for (TBuf::iterator i = Buffers.begin(); i != Buffers.end(); i++)
	alDeleteBuffers(1, &i->second.ID);*/
	// ��������� ������� ��������
	alcMakeContextCurrent(0);
	// ���������� ��������
	alcDestroyContext(mALCcontext);
	// ��������� �������� ����������
	alcCloseDevice(mALCdevice);
}

bool SoundSystem::init()
{
	// ������� ���������.
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

	// �������� ���������.
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	// ���������� ���������. (������ 3 �������� � ����������� ���, ��������� 3 � �������)
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	// ��������� �������� �� ��������� ����������
	mALCdevice = alcOpenDevice("");
	// �������� �� ������
	if (!mALCdevice)
	{
		//ERRMSG("Default sound device not present");
		return false;
	}
	// ������� �������� ����������
	mALCcontext = alcCreateContext(mALCdevice, 0);
	if (!CheckALCError()) return false;

	// ������ �������� �������
	alcMakeContextCurrent(mALCcontext);

	// ������������� ��������� ���������
	// �������
	alListenerfv(AL_POSITION,    ListenerPos);
	// ��������
	alListenerfv(AL_VELOCITY,    ListenerVel);
	// ����������
	alListenerfv(AL_ORIENTATION, ListenerOri);
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
