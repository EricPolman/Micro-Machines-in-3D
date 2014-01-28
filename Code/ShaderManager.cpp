#include "ShaderManager.h"
#include "InputManager.h"
#include "OpenGLFunctions.h"

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

PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;

ShaderManager::~ShaderManager(void)
{
  for(auto i = m_shaders.begin(); i != m_shaders.end(); i++)
  {
    glDeleteShader(i->second.vertexShaderId);
    glDeleteShader(i->second.fragmentShaderId);
    glDeleteProgram(i->second.programId);
  }
  m_shaders.clear();
}


ShaderManager::ShaderManager()
{
  if(glShaderSource == 0)
  {
    glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
  }
}


void ShaderManager::Update(float a_fDeltaTime)
{
  /*if(Input->IsKeyDown(SDLK_UP))
  {
    lightpos[2] -= 0.05;
  }
  if(Input->IsKeyDown(SDLK_DOWN))
  {
    lightpos[2] += 0.05;
  }
  if(Input->IsKeyDown(SDLK_LEFT))
  {
    lightpos[0] -= 0.05;
  }
  if(Input->IsKeyDown(SDLK_RIGHT))
  {
    lightpos[0] += 0.05;
  }*/
}

//Helpful debugging piece, found it in a tutorial, but lost the link.
void ShaderManager::PrintShaderInfoLog(unsigned int obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

  if (infologLength > 0)
  {
      infoLog = (char *)malloc(infologLength);
      glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
  }
}

void ShaderManager::PrintProgramInfoLog(unsigned int obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

  glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

  if (infologLength > 0)
  {
      infoLog = (char *)malloc(infologLength);
      glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
  }
}


void ShaderManager::Load(std::string a_name, std::string a_pathWithoutExtension)
{
  ShaderData data;

  /* Loading GL_VERTEX_SHADER */
  std::ifstream vertShader(a_pathWithoutExtension + ".vert.glsl");
  std::string vertStr;
  std::string vertShaderContent;
  while(std::getline(vertShader, vertStr))
  {
    vertShaderContent += vertStr;
  }
  int vertShaderLength = vertShaderContent.size();
  
  const char* vertContents = vertShaderContent.c_str();
  
  data.vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(data.vertexShaderId, 1, &vertContents, &vertShaderLength);
  glCompileShader(data.vertexShaderId);

  /* End of loading GL_VERTEX_SHADER */

  /* Loading GL_FRAGMENT_SHADER */
  std::ifstream fragShader(a_pathWithoutExtension + ".frag.glsl");
  std::string fragStr;
  std::string fragStrContent;
  while(std::getline(fragShader, fragStr))
  {
    fragStrContent += fragStr;
  }
  int fragSize = fragStrContent.size();
  
  const char* fragContents = fragStrContent.c_str();
  
  data.fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(data.fragmentShaderId, 1, &fragContents, &fragSize);
  glCompileShader(data.fragmentShaderId);

  /* End of loading GL_FRAGMENT_SHADER */
  
  data.programId = glCreateProgram();
  glAttachShader(data.programId, data.vertexShaderId);
  glAttachShader(data.programId, data.fragmentShaderId);

  glLinkProgram(data.programId);
  
  PrintShaderInfoLog(data.vertexShaderId);
  PrintShaderInfoLog(data.fragmentShaderId);
  PrintProgramInfoLog(data.programId);

  m_shaders[a_name] = data;
}


unsigned int ShaderManager::GetProgram(std::string name)
{
  return m_shaders[name].programId;
}