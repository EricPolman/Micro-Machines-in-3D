#pragma once
#include "Entity.h"

namespace GameLayer
{
  class QuadTreeNode
  {
  public:
    QuadTreeNode(
      float x, float y, float width, float height, int maxElements, int maxLevels);
    ~QuadTreeNode(void);
  
    void Add(std::shared_ptr<Entity> a_entity, int level);
    void Remove(Entity* a_entity);
    void Split();
    void Update(float a_fDeltaTime);
    void Draw();
    void CheckCollision(Entity* a_entity);
  
    Rectangle m_bounds;

    QuadTreeNode* m_children[4];
    std::vector<std::shared_ptr<Entity>> m_entities;
    int m_maxElements;
    int m_maxLevels;

    bool m_bPlayerInRect;
  };
};