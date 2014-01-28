#pragma once
#include "UtilityClasses.h"
#include "Singleton.h"

namespace GameLayer
{
  class Camera : public Singleton<Camera>
  {
  public:
    ~Camera(void){}
	  Camera(void):m_topDown(false){}
  
    void Update(float a_fDeltaTime);
    void Draw();

    Vector3 m_position;
    Vector3* m_target;
    float* m_targetRotation;
    Vector3 m_direction;

  private:
    bool m_topDown;

  };
};
