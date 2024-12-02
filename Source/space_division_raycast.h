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
			DirectX::BoundingBox	bounding_box;
			std::vector<int>		triangle_indices;
		};

		std::vector<Triangle>	triangles;
		std::vector<Area>		areas;


	};
	map<Model*, CollisionMesh> model_divisions_;

	//矩形エリアを表す構造体
	struct Rect {
		float x_min, x_max, z_min, z_max;
	};
	//四分木ノード
	struct QuadTreeNode {
		Rect bounds;	//この四分木がカバーするエリア

		std::vector<CollisionMesh::Area*>areas;

		QuadTreeNode(const Rect& bounds) :bounds(bounds) {}
		~QuadTreeNode() { areas.clear(); }


	};
	vector<QuadTreeNode>node_index;
	map<Model*, QuadTreeNode>model_quad_parent_;

	const int node_depth_ = 16;
	const int offset[16] = 
	{ 
		0, 1, 5, 21,
		85, 341, 1365, 5461,
		21845, 87381, 349525, 1398101,
		5592405, 22369621, 89478485, 357913941 
	};


	int draw_box_=0;
	bool all_draw_ = false;

	

public:
    SpaceDivisionRayCast();
	~SpaceDivisionRayCast() { models_.clear(); model_divisions_.clear(); }

    //空間分割したいモデルを登録する。
    void Load(Model* model);

    //空間分割したモデルでレイキャストする
	bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		Model* model,
		DirectX::XMFLOAT3& hit_position,
		DirectX::XMFLOAT3& hit_normal);
	int GetNodeId(float x_min, float x_max, float z_min, float z_max);

	//確認
	void DebugDraw(RenderContext& rc ,Model*model);
	void DrowImgui();
};