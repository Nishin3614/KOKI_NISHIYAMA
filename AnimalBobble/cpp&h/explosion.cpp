// ----------------------------------------
//
// ���������̐���[explosion.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "explosion.h"

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
LPDIRECT3DTEXTURE9 CExplosion::m_pTex[CBullet::TYPE_MAX] = {};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CExplosion::CExplosion() : CScene_TWO::CScene_TWO(ACTOR_EXPLOSION,LAYER_PARTICLE)
{
	/* �ϐ��̏����� */
	// �ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	// �^�C�v
	m_type = CBullet::TYPE_BEE;
	// �̗�
	m_nLife = 100;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CExplosion::~CExplosion()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CExplosion::Init(void)
{
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(50.0f, 50.0f));
	// ������
	CScene_TWO::Init();	
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �F�ݒ�
	CScene_TWO::SetCol(m_col);
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex[m_type]);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CExplosion::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CExplosion::Update(void)
{
	// �̗͌���
	m_nLife--;
	// �����x�ύX
	m_col.a -= 0.01f;
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �X�V
	CScene_TWO::Update();
	// �F�̐ݒ�
	CScene_TWO::SetCol(m_col);
	// �̗͂�0�ȉ�
	if (m_nLife <= 0)
	{
		CScene::Release();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CExplosion::Draw(void)
{
	// �`�揈��
	CScene_TWO::Draw();
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CExplosion::Load(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	char cTex[CBullet::TYPE_MAX][128] =			// �e�N�X�`���[�ݒ�
	{
		"data/TEXTURE/Effect/Explosion2.png",
	};

	// �e�N�X�`���[�ݒ�
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTex[nCnt]);
	}

	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CExplosion::UnLoad(void)
{
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̊J��
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
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, CBullet::TYPE type)
{
	// �ϐ��錾
	CExplosion * pExplosion;		// ����2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pExplosion = new CExplosion();
	// ���݂̈ʒu
	pExplosion->m_pos = pos;
	// �^�C�v
	pExplosion->m_type = type;
	// �F
	switch (pExplosion->m_type)
	{
		// �n�`
	case CBullet::TYPE_BEE:
		pExplosion->m_col = D3DXCOLOR(1.0f, 0.7f, 0.2f, 1.0f);
		break;
		// �L
	case CBullet::TYPE_CAT:
		pExplosion->m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
		// ��
	case CBullet::TYPE_FISH:
		pExplosion->m_col = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
		// ��
	case CBullet::TYPE_HAWK:
		pExplosion->m_col = D3DXCOLOR(0.4f, 0.2f, 0.2f, 1.0f);
		break;
		// ��
	case CBullet::TYPE_SNAKE:
		pExplosion->m_col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f);
		break;
	default:
		break;
	}
	// ����������
	pExplosion->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pExplosion;
}