#pragma once
#include "manager.h"
#include "SDL_events.h"
#include <queue>
#include "Singleton.h"

namespace GameLayer{
  class EventManager : Manager, public Singleton<EventManager>
  {
  public:
	  ~EventManager(void);
	  EventManager(void){}
    void HandleEvent(const SDL_Event& event);
    virtual void Update(float a_fDeltaTime);
  private:
    std::queue<SDL_Event> m_eventQueue;
  };
};