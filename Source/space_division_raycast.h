#pragma once
#include<DirectXCollision.h>
#include<d3d11.h>

#include<map>

#include"Model.h"
#include"RenderContext.h"

using namespace std;
using namespace DirectX;

//空間分割レイキャスト用クラス
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

    //空間分割したいモデルを登録する。
    void Load(Model* model,DirectX::XMFLOAT4X4 world_transform);

    //空間分割したモデルでレイキャストする
	bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		Model* model,
		DirectX::XMFLOAT3& hit_position,
		DirectX::XMFLOAT3& hit_normal);

	//確認
	void DebugDraw(RenderContext& rc ,Model*model);
	void DrowImgui();
};