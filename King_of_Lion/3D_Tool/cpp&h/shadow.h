// ------------------------------------------------------------------------------------------
//
// �e���� [shadow.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_

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
	D3DXMATRIX  mtxWorldShadow;
	D3DXCOLOR	col;
	D3DXVECTOR3	size;
	bool		bUse;
} SHADOW;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

// �e�̐ݒ�
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);

// �e�̈ʒu�ݒ�
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

// �e�̍폜
void DeleteShadow(int nIdxShadow);

// �e�̏��
SHADOW *GetShadow(void);
#endif
