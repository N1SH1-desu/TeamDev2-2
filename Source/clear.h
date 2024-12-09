#pragma once
#include<InputMouse.h>
#include<wrl.h>

#include<memory>

#include"Sprite.h"

class Clear
{
private:
    std::unique_ptr<Sprite> sprite_clear_ = nullptr;
    std::unique_ptr<Sprite> sprite_select_ = nullptr;
    std::unique_ptr<Sprite> sprite_title_ = nullptr;
    std::unique_ptr<Sprite> sprite_back_ = nullptr;

    //正の時クリア
    bool flag_ = false;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_;
public:
    Clear();
    ~Clear() = default;

    static Clear& Instance() {
        static Clear instance_;
        return instance_;
    }

    //マウスの位置が欲しいので引数に入れている
    void Update(float elapsedTime, InputMouse* mouse);
    void Render(float elapsedTime);

    void SetClearFlag(bool flag) { this->flag_ = flag; }
    bool GetClearFlag()          { return this->flag_; }
};