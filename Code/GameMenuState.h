#pragma once
#include "state.h"
#include "Core.h"
#include "Singleton.h"
#include "Sprite.h"
#include "GuiButton.h"

namespace GameLayer
{
  class GameMenuState :
    public State<Core>, public Singleton<GameMenuState>
  {
  public:
    GameMenuState(void);
    ~GameMenuState(void);
  
	  virtual void Enter(Core* a_core);
	  virtual void Update(Core* a_core, float a_fDeltaTime);
	  virtual void Draw(Core* a_core);
	  virtual void Exit(Core* a_core);
    
    static void StartRace();
    static void QuitGame();
    bool m_bStartRace;
    std::shared_ptr<Sprite> m_pBackgroundSprite;
    GuiButton *m_startbutton, *m_quitbutton;
  };
};
