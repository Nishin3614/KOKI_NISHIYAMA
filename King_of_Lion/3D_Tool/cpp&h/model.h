// ------------------------------------------------------------------------------------------
//
// ���f������ [model.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------

// ---------- �g���C�G ---------- //
typedef struct
{
	LPD3DXMESH		paMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	paBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD			nNumMat;			// �}�e���A���̐�
	D3DXMATRIX		mtxWorldModel;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3		pos;				// �ʒu
	D3DXVECTOR3		posLast;			// �s�������ʒu
	D3DXVECTOR3		posKeyBetween;		// �L�[�Ԃ̋���
	D3DXVECTOR3		posOrigin;			// �����ʒu
	D3DXVECTOR3		rot;				// ���݉�]��
	D3DXVECTOR3		rotLast;			// �s����������
	D3DXVECTOR3		rotKeyBetween;		// �L�[�Ԃ̉�]��
	D3DXVECTOR3		size;				// �T�C�Y
	D3DXVECTOR3		vtxMinMaterials;	// ���f���̈ʒu�̍ŏ��l
	D3DXVECTOR3		vtxMaxMaterials;	// ���f���̈ʒu�̍ő�l
	int				nIdxModelParent;	// �e���f���̃C���f�b�N�X
	int				nFram;				// �t���[��
	int				nCntKeySet;			// �L�[�Z�b�g�J�E���g
} MODEL;

// ---------- �L�[�v�f ---------- //
typedef struct 
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} KEY;

// ---------- �L�[��� ---------- //
typedef struct
{
	int nFram;
	KEY key[15];	// �p�[�c��
} KEY_INFO;
#endif