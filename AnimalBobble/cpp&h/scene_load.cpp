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
#include "effect.h"
#include "tlybg.h"
#include "frame.h"
#include "ceiling.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "number.h"
#include "box.h"
#include "mark.h"
#include "prediction.h"
#include "rank_ui.h"
#include "topscore.h"
#include "title_ui.h"
#include "selection_ui.h"
#include "pause_ui.h"
#include "game_ui.h"
#include "play_element_ui.h"
#include "tutorial_ui.h"
#include "line.h"

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
	// �g
	CFrame::Load();
	// �V��
	CCeiling::Load();
	// �G�t�F�N�g
	CEffect::Load();
	// ����
	CExplosion::Load();
	// �v���C���[
	CPlayer::Load();
	// �\����
	CPrediction::Load();
	// �e
	CBullet::Load();
	// �ԍ�
	CNumber::Load();
	// ��
	CBox::Load();
	// �ڈ�
	CMark::Load();
	// �^�C�g��UI
	CTitle_ui::Load();
	// �`���[�g���A��UI
	CTutorial_ui::Load();
	// �I��UI
	CSelection_ui::Load();
	// �����N�pUI
	CRank_ui::Load();
	// ��ʃX�R�A
	CTopscore::Load();
	// �v���C���[�pUI
	CPlay_element_ui::Load();
	// �Q�[���pUI
	CGame_ui::Load();
	// �|�[�YUI
	CPause_ui::Load();
	// ��
	CLine::Load();
}

// ----------------------------------------
// �ǂݍ��񂾂��̂�j�����鏈��
// ----------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* �ǂݍ��񂾂��̂̔j��*/
	// �w�i
	CTlyBg::UnLoad();
	// �g
	CFrame::UnLoad();
	// �V��
	CCeiling::UnLoad();
	// �G�t�F�N�g
	CEffect::UnLoad();
	// ����
	CExplosion::UnLoad();
	// �v���C���[
	CPlayer::UnLoad();
	// �\����
	CPrediction::UnLoad();
	// �e
	CBullet::UnLoad();
	// �ԍ�
	CNumber::UnLoad();
	// ��
	CBox::UnLoad();
	// �ڈ�
	CMark::UnLoad();
	// �^�C�g��UI
	CTitle_ui::UnLoad();
	// �`���[�g���A��UI
	CTutorial_ui::UnLoad();
	// �I��UI
	CSelection_ui::UnLoad();
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
	CLine::UnLoad();
}