#pragma once
#include<wrl.h>

#include<memory>

#include"Sprite.h"

//�|�[�Y��ʂɐ؂�ւ���N���X
class Pause
{
private:
    bool flag_;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;

    //���W�ƃT�C�Y
    const float base_pos_x_ = 320.f;
    const float base_pos_y_ = 120.f;
    const float base_size_x_ = 640.f;
    const float base_size_y_ = 120.f;

    //���݂̃X�e�[�W��ێ�
    int stage_nun_;

    std::unique_ptr<Sprite> pause_select_;
    std::unique_ptr<Sprite> pause_retry_;
    std::unique_ptr<Sprite> pause_title_;

public:
    Pause();
    ~Pause() = default;

    static Pause& Instance() {
        static Pause pause;
        return pause;
    }

    //�X�e�[�W�̃i���o�[���擾
    void SetStageNum(int number) { this->stage_nun_ = number; }

    //�|�[�Y��؂�ւ���֐�
    void SetPause(bool flag);

    //�|�[�Y��Ԃ��ǂ����m�F����֐�
    bool GetPause() { return flag_; }

    //�|�[�Y��ʂ̍X�V����
    void Update(float elapsedTime);

    //�|�[�Y��ʂ�`�悷��
    void Render(float elapsedTime);
};