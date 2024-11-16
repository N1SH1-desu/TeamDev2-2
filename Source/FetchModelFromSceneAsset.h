#pragma once
#include <Model.h>
#include <vector>
#include <memory>

class SceneModel
{
private:
    std::vector<std::shared_ptr<Model>> sceneModels;
    std::shared_ptr<Model> scene;
public:
    SceneModel(const char* sceneFile);
};