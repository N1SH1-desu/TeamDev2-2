#pragma once
#include <Model.h>
#include <vector>
#include <memory>
#include <ModelRenderer.h>

class SceneModel
{
private:
    std::vector<std::shared_ptr<Model>> sceneModels;
    std::shared_ptr<Model> scene;
    std::vector<std::pair<UINT, DirectX::XMFLOAT4X4>> commitedBlocks;
public:
    SceneModel(const char* sceneFile);
    
    void SelectedBlockRender(RenderContext& rc, ModelRenderer* renderer, DirectX::XMFLOAT4X4 worldTransform, UINT index = 0, ShaderId id = ShaderId::Basic);
};