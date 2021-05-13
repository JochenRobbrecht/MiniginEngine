#pragma once
#include "Singleton.h"

class Time final: public Singleton<Time>
{
public:
	Time();
	
	void SetElapsedSec(float elapsedSec);
	float GetElapsedSec()const;
private:
	float m_ElapsedSec;
};

