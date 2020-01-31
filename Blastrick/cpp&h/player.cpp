// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f������ [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "floor.h"
#include "input.h"
#include "camera.h"
#include "collider.h"

#include "trajectory.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "meshsphere.h"
#include "bgdome.h"
#include "collision.h"


#include "2Dgauge.h"
#include "stategauge.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_KEYMOVE (5)
#define PLAYER_G (0.5f)			// �d��
#define PLAYER_RESISTANCE (0.5f)// ��R��
#define PLAYER_MOTIONFILE "data/LOAD/PLAYER/Tricker.txt"	// ���[�V�����̃t�@�C����

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer() : CCharacter::CCharacter(ACTOR_PLAYER)
{
	m_posold = D3DVECTOR3_ZERO;		// �O�̈ʒu
	m_nConbo = 0;					// �R���{
	m_bTrick = false;				// �鉜�`�����ǂ���
	p2DHPGauge = NULL;				// HP�Q�[�W
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	CCharacter::SetCharacter(CHARACTER_PLAYER);
	CCharacter::Init();
	// �J�����̏�����
	CManager::GetRenderer()->GetCamera()->InfoInit(
		CCharacter::GetPos() +
		D3DXVECTOR3(0.0f, 100.0f, 0.0f),	// �J�����ʒu
		CCharacter::GetRot());				// �J������]
	
	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(50.0f, 660.0f, 0.0f);

	// HP�Q�[�W�̐���
	p2DHPGauge = C2DGauge::Create(
		pos + D3DXVECTOR3(28.5f, 30.0f, 0.0f),
		D3DXVECTOR2(500.0f, 25.0f),
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
	);
	// �鉜�`�Q�[�W�̕ω��萔��ݒ�
	p2DHPGauge->SetConstance((float)CCharacter::GetStatus().nMaxLife);

	// MP�Q�[�W�̐���
	p2DTrickGauge = C2DGauge::Create(
		pos + D3DXVECTOR3(40.0f, 10.0f, 0.0f),
		D3DXVECTOR2(180.0f, 15.0f),
		D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f)
	);
	// �鉜�`�Q�[�W�̕ω��萔��ݒ�
	p2DTrickGauge->SetConstance((float)CCharacter::GetStatus().nMaxMP);
	// �X�e�[�g�Q�[�W�̐���
	CStateGauge::Create(D3DXVECTOR3(50.0f, 660.0f, 0.0f),
		D3DXVECTOR2(100.0f, 100.0f)
	);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();
	// HP�Q�[�W���k����
	if (p2DHPGauge != NULL)
	{
		p2DHPGauge = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// �j���[�g�������[�V������
	CCharacter::SetMotion(MOTIONTYPE_NEUTRAL);
	// �J�����̒����_�ݒ�
	CManager::GetRenderer()->GetCamera()->SetPosR(
		CCharacter::GetPos(),
		CCharacter::GetRot()
	);
	// �U��
	Attack();
	// ���[�V�������
	MotionState();

	if (CCharacter::GetAttack() == false)
	{
		// �ړ�
		Move();
	}
	else
	{
		// ��~
		Stop();
	}
	if (m_bDamage)
	{
		// HP�Q�[�W�̕ω��萔��ݒ�
		p2DHPGauge->ChangeGauge((float)CCharacter::GetStatus().nLife);
		m_bDamage = false;
	}
	 // �X�V
	CCharacter::Update();
	// �J�����ݒ�
	CManager::GetRenderer()->GetCamera()->SetType((CCamera::TYPE)CCharacter::GetCameraType());
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ړ�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Move(void)
{
	// �ϐ��錾
	CFloor * pFloor = NULL;			// ��
	D3DXVECTOR3 move, rot;			// �ړ��ʁA��]
	bool bMove = false;				// �ړ����
	float fRot;						// ��]
	// ���擾
	rot = CCharacter::GetRotDest();								// �ړI��]��
	move = CCharacter::GetMove();								// �ړ���
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// �J������]

	// �ړ� //
	/* �W���C�p�b�h */
	// �p�b�h�p //
	int nValueH, nValueV;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fMove;			// �ړ����x
	float fAngle;			// �X�e�B�b�N�p�x�̌v�Z�p�ϐ�
	fAngle = 0.0f;			// �p�x

	if (CManager::GetJoy() != NULL)
	{
		// �Q�[���p�b�h�̃X�e�B�b�N�����擾
		CManager::GetJoy()->GetStickLeft(0, &nValueH, &nValueV);

		/* �v���C���[�ړ� */
		// �Q�[���p�b�h�ړ�
		if (nValueH != 0 || nValueV != 0)
		{
			// �p�x�̌v�Z
			fAngle = atan2f((float)nValueH, (float)nValueV);

			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}
			else if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}
			// ���x�̌v�Z
			if (abs(nValueH) > abs(nValueV))
			{
				fMove = (abs(nValueH) * PLAYER_KEYMOVE) / 1024.0f;
			}
			else
			{
				fMove = (abs(nValueV) * PLAYER_KEYMOVE) / 1024.0f;
			}
			rot.y = fAngle + fRot;

			// �X�e�B�b�N�̊p�x�ɂ���ăv���C���[�ړ�
			move.x -= sinf(fAngle + fRot) * (fMove);
			move.z -= cosf(fAngle + fRot) * (fMove);
			// �ړ����on
			bMove = true;
		}
	}
	/* �L�[�{�[�h */
	// ��
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_A))
	{
		// �ړ����on
		bMove = true;
		// ��
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_W))
		{
			rot.y = -D3DX_PI * 0.25f + fRot;

			move.x += sinf(D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
		}
		// ��O
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_S))
		{
			rot.y = -D3DX_PI * 0.75f + fRot;

			move.x += sinf(D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
		}
		// ��
		else
		{
			rot.y = -D3DX_PI * 0.5f + fRot;
			move.x += sinf(D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
		}
	}
	// �E
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_D))
	{
		// �ړ����on
		bMove = true;

		// ��
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_W))
		{
			rot.y = D3DX_PI * 0.25f + fRot;

			move.x += sinf(-D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(-D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
		}
		// ��O
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_S))
		{
			rot.y = D3DX_PI * 0.75f + fRot;

			move.x += sinf(-D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(-D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
		}
		// �E
		else
		{
			rot.y = D3DX_PI * 0.5f + fRot;

			move.x += sinf(-D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(-D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
		}
	}
	// ���ɍs��
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_W))
	{
		// �ړ����on
		bMove = true;
		rot.y = D3DX_PI * 0.0f + fRot;
		move.x += sinf(-D3DX_PI * 1.0f + fRot) * PLAYER_KEYMOVE;
		move.z += cosf(-D3DX_PI * 1.0f + fRot) * PLAYER_KEYMOVE;
	}
	// ��O�ɍs��
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_S))
	{
		// �ړ����on
		bMove = true;
		rot.y = D3DX_PI * 1.0f + fRot;
		move.x += sinf(D3DX_PI * 0.0f + fRot) * PLAYER_KEYMOVE;
		move.z += cosf(D3DX_PI * 0.0f + fRot) * PLAYER_KEYMOVE;
	}
	// �ړ���ԂȂ�
	if (bMove == true)
	{
		CCharacter::SetMotion(MOTIONTYPE_MOVE);
	}
	// ��R��
	move.x *= PLAYER_RESISTANCE;
	move.z *= PLAYER_RESISTANCE;

	CCharacter::SetMove(move);
	CCharacter::SetRotDest(rot);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��~����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Stop(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�������̏�ԏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MotionState(void)
{
	// �ϐ��錾
	D3DXVECTOR3 move;

	// ���[�V�������̊e����
	switch (CCharacter::GetMotion())
	{
	case MOTIONTYPE_NEUTRAL:
		m_bTrick = false;

		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_SLASHER:
		// �p�[�e�B�N������
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_SPARKS,
			CCharacter::GetPos()
		);
		break;
	case MOTIONTYPE_TRICK1_FIREUPSIDE:
		// �L�[�J�E���g8
		if (CCharacter::GetKeyInfoCnt() == 8)
		{
			move = CCharacter::GetMove();
			move.y += 1.03f;
			CCharacter::SetMove(move);
		}
		break;
	case MOTIONTYPE_TRICK2_FLASHSTRIKE:
		// �L�[�J�E���g34,35,36

		break;
	default:
		break;
	}
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �U������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Motion_Attack(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �U������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Attack(void)
{
	// �鉜�`���͊֐��𔲂���
	if (m_bTrick)
	{
		return;
	}

	// �ʏ�U��
	At_Normal();
	// �鉜�`
	Trick();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʏ�U������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_Normal(void)
{
	// �U������������֐��𔲂���
	if (CCharacter::GetAttack())
	{
		return;
	}
	// �C���^�[�o���R���{�J�E���g�A�b�v
	m_nInvalConbo++;
	// �R���{������
	if (m_nInvalConbo >= 30)
	{
		m_nConbo = 0;
	}
	// �R���{�U��
	// �L�[�{�[�h�p //
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// �R���{�U��
		ConboAttack();
		m_nInvalConbo = 0;
		m_nConbo++;
	}
	// �W���C�p�b�h�p //
	if (CManager::GetJoy() == NULL)
	{
		return;
	}
	if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B))
	{
		// �R���{�U��
		ConboAttack();
		m_nInvalConbo = 0;
		m_nConbo++;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���{�U������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::ConboAttack(void)
{
	// �R���{�U��
	switch (m_nConbo)
	{
		// ����R���{
	case 0:
		At_DiagonalCut();
		break;
		// 1�R���{
	case 1:
		At_ReverseSide();
		break;
		// 2�R���{
	case 2:
		At_ReverseSide();
		break;
		// �R���{�̏I��
	default:
		m_nConbo = 0;
		m_nInvalConbo = 0;
		// �鉜�`
		Trick1_FireUpside();
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���{1_�ǉ����͂Ȃ�(�΂ߐ؂�)����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_DiagonalCut(void)
{
	// �ϐ��錾
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	// �ړ��ݒ�
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK1_FIREUPSIDE);

	// �O�Ղ̐ݒ�
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// �ʒu
		CTrajectory::TYPE_NORMAL,		// �^�C�v
		nFrame							// �t���[����
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// ���[�V�����ݒ�
	CCharacter::SetMotion(MOTIONTYPE_DIAGONALCUT);
	// �U����Ԑݒ�
	CCharacter::SetAttack(true);
	// ��Ԑݒ�
	//CCharacter::SetState(STATE_;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���{1_�ǉ����́�(�˂�)����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_Thrust(void)
{
	// �ړ��ݒ�
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���{2_�ǉ�����(�t�؂�)����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_ReverseSide(void)
{
	// �ϐ��錾
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	// �ړ��ݒ�
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK1_FIREUPSIDE);

	// �O�Ղ̐ݒ�
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// �ʒu
		CTrajectory::TYPE_NORMAL,		// �^�C�v
		nFrame							// �t���[����
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// ���[�V�����ݒ�
	CCharacter::SetMotion(MOTIONTYPE_REVERSESIDE);
	// �U����Ԑݒ�
	CCharacter::SetAttack(true);
	// ��Ԑݒ�
	//CCharacter::SetState(STATE_;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �鉜�`����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick(void)
{
	// �鉜�`1
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_1))
	{
		Trick1_FireUpside();
		m_bTrick = true;
	}
	// �鉜�`2
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_2))
	{
		Trick2_FlashStrike();
		m_bTrick = true;
	}
	// �鉜�`3
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_3))
	{
		Trick3_PainLightning();
		m_bTrick = true;
	}

	// �W���C�p�b�h�̏��� //
	if (CManager::GetJoy() == NULL)
	{
		return;
	}
	// �鉜�`1
	if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_LEFT_1))
	{
		Trick1_FireUpside();
		m_bTrick = true;
	}
	// �鉜�`2
	else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_RIGHT_1))
	{
		Trick2_FlashStrike();
		m_bTrick = true;
	}
	// �鉜�`3
	else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_RIGHT_2))
	{
		Trick3_PainLightning();
		m_bTrick = true;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �鉜�`1����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick1_FireUpside(void)
{
	// �ϐ��錾
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f,100.0f,0.0f);

	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK1_FIREUPSIDE);
	// ���[�V�����ݒ�
	CCharacter::SetMotion(MOTIONTYPE_TRICK1_FIREUPSIDE);
	// �ړ��ݒ�
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// �U����Ԑݒ�
	CCharacter::SetAttack(true);
	// �O�Ղ̐ݒ�
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// �ʒu
		CTrajectory::TYPE_TRICK1,		// �^�C�v
		nFrame							// �t���[����
	);
	// �ʒu
	D3DXVec3TransformCoord(
		&pos,
		&offset,
		CCharacter::GetMatrix(15)
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_WEAVELINE,
		pos
	);
	// ���b�V���X�t�B�A
	CMeshsphere::Set(
		pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		1000.0f,
		nFrame
	);
	/*
	// �w�i�̃h�[��
	CBgdome::Set(
	pos,
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	nFrame
	);
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �鉜�`2����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick2_FlashStrike(void)
{
	// �ϐ��錾
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK2_FLASHSTRIKE);

	// ���[�V�����ݒ�
	CCharacter::SetMotion(MOTIONTYPE_TRICK2_FLASHSTRIKE);
	// �ړ��ݒ�
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// �U����Ԑݒ�
	CCharacter::SetAttack(true);
	// �O�Ղ̐ݒ�
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// �ʒu
		CTrajectory::TYPE_TRICK2,		// �^�C�v
		nFrame							// �t���[����
	);
	// �ʒu
	D3DXVec3TransformCoord(
		&pos,
		&offset,
		CCharacter::GetMatrix(15)
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_WEAVELINE,
		pos
	);
	// ���b�V���X�t�B�A
	CMeshsphere::Set(
		pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		1000.0f,
		nFrame
	);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �鉜�`3����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick3_PainLightning(void)
{
	// �ϐ��錾
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK3_PAINLIGHTNING);

	// ���[�V�����ݒ� 
	CCharacter::SetMotion(MOTIONTYPE_TRICK3_PAINLIGHTNING);
	// �ړ��ݒ�
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// �U����Ԑݒ�
	CCharacter::SetAttack(true);
	// �O�Ղ̐ݒ�
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// �ʒu
		CTrajectory::TYPE_TRICK1,		// �^�C�v
		nFrame							// �t���[����
	);
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// �ʒu
	D3DXVec3TransformCoord(
		&pos,
		&offset,
		CCharacter::GetMatrix(15)
	);

	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_CROSSLINE,
		pos
	);
	// ���b�V���X�t�B�A
	CMeshsphere::Set(
		pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		1000.0f,
		nFrame
	);

	/*
	// ���o�ݒ�
	CPerfomance::Set(CCharacter::GetMatrix(15),
		CPerfomance::TYPE_0,
		CCharacter::GetPos(),
		CCharacter::GetRot(),
		CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK3_PAINLIGHTNING)
	);
	/*
	// �U������
	CCollider::Create(
		&CCharacter::GetPos(),							// �ʒu
		D3DXVECTOR3(									// ���S�ʒu����̋���
			100.0f * -sinf(CCharacter::GetRot().y),
			100.0f,
			100.0f * -cosf(CCharacter::GetRot().y)),
		CCharacter::GetStatus().nAttack,				// �U����
		0												// �������G��
	);
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Debug(void)
{
	CDebugproc::Print("�L�[�J�E���g:%d\n",CCharacter::GetKeyInfoCnt());
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_N))
	{
		D3DXVECTOR3 pos;
		pos = p2DTrickGauge->GetPos();
		pos.x += 0.1f;
		p2DTrickGauge->SetPos(pos);
	}
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_M))
	{
		D3DXVECTOR3 pos;
		pos = p2DTrickGauge->GetPos();
		pos.x -= 0.1f;
		p2DTrickGauge->SetPos(pos);
	}
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create()
{
	// �ϐ��錾
	CPlayer * pPlayer;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPlayer = new CPlayer();
	// ����������
	pPlayer->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pPlayer;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���\�[�X���ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	CCharacter::Load(
		CCharacter::CHARACTER_PLAYER,
		MOTIONTYPE_MAX, 
		PLAYER_MOTIONFILE);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾃��\�[�X����j������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::UnLoad(void)
{
	CCharacter::UnLoad(
		CCharacter::CHARACTER_PLAYER,
		MOTIONTYPE_MAX);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu�擾����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return 	CCharacter::GetPos();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��]�擾����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return CCharacter::GetRot();
}