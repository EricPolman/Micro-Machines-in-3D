#include "QuadTreeNode.h"
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
#include "Camera.h"

using namespace GameLayer;

QuadTreeNode::QuadTreeNode(float x, float y, float width, float height, int maxElements, int maxLevels)
  : m_bounds(x, y, width, height), m_maxElements(maxElements), m_maxLevels(maxLevels)
{
  for(int i = 0; i < 4; i++)
    m_children[i] = 0;

  m_bPlayerInRect = false;
}


QuadTreeNode::~QuadTreeNode(void)
{
  for(auto i = m_entities.begin(); i != m_entities.end();)
  {
    i = m_entities.erase(i);
  }
  m_entities.clear();

  if(m_children[0])
    for(int i = 0; i < 4; i++)
      delete m_children[i];
}


void QuadTreeNode::Split()
{
  m_children[0] = new QuadTreeNode(
    m_bounds.x,m_bounds.y,m_bounds.width/2,
    m_bounds.height/2,
    m_maxElements, m_maxLevels
  );

  m_children[1] = new QuadTreeNode(
    m_bounds.x + m_bounds.width/2,m_bounds.y,
    m_bounds.width/2,m_bounds.height/2,
    m_maxElements, m_maxLevels
  );

  m_children[2] = new QuadTreeNode(
    m_bounds.x,m_bounds.y + m_bounds.height/2,m_bounds.width/2,
    m_bounds.height/2,
    m_maxElements, m_maxLevels
  );

  m_children[3] = new QuadTreeNode(
    m_bounds.x + m_bounds.width/2,m_bounds.y + m_bounds.height/2,
    m_bounds.width/2,m_bounds.height/2,
    m_maxElements, m_maxLevels
  );
}


void QuadTreeNode::Add(std::shared_ptr<Entity> a_entity, int level)
{
  if(m_children[0])
  {
    for(int i = 0; i < 4; i++)
    {
      Rectangle rect = a_entity->GetBoundingRectangle();
      if(m_children[i]->m_bounds.Intersects(a_entity->GetBoundingRectangle()))
      {
        m_children[i]->Add(a_entity, level + 1);
      }
    }
  }
  else
  {
    if(m_entities.size() < m_maxElements)
    {
      m_entities.push_back(a_entity);
    }
    else
    {
      if(level < m_maxLevels)
      {
        Split();
        for(int i = 0; i < 4; i++)
        {
          if(m_children[i]->m_bounds.Intersects(a_entity->GetBoundingRectangle()))
          {
            m_children[i]->Add(a_entity, level + 1);
          }

          for(auto iter = m_entities.begin(); iter != m_entities.end(); iter++)
          {
            if(m_children[i]->m_bounds.Intersects((*iter)->GetBoundingRectangle()))
            {
              m_children[i]->Add(*iter, level + 1);
            }
          }
        }
        m_entities.clear();
      }
      else
      {
        m_entities.push_back(a_entity);
      }
    }
  }
}


void QuadTreeNode::Remove(Entity* a_entity)
{
  if(m_children[0])
  {
    for(int i = 0; i < 4; i++)
    {
      m_children[i]->Remove(a_entity);
    }
  }
  else
  {
    for(auto iter = m_entities.begin(); iter != m_entities.end(); )
    {
      if(iter->get() == a_entity)
      {
        iter = m_entities.erase(iter);
      }
      else
      {
        iter++;
      }
    }
  }
}


void QuadTreeNode::Update(float a_fDeltaTime)
{
  if(m_children[0])
  {
    for(int i = 0; i < 4; i++)
    {
      m_children[i]->Update(a_fDeltaTime);
    }
  }
  else
  {
    for(auto i = m_entities.begin(); i != m_entities.end();)
    {
      (*i)->Update(a_fDeltaTime);
      if((*i)->GetState() == EntityState::REMOVED)
      {
        i = m_entities.erase(i);
      }
      else 
      {
        i++;
      }
    }
  }
}


void QuadTreeNode::Draw()
{
  if(m_children[0])
  {
    for(int i = 0; i < 4; i++)
    {
      m_children[i]->Draw();
    }
  }
  else
  {
    for(auto i = m_entities.begin(); i != m_entities.end(); i++)
    {
      if((*i)->IsOccludable())
      {
        Vector3 camPos = Camera::GetInstance()->m_position;
        Vector3 camDir = Camera::GetInstance()->m_direction;
        Vector3 iPos = (*i)->m_position;

        if(camDir.Dot(iPos - camPos) < 0 || camPos.DistanceSquared(iPos) > 20000)
          continue;
      }
      (*i)->Draw();
    }
    /*if(m_bPlayerInRect)
      glLineWidth(3);
    glBegin(GL_LINE_STRIP);

    glVertex3f(m_bounds.x, 2, m_bounds.y);
    glVertex3f(m_bounds.x + m_bounds.width, 2, m_bounds.y);
    glVertex3f(m_bounds.x + m_bounds.width, 2, m_bounds.y + m_bounds.height);
    glVertex3f(m_bounds.x, 2, m_bounds.y + m_bounds.height);
    glVertex3f(m_bounds.x, 2, m_bounds.y);

    glEnd();
    if(m_bPlayerInRect)
      glLineWidth(1);*/
  }
  m_bPlayerInRect = false;
}


void QuadTreeNode::CheckCollision(Entity* a_entity)
{
  if(m_children[0])
  {
    for(int i = 0; i < 4; i++)
    {
      if(m_children[i]->m_bounds.Intersects(a_entity->GetBoundingRectangle()))
      {
        m_children[i]->CheckCollision(a_entity);        
      }
    }
  }
  else
  {
    m_bPlayerInRect = true;
    for(auto iter = m_entities.begin(); iter != m_entities.end(); iter++)
    {
      if(a_entity == iter->get())
        continue;

      if(a_entity->GetCollider() == nullptr || iter->get()->GetCollider() == nullptr)
        continue;

      Vector3 vec = a_entity->GetCollider()->Intersects((iter->get())->GetCollider());
      if(vec.x != 0 || vec.z != 0)
      {
        a_entity->Hit(iter->get(), vec);
        (*iter)->Hit(a_entity, -vec);
      }
    }
  }
}