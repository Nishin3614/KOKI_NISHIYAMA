// ------------------------------------------
//
// �����L���O���� [ranking.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "ranking.h"
/* �`�� */
#include "rank_ui.h"
#include "topscore.h"
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
CRanking::CRanking()
{
	m_CntChange = 0;
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CRanking::~CRanking()
{
}

// ------------------------------------------
// ������
// ------------------------------------------
void CRanking::Init(void)
{
	/* �쐬 */
	// �����N�pui
	CRank_ui::Create();
	// �J�E���g�J��
	m_CntChange = 0;
}

// ------------------------------------------
// �I��
// ------------------------------------------
void CRanking::Uninit(void)
{
	CScene::ReleaseAll();
}

// ------------------------------------------
// �X�V
// ------------------------------------------
void CRanking::Update(void)
{
	// �J�E���g�J�ڃA�b�v
	m_CntChange++;
	if (m_CntChange >= INTERVAL_TIME)
	{
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CTopscore::SetScore(0);
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	// ���[�h�̐ݒ�
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// �G���^�[��
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CTopscore::SetScore(0);
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	// �I����ʑJ��
	else if (CManager::GetJoy() != NULL)
	{
		// ���[�h�̐ݒ�
		if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_START) ||
			CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B) ||
			CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A))
		{
			// �G���^�[��
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
			// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CTopscore::SetScore(0);
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
		}
	}
}

// ------------------------------------------
// �`��
// ------------------------------------------
void CRanking::Draw(void)
{
}
