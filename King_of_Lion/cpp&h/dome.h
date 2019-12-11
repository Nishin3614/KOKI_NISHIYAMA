// ------------------------------------------------------------------------------------------
//
// �h�[������ [dome.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _DOME_H_
#define _DOME_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ========================
// �^�C�v
// ========================
typedef enum
{
	DOMETYPE_ = 0,	// �S�[���n�_
	DOMETYPE_MAX
} DOMETYPE;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffDome = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;								// �ʒu
	D3DXVECTOR3 move;								// �ړ���
	D3DXVECTOR3 rot;								// ��]��
	D3DXVECTOR3 size;								// �T�C�Y
	D3DXVECTOR3 OriginBlock;						// �u���b�N�`��̌��_
	D3DXCOLOR	col;								// �J���[
	D3DXMATRIX  mtxWorldDome;						// ���[���h�}�g���b�N�X
	int	nNumberVertexMeshField;						// �����_��
	int	nNumIndex;									// ���C���f�b�N�X
	int	nNumPolygon;								// ���h�[��
	int	nNumber;									// ���_
	int	nBlock_Depth;								// �c�u���b�N��
	int	nBlock_Width;								// ���u���b�N��
	int nType;										// �^�C�v
	bool	bUse;									// �g�p���
} DOME;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitDome(void);
void UninitDome(void);
void UpdateDome(void);
void DrawDome(void);

// �g�p���̂��̂�������
void UseInitDome(void);

int SetDome(
	D3DXVECTOR3 pos,
	D3DXCOLOR	col,
	int nType);
void DeleteDome(int nIdx);

DOME *GetDome(void);
#endif
