// ------------------------------------------------------------------------------------------
//
// �X�R�A���� [rollscore.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _ROLLSCORE_H_
#define _ROLLSCORE_H_

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
void InitRollScore(void);
void UninitRollScore(void);
void UpdateRollScore(void);
void DrawRollScore(void);

// �X�R�A�v�Z�֐�
void AddRollScore(int nValue);
int GetRollScore(void);
#endif