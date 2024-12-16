#pragma once
#include<wrl.h>

#include<memory>
#include<InputMouse.h>

#include"Graphics.h"
#include"Sprite.h"

//ポーズ画面に切り替えるクラス
class Pause
{
private:
    bool flag_;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_;

    //座標とサイズ
    float base_pos_x_ = 320.f;
    float base_pos_y_ = 120.f;
    float base_size_x_ = 640.f;
    float base_size_y_ = 120.f;


    float scale_[3];

    //現在のステージを保持
    int stage_nun_;

    float animation_timer_ = 0.f;



    //表示する文字
    std::unique_ptr<Sprite> pause_select_;
    std::unique_ptr<Sprite> pause_retry_;
    std::unique_ptr<Sprite> pause_title_;
    std::unique_ptr<Sprite> pause_key_;
    std::unique_ptr<Sprite> pause_g_;
    std::unique_ptr<Sprite> pause_space_;

    //画面全体を暗くするための四角形
    std::unique_ptr<Sprite> pause_back_;

public:
    Pause();
    ~Pause() { animation_timer_ = 0.f; }

    static Pause& Instance() {
        static Pause pause;
        return pause;
    }

    //ステージのナンバーを取得
    void SetStageNum(int number){this->stage_nun_=number;}

    //ポーズを切り替える関数
    void SetPause(bool flag);

    //ポーズ状態かどうか確認する関数
    bool GetPause() { return flag_; }

    //ポーズ画面の更新処理
    void Update(float elapsedTime);

    //ポーズ画面を描画する
    void Render(float elapsedTime);
};