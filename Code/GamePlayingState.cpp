#include "GamePlayingState.h"
#include "SpriteManager.h"
#include "Shapes.h"
#include "EventManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Boat.h"
#include "AirTimeArea.h"
#include "Bubble.h"
#include "OpenGLFunctions.h"
#include "InvisibleWall.h"
#include "RaceManager.h"
#include "TextRenderer.h"
#include "GameMenuState.h"
#include "MineSpawner.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "SoundManager.h"
#include "WaterPlane.h"

using namespace GameLayer;


GamePlayingState::GamePlayingState(void)
{
}


GamePlayingState::~GamePlayingState(void)
{
}


void GamePlayingState::Enter(Core* a_core)
{
  Input;
  LoadAssets("assets/assets.csv");
  LoadScene("assets/scene.csv");
  RaceManager::GetInstance()->Init();
  
  Entity* e;
  e = new Boat(Vector3(75,0,75));
  e->m_rotation.y = -90;
  e->AssignTexture("boat_diffuse");
  Camera::GetInstance()->m_target = &e->m_previousPosition;
  Camera::GetInstance()->m_targetRotation = &e->m_rotation.y;
  
  std::vector<Waypoint*>& waypoints = RaceManager::GetInstance()->GetWaypoints();

  srand(42 * 42);
  for(auto i = waypoints.begin() + 1; i != waypoints.end(); i++)
  {
    e = new MineSpawner((*i)->GetPosition() + Vector3(-5 + rand()%10, -0.5, -5 + rand()%10), 0);
    e = new MineSpawner((*i)->GetPosition() + Vector3(-5 + rand()%10, -0.5, -5 + rand()%10), 0);
    for(int j = 0; j < 50; j++)
    {
      e = new Bubble((*i)->GetPosition() + Vector3(-5 + rand()%10, 0.3f, -5 + rand()%10), 1 + (float)(rand()%10) / 20);
    }
  }

  RaceManager::GetInstance()->SpawnMines();

  m_fFrameTimer = 0;
}


const float TIMESTEP = 0.02f;
void GamePlayingState::Update(Core* a_core, float a_fDeltaTime)
{
  m_fFrameTimer += a_fDeltaTime;
  if(m_fFrameTimer > TIMESTEP)
  {
    for(auto i = a_core->GetManagers().begin(); i != a_core->GetManagers().end(); i++ )
    {
      (*i)->Update(TIMESTEP);
    }  
    Camera::GetInstance()->Update(TIMESTEP);
    m_fFrameTimer -= TIMESTEP;
    if(m_fFrameTimer > TIMESTEP * 5)
      m_fFrameTimer = TIMESTEP;
  }
  if(Input->IsKeyPressed(SDLK_p))
  {
    a_core->ChangeState(GameMenuState::GetInstance());
  }
}


void GamePlayingState::Draw(Core* a_core)
{
  Draw3D();
  Draw2D();
}


void GamePlayingState::Exit(Core* a_core)
{
  SpriteManager::Reset();
  a_core->GetManagers().clear();
  TextureManager::Reset();
  MeshManager::Reset();
  EntityManager::Reset();
  RaceManager::Reset();
  TextRenderer::Reset();
  Camera::Reset();
  InputManager::Reset();
  EventManager::Reset();
  ShaderManager::Reset();
  SoundManager::Reset();
}


GLfloat lightpos[] = {1,2,-1};
void GamePlayingState::Draw3D()
{
  OGLF::Prepare3D();
  Camera::GetInstance()->Draw();
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  Entities->Draw();
}

void GamePlayingState::Draw2D()
{
  OGLF::Prepare2D();
  RaceManager::GetInstance()->Draw();
  Sprites->Draw();
  //TextRenderer::GetInstance()->DrawString("micro machines", "dream-28", Vector2(10,600 - 38));
}


void GamePlayingState::LoadAssets(std::string a_assetList)
{
  std::ifstream file;
  file.open(a_assetList);

  std::string line;
  std::stringstream sstr;

  while(!file.eof())
  {
    line.clear();
    std::getline(file, line);
    std::vector<std::string> lineExploded = explode(line, ',');
    if(lineExploded.size() == 0)
      continue;

    if(lineExploded[0] == "shader")
    {
      Shaders->Load(lineExploded[1], lineExploded[2]);
    }
    else if(lineExploded[0] == "font")
    {
      TextRenderer::GetInstance()->AddFont
        (lineExploded[1], lineExploded[2], std::atoi(lineExploded[3].c_str()));
    }
    else if(lineExploded[0] == "texture")
    {
      Textures->Load(lineExploded[1], lineExploded[2]);
    }
    else if(lineExploded[0] == "mesh")
    {
      Meshes->Load(lineExploded[1], lineExploded[2]);
    }
    else if(lineExploded[0] == "audio")
    {
      if(lineExploded[1] == "music")
      {
        Sounds->SetMusic(lineExploded[2], true);
      }
      else
      {
        Sounds->Load(lineExploded[2]);
      }
    }
  }

  file.close();
}


void GamePlayingState::LoadScene(std::string a_sceneFile)
{
  std::ifstream file;
  file.open(a_sceneFile);

  std::string line;
  std::stringstream sstr;
  Entity* e;

  while(!file.eof())
  {
    line.clear();
    std::getline(file, line);
    std::vector<std::string> lineExploded = explode(line, ',');
    if(lineExploded.size() == 0 || lineExploded[0] == "type")
      continue;

    if(lineExploded[0] == "entity")
    {
      Vector3 pos;
      sscanf(lineExploded[1].c_str(), "%f|%f|%f", &pos.x, &pos.y, &pos.z);
      
      if(lineExploded[5] != "")
      {  
        e = new Entity(lineExploded[4], pos, EntityType::STATIC);
      }
      else
      {
        e = new Entity(lineExploded[4], pos, EntityType::DEFAULT);
      }

      e->Scale(atof(lineExploded[3].c_str()));

      if(lineExploded[2] != "")
      {
        Vector3 rotation;
        sscanf(lineExploded[2].c_str(), "%f|%f|%f", &rotation.x, &rotation.y, &rotation.z);
        e->Rotate(rotation);
      }
      if(lineExploded[5] != "")
      {
        e->SetCollider(lineExploded[5]);
      }
    }
    else if(lineExploded[0] == "inviswall")
    {
      float x, y, z;
      float w, h, d;
      sscanf(lineExploded[1].c_str(), "%f|%f|%f", &x, &y, &z);
      sscanf(lineExploded[3].c_str(), "%f|%f|%f", &w, &h, &d);

      if(lineExploded[9] == "rotated")
      {
        e = new InvisibleWall(Vector3(x, y, z), w, h, d, PI / 4, atof(lineExploded[5].c_str()));
      }
      else
      {
        e = new InvisibleWall(Vector3(x, y, z), w, h, d, 0, atof(lineExploded[5].c_str()));
      }
    }
    else if(lineExploded[0] == "airtimearea")
    {
      float x, y, z;
      float w, h, d;
      sscanf(lineExploded[1].c_str(), "%f|%f|%f", &x, &y, &z);
      sscanf(lineExploded[3].c_str(), "%f|%f|%f", &w, &h, &d);
      e = new AirTimeArea(
        lineExploded[4], Vector3(x,y,z), Vector3(w,h,d), atoi(lineExploded[9].c_str())
      );

      if(lineExploded[2] != "")
      {
        Vector3 rotation;
        sscanf(lineExploded[2].c_str(), "%f|%f|%f", &rotation.x, &rotation.y, &rotation.z);
        e->Rotate(rotation);
      }
    }
    else if(lineExploded[0] == "waypoint")
    {
      float x, y, z;
      sscanf(lineExploded[1].c_str(), "%f|%f|%f", &x, &y, &z);
      
      e = new Waypoint(Vector3(x, y, z), Vector3(12,10,12));
    }
    else if(lineExploded[0] == "water")
    {
      float x, y, z;
      sscanf(lineExploded[1].c_str(), "%f|%f|%f", &x, &y, &z);
      
      e = new WaterPlane(Vector3(x, y, z));
    }

    if(e != nullptr && lineExploded[0] != "water")
    {
      e->SetOccludability(atoi(lineExploded[8].c_str()));
      if(lineExploded[6] != "")
      {
        e->AssignTexture(lineExploded[6]);
      }
      e = nullptr;
    }
    
  }

  file.close();
}