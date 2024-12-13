#pragma once
#pragma once
#include<InputMouse.h>
#include<wrl.h>

#include<memory>

#include"Sprite.h"
#include"Audio/Audio.h"

class Clear
{
private:
    std::unique_ptr<Sprite> sprite_clear_ = nullptr;
    std::unique_ptr<Sprite> sprite_select_ = nullptr;
    std::unique_ptr<Sprite> sprite_title_ = nullptr;
    std::unique_ptr<Sprite> sprite_back_ = nullptr;

    //描画位置
    float base_x_ = 160.f;      //1280/8
    float base_y_ = 120.f;      //720/6

    float clear_scale_ = 0.f;
    float select_scale_ = 1.f;
    float title_scale_ = 1.f;

    //正の時クリア
    bool flag_ = false;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_;

    std::unique_ptr<AudioSource>    sound_;
public:
    Clear();
    ~Clear() = default;

    static Clear& Instance() {
        static Clear instance_;
        return instance_;
    }

    void Initialize();
    //マウスの位置が欲しいので引数に入れている
    void Update(float elapsedTime);
    void Render(float elapsedTime);

    void SetClearFlag(bool flag) { this->flag_ = flag; }
    bool GetClearFlag() { return this->flag_; }
};