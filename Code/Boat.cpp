#include "Boat.h"
#include "InputManager.h"
#include "Bubble.h"
#include "SphereCollider.h"
#include "RaceManager.h"
#include "InvisibleWall.h"

using namespace GameLayer;

Boat::Boat(Vector3 a_position)
  : Entity("boat", a_position), m_fDamping(0.98f)
{
  m_pCollider = std::unique_ptr<Collider>(
    new SphereCollider(2.2f, &m_position)
  );
  m_previousPosition = m_position;
  m_type = EntityType::RACER;
}


Boat::~Boat(void)
{
}


void Boat::Update(float a_fDeltaTime)
{
  switch(RaceManager::GetInstance()->GetState())
  {
  case RaceState::RUNNING:
  {
    Entity::Update(a_fDeltaTime);

    if(Input->IsKeyDown(SDLK_a))
    {
      m_rotation.y += a_fDeltaTime * 150;
    }
    if(Input->IsKeyDown(SDLK_d))
    {
      m_rotation.y -= a_fDeltaTime * 150;
    }
    if(Input->IsKeyDown(SDLK_SPACE))
    {
      float xAcc = cos(m_rotation.y * PI/180);
      float zAcc = sin(m_rotation.y * PI/180);
      Accelerate(Vector3(xAcc, 0, -zAcc) * 30);
    }

    Integrate(a_fDeltaTime);
  }
  break;
  case RaceState::ENDED:
    Integrate(a_fDeltaTime);
    break;
  }

  if(!m_bInAirTimeArea)
  {
    if(m_position.y < 0)
    {
      m_position.y *= 0.6f;
    }
    Move(Vector3(0, (0 - m_position.y) * 0.02f, 0));
  }
  m_bInAirTimeArea = false;
}


void Boat::Integrate(float a_fDeltaTime)
{
  Vector3 previousPosition = m_position;

  m_acceleration += m_accumulatedForce * a_fDeltaTime;
  m_accumulatedForce = Vector3();

  Vector3 diff = m_position - m_previousPosition;
  m_position += diff + m_acceleration * a_fDeltaTime * a_fDeltaTime;

  m_previousPosition = previousPosition;

  m_position = m_position - diff * (1-m_fDamping);
  m_acceleration *= m_fDamping;
}


void Boat::Hit(Entity* a_pOther, Vector3& a_intersection)
{
  if(!a_pOther->GetCollider()->m_bIsTrigger)
  {
    switch(a_pOther->GetType())
    {
    case EntityType::BUBBLE:
      {
        Vector3 normalizedIntersection = a_intersection;
        normalizedIntersection.Normalize();
        m_position = m_position - normalizedIntersection * -0.01f;
      }
      break;
    case EntityType::EXPLOSION:
      {
        Vector3 normalizedIntersection = a_intersection;
        normalizedIntersection.Normalize();
        m_position = m_position - normalizedIntersection * -0.02f;
      }
      break;
    case EntityType::STATIC:
      {
        Vector3 normalizedIntersection = a_intersection;
        normalizedIntersection.Normalize();
        InvisibleWall* wallPtr = dynamic_cast<InvisibleWall*>(a_pOther);
        if(wallPtr != nullptr)
        {
          if(wallPtr->GetDensity() == 2)
          {
            m_position = m_position + a_intersection;
          }
          else
          {
            m_position = m_position - normalizedIntersection * -0.05f * wallPtr->GetDensity();
          }
        }
        else
        {
          m_position = m_position - normalizedIntersection * -0.05f;
        }
      }
      break;
    default:
      {
        Vector3 normalizedIntersection = a_intersection;
        normalizedIntersection.Normalize();
        m_position = m_position - normalizedIntersection * -0.03f;
      }
      break;
    }
  }
}


void Boat::Accelerate(Vector3 a_force)
{
  m_accumulatedForce += a_force;
}