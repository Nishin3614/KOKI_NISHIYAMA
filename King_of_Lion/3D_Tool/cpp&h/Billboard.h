// ------------------------------------------------------------------------------------------
//
// �|���S������ [billboard.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "materials.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define BILLBOARD_MAX (128)

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��̈ʒu
	D3DXVECTOR3 rot;			// ��]��
	D3DXVECTOR3 size;			// �T�C�Y
	D3DXMATRIX  mtxWorldBillboard;	// �}�g���b�N�X
	WALL		*pWall;			// �Ώۂ̕ǂ̃|�C���g
	MATERIALS	*pMaterials;	// �Ώۂ̑f�ނ̃|�C���g
	int			nType;			// ���
	bool		bUse;			// �g�p���
	bool		bDisp;			// �\�����
} BILLBOARD;						

typedef struct
{
	D3DXVECTOR3 size;									// �T�C�Y
	int nType;											// �^�C�v
} BILLBOARDSET;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);

void SetBillboard(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nType);

void NotDispBillboard(void);	// �\���ؑ֎��ɕ\����

// �Z�[�u
void SaveBillboard(void);

// ���[�h
void LoadBillboard(void);

BILLBOARD *GetBillboard(void);
#endif
