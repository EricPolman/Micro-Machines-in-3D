#include "OpenGLFunctions.h"
#include "wglext.h"

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLUNIFORM3FPROC glUniform3f = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;

using namespace GameLayer;

void OGLF::Init()
{
  if(glGetUniformLocation == 0)
  {
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
    glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
  }
}

GLint OGLF::GetUniformLocation(GLuint program, const GLchar* name)
{
  return glGetUniformLocation(program, name);
}
void OGLF::Uniform1f(GLint location, GLfloat value)
{
  glUniform1f(location, value);
}
void OGLF::Uniform3f(GLint location, Vector3 value)
{
  glUniform3f(location, value.x, value.y, value.z);
}
void OGLF::GenBuffers(GLsizei n, GLuint* buffers)
{
  glGenBuffers(n, buffers);
}
void OGLF::BindBuffer(GLenum target, GLuint buffer)
{
  glBindBuffer(target, buffer);
}
void OGLF::BufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
  glBufferData(target, size, data, usage);
}

void OGLF::EnableVertexAttribArray(GLuint index)
{
  glEnableVertexAttribArray(index);
}

void OGLF::VertexAttribPointer(GLuint index, GLint size, GLenum type, 
  GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void OGLF::DeleteBuffers(GLsizei amount, GLuint* ids)
{
  glDeleteBuffers(amount, ids);
}
  

void OGLF::Prepare2D()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0.0f, 800, 600, 0.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glTranslatef(0.375, 0.375, 0.0);

  //glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  //glDepthMask(false);
  glClear(GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_TEXTURE_2D);
  glCullFace(GL_FRONT);
  
}


void OGLF::Prepare3D()
{
  glViewport( 0, 0, 800, 600 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
 	//glOrtho( 0, SCRWIDTH, 0, SCRHEIGHT, -10000, 10000);
	gluPerspective(90, 800.0/600, 0.1, 1000);
  
  glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
	glEnable( GL_TEXTURE_2D );
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
	glShadeModel( GL_SMOOTH );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);

  glEnable(GL_DEPTH_TEST);
  
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glDepthMask(true);
}