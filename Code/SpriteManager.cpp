#include "SpriteManager.h"
#include <time.h>

using namespace GameLayer;

SpriteManager::~SpriteManager(void)
{
	m_sprites.clear();
}

int SpriteManager::Add(char* a_sPath)
{
  std::shared_ptr<Sprite> ptr(Sprite::Create(a_sPath));
  int id = rand()%RAND_MAX;
  while(m_sprites.find(id) != m_sprites.end())
  {
    id = rand()%RAND_MAX;
  }
  m_sprites[id] = (std::move(ptr));
  return id;
}

void SpriteManager::Remove(int a_iId)
{
  m_sprites.erase(a_iId);
}

void SpriteManager::Update(float a_fDeltaTime)
{
  for(auto i = m_sprites.begin(); i != m_sprites.end();i++)
	{
    if(i->second->GetSpriteState() == SpriteState::DESTROYED)
    {
      //Push to removal thingy.
      //m_sprites.erase(i->first);
    }
	}
}

void SpriteManager::Draw()
{
  for(auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
	{
    if(i->second->GetSpriteState() == SpriteState::VISIBLE)
      i->second->Draw();
  }
}

std::weak_ptr<Sprite> SpriteManager::Get(int a_iId)
{
  std::weak_ptr<Sprite> ptr(m_sprites[a_iId]);
  return ptr;
}