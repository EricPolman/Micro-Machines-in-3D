#pragma once
#include "entity.h"

namespace GameLayer
{
  class Bubble :
    public Entity
  {
  public:
    Bubble(Vector3 a_position, float a_scale);
    ~Bubble(void);

    virtual void Update(float a_fDeltaTime);
    virtual void Draw();
    virtual void Hit(Entity* a_pOther, Vector3& a_intersection);

  private:
    float m_fShaderTime;
    Vector3 m_previousPosition;
    Vector3 m_acceleration;
    float m_fDamping;
  };
};
