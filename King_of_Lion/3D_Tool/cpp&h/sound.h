// ------------------------------------------------------------------------------------------
//
// �T�E���h���� [sound.h]
// Author : KOKI NISHIYAMA
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
	SOUND_LABEL_SE_START,			// ���艹
	SOUND_LABEL_SE_LOWKICK,			// �q�b�g��
	SOUND_LABEL_SE_HIGHKICK,		// �G�l�~�[���|���ꂽ��
	SOUND_LABEL_SE_CATFOOD,			// �L�ʉ�
	SOUND_LABEL_SE_DIAMOND,			// �_�C�������h��
	SOUND_LABEL_SE_CATRECOVERY,		// �񕜃A�C�e����
	SOUND_LABEL_SE_CATDAMEGE,		// �v���C�����U�����ꂽ��
	SOUND_LABEL_SE_CATDIE,			// �v���C�������񂾉�
	SOUND_LABEL_SE_BOSSLAST,		// �{�X���|���ꂽ��
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
