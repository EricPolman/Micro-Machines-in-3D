#include "EventManager.h"
#include "InputManager.h"

using namespace GameLayer;

EventManager::~EventManager(void)
{
  m_eventQueue.empty();
}

void EventManager::HandleEvent(const SDL_Event& a_event)
{
  m_eventQueue.push(a_event);
}

void EventManager::Update(float a_fDeltaTime)
{
  while(m_eventQueue.size() > 0)
  {
    switch (m_eventQueue.front().type)
	  {
	  case SDL_QUIT:
		  //exitapp = 1;
		  break;
	  case SDL_KEYDOWN:
		  if (m_eventQueue.front().key.keysym.sym == SDLK_ESCAPE) 
		  {
			  //exitapp = 1;
			  // find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey
		  }
		  Input->SetKeyPressed( m_eventQueue.front().key.keysym.sym );
		  break;
	  case SDL_KEYUP:
      Input->SetKeyUp( m_eventQueue.front().key.keysym.sym );
		  break;
	  case SDL_MOUSEMOTION:
		  Input->SetMousePosition( m_eventQueue.front().motion.x, m_eventQueue.front().motion.y );
		  Input->SetMouseMovement( m_eventQueue.front().motion.xrel, m_eventQueue.front().motion.yrel );
		  break;
	  case SDL_MOUSEBUTTONUP:
		  Input->SetMouseButtonUp( m_eventQueue.front().button.button );
		  break;
	  case SDL_MOUSEBUTTONDOWN:
		  Input->SetMouseButtonPressed( m_eventQueue.front().button.button );
		  break;
	  default:
		  // more info on events in SDL: http://sdl.beuc.net/sdl.wiki/SDL_Event
		  break;
	  }

    m_eventQueue.pop();
  }
}