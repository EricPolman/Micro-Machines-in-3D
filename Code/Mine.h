#pragma once
#include "entity.h"

namespace GameLayer
{
  class MineSpawner;

  class Mine :
    public Entity
  {
  public:
    Mine(Vector3 a_position, float a_scale, MineSpawner* a_pSpawner);
    ~Mine(void);

    void Update(float a_fDeltaTime);
    void Hit(Entity* a_pEntity, Vector3& a_intersection);
    void FlagSpawnerDestroyed(){m_pSpawner = nullptr;}

  private:
    MineSpawner* m_pSpawner;
  };
};
