// ------------------------------------------------------------------------------------------
//
// �v�f�w�i�����̐���[element.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "element.h"
#include "input.h"
#include "joypad.h"
#include "mouse.h"
#include "camera.h"
#include "light.h"
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
#include "trajectory.h"
#include "buffalo.h"
#include "item.h"
#include "king_rion.h"
#include "Billboard.h"
#include "range.h"
#include "water.h"
#include "shogira.h"
#include "prediction.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �v�f�w�i����������
//
// ------------------------------------------------------------------------------------------
void InitElement(void)
{
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

	// ���̏���������
	InitWATER();
}

// ------------------------------------------------------------------------------------------
//
// �v�f�w�i�I������
//
// ------------------------------------------------------------------------------------------
void UninitElement(void)
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

	// �O�Ղ̏I������
	UninitTrajectory();

	// ���̏I������
	UninitWATER();
}

// ------------------------------------------------------------------------------------------
//
// �v�f�w�i�X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateElement(void)
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

	// �O�Ղ̍X�V����
	UpdateTrajectory();

	// ���̍X�V����
	UpdateWATER();
}

// ------------------------------------------------------------------------------------------
//
// �v�f�w�i�`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawElement(void)
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
}