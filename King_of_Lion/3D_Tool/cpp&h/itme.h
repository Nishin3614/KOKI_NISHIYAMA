// ------------------------------------------------------------------------------------------
//
// �A�C�e������ [item.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _ITME_H_
#define _ITME_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndex = NULL;			// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;								// �ʒu
	D3DXVECTOR3 move;								// �ړ���
	D3DXVECTOR3 rot;								// ��]��
	D3DXVECTOR3 size;								// �T�C�Y
	D3DXMATRIX  mtxWorldItem;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 OriginBlock;						// �u���b�N�`��̌��_
	int	nNumberVertexMeshField;						// �����_��
	int nNumIndex;									// ���C���f�b�N�X
	int nNumPolygon;								// ���|���S��
	int nNumber;									// ���_
	int nBlock_Depth;								// �c�u���b�N��
	int nBlock_Width;								// ���u���b�N��
	int nType;										// �^�C�v
	bool		bUse;								// �g�p���
	bool		bDisp;								// �\�����
} ITME;

typedef struct
{
	D3DXVECTOR3 size;								// �T�C�Y
	D3DXVECTOR3 rot;								// ��]��
	int nBlock_Depth;								// �c�u���b�N��
	int nBlock_Width;								// ���u���b�N��
	int nType;										// �^�C�v
} ITMESET;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType);

// �����蔻��
bool ColisionItem(
	D3DXVECTOR3 *pos,		// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,	// �O��̃|�W�V����
	D3DXVECTOR3 *move,		// �ړ���
	D3DXVECTOR3 *size,		// �T�C�Y
	ITME		**ppItemRet);

void NotDispItem(void);	// �\���ؑ֎��ɕ\����

// �Z�[�u
void SaveItem(void);

// ���[�h
void LoadItem(void);

ITME *GetItem(void);

#ifdef _DEBUG			
// ���̏��
//void DrawItemInfo(
//	D3DXVECTOR3 size,
//	int nBlock_W,
//	int nBlock_H,
//	int nType);
#endif

#endif
