#pragma once
#include<windows.h>
#include<memory>

#include"Sprite.h"

//�������̐������Ǘ�����Ƃ������A�]�݂̐�����Ԃ�
class NumberManager
{
private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_;


    std::unique_ptr<Sprite> sprite_number_;
    
    //�^�C�}�[�ׂ̈̕ϐ�����
    float timer_;
    LARGE_INTEGER frequency_, start_, now_;

    float sprite_depth_ = 0.1;
public:
    static NumberManager& Instance() {
        static NumberManager instance_;
        return instance_;
    }

    NumberManager();
    ~NumberManager() = default;

    //�^�C�}�[��ݒ肷��
    void SetTimer(int timer);
    //�^�C�}�[�̃A�b�v�f�[�g
    void UpdateTimer();

    //����0.1f
    //�^�C�}�[�̕`��
    void DrawTimer(DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
    //z=0.1f
    //���ꂽ���l��`�悷��
    //�񌅂̐����܂ŕ`��ł���
    void DrawNumber(int number,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
};