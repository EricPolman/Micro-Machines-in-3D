#pragma once
#include "Sprite.h"
#include "Manager.h"
#include <map>
#include "Singleton.h"

namespace GameLayer{
  class SpriteManager : Manager, public Singleton<SpriteManager>
  {
  public:
	  ~SpriteManager(void);
	  SpriteManager(void){}
    int Add(char* a_sPath);
    void Remove(int a_iId);
    std::weak_ptr<Sprite> Get(int a_iId);
    virtual void Update(float a_fDeltaTime);
    void Draw();
  private:
    std::map<int, std::shared_ptr<Sprite>> m_sprites;
  };
};

#define Sprites SpriteManager::GetInstance()