#pragma once
#include "entity.h"

namespace GameLayer
{
  class Explosion :
    public Entity
  {
  public:
    Explosion(Vector3 a_position);
    ~Explosion(void);

    virtual void Update(float a_fDeltaTime);
    inline virtual void Draw();

  private:
    Vector2 m_currentFrame;
    Vector2 m_frames;
    float m_fExpansionTimer, m_fFrameTimer;
  };
};
