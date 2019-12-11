// ------------------------------------------------------------------------------------------
//
// �\�������� [prediction.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _PRIDICTION_H_
#define _PRIDICTION_H_

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
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldPrediction;
	D3DXCOLOR	col;
	D3DXVECTOR3	size;
	bool		bUse;
} PRIDICTION;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitPrediction(void);
void UninitPrediction(void);
void UpdatePrediction(void);
void DrawPrediction(void);

// �\�����̐ݒ�
int SetPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 size,D3DXCOLOR col);

// �\�����̈ʒu�ݒ�
void SetPositionPrediction(int nIdxPrediction, D3DXVECTOR3 pos);

// �\�����̍폜
void DeletePrediction(int nIdxPrediction);

// �\�����̏��
PRIDICTION *GetPrediction(void);
#endif
