#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

class EndlessGridRenderer
{
public:
	struct cbFrame
	{
		DirectX::XMFLOAT4X4 viewProjMat;
		DirectX::XMFLOAT4X4 invViewProjMat;
	};
public:
	EndlessGridRenderer(ID3D11Device* device);

	void Draw(ID3D11DeviceContext* dc, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cbPerFrame;
};
