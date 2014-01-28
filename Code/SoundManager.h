#pragma once
#include "Singleton.h"
#include <map>
#include "SDL_mixer.h"

class SoundManager : public Singleton<SoundManager>
{
public:
	~SoundManager(void);
	SoundManager(void);

  std::map<std::string, Mix_Chunk*> m_sounds;
	void Play(std::string soundName);
	void Load(std::string soundName);
	void Pause();
	void Resume();
	void SetMusic(std::string soundName, bool a_bStartPlaying);
private:
  Mix_Music* m_pMusic;
};

#define Sounds SoundManager::GetInstance()