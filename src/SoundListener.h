#ifndef SOUNDLISTENER_H
#define SOUNDLISTENER_H

#include <al.h>
#include <alc.h>
#include <alut.h>

namespace JGC
{
	class SoundListener
	{
	public:
		SoundListener(float xPosX, float xPosY, float xPosZ);
		~SoundListener();

		void move(float xNewX, float xNewY, float xNewZ);
	};
}

#endif

