#include "GameMenuState.h"
#include "TextRenderer.h"
#include "SpriteManager.h"
#include "OpenGLFunctions.h"
#include "GuiButton.h"
#include "GamePlayingState.h"
#include "InputManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "SoundManager.h"

using namespace GameLayer;


GameMenuState::GameMenuState(void)
{
}


GameMenuState::~GameMenuState(void)
{
}

void GameMenuState::Enter(Core* a_core)
{
  Input;
  OGLF::Prepare2D();
  int spr = Sprites->Add("assets/sprites/menu_background.png");
  m_pBackgroundSprite = Sprites->Get(spr).lock();
  m_pBackgroundSprite->Translate(400, 300, -1);
  m_startbutton = new GuiButton(400,410, "start", &StartRace);
  m_quitbutton = new GuiButton(400,500, "quit", &QuitGame);
  m_bStartRace = false;
  glClearColor(0.929f, 0.239f, 0.239f, 0);
  Sounds->SetMusic("menu_music",true);
}


void GameMenuState::Update(Core* a_core, float a_fDeltaTime)
{
  for(auto i = a_core->GetManagers().begin(); i != a_core->GetManagers().end(); i++ )
  {
    (*i)->Update(a_fDeltaTime);
  }
  
  m_startbutton->Update(a_fDeltaTime);
  m_quitbutton->Update(a_fDeltaTime);

  if(m_bStartRace)
    a_core->ChangeState(GamePlayingState::GetInstance());

}


void GameMenuState::Draw(Core* a_core)
{
  Sprites->Draw();
}


void GameMenuState::Exit(Core* a_core)
{
  a_core->GetManagers().clear();
  TextureManager::Reset();
  SpriteManager::Reset();
  InputManager::Reset();
  EventManager::Reset();
  TextRenderer::Reset();
  SoundManager::Reset();
  m_pBackgroundSprite.reset();
  glClearColor(0, 0, 0, 0);
  delete m_quitbutton;
  delete m_startbutton;
}


void GameMenuState::StartRace()
{
  GetInstance()->m_bStartRace = true;
}

void GameMenuState::QuitGame()
{
  GameLayerCore->QuitGame();
}