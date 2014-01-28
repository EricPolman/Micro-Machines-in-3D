#pragma once

namespace GameLayer{
  enum SpriteState {VISIBLE, INVISIBLE, DESTROYED};

  class Sprite
  {
  public:
    virtual void Draw() = 0;
    virtual void Rotate(float a_amount, int x = 0, int y = 0, int z = 1) = 0;
    virtual void Scale(float a_amount) = 0;
    virtual void Translate(float a_x, float a_y, float a_z = 0) = 0;
    virtual float GetWidth() = 0;
    virtual float GetHeight() = 0;
    
    virtual void Destroy() = 0;
    SpriteState GetSpriteState() const{ return m_state; }
    void SetVisibility(bool a_visibility)
    {
      if(a_visibility) m_state = SpriteState::VISIBLE;
      else m_state = SpriteState::INVISIBLE;
    }

    static Sprite* Create(char* a_path);

    static void SetRenderTarget(void* a_pTarget);
  protected:
    SpriteState m_state;
  };
};