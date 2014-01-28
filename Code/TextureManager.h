#pragma once
#include "manager.h"
#include <map>
#include <string>
#include "Singleton.h"

namespace GameLayer{
  class TextureManager : public Singleton<TextureManager>
  {
  public:
	  ~TextureManager(void);
	  TextureManager(void){}
    void Load(std::string name, std::string a_path);
    unsigned int GetTexture(std::string name);
  private:

    std::map<std::string, unsigned int> m_textures;
  };
};

#define Textures TextureManager::GetInstance()