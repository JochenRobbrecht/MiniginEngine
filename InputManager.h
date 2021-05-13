#pragma once
#include "Singleton.h" 
#include <vector>
#include <SDL.h>

class Command;

enum class ControllerButtonValue
{
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
	Up = 0x0001,
	Down = 0x0002,
	Left = 0x0004,
	Right = 0x0008,
	Start = 0x0010,
	Back = 0x0020,
	LTrigger = 0x0100,
	RTrigger = 0x0200,
	LStick = 0x0040,
	RStick = 0x0080,
};
enum class ButtonType
{
	ButtonDown,
	ButtonUp,
	IsPressed,
};
struct ControllerButton
{
	ButtonType buttonType;
	ControllerButtonValue buttonValue;
	bool pressedLastFrame = false;
	ControllerButton(ButtonType buttonType, ControllerButtonValue buttonValue)
		: buttonType{ buttonType }
		, buttonValue{ buttonValue }
		, pressedLastFrame{ false }
	{ }
};
struct SDL
{
	SDL_Event event;
	unsigned int playerIndex;
	SDL(SDL_EventType type, int32_t keyOrButton, unsigned int playerIndex)
		: event{}
		, playerIndex{ playerIndex }
	{
		event.type = type;
		event.key.keysym.sym = keyOrButton;
		event.button.button = UINT8(keyOrButton);
	}
};

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	~InputManager();

	void ProcessInput(bool& doContinue);

	void AddCommand(ControllerButton button, std::shared_ptr<Command> command);
	void AddCommand(SDL event, std::shared_ptr<Command> command);

	std::vector<std::shared_ptr<Command>> GetCommandsToFire();
private:
	std::vector<std::shared_ptr<Command>> m_CommandsToFire;
	//XINPUT
	std::vector<std::pair<ControllerButton, std::shared_ptr<Command>>> m_ControllerCommands;
	//SDL
	std::vector<std::pair<SDL, std::shared_ptr<Command>>> m_SDLCommands;

};