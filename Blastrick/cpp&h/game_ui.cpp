// ----------------------------------------
//
// �|�[�Y�pui�����̐���[selection_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "game_ui.h"
#include "fade.h"
#include "mark.h"
#include "game.h"
#include "number.h"
#include "score.h"

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
LPDIRECT3DTEXTURE9 CGame_ui::m_pTex[GAME_UI_MAX] = {};
D3DXVECTOR3 CGame_ui::m_pos[GAME_UI_MAX] = {};
D3DXVECTOR2 CGame_ui::m_size[GAME_UI_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CGame_ui::CGame_ui() : CScene::CScene(ACTRO_GAME_UI, LAYER_UI)
{
	// ������
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_pNumber = NULL;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CGame_ui::~CGame_ui()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CGame_ui::Init(void)
{
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
		// �X�^�[�g
		if (nCnt == GAME_UI_START)
		{
			m_pNumber = CNumber::Create();
			m_pNumber->SetTex(CNumber::TEX_SCORE);
			m_pNumber->SetPos(
				m_aScene_Two[nCnt]->GetPosition() + D3DXVECTOR3(0.0f,200.0f,0.0f),
				200.0f);
			m_pNumber->SetNum(1);
			m_pNumber->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f,1.0f));
		}
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CGame_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt]->Uninit();
		delete m_aScene_Two[nCnt];
		m_aScene_Two[nCnt] = NULL;
	}
	// �ԍ�
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CGame_ui::Update(void)
{
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		// ��ނɂ���ď�����ς���
		m_aScene_Two[nCnt]->Update();
	}
	// �ԍ�
	if (m_pNumber != NULL)
	{
		m_pNumber->Update();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CGame_ui::Draw(void)
{
	CGame::STATE state = CManager::GetGame()->GetState();
	// �I���X�^�[�g��
	if (state == CGame::STATE_START)
	{
		m_aScene_Two[CGame_ui::GAME_UI_START]->Draw();
		// �ԍ�
		if (m_pNumber != NULL)
		{
			m_pNumber->Draw();
		}
	}
	// �I���N���A��
	else if (state == CGame::STATE_CLEAR)
	{
		m_aScene_Two[CGame_ui::GAME_UI_STAGECLEAR]->Draw();
	}
	// �I���I�[�o�[��oi
	else if (state == CGame::STATE_GAMEOVER)
	{
		m_aScene_Two[CGame_ui::GAME_UI_GAMEOVER]->Draw();
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CGame_ui::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// �ԍ��擾����
// ----------------------------------------
CNumber * CGame_ui::GetNumber(void)
{
	return m_pNumber;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CGame_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[GAME_UI_MAX][72] =
	{
		{"data/TEXTURE/UI/stage.png"},
		{"data/TEXTURE/UI/stageclear.png"},
		{"data/TEXTURE/UI/gameover.png"},
	};
	// �ʒu�����
	D3DXVECTOR3 pos[GAME_UI_MAX] =
	{
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
	};
	// �T�C�Y�����
	D3DXVECTOR2 size[GAME_UI_MAX] =
	{
		{D3DXVECTOR2(SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f)},
		{D3DXVECTOR2(SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f)},
		{D3DXVECTOR2(SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f)},
	};

	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
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
void CGame_ui::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
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
CGame_ui * CGame_ui::Create(void)
{
	// �ϐ��錾
	CGame_ui * pGame_ui;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pGame_ui = new CGame_ui();
	// ����������
	pGame_ui->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pGame_ui;
}
