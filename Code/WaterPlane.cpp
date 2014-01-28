#include "WaterPlane.h"
#include "OpenGLFunctions.h"
#include <iostream>

using namespace GameLayer;

WaterPlane::WaterPlane(Vector3 a_position)
  : Entity("waterplane", a_position), m_fWaveTime(0)
{
  SetOccludability(false);
  AssignTexture("water");
  AssignShader("water");
}


WaterPlane::~WaterPlane(void)
{
}


void WaterPlane::Draw()
{
  glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale.x, m_scale.y, m_scale.z);

    glUseProgram(m_renderData.m_programId);
    glBindTexture(GL_TEXTURE_2D, m_renderData.m_textureId);

    GLint myUniformLocation = 
      OGLF::GetUniformLocation(m_renderData.m_programId, "waveTime");
    OGLF::Uniform1f(myUniformLocation, m_fWaveTime);

    m_renderData.m_pMesh->Draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
  glPopMatrix();
  glDisable(GL_BLEND);
}


void WaterPlane::Update(float a_fDeltaTime)
{
  m_fWaveTime += a_fDeltaTime;
  if(m_fWaveTime > 3.14159265359 * 4)
    m_fWaveTime -= 3.14159265359 * 4;
}