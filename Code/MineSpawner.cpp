#include "MineSpawner.h"
#include "Mine.h"
#include "RaceManager.h"

using namespace GameLayer;

MineSpawner::MineSpawner(Vector3 a_position, int a_seed)
  : Entity(a_position), m_pCurrentMine(nullptr), m_seed(a_seed)
{
  RaceManager::GetInstance()->AddMineSpawner(this);
}


MineSpawner::~MineSpawner(void)
{
  if(m_pCurrentMine != nullptr)
    m_pCurrentMine->FlagSpawnerDestroyed();
}


void MineSpawner::Spawn()
{
  if(m_pCurrentMine == nullptr)
  {
    m_pCurrentMine = new Mine(m_position, 2, this);
  }
}


void MineSpawner::Reset()
{
  m_pCurrentMine = nullptr;
}


void MineSpawner::Update(float a_fDeltaTime)
{
}