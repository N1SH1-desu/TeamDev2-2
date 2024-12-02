#pragma once

#include <DirectXMath.h>
#include "Model.h"

//ヒット結果
struct  HitResult
{
	DirectX::XMFLOAT3 position = { 0,0,0 };//レイとポリゴンの交点
	DirectX::XMFLOAT3 normal = { 0,0,0 };//レイとポリゴンの法線ベクトル
	float distance = 0.0f;//レイの終点から交点の距離
	int  materialIndex = -1;//衝突したポリゴンのマテリアル番号
	DirectX::XMFLOAT3 rotation = { 0,0,0 };//モデルの回転
};

// コリジョン
class Collision
{
public:
	// レイキャスト
	static bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const DirectX::XMFLOAT4X4& worldTransform,
		const Model* model,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal);
	//球と球の交差判定
	static bool IntersectSphereVsSphere
	(const DirectX::XMFLOAT3& positionA, float radiusA,
		const DirectX::XMFLOAT3& positionB, float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	//円柱と円柱の交差判定
	static bool InteresectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);
	//球と円柱の交差判定
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);
	//レイとモデルの交差判定
	static bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

};
