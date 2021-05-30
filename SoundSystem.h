#pragma once
//#include "../3rdParty/Simple-SDL2-Audio-master/src/audio.h"
//#include <map>
#include <list>
#include <mutex>

using SoundID = unsigned short;

struct SoundMessage
{
	std::string pathString;
	const float volume;
	SoundMessage(const std::string& pathString, const float volume = 100.f);
};

//BASE
class SoundSystem
{
public:
	virtual ~SoundSystem() = default;
	virtual void Update() = 0;
	virtual void Play(const std::string& pathString, const float volume) = 0;
};
//INHERITANCE 
class LogNullSoundSystem final : public SoundSystem
{
public:
	virtual void Update() override;
	virtual void Play(const std::string& pathString, const float volume) override;
};

class SDLSoundSystem final : public SoundSystem
{
public:
	SDLSoundSystem();
	~SDLSoundSystem();
	virtual void Update() override;
	virtual void Play(const std::string& pathString, const float volume) override;
private:
	std::list<SoundMessage> m_SoundQueue;
	std::mutex m_Mutex;
};

class AudioServiceLocator final
{
public:
	AudioServiceLocator() = delete;

	static void Update();

	static void Destroy();
	static SoundSystem& GetSoundSystem();
	static void RegisterSoundSystem(SoundSystem* ss);
private:
	static SoundSystem* m_Instance;
	static LogNullSoundSystem m_Default;
};

