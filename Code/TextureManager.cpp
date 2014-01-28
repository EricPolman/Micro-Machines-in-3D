#include "TextureManager.h"
#include "InputManager.h"
#include "FreeImage.h"
#include <iostream>

/* OpenGL specific includes*/
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
/* End of OpenGL specific includes*/

#include <string>
#include <fstream>
#include "OpenGLFunctions.h"

using namespace GameLayer;


TextureManager::~TextureManager(void)
{
  for(auto i = m_textures.begin(); i != m_textures.end(); i++)
  {
    glDeleteTextures(1, &i->second);
  }
  m_textures.clear();
}

void TextureManager::Load(std::string name, std::string a_path)
{
  char* path = const_cast<char*>(a_path.c_str());
  unsigned int id = 0;
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType( path, 0 );
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename( path );
	FIBITMAP* tmp = FreeImage_Load( fif, path );
	FIBITMAP* dib = FreeImage_ConvertTo32Bits( tmp );
	int width = FreeImage_GetWidth( dib );
	int height = FreeImage_GetHeight( dib );

  glGenTextures( 1, &id );
  glBindTexture( GL_TEXTURE_2D, id );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA8, 
    width, height, 0, GL_BGRA, 
    GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(dib) 
  );
  
  glBindTexture( GL_TEXTURE_2D, 0 );

  //Remove buffers
	FreeImage_Unload( tmp );
	FreeImage_Unload( dib );

  m_textures[name] = id;
}


unsigned int TextureManager::GetTexture(std::string name)
{
  return m_textures[name];
}