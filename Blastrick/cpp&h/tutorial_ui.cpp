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
#include "tutorial_ui.h"
#include "fade.h"
#include "input.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define TUTORIAL_UI_ANIM (1)
#define TUTORIAL_UI_SPLIT (1.0f / TUTORIAL_UI_ANIM)

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
LPDIRECT3DTEXTURE9 CTutorial_ui::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CTutorial_ui::CTutorial_ui() : CScene::CScene(ACTOR_TUTORIAL_UI, LAYER_UI)
{
	// ������
	m_aScene_Two = NULL;
	m_nSelect = 0;
	m_fAnim = 0;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CTutorial_ui::~CTutorial_ui()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CTutorial_ui::Init(void)
{
	// 2D�V�[�����쐬
	m_aScene_Two = CScene_TWO::Create(
		CScene_TWO::OFFSET_TYPE_CENTER,							// �Z���^�[
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),	// �ʒu
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT)				// �T�C�Y
	);
	// �e�N�X�`���[����ݒ�
	m_aScene_Two->BindTexture(m_pTex);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CTutorial_ui::Uninit(void)
{
	if (m_aScene_Two != NULL)
	{
		m_aScene_Two = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CTutorial_ui::Update(void)
{
	if (m_aScene_Two != NULL)
	{
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			Select();
		}
		if (CManager::GetJoy() != NULL)
		{
			if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_START) ||
				CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B) ||
				CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_A))
			{
				Select();
			}
		}
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CTutorial_ui::Draw(void)
{
	if (m_aScene_Two != NULL)
	{
		// ��ނɂ���ď�����ς���
		m_aScene_Two->Draw();
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CTutorial_ui::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CTutorial_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�����
	char TexData[72] =
	{
		"data/TEXTURE/UI/Tutorial.png",
	};

		// �e�N�X�`���[����������
	if (strcmp(TexData, "") != 0)
	{
		/* �e�N�X�`���[�̃��[�h */
		D3DXCreateTextureFromFile(pDevice, TexData, &m_pTex);
	}
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CTutorial_ui::UnLoad(void)
{
	// �e�N�X�`���̊J��
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CTutorial_ui * CTutorial_ui::Create(void)
{
	// �ϐ��錾
	CTutorial_ui * pTutorial_ui;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pTutorial_ui = new CTutorial_ui();
	// ����������
	pTutorial_ui->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pTutorial_ui;
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CTutorial_ui::Select(void)
{
	// �I���A�b�v
	m_nSelect++;
	// �G���^�[��
	CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);

	// �I����ʂ�
	if (m_nSelect >= TUTORIAL_UI_ANIM)
	{
		// ��ʑJ�ڂ̏�Ԃ��J�ڂ��Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
	}
	// ���̃y�[�W��
	else
	{
		m_aScene_Two->SetTex(
			D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f),
			D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
	}
}
