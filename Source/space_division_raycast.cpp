#include"space_division_raycast.h"

#include<imgui.h>

#include<sstream>

#include"Graphics.h"
#include"ShapeRenderer.h"
#include"PrimitiveRenderer.h"

SpaceDivisionRayCast::SpaceDivisionRayCast()
{

}

//四分木空間分割させたいモデルを読み込む.被りがあった場合は入れない
//読み込んだモデルを四分木しておく
void SpaceDivisionRayCast::Load(Model* model,DirectX::XMFLOAT4X4 world_transform)
{
    if (models_.size() > 0)
    {
        int size = models_.size();
        for (int i = 0; i < size; i++)
        {
            if (models_[i] == model)return;
        }
    }

    //モデルの全体AABB格納用変数
    DirectX::XMVECTOR VolumeMin = DirectX::XMVectorReplicate(FLT_MAX);
    DirectX::XMVECTOR VolumeMax = DirectX::XMVectorReplicate(-FLT_MAX);

    //頂点データをワールド空間変換し、三角形データを作成
    int meshes_size = model->GetResource()->GetMeshes().size();
    const ModelResource::Mesh* mesh = model->GetResource()->GetMeshes().data();
    for (int mesh_i = 0; mesh_i < meshes_size; mesh_i++)
    {        
        DirectX::XMMATRIX parent_world_transform = DirectX::XMLoadFloat4x4(&world_transform);
        DirectX::XMMATRIX local_transform = DirectX::XMLoadFloat4x4(&model->GetNodes().at(mesh_i).localTransform);
        DirectX::XMMATRIX parent_global_transform;
        if (model->GetNodes().at(mesh_i).parent!=nullptr)
        {
            parent_global_transform = DirectX::XMLoadFloat4x4(&model->GetNodes().at(mesh_i).parent->globalTransform);
        }
        else
        {
            parent_global_transform = DirectX::XMMatrixIdentity();
        }
        DirectX::XMMATRIX global_transform =local_transform * parent_global_transform;
        DirectX::XMMATRIX world_transform = global_transform * parent_world_transform;
        //頂点データをワールド空間変換
        size_t indices_size = mesh[mesh_i].indices.size();
        const UINT* mesh_index = mesh[mesh_i].indices.data();
        const ModelResource::Vertex* mesh_vertex = mesh[mesh_i].vertices.data();
        for (size_t i = 0; i < indices_size; i += 3)
        {
            uint32_t a = mesh_index[i + 0];
            uint32_t b = mesh_index[i + 1];
            uint32_t c = mesh_index[i + 2];
            DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&mesh_vertex[a].position);
            DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&mesh_vertex[b].position);
            DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&mesh_vertex[c].position);
            A = DirectX::XMVector3Transform(A, world_transform);
            B = DirectX::XMVector3Transform(B, world_transform);
            C = DirectX::XMVector3Transform(C, world_transform);
            //法線ベクトルの算出
            DirectX::XMVECTOR N = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(B, A), DirectX::XMVectorSubtract(C, A));
            if (DirectX::XMVector3Equal(N, DirectX::XMVectorZero()))
            {
                //面を構成できない場合は除外
                continue;
            }
            N = DirectX::XMVector3Normalize(N);
            //モデルの三角形データを格納
            CollisionMesh::Triangle& triangle = model_divisions_[model].triangles.emplace_back();
            DirectX::XMStoreFloat3(&triangle.positions[0], A);
            DirectX::XMStoreFloat3(&triangle.positions[1], B);
            DirectX::XMStoreFloat3(&triangle.positions[2], C);
            DirectX::XMStoreFloat3(&triangle.normal, N);
            //モデル全体のAABBを計算
            VolumeMin = DirectX::XMVectorMin(VolumeMin, A);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, B);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, C);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, A);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, B);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, C);
        }
    }

    //モデル全体のAABB
    DirectX::XMFLOAT3 volume_min, volume_max;
    DirectX::XMStoreFloat3(&volume_min, VolumeMin);
    DirectX::XMStoreFloat3(&volume_max, VolumeMax);

    float box_size_y = volume_max.y - volume_min.y;

    //モデル全体のAABBからXZ平面に指定のサイズで分割されたコリジョンエリアを作成する
    for (float x = volume_min.x; x < volume_max.x; x += cell_size_)
    {
        for (float z = volume_min.z; z < volume_max.z; z += cell_size_)
        {
            //小さいコリジョンエリアのAABBを算出
            CollisionMesh::Area& area = model_divisions_[model].areas.emplace_back();
            area.boundingBox.Center.x = x;
            area.boundingBox.Center.y = box_size_y*0.5f;
            area.boundingBox.Center.z = z;
            area.boundingBox.Extents.x =  area.boundingBox.Extents.z = cell_size_*0.5f;
            area.boundingBox.Extents.y = box_size_y*.5f;

            //AABBに所属する三角形を抽出
            //for (const  CollisionMesh::Triangle& triangle : model_divisions_[model].triangles)
            //{
            //    DirectX::XMVECTOR triangle_A = DirectX::XMLoadFloat3(&triangle.positions[0]);
            //    DirectX::XMVECTOR triangle_B = DirectX::XMLoadFloat3(&triangle.positions[1]);
            //    DirectX::XMVECTOR triangle_C = DirectX::XMLoadFloat3(&triangle.positions[2]);
            //    if (area.boundingBox.Intersects(
            //        triangle_A,
            //        triangle_B,
            //        triangle_C
            //    ))
            //    {
            //        area.triangleIndices.emplace_back();
            //    }
            //}

            //AABBに所属する三角形を抽出
            const int size = model_divisions_[model].triangles.size();
            CollisionMesh::Triangle* triangle = model_divisions_[model].triangles.data();
            for (int i = 0; i < size; i++)
            {
                DirectX::XMVECTOR a = DirectX::XMLoadFloat3(&triangle[i].positions[0]);
                DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&triangle[i].positions[1]);
                DirectX::XMVECTOR c = DirectX::XMLoadFloat3(&triangle[i].positions[2]);
                //if (area.boundingBox.Intersects(a, b, c))
                DirectX::XMVECTOR vec_tri_min = DirectX::XMVectorReplicate(FLT_MAX);
                DirectX::XMVECTOR vec_tri_max = DirectX::XMVectorReplicate(-FLT_MAX);
                vec_tri_min = DirectX::XMVectorMin(DirectX::XMVectorMin(a, b), c);
                vec_tri_max = DirectX::XMVectorMax(DirectX::XMVectorMax(a, b), c);
                DirectX::XMFLOAT3 tri_min, tri_max;
                DirectX::XMStoreFloat3(&tri_min, vec_tri_min);
                DirectX::XMStoreFloat3(&tri_max, vec_tri_max);
                if (area.boundingBox.Center.x - area.boundingBox.Extents.x > tri_max.x)continue;
                if(area.boundingBox.Center.x + area.boundingBox.Extents.x < tri_min.x)continue;
                if (area.boundingBox.Center.z - area.boundingBox.Extents.z > tri_max.z)continue;
                if(area.boundingBox.Center.z + area.boundingBox.Extents.z < tri_min.z)continue;

                {
                    area.triangleIndices.emplace_back(i);
                }
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
                if (area[area_i].boundingBox.Intersects(vec_start, direction, dist))
                {
                    //当たったAABBの中にあるメッシュの三角形とレイの当たり判定を取る
                    int triangles_size = area[area_i].triangleIndices.size();
                    const CollisionMesh::Triangle* triangle = collision_mesh.triangles.data();
                    for (int triangle_i = 0; triangle_i < triangles_size; triangle_i++)
                    {
                        DirectX::XMVECTOR triangle_a = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[0]);
                        DirectX::XMVECTOR triangle_b = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[1]);
                        DirectX::XMVECTOR triangle_c = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[2]);
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
                            hit_normal = triangle[triangle_i].normal;
                            hit = true;
                        }
                    }
                }
            }

 /*           for (const CollisionMesh::Area& area : collision_mesh.areas)
            {
                float dist = distance;

                if (area.boundingBox.Intersects(vec_start, direction, dist))
                {
                    int triangles_size = area.triangleIndices.size();
                    const CollisionMesh::Triangle* triangle = collision_mesh.triangles.data();
                    for (int triangle_i = 0; triangle_i < triangles_size; triangle_i++)
                    {
                        DirectX::XMVECTOR triangle_A = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[0]);
                        DirectX::XMVECTOR triangle_B = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[1]);
                        DirectX::XMVECTOR triangle_C = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[2]);
                        dist = distance;
                        if (DirectX::TriangleTests::Intersects(vec_start, direction, triangle_A, triangle_B, triangle_C, dist))
                        {
                            if (distance < dist)continue;
                            distance = dist;
                            hit_normal = triangle[triangle_i].normal;
                            hit = true;
                            break;
                        }
                    }
                }
                if (hit)break;
            }*/

            //for (const CollisionMesh::Triangle& triangle : collision_mesh.triangles)
            //{
            //    DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&triangle.positions[0]);
            //    DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&triangle.positions[1]);
            //    DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&triangle.positions[2]);
            //    float dist = distance;
            //    if (DirectX::TriangleTests::Intersects(vec_start, direction, A, B, C, dist))
            //    {
            //        if (distance < dist) continue;
            //        distance = dist;
            //        hit_normal = triangle.normal;
            //        hit = true;
            //    }
            //}
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
            polygonColor = { 0,0,0,1 };
        }
        else
        {
            boxColor = { 0,1,0,1 };
            polygonColor = { 0, 0, 1, 0.5f };
        }
        //エリアに保存されているトライアングル情報を出す
        int triangles_size = area[i].triangleIndices.size();
        const CollisionMesh::Triangle* triangle = model_divisions_[model].triangles.data();
        for (int triangle_i = 0; triangle_i < triangles_size; triangle_i++)
        {
            //primitiveRenderer->AddVertex(triangle[triangle_i].positions[0], polygonColor);
            //primitiveRenderer->AddVertex(triangle[triangle_i].positions[1], polygonColor);
            //primitiveRenderer->AddVertex(triangle[triangle_i].positions[2], polygonColor);
        }

        shape_renderer->DrawBox(area[i].boundingBox.Center, boxAngle, area[i].boundingBox.Extents, boxColor);
    }

    ////コチラはモデルから取り出した全ての三角形を描画する
    //// 三角形ポリゴン描画
    for (CollisionMesh::Triangle& triangle : model_divisions_[model].triangles)
    {
        primitiveRenderer->AddVertex(triangle.positions[0], polygonColor);
        primitiveRenderer->AddVertex(triangle.positions[1], polygonColor);
        primitiveRenderer->AddVertex(triangle.positions[2], polygonColor);
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

    float window_alpha = 0.5f;
    ImGui::SetNextWindowBgAlpha(window_alpha);

    if (ImGui::Begin("SpaceDivision"))
    {
        size_t size = models_.size();
        for (int i = 0; i < size; i++)
        {
            ImGui::Separator();

            CollisionMesh collision_mesh = model_divisions_[models_[i]];
            int area_size = collision_mesh.areas.size();
            ImGui::InputInt("area_size", &area_size);
            int area_triangle_size = collision_mesh.triangles.size();
            ImGui::InputInt("area_triangle_size", &area_triangle_size);

            ImGui::InputInt("draw_box", &draw_box_);
            if (draw_box_ >= area_size)draw_box_ = draw_box_ - area_size;
            
            int triangle_parsent = 0;
            CollisionMesh::Area* area = collision_mesh.areas.data();
            ImGui::BeginChild(ImGui::GetID((void*)i), ImVec2(300, 200), ImGuiWindowFlags_NoTitleBar);
            {
                for (int area_i = 0; area_i < area_size; area_i++)
                {
                    outs.str("");
                    int triangle_in_area = area[area_i].triangleIndices.size();
                    outs << area_i << " : triangle_in_area";
                    ImGui::InputInt(outs.str().c_str(), &triangle_in_area);
                    triangle_parsent += triangle_in_area;

                }
                triangle_parsent = static_cast<int>((triangle_parsent / (area_triangle_size*0.01f )));
                
            }
            ImGui::EndChild();
            //ImGui::InputInt("", &triangle_parsent);
            outs.str("");
            outs << triangle_parsent << ": in_box";
            ImGui::TextWrapped(outs.str().c_str());
        }
    }

    ImGui::End();
#endif
}