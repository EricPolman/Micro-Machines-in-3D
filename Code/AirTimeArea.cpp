#include "AirTimeArea.h"
#include "OpenGLFunctions.h"
#include "BoxCollider.h"
#include "Camera.h"
#include "Boat.h"
#include <algorithm>

using namespace GameLayer;

AirTimeArea::AirTimeArea(std::string a_mesh, Vector3 a_position, Vector3 a_size, bool a_bIsBouncy)
  : Entity(a_mesh, a_position), m_fShaderTime(0)
{
  m_type = EntityType::STATIC;
  m_pCollider = std::unique_ptr<Collider>(
    new AABBCollider(&m_position, a_size.x, a_size.y, a_size.z)
  );
  m_pCollider->m_bIsTrigger = true;
  AssignShader("diffuse");
  m_bIsBouncy = a_bIsBouncy;
}


AirTimeArea::~AirTimeArea(void)
{
}


void AirTimeArea::Update(float a_fDeltaTime)
{
  m_fShaderTime += a_fDeltaTime;
}

/*void AirTimeArea::Draw()
{
  glEnable(GL_BLEND);

  glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale.x, m_scale.y, m_scale.z);
    
    glUseProgram(m_renderData.m_programId);

    m_renderData.m_pMesh->Draw();

    glUseProgram(0);
  glPopMatrix();
  glDisable(GL_BLEND);
}*/


void AirTimeArea::Hit(Entity* a_pOther, Vector3& a_intersection)
{
  switch(a_pOther->GetType())
  {
  case EntityType::RACER:
    {
      Boat* boat = (Boat*)a_pOther;
      boat->InAirTimeArea(true);

      if(m_bIsBouncy)
      {
        boat->Move(Vector3(0, (2 - boat->GetPosition().y) * 0.01f, 0));
      }
      else
      {
        boat->SetPosition(Vector3(boat->GetPosition().x, std::min(boat->GetPosition().y + (3 - boat->GetPosition().y) * 0.04f, 3.0f), boat->GetPosition().z));
      }
    }
    break;
  case EntityType::BUBBLE:
    {

    }
    break;
  }
}