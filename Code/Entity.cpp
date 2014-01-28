#include "Entity.h"
#include "EntityManager.h"
#include "MeshManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include <algorithm>

extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"

using namespace GameLayer;

Entity::Entity(std::string mesh)
  : m_scale(1,1,1), m_pCollider(nullptr), m_type(EntityType::DEFAULT),
  m_state(EntityState::ACTIVE), m_bIsOccludable(true)
{
  Entities->Add(this);
  m_renderData.m_pMesh = Meshes->GetMesh(mesh);
}
Entity::Entity(Vector3 a_position)
  : m_scale(1,1,1), m_pCollider(nullptr), m_position(a_position), 
    m_type(EntityType::DEFAULT), m_state(EntityState::ACTIVE), 
    m_bIsOccludable(true)
{
  Entities->Add(this);
}
Entity::Entity(std::string mesh, Vector3 a_position, EntityType a_type)
  : m_scale(1,1,1), m_position(a_position), m_pCollider(nullptr)
  , m_type(a_type), m_state(EntityState::ACTIVE),
  m_bIsOccludable(true)
{
  Entities->Add(this);
  m_renderData.m_pMesh = Meshes->GetMesh(mesh);
  AssignShader("diffuse");
}


Entity::~Entity(void)
{
}
/*
Entity::Entity(const Entity& a_other)
{

}

Entity& Entity::operator=(const Entity& a_other)
{

}*/

void Entity::Update(float a_fDeltaTime)
{
}


void Entity::Draw()
{
  if(m_renderData.m_pMesh != nullptr)
  {
    glPushMatrix();
      glTranslatef(m_position.x, m_position.y, m_position.z);
      glScalef(m_scale.x, m_scale.y, m_scale.z);
      glRotatef(m_rotation.x, 1, 0, 0);
      glRotatef(m_rotation.y, 0, 1, 0);
      glRotatef(m_rotation.z, 0, 0, 1);
    
      glUseProgram(m_renderData.m_programId);
      if(m_renderData.m_textureId != 0)
      {
        glBindTexture(GL_TEXTURE_2D, m_renderData.m_textureId);
      }
      m_renderData.m_pMesh->Draw();
    
      if(m_renderData.m_textureId != 0)
      {
        glBindTexture(GL_TEXTURE_2D, 0);
      }
      glUseProgram(0);
    glPopMatrix();
  }
}

void Entity::AssignTexture(std::string a_name)
{
  m_renderData.m_textureId = Textures->GetTexture(a_name);
  AssignShader("diffuse_textured");
}

void Entity::AssignShader(std::string a_name)
{
  m_renderData.m_programId = Shaders->GetProgram(a_name);
}

void Entity::AssignMesh(std::string a_name)
{
  m_renderData.m_pMesh = Meshes->GetMesh(a_name);
}


void Entity::Hit(Entity* a_pOther, Vector3& a_intersection)
{
  
}


Rectangle Entity::GetBoundingRectangle()
{
  Rectangle rect;
  if(m_pCollider == nullptr)
    return rect;

  switch(m_pCollider->m_collisionType)
  {
  case CollisionType::SPHERE:
    {
      SphereCollider sCol = *(SphereCollider*)m_pCollider.get();
      rect.x = sCol.GetPosition().x - sCol.GetRadius();
      rect.y = sCol.GetPosition().z - sCol.GetRadius();
      rect.width = sCol.GetRadius() * 2;
      rect.height = sCol.GetRadius() * 2;
      break;
    }
  case CollisionType::AABB:
    {
      AABBCollider aCol = *(AABBCollider*)m_pCollider.get();
      rect.x = aCol.GetBegin().x;
      rect.y = aCol.GetBegin().z;
      rect.width = aCol.GetEnd().x - aCol.GetBegin().x;
      rect.height = aCol.GetEnd().z - aCol.GetBegin().z;
      break;
    }
  case CollisionType::OBB:
    {
      OBBCollider oCol = *(OBBCollider*)m_pCollider.get();
      float minXPos = oCol.GetPosition().x - (std::max(oCol.m_width, oCol.m_depth) / 2) * 1.4f;
      float minZPos = oCol.GetPosition().z - (std::max(oCol.m_width, oCol.m_depth) / 2) * 1.4f;
      rect.x = minXPos;
      rect.y = minZPos;
      rect.width = std::max(oCol.m_width, oCol.m_depth) * 1.4f;
      rect.height = std::max(oCol.m_width, oCol.m_depth) * 1.4f;
      break;
    }
  }

  return rect;
}


void Entity::SetCollider(std::string a_colliderData)
{
  std::vector<std::string> colliderData = explode(a_colliderData, '|');
  if(colliderData[0] == "sphere")
  {
    m_pCollider = std::unique_ptr<Collider>(new SphereCollider(atof(colliderData[1].c_str()), &m_position));
  }
  else if(colliderData[0] == "aabb")
  {
    //TODO
  }
  else if(colliderData[0] == "obb")
  {
    //TODO
  }
}