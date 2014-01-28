#include "Mesh.h"
#include <assert.h>
/* OpenGL specific includes*/
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
#include "wglext.h"
/* End of OpenGL specific includes*/
#include <string>
#include <iostream>
#include <fstream>
#include "ShaderManager.h"
#include "TextureManager.h"

using namespace GameLayer;

Mesh::Mesh(const char* a_path)
{
  Load(a_path);
}


Mesh::~Mesh(void)
{
  glDeleteLists(m_listId, 1);
}


void Mesh::Load(const char* a_path)
{
  //Used http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ \
  for learning how to load an .OBJ model.
  FILE* file = fopen(a_path, "r");
  if(!file)
    assert(0 && "Can't open file!");
  
  std::vector<Vector3> normals;
  std::vector<Vector3> vertices;
  std::vector<Vector2> uvcoords;
    
  while(1)
  {
    char lineHeader[128];  

    int res = fscanf(file, "%s", lineHeader);
    if(res == EOF)
      break;

    if(strcmp(lineHeader, "v") == 0) //First character of line is "v", indicating vertex
    {
      Vector3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      m_vertices.push_back(vertex);
    }
    else if ( strcmp( lineHeader, "vt" ) == 0 )
    {
      Vector2 uvCoord;
      fscanf(file, "%f %f\n", &uvCoord.x, &uvCoord.y );
      m_uvcoords.push_back(uvCoord);
    }
    else if ( strcmp( lineHeader, "vn" ) == 0 )
    {
      Vector3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
      m_normals.push_back(normal);
    }
    else if ( strcmp( lineHeader, "f" ) == 0 )
    {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
        &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
        &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
        &vertexIndex[2], &uvIndex[2], &normalIndex[2] 
      );

      if (matches != 9){
          assert(0 && "Error found in your file.");
      }

      m_vertexIndices.push_back(vertexIndex[0]);
      m_vertexIndices.push_back(vertexIndex[1]);
      m_vertexIndices.push_back(vertexIndex[2]);
      m_uvcoordIndices.push_back(uvIndex[0]);
      m_uvcoordIndices.push_back(uvIndex[1]);
      m_uvcoordIndices.push_back(uvIndex[2]);
      m_normalIndices.push_back(normalIndex[0]);
      m_normalIndices.push_back(normalIndex[1]);
      m_normalIndices.push_back(normalIndex[2]);
    }
  }
  fclose(file);

  m_listId = glGenLists(1);
  glNewList(m_listId, GL_COMPILE);
    glBegin(GL_TRIANGLES);
      for(int i = 0; i < m_vertexIndices.size(); i++)
      {
        glTexCoord2f(m_uvcoords[m_uvcoordIndices[i]-1].x, m_uvcoords[m_uvcoordIndices[i]-1].y);
        glNormal3f(
          m_normals[m_normalIndices[i]-1].x, 
          m_normals[m_normalIndices[i]-1].y, 
          m_normals[m_normalIndices[i]-1].z
        );
        glVertex3f(
          m_vertices[m_vertexIndices[i]-1].x, 
          m_vertices[m_vertexIndices[i]-1].y, 
          m_vertices[m_vertexIndices[i]-1].z
        );
      }
    glEnd();
  glEndList();
}


void Mesh::Draw()
{
  glCallList(m_listId);
}