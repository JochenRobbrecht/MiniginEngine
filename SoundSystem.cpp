#include "MiniginPCH.h"
#include "SoundSystem.h"
#include "../3rdParty/Simple-SDL2-Audio-master/src/audio.c"
#include <mutex>


LogNullSoundSystem AudioServiceLocator::m_Default;
SoundSystem* AudioServiceLocator::m_Instance = &AudioServiceLocator::m_Default;

void AudioServiceLocator::Update()
{
	m_Instance->Update();
}

void AudioServiceLocator::Destroy()
{
	if (m_Instance)delete m_Instance;
	m_Instance = nullptr;
}

SoundSystem& AudioServiceLocator::GetSoundSystem()
{
	return *m_Instance;
}

void AudioServiceLocator::RegisterSoundSystem(SoundSystem* ss)
{
	if (ss)m_Instance = ss;
	else { m_Instance = &m_Default; }
}

void LogNullSoundSystem::Update()
{
}

void LogNullSoundSystem::Play(const std::string& pathString, const float volume)
{
	std::cout << "Request to play Sound with pathString\"" << pathString << "\" at volume \""  << volume << "\"(0-100)" << std::endl;
}



SDLSoundSystem::SDLSoundSystem()
	: m_SoundQueue{}
	, m_Mutex{ }
{
	initAudio();
}

SDLSoundSystem::~SDLSoundSystem()
{
	endAudio();
}

void SDLSoundSystem::Update()
{
	m_Mutex.lock();
	while (!m_SoundQueue.empty() && gSoundCount < AUDIO_MAX_SOUNDS)
	{
		SoundMessage sound = m_SoundQueue.front();

		m_SoundQueue.pop_front();

		playSound(sound.pathString.c_str(), uint8_t(SDL_MIX_MAXVOLUME * (sound.volume / 100.f)));
	}
	m_Mutex.unlock();
}

void SDLSoundSystem::Play(const std::string& pathString, const float volume)
{
	m_Mutex.lock();
	if (m_SoundQueue.size() >= AUDIO_MAX_SOUNDS)
	{
		m_Mutex.unlock();
		return;
	}

	m_SoundQueue.push_back(SoundMessage{ pathString,volume });	
	m_Mutex.unlock();
}

SoundMessage::SoundMessage(const std::string& pathString, const float volume)
	: pathString{ pathString }
	, volume{ volume }
{
}
