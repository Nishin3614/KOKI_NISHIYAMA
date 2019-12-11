// ------------------------------------------------------------------------------------------
//
// Calculation�w�b�_�[�����̐���[calculation.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // �t�@�C������������߂�

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// Calculation���
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CCalculation
{
public:
	/* �֐� */
	// �����̌v�Z //
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,	// �ʒuA
		D3DXVECTOR3 &posB	// �ʒuB
	);

	// �O�ς̌v�Z //
	static float Cross_product(
		D3DXVECTOR3 &VecA,	// �ʒuA
		D3DXVECTOR3 &VecB	// �ʒuB
	);

	// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
	static bool SquareBottom_Judg(
		D3DXVECTOR3 &pos,		// �ʒu
		D3DXVECTOR3 &size,		// �T�C�Y
		D3DXVECTOR3 &rot,		// ��]��
		int			nBlock_W,	// ���u���b�N
		int			nBlock_H,	// �c�u���b�N
		D3DXVECTOR3 &worldpos	// ���[���h���W
	);

	// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
	static bool SquareSide_Judg(
		D3DXVECTOR3 &pos,		// �ʒu
		D3DXVECTOR3 &size,		// �T�C�Y
		D3DXVECTOR3 &rot,		// ��]��
		int			nBlock_W,	// ���u���b�N
		int			nBlock_H,	// �c�u���b�N
		D3DXVECTOR3 &worldpos	// ���[���h���W
	);

	// �~�̑����Z //
	static float Addition_Circle(
		float Length_x,	// x
		float Length_y,	// y
		float Length_z	// z
	);

	// �~�̓����蔻�� //
	static bool Collision_Circle(
		D3DXVECTOR3 &Me,		// �����̈ʒu
		float		fMeSize,	// ���a
		D3DXVECTOR3 &Opponent,	// ����̈ʒu
		float		fOppSize	// ���a
	);

	// ��]�ʂ̏�� //
	static float Rot_One_Limit(
		float &fRot	// ��]��
	);

	// �X�N���[�����W�����[���h���W�ɕϊ�
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// �}�E�X�|�C���g
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj		// �v���W�F�N�V�����}�g���b�N�X
	);
	// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,		// �}�E�X�|�C���g
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj,		// �v���W�F�N�V�����}�g���b�N�X
		D3DXVECTOR3 &obj		// �I�u�W�F�N�g�̈ʒu
	);
	// ���[���h���W���X�N���[�����W�ɕϊ�(����)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// �I�u�W�F�N�g�̈ʒu
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj		// �v���W�F�N�V�����}�g���b�N�X
	);
	// �ꎟ�֐�
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 &Apos,	// �n�_
		D3DXVECTOR3 &Bpos	// �I�_
	);
	// 2�����̌�_(�A��������)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 &ALinear,	// ���@
		D3DXVECTOR2 &BLinear	// ���A
	);
protected:

private:
	/* �ϐ� */
};

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------

#endif