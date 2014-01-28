#pragma once
#include "UtilityClasses.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

namespace GameLayer
{
  class Physics
  {
  public:
    static void SphereSphereIntersect(SphereCollider& a, SphereCollider& b, Vector3& result);
    static void SphereAABBIntersect(SphereCollider& a, AABBCollider& b, Vector3& result);
    static void SphereOBBIntersect(SphereCollider& a, OBBCollider& b, Vector3& result);

    static void PointOnOBB(OBBCollider& obb, Vector3& point, Vector3& result, float& distanceOnX, float& distanceOnZ);
  };
};
