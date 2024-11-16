#include "FetchModelFromSceneAsset.h"
#include "ModelResource.h"

SceneModel::SceneModel(const char* sceneFile)
{
    scene = std::make_shared<Model>(sceneFile);

    const ModelResource* resource =  scene->GetResource();
    
    size_t meshSize = resource->GetMeshes().size();

    for (int i = 0; i < meshSize; i++)
    {
        ModelResource::Mesh& mesh = const_cast<ModelResource::Mesh&>(resource->GetMeshes().at(i));
        Model::Node node = scene->GetNodes().at(mesh.nodeIndex);
        
        mesh.nodeIndex = 0;

        sceneModels.emplace_back(std::make_shared<Model>(std::move(node), std::move(mesh)));
    }
}
