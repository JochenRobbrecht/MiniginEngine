#pragma once

class GameObject; 

enum class Event
{
	PlayerDied,
	ColorChange,
	DefeatCoily,
	LevelFinish,
	CatchSAndS
};

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void OnNotify(GameObject* gameObject, Event event, unsigned int playerIndex) = 0;
};

class Observable
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

protected:
	void Notify(GameObject* gameObject, Event event, unsigned int playerIndex);
private:
	std::vector <Observer*> m_pObservers;
};

