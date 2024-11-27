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

//�s��X�V����
void Object::UpdateTransform()
{
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//�ʒu�s��
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//��]�s��
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

	DirectX::XMMATRIX R = Y * X * Z;

	//���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;

	//���[���h�s������o��
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