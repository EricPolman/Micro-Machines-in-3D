#pragma once
#include <vector>

namespace GameLayer
{
  class Manager
  {
  public:
    Manager(void);
    virtual ~Manager(void);

    virtual void Update(float a_fDeltaTime) = 0;
  };
};
