#pragma once
#include "state.h"
#include "Core.h"
#include "Singleton.h"

namespace GameLayer
{
  class GamePlayingState :
    public State<Core>, public Singleton<GamePlayingState>
  {
  public:
    GamePlayingState(void);
    ~GamePlayingState(void);
  
	  virtual void Enter(Core* a_core);
	  virtual void Update(Core* a_core, float a_fDeltaTime);
	  virtual void Draw(Core* a_core);
	  virtual void Exit(Core* a_core);

  private:
    float m_fFrameTimer;
    int m_countdownCount;
    
    void LoadAssets(std::string a_assetList);
    void LoadScene(std::string a_sceneFile);

    void Draw2D();
    void Draw3D();
  };
};
