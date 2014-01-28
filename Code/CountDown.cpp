#include "CountDown.h"
#include "SpriteManager.h"
#include "SoundManager.h"

using namespace GameLayer;

CountDown::CountDown(void)
  : m_fFrameTimer(0), m_countdownCount(0), m_bDestroyMe(false), m_fKillTimer(0)
{
  int i = Sprites->Add("assets/sprites/countdown/countdown_0.png");
  m_countDownSprites.push_back(
    Sprites->Get(i).lock()
  );

  i = Sprites->Add("assets/sprites/countdown/countdown_1.png");
  m_countDownSprites.push_back(
    Sprites->Get(i).lock()
  );

  i = Sprites->Add("assets/sprites/countdown/countdown_2.png");
  m_countDownSprites.push_back(
    Sprites->Get(i).lock()
  );

  i = Sprites->Add("assets/sprites/countdown/countdown_3.png");
  m_countDownSprites.push_back(
    Sprites->Get(i).lock()
  );

  i = Sprites->Add("assets/sprites/countdown/countdown_4.png");
  m_countDownSprites.push_back(
    Sprites->Get(i).lock()
  );

  for(auto i = m_countDownSprites.begin(); i != m_countDownSprites.end(); i++)
  {
    i->get()->Translate(400,50);
  }
}


CountDown::~CountDown(void)
{
}


void CountDown::Update(float a_fDeltaTime)
{
  m_fFrameTimer += a_fDeltaTime;
  if(m_fFrameTimer > 1)
  {
    m_fFrameTimer -= 1;
    if(m_countdownCount < 4)
    {
      Sounds->Play("ding");
      m_countdownCount++;
    }
    SetVisibilities(m_countdownCount);
  }

  if(m_countdownCount == 4)
  {
    m_countDownSprites[4]->Translate(0, -100 * a_fDeltaTime);
    m_fKillTimer += a_fDeltaTime;
    if(m_fKillTimer > 3)
      m_bDestroyMe = true;
  }
}


void CountDown::SetVisibilities(int a_visibleIndex)
{
  for(int i = 0; i < m_countDownSprites.size(); i++)
  {
    if(i == a_visibleIndex)
      m_countDownSprites[i]->SetVisibility(true);
    else
      m_countDownSprites[i]->SetVisibility(false);
  }
}