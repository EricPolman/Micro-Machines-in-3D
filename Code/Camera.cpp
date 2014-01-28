#include "Camera.h"
#include "InputManager.h"
extern "C" 
{ 
#include "glew.h" 
}
#include "gl.h"

using namespace GameLayer;


void Camera::Update(float a_fDeltaTime)
{
  Vector3 direction(-cosf(*m_targetRotation * PI/180), 0, sinf(*m_targetRotation * PI/180));
  Vector3 newPos = *m_target - Vector3(0.0f, -2.0f) + direction * 3.0f;
  m_position = m_position + (newPos - m_position) * 8 * a_fDeltaTime;
    
  m_direction = *m_target - m_position;
  m_direction.Normalize();

  if(Input->IsKeyPressed(SDLK_c))
  {
    m_topDown = !m_topDown;
  }
  
}

void Camera::Draw()
{
  if(!m_topDown)
  {
    gluLookAt(
      m_position.x, m_position.y, m_position.z,
      m_target->x, m_target->y, m_target->z,
      0, 1, 0
    );
  }
  else
  {
    gluLookAt(
      0, 120, 0,
      0, 0, 0,
      -1, 0, 0
    );
  }
}