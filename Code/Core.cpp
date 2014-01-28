#include "Core.h"
#include <assert.h>
#include <algorithm>
#include "EventManager.h"
#include "GamePlayingState.h";
#include "GameMenuState.h";
#include "InputManager.h"
#include "OpenGLFunctions.h"

using namespace GameLayer;

void* Core::g_pRenderTarget = nullptr;

bool Core::instanceFlag = false;
Core* Core::single = nullptr;
Core* Core::GetInstance()
{
  if(! instanceFlag)
  {
    single = new Core();
    instanceFlag = true;
    return single;
  }
  else
  {
    return single;
  }
}

void Core::Init(void* a_pGraphics)
{
  m_bQuit = false;
  m_state = nullptr;
  OGLF::Init();
  ChangeState(GameMenuState::GetInstance());
}

bool Core::Update(float a_fDeltaTime)
{
  if(m_state != nullptr)
  {
    m_state->Update(this, a_fDeltaTime);
    m_state->Draw(this);
  }

  return m_bQuit;
}


void Core::Register(Manager* a_pManager)
{
  for(auto i = m_managers.begin(); i != m_managers.end(); i++ )
  {
    if(*i == a_pManager)
      assert(false && "Manager is already in m_managers");
  }
  m_managers.push_back(a_pManager);
}


void Core::Unregister(Manager* a_pManager)
{
  for(auto i = m_managers.begin(); i != m_managers.end(); )
  {
    if(*i == a_pManager)
      i = m_managers.erase(i);
    else
      i++;
  }
}


void Core::PassEvent(SDL_Event& a_event)
{
  EventManager::GetInstance()->HandleEvent(a_event);
}


void Core::ChangeState(State<Core>* a_newState)
{
  if(m_state != nullptr)
    m_state->Exit(this);

  m_state = a_newState;
  m_state->Enter(this);
}


void Core::QuitGame()
{
  m_bQuit = true;
}