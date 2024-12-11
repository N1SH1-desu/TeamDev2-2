#pragma once
#include <DirectXMath.h>

class function {
public:

	static function& getInstance()
	{
		static function instance;
		return instance;
	}

	const DirectX::XMFLOAT2 Get_Cursor_Pos();

	bool Click_Colision(
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size
	);

	bool Box_Colision(
		DirectX::XMFLOAT2 posA,
		DirectX::XMFLOAT2 sizeA,
		DirectX::XMFLOAT2 posB,
		DirectX::XMFLOAT2 sizeB
	);
};