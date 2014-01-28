#include "Physics.h"
#include <algorithm>

using namespace GameLayer;

void Physics::SphereSphereIntersect(SphereCollider& a, SphereCollider& b, Vector3& result)
{
  if(a.GetPosition().DistanceSquared(b.GetPosition())
    < a.GetRadius() * a.GetRadius() + b.GetRadius() * b.GetRadius())
  {
    Vector3 res = a.GetPosition() - b.GetPosition();
    //res.Normalize();
    result = res;
  }
}


void Physics::SphereAABBIntersect(SphereCollider& a, AABBCollider& b, Vector3& result)
{
  /* Using closest point on AABB */
  float newX = a.GetPosition().x;
  float newY = a.GetPosition().y;
  float newZ = a.GetPosition().z;

  newX = std::max(newX, b.m_begin.x);
  newX = std::min(newX, b.m_end.x);
    
  newY = std::max(newY, b.m_begin.y);
  newY = std::min(newY, b.m_end.y);
    
  newZ = std::max(newZ, b.m_begin.z);
  newZ = std::min(newZ, b.m_end.z);

  if(Vector3(newX, newY, newZ).DistanceSquared(a.GetPosition()) 
    > a.GetRadius() * a.GetRadius())
  {
    return;
  }
  // There is an intersection.
    
  // This bit is about finding the shortest intersection
  Vector3 resolution;
  if(a.GetPosition().x < (b.m_begin.x + b.m_end.x) * 0.5)
    resolution.x = b.m_begin.x - a.GetPosition().x - a.GetRadius();
  else
    resolution.x = b.m_end.x - a.GetPosition().x + a.GetRadius();

  //We don't want the objects to go down the y-axis in this game
  /*resolution.y = b.m_end.y - a.GetPosition().y;
  if(abs(resolution.y) > abs(b.m_end.y - a.GetPosition().y))
    resolution.y = b.m_end.y - a.GetPosition().y;*/
    
  if(a.GetPosition().z < (b.m_begin.z + b.m_end.z) * 0.5)
    resolution.z = b.m_begin.z - a.GetPosition().z - a.GetRadius();
  else
    resolution.z = b.m_end.z - a.GetPosition().z + a.GetRadius();

  if(abs(resolution.x) < abs(resolution.z))
    resolution.z = 0;
  else
    resolution.x = 0;

  result = resolution;
}


void Physics::SphereOBBIntersect(SphereCollider& a, OBBCollider& b, Vector3& result)
{
  Vector3 pointInOBB;
  float distanceOnX = 0;
  float distanceOnZ = 0;
  PointOnOBB(b, a.GetPosition(), pointInOBB, distanceOnX, distanceOnZ);
  if(a.GetPosition().DistanceSquared(pointInOBB) > 
    a.GetRadius() * a.GetRadius())
  {
    return; // No intersection
  }

  if(abs(distanceOnX) < abs(distanceOnZ))
  {
    result = b.m_forward * distanceOnZ;// * distanceOnX;
  }
  else
  {
    result = b.m_right * distanceOnX;// * distanceOnZ;
  }
  //result.Normalize();
}


void Physics::PointOnOBB(OBBCollider& obb, Vector3& point, Vector3& result, float& distanceOnX, float& distanceOnZ)
{
  Vector3 difference = point - obb.GetPosition();
  result = obb.GetPosition();
  
  float xDist = difference.Dot(obb.m_right);
  distanceOnX = xDist;
  if(xDist > obb.m_width * 0.5f)
    xDist = obb.m_width * 0.5f;
  if(xDist < obb.m_width * -0.5f)
    xDist = -obb.m_width * 0.5f;

  result += obb.m_right * xDist;
  
  float yDist = difference.Dot(obb.m_up);
  if(yDist > obb.m_height * 0.5f)
    yDist = obb.m_height * 0.5f;
  if(yDist < obb.m_height * -0.5f)
    yDist = -obb.m_height * 0.5f;

  result += obb.m_up * yDist;
  
  float zDist = difference.Dot(obb.m_forward);
  distanceOnZ = zDist;
  if(zDist > obb.m_depth * 0.5f)
    zDist = obb.m_depth * 0.5f;
  if(zDist < obb.m_depth * -0.5f)
    zDist = -obb.m_depth * 0.5f;

  result += obb.m_forward * zDist;
}