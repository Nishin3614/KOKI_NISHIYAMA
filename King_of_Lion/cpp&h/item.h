// ------------------------------------------------------------------------------------------
//
// �A�C�e������ [item.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "floor.h"

#include "materials.h"

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posold;			// �O��̈ʒu���
	D3DXVECTOR3 rot;			// ��]��
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 size;			// �T�C�Y
	D3DXMATRIX  mtxWorldItem;	// �}�g���b�N�X
	float		fRadius;		// ���a
	int			nLife;			// �o������
	int			nIdxShadow;		// �e��ID
	int			nType;			// ���
	int			nAttack;		// �U����
	bool		bUse;			// �g�p���

	// �O������
	WALL		*pWall;					// �Ώۂ̕ǂ̃|�C���g
	FLOOR		*pMeshField;			// �Ώۂ̏��̃|�C���g
	MATERIALS	*pMaterials;			// �Ώۂ̑f�ނ̃|�C���g

} ITEM;						

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

// �A�C�e���̐ݒ�
void SetItem(
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR3 rot,	// ��]
	D3DXVECTOR3 move,	// �ړ���
	D3DXVECTOR3 size,	// �T�C�Y
	float	fRadius,	// ���a
	int nType,			// �^�C�v
	int nLife			// ���C�t
);

void InforItem(void);

ITEM *GetItem(void);
#endif
