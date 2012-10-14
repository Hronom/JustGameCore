#include "SoundListener.h"

namespace JGC
{
	SoundListener::SoundListener(float xPosX, float xPosY, float xPosZ)
	{
		// Позиция слушателя.
		ALfloat xListenerPos[] = {xPosX, xPosY, xPosZ};

		// Скорость слушателя.
		ALfloat xListenerVel[] = {0.0, 0.0, 0.0};

		// Ориентация слушателя. (Первые 3 элемента – направление «на», последние 3 – «вверх»)
		ALfloat xListenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

		// Устанавливаем параметры слушателя
		// Позиция
		alListenerfv(AL_POSITION, xListenerPos);
		// Скорость
		alListenerfv(AL_VELOCITY, xListenerVel);
		// Ориентация
		alListenerfv(AL_ORIENTATION, xListenerOri);
	}

	SoundListener::~SoundListener()
	{
		//...
	}

	void SoundListener::move(float xNewX, float xNewY, float xNewZ)
	{
		ALfloat xPos[3] = {xNewX, xNewY, xNewZ};
		alListenerfv(AL_POSITION, xPos);
	}
}
