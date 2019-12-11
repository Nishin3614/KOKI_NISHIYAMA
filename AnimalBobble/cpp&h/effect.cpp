// ----------------------------------------
//
// �G�t�F�N�g�����̐���[effect.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "effect.h"

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
LPDIRECT3DTEXTURE9 CEffect::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CEffect::CEffect() : CScene_TWO::CScene_TWO(ACTOR_EFFECT, LAYER_PARTICLE)
{
	// �ϐ��̏�����
	m_fSize = 0.0f;
	m_fRot = 0;
	m_fGoalrot = 0;
	m_fDiffrot = 0;
	m_Speed = 0;
	m_fSize = 0;
	m_fGoalsize = 0;
	m_fDiffsize = 0;
	m_nLife = 0;
	m_nSplit = 0;
	m_nCntSplit = 0;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CEffect::~CEffect()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CEffect::Init(void)
{
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(m_fSize, m_fSize));
	// ������
	CScene_TWO::Init();
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �J���[�ݒ�
	CScene_TWO::SetCol(m_col);
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CEffect::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CEffect::Update(void)
{
	// ���C�t�̌���
	m_nLife--;
	// �����J�E���g�A�b�v
	m_nCntSplit++;
	// �J���[�̕ύX
	m_col -= m_Diffcol;
	// �T�C�Y�̕ύX
	m_fSize -= m_fDiffsize;
	// �p�x�̕ύX
	if (m_nCntSplit <= m_nSplit)
	{
		m_fRot -= m_fDiffrot;
	}
	m_pos.x += sinf(m_fRot) * m_Speed;
	m_pos.y += cosf(m_fRot) * m_Speed;

	// �p�x�ύX
	CScene_TWO::SetRot(m_fRot);
	// �T�C�Y�ύX
	CScene_TWO::SetSize(D3DXVECTOR2(m_fSize, m_fSize));
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �J���[�ݒ�
	CScene_TWO::SetCol(m_col);
	// �X�V
	CScene_TWO::Update();

	// �I��
	if (m_nLife <= 0)
	{
		Release();
		return;
	}

}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CEffect::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene_TWO::Draw();

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bobble.png", &m_pTex);

	return S_OK;
	//"data/TEXTURE/bobble.png"
	//"data/TEXTURE/Effect/effect.jpg"
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CEffect::UnLoad(void)
{
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}


// ----------------------------------------
// ��������
// ----------------------------------------
void CEffect::Fountain(
	float fGoalrot,
	int nSplit)
{
	m_fGoalrot = fGoalrot;
	m_nSplit = nSplit;
	m_fDiffrot = (m_fGoalrot - m_fRot) / m_nSplit;
}

// ----------------------------------------
// �~����
// ----------------------------------------
void CEffect::Circle(
	int nSplit
)
{
	m_nSplit = nSplit;
	m_fDiffrot = D3DX_PI / m_nSplit;
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CEffect * CEffect::Create(
	D3DXVECTOR3 pos,		// �ʒu
	D3DXCOLOR col,			// �J���[
	D3DXCOLOR goalcol,		// �ڕW�̃J���[
	float fRot,				// �����p�x
	float fSpeed,			// ����
	float fSize,			// �T�C�Y
	float fGoalsize,		// �ڕW�T�C�Y
	int nLife				// ���C�t(������)
)
{
	// �ϐ��錾
	CEffect * pEffect;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pEffect = new CEffect();
	/* �ϐ���� */
	pEffect->m_pos = pos;
	pEffect->m_col = col;
	pEffect->m_Goalcol = goalcol;
	pEffect->m_fRot = fRot;
	pEffect->m_Speed = fSpeed;
	pEffect->m_fSize = fSize;
	pEffect->m_fGoalsize = fGoalsize;
	pEffect->m_nLife = nLife;
	pEffect->m_Diffcol = (pEffect->m_Goalcol - pEffect->m_col);
	pEffect->m_Diffcol /= (float)pEffect->m_nLife;
	pEffect->m_fDiffsize = (pEffect->m_fGoalsize - pEffect->m_fSize) / pEffect->m_nLife;

	// ����������
	pEffect->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pEffect;
}
