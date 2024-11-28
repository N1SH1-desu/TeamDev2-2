#pragma once
#include "Stage.h"
#include "Singleton.h"
#include "Manager.h"
#include "Collision.h"

class StageManager : public Manager<Stage>, public Singleton<StageManager>
{
	friend class Singleton<StageManager>;

public:
	void Initialize() override;
	void Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID);

	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult hit);
};