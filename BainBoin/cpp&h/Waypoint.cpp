// ----------------------------------------
//
// ���Ԓn�_�����̐���[waypoint.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "waypoint.h"
#include "player.h"
#include "meshdome.h"
#include "fade.h"
#include "debugproc.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define WAYPOINT_ROTMOVE (0.01f)				// �p�x�̈ړ���
#define WAYPOINT_SIZE (100.0f)					// �T�C�Y
#define WAYPOINT_TEX "data/TEXTURE/Line.png"	// �e�N�X�`���[
#define WAYPOINT_ANGLE (D3DX_PI / 1)			// �p�Ȃ̊p�x

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
LPDIRECT3DTEXTURE9 CWaypoint::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CWaypoint::CWaypoint() : CScene(ACTOR_WAYPOINT, LAYER_3DOBJECT)
{
	/* �ϐ��̏����� */
	// ��]��
	m_pos = D3DXVECTOR3(0.0f, 2600.0f, 0.0f);
	m_bWaypoint = false;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CWaypoint::~CWaypoint()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CWaypoint::Init(void)
{
	// ���b�V���h�[���̐���
	m_dome = CMeshdome::Create(
		m_pos,
		D3DXVECTOR3(50.0f, 500.0f, 50.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
		20, 1,
		CMeshdome::TYPE_WAVE
	);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CWaypoint::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CWaypoint::Update(void)
{
	CPlayer *pPlayer = NULL;

	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}

	// �Q�[���J��
	// �����L���O��
	if (CCalculation::Collision_Circle(
		m_pos,	// ���݂̃I�u�W�F�N�g�̈ʒu
		50.0f,				// ���݂̃I�u�W�F�N�g�̔��a
		pPlayer->GetPos(),	// �ړI�̃I�u�W�F�N�g�̈ʒu
		1))					// �ړI�̃I�u�W�F�N�g�̔��a
	{
		m_bWaypoint = true;
		m_dome->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f));
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CWaypoint::Draw(void)
{
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CWaypoint::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, WAYPOINT_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CWaypoint::UnLoad(void)
{
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CWaypoint * CWaypoint::Create(
	D3DXVECTOR3 pos
)
{
	// �ϐ��錾
	CWaypoint * pWaypoint;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pWaypoint = new CWaypoint();
	// �ʒu���
	pWaypoint->m_pos = pos;
	// ����������
	pWaypoint->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pWaypoint;
}

// ----------------------------------------
// ���Ԓn�_�擾����
// ----------------------------------------
bool CWaypoint::GetWaypoint(void)
{
	return m_bWaypoint;
}

// ----------------------------------------
// ���Ԓn�_�̈ʒu�擾����
// ----------------------------------------
D3DXVECTOR3 CWaypoint::GetPos(void)
{
	return m_pos;
}
