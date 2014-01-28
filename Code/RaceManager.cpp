#include "RaceManager.h"
#include <iostream> 
#include "TextRenderer.h"
#include <sstream> 
#include "Boat.h"
#include "Camera.h"

using namespace GameLayer;

RaceManager::~RaceManager(void)
{
  m_waypoints.clear();
  m_racerData.clear();
}

void RaceManager::Init()
{
  TextRenderer::GetInstance()->AddString("lapcounter", "Lap: 1 of 4", "andes-28");
  TextRenderer::GetInstance()->AddString("timer", "0 seconds", "andes-28");
  TextRenderer::GetInstance()->AddString("wrongdir", "Wrong direction", "andes-28");
  TextRenderer::GetInstance()->AddString("finished", "Finished in some time", "andes-50");
  //Sounds->Pause();
  m_countDown = std::unique_ptr<CountDown>(new CountDown());
  //m_countDown->Translate(100, 100);
}

void RaceManager::Update(float a_fDeltaTime)
{
  std::stringstream sstr;
  std::string str;

  if(m_countDown.get() != nullptr)
  {
    m_countDown->Update(a_fDeltaTime);
    if(m_countDown->m_bDestroyMe)
      m_countDown.reset();
  }
  switch(m_raceState)
  {
  default:
  case RaceState::STARTING:
    m_fCountDownTimer += a_fDeltaTime;

    if(m_fCountDownTimer > 4)
    {
      SetState(RaceState::RUNNING);
    }
    break;
  case RaceState::RUNNING:
    {
      sstr << "Lap: " << (m_racerData.begin()->second.lapNumber + 1);
      sstr << " of 4";
      str = sstr.str();
      TextRenderer::GetInstance()->UpdateString("lapcounter", str);

      sstr.str("");
      sstr << "Time: " << (int)m_racerData.begin()->second.raceTime / 60 << ":";
      
      if((int)m_racerData.begin()->second.raceTime % 60 < 10)
        sstr << "0";
      sstr << ((int)m_racerData.begin()->second.raceTime % 60) << "";
      str = sstr.str();
      TextRenderer::GetInstance()->UpdateString("timer", str);

      for(auto i = m_racerData.begin(); i != m_racerData.end(); i++)
      {
        if(i->second.lapNumber + 1 > 4)
          m_raceState = RaceState::ENDED;

        if(!i->second.finished)
        {
          i->second.raceTime += a_fDeltaTime;
        }
      }
    }
  break;
  case RaceState::PAUSED:

    break;
  case RaceState::ENDED:
    sstr << "Finished in: " << (int)m_racerData.begin()->second.raceTime / 60 << ":";
      
    if((int)m_racerData.begin()->second.raceTime % 60 < 10)
      sstr << "0";
    sstr << ((int)m_racerData.begin()->second.raceTime % 60) << "";
    str = sstr.str();

    char* lap = const_cast<char*>(str.c_str());
    TextRenderer::GetInstance()->UpdateString("finished", str);
    break;
  }
}


void RaceManager::Draw()
{
  switch(m_raceState)
  {
  default:
  case RaceState::STARTING:
    {

    }
    break;
  case RaceState::RUNNING:
    {
      if(m_racerData.size() > 0)
      {
        TextRenderer::GetInstance()->DrawString("lapcounter", Vector2(10,10));
  
        TextRenderer::GetInstance()->DrawString("timer", Vector2(10,50));

        auto nextWaypoint = m_waypoints.begin();
        for(;nextWaypoint != m_waypoints.end(); nextWaypoint++)
        {
          if(*nextWaypoint == m_racerData.begin()->second.currentWaypoint)
          {
            nextWaypoint++;
            break;
          }
        }
        if(nextWaypoint == m_waypoints.end())
          nextWaypoint = m_waypoints.begin();
    
        Boat* racerOne = (Boat*)m_racerData.begin()->first;

        if((Camera::GetInstance()->m_direction).Dot((*nextWaypoint)->GetPosition() - racerOne->GetPosition()) < 0)
        {
          TextRenderer::GetInstance()->DrawString("wrongdir", Vector2(20,600 - 48));
        }
      }
    }
  break;
  case RaceState::PAUSED:

    break;
  case RaceState::ENDED:
    {
      TextRenderer::GetInstance()->DrawString("finished", Vector2(70,280));
    }
    break;
  }
}


void RaceManager::AddWaypoint(Waypoint* a_waypoint)
{
  m_waypoints.push_back(a_waypoint);
}


void RaceManager::ProcessWaypointHit(Waypoint* a_waypoint, Entity* a_entity)
{
  if(a_entity->GetType() == EntityType::RACER)
  {
    int curWaypoint = 0;
    for(; curWaypoint < m_waypoints.size(); curWaypoint++ )
    {
      if(a_waypoint == m_waypoints[curWaypoint])
        break;
    }
    int targetPrevious = 
      (curWaypoint == 0 ? m_waypoints.size()-1 : curWaypoint-1);


    if(m_racerData[a_entity].currentWaypoint == m_waypoints[targetPrevious])
    {
      if(a_waypoint == m_waypoints[0])
      {
        m_racerData[a_entity].lapNumber++;
        SpawnMines();
      }
      m_racerData[a_entity].currentWaypoint = a_waypoint;
    }
    else if(m_racerData[a_entity].currentWaypoint == nullptr)
    {
      m_racerData[a_entity].currentWaypoint = a_waypoint;
    }
  }
}


void RaceManager::AddMineSpawner(MineSpawner* a_pSpawner)
{
  m_mineSpawners.push_back(a_pSpawner);
}


void RaceManager::SpawnMines()
{
  for(auto i = m_mineSpawners.begin(); i != m_mineSpawners.end(); i++)
  {
    (*i)->Spawn();
  }
}