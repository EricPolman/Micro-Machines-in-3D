#pragma once
#include "entity.h"

namespace GameLayer
{
  class WaterPlane :
    public Entity
  {
  public:
    WaterPlane(Vector3 a_position);
    ~WaterPlane(void);

    virtual void Update(float a_fDeltaTime);
    virtual void Draw();

  private:
    float m_fWaveTime;
  };
};
