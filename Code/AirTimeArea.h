#pragma once
#include "entity.h"

namespace GameLayer
{
  class AirTimeArea :
    public Entity
  {
  public:
    AirTimeArea(std::string a_mesh, Vector3 a_position, Vector3 a_size, bool a_bIsBouncy = true);
    virtual ~AirTimeArea(void);

    virtual void Update(float a_fDeltaTime);
    //virtual void Draw();

    virtual void Hit(Entity* a_pOther, Vector3& a_intersection);
  private:    
    float m_fShaderTime;
    bool m_bIsBouncy;
  };
};
