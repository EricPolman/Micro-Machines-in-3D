#include "InputManager.h"

using namespace GameLayer;

InputManager::InputManager(void)
{
}


InputManager::~InputManager(void)
{
}


bool InputManager::IsKeyPressed(SDLKey a_key){return m_keysPressed[a_key];}
bool InputManager::IsKeyDown(SDLKey a_key){return m_keysDown[a_key];}
bool InputManager::IsKeyUp(SDLKey a_key){return m_keysUp[a_key];}

void InputManager::SetKeyPressed(SDLKey a_key){m_keysUp[a_key] = false; m_keysDown[a_key] = true; m_keysPressed[a_key] = true;}
void InputManager::SetKeyUp(SDLKey a_key){m_keysUp[a_key] = true; m_keysDown[a_key] = false;}

void InputManager::Update(float a_fDeltaTime)
{
	for(std::map<SDLKey, bool>::iterator iter = m_keysPressed.begin(); iter != m_keysPressed.end(); iter++)
	{
		iter->second = false;
	}	
	for(std::map<SDLKey, bool>::iterator iter = m_keysUp.begin(); iter != m_keysUp.end(); iter++)
	{
		iter->second = false;
	}

	for(std::map<Uint8, bool>::iterator iter = m_mouseButtonPressed.begin(); iter != m_mouseButtonPressed.end(); iter++)
	{
		iter->second = false;
	}	
	for(std::map<Uint8, bool>::iterator iter = m_mouseButtonUp.begin(); iter != m_mouseButtonUp.end(); iter++)
	{
		iter->second = false;
	}
}

bool InputManager::IsMouseButtonPressed(Uint8 a_mouseButton){return m_mouseButtonPressed[a_mouseButton];}
bool InputManager::IsMouseButtonDown(Uint8 a_mouseButton){return m_mouseButtonDown[a_mouseButton];}
bool InputManager::IsMouseButtonUp(Uint8 a_mouseButton){return m_mouseButtonUp[a_mouseButton];}

void InputManager::SetMouseButtonPressed(Uint8 a_mouseButton)
{
	m_mouseButtonUp[a_mouseButton] = false; 
	m_mouseButtonDown[a_mouseButton] = true; 
	m_mouseButtonPressed[a_mouseButton] = true;
}

void InputManager::SetMouseButtonUp(Uint8 a_mouseButton)
{
	m_mouseButtonUp[a_mouseButton] = true; 
	m_mouseButtonDown[a_mouseButton] = false;
}


void InputManager::SetMousePosition(float a_X, float a_Y)
{
	m_mousePosition.x = a_X;
	m_mousePosition.y = a_Y;
}


void InputManager::SetMouseMovement(float a_X, float a_Y)
{
	m_mouseMovement.x = a_X;
	m_mouseMovement.y = a_Y;
}

Vector2i InputManager::GetMousePosition()
{
	return m_mousePosition;
}