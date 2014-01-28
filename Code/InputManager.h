#pragma once
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include "UtilityClasses.h"
#include <map>
#include "Manager.h"
#include "Singleton.h"

namespace GameLayer
{
  class InputManager : Manager, public Singleton<InputManager>
  {
  public:
	  ~InputManager(void);
	  InputManager(void);
	
	  bool IsKeyPressed(SDLKey a_key);
	  bool IsKeyDown(SDLKey a_key);
	  bool IsKeyUp(SDLKey a_key);
	
	  void SetKeyPressed(SDLKey a_key);
	  void SetKeyUp(SDLKey a_key);

	  bool IsMouseButtonPressed(Uint8 a_mouseButton);
	  bool IsMouseButtonDown(Uint8 a_mouseButton);
	  bool IsMouseButtonUp(Uint8 a_mouseButton);
	
	  void SetMouseButtonPressed(Uint8 a_mouseButton);
	  void SetMouseButtonUp(Uint8 a_mouseButton);
	
	  void SetMousePosition(float x, float y);
	  void SetMouseMovement(float x, float y);
	  Vector2i GetMousePosition();
  
    virtual void Update(float a_fDeltaTime);
  private:
	
	  Vector2i m_mousePosition;
	  Vector2i m_mouseMovement;
	
	  std::map<SDLKey, bool> m_keysDown;
	  std::map<SDLKey, bool> m_keysPressed;
	  std::map<SDLKey, bool> m_keysUp;
	
	  std::map<Uint8, bool> m_mouseButtonDown;
	  std::map<Uint8, bool> m_mouseButtonPressed;
	  std::map<Uint8, bool> m_mouseButtonUp;
  };
};
#define Input InputManager::GetInstance()