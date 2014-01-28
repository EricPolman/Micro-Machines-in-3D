#pragma once
#include "sprite.h"
#include "UtilityClasses.h"

namespace GameLayer
{
  class CountDown
  {
  public:
    CountDown(void);
    ~CountDown(void);
    void Update(float a_fDeltaTime);
    
    bool m_bDestroyMe;
  protected:
    float m_fFrameTimer;
    float m_fKillTimer;
    std::vector<std::shared_ptr<Sprite>> m_countDownSprites;
    int m_countdownCount;
    void SetVisibilities(int a_visibleIndex);
  };
};
