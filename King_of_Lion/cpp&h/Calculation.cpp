// ------------------------------------------------------------------------------------------
//
// Calculation�����̐���[calculation.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "Calculation.h"

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �O�ς̌v�Z
// ------------------------------------------------------------------------------------------
float Cross_product(
	D3DXVECTOR3 VecA,
	D3DXVECTOR3 VecB)
{
	return VecA.z * VecB.x - VecA.x * VecB.z;
}

// ------------------------------------------------------------------------------------------
// ��ʂ̎l�p�`�̒��ɓ����Ă��邩���Ȃ���
// ------------------------------------------------------------------------------------------
bool SquareBottom_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
)
{
	// �ϐ��錾
	D3DXVECTOR3 SquareDiff[4];
	D3DXVECTOR3 WorldDiff[4];
	float		fCross_p[4];
	bool		bCross = false;

	// ���̋���
	SquareDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// ��̋���
	SquareDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// �E�̋���
	SquareDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// ���̋���
	SquareDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// ���̋���
	WorldDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// ��̋���
	WorldDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// �E�̋���
	WorldDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// ���̋���
	WorldDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// �O�ς̒����O��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// �O��
		fCross_p[nCnt] = Cross_product(SquareDiff[nCnt], WorldDiff[nCnt]);

		// �O�ς̔��� //
		// ��
		if (fCross_p[nCnt] > 0)
		{
			bCross = true;
		}

		// �O
		else
		{
			return false;
		}
	}

	// �����O����Ԃ�
	return bCross;
}

// ------------------------------------------------------------------------------------------
// ���ʂ̎l�p�`�̒��ɓ����Ă��邩���Ȃ���
// ------------------------------------------------------------------------------------------
bool SquareSide_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int nBlock_W,
	int nBlock_H,
	D3DXVECTOR3 worldpos)
{
	// �����͈̔�
	if (worldpos.y < pos.y + size.y * nBlock_H &&
		worldpos.y >= pos.y)
	{
		// ���͈̔�
		if (worldpos.x > pos.x - size.x * 0.5f * nBlock_W * sinf(rot.y) &&
			worldpos.x < pos.x + size.x * 0.5f * nBlock_W * sinf(rot.y))
		{
			return true;
		}
		// ���͈̔�
		if (worldpos.z > pos.z - size.x * 0.5f * nBlock_W * cosf(rot.y) &&
			worldpos.z < pos.z + size.x * 0.5f * nBlock_W * cosf(rot.y))
		{
			return true;
		}
	}


	// �����O����Ԃ�
	return false;
}

// ------------------------------------------------------------------------------------------
// ���W���m�̍��v�Z
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 Difference_Between(
	D3DXVECTOR3 posA,
	D3DXVECTOR3 posB)
{
	// ��
	D3DXVECTOR3 Diff;

	//���W���m�̍��v�Z
	Diff = posA - posB;

	return Diff;
}

// ------------------------------------------------------------------------------------------
// �~�̑����Z�v�Z
// ------------------------------------------------------------------------------------------
float Addition_Circle(
	float Length_x, 
	float Length_y, 
	float Length_z)
{
	// �ϐ��錾
	float fLengthX = 0;
	float fLengthY = 0;
	float fLengthZ = 0;
	float Lenght = 0;

	// �~�̌v�Z
	fLengthX = Length_x * Length_x;
	fLengthY = Length_y * Length_y;
	fLengthZ = Length_z * Length_z;

	// �~���m�̑����Z
	Lenght = fLengthX + fLengthY + fLengthZ;

	return Lenght;
}

// ------------------------------------------------------------------------------------------
// �~�̓����蔻��
// ------------------------------------------------------------------------------------------
bool Collision_Circle(D3DXVECTOR3 Me, float fMeSize, D3DXVECTOR3 Opponent, float fOppSize)
{
	// �ϐ��錾
	float fRange;		// �͈�
	float fDistance;	// ����

						// �͈͂̌v�Z
	fRange = fMeSize * fMeSize + fOppSize * fOppSize;

	// ���ۂ̋����̌v�Z
	fDistance =
		(Me.x - Opponent.x) * (Me.x - Opponent.x) +
		(Me.y - Opponent.y) * (Me.y - Opponent.y) +
		(Me.z - Opponent.z) * (Me.z - Opponent.z);

	// �������Ă��邩���Ȃ�����Ԃ�
	return (fRange >= fDistance) ? true : false;
}

// ------------------------------------------------------------------------------------------
// ��]�̌��E�v�Z
// ------------------------------------------------------------------------------------------
float Rot_One_Limit(float fRot)
{
	// -3.14�`3.14�ɉ�]�������Œ肳����
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;		// -3.14����
	}

	else if (fRot > D3DX_PI)
	{
		fRot += -D3DX_PI * 2;	// 3.14����
	}

	return fRot;
}
// ------------------------------------------------------------------------------------------
// �X�N���[�����W�����[���h���W�ɕϊ�
// ------------------------------------------------------------------------------------------
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,	// �}�E�X�|�C���g
	float Sx,				// �X�N���[��X���W
	float Sy,				// �X�N���[��Y���W
	float fZ,			// �ˉe��Ԃł�Z�l�i0�`1�j
	float Screen_w,		// �X�N���[���̉���
	float Screen_h,		// �X�N���[���̍���
	D3DXMATRIX* mtxView,// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj)	// �v���W�F�N�V�����}�g���b�N�X
{
	// �e�s��̋t�s����Z�o //
	// �ϐ��錾
	D3DXMATRIX mtxInvViex;		// �v�Z�p�t�s��r���[�}�g���b�N�X
	D3DXMATRIX mtxInvPrj;		// �v�Z�p�t�s��v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxVP;			// �v�Z�p�r���[�|�b�g
	D3DXMATRIX mtxInvViexport;	// �v�Z�p�t�s��r���[�|�b�g
	D3DXMATRIX mtxChange;		// ���W�ϊ��p�}�g���b�N�X

								// �r���[�|�b�g�̍s��쐬
	D3DXMatrixIdentity(&mtxVP);	// ������
	mtxVP._11 = Screen_w / 2.0f;
	mtxVP._22 = -Screen_h / 2.0f;
	mtxVP._41 = Screen_w / 2.0f;
	mtxVP._42 = Screen_h / 2.0f;

	// �t�s��̌v�Z
	D3DXMatrixInverse(&mtxInvViex, NULL, mtxView);		// �r���[�}�g���b�N�X
	D3DXMatrixInverse(&mtxInvPrj, NULL, mtxPrj);		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixInverse(&mtxInvViexport, NULL, &mtxVP);	// �r���[�|�b�g�}�g���b�N�X

														// �X�N���[�����烏�[���h��
	mtxChange = mtxInvViexport * mtxInvPrj * mtxInvViex;		// ���W�ϊ��p
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &mtxChange);	// �X�N���[�����烏�[���h��

																		// ���[���h�̃|�C���g��Ԃ�
	return pout;
}

// ------------------------------------------------------------------------------------------
// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
// ------------------------------------------------------------------------------------------
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,		// ���[���h���W�̓��ꕨ
	float Sx,				// �X�N���[��X���W
	float Sy,				// �X�N���[��Y���W
	float Screen_w,			// �X�N���[���̉���
	float Screen_h,			// �X�N���[���̍���
	D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj,		// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 obj			// �I�u�W�F�N�g�̈ʒu
)
{
	// �ϐ��錾
	D3DXVECTOR3 Mearpos;	// �ŋߒl
	D3DXVECTOR3 Farpos;		// �ŉ��l
	D3DXVECTOR3 Ray;		// �}�E�X�|�C���g����

							// �X�N���[�����W�����[���h���W�ɕϊ�
	CalcScreenToWorld(		// �ŋߒl
		&Mearpos,
		Sx, Sy,
		0.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);

	CalcScreenToWorld(		// �ŉ��l
		&Farpos,
		Sx, Sy,
		1.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);

	// �}�E�X�|�C���g����
	Ray = Farpos - Mearpos;
	D3DXVec3Normalize(&Ray, &Ray);	// ���K��

	D3DXVECTOR3 diff;

	// ���P����K�v������
	diff = Mearpos - obj;
	diff = -(*D3DXVec3Normalize(&diff, &diff));

	// �����Ƃ̌�_ //
	if (Ray.y <= 0)
	{
		// ����_
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, 1, 0));		// �}�E�X�̕����̓���
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, 1, 0));	// ���_�̓���
		*pout = Mearpos + (LP0 / LRay)*Ray;
	}

	/* �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��	*/
	// �㏰�Ƃ̌�_ //
	else
	{
		// ����_
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, -1, 0));		// �}�E�X�̕����̓���
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, -1, 0));	// ���_�̓���
		*pout = Mearpos + (LP0 / LRay)*Ray;

		//*pout = Farpos;
	}

	return pout;
}

// ------------------------------------------------------------------------------------------
// ���[���h���W���X�N���[�����W�ɕϊ�(����)
// ------------------------------------------------------------------------------------------
D3DXVECTOR3* CalcWorldToScreen(
	D3DXVECTOR3* Obj,	// �I�u�W�F�N�g�̈ʒu
	D3DXMATRIX Wobj,	// ���[���h���W��̃I�u�W�F�N�g�̈ʒu
	float Screen_w,		// �X�N���[���̉���
	float Screen_h,		// �X�N���[���̍���
	D3DXMATRIX* mtxView,// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj)	// �v���W�F�N�V�����}�g���b�N�X
{
	// �e�s��̋t�s����Z�o //
	// �ϐ��錾
	D3DXMATRIX mtxVP;			// �v�Z�p�r���[�|�b�g
	D3DXMATRIX mtxChange;				// 

										// �r���[�|�b�g�̍s��쐬
	D3DXMatrixIdentity(&mtxVP);			// ������
	mtxVP._11 = SCREEN_WIDTH / 2.0f;
	mtxVP._22 = -SCREEN_HEIGHT / 2.0f;
	mtxVP._41 = SCREEN_WIDTH / 2.0f;
	mtxVP._42 = SCREEN_HEIGHT / 2.0f;

	mtxChange = Wobj * mtxVP * *mtxPrj * *mtxView;

	return Obj;
}