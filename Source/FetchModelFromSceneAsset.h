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

    const std::vector<std::pair<UINT, DirectX::XMFLOAT4X4>>& GetCommited() const { return commitedBlocks; }
    const std::vector<std::shared_ptr<Model>>& GetSceneModels() const { return sceneModels; }
    
    void SelectedBlockRender(RenderContext& rc, ModelRenderer* renderer, DirectX::XMFLOAT4X4 worldTransform, UINT index = 0, ShaderId id = ShaderId::Basic, bool isOrtho = false);
    void CommitBlock(std::pair<UINT, DirectX::XMFLOAT4X4>&& pair) { commitedBlocks.emplace_back(pair); }
    void RenderCommitedBlocks(RenderContext& rc, ModelRenderer* renderer, ShaderId id = ShaderId::Basic, bool ortho = false);

    //プレイヤーの方でモデル参照するために追記 間違ってたらごめん
    std::vector<std::shared_ptr<Model>>& GetSceneModels() { return sceneModels; }
    std::vector<std::pair<UINT, DirectX::XMFLOAT4X4>>& GetCommitedBlocks() { return commitedBlocks; }
};