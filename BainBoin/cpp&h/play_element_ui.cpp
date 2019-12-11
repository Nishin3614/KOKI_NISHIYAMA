// ----------------------------------------
//
// �v���C�pUI�����̐���[play_element_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "play_element_ui.h"
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
LPDIRECT3DTEXTURE9 CPlay_element_ui::m_pTex[PLAY_ELEMENT_UI_MAX] = {};
D3DXVECTOR3 CPlay_element_ui::m_pos[PLAY_ELEMENT_UI_MAX] = {};
D3DXVECTOR2 CPlay_element_ui::m_size[PLAY_ELEMENT_UI_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CPlay_element_ui::CPlay_element_ui() : CScene::CScene(ACTOR_PLAYER_ELEMENT_UI, LAYER_UI)
{
	// ������
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	playerID = CManager::PLAYER_1;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CPlay_element_ui::~CPlay_element_ui()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CPlay_element_ui::Init(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CPlay_element_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt]->Uninit();
		delete m_aScene_Two[nCnt];
		m_aScene_Two[nCnt] = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CPlay_element_ui::Update(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		// ��ނɂ���ď�����ς���
		m_aScene_Two[nCnt]->Update();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CPlay_element_ui::Draw(void)
{
	// �X�R�A��UI
	m_aScene_Two[CPlay_element_ui::PLAY_ELEMENT_UI_SCORE]->Draw();
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CPlay_element_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[PLAY_ELEMENT_UI_MAX][72] =
	{
		{"data/TEXTURE/UI/Frame_Score2.png"},	// �X�R�A
		{"data/TEXTURE/UI/Time_Comment.png"},	// �^�C��
	};
	// �ʒu�����
	D3DXVECTOR3 pos[PLAY_ELEMENT_UI_MAX] =
	{
		{D3DXVECTOR3(100.0f,100.0f,0.0f)},
		{D3DXVECTOR3(20.0f,20.0f,0.0f)},
	};
	// �T�C�Y�����
	D3DXVECTOR2 size[PLAY_ELEMENT_UI_MAX] =
	{
		{D3DXVECTOR2(500.0f,70.0f)},
		{D3DXVECTOR2(50.0f,50.0f) },
	};

	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
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
void CPlay_element_ui::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
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
CPlay_element_ui * CPlay_element_ui::Create(CManager::PLAYER playerId)
{
	// �ϐ��錾
	CPlay_element_ui * pPlay_element_ui;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPlay_element_ui = new CPlay_element_ui();
	// 
	pPlay_element_ui->playerID = playerId;
	// ����������
	pPlay_element_ui->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pPlay_element_ui;
}
