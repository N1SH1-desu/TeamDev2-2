#pragma once

#include<memory>

#include"Sprite.h"

//�������̐������Ǘ�����Ƃ������A�]�݂̐�����Ԃ�
class number_namager
{
private:
    std::unique_ptr<Sprite> sprite_number_;
    float timer_;
public:

    number_namager();
    ~number_namager() = default;

    //�^�C�}�[��ݒ肷��
    void SetTimer(float timer);
    //�^�C�}�[�̃A�b�v�f�[�g
    void UpdateTimer(float elapsedTime);

    //�^�C�}�[�̕`��
    void DrawTimer(DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
    //���ꂽ���l��`�悷��
    void DrawNumber(float number,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
};