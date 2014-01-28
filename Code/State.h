#pragma once

template <class entity_type>
class State
{
public:
	virtual ~State(){}

	virtual void Enter(entity_type*)=0;
	virtual void Update(entity_type* a_entity, float a_fDeltaTime)=0;
	virtual void Draw(entity_type* a_entity)=0;
	virtual void Exit(entity_type*)=0;
};
