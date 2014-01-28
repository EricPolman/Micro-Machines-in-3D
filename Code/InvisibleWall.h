#pragma once
#include "entity.h"
#include "UtilityClasses.h"

namespace GameLayer
{
  class InvisibleWall :
    public Entity
  {
  public:
    InvisibleWall(Vector3 a_position, float a_width, float a_height, float a_depth, float a_rotation = 0, float a_density = 1);
    ~InvisibleWall(void);

    virtual void Draw();
    virtual void Hit(Entity* a_pOther, Vector3& a_intersection);
    float GetDensity() const {return m_density;}
    float m_width;
    float m_height;
    float m_depth;
    float m_rotation;
    float m_density;
  };
};
