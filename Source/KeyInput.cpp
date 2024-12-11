#include "KeyInput.h"
#include <Windows.h>

namespace Input
{
	void KeyInput::Update()
	{
		for (short key = 0; key < 256; ++key)
		{
			short keyState = GetAsyncKeyState(key);
			bool isPressed = (keyState & 0x8000) != 0;

			if (isPressed)
			{
				keyField[key] = KeyStatus::Press;
			}
			else
			{
				if (keyField[key] == KeyStatus::Press)
				{
					keyField[key] = KeyStatus::Release;
				}
				else if(keyField[key] == KeyStatus::Release)
				{
					keyField[key] = KeyStatus::Free;
				}
			}
		}
	}

}
