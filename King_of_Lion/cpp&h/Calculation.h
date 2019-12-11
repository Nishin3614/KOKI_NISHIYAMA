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
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
// �O�ς̌v�Z //
float Cross_product(
	D3DXVECTOR3 VecA,
	D3DXVECTOR3 VecB);

// �����̌v�Z //
D3DXVECTOR3 Difference_Between(
	D3DXVECTOR3 posA,
	D3DXVECTOR3 posB);

// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
bool SquareBottom_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
);

// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
bool SquareSide_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
);

// �~�̑����Z //
float Addition_Circle(
	float Length_x,
	float Length_y,
	float Length_z);

// �~�̓����蔻�� //
bool Collision_Circle(
	D3DXVECTOR3 Me,
	float		fMeSize,
	D3DXVECTOR3 Opponent,
	float		fOppSize);

// ��]�ʂ̏�� //
float Rot_One_Limit(
	float fRot);

// �X�N���[�����W�����[���h���W�ɕϊ�
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,		// �}�E�X�|�C���g
	float Sx,					// �X�N���[��X���W
	float Sy,					// �X�N���[��Y���W
	float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
	float Screen_w,			// �X�N���[���̉���
	float Screen_h,			// �X�N���[���̍���
	D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj);	// �v���W�F�N�V�����}�g���b�N�X

							// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,		// �}�E�X�|�C���g
	float Sx,				// �X�N���[��X���W
	float Sy,				// �X�N���[��Y���W
	float Screen_w,			// �X�N���[���̉���
	float Screen_h,			// �X�N���[���̍���
	D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj,		// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 obj);		// �I�u�W�F�N�g�̈ʒu

							// ���[���h���W���X�N���[�����W�ɕϊ�(����)
D3DXVECTOR3* CalcWorldToScreen(
	D3DXVECTOR3* Obj,		// �I�u�W�F�N�g�̈ʒu
	D3DXMATRIX Wobj,		// ���[���h���W��ł̃I�u�W�F�N�g�̃}�g���b�N�X
	float Screen_w,			// �X�N���[���̉���
	float Screen_h,			// �X�N���[���̍���
	D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj);	// �v���W�F�N�V�����}�g���b�N�X

#endif