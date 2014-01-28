#pragma once
#include "manager.h"
#include <map>
#include <string>
#include "Mesh.h"
#include "Singleton.h"

namespace GameLayer{
  class MeshManager : Manager, public Singleton<MeshManager>
  {
  public:
	  ~MeshManager(void);
	  MeshManager(void){}
    virtual void Update(float a_fDeltaTime);
    void Load(std::string name, std::string a_path);
    Mesh* GetMesh(std::string name);
  private:

    std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
  };
};

#define Meshes MeshManager::GetInstance()