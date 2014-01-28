#include "SoundManager.h"
#include <string>
#include "SDL.h"

SoundManager::SoundManager(void)
{
	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 );
}


SoundManager::~SoundManager(void)
{
	for(auto i = m_sounds.begin(); i != m_sounds.end(); i++)
  {
    Mix_FreeChunk(i->second);
  }
  Mix_FreeMusic(m_pMusic);
  Mix_CloseAudio();
}


void SoundManager::Play(std::string a_soundName)
{
  Mix_PlayChannel( -1, m_sounds[a_soundName], 0 );
}


void SoundManager::SetMusic(std::string a_soundName, bool a_bStartPlaying)
{
	m_pMusic = Mix_LoadMUS(std::string("assets/audio/"+a_soundName+".wav").c_str());
	if(a_bStartPlaying)
		Mix_PlayMusic( m_pMusic, -1 );
}


void SoundManager::Load(std::string a_soundName)
{
	m_sounds[a_soundName] = 
      Mix_LoadWAV(std::string("assets/audio/"+a_soundName+".wav").c_str());
}


void SoundManager::Pause()
{
	Mix_PauseMusic();
}

void SoundManager::Resume()
{
	Mix_ResumeMusic();
}