// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �V�[�������̐���[scene_load.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_load.h"
/* �`�� */
#include "score.h"
#include "number.h"
#include "mark.h"
#include "rank_ui.h"
#include "topscore.h"
#include "title_ui.h"
#include "pause_ui.h"
#include "game_ui.h"
#include "play_element_ui.h"
#include "tutorial_ui.h"
#include "2Dgauge.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "item.h"
#include "floor.h"
#include "meshwall.h"
#include "meshdome.h"
#include "meshsphere.h"
#include "collision.h"
#include "trajectory.h"
#include "camera.h"
#include "forceline.h"
#include "player.h"
#include "rothuman.h"
#include "staticobj.h"
#include "stategauge.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_load::CScene_load()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_load::~CScene_load()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �V�[���e�q�쐬����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_load::LoadAll(void)
{
	/* ----------�e�N�X�`���[�ǂݍ���---------- */
	/* UI */
	// �^�C�g��UI
	if(!CTitle_ui::Load()== S_OK)
	{
		CCalculation::Messanger("�^�C�g��UI�ǂݎ�莸�s");
	}
	// �`���[�g���A��UI
	if (!CTutorial_ui::Load()== S_OK)
	{
		CCalculation::Messanger("�`���[�g���A��UI�ǂݎ�莸�s");
	}
	// �����N�pUI
	if (!CRank_ui::Load()== S_OK)
	{
		CCalculation::Messanger("�����NUI�ǂݎ�莸�s");
	}
	// ��ʃX�R�A
	if (!CTopscore::Load()== S_OK)
	{
		CCalculation::Messanger("��ʃX�R�A�ǂݎ�莸�s");
	}
	// �v���C���[�pUI
	if(!CPlay_element_ui::Load()== S_OK)
	{
		CCalculation::Messanger("�v���C���[UI�ǂݎ�莸�s");
	}
	// �Q�[���pUI
	if(!CGame_ui::Load()== S_OK)
	{
		CCalculation::Messanger("�Q�[��UI�ǂݎ�莸�s");
	}
	// �|�[�YUI
	if(!CPause_ui::Load()== S_OK)
	{
		CCalculation::Messanger("�|�[�YUI�ǂݎ�莸�s");
	}
	/* ���� */
	// 3Deffect
	if(!C3DEffect::Load()== S_OK)
	{
		CCalculation::Messanger("3D�G�t�F�N�g�ǂݎ�莸�s");
	}
	// 3Dparticle
	if(!C3DParticle::Load()== S_OK)
	{
		CCalculation::Messanger("3D�p�[�e�B�N���ǂݎ�莸�s");
	}
	// �O��
	if (!CTrajectory::Load() == S_OK)
	{
		CCalculation::Messanger("�O�Փǂݎ�莸�s");
	}

	/* �Q�[���ɕK�v�Ȃ��� */
	// �����蔻��
	if(!CCollision::Load()== S_OK)
	{
		CCalculation::Messanger("�����蔻��ǂݎ�莸�s");
	}
	// �J����
	if (!CCamera::Load() == S_OK)
	{
		CCalculation::Messanger("�J�����ǂݎ�莸�s");
	}
	// �X�e�[�W�Q�[�W
	if (!CStateGauge::Load() == S_OK)
	{
		CCalculation::Messanger("�X�e�[�g�Q�[�W�ǂݎ�莸�s");
	}
	// 2D�Q�[�W
	if (!C2DGauge::Load() == S_OK)
	{
		CCalculation::Messanger("2D�Q�[�W�ǂݎ�莸�s");
	}

	/* 2D�I�u�W�F�N�g */
	// �W����
	if (!CForceline::Load() == S_OK)
	{
		CCalculation::Messanger("�W�����ǂݎ�莸�s");
	}
	// �ԍ�
	if (!CNumber::Load() == S_OK)
	{
		CCalculation::Messanger("�ԍ��ǂݎ�莸�s");
	}
	// �ڈ�
	if (!CMark::Load() == S_OK)
	{
		CCalculation::Messanger("�ڈ�ǂݎ�莸�s");
	}

	/* 3D�I�u�W�F�N�g*/

	/* ���b�V�� */
	// ��
	if (!CFloor::Load() == S_OK)
	{
		CCalculation::Messanger("���ǂݎ�莸�s");
	}
	// ���b�V���h�[��
	if (!CMeshdome::Load() == S_OK)
	{
		CCalculation::Messanger("���b�V���h�[���ǂݎ�莸�s");
	}
	// ���b�V���X�t�B�A
	if (!CMeshsphere::Load() == S_OK)
	{
		CCalculation::Messanger("���b�V���X�t�B�A�ǂݎ�莸�s");
	}
	// ���b�V���E�H�[��
	if (!CMeshwall::Load() == S_OK)
	{
		CCalculation::Messanger("���b�V���E�H�[���ǂݎ�莸�s");
	}

	/* 3D���f��*/
	// �v���C���[
	if(!CPlayer::Load()== S_OK)
	{
		CCalculation::Messanger("�v���C���[�ǂݎ�莸�s");
	}
	// �_�l��
	if (!CRothuman::Load() == S_OK)
	{
		CCalculation::Messanger("�_�l�ԓǂݎ�莸�s");
	}
	// �ÓI�I�u�W�F�N�g
	if (!CStaticobj::Load() == S_OK)
	{
		CCalculation::Messanger("�ÓI�I�u�W�F�N�g�ǂݎ�莸�s");
	}
	// �A�C�e��
	if (!CItem::Load() == S_OK)
	{
		CCalculation::Messanger("�A�C�e���ǂݎ�莸�s");
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾂��̂�j�����鏈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* �ǂݍ��񂾂��̂̔j��*/
	// �ԍ�
	CNumber::UnLoad();
	// �ڈ�
	CMark::UnLoad();
	// �^�C�g��UI
	CTitle_ui::UnLoad();
	// �`���[�g���A��UI
	CTutorial_ui::UnLoad();
	// �����N�pUI
	CRank_ui::UnLoad();
	// ��ʃX�R�A
	CTopscore::Save();
	// �v���C���[�pUI
	CPlay_element_ui::UnLoad();
	// �Q�[���pUI
	CGame_ui::UnLoad();
	// �|�[�YUI
	CPause_ui::UnLoad();
	// ��
	CFloor::UnLoad();
	// �v���C���[
	CPlayer::UnLoad();
	// �_�l��
	CRothuman::UnLoad();
	// 3Deffect
	C3DEffect::Unload();
	// 3Dparticle
	C3DParticle::Unload();
	// �O��
	CTrajectory::Unload();
	// ���b�V���h�[��
	CMeshdome::UnLoad();
	// ���b�V���X�t�B�A
	CMeshsphere::Unload();
	// �A�C�e��
	CItem::UnLoad();
	// 2D�Q�[�W
	C2DGauge::UnLoad();
	// �W����
	CForceline::UnLoad();
	// �ÓI�I�u�W�F�N�g
	CStaticobj::UnLoad();
	// ���b�V���E�H�[��
	CMeshwall::UnLoad();
}