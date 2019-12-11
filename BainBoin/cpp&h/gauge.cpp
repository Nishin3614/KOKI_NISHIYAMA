// ----------------------------------------
//
// �|�[�Y�pui�����̐���[tutorial_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "gauge.h"
#include "fade.h"
#include "input.h"
#include "player.h"
#include "game.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define GAUGE_X				(50 / 2)		//�Q�[�W�̉���
#define GAUGE_Y				(120)			//�Q�[�W�̏c��

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
LPDIRECT3DTEXTURE9	CGauge::m_pTex[GAUGE_MAX] = {};
D3DXVECTOR3			CGauge::m_pos[GAUGE_MAX] = {};
D3DXVECTOR2			CGauge::m_size[GAUGE_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CGauge::CGauge() : CScene::CScene(ACTRO_GAME_UI, LAYER_UI)
{
	// ������
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}

	m_nCharge = 0; //�Q�[�W�̃`���[�W�x
	m_nMaxGauge = 0;  //�Q�[�W�̃`���[�W�ʂ̏����l
	m_fTex = 0.0f;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CGauge::~CGauge()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CGauge::Init(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[GAUGE_GAUGE]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CGauge::Uninit(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
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
void CGauge::Update(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// ��ނɂ���ď�����ς���
			Various(GAUGE_GAUGE);
			// ��ނɂ���ď�����ς���
			//m_aScene_Two[nCnt]->Update();
		}
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CGauge::Draw(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// ��ނɂ���ď�����ς���
			m_aScene_Two[nCnt]->Draw();
		}
	}

}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CGauge::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[GAUGE_MAX][72] =
	{
		"data/TEXTURE/UI/Gauge.png",
		"data/TEXTURE/UI/frame.png",
	};
	// �ʒu�����
	D3DXVECTOR3 pos[GAUGE_MAX] =
	{
		D3DXVECTOR3(1200,620,0),
		D3DXVECTOR3(1200,500,0),
	};
	// �T�C�Y�����
	D3DXVECTOR2 size[GAUGE_MAX] =
	{
		D3DXVECTOR2(GAUGE_X / 2,120),
		D3DXVECTOR2(GAUGE_X * 2 + 10,250),
	};
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
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
void CGauge::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
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
CGauge * CGauge::Create(void)
{
	// �ϐ��錾
	CGauge * pGauge;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pGauge = new CGauge();
	// ����������
	pGauge->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pGauge;
}

// ----------------------------------------
// �Q�[�W�̒l�̎擾
// ----------------------------------------
int CGauge::GetGaugeValue(void)
{
	return m_nCharge;
}

// ----------------------------------------
// �l�̍Đݒ�(������)
// ----------------------------------------
void CGauge::ResetJump(void)
{
	m_nCharge = 0;
	m_nMaxGauge = 0;
	D3DXVECTOR2 size(GAUGE_X, (float)m_nMaxGauge);
	m_aScene_Two[GAUGE_GAUGE]->SetGaugeSize(size);
	m_fTex = 0.0f;
}
// ----------------------------------------
// ��ޕʂ̏���
// ----------------------------------------
void CGauge::Various(int nCnt)
{
	// ���擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (nCnt == GAUGE_GAUGE)
	{
		for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
		{
			if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
		}
		if (pPlayer->GetbJump() == false)
		{
			if (CManager::GetKeyboard()->GetKeyboardPress(DIK_SPACE))
			{//space�������Ă���Ƃ��ɃQ�[�W�̒l�����Z
				float fHeight = 50 * ((float)m_nCharge) / 25;

				if (m_nCharge < GAGE_VALUE_MAX)
				{
					m_nCharge++;
				}

				if (m_nMaxGauge < fHeight)
				{
					if (m_nMaxGauge >= 0)
					{
						m_nMaxGauge += 2;

						D3DXVECTOR2 size(GAUGE_X, (float)m_nMaxGauge);
						m_aScene_Two[GAUGE_GAUGE]->SetGaugeSize(size);

						m_fTex -= 0.0083f;

						m_aScene_Two[GAUGE_GAUGE]->SetTex(D3DXVECTOR2(1.0f, m_fTex), D3DXVECTOR2(0.0f, 0.0f));
					}
				}
			}

			if (CManager::GetJoy()->GetPress(0, CJoypad::KEY_A))
			{
				float fHeight = 50 * ((float)m_nCharge) / 25;

				if (m_nCharge < GAGE_VALUE_MAX)
				{
					m_nCharge++;
				}

				if (m_nMaxGauge < fHeight)
				{
					if (m_nMaxGauge >= 0)
					{
						m_nMaxGauge += 2;

						D3DXVECTOR2 size(GAUGE_X, (float)m_nMaxGauge);
						m_aScene_Two[GAUGE_GAUGE]->SetGaugeSize(size);

						m_fTex -= 0.0083f;

						m_aScene_Two[GAUGE_GAUGE]->SetTex(D3DXVECTOR2(1.0f, m_fTex), D3DXVECTOR2(0.0f, 0.0f));
					}
				}
			}
		}
	}
}
