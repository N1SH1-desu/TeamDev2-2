#include "InputMouse.h"
#include "Graphics.h"

InputMouse::InputMouse(HWND hWnd) :
	hWnd(hWnd),
	mousePosition(),
	isLBottonDown(),
	isRBottonDown()
{
}

void InputMouse::ProcessCommand()
{
	for (size_t i = 0u; i < commandList.size(); i++)
	{
		MouseCommand& command = commandList.front();
		switch (command.name)
		{
		case MouseCommand::CommandName::M_MOVE:
			mousePosition = *command.position;
			break;
		case MouseCommand::CommandName::M_LBUTTON_DOWN:
			isLBottonDown = true;
			break;
		case MouseCommand::CommandName::M_LBUTTON_UP:
			isLBottonDown = false;
			break;
		case MouseCommand::CommandName::M_RBUTTON_DOWN:
			isRBottonDown = true;
			break;
		case MouseCommand::CommandName::M_RBUTTON_UP:
			isRBottonDown = false;
			break;
		}

		commandList.pop();
	}
}
