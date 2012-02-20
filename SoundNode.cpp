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
	// Структура содержащая аудиопараметры
	SndInfo    buffer;
	// Формат данных в буфере
	ALenum    format;
	// Указатель на массив данных звука
	ALvoid    *data;
	// Размер этого массива
	ALsizei    size;
	// Частота звука в герцах
	ALsizei    freq;
	// Идентификатор циклического воспроизведения
	ALboolean  loop;
	// Идентификатор буфера
	ALuint    BufID = 0;

	// Заполняем SndInfo данными
	buffer.Filename = xFilename;
	// Ищем, а нет ли уже существующего буфера с данным звуком?
	for (std::map<ALuint, SndInfo>::iterator i = mBuffers.begin(); i != mBuffers.end(); i++)
	{
		if (i->second.Filename == xFilename) BufID = i->first;
	}

	// Если звук загружаем впервые
	if (!BufID)
	{
		// Создаём буфер
		alGenBuffers(1, &buffer.ID);
		if (!CheckALError()) return false;
		// Загружаем данные из wav файла
		alutLoadWAVFile((ALbyte *)xFilename.data(), &format, &data,
			&size, &freq, &loop);
		if (!CheckALError()) return false;

		buffer.Format      = format;
		buffer.Rate      = freq;
		// Заполняем буфер данными
		alBufferData(buffer.ID, format, data, size, freq);
		if (!CheckALError()) return false;
		// Выгружаем файл за ненадобностью
		alutUnloadWAV(format, data, size, freq);
		if (!CheckALError()) return false;

		// Добавляем этот буфер в массив
		mBuffers[buffer.ID] = buffer;
	}
	else 
		buffer = mBuffers[BufID];

	// Ассоциируем буфер с источником
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
