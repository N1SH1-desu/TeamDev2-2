#include "EndlessGridRenderer.h"
#include "GpuResourceUtils.h"
#include "Misc.h"

EndlessGridRenderer::EndlessGridRenderer(ID3D11Device* device)
{
	HRESULT hr{};

	hr = GpuResourceUtils::LoadVertexShader(
		device,
		"Data/Shader/GridVS.cso",
		nullptr,
		0,
		nullptr,
		&vertexShader
	);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	hr = GpuResourceUtils::LoadPixelShader(
		device,
		"Data/Shader/GridPS.cso",
		&pixelShader
	);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufDesc.MiscFlags = 0u;
	bufDesc.StructureByteStride = 0u;
	bufDesc.ByteWidth = sizeof(EndlessGridRenderer::cbFrame);

	hr = device->CreateBuffer(&bufDesc, nullptr, &cbPerFrame);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void EndlessGridRenderer::Draw(ID3D11DeviceContext* dc, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	HRESULT hr{};

	DirectX::XMMATRIX ViewProj = DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&proj);

	cbFrame frame{};
	DirectX::XMStoreFloat4x4(&frame.viewProjMat, ViewProj);
	DirectX::XMStoreFloat4x4(&frame.invViewProjMat, DirectX::XMMatrixInverse(nullptr, ViewProj));

	D3D11_MAPPED_SUBRESOURCE mapped{};
	hr = dc->Map(cbPerFrame.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	memcpy(mapped.pData, &frame, sizeof(frame));
	dc->Unmap(cbPerFrame.Get(), 0);

	dc->VSSetConstantBuffers(0u, 1u, cbPerFrame.GetAddressOf());

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->VSSetShader(vertexShader.Get(), nullptr, 0u);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0u);
	dc->IASetInputLayout(nullptr);

	dc->Draw(6, 0);
}
