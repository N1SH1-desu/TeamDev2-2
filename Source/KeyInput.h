#pragma once

#include <unordered_map>

namespace Input
{
	enum KeyStatus
	{
		Free,
		Press,
		Release
	};

	class KeyInput
	{
	public:
		KeyInput() = default;

		void Update();

	    KeyStatus GetKeyStatus(short key) const  { return keyField.at(key); }

	private:
		std::unordered_map<short, KeyStatus> keyField;
	};
}
