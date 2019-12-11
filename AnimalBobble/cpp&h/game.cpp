// ------------------------------------------------------------------------------------------
//
// �Q�[������ [game.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "game.h"
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
#include "array.h"
#include "fade.h"
#include "game_ui.h"
#include "play_element_ui.h"
#include "topscore.h"

/* �|�[�Y */
#include "pause.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------------------------------------------------------
CGame::STATE CGame::m_state = CGame::STATE_NORMAL;	// �Q�[�����
int CGame::m_nStage = 0;							// �X�e�[�W

// ------------------------------------------------------------------------------------------
// �R���X�g���N�^
// ------------------------------------------------------------------------------------------
CGame::CGame()
{
	m_state = CGame::STATE_NORMAL;
	m_pause = NULL;
}

// ------------------------------------------------------------------------------------------
// �f�X�g���N�^
// ------------------------------------------------------------------------------------------
CGame::~CGame()
{
}

// ------------------------------------------------------------------------------------------
// ������
// ------------------------------------------------------------------------------------------
void CGame::Init(void)
{
	// �ϐ��錾
	CFrame * pFrame;								// �t���[��
	D3DXVECTOR3 FramePos;							// �t���[���ʒu
	D3DXVECTOR2 FrameSize;							// �t���[���T�C�Y
	CManager::PLAYER playerID = CManager::PLAYER_1;	// �v���C���[�ԍ�

	/* ������ */
	// ���
	m_state = STATE_NORMAL;
	// �X�e�[�W
	m_nStage = 0;

	/* �쐬 */
	// �w�i
	CTlyBg::Create(CManager::MODE_GAME);
	// �g
	pFrame = CFrame::Create(playerID, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50.0f, 0.0f));
	// ���擾
	FramePos = pFrame->GetPos();	// �ʒu
	FrameSize = pFrame->GetSize();	// �T�C�Y
	// �V��
	CCeiling::Create(
		playerID,
		D3DXVECTOR3(FramePos.x, FramePos.y - FrameSize.y / 2, 0.0f),
		D3DXVECTOR2(FrameSize.x / 2, 1.0f));
	// �X�R�A
	CScore::Create(
		playerID,
		D3DXVECTOR3(FramePos.x,FramePos.y - FrameSize.y / 2 -10.0f,0.0f));
	// ��
	CBox::Create(playerID, D3DXVECTOR3(FramePos.x - 200.0f, FramePos.y + FrameSize.y / 2 - 50.0f, 0.0f));
	// �v���C���[
	CPlayer::Create(playerID,D3DXVECTOR3(FramePos.x, FramePos.y + FrameSize.y / 2 - 50.0f,0.0f));
	// �z��
	CArray::Create(playerID,FramePos,FrameSize);
	// �e�̓ǂݍ���
	CBullet::LoadStage();
	// �e�̐���
	CBullet::LoadCreate(playerID, 0, m_nStage);
	// �v���C���[�pUI
	CPlay_element_ui::Create(playerID);
	// �Q�[���pUI
	CGame_ui::Create();
	// �|�[�Y�̐���
	m_pause = new CPause();
	// �|�[�Y�̏�����
	m_pause->Init();
}

// ------------------------------------------------------------------------------------------
// �I��
// ------------------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// �|�[�Y
	if (m_pause != NULL)
	{
		CPause::SetPause(false);
		m_pause->Uninit();
		delete m_pause;
		m_pause = NULL;
	}

	CScene::ReleaseAll();
}

// ------------------------------------------------------------------------------------------
// �X�V
// ------------------------------------------------------------------------------------------
void CGame::Update(void)
{
	// �ϐ��錾
	CGame_ui * pGame_ui = NULL;
	CScore * pScore = NULL;
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pGame_ui == NULL) pGame_ui = (CGame_ui*)CScene::GetActor(CScene::ACTOR_GAME_UI, CScene::LAYER_UI, nCntLayer);	// �Q�[��ui
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_OBJ, nCntLayer);			// �X�R�A
	}

	/* �|�[�Y�p */
	// �Q�[����Ԃ̕ύX
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_P))
	{
		// �Q�[����Ԃ��|�[�Y��
		if (m_state != STATE_PAUSE)
		{
			m_state = STATE_PAUSE;
			CPause::SetPause(true);
		}
		// �Q�[����Ԃ��m�[�}����
		else
		{
			m_state = STATE_NORMAL;
			CPause::SetPause(false);
		}
	}
	/* �N���A */
	// �������Ȃ��Ȃ�����
	if (CBullet::GetAllBullet() <= 0)
	{
		// �N���A��Ԃ���Ȃ��Ƃ�
		if (m_state != STATE_CLEAR)
		{
			// �N���A���̉�
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_CLEAR);
			m_state = STATE_CLEAR;
		}
	}
	// �N���A��ԂȂ��
	if (m_state == STATE_CLEAR)
	{
		// �G���^�[��������
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			// �X�e�[�W�A�b�v
			m_nStage++;
			pGame_ui->GetNumber()->SetNum(m_nStage + 1);

			// �l�N�X�g�X�e�[�W
			if (m_nStage < ONEPLAYER_STAGE)
			{
				// �e�̐���
				CBullet::LoadCreate(CManager::PLAYER_1, 0, m_nStage);
				// �m�[�}����Ԃ�
				m_state = STATE_START;
			}
			// �S���N���A
			else
			{
				// �t�F�C�h������Ȃ��ꍇ
				if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
				{
					// �m�[�}����Ԃ�
					m_state = STATE_NORMAL;
					m_nStage = 0;
					// MY�X�R�A��ݒ�
					CTopscore::SetScore(pScore->GetScore());
					CManager::GetFade()->SetFade(CManager::MODE_RANKING);	// �����L���O
				}
			}
		}
	}
	/* �X�^�[�g */
	else if (m_state == STATE_START)
	{
		// �G���^�[��������
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			// �m�[�}����Ԃ�
			m_state = STATE_NORMAL;
		}
	}
	// �Q�[���I�[�o�[
	else if (m_state == STATE_GAMEOVER)
	{
		// �G���^�[��������
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			// �t�F�C�h������Ȃ��ꍇ
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				// �m�[�}����Ԃ�
				m_state = STATE_NORMAL;
				m_nStage = 0;
				// MY�X�R�A��ݒ�
				CTopscore::SetScore(pScore->GetScore());
				// �G���^�[��
				CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
				CManager::GetFade()->SetFade(CManager::MODE_RANKING);	// �����L���O
			}
		}
	}
	// �|�[�Y��ԂȂ��
	else if (m_state == STATE_PAUSE)
	{
		if (m_pause != NULL)
		{
			m_pause->Update();
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`��
// ------------------------------------------------------------------------------------------
void CGame::Draw(void)
{
	if (m_state == STATE_PAUSE)
	{
		if (m_pause != NULL)
		{
			m_pause->Draw();
		}
	}
}

// ------------------------------------------------------------------------------------------
// ��Ԏ擾
// ------------------------------------------------------------------------------------------
CGame::STATE CGame::GetState(void)
{
	return m_state;
}

// ------------------------------------------------------------------------------------------
// �X�e�[�W�擾
// ------------------------------------------------------------------------------------------
int CGame::GetStage(void)
{
	return m_nStage;
}

// ------------------------------------------------------------------------------------------
// ��Ԑݒ�
// ------------------------------------------------------------------------------------------
void CGame::SetState(STATE state)
{
	m_state = state;
}
