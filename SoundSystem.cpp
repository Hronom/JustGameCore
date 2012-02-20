#include "SoundSystem.h"

SoundSystem::SoundSystem(ISystemsListener *xMainListener)
{
	mMainListener = xMainListener;
}

SoundSystem::~SoundSystem()
{
	// Очищаем все буффера
	/* for (TBuf::iterator i = Buffers.begin(); i != Buffers.end(); i++)
	alDeleteBuffers(1, &i->second.ID);*/
	// Выключаем текущий контекст
	alcMakeContextCurrent(0);
	// Уничтожаем контекст
	alcDestroyContext(mALCcontext);
	// Закрываем звуковое устройство
	alcCloseDevice(mALCdevice);
}

bool SoundSystem::init()
{
	// Позиция слушателя.
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

	// Скорость слушателя.
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	// Ориентация слушателя. (Первые 3 элемента – направление «на», последние 3 – «вверх»)
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	// Открываем заданное по умолчанию устройство
	mALCdevice = alcOpenDevice("");
	// Проверка на ошибки
	if (!mALCdevice)
	{
		//ERRMSG("Default sound device not present");
		return false;
	}
	// Создаем контекст рендеринга
	mALCcontext = alcCreateContext(mALCdevice, 0);
	if (!CheckALCError()) return false;

	// Делаем контекст текущим
	alcMakeContextCurrent(mALCcontext);

	// Устанавливаем параметры слушателя
	// Позиция
	alListenerfv(AL_POSITION,    ListenerPos);
	// Скорость
	alListenerfv(AL_VELOCITY,    ListenerVel);
	// Ориентация
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
