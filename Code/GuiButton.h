#pragma once
#include "UtilityClasses.h"
#include "Sprite.h"
#include <string>
#include <algorithm>

namespace GameLayer
{
  enum ButtonState{BUTTON_NORMAL,BUTTON_HOVER,BUTTON_DOWN};

  class GuiButton
  {
  public:
	  GuiButton(float a_fX, float a_fY, std::string a_buttonUnique, void (*a_funcionPointer)());
	  ~GuiButton(void);

	  std::shared_ptr<Sprite> m_pButtonNormal;
	  std::shared_ptr<Sprite> m_pButtonHover;
	  std::shared_ptr<Sprite> m_pButtonDown;
	  ButtonState m_state;
	  Vector2 m_position;

	  void (*m_funcionPointer)();

	  void Update(float a_fDeltaTime);
  private:
    void ChangeState();
  };
};
