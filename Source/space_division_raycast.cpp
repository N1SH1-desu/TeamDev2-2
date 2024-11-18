#include"space_division_raycast.h"

#include"Graphics.h"
#include"ShapeRenderer.h"
#include"PrimitiveRenderer.h"

SpaceDivisionRaycast::SpaceDivisionRaycast()
{

}

//�l���؋�ԕ��������������f����ǂݍ���.��肪�������ꍇ�͓���Ȃ�
//�ǂݍ��񂾃��f�����l���؂��Ă���
void SpaceDivisionRaycast::Load(Model* model,DirectX::XMFLOAT4X4 world_transform)
{
    if (models_.size() > 0)
    {
        int size = models_.size();
        for (int i = 0; i < size; i++)
        {
            if (models_[i] == model)return;
        }
    }

    //���f���̑S��AABB�i�[�p�ϐ�
    DirectX::XMVECTOR VolumeMin = DirectX::XMVectorReplicate(FLT_MAX);
    DirectX::XMVECTOR VolumeMax = DirectX::XMVectorReplicate(-FLT_MAX);

    //���_�f�[�^�����[���h��ԕϊ����A�O�p�`�f�[�^���쐬
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

        //���_�f�[�^�����[���h��ԕϊ�
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

            //�@���x�N�g���̎Z�o
            DirectX::XMVECTOR N = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(B, A), DirectX::XMVectorSubtract(C, A));
            if (DirectX::XMVector3Equal(N, DirectX::XMVectorZero()))
            {
                //�ʂ��\���ł��Ȃ��ꍇ�͏��O
                continue;
            }
            N = DirectX::XMVector3Normalize(N);

            //���f���̎O�p�`�f�[�^���i�[
            CollisionMesh::Triangle& triangle = model_divisions_[model].triangles.emplace_back();
            DirectX::XMStoreFloat3(&triangle.positions[0], A);
            DirectX::XMStoreFloat3(&triangle.positions[1], B);
            DirectX::XMStoreFloat3(&triangle.positions[2], C);
            DirectX::XMStoreFloat3(&triangle.normal, N);

            //���f���S�̂�AABB���v�Z
            VolumeMin = DirectX::XMVectorMin(VolumeMin, A);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, B);
            VolumeMin = DirectX::XMVectorMin(VolumeMin, C);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, A);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, B);
            VolumeMax = DirectX::XMVectorMax(VolumeMax, C);
        }
    }

    //���f���S�̂�AABB
    DirectX::XMFLOAT3 volume_min, volume_max;
    DirectX::XMStoreFloat3(&volume_min, VolumeMin);
    DirectX::XMStoreFloat3(&volume_max, VolumeMax);



    //���f���S�̂�AABB����XZ���ʂɎw��̃T�C�Y�ŕ������ꂽ�R���W�����G���A���쐬����
    for (float x = volume_min.x; x < volume_max.x; x += cell_size_)
    {
        for (float z = volume_min.z; z < volume_max.z; z += cell_size_)
        {
            //�������R���W�����G���A��AABB���Z�o
            CollisionMesh::Area& area = model_divisions_[model].areas.emplace_back();
            area.boundingBox.Center.x = x;
            area.boundingBox.Center.y = 0;
            area.boundingBox.Center.z = z;
            area.boundingBox.Extents.x =  area.boundingBox.Extents.z = cell_size_*0.5f;
            area.boundingBox.Extents.y = 10.0f;
            //AABB�ɏ�������O�p�`�𒊏o


            //AABB�ɏ�������O�p�`�𒊏o
            for (const  CollisionMesh::Triangle& triangle : model_divisions_[model].triangles)
            {
                DirectX::XMVECTOR triangle_A = DirectX::XMLoadFloat3(&triangle.positions[0]);
                DirectX::XMVECTOR triangle_B = DirectX::XMLoadFloat3(&triangle.positions[1]);
                DirectX::XMVECTOR triangle_C = DirectX::XMLoadFloat3(&triangle.positions[2]);
                if (area.boundingBox.Intersects(
                    triangle_A,
                    triangle_B,
                    triangle_C
                ))
                {
                    area.triangleIndices.emplace_back();
                }
            }
        }
    }


    models_.emplace_back(model);

}

//�l���؋�ԕ����Ń��C�L���X�g����
//���f���̃A�h���X�œǂݍ��܂ꂽ���̂�����ꍇ�l���؋�ԕ������C�L���X�g����
bool SpaceDivisionRaycast::RayCast(
    Model* model,
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
    float dist = distance;

    if (distance <= 0.0f)return false;

    int models_size = models_.size();
    for (int i = 0; i < models_size; i++)
    {
        if (models_[i] == model)
        {
            //������AABB�ƃ��C�̌�����������
            const CollisionMesh& collision_mesh = model_divisions_[model];
            int areas_size =collision_mesh.areas.size();
            const CollisionMesh::Area* area = collision_mesh.areas.data();
            //for (int area_i = 0; area_i < areas_size; area_i++)
            //{
            //    if (area[area_i].boundingBox.Intersects(vec_start, direction, dist))
            //    {
            //        //��������AABB�̒��ɂ��郁�b�V���̎O�p�`�ƃ��C�̓����蔻������
            //        int triangles_size = area[area_i].triangleIndices.size();
            //        const CollisionMesh::Triangle* triangle = collision_mesh.triangles.data();
            //        for (int triangle_i = 0; triangle_i < triangles_size; triangle_i++)
            //        {
            //            DirectX::XMVECTOR triangle_a = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[0]);
            //            DirectX::XMVECTOR triangle_b = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[1]);
            //            DirectX::XMVECTOR triangle_c = DirectX::XMLoadFloat3(&triangle[triangle_i].positions[2]);
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
            //                hit_normal = triangle[triangle_i].normal;
            //                hit = true;
            //            }
            //        }
            //    }
            //}

            for (const CollisionMesh::Area& area : collision_mesh.areas)
            {
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
            }

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

void SpaceDivisionRaycast::DebugDraw(RenderContext&rc,Model*model)
{
    PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();


    int size = models_.size();
    bool hit=false;
    //���f���������ĂȂ������炵�Ȃ�
    for (int i = 0; i < size; i++)if (models_[i] == model)hit = true;
    if (!hit)return;

    ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

    ShapeRenderer* shape_renderer = Graphics::Instance().GetShapeRenderer();
    const DirectX::XMFLOAT4 boxColor = { 0, 1, 0, 1 };
    const DirectX::XMFLOAT3 boxAngle = { 0, 0, 0 };

    const DirectX::XMFLOAT4 polygonColor = { 1, 0, 0, 0.5f };

    size_t area_size = model_divisions_[model].areas.size();
    CollisionMesh::Area* area = model_divisions_[model].areas.data();
    for (size_t i=0;i<area_size;i++)
    {
        int triangles_size = area[i].triangleIndices.size();
        const CollisionMesh::Triangle* triangle = model_divisions_[model].triangles.data();
        for (int triangle_i = 0; triangle_i < triangles_size; triangle_i++)
        {
            primitiveRenderer->AddVertex(triangle[triangle_i].positions[0], polygonColor);
            primitiveRenderer->AddVertex(triangle[triangle_i].positions[1], polygonColor);
            primitiveRenderer->AddVertex(triangle[triangle_i].positions[2], polygonColor);

        }
        primitiveRenderer->Render(dc, rc.camera->GetView(), rc.camera->GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        //shape_renderer->DrawBox(pos, boxAngle, area[i].boundingBox.Extents, boxColor);
        shape_renderer->DrawBox(area[i].boundingBox.Center, boxAngle, area[i].boundingBox.Extents, boxColor);
    }
    shape_renderer->Render(dc, rc.camera->GetView(), rc.camera->GetProjection());

    //// �O�p�`�|���S���`��
    //for (CollisionMesh::Triangle& triangle : model_divisions_[model].triangles)
    //{
    //    primitiveRenderer->AddVertex(triangle.positions[0], polygonColor);
    //    primitiveRenderer->AddVertex(triangle.positions[1], polygonColor);
    //    primitiveRenderer->AddVertex(triangle.positions[2], polygonColor);
    //}
    //primitiveRenderer->Render(dc, rc.camera->GetView(), rc.camera->GetProjection(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}