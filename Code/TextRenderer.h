#pragma once
#include "Singleton.h"
#include <map>
#include "SDL_ttf.h"
#include "UtilityClasses.h"
#include <string>

namespace GameLayer
{
  struct TextData 
  {
    unsigned int buffer;
    std::string font;
    std::string value;
    Vector2 dimensions;
  };

  class TextRenderer :
    public Singleton<TextRenderer>
  {
  public:
    TextRenderer(void);
    virtual ~TextRenderer(void);

    void AddFont(std::string a_name, std::string a_path, int a_pointSize);
    void DrawString(std::string a_key, Vector2 a_position);
    void AddString(std::string a_key, std::string a_value, std::string a_font);
    void UpdateString(std::string a_key, std::string a_value);
  private:
    std::map<std::string, TTF_Font*> m_fonts;
    std::map<std::string, TextData> m_strings;
  };
};
