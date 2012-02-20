#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "ISystemsListener.h"

#include <al.h>
#include <alc.h>
#include <alut.h>

class SoundSystem
{
private:
	ISystemsListener *mMainListener;

	ALCdevice *mALCdevice;
	ALCcontext *mALCcontext;

public:
	SoundSystem(ISystemsListener *xMainListener);
	~SoundSystem();

	bool init();

private:
	ALboolean CheckALCError();
	ALboolean CheckALError();
};

#endif

