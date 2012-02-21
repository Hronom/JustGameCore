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
	// Проверяем файл на наличие
	std::ifstream a(xFilename.c_str());
	if (!a.is_open()) return false;
	a.close();

	mLooped = xLooped;
	//mStreamed

	// Создаем источник соответствующий нашему звуку
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
	// Структура содержащая аудиопараметры
	SndInfo xBuffer;
	// Формат данных в буфере
	ALenum xFormat;
	// Указатель на массив данных звука
	ALvoid *xData;
	// Размер этого массива
	ALsizei xSize;
	// Частота звука в герцах
	ALsizei xFreq;
	// Идентификатор циклического воспроизведения
	ALboolean xLoop;
	// Идентификатор буфера
	ALuint xBufID = 0;

	// Заполняем SndInfo данными
	xBuffer.Filename = xFilename;
	// Ищем, а нет ли уже существующего буфера с данным звуком?
	for (std::map<ALuint, SndInfo>::iterator i = mBuffers.begin(); i != mBuffers.end(); i++)
	{
		if (i->second.Filename == xFilename) xBufID = i->first;
	}

	// Если звук загружаем впервые
	if (!xBufID)
	{
		// Создаём буфер
		alGenBuffers(1, &xBuffer.ID);
		if (!CheckALError()) return false;
		// Загружаем данные из wav файла
		alutLoadWAVFile((ALbyte *)xFilename.data(), &xFormat, &xData,	&xSize, &xFreq, &xLoop);
		if (!CheckALError()) return false;

		xBuffer.Format = xFormat;
		xBuffer.Rate = xFreq;
		// Заполняем буфер данными
		alBufferData(xBuffer.ID, xFormat, xData, xSize, xFreq);
		if (!CheckALError()) return false;
		// Выгружаем файл за ненадобностью
		alutUnloadWAV(xFormat, xData, xSize, xFreq);
		if (!CheckALError()) return false;

		// Добавляем этот буфер в массив
		mBuffers[xBuffer.ID] = xBuffer;
	}
	else 
		xBuffer = mBuffers[xBufID];

	// Ассоциируем буфер с источником
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
