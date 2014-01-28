#pragma once
#include "UtilityClasses.h"
enum CollisionType {SPHERE, AABB, OBB};

class Collider
{
public:
  Collider() : m_collisionType(CollisionType::SPHERE), m_bIsTrigger(false) {}
  virtual Vector3 Intersects(Collider* a_pOther) = 0;
  CollisionType m_collisionType;
  bool m_bIsTrigger;
};

