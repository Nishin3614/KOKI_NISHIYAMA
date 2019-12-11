// ------------------------------------------------------------------------------------------
//
// �T�E���h���� [sound.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �T�E���h�t�@�C��
//
// ------------------------------------------------------------------------------------------
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g��
	SOUND_LABEL_BGM_GAME,			// �Q�[��
	SOUND_LABEL_BGM_GAMEOVER,		// �Q�[���I�[�o�[
	SOUND_LABEL_BGM_CLEAR,			// �N���A
	SOUND_LABEL_BGM_RANKINGMOVE,	// �����L���O
	SOUND_LABEL_SE_ENTER,			// ���艹
	SOUND_LABEL_SE_ATTACK,			// �U��
	SOUND_LABEL_SE_COLISION,		// ����������
	SOUND_LABEL_SE_EAT,				// �H�ׂ��Ƃ�
	SOUND_LABEL_SE_BEAM,			// �e���ˎ�
	SOUND_LABEL_SE_BOSSVOICE,		// �{�X�̖���
	SOUND_LABEL_SE_RANKING,			// �����L���O���\
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
