// ----------------------------------------
//
// �V�[�������̐���[frame.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "frame.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define FRAME_TEXTRUE "data/TEXTURE/UI/Frame.png"

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
LPDIRECT3DTEXTURE9 CFrame::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CFrame::CFrame() : CScene_TWO::CScene_TWO(ACTOR_FRAME,LAYER_OBJ)
{
	m_PlayerId = CManager::PLAYER_1;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CFrame::~CFrame()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CFrame::Init(void)
{	
	CScene_TWO::SetSize(D3DXVECTOR2(500.0f, 600.0f));
	CScene_TWO::Init();
	CScene_TWO::SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CScene_TWO::BindTexture(m_pTex);
	CScene_TWO::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CFrame::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CFrame::Update(void)
{
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CFrame::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// �v���C���[�ԍ��擾����
// ----------------------------------------
CManager::PLAYER CFrame::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CFrame::GetPos(void)
{
	return CScene_TWO::GetPosition();
}

// ----------------------------------------
// �T�C�Y���擾����
// ----------------------------------------
D3DXVECTOR2 CFrame::GetSize(void)
{
	return CScene_TWO::GetSize();
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CFrame::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, FRAME_TEXTRUE, &m_pTex);

	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CFrame::UnLoad(void)
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
CFrame * CFrame::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos)
{
	// �ϐ��錾
	CFrame * pFrame;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pFrame = new CFrame();
	// ����������
	pFrame->Init();
	// �v���C���[�ԍ����
	pFrame->m_PlayerId = playerId;
	// �ʒu���
	pFrame->SetPosition(pos);
	// ���������I�u�W�F�N�g��Ԃ�
	return pFrame;
}