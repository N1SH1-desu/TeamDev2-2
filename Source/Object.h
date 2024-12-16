#pragma once
#include "Graphics.h"
#include "ModelRenderer.h"
#include <memory>

class Object
{
public:
	Object(const char* filepath = nullptr, DirectX::XMFLOAT3 position = { 1.0, 1.0, 1.0, }, DirectX::XMFLOAT3 scale = { 1.0, 1.0, 1.0 }, DirectX::XMFLOAT3 angle = { 0, 0, 0.01 }, float radius = 5.0f, float height = 5.0f);
	virtual ~Object() {};

	//更新処理
	virtual void Update(float elapsedTime);

	//描画処理
	virtual void Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID);

	DirectX::XMFLOAT3& GetPosition() { return position; }
	DirectX::XMFLOAT4X4& GetTransform() { return transform; }

	DirectX::XMVECTOR GetVec(DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 position);

	float& GetRadius() { return radius; }
	float& GetHeight() { return height;}

	Model* GetModel() { return model.get(); }

	virtual void DrawGUI() {};
protected:
	//行列更新処理
	virtual void UpdateTransform();

protected:
	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };

	DirectX::XMFLOAT4X4 transform = //位置、回転、スケール
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float radius = 0.0f;
	float height = 0.0f;

	std::unique_ptr<Model> model = nullptr;
};