// ------------------------------------------
//
// �`���[�g���A������ [tutorial.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "tutorial.h"
#include "tutorial_ui.h"

/* �`�� */
#include "fade.h"

// ------------------------------------------
//
// �}�N����`
//
// ------------------------------------------

// ------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------

// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CTutorial::CTutorial()
{
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CTutorial::~CTutorial()
{
}

// ------------------------------------------
// ������
// ------------------------------------------
void CTutorial::Init(void)
{
	CTutorial_ui::Create();
}

// ------------------------------------------
// �I��
// ------------------------------------------
void CTutorial::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------
// �X�V
// ------------------------------------------
void CTutorial::Update(void)
{
}

// ------------------------------------------
// �`��
// ------------------------------------------
void CTutorial::Draw(void)
{
}