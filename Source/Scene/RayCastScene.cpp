#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/RayCastScene.h"
#include"pause.h"
#include"clear.h"


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
		{ 0, 0, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);

	// ���f��
	model = std::make_unique<Model>("Data/Model/Cube/cube.003.1.mdl");

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0.8f, 0.0f);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(-1, 0, -1);
	DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);

	Stage::Instance().SelectStage(5);
	SpaceDivisionRayCast::Instance().Load(Stage::Instance().GetModel());

	//timer_ = std::make_unique<number_namager>();
	//timer_->SetTimer(60);
}

// �X�V����
void RayCastScene::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);
	static bool pause = false;
	if (GetAsyncKeyState('P') & 0x01)
	{
		pause = !pause;
		Pause::Instance().SetPause(pause);
	}
	
	if(!Pause::Instance().GetPause())
	{
	Stage* stage = &Stage::Instance();

	stage->Update(elapsedTime);


	static Model* cur_model = stage->GetModel();
	if (cur_model != stage->GetModel())
	{

		SpaceDivisionRayCast::Instance().Reload(stage->GetModel());

		cur_model = nullptr;
		cur_model = stage->GetModel();
	}
	//timer_->UpdateTimer(elapsedTime);
	}

	Pause::Instance().Update(elapsedTime,refInputMouse);
	Clear::Instance().Update(elapsedTime,refInputMouse);
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
	//�����蔻��̊m�F�p�@������
	{
		DirectX::XMFLOAT3 s;
		DirectX::XMFLOAT3 e;
		float size_x = 10.f;
		float size_z = 2.f;
		float add_size = 1.f;
		for (float x = -size_x; x < size_x; x+=add_size)
		{
			for (float z = -size_z; z < size_z; z+=add_size)
			{
				
				s = { x,5.f,0.f+z };
				e = { x,-5.f,0.f+z };

				DirectX::XMFLOAT3 hitPosition, hitNormal;

				//if (Collision::RayCast(s, e, Stage::Instance().GetTransform(),Stage::Instance().GetModel(), hitPosition, hitNormal))
				if ( SpaceDivisionRayCast::Instance().RayCast(s, e, hitPosition, hitNormal))
				{
					// ���������ʒu�Ɩ@����\��
					shapeRenderer->DrawSphere(hitPosition, 0.2f, { 1, 0, 0, 1 });
					DirectX::XMFLOAT3 p = hitPosition;
					p.x += hitNormal.x * 1.0f;
					p.y += hitNormal.y * 1.0f;
					p.z += hitNormal.z * 1.0f;
					primitiveRenderer->AddVertex(hitPosition, { 1, 0, 0, 1 });
					primitiveRenderer->AddVertex(p, { 1, 0, 0, 1 });
				}
				{	// ���C�`��
					primitiveRenderer->AddVertex(s, { 0, 1, 1, 1 });
					primitiveRenderer->AddVertex(e, { 0, 1, 1, 1 });
				}
			}
		}

		//ray�`��
		primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			// �V�F�C�v�`��
		shapeRenderer->Render(dc, camera.GetView(), camera.GetProjection());
	}

	// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	Stage::Instance().Render(elapsedTime,rc);
	SpaceDivisionRayCast::Instance().DebugDraw(rc);

	Pause::Instance().Render(elapsedTime);
	
	Clear::Instance().Render(elapsedTime);

	//timer_->DrawTimer({0,0},{1280,720});
	//timer_->DrawNumber(17,{640,310},{128,72});
}

// GUI�`�揈��
void RayCastScene::DrawGUI()
{
	Stage::Instance().DrawGUI();
	SpaceDivisionRayCast::Instance().DrowImgui();
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
