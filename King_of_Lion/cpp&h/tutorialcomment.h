// ------------------------------------------------------------------------------------------
//
// �R�����g�w�b�_�[�����̐���[tutorialcomment.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _TUTORIALCOMMENT_H_
#define _TUTORIALCOMMENT_H_	 // �t�@�C������������߂�

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

#define TUTORIALCOMMENT_ON_Y (320)		// ��

// ------------------------------------------------------------------------------------------
//
// �R�����g���
//
// ------------------------------------------------------------------------------------------

typedef enum
{
	TUTORIALCOMMENTTYPE_BG = 0,
	TUTORIALCOMMENTTYPE_KEYBOARD,
	TUTORIALCOMMENTTYPE_JOYPAD,
	TUTORIALCOMMENTTYPE_MAX
}TUTORIALCOMMENTTYPE;
// ------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ------------------------------------------------------------------------------------------

typedef struct
{
	TUTORIALCOMMENTTYPE Type;	// �^�C�v
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR	col;	// �J���[
	float fXSize;		// X�T�C�Y
	float fYSize;		// Y�T�C�Y
	bool bUse;			// �g�p���Ă��邩�ǂ���
} TUTORIALCOMMENT;

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------

// ��{�v���g�^�C�v	//
void InitTutorialComment(void);
void UninitTutorialComment(void);
void UpdateTutorialComment(void);
void DrawTutorialComment(void);
void SetTutorialComment(
	TUTORIALCOMMENTTYPE Type,
	D3DXVECTOR3 pos,
	D3DXCOLOR col,
	float fXSize,
	float fYSize);

TUTORIALCOMMENT *GetTutorialComment(void);

#endif