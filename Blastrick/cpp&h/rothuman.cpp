// ----------------------------------------
//
// �_�l�ԏ��� [rothuman.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------
#include "rothuman.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define ROTHUMAN_MOTIONFILE "data/LOAD/CHARACTER/giraffe_motion.txt"	// ���[�V�����̃t�@�C����

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �I�[�o�[���[�h�R���X�g���N�^(���g�p)
// ----------------------------------------
CRothuman::CRothuman() : CEnemy::CEnemy(ACTOR_ROTHUMAN)
{
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CRothuman::~CRothuman()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CRothuman::Init(void)
{
	CCharacter::SetCharacter(CHARACTER_ROTHUMAN);
	CEnemy::Init();
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CRothuman::Uninit(void)
{
	CEnemy::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CRothuman::Update(void)
{
	// �G�l�~�[�X�V
	CEnemy::Update();
	// �U�����͓���Ȃ�
	if (!CCharacter::GetAttack() ||
		!(CCharacter::GetState() == CCharacter::STATE_DAMAGE ||
		CCharacter::GetState() == CCharacter::STATE_DIE))
	{
		// �s������
		AiAction();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CRothuman::Draw(void)
{
	CEnemy::Draw();
}

// ----------------------------------------
// ��������
// ----------------------------------------
CRothuman * CRothuman::Create(
	D3DXVECTOR3 pos			// �ʒu
)
{
	// �ϐ��錾
	CRothuman * pRothuman;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pRothuman = new CRothuman();
	// ����������
	pRothuman->Init();
	// �ʒu�ݒ�
	pRothuman->SetPos(pos);
	// ���������I�u�W�F�N�g��Ԃ�
	return pRothuman;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CRothuman::LoadCreate(void)
{
	// �ϐ��錾
	D3DXVECTOR3 pos;
	for (int nCntCreate = 0; nCntCreate < 10; nCntCreate++)
	{
		// �ϐ��錾
		CRothuman * pRothuman;
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pRothuman = new CRothuman();
		// �ʒu�ݒ�
		pos.x = (float)(rand() % 900) - (float)(rand() % 900);
		pos.z = (float)(rand() % 900) - (float)(rand() % 900);

		pRothuman->SetPos(pos);
		// ����������
		pRothuman->Init();
		pRothuman = NULL;
	}
	return E_NOTIMPL;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CRothuman::Load(void)
{
	CCharacter::Load(
		CCharacter::CHARACTER_ROTHUMAN,
		MOTIONTYPE_MAX, 
		ROTHUMAN_MOTIONFILE);
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j������
// ----------------------------------------
void CRothuman::UnLoad(void)
{
	CCharacter::UnLoad(
		CCharacter::CHARACTER_ROTHUMAN,
		MOTIONTYPE_MAX);
}

// ----------------------------------------
// �s����������
// ----------------------------------------
void CRothuman::AiAction(void)
{
	switch (m_Ai)
	{
	case AI_NORMALACTION:
		NormalAction();
		break;
	case AI_ATTACKACTION:
		AttackAction();
		break;
	default:
		break;
	}
}

// ----------------------------------------
// �U���s������
// ----------------------------------------
void CRothuman::AttackAction(void)
{
	if (!m_bAttackCollision)
	{
		return;
	}
	// �ϐ��錾
	int nParcent;
	D3DXVECTOR3 move;
	// ���擾
	move = CCharacter::GetMove();								// �ړ���

	nParcent = rand() % 100;

	if (nParcent > 20)
	{
		CCharacter::SetMotion(MOTIONTYPE_ATTACK);
	}
	else if (nParcent > 100)
	{
		CCharacter::SetMotion(MOTIONTYPE_ATTACK);
	}
	CCharacter::SetAttack(true);
	move = D3DVECTOR3_ZERO;
	CCharacter::SetMove(move);
}

// ----------------------------------------
// ���i�̍s������
// ----------------------------------------
void CRothuman::NormalAction(void)
{
	CCharacter::SetMotion(MOTIONTYPE_NEUTRAL);
}

// ----------------------------------------
// �ړ�����
// ----------------------------------------
void CRothuman::move(void)
{
	// �ϐ��錾
	D3DXVECTOR3 pos;	// �ʒu
}

// ----------------------------------------
// ��]����
// ----------------------------------------
void CRothuman::rot(void)
{
	// �ϐ��錾
	D3DXVECTOR3 rot;	// ��]
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CRothuman::Debug(void)
{
}
#endif // _DEBUG
