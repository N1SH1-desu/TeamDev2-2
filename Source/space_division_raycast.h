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
	CollisionMesh model_divisions_;

	//四分木ノード
	//深度ごとのモートンのオフセット
	static constexpr int offset[16] = 
	{ 
		0, 1, 5, 21,
		85, 341, 1365, 5461,
		21845, 87381, 349525, 1398101,
		5592405, 22369621, 89478485, 357913941 
	};
	DirectX::XMFLOAT3 volume_min_{}, volume_max_{};
	//当たり判定を取りたい二点空ノード番号を取る
	uint64_t GetCellId(float x_min, float x_max, float z_min, float z_max);
	
	struct QuadTreeNode {
		DirectX::XMFLOAT3 center;	//この四分木の中心
		DirectX::XMFLOAT3 half_size; //四分木空間の
		float depth;
		bool is_leaf;				//depth＝＝0ならtrue;
		//最大深度
		static const int node_depth_ = 5;

		CollisionMesh::Area area;

		QuadTreeNode()
			:center{0,0,0}
			,half_size{0,0,0}
			,depth{0}
			,is_leaf{false}
		{
		}
		~QuadTreeNode() {}

		//四分木空間を作成する関数
		static void CreateQuadTree(
			DirectX::XMFLOAT3 center,
			DirectX::XMFLOAT3 half_size,
			uint32_t depth,
			std::vector<QuadTreeNode>* node = {});

	};
	vector<QuadTreeNode>quad_tree_nodes_;



	int draw_box_=0;
	bool all_draw_ = false;
public:
    SpaceDivisionRayCast();
	~SpaceDivisionRayCast() { this->clear(); }

    //空間分割したいモデルを登録する。
    void Load(Model* model);
	//現在の空間分割を削除
	void clear();

    //空間分割したモデルでレイキャストする
	bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hit_position,
		DirectX::XMFLOAT3& hit_normal);

	//確認
	void DebugDraw(RenderContext& rc ,Model*model);
	void DrowImgui();
};