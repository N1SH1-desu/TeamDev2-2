#include"space_division_raycast.h"

#include<imgui.h>

#include<sstream>
#include<algorithm>

#include"Graphics.h"
#include"ShapeRenderer.h"
#include"PrimitiveRenderer.h"

namespace quad_tree_calc {
    //四分木の最大の深さにおける分割数の計算
    inline constexpr uint64_t MaxSplit(const uint64_t depth)
    {
        return (static_cast<uint64_t>(1)) << depth;
    }

    //最大16bitの入力を、bitを一つ飛ばしにして出力する
    inline uint64_t BitSeparate16(uint64_t n)
    {
        n = (n | (n << 8)) & static_cast<uint64_t>(0x00FF00FF); //9~16bit
        n = (n | (n << 4)) & static_cast<uint64_t>(0x0F0F0F0F); //9~16bit
        n = (n | (n << 2)) & static_cast<uint64_t>(0x33333333); //9~16bit
        n = (n | (n << 1)) & static_cast<uint64_t>(0x55555555); //9~16bit

        return n;
    }

    //最大16bitの入力Ⅹ、yから32BitのZを得る
    //x、yは最小分割単位のグリッド上で左下から右にx、上にy移動の意味
    inline  uint64_t GetZ(uint64_t x, uint64_t y)
    {
        return (BitSeparate16(x) | (BitSeparate16(y) << 1));
    }

    //下記のコードで使うテーブル
    const unsigned char s_abyLog2[32] = {
     1,  2, 29,  3, 30, 20, 25,  4, 31, 23, 21, 11, 26, 13, 16,  5,
    32, 28, 19, 24, 22,  10, 12, 15, 27, 18,  9, 14, 17,  8,  7,  6
    };

    //32bitの整数のMSBの位置を1-indexedで得る
    //0x00000001の場合は1を返す
    //0x00800F00の場合は24を返す
    //0x00000000の場合は未定義
    inline constexpr uint64_t GetMsbPos(uint32_t uVal)
    {
        //msbを残してすべてのビッドを1に設定
        uVal |= (uVal >> 1);    //2bit文のor
        uVal |= (uVal >> 2);    //4bit文のor
        uVal |= (uVal >> 4);    //8bit文のor
        uVal |= (uVal >> 8);    //16bit文のor
        uVal |= (uVal >> 16);   //32bit文のor

        //msbのみを残す
        uVal >>= 1;//1ビット右シフト;
        uVal++;

        //2のべき乗を利用してインデックス計算
        const uint32_t MAGIC = 0x07D6E531;
        uVal = (MAGIC * uVal) >> 27;

        //結果をテーブルから取得
        return static_cast<int64_t>(s_abyLog2[uVal & 0x1F]);
    }
}


SpaceDivisionRayCast::SpaceDivisionRayCast()
{

}

//四分木空間分割させたいモデルを読み込む.被りがあった場合は入れない
//読み込んだモデルを四分木しておく//
 void SpaceDivisionRayCast::Load(Model* model)
{


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
            CollisionMesh::Triangle& triangle = model_divisions_.triangles.emplace_back();
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

    //箱全体の大きさが欲しいので
    this->volume_min_ = volume_min;
    this->volume_max_ = volume_max;

    DirectX::XMFLOAT3 box_size{
        volume_max.x - volume_min.x,
        volume_max.y - volume_min.y,
        volume_max.z - volume_min.z
    };

    // 空間分割
    //for (float x = volume_min.x; x < volume_max.x; x += cell_size.x) {
    //    for (float z = volume_min.z; z < volume_max.z; z += cell_size.y) {
    //        CollisionMesh::Area& area = model_divisions_[model].areas.emplace_back();
    //        area.bounding_box.Center =
    //        { 
    //            x + cell_size.x * 0.5f,
    //            volume_min.y + box_size_y * 0.5f,
    //            z + cell_size.y * 0.5f 
    //        };
    //        area.bounding_box.Extents = { cell_size.x * 0.5f, box_size_y * 0.5f, cell_size.x * 0.5f };
    //        //この中でトライアングルの登録のためのfor文を書くと
    //        //四回同じfor文を通ることになるのでそれはちょっと…
    //    }
    //}


    //四分木空間の作成
    DirectX::XMFLOAT3 center = {
        volume_min.x + (box_size.x * 0.5f),
        volume_min.y + (box_size.y * 0.5f),
        volume_min.z + (box_size.z * 0.5f)
    };

    //もし四分木のノード配列に内容があるなら消しておく
    {
        if (quad_tree_nodes_.size() > 0)quad_tree_nodes_.clear();

        QuadTreeNode::CreateQuadTree(center, box_size, QuadTreeNode::node_depth_, &quad_tree_nodes_);
    }

    //四分木空間のエリアをモデルディビジョンに保存する
    {
        int node_size = quad_tree_nodes_.size();
        QuadTreeNode* quad_tree = quad_tree_nodes_.data();
        for (int node_i = 0; node_i < node_size; node_i++)
        {
            for (int j = 0; j < node_size - 1; j++)
            {
                if (quad_tree[j].depth > quad_tree[j + 1].depth)
                {
                    std::swap(quad_tree[j], quad_tree[j + 1]);
                }
            }
        }
        for (int i = 0; i < node_size; i++) {
            model_divisions_.areas.emplace_back(quad_tree[i].area);
        }
    }

    size_t triangles_size = model_divisions_.triangles.size();
    const auto* triangle = model_divisions_.triangles.data();
    for (size_t i = 0; i < triangles_size; ++i)
    {
        DirectX::XMFLOAT3 tri_min, tri_max;
        tri_min =
        {
            (std::min)((std::min)(triangle[i].positions[0].x,triangle[i].positions[1].x),triangle[i].positions[2].x),
            (std::min)((std::min)(triangle[i].positions[0].y,triangle[i].positions[1].y),triangle[i].positions[2].y),
            (std::min)((std::min)(triangle[i].positions[0].z,triangle[i].positions[1].z),triangle[i].positions[2].z)
        };
        tri_max = 
        {
            (std::max)((std::max)(triangle[i].positions[0].x,triangle[i].positions[1].x),triangle[i].positions[2].x),
            (std::max)((std::max)(triangle[i].positions[0].y,triangle[i].positions[1].y),triangle[i].positions[2].y),
            (std::max)((std::max)(triangle[i].positions[0].z,triangle[i].positions[1].z),triangle[i].positions[2].z)
        };

        int areas_size = model_divisions_.areas.size();
        const auto* area = model_divisions_.areas.data();
        for (int area_i=0;area_i<areas_size;area_i++)
        {

            //エリアの中に頂点があるならエリアに保存する
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
                model_divisions_.areas.at(area_i).triangle_indices.emplace_back(i);
            }
        }
    }

}

//
// 四分木構造体内の情報をすべてクリアする
//
void SpaceDivisionRayCast::Clear()
{
    quad_tree_nodes_.clear();
    model_divisions_.areas.clear();
    model_divisions_.triangles.clear();
}

void SpaceDivisionRayCast::Reload(Model* model)
{
    this->Clear();
    this->Load(model);
}


//四分木空間分割でレイキャストする
//モデルのアドレスで読み込まれたものがある場合四分木空間分割レイキャストする
bool SpaceDivisionRayCast::RayCast(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
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

            ////小さいAABBとレイの交差判定を取る
            //const CollisionMesh& collision_mesh = model_divisions_[model];
            //int areas_size =collision_mesh.areas.size();
            //const CollisionMesh::Area* area = collision_mesh.areas.data();
            //for (int area_i = 0; area_i < areas_size; area_i++)
            //{
            //    float dist = distance;
            //    if (area[area_i].bounding_box.Intersects(vec_start, direction, dist))
            //    {
            //        //当たったAABBの中にあるメッシュの三角形とレイの当たり判定を取る
            //        int triangles_size = area[area_i].triangle_indices.size();
            //        const int* triangle_i = area[area_i].triangle_indices.data();
            //        const CollisionMesh::Triangle* triangle = collision_mesh.triangles.data();
            //        for (int areas_triangle_i = 0; areas_triangle_i < triangles_size; areas_triangle_i++)
            //        {
            //            DirectX::XMVECTOR triangle_a = DirectX::XMLoadFloat3(&triangle[triangle_i[areas_triangle_i]].positions[0]);
            //            DirectX::XMVECTOR triangle_b = DirectX::XMLoadFloat3(&triangle[triangle_i[areas_triangle_i]].positions[1]);
            //            DirectX::XMVECTOR triangle_c = DirectX::XMLoadFloat3(&triangle[triangle_i[areas_triangle_i]].positions[2]);
            //            dist = distance;
            //            if (DirectX::TriangleTests::Intersects(
            //                vec_start,
            //                direction,
            //                triangle_a,
            //                triangle_b,
            //                triangle_c,
            //                dist))
            //            {
            //                if (distance < dist)continue;
            //                distance = dist;
            //                hit_normal = triangle[triangle_i[areas_triangle_i]].normal;
            //                hit = true;
            //            }
            //        }
            //    }
            //    if (hit)break;
            //}

            //四分木のモートンコードで検索する
    {
        //レイキャストのxz面の最大値と最小値で四角形を作る
        float x_min = (std::min)(start.x, end.x);
        float z_min = (std::min)(start.z, end.z);
        float x_max = (std::max)(start.x, end.x);
        float z_max = (std::max)(start.z, end.z);

        //id検索の前にボックス基準の座標系にする
        float box_space_x_min = x_min - volume_min_.x;
        float box_space_z_min = z_min - volume_min_.z;
        float box_space_x_max = x_max - volume_min_.x;
        float box_space_z_max = z_max - volume_min_.z;

        if (box_space_x_min < 0)return false;
        if (box_space_x_max < 0)return false;
        if (box_space_z_min < 0)return false;
        if (box_space_z_max < 0)return false;

        const uint64_t id = GetCellId(box_space_x_min, box_space_x_max, box_space_z_min, box_space_z_max);
        const CollisionMesh::Area* area = model_divisions_.areas.data();

        const auto area_triangle_size = area[id].triangle_indices.size();
        if (area_triangle_size == 0)
            return false;
        const int* area_triangle = area[id].triangle_indices.data();
        const CollisionMesh::Triangle* triangle = model_divisions_.triangles.data();
        for (int triangle_i = 0; triangle_i < area_triangle_size; triangle_i++)
        {
            DirectX::XMFLOAT3 triangle_a=triangle[area_triangle[triangle_i]].positions[0];
            DirectX::XMFLOAT3 triangle_b=triangle[area_triangle[triangle_i]].positions[1];
            DirectX::XMFLOAT3 triangle_c=triangle[area_triangle[triangle_i]].positions[2];

            //もしトライアングルがレイに重なってないならcontinue;
            {
                float triangle_min_x = (std::min)((std::min)(triangle_a.x, triangle_b.x), triangle_c.x);
                float triangle_min_z = (std::min)((std::min)(triangle_a.z, triangle_b.z), triangle_c.z);
                float triangle_max_x = (std::max)((std::max)(triangle_a.x, triangle_b.x), triangle_c.x);
                float triangle_max_z = (std::max)((std::max)(triangle_a.z, triangle_b.z), triangle_c.z);

                if (triangle_min_x > x_max)continue;
                if (triangle_max_x < x_min)continue;
                if (triangle_min_z > z_max)continue;
                if (triangle_max_z < z_min)continue;
            }

            DirectX::XMVECTOR vec_triangle_a = DirectX::XMLoadFloat3(&triangle_a);
            DirectX::XMVECTOR vec_triangle_b = DirectX::XMLoadFloat3(&triangle_b);
            DirectX::XMVECTOR vec_triangle_c = DirectX::XMLoadFloat3(&triangle_c);
            float dist = distance;
            if (DirectX::TriangleTests::Intersects(
                vec_start,
                direction,
                vec_triangle_a,
                vec_triangle_b,
                vec_triangle_c,
                dist))
            {
                if (distance < dist)continue;
                distance = dist;
                hit_normal = triangle[area_triangle[triangle_i]].normal;
                hit = true;
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

void SpaceDivisionRayCast::DebugDraw(RenderContext&rc)
{
    PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
    ShapeRenderer* shape_renderer = Graphics::Instance().GetShapeRenderer();

    bool hit=false;
    //モデルを持ってなかったらしない

    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

    DirectX::XMFLOAT4 boxColor = { 0, 1, 0, 1 };
    const DirectX::XMFLOAT3 boxAngle = { 0, 0, 0 };

    DirectX::XMFLOAT4 polygonColor = { 0, 0, 1, 0.5f };
    size_t area_size = model_divisions_.areas.size();
    CollisionMesh::Area* area = model_divisions_.areas.data();
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
            ////モデルにあるすべての頂点情報とエリアにある頂点情報を確認する
            ////有るものだけを描画する
            //const CollisionMesh::Triangle* triangle = model_divisions_.triangles.data();
            //int areas_triangle_size = area[i].triangle_indices.size();
            //const int* areas_triangle = area[i].triangle_indices.data();
            //for (int areas_triangle_index = 0; areas_triangle_index < areas_triangle_size; areas_triangle_index++)
            //{
            //    primitiveRenderer->AddVertex(triangle[areas_triangle[areas_triangle_index]].positions[0], polygonColor);
            //    primitiveRenderer->AddVertex(triangle[areas_triangle[areas_triangle_index]].positions[1], polygonColor);
            //    primitiveRenderer->AddVertex(triangle[areas_triangle[areas_triangle_index]].positions[2], polygonColor);
            //}
            
        }
        shape_renderer->DrawBox(area[i].bounding_box.Center, boxAngle, area[i].bounding_box.Extents, boxColor);
    }

    //////コチラはモデルから取り出した全ての三角形を描画する
    ////// 三角形ポリゴン描画
    //if (all_draw_)
    //{
    //    //polygonColor = { 1,1,1,1 };
    //    //size_t size = model_divisions_[model].triangles.size();
    //    //CollisionMesh::Triangle* triangle = model_divisions_[model].triangles.data();
    //    //for (size_t i = 0; i < size; i++)
    //    //{
    //    //    primitiveRenderer->AddVertex(triangle[i].positions[0], polygonColor);
    //    //    primitiveRenderer->AddVertex(triangle[i].positions[1], polygonColor);
    //    //    primitiveRenderer->AddVertex(triangle[i].positions[2], polygonColor);
    //    //}
    //}

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


        ImGui::Separator();

        CollisionMesh collision_mesh = model_divisions_;
        int area_size = collision_mesh.areas.size();
        ImGui::InputInt("area_size", &area_size);
        int area_triangle_size = collision_mesh.triangles.size();
        ImGui::InputInt("area_triangle_size", &area_triangle_size);

        if (draw_box_ >= area_size)draw_box_ = draw_box_ - area_size;

        //int triangle_parsent = 0;
        //CollisionMesh::Area* area = collision_mesh.areas.data();
        //ImGui::BeginChild(ImGui::GetID("SpaceDivision"), ImVec2(300, 200), ImGuiWindowFlags_NoTitleBar);
        //{
        //    //for (int area_i = 0; area_i < area_size; area_i++)
        //    //{
        //    //    outs.str("");
        //    //    int triangle_in_area = area[area_i].triangle_indices.size();
        //    //    if (triangle_in_area <= 0)continue;
        //    //    outs << area_i << " : areas_triangles:" << triangle_in_area;
        //    //    ImGui::TextWrapped(outs.str().c_str());
        //    //    triangle_parsent += triangle_in_area;
        //    //}
        //}
        //ImGui::EndChild();

        //triangle_parsent = static_cast<int>((triangle_parsent / (area_triangle_size * 0.01f)));
        ////ImGui::InputInt("", &triangle_parsent);
        //outs.str("");
        //outs << triangle_parsent << ": in_box";
        //ImGui::TextWrapped(outs.str().c_str());

    }

    ImGui::End();
#endif
}

//
// 四分木を渡された深度分作成する
//
void SpaceDivisionRayCast::QuadTreeNode::CreateQuadTree(
    DirectX::XMFLOAT3 center,
    DirectX::XMFLOAT3 half_size,
    uint32_t depth,
    std::vector<QuadTreeNode>* node)
{
    QuadTreeNode parent;
    parent.center = center;
    parent.half_size = half_size;
    parent.depth = node_depth_ - depth;
    parent.is_leaf = (depth <= 0);

    //エリアを作る
    parent.area.bounding_box.Center = center;
    parent.area.bounding_box.Extents = {
        half_size.x*0.5f,
        half_size.y*0.5f,
        half_size.z*0.5f
    };


    //ノードをリストに追加
    if (node != nullptr)
    {
        node->emplace_back(parent);
    }

    //リーフノード(最後尾)なら再起終了
    if (parent.is_leaf)return;

    //四分木の各子ノードを生成
    DirectX::XMFLOAT3 quad_size = { half_size.x * 0.5f,half_size.y,half_size.z * 0.5f };
    const DirectX::XMFLOAT3 offsets[4]{
        {-quad_size.x * 0.5f,0.0f,-quad_size.z * 0.5f},   //左下
        {quad_size.x * 0.5f,0.0f,-quad_size.z * 0.5f},    //右下
        {-quad_size.x * 0.5f,0.0f,quad_size.z * 0.5f},    //左上
        {quad_size.x * 0.5f,0.0f,quad_size.z * 0.5f}      //右上
    };

    //子を再起処理で呼び出す
    for (int i = 0; i < 4; i++)
    {
        DirectX::XMVECTOR vec_child_offset = DirectX::XMLoadFloat3(&offsets[i]);
        DirectX::XMVECTOR vec_child_center =
            DirectX::XMVectorAdd(
                DirectX::XMLoadFloat3(&center),
                vec_child_offset);
        DirectX::XMFLOAT3 child_center;
        DirectX::XMStoreFloat3(&child_center, vec_child_center);

        CreateQuadTree(child_center, quad_size, depth-1, node);
    }
}
//
// 渡された座標がどのノードのIDの位置にあるかを算出する。
//
uint64_t SpaceDivisionRayCast::GetCellId(float x_min, float x_max, float z_min, float z_max)
{
    //オブジェクトの全体を計算
    const float box_x = volume_max_.x - volume_min_.x;
    float box_z = volume_max_.z - volume_min_.z;

    //単位長方形の幅を計算
    const float dx = box_x / quad_tree_calc::MaxSplit(static_cast<uint64_t>(QuadTreeNode::node_depth_));
    const float dz = box_z / quad_tree_calc::MaxSplit(static_cast<uint64_t>(QuadTreeNode::node_depth_));
    constexpr int64_t max_split_num = quad_tree_calc::MaxSplit(static_cast<uint64_t>(QuadTreeNode::node_depth_));

    //引数の座標を0～MAX_SPLIT-1のグリッド座標へ変換
    int64_t ix_min = static_cast<int64_t>(x_min / dx);
    int64_t iz_min = static_cast<int64_t>(z_min / dz);
    int64_t ix_max = static_cast<int64_t>(x_max / dx);
    int64_t iz_max = static_cast<int64_t>(z_max / dz);
    if (ix_min < 0 || ix_max >= max_split_num)
    {
        ix_min = (ix_min < 0) ? 0 : (ix_min >= max_split_num) ? max_split_num - 1 : ix_min;
        ix_max = (ix_max < 0) ? 0 : (ix_max >= max_split_num) ? max_split_num - 1 : ix_max;
    }
    if (iz_min < 0 || iz_max >= max_split_num)
    {
        iz_min = (iz_min < 0) ? 0 : (iz_min >= max_split_num) ? max_split_num - 1 : iz_min;
        iz_max = (iz_max < 0) ? 0 : (iz_max >= max_split_num) ? max_split_num - 1 : iz_max;
    }

    //0<=ix_min,iz_min,ix_max,iz_max<max_split_num<=2^16
    const uint64_t x = ix_min ^ ix_max;
    const uint64_t z = iz_min ^ iz_max;
    const uint64_t xz = x | z;
    const uint64_t z_valume = quad_tree_calc::GetZ(ix_min, iz_min);
    //xzの値が0なら
    if (xz == 0)
    {
        return offset[QuadTreeNode::node_depth_] + z_valume;
    }
    //1-indexed,assume xz<2^32
    uint64_t msb_pos = quad_tree_calc::GetMsbPos(static_cast<uint32_t>(xz));
    uint64_t l = QuadTreeNode::node_depth_ - msb_pos;
    return offset[l] + (z_valume >> (msb_pos << 1));


}