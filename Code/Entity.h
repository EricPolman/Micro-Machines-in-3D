#pragma once
#include "Mesh.h"
#include "UtilityClasses.h"
#include "Collider.h"

namespace GameLayer{
  struct RenderData 
  {
    RenderData() : m_pMesh(nullptr), m_programId(0), m_textureId(0){;}
    unsigned int m_programId;
    unsigned int m_textureId;
    Mesh* m_pMesh;
  };
  
  enum EntityType {DEFAULT, RACER, BUBBLE, STATIC, EXPLOSION};
  enum EntityState {ACTIVE, INACTIVE, REMOVED};

  class Entity
  {
  public:
    Entity(std::string mesh);
    Entity::Entity(Vector3 a_position);
    Entity(std::string mesh, Vector3 a_position, EntityType a_type = EntityType::DEFAULT);
    virtual ~Entity(void);
    //Entity(const Entity& a_other);
    //Entity& operator=(const Entity& a_other);
    
    virtual void Update(float a_fDeltaTime);
    virtual void Draw();
    virtual void Hit(Entity* a_pOther, Vector3& a_intersection);
    virtual Rectangle GetBoundingRectangle();

    void SetType(EntityType a_type){m_type = a_type;}
	  inline EntityType GetType()const{return m_type;}
    void SetState(EntityState a_state){m_state = a_state;}
	  inline EntityState GetState()const{return m_state;}
    
    void SetPosition(Vector3 a_position){m_position = a_position;}
    void Move(Vector3 a_translation){m_position += a_translation;}
    void Scale(float a_scale) {m_scale *= a_scale;}
    void Rotate(Vector3 a_amount) {m_rotation += a_amount;}
	  inline Vector3 GetPosition()const{return m_position;}
    Vector3 m_position;
    Vector3 m_rotation;
    Vector3 m_previousPosition;
    Vector3 m_scale;
    
    void AssignTexture(std::string a_name);
    void AssignShader(std::string a_name);
    void AssignMesh(std::string a_name);

    void SetCollider(std::string a_colliderData);
    Collider* GetCollider() const {return m_pCollider.get();}
    bool IsOccludable() const {return m_bIsOccludable;}
    void SetOccludability(bool a_value) {m_bIsOccludable = a_value;};
  protected:
    bool m_bIsOccludable;
    EntityType m_type;
    EntityState m_state;
    std::unique_ptr<Collider> m_pCollider;
    RenderData m_renderData;
  };
};
