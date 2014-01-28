#include "SphereCollider.h"
#include <algorithm>
#include "Physics.h"

using namespace GameLayer;

SphereCollider::SphereCollider(float a_Radius, Vector3* a_position)
{
  m_fRadius = a_Radius;
  m_position = a_position;
  m_collisionType = CollisionType::SPHERE;
}


SphereCollider::~SphereCollider(void)
{
}


Vector3 SphereCollider::Intersects(Collider* a_pOther)
{
  Vector3 result;
  switch(a_pOther->m_collisionType)
  {
  case CollisionType::SPHERE:
  {
    SphereCollider* col = (SphereCollider*)a_pOther;
    Physics::SphereSphereIntersect(*this, *col, result);
    break;
  }
  case CollisionType::AABB:
  {
    AABBCollider* aabb = (AABBCollider*)a_pOther;
    Physics::SphereAABBIntersect(*this, *aabb, result);
    break;
  }
  case CollisionType::OBB:
  {
    OBBCollider* obb = (OBBCollider*)a_pOther;
    Physics::SphereOBBIntersect(*this, *obb, result);
    break;
  }
  }
  return result;
}