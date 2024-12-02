#pragma once
#include <Windows.h>
#include <queue>
#include <optional>

struct MouseCommand
{
	enum CommandName
	{
		M_MOVE,
		M_LBUTTON_DOWN,
		M_LBUTTON_UP,
		M_RBUTTON_DOWN,
		M_RBUTTON_UP
	};

	MouseCommand(CommandName commandName, POINTS p = {})
	{
		name = commandName;
		position = p;
	}

	CommandName name;
	std::optional<POINTS> position;
};

class InputMouse
{
public:
	InputMouse(InputMouse&) = delete;
	InputMouse(InputMouse&&) = delete;
	InputMouse& operator=(InputMouse&) = delete;
	InputMouse& operator=(InputMouse&&) = delete;

public:
	InputMouse(HWND hWnd);

	void InQueueCommand(MouseCommand&& mouseCmd) { commandList.push(std::move(mouseCmd)); }

	void ProcessCommand();

	POINTS GetPosition() const { return mousePosition; }

	bool IsLBottonDowned() const { return isLBottonDown; }
	bool IsRBottonDowned() const { return isRBottonDown; }

private:
	HWND hWnd;
	POINTS mousePosition;

	bool isLBottonDown = false;
	bool isRBottonDown = false;

	std::queue<MouseCommand> commandList;
};
 