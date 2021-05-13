#include "MiniginPCH.h"
#include "InputManager.h"	
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#include "Commands.h"
#include <algorithm>
#include <iostream>
#include <memory>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	/*for (std::pair<ControllerButton, Command*> p : m_ControllerCommands)
	{
		delete p.second;
	}
	for (std::pair<SDL_Event, Command*> p : m_SDLCommands)
	{
		delete p.second;
	}*/
}

void InputManager::ProcessInput(bool& doContinue)
{
	std::vector<std::shared_ptr<Command>> commandsVec;

	//SDL

	//build playerindex in keyboardevent arg//wrapper?
	//fix deletion of comps if observers!
	//fix inputcomponents all executing commands that dont have any observers anymore(delete observer if weak ptr no good)

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			doContinue = false;
		}
		for (std::pair<SDL, std::shared_ptr<Command>>& p : m_SDLCommands)
		{
			if (p.second->GetPlayerIndex() == p.first.playerIndex && e.type == p.first.event.type)
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == p.first.event.key.keysym.sym)
						commandsVec.push_back(p.second);
					break;
				case SDL_KEYUP:
					if (e.key.keysym.sym == p.first.event.key.keysym.sym)
						commandsVec.push_back(p.second);
					break;
				case SDL_MOUSEMOTION:

					break;
				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == p.first.event.button.button)
						commandsVec.push_back(p.second);
					break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == p.first.event.button.button)
						commandsVec.push_back(p.second);
					break;
				case SDL_MOUSEWHEEL:

					break;
				}
			}
		}
	}


	//XINPUT

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	unsigned int maxPlayers{ 4 };
	for (unsigned int playerIndex = 0; playerIndex < maxPlayers; playerIndex++)
	{
		auto a = XInputGetState(playerIndex, &state);
		if (a != ERROR_SUCCESS)continue;

		auto buttons = state.Gamepad.wButtons;

		for (std::pair<ControllerButton, std::shared_ptr<Command>>& p : m_ControllerCommands)
		{
			if (p.second->GetPlayerIndex() == playerIndex)
			{
				if (int(p.first.buttonValue) == buttons)
				{
					if (p.first.pressedLastFrame && p.first.buttonType == ButtonType::IsPressed)
						commandsVec.push_back(p.second);

					else if (!p.first.pressedLastFrame && p.first.buttonType == ButtonType::ButtonDown)
						commandsVec.push_back(p.second);

					p.first.pressedLastFrame = true;
				}
				else
				{
					if (p.first.pressedLastFrame && p.first.buttonType == ButtonType::ButtonUp)
						commandsVec.push_back(p.second);
					p.first.pressedLastFrame = false;
				}
			}
		}
	}
	m_CommandsToFire = commandsVec;
}

void InputManager::AddCommand( ControllerButton button, std::shared_ptr<Command> command)
{
	auto it = std::find_if(m_ControllerCommands.begin(), m_ControllerCommands.end(), [button, command](const std::pair<ControllerButton, std::shared_ptr<Command>>& p)
		{
			//no command should be set on 2 buttons for the same controller
			if (p.second->GetPlayerIndex() == command->GetPlayerIndex() && command == p.second)return true;
			//no button should execute 2 commands
			else if (p.second->GetPlayerIndex() == command->GetPlayerIndex() && button.buttonValue == p.first.buttonValue)return true;
			return false;
		});
	if (it != m_ControllerCommands.end())return;
	m_ControllerCommands.push_back(std::pair<ControllerButton, std::shared_ptr<Command>>{button, command});
}

void InputManager::AddCommand(SDL event, std::shared_ptr<Command> command)
{
	m_SDLCommands.push_back(std::pair<SDL, std::shared_ptr<Command>>{event, command});
}

std::vector<std::shared_ptr<Command>> InputManager::GetCommandsToFire()
{
	return m_CommandsToFire;
}

