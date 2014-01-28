#pragma once
#include "manager.h"
#include <map>
#include <string>
#include "Singleton.h"

namespace GameLayer{
  struct ShaderData
  {
    unsigned int vertexShaderId;
    unsigned int fragmentShaderId;
    unsigned int programId;
  private:
    unsigned int padding;
  };

  class ShaderManager : Manager, public Singleton<ShaderManager>
  {
  public:
	  ~ShaderManager(void);
	  ShaderManager(void);
    virtual void Update(float a_fDeltaTime);
    void Load(std::string name, std::string a_pathWithoutExtension);
    unsigned int GetProgram(std::string name);
  private:

    std::map<std::string, ShaderData> m_shaders;
    void PrintShaderInfoLog(unsigned int obj);
    void PrintProgramInfoLog(unsigned int obj);
  };
};

#define Shaders ShaderManager::GetInstance()