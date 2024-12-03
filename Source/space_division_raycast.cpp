#include"space_division_raycast.h"

#include<imgui.h>

#include<sstream>
#include<algorithm>

#include"Graphics.h"
#include"ShapeRenderer.h"
#include"PrimitiveRenderer.h"

SpaceDivisionRayCast::SpaceDivisionRayCast()
{

}

//四分木空間分割させたいモデルを読み込む.被りがあった場合は入れない
//読み込んだモデルを四分木しておく//
 void SpaceDivisionRayCast::Load(Model* model)
{
    // 重複モデルのスキップ
    if (std::find(models_.begin(), models_.end(), model) != models_.end()) {
        return;
    }

    DirectX::XMVECTOR VolumeMin = DirectX::XMVectorReplicate(FLT_MAX);
    DirectX::XMVECTOR VolumeMax = DirectX::XMVectorReplicate(-FLT_MAX);

    const auto& meshes = model->GetResource()->GetMeshes();
    const auto& nodes = model->GetNodes();
    size_t meshes_size = meshes.size();
    for (size_t mesh_i = 0; mesh_i < meshes_size; ++mesh_i) {
        const auto& mesh = meshes[mesh_i];

        
        // ノードの行列変換
        const auto& node = nodes[mesh_i];
        DirectX::XMMATRIX local_transform = DirectX::XMLoadFloat4x4(&node.localTransform);
        DirectX::XMMATRIX parent_transform = node.parent
            ? DirectX::XMLoadFloat4x4(&node.parent->globalTransform)
            : DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX global_transform = local_transform * parent_transform;

        // 頂点データをワールド空間に変換
        size_t size = mesh.indices.size();
        for (size_t i = 0; i < size; i += 3) {
            uint32_t a = mesh.indices[i + 0];
            uint32_t b = mesh.indices[i + 1];
            uint32_t c = mesh.indices[i + 2];

            DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&mesh.vertices[a].position);
            DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&mesh.vertices[b].position);
            DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&mesh.vertices[c].position);

            A = DirectX::XMVector3Transform(A, global_transform);
            B = DirectX::XMVector3Transform(B, global_transform);
            C = DirectX::XMVector3Transform(C, global_transform);

            // 面積ゼロの三角形をスキップ
            DirectX::XMVECTOR N = DirectX::XMVector3Cross(
                DirectX::XMVectorSubtract(A,C),
                DirectX::XMVectorSubtract(B,C)
            );
            if (DirectX::XMVector3Equal(N, DirectX::XMVectorZero())) continue;

            // 三角形のデータを保存
            CollisionMesh::Triangle& triangle = model_divisions_[model].triangles.emplace_back();
            DirectX::XMStoreFloat3(&triangle.positions[0], A);
            DirectX::XMStoreFloat3(&triangle.positions[1], B);
            DirectX::XMStoreFloat3(&triangle.positions[2], C);
            DirectX::XMStoreFloat3(&triangle.normal, DirectX::XMVector3Normalize(N));

            // AABB 更新
            VolumeMin = DirectX::XMVectorMin(VolumeMin, A);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, B);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, C);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, A);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, B);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, C);
        }
    }

    // モデル全体の AABB を保存
    DirectX::XMFLOAT3 volume_min, volume_max;
    DirectX::XMStoreFloat3(&volume_min, VolumeMin);
    DirectX::XMStoreFloat3(&volume_max, VolumeMax);

    float box_size_y = volume_max.y - volume_min.y;

    XMFLOAT2 cell_size = {2,2 };

    // 空間分割
    for (float x = volume_min.x; x < volume_max.x; x += cell_size.x) {
        for (float z = volume_min.z; z < volume_max.z; z += cell_size.y) {
            CollisionMesh::Area& area = model_divisions_[model].areas.emplace_back();
            area.bounding_box.Center =
            { 
                x + cell_size.x * 0.5f,
                volume_min.y + box_size_y * 0.5f,
                z + cell_size.y * 0.5f 
            };
            area.bounding_box.Extents = { cell_size.x * 0.5f, box_size_y * 0.5f, cell_size.x * 0.5f };
            //この中でトライアングルの登録のためのfor文を書くと
            //四回同じfor文を通ることになるのでそれはちょっと…
        }
    }

    for (int i = 0; i < node_depth_; i++)
    {
        //int d = (i==0)
        //    ?1
        //    :2<<i-1;
        //const float cell_size_x = (volume_max.x - volume_min.x) / static_cast<float>(d);
        //const float cell_size_z = (volume_max.z - volume_min.z) / static_cast<float>(d);

        //if (d == 1)
        //{
        //    CollisionMesh::Area area;
        //    //(0,0)
        //    area.bounding_box.Center =
        //    {
        //        volume_min.x + cell_size_x * 0.5f,
        //        volume_min.y + box_size_y * 0.5f,
        //        volume_min.z + cell_size_z * 0.5f
        //    };
        //    area.bounding_box.Extents = { cell_size_x * 0.5f, box_size_y * 0.5f, cell_size_z * 0.5f };
        //    model_divisions_[model].areas.emplace_back(area);
        //}
        //else
        //{
        //    for (int z = 0; z < d; z += 2)
        //    {
        //        for (int x = 0; x < d; x++)
        //        {
        //            CollisionMesh::Area area;
        //            //(0,0)
        //            area.bounding_box.Center =
        //            {
        //                volume_min.x + ((x * cell_size_x) + cell_size_x * 0.5f),
        //                volume_min.y + box_size_y * 0.5f,
        //                volume_min.z + ((z * cell_size_z) + cell_size_z * 0.5f)
        //            };
        //            area.bounding_box.Extents = { cell_size_x * 0.5f, box_size_y * 0.5f, cell_size_z * 0.5f };
        //            model_divisions_[model].areas.emplace_back(area);
        //            //(0,1)

        //            x += 1;
        //            area.bounding_box.Center =
        //            {
        //                volume_min.x + ((x * cell_size_x) + cell_size_x * 0.5f),
        //                volume_min.y + box_size_y * 0.5f,
        //                volume_min.z + ((z * cell_size_z) + cell_size_z * 0.5f)
        //            };
        //            area.bounding_box.Extents = { cell_size_x * 0.5f, box_size_y * 0.5f, cell_size_z * 0.5f };
        //            model_divisions_[model].areas.emplace_back(area);
        //            //(1,0)
        //            x -= 1;
        //            z += 1;
        //            area.bounding_box.Center =
        //            {
        //                volume_min.x + ((x * cell_size_x) + cell_size_x * 0.5f),
        //                volume_min.y + box_size_y * 0.5f,
        //                volume_min.z + ((z * cell_size_z) + cell_size_z * 0.5f)
        //            };
        //            area.bounding_box.Extents = { cell_size_x * 0.5f, box_size_y * 0.5f, cell_size_z * 0.5f };
        //            model_divisions_[model].areas.emplace_back(area);
        //            //(1,1)
        //            x += 1;
        //            area.bounding_box.Center =
        //            {
        //                volume_min.x + ((x * cell_size_x) + cell_size_x * 0.5f),
        //                volume_min.y + box_size_y * 0.5f,
        //                volume_min.z + ((z * cell_size_z) + cell_size_z * 0.5f)
        //            };
        //            area.bounding_box.Extents = { cell_size_x * 0.5f, box_size_y * 0.5f, cell_size_z * 0.5f };
        //            model_divisions_[model].areas.emplace_back(area);
        //            //計算の都合上、zをここで下げておく？
        //            z -= 1;
        //        }
        //    }
        //}
    }

    size_t triangles_size = model_divisions_[model].triangles.size();
    const auto* triangle = model_divisions_[model].triangles.data();
    for (size_t i = 0; i < triangles_size; ++i)
    {
        DirectX::XMFLOAT3 tri_min, tri_max;
        tri_min =
        {
            (std::min)((std::min)(triangle[i].positions[0].x,triangle[i].positions[1].x),triangle[i].positions[2].x),
            (std::min)((std::min)(triangle[i].positions[0].y,triangle[i].positions[1].y),triangle[i].positions[2].y),
            (std::min)((std::min)(triangle[i].positions[0].z,triangle[i].positions[1].z),triangle[i].positions[2].z)
        };
        tri_max = {
            (std::max)((std::max)(triangle[i].positions[0].x,triangle[i].positions[1].x),triangle[i].positions[2].x),
            (std::max)((std::max)(triangle[i].positions[0].y,triangle[i].positions[1].y),triangle[i].positions[2].y),
            (std::max)((std::max)(triangle[i].positions[0].z,triangle[i].positions[1].z),triangle[i].positions[2].z)
        };

        int areas_size = model_divisions_[model].areas.size();
        const auto* area = model_divisions_[model].areas.data();
        for (int area_i=0;area_i<areas_size;area_i++)
        {
            DirectX::XMFLOAT3 area_min, area_max;
            area_min = {
                area[area_i].bounding_box.Center.x - area[area_i].bounding_box.Extents.x,
                area[area_i].bounding_box.Center.y,
                area[area_i].bounding_box.Center.z - area[area_i].bounding_box.Extents.z
            };
            area_max = {
                area[area_i].bounding_box.Center.x + area[area_i].bounding_box.Extents.x,
                area[area_i].bounding_box.Center.y,
                area[area_i].bounding_box.Center.z + area[area_i].bounding_box.Extents.z
            };

            if (area_min.x > tri_max.x)continue;
            if (area_max.x < tri_min.x)continue;
            if (area_min.z > tri_max.z)continue;
            if (area_max.z < tri_min.z)continue;

            {
                model_divisions_[model].areas.at(area_i).triangle_indices.emplace_back(i);
            }
        }
    }

    models_.emplace_back(model);
}


//四分木空間分割でレイキャストする
//モデルのアドレスで読み込まれたものがある場合四分木空間分割レイキャストする
bool SpaceDivisionRayCast::RayCast(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    Model* model,
    DirectX::XMFLOAT3& hit_position,
    DirectX::XMFLOAT3& hit_normal)
{
    bool hit = false;
    DirectX::XMVECTOR vec_start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR vec_end = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(vec_end, vec_start);
    DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(vec);
    float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vec));

    if (distance <= 0.0f)return false;

    int models_size = models_.size();
    for (int i = 0; i < models_size; i++)
    {
        if (models_[i] == model)
        {
            //小さいAABBとレイの交差判定を取る
            const CollisionMesh& collision_mesh = model_divisions_[model];
            int areas_size =collision_mesh.areas.size();
            const CollisionMesh::Area* area = collision_mesh.areas.data();
            for (int area_i = 0; area_i < areas_size; area_i++)
            {
                float dist = distance;
                if (area[area_i].bounding_box.Intersects(vec_start, direction, dist))
                {
                    //当たったAABBの中にあるメッシュの三角形とレイの当たり判定を取る
                    int triangles_size = area[area_i].triangle_indices.size();
                    const int* triangle_i = area[area_i].triangle_indices.data();
                    const CollisionMesh::Triangle* triangle = collision_mesh.triangles.data();
                    for (int areas_triangle_i = 0; areas_triangle_i < triangles_size; areas_triangle_i++)
                    {
                        DirectX::XMVECTOR triangle_a = DirectX::XMLoadFloat3(&triangle[triangle_i[areas_triangle_i]].positions[0]);
                        DirectX::XMVECTOR triangle_b = DirectX::XMLoadFloat3(&triangle[triangle_i[areas_triangle_i]].positions[1]);
                        DirectX::XMVECTOR triangle_c = DirectX::XMLoadFloat3(&triangle[triangle_i[areas_triangle_i]].positions[2]);
                        dist = distance;
                        if (DirectX::TriangleTests::Intersects(
                            vec_start,
                            direction,
                            triangle_a,
                            triangle_b,
                            triangle_c,
                            dist))
                        {
                            if (distance < dist)continue;
                            distance = dist;
                            hit_normal = triangle[triangle_i[areas_triangle_i]].normal;
                            hit = true;
                        }
                    }
                }
                if (hit)break;
            }
        }
    }
    if (hit)
    {
        DirectX::XMVECTOR vec_hit_position = DirectX::XMVectorAdd(vec_start, DirectX::XMVectorScale(direction, distance));
        DirectX::XMStoreFloat3(&hit_position, vec_hit_position);
    }
    return hit;
}

void SpaceDivisionRayCast::DebugDraw(RenderContext&rc,Model*model)
{
    PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
    ShapeRenderer* shape_renderer = Graphics::Instance().GetShapeRenderer();

    int size = models_.size();
    bool hit=false;
    //モデルを持ってなかったらしない
    for (int i = 0; i < size; i++)if (models_[i] == model)hit = true;
    if (!hit)return;

    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

    DirectX::XMFLOAT4 boxColor = { 0, 1, 0, 1 };
    const DirectX::XMFLOAT3 boxAngle = { 0, 0, 0 };

    DirectX::XMFLOAT4 polygonColor = { 0, 0, 1, 0.5f };
    size_t area_size = model_divisions_[model].areas.size();
    CollisionMesh::Area* area = model_divisions_[model].areas.data();
    for (int i = 0; i < area_size; i++)
    {
        if (draw_box_ == i)
        {
            boxColor = { 0,0,0,1 };
            polygonColor = { 1,1,1,1 };
        }
        else
        {
            boxColor = { 0,1,0,0.25f };
            polygonColor = { 0, 0, 1, 1 };
        }
        //エリアに保存されているトライアングル情報を出す
        if (!all_draw_)
        {
            //モデルにあるすべての頂点情報とエリアにある頂点情報を確認する
            //有るものだけを描画する
            const CollisionMesh::Triangle* triangle = model_divisions_[model].triangles.data();
            int areas_triangle_size = area[i].triangle_indices.size();
            const int* areas_triangle = area[i].triangle_indices.data();

            for (int areas_triangle_index = 0; areas_triangle_index < areas_triangle_size; areas_triangle_index++)
            {

                primitiveRenderer->AddVertex(triangle[areas_triangle[areas_triangle_index]].positions[0], polygonColor);
                primitiveRenderer->AddVertex(triangle[areas_triangle[areas_triangle_index]].positions[1], polygonColor);
                primitiveRenderer->AddVertex(triangle[areas_triangle[areas_triangle_index]].positions[2], polygonColor);
            }
            
        }
        shape_renderer->DrawBox(area[i].bounding_box.Center, boxAngle, area[i].bounding_box.Extents, boxColor);
    }

    ////コチラはモデルから取り出した全ての三角形を描画する
    //// 三角形ポリゴン描画
    if (all_draw_)
    {
        polygonColor = { 1,1,1,1 };
        size_t size = model_divisions_[model].triangles.size();
        CollisionMesh::Triangle* triangle = model_divisions_[model].triangles.data();
        for (size_t i = 0; i < size; i++)
        {
            primitiveRenderer->AddVertex(triangle[i].positions[0], polygonColor);
            primitiveRenderer->AddVertex(triangle[i].positions[1], polygonColor);
            primitiveRenderer->AddVertex(triangle[i].positions[2], polygonColor);
        }
    }

    primitiveRenderer->Render(dc, rc.camera->GetView(), rc.camera->GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    shape_renderer->Render(dc, rc.camera->GetView(), rc.camera->GetProjection());
}

void SpaceDivisionRayCast::DrowImgui()
{
#if _DEBUG
    Graphics* graphics = &Graphics::Instance();

    std::ostringstream outs;
    outs.precision(2);

    ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
    ImGui::SetNextWindowPos({ pos.x + 300,pos.y + 500 });
    ImGui::SetNextWindowSize({ 980,220 });
    float window_alpha = 0.5f;
    ImGui::SetNextWindowBgAlpha(window_alpha);

    if (ImGui::Begin("SpaceDivision"))
    {
        if (ImGui::Button("all_draw_"))all_draw_ = !all_draw_;
        
        ImGui::InputInt("draw_box", &draw_box_);

        size_t size = models_.size();
        for (int i = 0; i < size; i++)
        {
            ImGui::Separator();

            CollisionMesh collision_mesh = model_divisions_[models_[i]];
            int area_size = collision_mesh.areas.size();
            ImGui::InputInt("area_size", &area_size);
            int area_triangle_size = collision_mesh.triangles.size();
            ImGui::InputInt("area_triangle_size", &area_triangle_size);

            if (draw_box_ >= area_size)draw_box_ = draw_box_ - area_size;
            
            int triangle_parsent = 0;
            CollisionMesh::Area* area = collision_mesh.areas.data();
            ImGui::BeginChild(ImGui::GetID((void*)i), ImVec2(300, 200), ImGuiWindowFlags_NoTitleBar);
            {
                for (int area_i = 0; area_i < area_size; area_i++)
                {
                    outs.str("");
                    int triangle_in_area = area[area_i].triangle_indices.size();
                    outs << area_i << " : areas_triangles:"<<triangle_in_area;
                    ImGui::TextWrapped(outs.str().c_str());
                    triangle_parsent += triangle_in_area;

                }
            }
            ImGui::EndChild();
                
            triangle_parsent = static_cast<int>((triangle_parsent / (area_triangle_size*0.01f )));
            //ImGui::InputInt("", &triangle_parsent);
            outs.str("");
            outs << triangle_parsent << ": in_box";
            ImGui::TextWrapped(outs.str().c_str());
        }
    }

    ImGui::End();
#endif
}