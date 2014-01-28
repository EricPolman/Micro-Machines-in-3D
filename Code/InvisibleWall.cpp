#include "InvisibleWall.h"
#include "BoxCollider.h"
#include <iostream>

using namespace GameLayer;

InvisibleWall::InvisibleWall(
  Vector3 a_position, float a_width, float a_height, 
  float a_depth, float a_rotation, float a_density)
  : Entity(a_position), m_width(a_width), m_height(a_height), 
  m_depth(a_depth), m_rotation(a_rotation), m_density(a_density)
{
  if(m_rotation == 0)
    m_pCollider = std::unique_ptr<Collider>(
      new AABBCollider(&m_position, a_width, a_height, a_depth)
    );
  else
    m_pCollider = std::unique_ptr<Collider>(
      new OBBCollider(&m_position, a_width, a_height, a_depth, m_rotation)
    );

  m_type = EntityType::STATIC;
}


InvisibleWall::~InvisibleWall(void)
{
}


void InvisibleWall::Draw()
{
  /*glLineWidth(10);
  glBegin(GL_LINES);
    glVertex3f(
      m_position.x - m_width / 2, 
      m_position.y - m_height / 2, 
      m_position.z - m_depth / 2
    );
    glVertex3f(
      m_position.x + m_width / 2, 
      m_position.y + m_height / 2, 
      m_position.z + m_depth / 2
    );
  glEnd();*/
}

void InvisibleWall::Hit(Entity* a_pOther, Vector3& a_intersection)
{
  //Intentionally left blank, we don't want it to do anything on collision.
}