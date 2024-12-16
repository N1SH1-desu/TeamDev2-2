#pragma once
#include<wrl.h>

#include<memory>
#include<InputMouse.h>

#include"Graphics.h"
#include"Sprite.h"

//�|�[�Y��ʂɐ؂�ւ���N���X
class Pause
{
private:
    bool flag_;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_;

    //���W�ƃT�C�Y
    float base_pos_x_ = 320.f;
    float base_pos_y_ = 120.f;
    float base_size_x_ = 640.f;
    float base_size_y_ = 120.f;


    float scale_[3];

    //���݂̃X�e�[�W��ێ�
    int stage_nun_;

    float animation_timer_ = 0.f;



    //�\�����镶��
    std::unique_ptr<Sprite> pause_select_;
    std::unique_ptr<Sprite> pause_retry_;
    std::unique_ptr<Sprite> pause_title_;
    std::unique_ptr<Sprite> pause_key_;
    std::unique_ptr<Sprite> pause_g_;
    std::unique_ptr<Sprite> pause_space_;

    //��ʑS�̂��Â����邽�߂̎l�p�`
    std::unique_ptr<Sprite> pause_back_;

public:
    Pause();
    ~Pause() { animation_timer_ = 0.f; }

    static Pause& Instance() {
        static Pause pause;
        return pause;
    }

    //�X�e�[�W�̃i���o�[���擾
    void SetStageNum(int number){this->stage_nun_=number;}

    //�|�[�Y��؂�ւ���֐�
    void SetPause(bool flag);

    //�|�[�Y��Ԃ��ǂ����m�F����֐�
    bool GetPause() { return flag_; }

    //�|�[�Y��ʂ̍X�V����
    void Update(float elapsedTime);

    //�|�[�Y��ʂ�`�悷��
    void Render(float elapsedTime);
};