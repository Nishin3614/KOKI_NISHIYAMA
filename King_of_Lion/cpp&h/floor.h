// ------------------------------------------------------------------------------------------
//
// ������ [floor.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define FLOOR_MAX (128)						// ���̍ő吔

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffFLOOR = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;									// �ʒu
	D3DXVECTOR3 move;									// �ړ���
	D3DXVECTOR3 rot;									// ��]��
	D3DXVECTOR3 size;									// �T�C�Y
	D3DXVECTOR3 OriginBlock;							// �u���b�N�`��̌��_
	D3DXMATRIX  mtxWorldFLOOR;							// ���[���h�}�g���b�N�X
	int	nNumberVertexMeshField;							// �����_��
	int nNumIndex;										// ���C���f�b�N�X
	int nNumPolygon;									// ����
	int nNumber;										// ���_
	int nBlock_Depth;									// �c�u���b�N��
	int nBlock_Width;									// ���u���b�N��
	int nType;											// ���
	bool bUse;											// �g�p���
	bool bDisp;											// �\�����
} FLOOR;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitFLOOR(void);
void UninitFLOOR(void);
void UpdateFLOOR(void);
void DrawFLOOR(void);

// �����蔻��
bool ColisionFLOOR(
	D3DXVECTOR3 *pos,			// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,		// �O��̃|�W�V����
	D3DXVECTOR3 *move,			// �ړ���
	D3DXVECTOR3 *size,			// �T�C�Y
	FLOOR		**ppFloorRet	// ���̃A�h���X���̃A�h���X
);

// ���̏��
FLOOR *GetFLOOR(void);
#endif
