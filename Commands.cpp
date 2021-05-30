#include "MiniginPCH.h"
#include "Commands.h"
#include <iostream>
#include "GameObject.h"
#include "SoundSystem.h"

Command::Command(unsigned int playerIndex)
	:m_PlayerIndex{playerIndex}
{
}

int unsigned Command::GetPlayerIndex()
{
	return m_PlayerIndex;
}

///
///COMMANDS
///

KillCommand::KillCommand(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void KillCommand::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::PlayerDied, m_PlayerIndex);
}


ColorChange::ColorChange(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void ColorChange::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::ColorChange, m_PlayerIndex);
}

DefeatCoily::DefeatCoily(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void DefeatCoily::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::DefeatCoily, m_PlayerIndex);
}

CatchSAndS::CatchSAndS(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void CatchSAndS::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::CatchSAndS, m_PlayerIndex);
}

LevelFinish::LevelFinish(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void LevelFinish::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::LevelFinish, m_PlayerIndex);
	for (size_t i = 0; i < 1; i++)
	{
		auto& ss = AudioServiceLocator::GetSoundSystem();
		ss.Play("../Data/Sounds/door1.wav", 100.f);
	}
}

UpCommand::UpCommand(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void UpCommand::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::MoveUp, m_PlayerIndex);
}

DownCommand::DownCommand(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void DownCommand::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::MoveDown, m_PlayerIndex);
}

LeftCommand::LeftCommand(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void LeftCommand::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::MoveLeft, m_PlayerIndex);
}

RightCommand::RightCommand(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void RightCommand::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::MoveRight, m_PlayerIndex);
}
