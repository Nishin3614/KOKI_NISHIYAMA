// ------------------------------------------------------------------------------------------
//
// �������� [range.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _RANGE_H_
#define _RANGE_H_

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
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 size;
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldRange;
	D3DXCOLOR	col;
	int			nType;				// �^�C�v
	bool		bUse;
	bool		bRange;	// �͈͂ɓ�������J��
} RANGE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitRange(void);
void UninitRange(void);
void UpdateRange(void);
void DrawRange(void);

void SetRange(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int			nType);

void PlayerPos_Range(void);

// �����̏��
RANGE *GetRange(void);
#endif
