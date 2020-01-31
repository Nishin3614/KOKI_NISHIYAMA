// ----------------------------------------
//
// �W���������̐���[forceline.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "forceline.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define FORCELINE_TEX "data/TEXTURE/forceline2.jpg"

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
LPDIRECT3DTEXTURE9 CForceline::m_pTex = NULL;
bool CForceline::m_bUse = false;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CForceline::CForceline() : CScene_TWO::CScene_TWO(ACTOR_FORCELINE,LAYER_UI)
{
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CForceline::~CForceline()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CForceline::Init(void)
{	
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	// ������
	CScene_TWO::Init();
	// �ʒu�ݒ�
	CScene_TWO::SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	CScene_TWO::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
	// ���_�e�N�X�`���[�ݒ�
	CScene_TWO::SetTex(
		D3DXVECTOR2(0.0f,0.0f),
		D3DXVECTOR2(1.0f,1.0f));
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CForceline::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CForceline::Update(void)
{
	if (m_bUse)
	{
		CScene_TWO::Update();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CForceline::Draw(void)
{
	if (m_bUse)
	{
		//CScene_TWO::Draw();
	}
}

// ----------------------------------------
// �g�p��ԏ���
// ----------------------------------------
void CForceline::Use(bool bUse)
{
	m_bUse = bUse;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CForceline::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, FORCELINE_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CForceline::UnLoad(void)
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
CForceline * CForceline::Create(void)
{
	// �ϐ��錾
	CForceline * pForceline;		// �W����2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pForceline = new CForceline();
	// ����������
	pForceline->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pForceline;
}