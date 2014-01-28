#pragma once
#include "Entity.h"

namespace GameLayer
{
  class Waypoint
    : public Entity
  {
  public:
    Waypoint(Vector3 a_position, Vector3 a_size);
    ~Waypoint(void);

    virtual void Draw();
    virtual void Hit(Entity* a_pOther, Vector3& a_intersection);
  };
};
