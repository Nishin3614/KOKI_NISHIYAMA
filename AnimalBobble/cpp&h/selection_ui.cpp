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
#include "selection_ui.h"
#include "mark.h"
#include "fade.h"

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
LPDIRECT3DTEXTURE9 CSelection_ui::m_pTex[SELECTION_UI_MAX] = {};
D3DXVECTOR3 CSelection_ui::m_pos[SELECTION_UI_MAX] = {};
D3DXVECTOR2 CSelection_ui::m_size[SELECTION_UI_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CSelection_ui::CSelection_ui() : CScene::CScene(ACTOR_SELECTION_UI, LAYER_UI)
{
	// ������
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_fMult = 1.0f;
	m_fAdd = 0.001f;
	m_nCntCol = 0;
	m_mark = NULL;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CSelection_ui::~CSelection_ui()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CSelection_ui::Init(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
		// �w�i
		if (nCnt == SELECTION_UI_BG)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));
		}
		// �I����ʂ̈�l�̏ꍇ
		if (nCnt == SELECTION_UI_SINGLE)
		{
			// �ڈ�
			m_mark = CMark::Create(
				m_aScene_Two[nCnt]->GetPosition(),
				m_aScene_Two[nCnt]->GetSize(),
				CScene::ACTOR_MARK);
			m_nSelect = SELECTION_UI_SINGLE;
		}
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CSelection_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
void CSelection_ui::Update(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// ��ނɂ���ď�����ς���
			m_aScene_Two[nCnt]->Update();
		}
	}
	D3DXVECTOR2 size = m_size[m_nSelect];	// �T�C�Y�̎擾

	// ������������
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_DOWN))
	{
		// �I���A�b�v
		m_nSelect += 1;

		// �����������
		if (m_nSelect > SELECTION_UI_DISCRIPTION)
		{
			m_nSelect = SELECTION_UI_SINGLE;
		}
		// �T�C�Y�̐ݒ�
		m_aScene_Two[m_nSelect]->SetSize(size);
		m_fMult = 1.0f;
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// �����������
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_UP))
	{
		// �I���A�b�v
		m_nSelect -= 1;
		// ������������
		if (m_nSelect < SELECTION_UI_SINGLE)
		{
			m_nSelect = SELECTION_UI_DISCRIPTION;
		}
		// �T�C�Y�̐ݒ�
		m_aScene_Two[m_nSelect]->SetSize(size);
		m_fMult = 1.0f;
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// �G���^�[��������
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{

			// �G���^�[��
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
			// ��l�ŃQ�[��
			if (m_nSelect == SELECTION_UI_SINGLE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
			// �`���[�g���A��
			if (m_nSelect == SELECTION_UI_DISCRIPTION)
			{
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
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
	size *= m_fMult;

	// �T�C�Y�̐ݒ�
	m_aScene_Two[m_nSelect]->SetSize(size);

}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CSelection_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
HRESULT CSelection_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[SELECTION_UI_MAX][72] =
	{
		"png",
		"data/TEXTURE/UI/Single.png",
		"data/TEXTURE/UI/Description.png" 
	};
	// �ʒu�����
	D3DXVECTOR3 pos[SELECTION_UI_MAX] =
	{
		D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2,200.0f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2,500.0f,0.0f)
	};
	// �T�C�Y�����
	D3DXVECTOR2 size[SELECTION_UI_MAX] =
	{
		D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT),
		D3DXVECTOR2(700.0f,300.0f),
		D3DXVECTOR2(700.0f,300.0f)
	};

	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
void CSelection_ui::UnLoad(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
CSelection_ui * CSelection_ui::Create(void)
{
	// �ϐ��錾
	CSelection_ui * pSelection_ui;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pSelection_ui = new CSelection_ui();
	// ����������
	pSelection_ui->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pSelection_ui;
}
