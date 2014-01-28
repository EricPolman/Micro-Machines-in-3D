#pragma once
#include "UtilityClasses.h"
#include <vector>

namespace GameLayer
{
  class Mesh
  {
  public:
    Mesh(const char* a_path);
    ~Mesh(void);

    void Load(const char* a_path);
    void Draw();
  private:
    std::vector<Vector3> m_vertices;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_uvcoords;

    std::vector<unsigned int> m_vertexIndices;
    std::vector<unsigned int> m_normalIndices;
    std::vector<unsigned int> m_uvcoordIndices;
  
    unsigned int m_listId;
  };
};