#pragma once
#include<windows.h>
#include<memory>

#include"Sprite.h"

//いくつかの数字を管理するというより、望みの数字を返す
class NumberManager
{
private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_;


    std::unique_ptr<Sprite> sprite_number_;
    
    //タイマーの為の変数たち
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

    //タイマーを設定する
    void SetTimer(int timer);
    //タイマーのアップデート
    void UpdateTimer();

    //ｚ＝0.1f
    //タイマーの描画
    void DrawTimer(DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
    //z=0.1f
    //入れた数値を描画する
    //二桁の数字まで描画できる
    void DrawNumber(int number,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
};