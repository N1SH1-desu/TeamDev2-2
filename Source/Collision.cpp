#include <DirectXCollision.h>
#include "Collision.h"

// レイキャスト
bool Collision::RayCast(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const DirectX::XMFLOAT4X4& worldTransform,
	const Model* model,
	DirectX::XMFLOAT3& hitPosition,
	DirectX::XMFLOAT3& hitNormal)
{
	bool hit = false;

	// 始点と終点からレイのベクトルと長さを求める
	DirectX::XMVECTOR WorldRayStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldRayEnd = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldRayEnd, WorldRayStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	float nearestDist = DirectX::XMVectorGetX(WorldRayLength);
	if (nearestDist <= 0.0f) return false;

	DirectX::XMMATRIX ParentWorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);

	// モデル内の全てのメッシュと交差判定を行う
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// メッシュのワールド行列を求める
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
		DirectX::XMMATRIX GlobalTransform = DirectX::XMLoadFloat4x4(&node.globalTransform);
		DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixMultiply(GlobalTransform, ParentWorldTransform);

		// レイをメッシュのローカル空間に変換する
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
		DirectX::XMVECTOR LocalRayStart = DirectX::XMVector3Transform(WorldRayStart, InverseWorldTransform);
		DirectX::XMVECTOR LocalRayEnd = DirectX::XMVector3Transform(WorldRayEnd, InverseWorldTransform);
		DirectX::XMVECTOR LocalRayVec = DirectX::XMVectorSubtract(LocalRayEnd, LocalRayStart);
		DirectX::XMVECTOR LocalRayDirection = DirectX::XMVector3Normalize(LocalRayVec);
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(LocalRayVec));

		// メッシュ内の全ての三角形と交差判定を行う
		for (size_t i = 0; i < mesh.indices.size(); i += 3)
		{
			// 三角形の頂点座標を取得
			const ModelResource::Vertex& a = mesh.vertices.at(mesh.indices.at(i + 0));
			const ModelResource::Vertex& b = mesh.vertices.at(mesh.indices.at(i + 1));
			const ModelResource::Vertex& c = mesh.vertices.at(mesh.indices.at(i + 2));

			DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
			DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
			DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

			// 三角形の交差判定
			if (DirectX::TriangleTests::Intersects(LocalRayStart, LocalRayDirection, A, B, C, distance))
			{
				// メッシュのローカル空間での交点を求める
				DirectX::XMVECTOR LocalHitVec = DirectX::XMVectorScale(LocalRayDirection, distance);
				DirectX::XMVECTOR LocalHitPosition = DirectX::XMVectorAdd(LocalRayStart, LocalHitVec);

				// メッシュのローカル空間での交点をワールド空間に変換する
				DirectX::XMVECTOR WorldHitPosition = DirectX::XMVector3Transform(LocalHitPosition, WorldTransform);

				// ワールド空間でのレイの始点から交点までの距離を求める
				DirectX::XMVECTOR WorldHitVec = DirectX::XMVectorSubtract(WorldHitPosition, WorldRayStart);
				DirectX::XMVECTOR WorldHitDist = DirectX::XMVector3Length(WorldHitVec);
				float worldHitDist = DirectX::XMVectorGetX(WorldHitDist);

				// 交点までの距離が今までに計算した交点より近い場合は交差したと判定する
				if (worldHitDist <= nearestDist)
				{
					// メッシュのローカル空間での三角形の法線ベクトルを算出
					DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
					DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
					DirectX::XMVECTOR LocalHitNormal = DirectX::XMVector3Cross(AB, BC);

					// メッシュのローカル空間での法線ベクトルをワールド空間に変換する
					DirectX::XMVECTOR WorldHitNormal = DirectX::XMVector3TransformNormal(LocalHitNormal, WorldTransform);

					// 三角形の裏表判定（内積の結果がマイナスならば表向き）
					DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(LocalRayDirection, LocalHitNormal);
					float dot = DirectX::XMVectorGetX(Dot);
					if (dot < 0)
					{
						// 交差したのでヒット情報を格納
						DirectX::XMStoreFloat3(&hitNormal, DirectX::XMVector3Normalize(WorldHitNormal));
						DirectX::XMStoreFloat3(&hitPosition, WorldHitPosition);

						// 交点までの距離を更新する
						nearestDist = worldHitDist;
						hit = true;
					}
				}
			}
		}
	}
	return hit;
}
//球と球の交差判定

bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
{
	//A-Bの単位ベクトルを算出
	DirectX::XMVECTOR PotisionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PotisionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PotisionB, PotisionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//距離判定
	float range = radiusA + radiusB;
	if (lengthSq > range) {
		return false;
	}

	//AがBを押し出す
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	Vec = DirectX::XMVectorAdd(Vec, PotisionA);

	DirectX::XMStoreFloat3(&outPositionB, Vec);


	return true;
}

bool Collision::InteresectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA, float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB, float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	//Aの足元がBの頭より上なら当たってない
	if (positionA.y > heightB + positionB.y) {
		return false;
	}
	//Aの頭がBの足元より下なら当たってない
	if (heightA + positionA.y < positionB.y) {
		return false;
	}
	//XZ平面での範囲チェック
	DirectX::XMFLOAT2 posA = { positionA.x,positionA.z };
	DirectX::XMFLOAT2 posB = { positionB.x,positionB.z };
	DirectX::XMVECTOR PoA = DirectX::XMLoadFloat2(&posA);
	DirectX::XMVECTOR PoB = DirectX::XMLoadFloat2(&posB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PoB, PoA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);
	float range = radiusA + radiusB;
	if (lengthSq > range) {
		return false;
	}
	//AがBを押し出す
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	Vec = DirectX::XMVectorAdd(Vec, PoA);
	DirectX::XMFLOAT2 loadpos;
	DirectX::XMStoreFloat2(&loadpos, Vec);

	outPositionB.x = loadpos.x;
	outPositionB.y = positionB.y;
	outPositionB.z = loadpos.y;



	return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
	//Aの足元がBの頭より上なら当たってない
	if (spherePosition.y > cylinderHeight + cylinderPosition.y) {
		return false;
	}
	//Aの頭がBの足元より下なら当たってない
	if (cylinderHeight + spherePosition.y < cylinderPosition.y) {
		return false;
	}
	//XZ平面での範囲チェック
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range)return false;

	vx /= distXZ;
	vz /= distXZ;
	outCylinderPosition.x = spherePosition.x + (vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + (vz * range);

	//DirectX::XMFLOAT2 posA = { spherePosition.x,spherePosition.z };
	//DirectX::XMFLOAT2 posB = { cylinderPosition.x,cylinderPosition.z };
	//DirectX::XMVECTOR PoA = DirectX::XMLoadFloat2(&posA);
	//DirectX::XMVECTOR PoB = DirectX::XMLoadFloat2(&posB);
	//DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PoB, PoA);
	//DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	//float lengthSq;
	//DirectX::XMStoreFloat(&lengthSq, LengthSq);
	//float range = 0;
	//if (cylinderPosition.y <= spherePosition.y &&
	//	spherePosition.y <= cylinderPosition.y + cylinderHeight) {
	//	range = sphereRadius + cylinderRadius;
	//}
	//else
	//{
	//	if (spherePosition.y > cylinderPosition.y + cylinderHeight) {
	//		float subHeight = spherePosition.y - (cylinderPosition.y + cylinderHeight);
	//		range = cylinderRadius + sqrtf(sphereRadius * sphereRadius - subHeight * subHeight);
	//	}
	//}
	//
	//if (lengthSq > range) {
	//	return false;
	//}
	//Vec = DirectX::XMVector3Normalize(Vec);
	//Vec = DirectX::XMVectorScale(Vec, range);
	//Vec = DirectX::XMVectorAdd(Vec, PoA);
	//DirectX::XMFLOAT2 ResultPos;
	//DirectX::XMStoreFloat2(&ResultPos, Vec);
	//
	//outCylinderPosition.x = ResultPos.x;
	//outCylinderPosition.y = cylinderPosition.y;
	//outCylinderPosition.z = ResultPos.y;



	return true;
}

//レイとモデルの交差判定
bool Collision::IntersectRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
{
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	//ワールド空間のレイの長さ
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes()) {
		//メッシュノード取得
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		//レイをワールド空間からローカル空間へ変換
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		//レイの長さ
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//三角形との交差判定
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;

		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets) {
			for (UINT i = 0; i < subset.indexCount; i += 3) {
				UINT index = subset.startIndex + i;
				//三角形の頂点を抽出
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//三角形の三辺ベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//三角形の法線ベクトルを算出
				DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

				//内積の結果がプラスならば裏向き	
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
				float dott;
				DirectX::XMStoreFloat(&dott, Dot);
				if (dott >= 0.0f)continue;

				//	レイと平面の交点を算出
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(SA, N), Dot);
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x<.0f || x>neart) continue;//交点までの距離が今までに計算した最近距離より大きい時はスキップ	

				DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

				//交点が三角形の内側にあるか判定
				//1つ目
				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;

				//2つめ
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;

				//３つめ
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f)continue;
				//最近距離を更新
				neart = x;
				//交点と法線を更新
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0) {
			//ローカル空間からワールド空間へ変換
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			//ヒット情報保存
			if (result.distance > distance) {
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}

	//if (end.y < 0.0f) {
	//	result.position.x = end.x;
	//	result.position.y = 0.0f;
	//	result.position.z = end.z;
	//	result.normal.x = 0.0f;
	//	result.normal.y = 1.0f;
	//	result.normal.z = 0.0f;
	//	return true;
	//}
	return hit;
}
