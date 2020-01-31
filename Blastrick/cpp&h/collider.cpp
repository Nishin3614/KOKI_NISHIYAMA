// ----------------------------------------
//
// �����蔻�菈���̐���[collider.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "collider.h"
#include "character.h"
#include "meshsphere.h"

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CCollider::CCollider()
{
	// �ϐ��̏�����
	m_mtx = NULL;				// �s��
	m_offset = D3DVECTOR3_ZERO;	// �I�t�Z�b�g�ʒu
	m_pos = D3DVECTOR3_ZERO;	// ���S���W
	m_fRadius = 0;				// ���a
	m_nStartFrame = 0;			// �X�^�[�g
	m_nEndFrame = 0;			// �G���h
	m_nCntFrame = 0;			// �t���[���J�E���g
	m_nAttack = 0;				// �U����
	m_nCharacter = 0;			// �L�����N�^�[�^�C�v
}

// ----------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------
CCollider::CCollider(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	// �ϐ��̏�����
	m_mtx = NULL;				// �s��
	m_offset = D3DVECTOR3_ZERO;	// �I�t�Z�b�g�ʒu
	m_pos = D3DVECTOR3_ZERO;	// ���S���W
	m_fRadius = 0;				// ���a
	m_nStartFrame = 0;			// �X�^�[�g
	m_nEndFrame = 0;			// �G���h
	m_nCntFrame = 0;			// �t���[���J�E���g
	m_nAttack = 0;				// �U����
	m_nCharacter = 0;			// �L�����N�^�[�^�C�v
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CCollider::~CCollider()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CCollider::Init(void)
{
	/*
	CMeshsphere *pSphere;
	pSphere = CMeshsphere::Create();
	pSphere->Set(m_pos, m_fRadius);
	*/
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CCollider::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CCollider::Update(void)
{
	switch (m_owner)
	{
	case OWNER_MYSELF:
		OwnerMyself();
		break;
	case OWNER_YOU:
		OwnerYou();
		break;
	}
}

// ----------------------------------------
// �Ǘ��Ҏ�������
// ----------------------------------------
void CCollider::OwnerMyself(void)
{
	// �t���[���O
	if (m_nCntFrame < m_nStartFrame)
	{
		return;
	}
	else if (m_nCntFrame >= m_nEndFrame)
	{
		m_nCntFrame = 0;
		m_mtx = NULL;
		m_bUse = false;
		Release();
		return;
	}
	// �ʒu
	D3DXVec3TransformCoord(
		&m_pos,
		&m_offset,
		m_mtx
	);
	// �ϐ��錾
	CCharacter * pCharacter = NULL;	// ��
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		pCharacter = (CCharacter*)CScene::GetScene(CScene::LAYER_CHARACTER, nCntLayer);
		// ���݂��Ă��邩�ǂ���
		if (pCharacter == NULL)
		{
			continue;
		}
		// �����K�p���邩�ǂ���
		else if (this->m_nCharacter == pCharacter->GetCharacter() ||		// ������
			pCharacter->GetStatus().bInvincible)							// ���G��Ԃł͂Ȃ���
		{
			continue;
		}
		// ���̓����蔻��
		if (CCalculation::Collision_Circle(
			m_pos,					// �ʒu
			m_fRadius,				// �͈�
			pCharacter->GetPos(),	// �L�����N�^�[�ʒu
			100.0f					// �͈�
		))
		{
			// �_���[�W����
			pCharacter->AplayDamage(
				m_pos,
				CCharacter::BLUST_STOP,
				m_nAttack
			);
		}
	}
	// �t���[���J�E���g
	m_nCntFrame++;
}

// ----------------------------------------
// �Ǘ��Ҏg�p���I�u�W�F�N�g�Ă��鏈��
// ----------------------------------------
void CCollider::OwnerYou(void)
{
	// �g�p��Ԃ�false�̏ꍇ������
	if (!m_bUse)
	{
		return;
	}
	// �ϐ��錾
	CCharacter * pCharacter = NULL;	// ��
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		pCharacter = (CCharacter*)CScene::GetScene(CScene::LAYER_CHARACTER, nCntLayer);
		// ���݂��Ă��邩�ǂ���
		if (pCharacter == NULL)
		{
			continue;
		}
		// �����K�p���邩�ǂ���
		else if (this->m_nCharacter == pCharacter->GetCharacter() &&		// ������
			pCharacter->GetStatus().bInvincible)							// ���G��Ԃł͂Ȃ���
		{
			continue;
		}
		// ���̓����蔻��
		if (CCalculation::Collision_Circle(
			m_pos,					// �ʒu
			m_fRadius,				// �͈�
			pCharacter->GetPos(),	// �L�����N�^�[�ʒu
			100.0f					// �͈�
		))
		{
			// �_���[�W����
			pCharacter->AplayDamage(
				m_pos,
				CCharacter::BLUST_STOP,
				m_nAttack
			);
		}
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CCollider::Draw(void)
{
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CCollider::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// �ʒu����̍쐬����
// ----------------------------------------
CCollider * CCollider::Create(
	D3DXVECTOR3	const &pos,			// �ʒu
	float		const &fRadius,		// ���a
	int			const &nAttack,		// �U����
	int			const &nCharacter,	// �L�����N�^�[
	OWNER		const &owner		// �Ǘ���
)
{
	// �ϐ��錾
	CCollider * pCollider;		// �����蔻��N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pCollider = new CCollider(ACTOR_COLLIDER,LAYER_UI);
	// �ݒ�
	pCollider->m_pos = pos;					// �ʒu
	pCollider->m_fRadius = fRadius;			// ���a
	pCollider->m_nAttack = nAttack;			// �U����
	pCollider->m_nCharacter = nCharacter;	// �L�����N�^�[
	pCollider->m_owner = owner;				// �Ǘ���
	// ����������
	pCollider->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pCollider;
}

// ----------------------------------------
// �s���񂩂�̍쐬����
// ----------------------------------------
CCollider * CCollider::Create(
	D3DXMATRIX	*mtx,				// �s��
	D3DXVECTOR3 const &offset,		// �I�t�Z�b�g�ʒu
	float		const &fRadius,		// ���a
	int			const &nStartFrame,	// �X�^�[�g
	int			const &nEndFrame,	// �G���h
	int			const &nAttack,		// �U����
	int			const &nCharacter,	// �L�����N�^�[
	OWNER		const &owner		// �Ǘ���
)
{
	// �ϐ��錾
	CCollider * pCollider;		// �����蔻��N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pCollider = new CCollider(ACTOR_COLLIDER, LAYER_UI);
	// �ݒ�
	pCollider->m_nCharacter = nCharacter;
	pCollider->m_owner = owner;
	pCollider->Set(
		mtx,
		offset,		// �I�t�Z�b�g�ʒu
		fRadius,		// ���a
		nStartFrame,	// �X�^�[�g
		nEndFrame,	// �G���h
		nAttack		// �U����
	);
	// ����������
	pCollider->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pCollider;
}

// ----------------------------------------
// �����蔻��ݒ�ݒ菈��
// ----------------------------------------
void CCollider::Set(
	D3DXMATRIX	*mtx,				// �s��
	D3DXVECTOR3 const &offset,		// �I�t�Z�b�g�ʒu
	float		const &fRadius,		// ���a
	int			const &nStartFrame,	// �X�^�[�g
	int			const &nEndFrame,	// �G���h
	int			const &nAttack		// �U����
)
{
	// �s�񂪂Ȃ��ꍇ������
	if (mtx == NULL) return;
	// �ݒ�
	m_mtx = mtx;					// �s��
	m_offset = offset;				// �I�t�Z�b�g�ʒu
	D3DXVec3TransformCoord(
		&m_pos,
		&m_offset,
		m_mtx
	);
	m_fRadius = fRadius;			// ���a
	m_nStartFrame = nStartFrame;	// �X�^�[�g
	m_nEndFrame = nEndFrame;		// �G���h
	m_nAttack = nAttack;			// �U����
}

// ----------------------------------------
// �g�p��Ԑݒ菈��
// ----------------------------------------
void CCollider::SetUse(bool const bUse)
{
	// �ϐ��錾
	CCharacter * pCharacter = NULL;	// ��
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		pCharacter = (CCharacter*)CScene::GetScene(CScene::LAYER_CHARACTER, nCntLayer);
		// ���݂��Ă��邩�ǂ���
		if (pCharacter == NULL)
		{
			continue;
		}
		// �����K�p���邩�ǂ���
		else if (this->m_nCharacter == pCharacter->GetCharacter())		// ������
		{
			continue;
		}
		// �L�����N�^�[�̓����蔻����
		pCharacter->SetInvi(bUse);
	}
	// �g�p���
	m_bUse = bUse;
}

// ----------------------------------------
// �ʒu�ݒ菈��
// ----------------------------------------
void CCollider::SetPos(D3DXVECTOR3 const & pos)
{
	m_pos = pos;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CCollider::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// �L�����N�^�[���擾����
// ----------------------------------------
int CCollider::GetCharacter(void)
{
	return m_nCharacter;
}
