#include "MiniginPCH.h"
#include "Observe.h"
#include "GameObject.h"
#include <algorithm>

void Observable::AddObserver(Observer* observer)
{
	m_pObservers.push_back(observer);
}

void Observable::RemoveObserver(Observer* observer)
{
	auto it = std::remove(m_pObservers.begin(), m_pObservers.end(), observer);
	if (it != m_pObservers.end())m_pObservers.erase(it);
}

void Observable::Notify(GameObject* gameObject, Event event, unsigned int playerIndex)
{
	for (Observer* observer : m_pObservers)
	{
		observer->OnNotify(gameObject, event, playerIndex);
	}
}
