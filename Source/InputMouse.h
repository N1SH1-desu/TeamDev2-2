#pragma once
#include <Windows.h>
#include <array>
#include <optional>

struct MouseCommand
{
	enum Command
	{
		M_MOVE,
		M_LBUTTON_DOWN,
		M_LBUTTON_UP,
		M_RBUTTON_DOWN,
		M_RBUTTON_UP
	};

	Command command;
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
	InputMouse();

	void MouseMove(short x, short y);
	void MouseDownLeft();
	void MouseDownRight();
	void MouseUpLeft();
	void MouseUpRight();

	bool IsOnLeft() const { return isOnLeft; }
	bool IsOnRight() const { return isOnRight; }

private:
	POINTS mousePosition;

	bool isOnLeft = false;
	bool isOnRight = false;

	
};
 