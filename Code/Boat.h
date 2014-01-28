#pragma once
#include "entity.h"

namespace GameLayer
{
  class Boat :
    public Entity
  {
  public:
    Boat(Vector3 a_position);
    ~Boat(void);

    virtual void Update(float a_fDeltaTime);
    virtual void Hit(Entity* a_pOther, Vector3& a_intersection);
    void Accelerate(Vector3 a_force);

    void InAirTimeArea(bool a_value){m_bInAirTimeArea = a_value;}
  protected:
    void Integrate(float a_fDeltaTime);
    Vector3 m_acceleration;
    float m_fDamping;
    Vector3 m_accumulatedForce;
    bool m_bInAirTimeArea;
  };
};
