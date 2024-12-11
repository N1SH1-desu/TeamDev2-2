#include "EndlessGridRenderer.h"
#include "GpuResourceUtils.h"
#include "Misc.h"
#include "imgui.h"

EndlessGridRenderer::EndlessGridRenderer(ID3D11Device* device)
	:
	data()
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
	bufDesc.ByteWidth = sizeof(EndlessGridRenderer::cbFrameData);

	hr = device->CreateBuffer(&bufDesc, nullptr, &cbPerFrame);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void EndlessGridRenderer::DrawGUI()
{
	if(ImGui::Begin("Grid Property"))
	{
		//ImGui::SliderFloat("GridCellSize", &data.gridCellSize, 0.001f, 0.5f);
	}
	ImGui::End();
}

void EndlessGridRenderer::Draw(ID3D11DeviceContext* dc, DirectX::XMFLOAT4X4 viewProj)
{
	HRESULT hr{};

	DrawGUI();

	data.viewProjMat = viewProj;
	DirectX::XMStoreFloat4x4(&data.inverseViewProj, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&viewProj)));

	D3D11_MAPPED_SUBRESOURCE mapped{};
	hr = dc->Map(cbPerFrame.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	memcpy(mapped.pData, &data, sizeof(data));
	dc->Unmap(cbPerFrame.Get(), 0);

	dc->VSSetConstantBuffers(0u, 1u, cbPerFrame.GetAddressOf());
	//dc->PSSetConstantBuffers(0u, 1u, cbPerFrame.GetAddressOf());

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->VSSetShader(vertexShader.Get(), nullptr, 0u);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0u);
	dc->IASetInputLayout(nullptr);

	dc->Draw(6, 0);
}
