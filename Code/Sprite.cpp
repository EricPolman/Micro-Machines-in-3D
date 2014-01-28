#include "Sprite.h"
#include "Core.h"
#include <assert.h>
#include "UtilityClasses.h"
#include <iostream>

#include "FreeImage.h"
/* TMPL8 specific includes*/
#include "surface.h"
/* End of TMPL8 specific includes*/

/* OpenGL specific includes*/
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
/* End of OpenGL specific includes*/

using namespace GameLayer;
  
/* TMPL8 */
#ifdef GAMELAYER_USE_TMPL8
class Sprite_Tmpl8 : public Sprite
{
public:
  Sprite_Tmpl8(char* a_sPath)
    : m_x(0), m_y(0), m_scale(1), m_rotation(0)
  {
    m_state = SpriteState::VISIBLE;
    m_pSurface = new Tmpl8::Surface(a_sPath);
  }
  virtual ~Sprite_Tmpl8(){ delete m_pSurface; }

  virtual void Draw()
  {
    if(m_state == SpriteState::VISIBLE)
    {
      m_pSurface->CopyToWithRotation(
        m_x, m_y, m_pSurface->GetWidth() * m_scale, 
        m_pSurface->GetHeight() * m_scale, m_rotation, 
        g_pRenderSurface, m_pSurface->GetWidth() * 0.5f * m_scale, 
        m_pSurface->GetHeight() * 0.5f * m_scale, true);
    }
  }
  virtual void Rotate(float a_amount)
  {
    m_rotation += a_amount;
  }
  virtual void Scale(float a_amount)
  {
    m_scale *= a_amount;
    m_pivotX = m_pSurface->GetWidth() * 0.5f * m_scale;
    m_pivotY = m_pSurface->GetHeight() * 0.5f * m_scale;
  }
  virtual void Translate(float a_x, float a_y)
  {
     m_x += a_x;
     m_y += a_y;
  }
  static void SetRenderSurface(Tmpl8::Surface* a_pSurface)
  {
    g_pRenderSurface = a_pSurface;
  }
  virtual void Destroy()
  {
    m_state = SpriteState::DESTROYED;
  }

protected:
  Tmpl8::Surface* m_pSurface;
  float m_x, m_y;
  float m_rotation;
  float m_scale;
  float m_pivotX, m_pivotY;

private:
  static Tmpl8::Surface* g_pRenderSurface;
};

Tmpl8::Surface* Sprite_Tmpl8::g_pRenderSurface = nullptr;
#endif
/* End of TMPL8 */

#ifdef GAMELAYER_USE_OPENGL
class Sprite_OpenGL : public Sprite
{
public:
  Sprite_OpenGL(char* a_sPath)
    : m_scale(1,1,1)
  {
    m_state = SpriteState::VISIBLE;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	  fif = FreeImage_GetFileType( a_sPath, 0 );
	  if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename( a_sPath );
	  FIBITMAP* tmp = FreeImage_Load( fif, a_sPath );
	  FIBITMAP* dib = FreeImage_ConvertTo32Bits( tmp );
	  m_width = FreeImage_GetWidth( dib );
	  m_height = FreeImage_GetHeight( dib );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenTextures( 1, &m_textureId );
    glBindTexture( GL_TEXTURE_2D, m_textureId );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA8, 
      m_width, m_height, 0, GL_BGRA, 
      GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(dib) 
    );
    
    glBindTexture( GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    //Remove buffers
	  FreeImage_Unload( tmp );
	  FreeImage_Unload( dib );
    
    std::cout << glGetError() << std::endl;
  }
  virtual ~Sprite_OpenGL(){glDeleteTextures(1, &m_textureId); }

  virtual void Draw()
  {
    glPushMatrix();
      glEnable(GL_BLEND);
      glEnable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glTranslatef(m_position.x, m_position.y, m_position.z);
      glScalef(m_scale.x, m_scale.y, m_scale.z);
      glRotatef(m_rotation.x, 1, 0, 0);
      glRotatef(m_rotation.y, 0, 1, 0);
      glRotatef(m_rotation.z, 0, 0, 1);
      glTranslatef(-m_width/2, -m_height/2, 0);

      //glColor3f( 1.0f, 1.0f, 1.0f );
      glBindTexture( GL_TEXTURE_2D, m_textureId );

      glBegin( GL_QUADS );
	      glNormal3f( 0, 0, 1 );
	      glTexCoord2f( 0.0f, 1.0f );
	      glVertex2f  ( 0.0f, 0 );
	      glTexCoord2f( 1.0f, 1.0f );
	      glVertex2f  ( m_width, 0 );
	      glTexCoord2f( 1.0f, 0.0f );
	      glVertex2f  ( m_width, m_height );
	      glTexCoord2f( 0.0f, 0.0f );
	      glVertex2f  ( 0.0f, m_height );
      glEnd();
      
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_BLEND);
    glPopMatrix();
    glBindTexture( GL_TEXTURE_2D, 0 );
  }
  virtual void Rotate(float a_amount, int x, int y, int z)
  {
    m_rotation.x += a_amount * x;
    m_rotation.y += a_amount * y;
    m_rotation.z += a_amount * z;
  }
  virtual void Scale(float a_amount)
  {
    m_scale *= a_amount;
  }
  virtual void Translate(float a_x, float a_y, float a_z)
  {
     m_position += Vector3(a_x, a_y, a_z);
  }
  virtual void Destroy()
  {
    m_state = SpriteState::DESTROYED;
  }

  virtual float GetWidth()
  {
    return m_width;
  }

  virtual float GetHeight()
  {
    return m_height;
  }

protected:
  Vector3 m_position;
  Vector3 m_scale;
  Vector3 m_rotation;
  int m_width, m_height;
  GLuint m_textureId;
};
#endif

Sprite* Sprite::Create(char* path)
{
#ifdef GAMELAYER_USE_TMPL8
  return new Sprite_Tmpl8(path);
#else if GAMELAYER_USE_OPENGL
  return new Sprite_OpenGL(path);
#endif
  assert(false && "Unimplemented platform.");
  return nullptr;
};

void Sprite::SetRenderTarget(void* a_pTarget)
{
#ifdef GAMELAYER_USE_TMPL8
    Sprite_Tmpl8::SetRenderSurface((Tmpl8::Surface*)a_pTarget);
#else if GAMELAYER_USE_OPENGL

#endif
  assert(false && "Unimplemented platform.");
}