#include "Mine.h"
#include "SphereCollider.h"
#include "Explosion.h"
#include "MineSpawner.h"

using namespace GameLayer;

Mine::Mine(Vector3 a_position, float a_scale, MineSpawner* a_pSpawner)
  : Entity("mine", a_position, EntityType::STATIC), m_pSpawner(a_pSpawner)
{
  Scale(a_scale);
  m_pCollider = std::unique_ptr<Collider>(
    new SphereCollider(a_scale * 0.5f, &m_position)
  );
  AssignTexture("mine");
}


Mine::~Mine(void)
{
  if(m_pSpawner != nullptr)
    m_pSpawner->Reset();
}


void Mine::Update(float a_fDeltaTime)
{
  Rotate(Vector3(0,a_fDeltaTime * 36,0));
}


void Mine::Hit(Entity* a_pEntity, Vector3& a_intersection)
{
  if(a_pEntity->GetType() == EntityType::RACER
    || a_pEntity->GetType() == EntityType::EXPLOSION)
  {
    new Explosion(m_position);
    m_state = EntityState::REMOVED;
  }
}