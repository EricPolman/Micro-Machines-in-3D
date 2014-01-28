#include "Bubble.h"
#include "OpenGLFunctions.h"
#include "Camera.h"
#include "SphereCollider.h"
#include <iostream>
#include "RaceManager.h"
#include "InvisibleWall.h"

using namespace GameLayer;

Bubble::Bubble(Vector3 a_position, float a_scale)
  : Entity("bubble", a_position), m_fShaderTime(0), 
    m_previousPosition(m_position), m_fDamping(0.985f)
{
  m_scale = Vector3(a_scale, a_scale, a_scale);
  m_pCollider = std::unique_ptr<Collider>(new SphereCollider(a_scale * 0.5, &m_position));
  AssignShader("bubble");
  m_type = EntityType::BUBBLE;
  m_position.y = 0.2f;
}


Bubble::~Bubble(void)
{
}

void Bubble::Update(float a_fDeltaTime)
{
  m_fShaderTime += a_fDeltaTime;
  if(m_fShaderTime > 2 * PI)
    m_fShaderTime -= 2 * PI;
  switch(RaceManager::GetInstance()->GetState())
  {
  case RaceState::RUNNING:
  {
    Vector3 previousPosition = m_position;
    Vector3 diff = m_position - m_previousPosition;
    m_position += diff + m_acceleration * a_fDeltaTime * a_fDeltaTime;

    m_previousPosition = previousPosition;

    m_position = m_position - diff * (1-m_fDamping);
    m_acceleration *= m_fDamping;
    m_position.y = 0;
  }
  break;
  }
}

void Bubble::Draw()
{
  Vector3 objToCam = Camera::GetInstance()->m_position - m_position;
  Vector3 objToCamProj = Vector3(objToCam.x, 0, objToCam.z);
  objToCamProj.Normalize();
  objToCam.Normalize();
  float cosine = objToCamProj.Dot(Vector3(0,0,1));
  Vector3 upVector = Vector3(0,0,1).Cross(objToCamProj);
  
  glEnable(GL_BLEND);

  glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale.x, m_scale.y, m_scale.z);
    
    glRotatef(acos(cosine) * 180/PI, upVector.x, upVector.y, upVector.z);
    
    cosine = objToCamProj.Dot(objToCam);
		if (objToCam.y < 0)
			glRotatef(acos(cosine)*180/PI,1,0,0);	
		else
			glRotatef(acos(cosine)*180/PI,-1,0,0);	

    glUseProgram(m_renderData.m_programId);
    GLint myUniformLocation = 
      OGLF::GetUniformLocation(m_renderData.m_programId, "time");
    OGLF::Uniform1f(myUniformLocation, m_fShaderTime);
    myUniformLocation = 
      OGLF::GetUniformLocation(m_renderData.m_programId, "camPos");
    OGLF::Uniform3f(myUniformLocation, Camera::GetInstance()->m_position);

    m_renderData.m_pMesh->Draw();

    glUseProgram(0);
  glPopMatrix();
  glDisable(GL_BLEND);
}
    
void Bubble::Hit(Entity* a_pOther, Vector3& a_intersection)
{
  if(!a_pOther->GetCollider()->m_bIsTrigger)
  {
    switch(a_pOther->GetType())
    {
    case EntityType::EXPLOSION:
      m_position = m_position - a_intersection * -0.08f;
      break;
    case EntityType::RACER:
      m_position = m_position - a_intersection * -0.05f;
      break;
    case EntityType::BUBBLE:
      m_position = m_position - a_intersection * -0.0075f;
      break;
    case EntityType::STATIC:
      {
        Vector3 normalizedIntersection = a_intersection;
        normalizedIntersection.Normalize();
        InvisibleWall* wallPtr = dynamic_cast<InvisibleWall*>(a_pOther);
        if(wallPtr != nullptr)
        {
          m_position = m_position - normalizedIntersection * -0.05f * wallPtr->GetDensity();
        }
        else
        {
          m_position = m_position - normalizedIntersection * -0.05f;
        }
      }
      break;
    default:
      Vector3 normalizedIntersection = a_intersection;
      normalizedIntersection.Normalize();
      m_position = m_position - normalizedIntersection * -0.05f;
      break;
    }
  }
}