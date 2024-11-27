#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/RayCastScene.h"


// �R���X�g���N�^
RayCastScene::RayCastScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// �J�����ݒ�
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// ��p
		screenWidth / screenHeight,			// ��ʃA�X�y�N�g��
		0.1f,								// �j�A�N���b�v
		1000.0f								// �t�@�[�N���b�v
	);
	camera.SetLookAt(
		{ 0, 5, -20 },		// ���_
		{ 0, 5, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);

	// ���f��
	model = std::make_unique<Model>("Data/Model/Cube/cube.003.1.mdl");

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0.8f, 0.0f);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(-1, 0, -1);
	DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);

	stage = std::make_unique<stage>(0);
}

// �X�V����
void RayCastScene::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);

	stage.get()->Update(elapsedTime);
}

// �`�揈��
void RayCastScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();


	// �����_�[�X�e�[�g�ݒ�
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullBack));

#if _DEBUG
	// �O���b�h�`��
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
#endif

	// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	stage.get()->Render(elapsedTime,&rc);
}

// GUI�`�揈��
void RayCastScene::DrawGUI()
{
	stage.get()->DrawGUI();
}

// ���C�L���X�g
bool RayCastScene::RayCast(				// ����������true��Ԃ�
	const DirectX::XMFLOAT3& start,		// ���C�̎n�_
	const DirectX::XMFLOAT3& end,		// ���C�̏I�_
	const DirectX::XMFLOAT3& vertexA,	// �O�p�`�̈�ڂ̒��_���W
	const DirectX::XMFLOAT3& vertexB,	// �O�p�`�̓�ڂ̒��_���W
	const DirectX::XMFLOAT3& vertexC,	// �O�p�`�̎O�ڂ̒��_���W
	DirectX::XMFLOAT3& hitPosition,		// ��_�̊i�[��
	DirectX::XMFLOAT3& hitNormal)		// �@���̊i�[��
{
	return false;
}
