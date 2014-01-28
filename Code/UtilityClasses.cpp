#include "UtilityClasses.h"
#include <algorithm>

bool Rectangle::Intersects(const Rectangle& a_other) const
{
  if(y >= a_other.y + a_other.height
		|| y + height <= a_other.y
		|| x >= a_other.x + a_other.width
		|| x + width <= a_other.x)
	{ return false;}
	//Now we know there is a collision.
  return true;
}

bool Rectangle::Intersects(const Vector3& a_position, float a_radius) const
{
  /* Using closest point on AABB */
  float newX = a_position.x;
  float newY = a_position.z;
  
  newX = std::max(newX, x);
  newX = std::min(newX, x + width);
  newY = std::max(newY, y);
  newY = std::min(newY, y + height);

  if(Vector2(newX, newY).DistanceSquared(Vector2(a_position.x, a_position.y)) 
    > a_radius * a_radius)
  {
    return false;
  }
  return true;
}