#include "GuiButton.h"
#include "InputManager.h"
#include "SpriteManager.h"

using namespace GameLayer;

GuiButton::GuiButton(float a_fX, float a_fY, std::string a_buttonUnique, void (*a_funcionPointer)())
{
	m_position.x = a_fX;
	m_position.y = a_fY;
  std::string normal("assets/sprites/buttons/button_"+a_buttonUnique+"_normal.png");
  int normalId = Sprites->Add(const_cast<char*>(normal.c_str()));
  std::string hover("assets/sprites/buttons/button_"+a_buttonUnique+"_hover.png");
  int hoverId = Sprites->Add(const_cast<char*>(hover.c_str()));
  std::string down("assets/sprites/buttons/button_"+a_buttonUnique+"_down.png");
  int downId = Sprites->Add(const_cast<char*>(down.c_str()));
  
  m_pButtonNormal = Sprites->Get(normalId).lock();
  m_pButtonNormal->Translate(a_fX, a_fY, 1);
  m_pButtonHover = Sprites->Get(hoverId).lock();
  m_pButtonHover->Translate(a_fX, a_fY, 1);
  m_pButtonDown = Sprites->Get(downId).lock();
  m_pButtonDown->Translate(a_fX, a_fY, 1);
	m_state = BUTTON_NORMAL;

	m_funcionPointer = a_funcionPointer;
}


GuiButton::~GuiButton(void)
{
}


void GuiButton::Update(float a_fDeltaTime)
{
	Vector2i mousePos = Input->GetMousePosition();
	//std::cout << mousePos.x << " " << mousePos.y << std::endl;
	if(	mousePos.x > m_position.x - m_pButtonNormal->GetWidth() * 0.5f && mousePos.x < 
		m_position.x + m_pButtonNormal->GetWidth() * 0.5f &&
		mousePos.y > m_position.y - m_pButtonNormal->GetHeight() * 0.5f && mousePos.y < 
		m_position.y + m_pButtonNormal->GetHeight() * 0.5f)
	{
		if(Input->IsMouseButtonDown(SDL_BUTTON_LEFT))
		{
			m_state = BUTTON_DOWN;
		}
		else if(Input->IsMouseButtonUp(SDL_BUTTON_LEFT))
		{
			m_funcionPointer();
		}
		else
		{
			m_state = BUTTON_HOVER;
		}
	}
	else
	{
		m_state = BUTTON_NORMAL;
	}
  ChangeState();
}


void GuiButton::ChangeState()
{
	switch(m_state)
	{
	case BUTTON_NORMAL:
    m_pButtonNormal->SetVisibility(true);
    m_pButtonDown->SetVisibility(false);
    m_pButtonHover->SetVisibility(false);
		break;
	case BUTTON_HOVER:
    m_pButtonNormal->SetVisibility(false);
    m_pButtonDown->SetVisibility(false);
    m_pButtonHover->SetVisibility(true);
		break;
	case BUTTON_DOWN:
    m_pButtonNormal->SetVisibility(false);
    m_pButtonDown->SetVisibility(true);
    m_pButtonHover->SetVisibility(false);
		break;
	}
}