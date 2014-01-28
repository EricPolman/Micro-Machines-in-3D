#pragma once
#include "Manager.h"
#include <vector>
#include "SDL_events.h"
#include "State.h"

namespace GameLayer{
  class Core
  {
  public:
    static Core* GetInstance(void);

    void Init(void* a_pGraphics);
    bool Update(float a_fDeltaTime);

    void ChangeState(State<Core>* a_newState);

    void Register(Manager* a_pManager);
    void Unregister(Manager* a_pManager);
    
    std::vector<Manager*>& GetManagers(){return m_managers;}

    static void* g_pRenderTarget;
    void PassEvent(SDL_Event& a_event);
    void QuitGame();
  private:
    State<Core>* m_state;
    bool m_bQuit;
    static bool instanceFlag;
	  static Core* single;

    std::vector<Manager*> m_managers;
  };
};

#define GameLayerCore GameLayer::Core::GetInstance()
