#include "InputMouse.h"
#include "Graphics.h"

InputMouse::InputMouse() :
	mousePosition(),
	isOnLeft(),
	isOnRight()
{
}

void InputMouse::MouseMove(short x, short y)
{
	mousePosition.x = x;
	mousePosition.y = y;
}

void InputMouse::MouseDownLeft()
{
	isOnLeft = true;
}

void InputMouse::MouseDownRight()
{
	isOnRight = true;
}

void InputMouse::MouseUpLeft()
{
	isOnLeft = false;
}

void InputMouse::MouseUpRight()
{
	isOnRight = false;
}
