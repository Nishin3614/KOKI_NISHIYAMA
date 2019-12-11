// ------------------------------------------------------------------------------------------
//
// �`���[�g���A�������̐���[tutorial.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "tutorial.h"
#include "tutorialcomment.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "titlecomment.h"
#include "camera.h"
#include "floor.h"
#include "wall.h"
#include "materials.h"
#include "giraffe.h"
#include "light.h"
#include "shadow.h"
#include "trajectory.h"
#include "hemisphere.h"
#include "dome.h"
#include "player.h"
#include "Billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "item.h"
#include "hpgauge.h"
#include "range.h"
#include "select.h"
#include "selectcomment.h"
#include "arrow.h"
#include "buffalo.h"
#include "water.h"
#include "shogira.h"
#include "prediction.h"

// ------------------------------------------------------------------------------------------
//
// �`���[�g���A������������
//
// ------------------------------------------------------------------------------------------
int g_TutorialState = TUTORIALSSTATE_NORMAL;	// �ʏ���

// ------------------------------------------------------------------------------------------
//
// �`���[�g���A������������
//
// ------------------------------------------------------------------------------------------

void InitTutorial(void)
{
	// ��Ԃ̏�����
	g_TutorialState = TUTORIALSSTATE_NORMAL;	// �ʏ���

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

	// �v���C���[�̏���������
	InitPlayer();

	// �A�C�e���̏���������
	InitItem();

	// �L�����̏���������
	InitGiraffe();

	// �L�����̏���������
	InitShogira();

	// �o�b�t�@���[�̏���������
	InitBuffalo();

	// ���̏���������
	InitWATER();

	// HP�Q�[�W�̏���������
	InitHpGauge();

	// �`���[�g���A���R�����g������
	InitTutorialComment();

	// �I���̏�����
	InitSelect();

	// �I���R�����g�̏�����
	InitSelectComment();

	// �I���R�����g�̃^�C�v
	SceneSelectComment(SELECTCOMMENTSCENE_TUTORIALS);

	// ���̏�����
	InitArrow();
}

// ------------------------------------------------------------------------------------------
//
// �`���[�g���A���I������
//
// ------------------------------------------------------------------------------------------

void UninitTutorial(void)
{
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

	// �r���{�[�h�̏I������
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

	// ���̏I������
	UninitWATER();

	// �O�Ղ̏I������
	UninitTrajectory();

	// HP�Q�[�W�̏I������
	UninitHpGauge();

	// �`���[�g���A���R�����g�I������
	UninitTutorialComment();

	// �I���̏I������
	UninitSelect();

	// �I���R�����g�̏I������
	UninitSelectComment();

	// ���̏I������
	UninitArrow();
}

// ------------------------------------------------------------------------------------------
//
// �`���[�g���A���X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateTutorial(void)
{
	switch (g_TutorialState)
	{
	case TUTORIALSSTATE_NORMAL:
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
		//UpdateBuffalo();

		// ���̍X�V����
		UpdateWATER();

		// �O�Ղ̍X�V����
		UpdateTrajectory();

		// HP�Q�[�W�̍X�V����
		UpdateHpGauge();

		// �`���[�g���A���R�����g�X�V����
		UpdateTutorialComment();

		break;

	case TUTORIALSSTATE_SELECT:
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
}

// ------------------------------------------------------------------------------------------
//
// �`���[�g���A���`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawTutorial(void)
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

	// �o�b�t�@���[�̕`�揈��
	DrawBuffalo();

	// �r���{�[�h�̕`�揈��
	DrawBillboard();

	// �O�Ղ̕`�揈��
	DrawTrajectory();

	// ���̕`�揈��
	DrawWATER();

	// HP�Q�[�W�̕`�揈��
	DrawHpGauge();

	// �`���[�g���A���R�����g�`�揈��
	DrawTutorialComment();

	// �I����
	if (g_TutorialState == TUTORIALSSTATE_SELECT)
	{
		// �I���̕`�揈��
		DrawSelect();

		// �I���R�����g�̕`�揈��
		DrawSelectComment();

		// ���̕`�揈��
		DrawArrow();
	}
}

// ------------------------------------------------------------------------------------------
//
// �`���[�g���A���ݒ菈��
//
// ------------------------------------------------------------------------------------------
void SetTutorialState(TUTORIALSSTATE state)
{
	g_TutorialState = state;
}
