#pragma once
#include "entity.h"

namespace GameLayer
{
  class Mine;

  class MineSpawner :
    public Entity
  {
  public:
    MineSpawner(Vector3 a_position, int a_seed);
    ~MineSpawner(void);
    
    void Reset();
    void Spawn();
    virtual void Update(float a_fDeltaTime);

  private:
    Mine* m_pCurrentMine;
    int m_seed;
  };
};
