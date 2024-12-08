#pragma once

#include<memory>

#include"Sprite.h"

//ポーズ画面に切り替えるクラス
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

    //ポーズを切り替える関数
    void SetPause(bool flag);

    //ポーズ状態かどうか確認する関数
    bool GetPause() { return flag_; }

    //ポーズ画面の更新処理
    void Update(float elapsedTime);

    //ポーズ画面を描画する
    void Render(float elapsedTime);
};