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
protected:
	unsigned int m_PlayerIndex;
};

class UpCommand final : public Command
{
public:
	UpCommand(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class DownCommand final : public Command
{
public:
	DownCommand(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class LeftCommand final : public Command
{
public:
	LeftCommand(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
};

class RightCommand final : public Command
{
public:
	RightCommand(unsigned int playerIndex);
	virtual void Execute(GameObject* gameObject) override;
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