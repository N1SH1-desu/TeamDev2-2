#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/ProjectScreenScene.h"
#include "ControlTetrisBlock.h"

// �R���X�g���N�^
ProjectScreenScene::ProjectScreenScene()
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
	camera.SetOrthoGraphic(
		128.0,
		72.0,
		0.1f,
		1000.0f
	);
	camera.SetLookAt(
		{ 0, 0, -20 },		// ���_
		{ 0, 0, 0 },		// �����_
		{ 0, 1, 0 }			// ��x�N�g��
	);
	cameraController.SyncCameraToController(camera);

	sprite = std::make_unique<Sprite>(device);
	stage.model = std::make_unique<Model>("Data/Model/Stage/ExampleStage.mdl");

	sceneModels = std::make_unique<SceneModel>("Data/Model/TetrisBlock/Colors.mdl");

	stage.scale = { 8.0f, 8.0f, 8.0f };
	stage.position = { 0.0f, 0.0f, 0.0f };
	stage.angle = { 0.0f, 0.0f, 0.0f };
}

// �X�V����
void ProjectScreenScene::Update(float elapsedTime)
{
	// �J�����X�V����
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);

	// �}�E�X���N���b�N����
	if (GetAsyncKeyState(VK_LBUTTON) & 0x01)
	{
		// �X�N���[���T�C�Y�擾
		float screenWidth = Graphics::Instance().GetScreenWidth();
		float screenHeight = Graphics::Instance().GetScreenHeight();

		// �}�E�X�J�[�\���ʒu�̎擾
		POINT cursor;
		::GetCursorPos(&cursor);
		::ScreenToClient(Graphics::Instance().GetWindowHandle(), &cursor);

	}

	keyInput.Update();

	{
		if (keyInput.GetKeyStatus('G') == Input::Release)
		{
			EditerMode = !EditerMode;
		}

		{
			Grid2DRenderer* g2R = Graphics::Instance().GetGridRenderer();
			g2R->Update(elapsedTime, EditerMode);
		}

		if (EditerMode)
		{
			{
				POINTS mPoints = refInputMouse->GetPosition();

				static int rotate = 0;
				static int x = 0;
				static int y = 0;

				int xGrid = mPoints.x / static_cast<SHORT>(Grid2DRenderer::grid_size);
				int yGrid = mPoints.y / static_cast<SHORT>(Grid2DRenderer::grid_size);

				if (tetroCollision.DetectionCollide(static_cast<Tetromino::TetrominoType>(tetroType), yGrid, xGrid, rotate))
				{
					x = xGrid;
					y = yGrid;
				}

				tetroRenderer.CalcWorldPosition(static_cast<Tetromino::TetrominoType>(tetroType), y, x, rotate);
				tetroRenderer.UpdateTransform(stage.scale);

				if (keyInput.GetKeyStatus('F') == Input::Release)
				{
					rotate++;
					if (rotate > 3)
					{
						rotate = 0;
					}
				}

				if (keyInput.GetKeyStatus(VK_SPACE) == Input::Release)
				{
					if (tetroCollision.PlaceTetromino(static_cast<Tetromino::TetrominoType>(tetroType), y, x, rotate))
					{
						auto transforms = tetroRenderer.GetTransforms();
						for (DirectX::XMFLOAT4X4 ts : transforms)
						{
							sceneModels->CommitBlock({ 0u, std::move(ts) });
						}
					}
				}
			}
		}
	}



	// �X�e�[�W�s��X�V����
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(stage.scale.x, stage.scale.y, stage.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(stage.angle.x, stage.angle.y, stage.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(stage.position.x, stage.position.y, stage.position.z);
		DirectX::XMStoreFloat4x4(&stage.transform, S * R * T);
	}

	// �I�u�W�F�N�g�s��X�V����
	for (Object& obj : objs)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(obj.scale.x, obj.scale.y, obj.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(obj.angle.x, obj.angle.y, obj.angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(obj.position.x, obj.position.y, obj.position.z);
		DirectX::XMStoreFloat4x4(&obj.transform, S * R * T);
	}
}

// �`�揈��
void ProjectScreenScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();
	EndlessGridRenderer* gridRenderer = Graphics::Instance().GetEndlessGridRenderer();
	ID2D1DeviceContext* d2dContext = Graphics::Instance().GetGfx2D()->GetContext();
	Grid2DRenderer* grid2DRenderer = Graphics::Instance().GetGridRenderer();

	// ���f���`��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	//modelRenderer->Render(rc, stage.transform, stage.model.get(), ShaderId::Lambert);

	// �X�N���[���T�C�Y�擾
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	for (const Object& obj : objs)
	{
		//modelRenderer->Render(rc, obj.transform, obj.model.get(), ShaderId::Lambert);
	}

	if (EditerMode)
	{
		{
			for (const DirectX::XMFLOAT4X4& tf : tetroRenderer.GetTransforms())
			{
				sceneModels->SelectedBlockRender(rc, modelRenderer, tf, 0u, ShaderId::Lambert, true);
			}
		}
	}

	sceneModels->RenderCommitedBlocks(rc, modelRenderer, ShaderId::Lambert, true);
	grid2DRenderer->Draw(d2dContext);
}

// GUI�`�揈��
void ProjectScreenScene::DrawGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();

	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 180), ImGuiCond_Once);

	if (ImGui::Begin(u8"�X�N���[�����W�ϊ�", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		ImGui::Text(u8"�N���b�N�F�L�����z�u");

		POINTS pos = refInputMouse->GetPosition();
		int v[2] = { pos.x, pos.y };
		ImGui::InputInt2("Mouse Position", v);

		ImGui::SliderFloat3("Block Position", &stage.position.x, -200.0f, 200.0f);
		ImGui::InputFloat3("Block Pos Input", &stage.position.x);
		ImGui::SliderFloat3("Block Scale", &stage.scale.x, 0.01f, 10.0f);
		ImGui::SliderFloat3("Block Rotate", &stage.angle.x, 0.0f, 20.0f);
	}
	ImGui::End();
}
