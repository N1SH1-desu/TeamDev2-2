#pragma once

#include<memory>

#include"Sprite.h"

//いくつかの数字を管理するというより、望みの数字を返す
class number_namager
{
private:
    std::unique_ptr<Sprite> sprite_number_;
    float timer_;
public:

    number_namager();
    ~number_namager() = default;

    //タイマーを設定する
    void SetTimer(float timer);
    //タイマーのアップデート
    void UpdateTimer(float elapsedTime);

    //タイマーの描画
    void DrawTimer(DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
    //入れた数値を描画する
    void DrawNumber(float number,DirectX::XMFLOAT2 pos,DirectX::XMFLOAT2 size);
};