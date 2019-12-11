// ----------------------------------------
//
// �V�䏈���̐���[ceiling.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "ceiling.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define CEILING_TEXTRUE "data/TEXTURE/Ceiling.png"

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
LPDIRECT3DTEXTURE9 CCeiling::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CCeiling::CCeiling() : CScene_TWO::CScene_TWO(ACTOR_CEILING,LAYER_OBJ)
{
	m_PlayerId = CManager::PLAYER_1;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CCeiling::~CCeiling()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CCeiling::Init(void)
{	
	CScene_TWO::SetSize(D3DXVECTOR2(0.0f, 0.0f));
	CScene_TWO::Init();
	CScene_TWO::SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CScene_TWO::BindTexture(m_pTex);
	CScene_TWO::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CCeiling::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CCeiling::Update(void)
{
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CCeiling::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// �v���C���[�ԍ��擾����
// ----------------------------------------
CManager::PLAYER CCeiling::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CCeiling::GetPos(void)
{
	return CScene_TWO::GetPosition();
}

// ----------------------------------------
// �T�C�Y���擾����
// ----------------------------------------
D3DXVECTOR2 CCeiling::GetSize(void)
{
	return CScene_TWO::GetSize();
}

// ----------------------------------------
// �ʒu�E�T�C�Y�ݒ菈��
// ----------------------------------------
void CCeiling::SetPosSize(
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR2 size	// �T�C�Y
)
{
	CScene_TWO::SetSize(size);
	CScene_TWO::SetPosition(pos);
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CCeiling::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, CEILING_TEXTRUE, &m_pTex);

	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CCeiling::UnLoad(void)
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
CCeiling * CCeiling::Create(
	CManager::PLAYER playerId,	// �v���C���[�ԍ�
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR2 size	// �T�C�Y
)
{
	// �ϐ��錾
	CCeiling * pCeiling;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pCeiling = new CCeiling();
	// ����������
	pCeiling->Init();
	// �v���C���[�ԍ����
	pCeiling->m_PlayerId = playerId;
	// �T�C�Y
	pCeiling->SetSize(size);
	// �ʒu���
	pCeiling->SetPosition(pos);
	// ���������I�u�W�F�N�g��Ԃ�
	return pCeiling;
}