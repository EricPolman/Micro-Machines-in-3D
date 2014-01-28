#pragma once
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
#include "UtilityClasses.h"

namespace GameLayer
{
  class OpenGLFunctions
  {
  public:
    static void Init();
  
    static GLint GetUniformLocation(GLuint program, const GLchar* name);
    static void Uniform1f(GLint location, GLfloat value);
    static void Uniform3f(GLint location, Vector3 value);
    static void GenBuffers(GLsizei n, GLuint* buffers);
    static void BindBuffer(GLenum target, GLuint buffer);
    static void DeleteBuffers(GLsizei, GLuint*);
    //static void DrawArrays(GLenum mode, GLint first, GLsizei size);
    static void EnableVertexAttribArray(GLuint);
    static void VertexAttribPointer(GLuint index, GLint size, GLenum type, 
      GLboolean normalized, GLsizei stride, const GLvoid* pointer);
    static void BufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
    static void Prepare2D();
    static void Prepare3D();
  };
};

#define OGLF OpenGLFunctions