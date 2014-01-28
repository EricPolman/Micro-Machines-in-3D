#pragma once
#include "manager.h"
#include <map>
#include <string>
#include <queue>
#include "Entity.h"
#include "QuadTreeNode.h"
#include "Singleton.h"

namespace GameLayer{
  class EntityManager : public Manager, public Singleton<EntityManager>
  {
  public:
	  EntityManager(void);
	  ~EntityManager(void);
    virtual void Update(float a_fDeltaTime);
    void Draw();
    void Add(Entity* a_object);
  private:
    
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::queue<Entity*> m_queue;
    float m_fFrameTimer;

    QuadTreeNode* m_staticsNode;
  };
};

#define Entities EntityManager::GetInstance()