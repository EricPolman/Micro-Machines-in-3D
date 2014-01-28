#include "Waypoint.h"
#include "BoxCollider.h"
#include <assert.h>
extern "C" {
  #include "glew.h"
}
#include "gl.h"
#include "RaceManager.h"

using namespace GameLayer;

Waypoint::Waypoint(Vector3 a_position, Vector3 a_size)
  : Entity(a_position)
{
  m_pCollider = std::unique_ptr<Collider>(
    new AABBCollider(&m_position, a_size.x, a_size.y, a_size.z)
  );
  m_pCollider->m_bIsTrigger = true;
  RaceManager::GetInstance()->AddWaypoint(this);
  m_type = EntityType::STATIC;
}


Waypoint::~Waypoint(void)
{
}


void Waypoint::Draw()
{
  /*glPointSize(20);
  glBegin(GL_POINTS);
    glVertex3f(m_position.x, m_position.y, m_position.z);
  glEnd();*/
}

void Waypoint::Hit(Entity* a_pOther, Vector3& a_intersection)
{
  RaceManager::GetInstance()->ProcessWaypointHit(this, a_pOther);
}