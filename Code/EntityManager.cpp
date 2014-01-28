#include "EntityManager.h"
#include "Camera.h"

using namespace GameLayer;

EntityManager::~EntityManager(void)
{
  m_entities.clear();
  while(m_queue.size() > 0)
  {
    delete m_queue.front();
    m_queue.pop();
  }
  m_queue.empty();
  delete m_staticsNode;
}


EntityManager::EntityManager(void)
  :m_fFrameTimer(0)
{
  m_staticsNode = new QuadTreeNode(-150, -200, 300, 400, 4, 5);
}


void EntityManager::Update(float a_fDeltaTime)
{
  while(m_queue.size() > 0)
  {
    if(m_queue.front()->GetType() == EntityType::STATIC)
    {
      m_staticsNode->Add(std::shared_ptr<Entity>(m_queue.front()), 0);
    }
    else
      m_entities.push_back(std::shared_ptr<Entity>(m_queue.front()));

    m_queue.pop();
  }

  Vector3 camPos = Camera::GetInstance()->m_position;
  Vector3 camDir = Camera::GetInstance()->m_direction;
  camPos += camDir * -5;

  QuadTreeNode dynamics(-150, -200, 300, 400, 4, 3);
  std::vector<std::shared_ptr<Entity>> entitiesInRange;

  for(auto i = m_entities.begin(); i != m_entities.end(); )
  {
    Vector3 iPos = i->get()->m_position;
    if(i->get()->IsOccludable() && (
      camDir.Dot(iPos - camPos) < 0 || camPos.DistanceSquared(iPos) > 2000))
    { 
      i++;
      continue;
    }
    auto iEntity = (*i).get();
    iEntity->Update(a_fDeltaTime);

    m_staticsNode->CheckCollision(iEntity);
    
    if(iEntity->GetState() == EntityState::REMOVED)
    {
      i = m_entities.erase(i);
    }
    else
    {
      entitiesInRange.push_back(*i);
      dynamics.Add(*i,0);
      i++;
    }
  }
  for(auto i = entitiesInRange.begin(); i != entitiesInRange.end(); i++)
  {
    dynamics.CheckCollision(i->get());
  }

  m_staticsNode->Update(a_fDeltaTime);
}


void EntityManager::Draw()
{
  m_staticsNode->Draw();
  for(auto i = m_entities.begin(); i != m_entities.end(); i++)
  {
    if(i->get()->IsOccludable())
    {
      Vector3 camPos = Camera::GetInstance()->m_position;
      Vector3 camDir = Camera::GetInstance()->m_direction;
      camPos = camPos - camDir * 20;
      Vector3 iPos = i->get()->m_position;

      if(camDir.Dot(iPos - camPos) > 0 && camPos.DistanceSquared(iPos) < 10000)
      { 
        (*i)->Draw();
      }
    }
    else
    {
      (*i)->Draw();
    }
  }
}


void EntityManager::Add(Entity* a_object)
{
  m_queue.push(a_object);
}