#pragma once
#include<DirectXMath.h>
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>
#include "Effect.h"
#include<vector>

enum EFFECT_TYPE
{
	SMOKE,
	POISON
};

//エフェクトマネージャー
class EffectManager
{
private:
	EffectManager() {}
	~EffectManager() {}

public:
	//唯一のインスタンス取得
	static EffectManager& instance()
	{
		static EffectManager instance;
		return instance;
	}

	//初期化
	void Initialize();

	//終了化
	void Finalize();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	//Effeckseerマネージャーの取得
	Effekseer::ManagerRef GetEffeckseerManager() { return effekseerManager; }

	//Effectの取得
	Effect* GetEffect(int type) { return effect.at(type).get(); }

private:
	Effekseer::ManagerRef effekseerManager;
	EffekseerRenderer::RendererRef effekseerRenderer;

	std::vector<std::unique_ptr<Effect>>effect;
};