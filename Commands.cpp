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
	Notify(gameObject, Event::PlayerDied, GetPlayerIndex());
}


ColorChange::ColorChange(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void ColorChange::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::ColorChange, GetPlayerIndex());
}

DefeatCoily::DefeatCoily(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void DefeatCoily::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::DefeatCoily, GetPlayerIndex());
}

CatchSAndS::CatchSAndS(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void CatchSAndS::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::CatchSAndS, GetPlayerIndex());
}

LevelFinish::LevelFinish(unsigned int playerIndex)
	:Command{ playerIndex }
{
}

void LevelFinish::Execute(GameObject* gameObject)
{
	Notify(gameObject, Event::LevelFinish, GetPlayerIndex());
	for (size_t i = 0; i < 1; i++)
	{
		auto& ss = AudioServiceLocator::GetSoundSystem();
		ss.Play("../Data/Sounds/door1.wav", 100.f);
	}
}
