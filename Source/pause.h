#pragma once

#include<memory>

#include"Sprite.h"

//�|�[�Y��ʂɐ؂�ւ���N���X
class Pause
{
private:
    bool flag_;

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

    //�|�[�Y��؂�ւ���֐�
    void SetPause(bool flag);

    //�|�[�Y��Ԃ��ǂ����m�F����֐�
    bool GetPause() { return flag_; }

    //�|�[�Y��ʂ̍X�V����
    void Update(float elapsedTime);

    //�|�[�Y��ʂ�`�悷��
    void Render(float elapsedTime);
};