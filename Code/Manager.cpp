#include "Manager.h"
#include "Core.h"

using namespace GameLayer;

Manager::Manager(void)
{
  Core::GetInstance()->Register(this);
}


Manager::~Manager(void)
{
  Core::GetInstance()->Unregister(this);
}
