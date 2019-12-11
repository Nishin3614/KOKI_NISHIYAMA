// ------------------------------------------------------------------------------------------
//
// �J�������� [camera.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �J�����̍\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posVDest;		// �ړI���_
	D3DXVECTOR3 posVDiff;		// �ړI���_���王�_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 posRDest;		// �ڕW�����_
	D3DXVECTOR3 posRDiff;		// �ړI�����_���璍���_
	D3DXVECTOR3 posU;			// ������x�N�g��
	D3DXVECTOR3 between;		// �v���C���[�Ƃ̋��� 
	D3DXVECTOR3 last;			// 
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ���_�̉�]��
	D3DXVECTOR3 rotDest;		// ���_�̖ړI��]�n�_
	D3DXVECTOR3 rotDiff;		// �ړI��]�n�_���猻�݉�]�n�_
	D3DXVECTOR3 rotBetween;		// �ړI��]�n�_�ƌ��݉�]�n�_�̍���
	D3DXMATRIX	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X(�e���r���_)
	D3DXMATRIX	mtxView;		// �r���[�}�g���b�N�X(�J�����}�����_)
	float		fLength;		// ���_�ƒ����_�̋���
	int			nCntTime;		// ���ʂɌ����̂ɂ����鎞��
	bool		bfront;			// ���ʂɌ����������Ȃ���
} CAMERA;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

// �J�����ݒ�
void SetCamera(void);

void CameraPosition(void);

// �J�������
CAMERA *GetCamera(void);
#endif
