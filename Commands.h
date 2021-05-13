#pragma once
#include "Observe.h"

class GameObject;

class Command : public Observable
{
public:
	Command(unsigned int playerIndex);
	virtual ~Command() = default;
	virtual void Execute(GameObject* gameObject) = 0;
	virtual unsigned int GetPlayerIndex();
private:
	unsigned int m_PlayerIndex;
};

class KillCommand final : public Command
{
public:
	KillCommand(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class ColorChange final : public Command
{
public:
	ColorChange(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class DefeatCoily final : public Command
{
public:
	DefeatCoily(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class CatchSAndS final : public Command
{
public:
	CatchSAndS(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class LevelFinish final : public Command
{
public:
	LevelFinish(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};