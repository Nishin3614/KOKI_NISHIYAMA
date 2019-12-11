// ------------------------------------------
//
// �^�C�g������ [title.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "title.h"
/* �`�� */
#include "tlybg.h"
#include "title_ui.h"
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
CTitle::CTitle()
{
	m_nCntChange = 0;
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CTitle::~CTitle()
{
}

// ------------------------------------------
// ������
// ------------------------------------------
void CTitle::Init(void)
{
	/* �쐬 */
	// �w�i
	CTlyBg::Create(CManager::MODE_TITLE);
	// UI
	CTitle_ui::Create();
	// �J�E���g�J��
	m_nCntChange = 0;
}

// ------------------------------------------
// �I��
// ------------------------------------------
void CTitle::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------
// �X�V
// ------------------------------------------
void CTitle::Update(void)
{
	// �J�E���g�J��
	m_nCntChange++;
	// �����L���O�J��
	if (m_nCntChange >= INTERVAL_TIME)
	{
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{		
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	// �I����ʑJ��
	//�L�[�{�[�h
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			// �G���^�[��
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		}
	}
	//�W���C�X�e�B�b�N
	else if (CManager::GetJoy()->GetPress(0, CJoypad::KEY_A)||
		CManager::GetJoy()->GetPress(0, CJoypad::KEY_B)||
		CManager::GetJoy()->GetPress(0, CJoypad::KEY_START))
	{
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			// �G���^�[��
			//CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);
		}
	}
}

// ------------------------------------------
// �`��
// ------------------------------------------
void CTitle::Draw(void)
{
}