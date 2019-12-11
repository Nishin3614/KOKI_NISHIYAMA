// ----------------------------------------
//
// �������̐���[box.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "box.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define BOX_TEX "data/TEXTURE/HossanAnimation.png"
#define BOX_SIZE (100.0f)
#define BOX_ANIMCOUNT (2)
#define BOX_HORIZONANIM (1.0f/BOX_ANIMCOUNT)
#define BOX_VERTICALANIM (1.0f/BOX_ANIMCOUNT)

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
LPDIRECT3DTEXTURE9 CBox::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CBox::CBox() : CScene_TWO::CScene_TWO(ACTOR_BOX,LAYER_OBJ)
{
	// �����̃A�j���[�V�����p�^�[��
	m_nHorizonAnim = 0;
	// �����̃A�j���[�V�����p�^�[��
	m_nVirticalAnim = 1;
	// �A�j���[�V�����J�E���g
	m_nCntAnim = 0;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CBox::~CBox()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CBox::Init(void)
{	
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(BOX_SIZE, BOX_SIZE));
	// ������
	CScene_TWO::Init();
	// �ʒu�ݒ�
	CScene_TWO::SetPosition(D3DXVECTOR3(500.0f, 600.0f, 0.0f));
	// ���_�e�N�X�`���[�ݒ�
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM + BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM + BOX_VERTICALANIM));
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CBox::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CBox::Update(void)
{
	CScene_TWO::Update();
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CBox::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// �����̃A�j���[�V�����p�^�[���ݒ菈��
// ----------------------------------------
void CBox::SetVirticalAnim(int nVirtical)
{
	m_nVirticalAnim = nVirtical;
	// ���_�e�N�X�`���[�ݒ�
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM + BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM + BOX_VERTICALANIM));
}

// ----------------------------------------
// �����̃A�j���[�V�����p�^�[���ݒ菈��
// ----------------------------------------
void CBox::SetHorizonAnim(int nHorizon)
{
	m_nHorizonAnim = nHorizon;
	// ���_�e�N�X�`���[�ݒ�
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM + BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM + BOX_VERTICALANIM));
}

// ----------------------------------------
// �v���C���[�ԍ��擾����
// ----------------------------------------
CManager::PLAYER CBox::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CBox::GetPos(void)
{
	return CScene_TWO::GetPosition();
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CBox::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, BOX_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CBox::UnLoad(void)
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
CBox * CBox::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos)
{
	// �ϐ��錾
	CBox * pBox;		// ��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pBox = new CBox();
	// ����������
	pBox->Init();
	// �v���C���[�ԍ�
	pBox->m_PlayerId = playerId;
	// �ʒu�ݒ�
	pBox->SetPosition(pos);
	// ���������I�u�W�F�N�g��Ԃ�
	return pBox;
}
// ----------------------------------------
// �A�j���[�V��������
// ----------------------------------------
void CBox::Animation(void)
{
	// �A�j���[�V�����J�E���g�A�b�v
	m_nCntAnim++;
	// 20�t���[�����A�j���[�V��������
	if (m_nCntAnim % 20 == 0)
	{
		// �A�j���[�V�����ω�
		m_nHorizonAnim = (m_nHorizonAnim + 1) % BOX_ANIMCOUNT;
	}
}
