// ------------------------------------------------------------------------------------------
//
// ������ [water.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _WATER_H_
#define _WATER_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define WATER_MAX (16)						// ���̍ő吔

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffWATER = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;									// �ʒu
	D3DXVECTOR3 move;									// �ړ���
	D3DXVECTOR3 rot;									// ��]��
	D3DXVECTOR3 size;									// �T�C�Y
	D3DXVECTOR3 OriginBlock;							// �u���b�N�`��̌��_
	D3DXMATRIX  mtxWorldWATER;							// ���[���h�}�g���b�N�X
	int	nNumberVertexMeshField;							// �����_��
	int nNumIndex;										// ���C���f�b�N�X
	int nNumPolygon;									// ����
	int nNumber;										// ���_
	int nBlock_Depth;									// �c�u���b�N��
	int nBlock_Width;									// ���u���b�N��
	int nType;											// ���
	bool bUse;											// �g�p���
	bool bDisp;											// �\�����
} WATER;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitWATER(void);
void UninitWATER(void);
void UpdateWATER(void);
void DrawWATER(void);

// �����蔻��
bool ColisionWATER(
	D3DXVECTOR3 *pos,		// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,	// �O��̃|�W�V����
	D3DXVECTOR3 *move,		// �ړ���
	D3DXVECTOR3 *size,		// �T�C�Y
	WATER		**ppWaterRet);

// ���̏��
WATER *GetWATER(void);
#endif
