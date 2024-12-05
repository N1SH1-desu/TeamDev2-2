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

//�G�t�F�N�g�}�l�[�W���[
class EffectManager
{
private:
	EffectManager() {}
	~EffectManager() {}

public:
	//�B��̃C���X�^���X�擾
	static EffectManager& instance()
	{
		static EffectManager instance;
		return instance;
	}

	//������
	void Initialize();

	//�I����
	void Finalize();

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	//Effeckseer�}�l�[�W���[�̎擾
	Effekseer::ManagerRef GetEffeckseerManager() { return effekseerManager; }

	//Effect�̎擾
	Effect* GetEffect(int type) { return effect.at(type).get(); }

private:
	Effekseer::ManagerRef effekseerManager;
	EffekseerRenderer::RendererRef effekseerRenderer;

	std::vector<std::unique_ptr<Effect>>effect;
};