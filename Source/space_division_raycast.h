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

	//�l���؃m�[�h
	//�[�x���Ƃ̃��[�g���̃I�t�Z�b�g
	static constexpr int offset[16] = 
	{ 
		0, 1, 5, 21,
		85, 341, 1365, 5461,
		21845, 87381, 349525, 1398101,
		5592405, 22369621, 89478485, 357913941 
	};
	DirectX::XMFLOAT3 volume_min_{}, volume_max_{};
	//�����蔻�����肽����_��m�[�h�ԍ������
	uint64_t GetCellId(float x_min, float x_max, float z_min, float z_max);
	
	struct QuadTreeNode {
		DirectX::XMFLOAT3 center;	//���̎l���؂̒��S
		DirectX::XMFLOAT3 half_size; //�l���؋�Ԃ�
		float depth;
		bool is_leaf;				//depth����0�Ȃ�true;
		//�ő�[�x
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

		//�l���؋�Ԃ��쐬����֐�
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

    //��ԕ������������f����o�^����B
    void Load(Model* model);
	//���݂̋�ԕ������폜
	void clear();

    //��ԕ����������f���Ń��C�L���X�g����
	bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hit_position,
		DirectX::XMFLOAT3& hit_normal);

	//�m�F
	void DebugDraw(RenderContext& rc ,Model*model);
	void DrowImgui();
};