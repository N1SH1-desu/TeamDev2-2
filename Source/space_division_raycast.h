#pragma once
#include<DirectXCollision.h>
#include<d3d11.h>

#include<map>

#include"Model.h"
#include"RenderContext.h"

using namespace std;
using namespace DirectX;

//��ԕ������C�L���X�g�p�N���X
class SpaceDivisionRayCast
{
private:
    vector<Model*> models_;
    
	struct CollisionMesh
	{
		struct Triangle
		{
			DirectX::XMFLOAT3	positions[3];
			DirectX::XMFLOAT3	normal;
		};
		struct Area
		{
			DirectX::BoundingBox	boundingBox;
			std::vector<int>		triangleIndices;
		};

		std::vector<Triangle>	triangles;
		std::vector<Area>		areas;
	};
	map<Model*, CollisionMesh> model_divisions_;

	const int cell_size_ = 2;
	int draw_box_=0;
	bool all_draw_ = false;

public:
    SpaceDivisionRayCast();
	~SpaceDivisionRayCast() { models_.clear(); model_divisions_.clear(); }

    //��ԕ������������f����o�^����B
    void Load(Model* model,DirectX::XMFLOAT4X4 world_transform);

    //��ԕ����������f���Ń��C�L���X�g����
	bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		Model* model,
		DirectX::XMFLOAT3& hit_position,
		DirectX::XMFLOAT3& hit_normal);

	//�m�F
	void DebugDraw(RenderContext& rc ,Model*model);
	void DrowImgui();
};