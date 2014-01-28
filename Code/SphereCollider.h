#pragma once
#include "collider.h"
#include "UtilityClasses.h"

namespace GameLayer
{
  class SphereCollider :
    public Collider
  {
  public:
    SphereCollider(float a_Radius, Vector3* a_position);
    virtual ~SphereCollider(void);

    virtual Vector3 Intersects(Collider* a_pOther);
    inline float GetRadius() const {return m_fRadius;}
    inline void SetRadius(float a_fRadius) {m_fRadius = a_fRadius;}
    inline Vector3& GetPosition() const {return *m_position;}
  protected:
    float m_fRadius;
    Vector3* m_position;
  };
};
