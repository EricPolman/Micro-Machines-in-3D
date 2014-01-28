#pragma once
#include "manager.h"
#include "Waypoint.h"
#include "MineSpawner.h"
#include "Entity.h"
#include "Singleton.h"
#include <map>
#include "CountDown.h"

namespace GameLayer
{
  struct RacerData
  {
    RacerData() : raceTime(0), lapNumber(0),
      currentWaypoint(nullptr), finished(false) {}
    float raceTime;
    int lapNumber;
    Waypoint* currentWaypoint;
    bool finished;
  };
  enum RaceState {RUNNING, STARTING, ENDED, PAUSED};

  class RaceManager :
    public Manager, public Singleton<RaceManager>
  {
  public:
    RaceManager(void):m_fCountDownTimer(0), m_raceState(RaceState::STARTING){}
    virtual ~RaceManager(void);
    
    void Init();
    virtual void Update(float a_fDeltaTime);
    virtual void Draw();
    
    void AddMineSpawner(MineSpawner* a_pSpawner);
    inline void SpawnMines();
    void AddWaypoint(Waypoint* a_waypoint);
    void ProcessWaypointHit(Waypoint* a_waypoint, Entity* a_entity);
    
    void SetState(RaceState a_state){m_raceState = a_state;}
	  inline RaceState GetState() const{return m_raceState;}
    std::vector<Waypoint*>& GetWaypoints(){return m_waypoints;}
  private:
    std::vector<Waypoint*> m_waypoints;
    std::vector<MineSpawner*> m_mineSpawners;
    std::map<Entity*, RacerData> m_racerData;
    float m_fCountDownTimer;
    RaceState m_raceState;
    std::unique_ptr<CountDown> m_countDown;
  };
};
