#include <imgui.h>
#include <algorithm>
#include "Graphics.h"
#include "Misc.h"
#include "GpuResourceUtils.h"
#include "Scene/UIAnimScene.h"

// �R���X�g���N�^
UIAnimScene::UIAnimScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr;

	// ���_�o�b�t�@
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(Vertex) * 24;
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&buffer_desc, nullptr, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���_�V�F�[�_�[
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = GpuResourceUtils::LoadVertexShader(
			device,
			"Data/Shader/SpriteVS.cso",
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			inputLayout.GetAddressOf(),
			vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}

	// �s�N�Z���V�F�[�_�[
	{
		hr = GpuResourceUtils::LoadPixelShader(
			device,
			"Data/Shader/SpritePS.cso",
			pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �e�N�X�`��
	{
		hr = GpuResourceUtils::LoadTexture(device, "Data/Sprite/jammo.png", shaderResourceView.GetAddressOf(), &textureDesc);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// �X�V����
void UIAnimScene::Update(float elapsedTime)
{
	auto updateAmount = [&](float& amount)
	{
		amount += 0.5f * elapsedTime;
		if (amount > 1.0f)
		{
			amount = 0.0f;
		}
	};
	updateAmount(easingAmount);
	updateAmount(horizontalFillAmount);
	updateAmount(verticalFillAmount);
	updateAmount(radialFillAmount);
}

// �`�揈��
void UIAnimScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();

	// �V�F�[�_�[�ݒ�
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);

	// �����_�[�X�e�[�g�ݒ�
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullNone));

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	dc->IASetInputLayout(inputLayout.Get());

	// ��ʃT�C�Y���e�N�X�`���T�C�Y�擾
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();
	float textureWidth = static_cast<float>(textureDesc.Width);
	float textureHeight = static_cast<float>(textureDesc.Height);

	// �`��
	DrawEasing(dc, 600, 50, 300, 300, 0, 0, textureWidth, textureHeight, easingAmount, screenWidth, screenHeight);
	DrawFillHorizontal(dc, 100, 400, 300, 300, 0, 0, textureWidth, textureHeight, horizontalFillAmount, screenWidth, screenHeight);
	DrawFillVertical(dc, 410, 400, 300, 300, 0, 0, textureWidth, textureHeight, verticalFillAmount, screenWidth, screenHeight);
	DrawFillRadial(dc, 720, 400, 300, 300, 0, 0, textureWidth, textureHeight, radialFillAmount, screenWidth, screenHeight);
}

// GUI�`�揈��
void UIAnimScene::DrawGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();

	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 180), ImGuiCond_Once);

	if (ImGui::Begin(u8"UI", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		ImGui::SliderFloat("EasingAmount", &easingAmount, 0.0f, 1.0f);
		ImGui::SliderFloat("HorizontalFillAmount", &horizontalFillAmount, 0.0f, 1.0f);
		ImGui::SliderFloat("VerticalFillAmount", &verticalFillAmount, 0.0f, 1.0f);
		ImGui::SliderFloat("RadialFillAmount", &radialFillAmount, 0.0f, 1.0f);
	}
	ImGui::End();
}

// �X�v���C�g�`��
void UIAnimScene::DrawSprite(
	ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float screenWidth,
	float screenHeight) const
{
}

// �����h��Ԃ��`��
void UIAnimScene::DrawFillHorizontal(
	ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh,	// �\�����W���T�C�Y
	float sx, float sy, float sw, float sh,	// �e�N�X�`�����W���T�C�Y
	float amount,							// �`�旦(0.0�`1.0)
	float screenWidth,
	float screenHeight) const
{
}

// �����h��Ԃ��`��
void UIAnimScene::DrawFillVertical(
	ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float amount,
	float screenWidth,
	float screenHeight) const
{
}

// �C�[�W���O�`��
void UIAnimScene::DrawEasing(
	ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float amount,
	float screenWidth,
	float screenHeight) const
{
}

// ���˓h��Ԃ��`��
void UIAnimScene::DrawFillRadial(
	ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float amount,
	float screenWidth,
	float screenHeight) const
{
	//  +----+----+
	//  |0   |1   |2
	//  |    |    |
	//  +----+----+
	//  |3   |4   |5
	//  |    |    |
	//  +----+----+
	//   6    7    8
	Vertex p[9];
	p[0].position.x = p[3].position.x = p[6].position.x = dx;
	p[1].position.x = p[4].position.x = p[7].position.x = dx + dw * 0.5f;
	p[2].position.x = p[5].position.x = p[8].position.x = dx + dw;

	p[0].position.y = p[1].position.y = p[2].position.y = dy;
	p[3].position.y = p[4].position.y = p[5].position.y = dy + dh * 0.5f;
	p[6].position.y = p[7].position.y = p[8].position.y = dy + dh;

	p[0].texcoord.x = p[3].texcoord.x = p[6].texcoord.x = sx;
	p[1].texcoord.x = p[4].texcoord.x = p[7].texcoord.x = sx + sw * 0.5f;
	p[2].texcoord.x = p[5].texcoord.x = p[8].texcoord.x = sx + sw;

	p[0].texcoord.y = p[1].texcoord.y = p[2].texcoord.y = sy;
	p[3].texcoord.y = p[4].texcoord.y = p[5].texcoord.y = sy + sh * 0.5f;
	p[6].texcoord.y = p[7].texcoord.y = p[8].texcoord.y = sy + sh;

	// ���_�ҏW�J�n
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	Vertex* v = reinterpret_cast<Vertex*>(mappedSubresource.pData);

	// �i�s�x(amount)�ɉ����ăX�v���C�g���~��`���悤�ɕ\�������悤�ɂ���
	{
		// +----+----+
		// |�_7 | 0�^|
		// | 6�_|�^1 |
		// +----+----+
		// | 5�^|�_2 |
		// |�^4 | 3�_|
		// +----+----+
	}

	// NDC���W�ϊ���UV���W�ϊ�
	for (int i = 0; i < 24; ++i)
	{
		v[i].color = { 1, 1, 1, 1 };
		v[i].position.x = 2.0f * v[i].position.x / screenWidth - 1.0f;
		v[i].position.y = 1.0f - 2.0f * v[i].position.y / screenHeight;
		v[i].position.z = 0.0f;
		v[i].texcoord.x = v[i].texcoord.x / textureDesc.Width;
		v[i].texcoord.y = v[i].texcoord.y / textureDesc.Height;
	}

	// ���_�ҏW�I��
	dc->Unmap(vertexBuffer.Get(), 0);

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	dc->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

	// �v���~�e�B�u�g�|���W�[�ݒ�
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �`��
	dc->Draw(24, 0);

}
