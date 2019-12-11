// ----------------------------------------
//
// �|�[�Y�pui�����̐���[pause_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "pause_ui.h"
#include "pause.h"
#include "topscore.h"
#include "fade.h"
#include "mark.h"
#include "game.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ----------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CPause_ui::m_pTex[PAUSE_UI_MAX] = {};
D3DXVECTOR3 CPause_ui::m_pos[PAUSE_UI_MAX] = {};
D3DXVECTOR2 CPause_ui::m_size[PAUSE_UI_MAX] = {};
int CPause_ui::m_nSelect = 2;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CPause_ui::CPause_ui() : CScene::CScene(ACTOR_PAUSE_UI, LAYER_UI)
{
	// ������
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_mark = NULL;
	m_nCntCol = 0;
	m_fMult = 1.0f;
	m_fAdd = 0;
	m_nCntCol = 0;
	m_nSelect = 2;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CPause_ui::~CPause_ui()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CPause_ui::Init(void)
{
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_aScene_Two[nCnt] = new CScene_TWO;
			m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
			m_aScene_Two[nCnt]->Init();
			if (nCnt == 0)
			{
				m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
			m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
			m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
			// �}�[�N�ݒ�
			if (m_pTex[nCnt] == m_pTex[PAUSE_UI_GAME])
			{
				// �ڈ�
				m_mark = CMark::Create(
					m_aScene_Two[nCnt]->GetPosition(),
					m_aScene_Two[nCnt]->GetSize(),
					ACTOR_PAUSE_UI);
				// �ڕW�̈ʒu�E�T�C�Y�̐ݒ菈��
				m_mark->SetPosGoal(m_aScene_Two[nCnt]->GetPosition(), m_aScene_Two[nCnt]->GetSize());
			}

		}
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CPause_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Uninit();
			delete m_aScene_Two[nCnt];
			m_aScene_Two[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CPause_ui::Update(void)
{
	D3DXVECTOR2 size = m_size[m_nSelect];	// �T�C�Y�̎擾

	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		// �k���`�F�b�N
		if (m_pTex[nCnt] != NULL)
		{
			// ��ނɂ���ď�����ς���
			m_aScene_Two[nCnt]->Update();
		}
	}

	// ������������
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_D) ||
		CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RIGHT))
	{
		// �I���A�b�v
		m_nSelect++;

		// �����������
		if (m_nSelect > PAUSE_UI_RESTART)
		{
			m_nSelect = PAUSE_UI_GAME;
		}
		// �T�C�Y�̐ݒ�
		m_aScene_Two[m_nSelect]->SetSize(size);
		// �{��
		m_fMult = 1.0f;
		// �ڕW�̈ʒu�E�T�C�Y�̐ݒ菈��
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// �����������
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_A) ||
		CManager::GetKeyboard()->GetKeyboardTrigger(DIK_LEFT))
	{
		// �I���A�b�v
		m_nSelect--;
		// ������������
		if (m_nSelect < PAUSE_UI_GAME)
		{
			m_nSelect = PAUSE_UI_RESTART;
		}
		// �T�C�Y�̐ݒ�
		m_aScene_Two[m_nSelect]->SetSize(size);
		// �{��
		m_fMult = 1.0f;
		// �ڕW�̈ʒu�E�T�C�Y�̐ݒ菈��
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// �G���^�[��������
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// �I����Ԃ̐ݒ�
		CManager::GetGame()->SetState(CGame::STATE_NORMAL);
		// �G���^�[��
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);

		// �Q�[���ɖ߂�
		if (m_nSelect == PAUSE_UI_GAME)
		{
			CManager::GetGame()->SetState(CGame::STATE_NORMAL);
			CPause::SetPause(false);
		}
		// �^�C�g���֖߂�
		if (m_nSelect == PAUSE_UI_TITLE)
		{
			// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
		}
		// ������x�v���C
		if (m_nSelect == PAUSE_UI_RESTART)
		{
			// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
		}
	}
	// ���Z�����Z
	if (m_fMult >= 1.1f || m_fMult <= 0.9f)
	{
		m_fAdd *= -1;
	}
	// �T�C�Y�̕ω�
	m_fMult += m_fAdd;

	// �T�C�Y�{��
	size *= m_fMult;

	// �T�C�Y�̐ݒ�
	m_aScene_Two[m_nSelect]->SetSize(size);
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CPause_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Draw();
		}
	}
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CPause_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[PAUSE_UI_MAX][72] =
	{
		{""},
		{"data/TEXTURE/PAUSE/00_pousecomment.png"},
		{"data/TEXTURE/PAUSE/01_GoGame.png"},
		{"data/TEXTURE/PAUSE/02_GoTitle.png"},
		{"data/TEXTURE/PAUSE/03_ReGame.png"},
	};
	// �ʒu�����
	D3DXVECTOR3 pos[PAUSE_UI_MAX] =
	{
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.25f, 500.0f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 500.0f,0.0f)},
	};
	// �T�C�Y�����
	D3DXVECTOR2 size[PAUSE_UI_MAX] =
	{
		{ D3DXVECTOR2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2) },
		{ D3DXVECTOR2(500.0f,500.0f) },
		{ D3DXVECTOR2(300.0f,300.0f) },
		{D3DXVECTOR2(300.0f,300.0f)},
		{D3DXVECTOR2(300.0f,300.0f)},
	};

	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		// �e�N�X�`���[����������
		if (strcmp(TexData[nCnt], "") != 0)
		{
			/* �e�N�X�`���[�̃��[�h */
			D3DXCreateTextureFromFile(pDevice, TexData[nCnt], &m_pTex[nCnt]);
			/* �ʒu�E�T�C�Y�̃��[�h */
			m_pos[nCnt] = pos[nCnt];
			m_size[nCnt] = size[nCnt];
		}
	}
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CPause_ui::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CPause_ui * CPause_ui::Create(void)
{
	// �ϐ��錾
	CPause_ui * pPause_ui;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPause_ui = new CPause_ui();
	// ����������
	pPause_ui->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pPause_ui;
}
