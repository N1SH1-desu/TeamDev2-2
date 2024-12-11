#include "function.h"
#include "InputMouse.h"

//�}�E�X��position���擾
const DirectX::XMFLOAT2 function::Get_Cursor_Pos()
{
	POINTS mousePosition = InputMouse::Instance().GetPosition();
	return DirectX::XMFLOAT2(mousePosition.x, mousePosition.y);
}


//�}�E�X�Ƌ�`�̓����蔻��
 bool function::Click_Colision(
	DirectX::XMFLOAT2 pos,
	DirectX::XMFLOAT2 size
)
{
	 POINTS mousePosition = InputMouse::Instance().GetPosition();

	if (mousePosition.x < pos.x)return false;
	if (mousePosition.x > pos.x + size.x) return false;
	if (mousePosition.y < pos.y)return false;
	if (mousePosition.y > pos.y + size.y)return false;

	return true;
}


//��`�̓����蔻��
bool function::Box_Colision(
	DirectX::XMFLOAT2 posA,
	DirectX::XMFLOAT2 sizeA,
	DirectX::XMFLOAT2 posB,
	DirectX::XMFLOAT2 sizeB
)
{
	float leftA = posA.x;
	float rightA = posA.x + sizeA.x;
	float topA = posA.y;
	float bottomA = posA.y + sizeA.y;

	float leftB = posB.x;
	float rightB = posB.x + sizeB.x;
	float topB = posB.y;
	float bottomB = posB.y + sizeB.y;

	if (rightA < leftB) return false;
	if (leftA > rightB) return false;
	if (bottomA < topB) return false;
	if (topA > bottomB) return false;

	return true;
}

