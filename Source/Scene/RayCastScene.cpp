#include <imgui.h>
#include <ImGuizmo.h>
#include <DirectXCollision.h>
#include "Graphics.h"
#include "Collision.h"
#include "Scene/RayCastScene.h"


// コンストラクタ
RayCastScene::RayCastScene()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// カメラ設定
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// 画角
		screenWidth / screenHeight,			// 画面アスペクト比
		0.1f,								// ニアクリップ
		1000.0f								// ファークリップ
	);
	camera.SetLookAt(
		{ 0, 5, -20 },		// 視点
		{ 0, 0, 0 },		// 注視点
		{ 0, 1, 0 }			// 上ベクトル
	);
	cameraController.SyncCameraToController(camera);

	// モデル
	model = std::make_unique<Model>("Data/Model/Cube/cube.003.1.mdl");

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0.8f, 0.0f);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(-1, 0, -1);
	DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);

	stage = std::make_unique<Stage>();
	space_division_raycast = std::make_unique<SpaceDivisionRayCast>();
	space_division_raycast->Load(stage.get()->GetModel(), stage.get()->GetTransform());
}

// 更新処理
void RayCastScene::Update(float elapsedTime)
{
	// カメラ更新処理
	cameraController.Update();
	cameraController.SyncControllerToCamera(camera);

	stage.get()->Update(elapsedTime);

	static int cur_num = stage->GetNumber();
	if (cur_num != stage.get()->GetNumber())
	{
		space_division_raycast->Load(stage->GetModel(), stage->GetTransform());
		cur_num = stage->GetNumber();
	}
}

// 描画処理
void RayCastScene::Render(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	RenderState* renderState = Graphics::Instance().GetRenderState();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();
	ModelRenderer* modelRenderer = Graphics::Instance().GetModelRenderer();


	// レンダーステート設定
	dc->OMSetBlendState(renderState->GetBlendState(BlendState::Opaque), nullptr, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(renderState->GetRasterizerState(RasterizerState::SolidCullBack));

#if _DEBUG
	// グリッド描画
	primitiveRenderer->DrawGrid(20, 1);
	primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
#endif
	//当たり判定の確認用　消すな
	{
		DirectX::XMFLOAT3 s;
		DirectX::XMFLOAT3 e;
		float size_x = 1.f;
		float size_z = 1.f;
		float add_size = 2.f;
		for (float x = -size_x; x < size_x; x+=add_size)
		{
			for (float z = -size_z; z < size_z; z+=add_size)
			{

				s = { x,10.f,-4.f+z };
				e = { x,-10.f,4.f+z };

				DirectX::XMFLOAT3 hitPosition, hitNormal;

				if (Collision::RayCast(s, e, stage->GetTransform(),stage.get()->GetModel(), hitPosition, hitNormal))
				//if ( space_division_raycast->RayCast(s, e,  stage->GetCollisionModel(), hitPosition, hitNormal))
				{
					// 交差した位置と法線を表示
					shapeRenderer->DrawSphere(hitPosition, 0.2f, { 1, 0, 0, 1 });
					DirectX::XMFLOAT3 p = hitPosition;
					p.x += hitNormal.x * 1.0f;
					p.y += hitNormal.y * 1.0f;
					p.z += hitNormal.z * 1.0f;
					primitiveRenderer->AddVertex(hitPosition, { 1, 0, 0, 1 });
					primitiveRenderer->AddVertex(p, { 1, 0, 0, 1 });
				}
				{	// レイ描画
					primitiveRenderer->AddVertex(s, { 0, 1, 1, 1 });
					primitiveRenderer->AddVertex(e, { 0, 1, 1, 1 });
				}
			}
		}

		//ray描画
		primitiveRenderer->Render(dc, camera.GetView(), camera.GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			// シェイプ描画
		shapeRenderer->Render(dc, camera.GetView(), camera.GetProjection());
	}

	// モデル描画
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderState;
	rc.camera = &camera;
	stage.get()->Render(elapsedTime,rc);
	space_division_raycast->DebugDraw(rc,stage->GetModel());
}

// GUI描画処理
void RayCastScene::DrawGUI()
{
	stage.get()->DrawGUI();
	space_division_raycast->DrowImgui();
}

// レイキャスト
bool RayCastScene::RayCast(				// 交差したらtrueを返す
	const DirectX::XMFLOAT3& start,		// レイの始点
	const DirectX::XMFLOAT3& end,		// レイの終点
	const DirectX::XMFLOAT3& vertexA,	// 三角形の一つ目の頂点座標
	const DirectX::XMFLOAT3& vertexB,	// 三角形の二つ目の頂点座標
	const DirectX::XMFLOAT3& vertexC,	// 三角形の三つ目の頂点座標
	DirectX::XMFLOAT3& hitPosition,		// 交点の格納先
	DirectX::XMFLOAT3& hitNormal)		// 法線の格納先
{
	return false;
}
