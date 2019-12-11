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
#include "polygon.h"
#include "player.h"
#include "materials.h"
#include "shadow.h"
#include "wall.h"
#include "bullet.h"
#include "floor.h"
#include "dome.h"
#include "explosion.h"
#include "effect.h"
#include "giraffe.h"
#include "hemisphere.h"
#include "fade.h"
#include "pouse.h"
#include "pousecoment.h"
#include "arrow.h"
#include "trajectory.h"
#include "buffalo.h"
#include "hpgauge.h"
#include "item.h"
#include "king_rion.h"
#include "UI.h"
#include "time.h"
#include "Billboard.h"
#include "range.h"
#include "gamefade.h"
#include "select.h"
#include "selectcomment.h"
#include "ranking.h"
#include "enemy.h"
#include "score.h"
#include "water.h"
#include "shogira.h"
#include "prediction.h"

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
#ifdef _DEBUG
void Draw_Debug_PlayerPos(void);

void Draw_Debug_Camera(void);

void Draw_Debug_ShadowPos(void);

void Draw_Debug_GiraffeInfo(void);

void Draw_Debug_ATAN2F(void);
#endif

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------
GAMESTATE g_GameState = GAMESTATE_NONE;	// ���
int g_nCntGameState = 0;				// �J�E���g
#ifdef _DEBUG
LPD3DXFONT			g_pGameFont;			// �t�H���g�ւ̃|�C���^
int					g_nNumberDebug = 0;		// �f�o�b�O�i���o�[
#endif

// ------------------------------------------------------------------------------------------
//
// �v���C���[����������
//
// ------------------------------------------------------------------------------------------

void InitGame(void)
{
#ifdef _DEBUG
	g_pGameFont = GetFont();
#endif
	// game������
	g_GameState = GAMESTATE_NORMAL;	// ��Ԑݒ�
	g_nCntGameState = 0;			// �J�E���g

	// �Q�[���t�F�[�h�̏���������
	InitGameFade(g_GameState);

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �����̏�����
	InitExplosion();

	// �G�t�F�N�g������
	InitEffect();

	// �O�Ղ̏���������
	InitTrajectory();

	// �e�̏���������
	InitBullet();

	// �r���{�[�h�̏���������
	InitBillboard();

	// �͈͂̏���������
	InitRange();

	// �|���S���̏���������
	InitFLOOR();

	// �h�[���̏�����
	InitDome();

	// �����̏�����
	InitHemisphere();

	// �ǂ̏���������
	InitWall();

	// �e�̏���������
	InitShadow();

	// �\�����̏���������
	InitPrediction();

	// �}�e���A���̏���������
	InitMaterials();

	// �A�C�e���̏���������
	InitItem();

	// �L�����̏���������
	InitGiraffe();

	// �L�����̏���������
	InitShogira();

	// �o�b�t�@���[�̏���������
	InitBuffalo();

	// �S�b�̉��̏���������
	InitKing_Rion();

	// �v���C���[�̏���������
	InitPlayer();

	// ���̏���������
	InitWATER();

	// HP�Q�[�W�̏���������
	InitHpGauge();

	// UI�̏���������
	InitUi();

	// �^�C���̏���������
	InitTime();

	// ���j���̏���������
	InitScore();

	// �|�[�Y�̏���������
	InitPouse();

	// �|�[�Y�R�����g�̏���������
	InitPouseComent();

	// �I���̏�����
	InitSelect();

	// �I���R�����g�̏�����
	InitSelectComment();

	// �I���R�����g�̃^�C�v
	SceneSelectComment(SELECTCOMMENTSCENE_AREA);

	// ���̏�����
	InitArrow();
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�I������
//
// ------------------------------------------------------------------------------------------

void UninitGame(void)
{
	// �Q�[���t�F�[�h�̏I������
	UninitGameFade();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �e�̏I������
	UninitBullet();

	// �r���{�[�h�̏���������
	UninitBillboard();

	// �͈͂̏I������
	UninitRange();

	// 3D�|���S���̏I������
	UninitFLOOR();

	// �h�[���̏I������
	UninitDome();

	// �����̏I������
	UninitHemisphere();

	// �ǂ̏I������
	UninitWall();

	// �e�̏I������
	UninitShadow();

	// �\�����̏I������
	UninitPrediction();

	// �}�e���A���̏I������
	UninitMaterials();

	// �v���C���[�̏I������
	UninitPlayer();

	// �A�C�e���̏I������
	UninitItem();

	// �L�����̏I������
	UninitGiraffe();

	// �L�����̏I������
	UninitShogira();
	
	// �o�b�t�@���[�̏I������
	UninitBuffalo();

	// �S�b�̉��̏I������
	UninitKing_Rion();

	// ���̏I������
	UninitWATER();

	// �O�Ղ̏I������
	UninitTrajectory();

	// HP�Q�[�W�̏I������
	UninitHpGauge();

	// UI�̏I������
	UninitUi();

	// �^�C���̏I������
	UninitTime();

	// ���j���̏I������
	UninitScore();

	// �I���̏I������
	UninitSelect();

	// �I���R�����g�̏I������
	UninitSelectComment();

	// �|�[�Y�̏I������
	UninitPouse();

	// �|�[�Y�R�����g�̏I������
	UninitPouseComent();

	// �|�[�Y�I���̏I������
	UninitArrow();
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateGame(void)
{
	if (g_GameState == GAMESTATE_NORMAL ||
		g_GameState == GAMESTATE_BOSS || 
		g_GameState == GAMESTATE_CLEAR || 
		g_GameState == GAMESTATE_GAMEOVER)
	{
		// �J�����̍X�V����
		UpdateCamera();

		// ���C�g�̍X�V����
		UpdateLight();

		// �����̍X�V����
		UpdateExplosion();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �e�̍X�V����
		UpdateBullet();

		// �r���{�[�h�̍X�V����
		UpdateBillboard();

		// �͈͂̍X�V����
		UpdateRange();

		// �|���S���̍X�V����
		UpdateFLOOR();

		// �h�[���̍X�V����
		UpdateDome();

		// �����̍X�V����
		UpdateHemisphere();

		// �ǂ̍X�V����
		UpdateWall();

		// �e�̍X�V����
		UpdateShadow();

		// �\�����̍X�V����
		UpdatePrediction();

		// �}�e���A���̍X�V����
		UpdateMaterials();

		// ���f���̍X�V����
		UpdatePlayer();

		// �A�C�e���̍X�V����
		UpdateItem();

		// �L�����̍X�V����
		UpdateGiraffe();

		// �L�����̍X�V����
		UpdateShogira();

		// �o�b�t�@���[�̍X�V����
		UpdateBuffalo();

		// �S�b�̉��̍X�V����
		UpdateKing_Rion();

		// ���̍X�V����
		UpdateWATER();

		// �O�Ղ̍X�V����
		UpdateTrajectory();

		// HP�Q�[�W�̍X�V����
		UpdateHpGauge();

		// UI�̍X�V����
		UpdateUi();

		// �^�C���̍X�V����
		UpdateTime();

		// ���j���̍X�V����
		UpdateScore();

		// �|�[�Y��
		if (GetKeyboardTrigger(DIK_P) ||
			GetJoyboardTrigger(0, JOYPADKEY_START))
		{
			SetGameState(GAMESTATE_POUSE);
		}
	}

	else if (g_GameState == GAMESTATE_POUSE)
	{
		// �|�[�Y�w�i
		UpdatePouse();

		// �R�����g
		UpdatePouseComent();

		// �I��
		UpdateArrow();

		if (GetKeyboardTrigger(DIK_P) ||
			GetJoyboardTrigger(0, JOYPADKEY_START))
		{
			SetGameState(GAMESTATE_NORMAL);
		}
	}
#ifdef _DEBUG
	// ���f���̈ʒu�E��]���
	if (GetKeyboardTrigger(DIK_1))
	{
		g_nNumberDebug = 0;
	}

	// �J�����̈ʒu���
	else if (GetKeyboardTrigger(DIK_2))
	{
		g_nNumberDebug = 1;
	}

	// �e�̈ʒu���
	else if (GetKeyboardTrigger(DIK_3))
	{
		g_nNumberDebug = 2;
	}

	// �L�������
	else if (GetKeyboardTrigger(DIK_4))
	{
		g_nNumberDebug = 3;
	}

	else if (GetKeyboardTrigger(DIK_5))
	{
		g_nNumberDebug = 4;
	}
#endif

	switch (g_GameState)
	{
	case GAMESTATE_CLEAR:
		g_nCntGameState++;
		if (g_nCntGameState >= 30)
		{
			g_GameState = GAMESTATE_NONE;
			SetFade(MODE_CLEAR);
		}
		break;

	case GAMESTATE_GAMEOVER:
		g_nCntGameState++;
		if (g_nCntGameState >= 30)
		{
			g_GameState = GAMESTATE_NONE;
			SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_POUSE:
		break;

	case GAMESTATE_TITLE:
		g_GameState = GAMESTATE_NONE;

		SetFade(MODE_TITLE);
		break;

	case GAMESTATE_SELECT:
		// �I���̍X�V����
		UpdateSelect();

		// �I���R�����g�̍X�V����
		UpdateSelectComment();

		// ���̍X�V����
		UpdateArrow();

		break;
	default:
		break;
	}

	// �Q�[���t�F�[�h�̍X�V����
	UpdateGameFade();
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

	// �|���S���̕`�揈��
	DrawFLOOR();

	// �ǂ̕`�揈��
	DrawWall();

	// �h�[���̕`�揈��
	DrawDome();

	// �����̕`�揈��
	DrawHemisphere();

	// �e�̕`�揈��
	DrawBullet();

	// �r���{�[�h�̕`�揈��
	DrawBillboard();

	// �͈͂̏I������
	DrawRange();

	// �����̕`�揈��
	DrawExplosion();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �e�̕`�揈��
	DrawShadow();

	// �\�����̕`�揈��
	DrawPrediction();

	// �}�e���A���̕`�揈��
	DrawMaterials();

	// ���f���̕`�揈��
	DrawPlayer();

	// �A�C�e���̕`�揈��
	DrawItem();

	// �L�����̕`�揈��
	DrawGiraffe();

	// �L�����̕`�揈��
	DrawShogira();

	// �o�b�t�@���[�̂̕`�揈��
	DrawBuffalo();

	// �S�b�̉��̕`�揈��
	DrawKing_Rion();

	// �O�Ղ̕`�揈��
	DrawTrajectory();

	// ���̕`�揈��
	DrawWATER();

	// HP�Q�[�W�̕`�揈��
	DrawHpGauge();

	// UI�̕`�揈��
	DrawUi();
	
	// �^�C���̕`�揈��
	DrawTime();

	// ���j���̕`�揈��
	DrawScore();

	switch (g_GameState)
	{
	case GAMESTATE_POUSE:
		// �|�[�Y�w�i
		DrawPouse();

		// �R�����g
		DrawPouseComent();

		// �I��
		DrawArrow();

		break;

	case GAMESTATE_SELECT:
		// �I���̕`�揈��
		DrawSelect();

		// �I���R�����g�̕`�揈��
		DrawSelectComment();

		// ���̕`�揈��
		DrawArrow();
		break;

	}

	// �Q�[���t�F�[�h�̕`�揈��
	DrawGameFade();

#ifdef _DEBUG
	// ���f��
	if (g_nNumberDebug == 0)
	{
		// ���f�����\��
		Draw_Debug_PlayerPos();
	}

	// �J�����̈ʒu���
	else if (g_nNumberDebug == 1)
	{
		// �J�������\��
		Draw_Debug_Camera();
	}

	// �e�̈ʒu���
	else if (g_nNumberDebug == 2)
	{
		// �e���\��
		Draw_Debug_ShadowPos();
	}

	// �L�������
	else if (g_nNumberDebug == 3)
	{
		// �L�������\��
		Draw_Debug_GiraffeInfo();
	}

	else if (g_nNumberDebug == 4)
	{
		// �A�[�N�^���W�F���g�����\��
		Draw_Debug_ATAN2F();
	}
#endif
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

	// �{�X��Ԃ�������
	if (g_GameState == GAMESTATE_BOSS)
	{
		// �J�����f�[�^�̈ʒu���
		CameraPosition();

		// �v���C���[�f�[�^�̈ʒu���
		PlayerPosition();

		// �r���{�[�h�̏���������
		InitBillboard();

		// �͈͂̏���������
		InitRange();

		// �h�[���̏���������
		InitDome();

		// �|���S���̏���������
		InitFLOOR();

		// �ǂ̏���������
		InitWall();

		// �}�e���A���̏���������
		InitMaterials();

		// �A�C�e���̏���������
		InitItem();

		// �L�����̏���������
		InitGiraffe();

		// �L�����̏���������
		InitShogira();

		// �o�b�t�@���[�̏���������
		InitBuffalo();

		// �S�b�̉��̏���������
		InitKing_Rion();
	}
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

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// ���f���̃|�W�V��������
// ------------------------------------------------------------------------------------------
void Draw_Debug_PlayerPos(void)
{
	PLAYER *pPlayer = GetPlayer();	// ���f���擾

	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	char aStr[128];										// �����i�[
	char aAllStr[1024];									// �ꊇ�Ɋi�[

														//-------------------- �R�����g --------------------//
														// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "//------------------------------//\n");

	// �ꊇ�ϐ���
	strcpy(&aAllStr[0], &aStr[0]);


	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "// [Q][A]:X���ړ�\t[Z]:X�����Z�b�g//\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "// [W][S]:Y���ړ�\t[X]:Y�����Z�b�g//\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "// [E][D]:Z���ړ�\t[C]:Z�����Z�b�g//\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "//------------------------------//\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̃|�W�V����>\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);
	//-------------------- �R�����g�I�� --------------------//


	//-------------------- ��]�\�� --------------------//
	// �J�E���g�f�o�b�O
	for (int nCntDebug = 0; nCntDebug < PLAYERTYPE_MAX; nCntDebug++)
	{
		// �ϐ��ɕ����𓖂Ă�
		sprintf(&aStr[0], "[%d]:(%0.3f,%0.3f,%0.3f)\n",
			nCntDebug,
			pPlayer->model[nCntDebug].rot.x,
			pPlayer->model[nCntDebug].rot.y,
			pPlayer->model[nCntDebug].rot.z);

		// �ꊇ�ϐ���
		strcat(&aAllStr[0], &aStr[0]);
	}
	//-------------------- ��]�\���I�� --------------------//

	// �e�L�X�g�`��
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// �J�����̃|�W�V��������
// ------------------------------------------------------------------------------------------
void Draw_Debug_Camera(void)
{
	CAMERA *pCamera;
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	char aStr[64];										// �����i�[
	char aAllStr[1024];									// �ꊇ�i�[

	// �J�������擾
	pCamera = GetCamera();

	//-------------------- �R�����g --------------------//
	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̃|�W�V����>\n");

	// �ꊇ�ϐ���
	strcpy(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[posV](%0.3f,%0.3f,%0.3f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[posR](%0.3f,%0.3f,%0.3f)\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[posU](%0.3f,%0.3f,%0.3f)\n", pCamera->posU.x, pCamera->posU.y, pCamera->posU.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	//-------------------- �R�����g --------------------//
	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̉�]����>\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[rot](%0.3f,%0.3f,%0.3f)\n", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �e�L�X�g�`��
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// �e�̃|�W�V��������
// ------------------------------------------------------------------------------------------
void Draw_Debug_ShadowPos(void)
{
	SHADOW *pShadow;									// �e�̏��
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	char aStr[64];										// �����i�[
	char aAllStr[256];									// �ꊇ�i�[

														// �J�������擾
	pShadow = GetShadow();

	//-------------------- �R�����g --------------------//
	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̃|�W�V����>\n");

	// �ꊇ�ϐ���
	strcpy(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[posV](%0.3f,%0.3f,%0.3f)\n", pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �e�L�X�g�`��
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// �e�̃|�W�V��������
// ------------------------------------------------------------------------------------------
void Draw_Debug_GiraffeInfo(void)
{
	GIRAFFE *pGiraffe;
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	char aStr[64];										// �����i�[
	char aAllStr[1024];									// �ꊇ�i�[

	// �J�������擾
	pGiraffe = GetGiraffe();

	//-------------------- �R�����g --------------------//
	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̃|�W�V����>\n");

	// �ꊇ�ϐ���
	strcpy(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[pos](%0.3f,%0.3f,%0.3f)\n", pGiraffe->pos.x, pGiraffe->pos.y, pGiraffe->pos.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̉�]����>\n");

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[rot](%0.3f,%0.3f,%0.3f)\n", pGiraffe->rot.x, pGiraffe->rot.y, pGiraffe->rot.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �e�L�X�g�`��
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// �A�[�N�^���W�F���g�̎�������
// ------------------------------------------------------------------------------------------
void Draw_Debug_ATAN2F(void)
{
	KING_RION * pKing_Rion = GetKing_Rion();			// �S�b�̉��̏��擾
	PLAYER * pPlayer = GetPlayer();						// �v���C���[���擾
	D3DXVECTOR3 diffpos;								// �S�b�̉��ƃv���C���[�܂ł̋���
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// �\���ꏊ�E�͈�
	char aStr[64];										// �����i�[
	char aAllStr[1024];									// �ꊇ�i�[
	float	fAngle;										// �S�b�̉����猩�ăv���C���[������p�x
	
	// �v�Z
	diffpos = pPlayer->pos - pKing_Rion->pos;	// �v���C���[�ƕS�b�̉��Ƃ̋���
	fAngle = (atan2f(diffpos.x, diffpos.z));	// �S�b�̉�����v���C���[�̊p�x

	//-------------------- �R�����g --------------------//
	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "<���݂̃|�W�V����>\n");

	// �ꊇ�ϐ���
	strcpy(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[�S�b�̉���pos](%0.3f,%0.3f,%0.3f)\n", 
		pKing_Rion->pos.x, pKing_Rion->pos.y, pKing_Rion->pos.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[��l����pos](%0.3f,%0.3f,%0.3f)\n",
		pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[����](%0.3f,%0.3f,%0.3f)\n", diffpos.x, diffpos.y, diffpos.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[�S�b�̉���rot](%0.3f,%0.3f,%0.3f)\n",
		pKing_Rion->rotLast.x, pKing_Rion->rotLast.y, pKing_Rion->rotLast.z);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �ϐ��ɕ����𓖂Ă���
	sprintf(&aStr[0], "[atan2f](%0.3f)\n", fAngle);

	// �ꊇ�ϐ���
	strcat(&aAllStr[0], &aStr[0]);

	// �e�L�X�g�`��
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif // _DEBUG