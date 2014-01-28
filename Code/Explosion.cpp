#include "Explosion.h"
#include "SphereCollider.h"
#include "OpenGLFunctions.h"
#include "Camera.h"
#include "SoundManager.h"

using namespace GameLayer;

Explosion::Explosion(Vector3 a_position)
  : Entity(a_position), m_fExpansionTimer(0),
  m_frames(5,5), m_fFrameTimer(0)
{
  m_pCollider = std::unique_ptr<Collider>(
    new SphereCollider(2, &m_position)
  );
  m_type = EntityType::EXPLOSION;
  AssignTexture("explosion_sheet");
  AssignShader("explosion");
  Scale(0.5f);
  SetOccludability(false);
  Sounds->Play("explosion");
}


Explosion::~Explosion(void)
{
}


void Explosion::Update(float a_fDeltaTime)
{
  m_fExpansionTimer += a_fDeltaTime;
  ((SphereCollider*)m_pCollider.get())->SetRadius(2 + m_fExpansionTimer * 15);
  Scale(1 + a_fDeltaTime * 5);
  if(m_fExpansionTimer > 0.6f)
    m_state = EntityState::REMOVED;

  m_fFrameTimer += a_fDeltaTime;
  if(m_fFrameTimer > 0.6f / (m_frames.x * m_frames.y))
  {
    m_fFrameTimer -= 0.6f / (m_frames.x * m_frames.y);
    m_currentFrame.x++;
    if(m_currentFrame.x == m_frames.x)
    {
      m_currentFrame.x = 0;
      m_currentFrame.y++;
    }
  }
}


void Explosion::Draw()
{
  Vector3 objToCam = Camera::GetInstance()->m_position - m_position;
  Vector3 objToCamProj = Vector3(objToCam.x, 0, objToCam.z);
  objToCamProj.Normalize();
  objToCam.Normalize();
  float cosine = objToCamProj.Dot(Vector3(0,0,1));
  Vector3 upVector = Vector3(0,0,1).Cross(objToCamProj);
  
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

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
    glBindTexture(GL_TEXTURE_2D, m_renderData.m_textureId);
    glBegin(GL_QUADS);
      float uStart = m_currentFrame.x / m_frames.x;
      float vStart = 1 - m_currentFrame.y / m_frames.y;
      glTexCoord2f(uStart,vStart-0.2);
      glVertex3f(-3,3,0);
      glTexCoord2f(uStart + 0.2f,vStart-0.2);
      glVertex3f(3,3,0);
      glTexCoord2f(uStart + 0.2f,vStart);
      glVertex3f(3,-3,0);
      glTexCoord2f(uStart,vStart);
      glVertex3f(-3,-3,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
}