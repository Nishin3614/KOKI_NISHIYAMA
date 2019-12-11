// ------------------------------------------
//
// �Q�[������ [game.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "game.h"
/* �`�� */
#include "tlybg.h"
#include "score.h"
#include "number.h"
#include "fade.h"
#include "item.h"
#include "floor.h"
#include "player.h"
#include "item.h"
#include "play_element_ui.h"
#include "scene_X.h"
#include "3Deffect.h"
#include "meshsphere.h"
#include "block.h"
#include "goal.h"
#include "time.h"
#include "score.h"
#include "player.h"
#include "gauge.h"
#include "Waypoint.h"

/* �����蔻�� */
#include "collision.h"

/* �|�[�Y */
#include "pause.h"

/* �C�N�~���ǉ� */
#include "GameUI.h"


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
CGame::STATE CGame::m_state = CGame::STATE_NORMAL;
int CGame::m_nStage = 0;	// �X�e�[�W
CPause *m_pPause[CPause::PAUSETYPE_MAX] = {};	// �|�[�Y�̏��
CPlayer *CGame::m_pPlayer = NULL;
CGauge *CGame::m_pGauge = NULL;
CTime *CGame::m_pTime = NULL;

// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CGame::CGame()
{
	m_state = CGame::STATE_NORMAL;
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CGame::~CGame()
{
}

// ------------------------------------------
// ������
// ------------------------------------------
void CGame::Init(void)
{
	// �ϐ��錾

	/* ������ */
	// ���
	m_state = STATE_NORMAL;
	// �X�e�[�W
	m_nStage = 0;
	CManager::PLAYER playerID = CManager::PLAYER_1;
	// �t���A�[
	CFloor::Create(
		D3DXVECTOR3(0.0f,50.0f,0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 1000.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		10, 10);
	// �v���C���[
	m_pPlayer = CPlayer::Create();
	// �u���b�N�̐���
	CBlock::LoadCreate();
	// �A�C�e���̐���
	CItem::LoadCreate();
	// effect
	C3DEffect::Create();
	// ���b�V���X�t�B�A
	CMeshsphere::create();
	CMeshsphere::Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f);
	CMeshsphere::SetRadius(0, 10000.0f);
	// �S�[���n�_
	CGoal::Create(
		D3DXVECTOR3(-100.0f, 3900.0f, 0.0f)
	);
	/*
	// ���Ԓn�_�n�_
	CWaypoint::Create(
		D3DXVECTOR3(0.0f, 2050.0f, 0.0f)
	);
	*/
	// �^�C��
	m_pTime = CTime::Create();
	// �X�R�A
	CScore::Create();
	/* �C�N�~���ǉ� */
	//CGameUI::Load();
	//CGameUI::Create();
	// �����蔻��
	CCollision::LoadCreate();
	m_pGauge = CGauge::Create();

}

// ------------------------------------------
// �I��
// ------------------------------------------
void CGame::Uninit(void)
{
	/* �C�N�~���ǉ� */
	CGameUI::Unload();
	// �V�[�����̍폜
	CScene::ReleaseAll();
	// �����蔻��̍폜
	CCollision::DeleteAll();
}

// ------------------------------------------
// �X�V
// ------------------------------------------
void CGame::Update(void)
{
	// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
	if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (CGame::GetTime()->GetTimeOverFlag())
		{
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}

	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_P) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_START))
	{
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			if (m_pPause[nCntPause] != NULL)
			{

				m_pPause[nCntPause]->Release();
				m_pPause[nCntPause] = NULL;
				CPause::SetPause(false);
			}
			else if (m_pPause[nCntPause] == NULL)
			{
				m_pPause[nCntPause] = m_pPause[nCntPause]->Create((CPause::PAUSETYPE)nCntPause);
				CPause::SetPause(true);
			}
		}
	}

	// �����蔻��
	CCollision::UpdateAll();
}

// ------------------------------------------
// �`��
// ------------------------------------------
void CGame::Draw(void)
{
}

// ------------------------------------------
// ��Ԏ擾
// ------------------------------------------
CGame::STATE CGame::GetState(void)
{
	return m_state;
}

// ------------------------------------------
// �X�e�[�W�擾
// ------------------------------------------
int CGame::GetStage(void)
{
	return m_nStage;
}

// ------------------------------------------
// ��Ԑݒ�
// ------------------------------------------
void CGame::SetState(STATE state)
{
	m_state = state;
}

CPlayer * CGame::GetPlayer(void)
{
	return m_pPlayer;
}

CGauge * CGame::GetGauge(void)
{
	return m_pGauge;
}

CTime * CGame::GetTime(void)
{
	return m_pTime;
}