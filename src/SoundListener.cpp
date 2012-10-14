#include "SoundListener.h"

namespace JGC
{
	SoundListener::SoundListener(float xPosX, float xPosY, float xPosZ)
	{
		// ������� ���������.
		ALfloat xListenerPos[] = {xPosX, xPosY, xPosZ};

		// �������� ���������.
		ALfloat xListenerVel[] = {0.0, 0.0, 0.0};

		// ���������� ���������. (������ 3 �������� � ����������� ���, ��������� 3 � �������)
		ALfloat xListenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

		// ������������� ��������� ���������
		// �������
		alListenerfv(AL_POSITION, xListenerPos);
		// ��������
		alListenerfv(AL_VELOCITY, xListenerVel);
		// ����������
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
