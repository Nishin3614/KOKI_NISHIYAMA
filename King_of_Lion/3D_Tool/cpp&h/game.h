// ------------------------------------------------------------------------------------------
//
// game�����̐���[game.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_	 // �t�@�C������������߂�

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �񋓌^
//
// ------------------------------------------------------------------------------------------
// ========================
// ���
// ========================
typedef enum
{
	GAMESTATE_3D = 0,	// 2D
	GAMESTATE_MAX 
} GAMESTATE;

// ========================
// �X�e�[�W�\��
// ========================
typedef enum
{
	GAMESTAGE_1 = 0,	// �X�e�[�W1
	GAMESTAGE_2,	// �X�e�[�W2
	GAMESTAGE_3,	// �X�e�[�W3
	GAMESTAGE_4,	// �X�e�[�W4
	GAMESTAGE_5,	// �X�e�[�W5
	GAMESTAGE_MAX
} GAMESTAGE;

// ========================
// �I�����
// ========================
typedef enum
{
	GAMESELECT_NONE = 0,	// �Ȃ���Ȃ�
	GAMESELECT_FLOOR,		// ��
	GAMESELECT_MATERIALS,	// �f��
	GAMESELECT_GIRAFFE,		// �L����
	GAMESELECT_WALL,		// ��
	GAMESELECT_BILLBOARD,	// �r���{�[�h
	GAMESELECT_SELECT,		// �I�����
	GAMESELECT_MAX
} GAMESELECT;

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------

// ��{�v���g�^�C�v	//
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE GetGameStete(void);

void NotDisp_Game(GAMESELECT select);
void SetGameSelect(GAMESELECT select);
GAMESELECT GetGameSelect(void);

// �Q�[���X�e�[�W�擾
int GetGameStage(void);

#endif