#pragma once

#include<memory>

#include"Sprite.h"

//いくつかの数字を管理するというより、望みの数字を返す
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

    //タイマーを設定する
    void SetTimer(int timer);
    //タイマーのアップデート
    void UpdateTimer(float elapsedTime);

    //タイマーの描画
    void DrawTimer(DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
    //入れた数値を描画する
    //二桁の数字まで描画できる
    void DrawNumber(int number,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
};