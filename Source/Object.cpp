#include "Object.h"

Object::Object(const char* filepath, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 angle, float radius, float height)
{
	if (filepath)
		model = std::make_unique<Model>(filepath);

	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->radius = radius;
	this->height = height;
}

//行列更新処理
void Object::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//位置行列
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//回転行列
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

	DirectX::XMMATRIX R = Y * X * Z;

	//ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;

	//ワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Object::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform();
}

void Object::Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
{
	modelRenderer->Render(rc, transform, model.get(), ID);
}

DirectX::XMVECTOR Object::GetVec(DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 position)
{
	return DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&position));
}