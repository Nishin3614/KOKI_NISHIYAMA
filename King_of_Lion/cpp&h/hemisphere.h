// ------------------------------------------------------------------------------------------
//
// �������� [hemisphere.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _HEMISPHERE_H_
#define _HEMISPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffHemisphere = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		pTextureHemisphere = NULL;	// ���_�e�N�X�`���[�̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;			// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;										// �ʒu
	D3DXVECTOR3 move;										// �ړ���
	D3DXVECTOR3 rot;										// ��]��
	D3DXVECTOR3 size;										// �T�C�Y
	D3DXVECTOR3 OriginBlock;								// �u���b�N�`��̌��_
	D3DXMATRIX  mtxWorldHemisphere;						// ���[���h�}�g���b�N�X
	int	nNumberVertexMeshField;								// �����_��
	int nNumIndex;											// ���C���f�b�N�X
	int nNumPolygon;										// ������
	int nNumber;											// ���_
	int nBlock_Depth;										// �c�u���b�N��
	int nBlock_Width;										// ���u���b�N��
	float fRadius;											// ���W�A���l
} HEMISPHERE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitHemisphere(void);
void UninitHemisphere(void);
void UpdateHemisphere(void);
void DrawHemisphere(void);

HEMISPHERE *GetHemisphere(void);
#endif
