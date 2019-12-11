// ----------------------------------------
//
// �V�[�������̐���[scene_load.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_load.h"

/* �`�� */
#include "tlybg.h"
#include "score.h"
#include "number.h"
#include "mark.h"
#include "rank_ui.h"
#include "title_ui.h"
#include "play_element_ui.h"
#include "tutorial_ui.h"
#include "floor.h"
#include "player.h"
#include "item.h"
#include "pause.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "block.h"
#include "meshdome.h"
#include "collision.h"
#include "gauge.h"
#include "meshsphere.h"
#include "topscore.h"

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CScene_load::CScene_load()
{
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CScene_load::~CScene_load()
{
}

// ----------------------------------------
// �V�[���e�q�쐬����
// ----------------------------------------
void CScene_load::LoadAll(void)
{
	/* �e�N�X�`���[�ǂݍ��� */
	// �w�i
	CTlyBg::Load();
	// �ԍ�
	CNumber::Load();
	// �ڈ�
	CMark::Load();
	// �^�C�g��UI
	CTitle_ui::Load();
	// �`���[�g���A��UI
	CTutorial_ui::Load();
	// �����N�pUI
	CRank_ui::Load();
	// �v���C���[�pUI
	CPlay_element_ui::Load();
	// ��
	CFloor::Load();
	// ���b�V���h�[��
	CMeshdome::Load();
	// ���b�V���X�t�B�A
	CMeshsphere::Load();
	// �v���C���[
	CPlayer::Load();
	// �|�[�Y
	CPause::Load();
	// �A�C�e��
	CItem::Load();
	// �u���b�N
	CBlock::Load();
	// 3Deffect
	C3DEffect::Load();
	// 3Dparticle
	C3DParticle::Load();
	// �����蔻��
	CCollision::Load();
	// �Q�[�W
	CGauge::Load();
	// ��ʃX�R�A
	CTopscore::Load();
}

// ----------------------------------------
// �ǂݍ��񂾂��̂�j�����鏈��
// ----------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* �ǂݍ��񂾂��̂̔j��*/
	// �w�i
	CTlyBg::UnLoad();
	// �ڈ�
	CMark::UnLoad();
	// �^�C�g��UI
	CTitle_ui::UnLoad();
	// �`���[�g���A��UI
	CTutorial_ui::UnLoad();
	// �����N�pUI
	CRank_ui::UnLoad();
	// �v���C���[�pUI
	CPlay_element_ui::UnLoad();
	// ��
	CFloor::UnLoad();
	// ���b�V���h�[��
	CMeshdome::UnLoad();
	// ���b�V���X�t�B�A
	CMeshsphere::Unload();
	// �v���C���[
	CPlayer::UnLoad();
	// �|�[�Y
	CPause::Unload();
	// �A�C�e��
	CItem::UnLoad();
	// �u���b�N
	CBlock::UnLoad();
	// 3Deffect
	C3DEffect::Unload();
	// 3Dparticle
	C3DParticle::Unload();
	// �Q�[�W
	CGauge::UnLoad();
}