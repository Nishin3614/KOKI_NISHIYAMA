// ------------------------------------------------------------------------------------------
//
// �Q�[�������̐���[game.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "game.h"
#include "input.h"
#include "joypad.h"
#include "mouse.h"
#include "camera.h"
#include "light.h"
#include "materials.h"
#include "shadow.h"
#include "wall.h"
#include "floor.h"
#include "giraffe.h"
#include "fade.h"
#include "arrangement.h"
#include "Billboard.h"
#include "seleob.h"
#include "debugproc.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define GAME1 "data/TEXTURE/BG001.png"	// �w�i
#define GAME_LEFT_X (0)			// ��
#define GAME_ON_Y (0)			// ��
#define GAME_SIZE_X (1280)		// �T�C�Y�i���j
#define GAME_SIZE_Y (720)		// �T�C�Y�i���j
#define SET_ENEMY_X (1350.0F)		// ENEMY��X�|�W

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_ThirdGame(void);
#ifdef _DEBUG
void Update_Debuggame(void);
#endif // _DEBUG
void SaveGame(void);			// �Q�[���Z�[�u�֐�
void LoadGame(void);			// �Q�[�����[�h�֐�

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------
GAMESTATE g_GameState = GAMESTATE_3D;		// ���
GAMESELECT g_GameSelect = GAMESELECT_FLOOR;	// �I�����
int g_nCntGameState = 0;					// �J�E���g
int g_GameStage = GAMESTAGE_1;	// �X�e�[�W���

#ifdef _DEBUG
int					g_nNumberDebug = 4;		// �f�o�b�O�i���o�[
#endif

// ------------------------------------------------------------------------------------------
//
// �v���C���[����������
//
// ------------------------------------------------------------------------------------------

void InitGame(void)
{
	// game������
	g_GameState = GAMESTATE_3D;			// ��Ԑݒ�
	g_GameSelect = GAMESELECT_FLOOR;	// �I�����
	g_nCntGameState = 0;			// �J�E���g

	// �J�����̏���������
	InitCamera();
	// ���C�g�̏���������
	InitLight();
	// �z�u�̏���������
	InitArrangement();
	// �r���{�[�h�̏���������
	InitBillboard();
	// �|���S���̏���������
	InitFloor();
	// �ǂ̏���������
	InitWall();
	// �e�̏���������
	InitShadow();
	// �}�e���A���̏���������
	InitMaterials();
	// �L�����̏���������
	InitGiraffe();
	// �I���I�u�̏���������
	InitSeleob();
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�I������
//
// ------------------------------------------------------------------------------------------

void UninitGame(void)
{
	// �J�����̏I������
	UninitCamera();
	// ���C�g�̏I������
	UninitLight();
	// �z�u�̏I������
	UninitArrangement();
	// �r���{�[�h�̏I������
	UninitBillboard();
	// 3D�|���S���̏I������
	UninitFloor();
	// �ǂ̏I������
	UninitWall();
	// �e�̏I������
	UninitShadow();
	// �}�e���A���̏I������
	UninitMaterials();
	// �L�����̏I������
	UninitGiraffe();
	// �I���I�u�̏I������
	UninitSeleob();
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateGame(void)
{
#ifdef _DEBUG
	// �f�o�b�O�\��
	Update_Debuggame();
#endif // _DEBUG

	// �J�����̍X�V����
	UpdateCamera();
	// ���C�g�̍X�V����
	UpdateLight();
	// �z�u�̍X�V����
	UpdateArrangement();
	// 3D�c�[��
	Update_ThirdGame();
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawGame(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// ���̕`�揈��
	DrawFloor();

	// �ǂ̕`�揈��
	DrawWall();

	// �}�e���A���̕`�揈��
	DrawMaterials();

	// �r���{�[�h�̕`�揈��
	DrawBillboard();

	// �L�����̕`�揈��
	DrawGiraffe();

	// �I���I�u�̕`�揈��
	DrawSeleob();
}

// ------------------------------------------------------------------------------------------
// 3D�Q�[��Update����
// ------------------------------------------------------------------------------------------
void Update_ThirdGame(void)
{
	// �Z�[�u
	if (GetKeyboardSpeacePress(DIK_RETURN))
	{
		// �ۑ�
		SaveGame();
	}
	// �z�u������� //
	// ��
	if (GetKeyboardSpeacePress(DIK_1))
	{
		// ��\����Ԃ�
		NotDisp_Game(GAMESELECT_FLOOR);
	}

	// �f��
	else if (GetKeyboardSpeacePress(DIK_2))
	{
		// ��\����Ԃ�
		NotDisp_Game(GAMESELECT_MATERIALS);
	}

	// �L����
	else if (GetKeyboardSpeacePress(DIK_3))
	{
		// ��\����Ԃ�
		NotDisp_Game(GAMESELECT_GIRAFFE);
	}

	// ��
	else if (GetKeyboardSpeacePress(DIK_4))
	{
		// ��\����Ԃ�
		NotDisp_Game(GAMESELECT_WALL);
	}

	// �r���{�[�h
	else if (GetKeyboardSpeacePress(DIK_5))
	{
		// ��\����Ԃ�
		NotDisp_Game(GAMESELECT_BILLBOARD);
	}

	// �����\�������Ȃ�
	else if (GetKeyboardSpeacePress(DIK_9))
	{
		// ��\����Ԃ�
		NotDisp_Game(GAMESELECT_SELECT);
	}

	// ���N���b�N
	if (GetKeyboardSpeacePress(DIK_RIGHT))
	{
		// �Q�[���̃Z�[�u
		SaveGame();
		// �X�e�[�W�J�E���g�A�b�v
		g_GameStage++;

		// ���x�𒴂�����
		if (g_GameStage >= GAMESTAGE_MAX)
		{
			g_GameStage = GAMESTAGE_1;
		}

		// �Q�[���̃��[�h
		LoadGame();
	}

	// ���N���b�N
	else if (GetKeyboardSpeacePress(DIK_LEFT))
	{
		// �Q�[���̃Z�[�u
		SaveGame();

		// �X�e�[�W�J�E���g�A�b�v
		g_GameStage--;

		// ���x�𒴂�����
		if (g_GameStage < 0)
		{
			g_GameStage = GAMESTAGE_5;
		}

		// �Q�[���̃��[�h
		LoadGame();
	}
	switch (g_GameSelect)
	{
	case GAMESELECT_FLOOR:
		// �|���S���̍X�V����
		UpdateFloor();
		break;

	case GAMESELECT_MATERIALS:
		// �}�e���A���̍X�V����
		UpdateMaterials();
		break;

	case GAMESELECT_GIRAFFE:
		// �L�����̍X�V����
		UpdateGiraffe();
		break;

	case GAMESELECT_WALL:
		// �ǂ̍X�V����
		UpdateWall();
		break;

	case GAMESELECT_BILLBOARD:
		// �ǂ̍X�V����
		UpdateBillboard();
		break;
	case GAMESELECT_SELECT:
		// �I���I�u�W�F�N�g�̍X�V����
		UpdateSeleob();
		break;
	}
}

// ------------------------------------------------------------------------------------------
// �ۑ�����
// ------------------------------------------------------------------------------------------
void SaveGame(void)
{
	// �f�ރf�[�^�̕ۑ�
	SaveMaterials();

	// ���f�[�^�̕ۑ�
	SaveFloor();

	// �L�����f�[�^�̕ۑ�
	SaveGiraffe();

	// �ǂ̕ۑ�
	SaveWall();

	// �r���{�[�h�̕ۑ�
	SaveBillboard();
}

// ------------------------------------------------------------------------------------------
// ���[�h�����֐�
// ------------------------------------------------------------------------------------------
void LoadGame(void)
{
	// �f�ރf�[�^�̕ۑ�
	LoadMaterials();

	// ���f�[�^�̕ۑ�
	LoadFloor();

	// �L�����f�[�^�̕ۑ�
	LoadGiraffe();

	// �ǂ̕ۑ�
	LoadWall();

	// �r���{�[�h�̕ۑ�
	LoadBillboard();
}

// ------------------------------------------------------------------------------------------
// ��\����Ԃ֏���
// ------------------------------------------------------------------------------------------
void NotDisp_Game(GAMESELECT select)
{
	// �f�ނ��\����Ԃ�
	NotDisp_Materials();

	// �L�������\����Ԃ�
	NotDispGiraffe();

	// �����\����Ԃ�
	NotDispFLoor();

	// �ǂ��\����Ԃ�
	NotDispWall();

	// �r���{�[�h���\����Ԃ�
	NotDispBillboard();

	g_GameSelect = select;
}

// ------------------------------------------------------------------------------------------
//
// �Q�[���X�e�[�g�̐ݒ�
//
// ------------------------------------------------------------------------------------------

void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCntGameState = 0;
}

// ------------------------------------------------------------------------------------------
//
// �Q�[���X�e�[�g�̎擾
//
// ------------------------------------------------------------------------------------------

GAMESTATE GetGameStete(void)
{
	return g_GameState;
}

// ------------------------------------------------------------------------------------------
//
// �Q�[���I���̐ݒ�
//
// ------------------------------------------------------------------------------------------

void SetGameSelect(GAMESELECT select)
{
	g_GameSelect = select;
}

// ------------------------------------------------------------------------------------------
//
// �Q�[���I���̎擾
//
// ------------------------------------------------------------------------------------------

GAMESELECT GetGameSelect(void)
{
	return g_GameSelect;
}

// ------------------------------------------------------------------------------------------
//
// �X�e�[�W���̎擾
//
// ------------------------------------------------------------------------------------------
int GetGameStage(void)
{
	return g_GameStage;
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------------------------------------------------------
void Update_Debuggame(void)
{
	// �ϐ��錾
	// ���擾
	ARRANGEMENT *pArrangement = GetArrangement();	// �z�u				// ���f���擾
	CAMERA		*pCamera = GetCamera();				// �J����

	// -------------------��{���-------------------- //
	PrintDebug("/*---------<��{����>----------*/\n");
	PrintDebug("[�}�E�X�̉E�N���b�N]:�J������]\n");
	PrintDebug("[�}�E�X�̃z�C�[���N���b�N]:�J�����ړ�\n");
	PrintDebug("[�}�E�X�̍��N���b�N]:�I���E�z�u\n");
	PrintDebug("[A][D]:�J����X���ړ� / [W][S]:�J����Z���ړ�\n");
	PrintDebug("[F][R]:�J����Y���ړ�\n");
	PrintDebug("[E][C]:�I�u�W�F�N�g��y���W�ύX\n");
	PrintDebug("[��][��]:�Z�[�u�ꏊ�؂�ւ�\n");
	PrintDebug("[1]:��	[2]:���f��\n");
	PrintDebug("[3]:�G	[4]:��\n");
	PrintDebug("[9]:�I�����[�h\n");
	PrintDebug("/*------------------------------*/\n");
	PrintDebug("�I�[�g�Z�[�u\n", g_GameStage);
	PrintDebug("���݂̃Z�[�u�ꏊ[%d/5]\n",g_GameStage);
	PrintDebug("���݂̃��[�h[%d]\n",g_GameSelect);
	PrintDebug("�I���ʒu���->(%0.3f,%0.3f,%0.3f)\n",
		pArrangement->pos.x,
		pArrangement->pos.y,
		pArrangement->pos.z
	);
	PrintDebug("�I����]���->(%0.3f,%0.3f,%0.3f)\n",
		pArrangement->rot.x,
		pArrangement->rot.y,
		pArrangement->rot.z
	);
	// -------------------���W���-------------------- //

}
#endif // _DEBUG
