#include"fogs.h"

#include<imgui.h>

#include"Graphics.h"
#include"Misc.h"

Fogs::Fogs()
{
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(fogConstants);

    ID3D11Device* dc = Graphics::Instance().GetDevice();
    HRESULT hr{ S_OK };
    hr = dc->CreateBuffer(&buffer_desc, nullptr, fog_constant_buffer_.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void Fogs::Update(float elapsedTime)
{

}

void Fogs::Render(float elapsedTime)
{
    ID3D11Device* dc = Graphics::Instance().GetDevice();
    ID3D11DeviceContext* immediate_context = Graphics::Instance().GetDeviceContext();

    fogConstants fogs{};
    fogs.fog_color = fog_color_;
    fogs.fog_range = this->fog_range_;
    immediate_context->UpdateSubresource(fog_constant_buffer_.Get(), 0, 0, &fogs, 0, 0);
    immediate_context->VSSetConstantBuffers(5, 1, fog_constant_buffer_.GetAddressOf());
    immediate_context->PSSetConstantBuffers(5, 1, fog_constant_buffer_.GetAddressOf());
}

void Fogs::DrawImgui()
{
#if _DEBUG
    ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
    ImGui::SetNextWindowPos({ pos.x + 100,pos.y + 500 });
    ImGui::SetNextWindowSize(ImVec2(280, 220));
    float window_alpha = 0.25f;
    ImGui::SetNextWindowBgAlpha(window_alpha);

    if (ImGui::Begin(u8"fogs", nullptr, ImGuiWindowFlags_NoNavInputs))
    {
        ImGui::SliderFloat("fog_near", &fog_range_.x, 0.1f, 100.f);
        ImGui::SliderFloat("fog_far", &fog_range_.y, 0.1f, 100.f);

    }
    ImGui::End();
#endif
}
