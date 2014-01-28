#include "MeshManager.h"
#include "InputManager.h"

/* OpenGL specific includes*/
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
#include "wglext.h"
/* End of OpenGL specific includes*/
#include <string>
#include <fstream>

using namespace GameLayer;

MeshManager::~MeshManager(void)
{
  m_meshes.clear();
}


void MeshManager::Update(float a_fDeltaTime)
{

}


void MeshManager::Load(std::string name, std::string a_path)
{
  m_meshes[name] = std::shared_ptr<Mesh>(new Mesh(a_path.c_str()));
}


Mesh* MeshManager::GetMesh(std::string name)
{
  return m_meshes[name].get();
}