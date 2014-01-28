#include "BoxCollider.h"

using namespace GameLayer;

AABBCollider::AABBCollider(Vector3* a_position, float a_width, float a_height, float a_depth)
  : m_position(a_position)
{
  m_begin = *m_position - Vector3(a_width / 2, a_height / 2, a_depth / 2);
  m_end = *m_position + Vector3(a_width / 2, a_height / 2, a_depth / 2);
  m_collisionType = CollisionType::AABB;
}


AABBCollider::~AABBCollider(void)
{
}


OBBCollider::OBBCollider
  (Vector3* a_position, float a_width, float a_height, 
  float a_depth, float a_rotation)
  : m_position(a_position), m_width(a_width), m_height(a_height),
    m_depth(a_depth)
{
  m_collisionType = CollisionType::OBB;
  SetRotation(a_rotation);
}

void OBBCollider::SetRotation(float a_amount)
{
  m_right = Vector3(cos(a_amount), 0, sin(a_amount));
  m_up = Vector3(0,1,0);
  m_forward = m_right.Cross(Vector3(0,1,0));
}