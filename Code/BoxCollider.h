#pragma once
#include "collider.h"
#include "UtilityClasses.h"

namespace GameLayer
{
  class AABBCollider :
    public Collider
  {
  public:
    AABBCollider(Vector3* a_position, float a_width, float a_height, float a_depth);
    ~AABBCollider(void);

    virtual Vector3 Intersects(Collider* a_pOther){return Vector3();}
    inline Vector3& GetPosition() const {return *m_position;}
    const inline Vector3& GetBegin() const {return m_begin;}
    const inline Vector3& GetEnd() const {return m_end;}
  
    Vector3 *m_position;
    Vector3 m_begin, m_end;
  };

  class OBBCollider :
    public Collider
  {
  public:
    OBBCollider(Vector3* a_position, float a_width, float a_height, float a_depth, float a_rotation);
    ~OBBCollider(void){}

    //We don't want to check collision within the box in this game. TODO for next project; add intersection for boxes.
    virtual Vector3 Intersects(Collider* a_pOther){return Vector3();}
    inline Vector3& GetPosition() const {return *m_position;}
    void SetRotation(float a_amount);
  
    Vector3 *m_position;
    Vector3 m_forward, m_right, m_up;
    float m_width, m_height, m_depth;
  };
};
