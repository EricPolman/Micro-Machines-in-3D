#include "TextRenderer.h"
extern "C" 
{ 
  #include "glew.h" 
}
#include "gl.h"
#include <iostream>

using namespace GameLayer;


TextRenderer::TextRenderer(void)
{
  if (TTF_Init() == -1) 
  {
    printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
    //Put here however you want to do about the error. 
    //you could say:
    //return true;
    //Or:
    //exit(1);
  }
}


TextRenderer::~TextRenderer(void)
{
  for(auto i = m_fonts.begin(); i != m_fonts.end();)
  {
    TTF_Font* ptr = i->second;
    i = m_fonts.erase(i);
    TTF_CloseFont(ptr);
    ptr = NULL;
    //TODO: Fix deleting
  }
  m_fonts.clear();

  for(auto i = m_strings.begin(); i != m_strings.end();i++)
  {
    glDeleteTextures(1, &i->second.buffer);
  }
}


void TextRenderer::AddFont(std::string a_name, std::string a_path, int a_pointSize)
{
  TTF_Font* tmpfont;
  char* path = const_cast<char*>(a_path.c_str());
  tmpfont = TTF_OpenFont(path, a_pointSize);
  if (tmpfont == NULL){
    printf("Unable to load font: %s %s \n", a_path, TTF_GetError());
    // Handle the error here.
  }
  m_fonts[a_name] = tmpfont;
}

 
SDL_Surface *drawtext(TTF_Font *fonttodraw, char fgR, char fgG, char fgB, char fgA, char text[])
{
  SDL_Color tmpfontcolor = {fgR,fgG,fgB,fgA};
  SDL_Surface *resulting_text;
 
  resulting_text = TTF_RenderText_Blended(fonttodraw, text, tmpfontcolor);
 
  return resulting_text;
}

void TextRenderer::DrawString(std::string a_key, Vector2 a_position)
{
  glEnable(GL_TEXTURE_2D);
  glEnable( GL_BLEND );

  glBindTexture(GL_TEXTURE_2D, m_strings[a_key].buffer);

  glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(a_position.x, a_position.y);
    glTexCoord2f(1, 0);
    glVertex2f(a_position.x + m_strings[a_key].dimensions.x, a_position.y);
    glTexCoord2f(1, 1);
    glVertex2f(a_position.x + m_strings[a_key].dimensions.x, a_position.y + m_strings[a_key].dimensions.y);
    glTexCoord2f(0, 1);
    glVertex2f(a_position.x, a_position.y + m_strings[a_key].dimensions.y);
  glEnd();
  
  glBindTexture(GL_TEXTURE_2D, 0); 

  glDisable(GL_TEXTURE_2D);
  glDisable( GL_BLEND );
}


void TextRenderer::AddString(std::string a_key, std::string a_value, std::string a_font)
{
  TextData data;
  data.value = a_value;
  data.font = a_font;

  SDL_Surface *text_surface = drawtext(m_fonts[a_font], 255, 255, 255, 1, const_cast<char*>(a_value.c_str()));

  unsigned int colors, texture_format, texture;

  colors = text_surface->format->BytesPerPixel;
  if (colors == 4) {   // alpha
    if (text_surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGBA;
    else
      texture_format = GL_BGRA_EXT;
  } else {             // no alpha
    if (text_surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGB;
    else
      texture_format = GL_BGR_EXT;
  }
  glEnable(GL_TEXTURE_2D);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    
  glTexImage2D(GL_TEXTURE_2D, 0, colors, text_surface->w, text_surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, text_surface->pixels);

  data.buffer = texture;
  
  data.dimensions = Vector2(text_surface->w, text_surface->h);
  m_strings[a_key] = data;

  SDL_FreeSurface(text_surface);
  glBindTexture(GL_TEXTURE_2D, 0);  
  glDisable(GL_TEXTURE_2D);
}


void TextRenderer::UpdateString(std::string a_key, std::string a_value)
{
  if(m_strings[a_key].value == a_value)
    return;

  SDL_Surface *text_surface = 
    drawtext(m_fonts[m_strings[a_key].font], 255, 255, 255, 1, const_cast<char*>(a_value.c_str()));

  unsigned int colors, texture_format, texture;

  colors = text_surface->format->BytesPerPixel;
  if (colors == 4) {   // alpha
    if (text_surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGBA;
    else
      texture_format = GL_BGRA_EXT;
  } else {             // no alpha
    if (text_surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGB;
    else
      texture_format = GL_BGR_EXT;
  }
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, m_strings[a_key].buffer);
    
  glTexImage2D(GL_TEXTURE_2D, 0, colors, text_surface->w, text_surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, text_surface->pixels);
  
  m_strings[a_key].dimensions = Vector2(text_surface->w, text_surface->h);
  m_strings[a_key].value = a_value;
  
  SDL_FreeSurface(text_surface);
  glBindTexture(GL_TEXTURE_2D, 0);  
  glDisable(GL_TEXTURE_2D);
}
