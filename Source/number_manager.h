#pragma once

#include<memory>

#include"Sprite.h"

//�������̐������Ǘ�����Ƃ������A�]�݂̐�����Ԃ�
class NumberManager
{
private:
    std::unique_ptr<Sprite> sprite_number_;
    float timer_;
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
    void UpdateTimer(float elapsedTime);

    //�^�C�}�[�̕`��
    void DrawTimer(DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
    //���ꂽ���l��`�悷��
    //�񌅂̐����܂ŕ`��ł���
    void DrawNumber(int number,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
};