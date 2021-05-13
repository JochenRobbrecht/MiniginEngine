#include "MiniginPCH.h"
#include "Time.h"

Time::Time()
	:m_ElapsedSec{}
{
}

void Time::SetElapsedSec(float elapsedSec)
{
	m_ElapsedSec = elapsedSec;
}

float Time::GetElapsedSec() const
{
	return m_ElapsedSec;
}
