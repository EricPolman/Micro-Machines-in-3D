#pragma once
#include <memory>

template <class Type>
class Singleton
{
public:
  virtual ~Singleton(void){}
  static Type* GetInstance(void)
  {
    if(single.get() == nullptr)
    {
      single = std::unique_ptr<Type>(new Type());
      return single.get();
    }
    else
    {
      return single.get();
    }
  }

  static void Reset(){single = nullptr;}
protected:
  static std::unique_ptr<Type> single;
};

template <class Type>
std::unique_ptr<Type> Singleton<Type>::single = nullptr;