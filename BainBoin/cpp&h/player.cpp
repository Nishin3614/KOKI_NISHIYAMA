// ----------------------------------------
//
// ���f������ [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------
#include "player.h"
#include "scene_X.h"
#include "floor.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "3Dparticle.h"
#include "gauge.h"
#include "game.h"
#include "item.h"
#include "score.h"
#include "Waypoint.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define PLAYER_KEYMOVE			(2)						// �L�[�ړ�
#define PLAYER_COLLISIONSIZE	(50.0f)					// �����蔻��̑傫��

#define PLAYER_FEXTEND_TIME		(4)						// 1�i�K�L�т鎞��
#define PLAYER_SEXTEND_TIME		(8)						// 2�i�K�L�т鎞��
#define PLAYER_TEXTEND_TIME		(12)					// 3�i�K�L�т鎞��

#define PLAYER_SHRINK_TIME		(30)					// �k�ގ���

#define PLAYER_SHRINK_DEFAU		(1.0f)					// �ʏ�̐L��
#define PLAYER_FIRST_EXTEND		(5.0f)					// 1�i�K�̐L��
#define PLAYER_SECOND_EXTEND	(10.0f)					// 2�i�K�̐L��
#define PLAYER_THIRD_EXTEND		(15.0f)					// 3�i�K�̐L��

#define PLAYER_FSHRINK_TIME		(PLAYER_FEXTEND_TIME + PLAYER_SHRINK_TIME)	// 1�i�K�k�ގ���
#define PLAYER_SSHRINK_TIME		(PLAYER_SEXTEND_TIME + PLAYER_SHRINK_TIME)	// 2�i�K�k�ގ���
#define PLAYER_TSHRINK_TIME		(PLAYER_TEXTEND_TIME + PLAYER_SHRINK_TIME)	// 3�i�K�k�ގ���

#define PLAYER_FJUMP_MAGNI		(1.0f)					// 1�i�K�̃W�����v�͂̔{��
#define PLAYER_SJUMP_MAGNI		(2.0f)					// 2�i�K�̃W�����v�͂̔{��
#define PLAYER_TJUMP_MAGNI		(3.0f)					// 3�i�K�̃W�����v�͂̔{��
#define PLAYER_KEYMOVE (2)
#define PLAYER_G (0.8f)					// �d��
#define PLAYER_RESISTANCE (0.9f)		// ��R��
#define PLAYER_COLLISIONSIZE (50.0f)	// �����蔻��̑傫��
#define PLAYER_REST_X (1300.0f)			// x�͈̔�
#define PLAYER_REST_Z (200.0f)			// z�͈̔�
#define	PLAYER_JUMP_UP			(0.1f)	// �W�����v�̃X�e�[�^�X�㏸��
#define	PLAYER_ROT_UP			(0.1f)	// ��]�̃X�e�[�^�X�㏸��
#define	PLAYER_JUMP_MOVE			(0.2f)	// ��]�̃X�e�[�^�X�㏸��
#define PLAYER_KEYMOVE			(2)

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------
CModel_info		*CPlayer::m_pModel_info = NULL;		// ���f�����
MODEL_OFFSET	*CPlayer::m_pModel_offset = NULL;	// ���f���̏����z�u
MOTION			*CPlayer::m_pMotion = NULL;			// ���[�V�������̕ۑ�

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CPlayer::CPlayer() : CScene::CScene(ACTOR_PLAYER, LAYER_3DOBJECT)
{
	m_pos = D3DXVECTOR3(0.0f,200.0f,0.0f);		// �ʒu
	m_posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݉�]��
	m_rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ������������
	m_rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�̍���
	m_size = D3DXVECTOR3(							// ���f���̃T�C�Y
		PLAYER_COLLISIONSIZE,
		PLAYER_COLLISIONSIZE,
		PLAYER_COLLISIONSIZE);
	m_bJump = false;								// �W�����v

	m_pCollision = NULL;

	m_JumpRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CPlayer::Init(void)
{
	// ���f���̐ݒ�
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// ���f������
		m_pModel[nCntModel] = CModel::Create();
		// ���f�����n��
		m_pModel[nCntModel]->BindModel(
			m_pModel_info[nCntModel].GetModel_info(),
			m_pModel_offset[nCntModel]
		);
		// ���ׂĂ̐e�ȊO
		if (nCntModel != 0)
		{
			// �e���ݒ�
			m_pModel[nCntModel]->SetParent(m_pModel[m_pModel_info[nCntModel].GetParent()]);
		}
	}
	// �X�e�[�^�X�ǂݍ���
	LoadStatus();
	// �����蔻�萶��
	m_pCollision = CCollision::Create();
	m_pCollision->SetPos(&m_pos);
	m_pCollision->SetSize(m_size);
	m_pCollision->SetMove(&m_move);
	m_pCollision->SetType(CCollision::OBJTYPE_PLAYER);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CPlayer::Uninit(void)
{
	// ���f��
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// �k���`�F�b�N
		if (m_pModel[nCntModel] != NULL)
		{
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CPlayer::Update(void)
{
	// �ϐ��錾
	CScore * pScore = NULL;	// ��
							// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntLayer++)
	{
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_UI, nCntLayer);	// ��
	}

	// �ړ�
	Move();
	// �t���[���J�E���g�A�b�v
	m_nFrame++;
	// ���[�V�����^�C�v���ω�
	// ������
	if (m_nMotiontype != m_nMotiontypeOld)
	{
		m_nFrame = 0;		// �t���[���L�[���̃J�E���g
		m_keyinfoCnt = 0;	// �L�[���̃J�E���g
	}
	// ���[�V�����̕ۑ�
	m_nMotiontypeOld = m_nMotiontype;
	// �t���[�����������ɂȂ�����
	if (m_nFrame == m_pMotion[m_nMotiontype].KeyInfo[m_keyinfoCnt].nFrame)
	{
		// ������
		m_nFrame = 0;	// �t���[��
		// �J�E���g�A�b�v
		m_keyinfoCnt++;
		// �L�[��񂪒�������
		if (m_keyinfoCnt >= m_pMotion[m_nMotiontype].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (m_pMotion[m_nMotiontype].nLoop == 0)
			{
				// ������
				m_keyinfoCnt = 0;				// �L�[���
				m_nMotiontype = 0;	// ���[�V�����^�C�v
			}

			// ���[�v���鎞
			else
			{
				// ������
				m_keyinfoCnt = 0;				// �L�[���
			}
		}
	}
	// ���f��
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// �k���`�F�b�N
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->SetMotion(m_pMotion[m_nMotiontype].KeyInfo[m_keyinfoCnt].Key[nCntModel], m_pMotion[m_nMotiontype].KeyInfo[m_keyinfoCnt].nFrame);
			m_pModel[nCntModel]->Update();
		}
	}
	CDebugproc::Print("�X�e�[�^�X���\n");
	CDebugproc::Print("�W�����v��		: %.2f\n", m_Status.fJumpforce);
	CDebugproc::Print("�W�����v�͏㏸��	: %.2f\n", m_Status.fRateJumpforce);
	CDebugproc::Print("��]��			: %.2f\n", m_Status.fRotforce);
	CDebugproc::Print("��]�͏㏸��		: %.2f\n", m_Status.fRateRotforce);

	m_pCollision->SetPos(&m_pos);

	MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
	CDebugproc::Print("�X�P�[��Y�� -> [%.4f]\n", pModelInfo->scal.y);
}

// ----------------------------------------
// �ړ�����
// ----------------------------------------
void CPlayer::Move(void)
{
	// �ϐ��錾
	float fRot;
	CFloor * pFloor = NULL;	// ��

	int nValueH = 0;//�X�e�B�b�N�̉�
	int nValueV = 0;//�X�e�B�b�N�̏c

	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pFloor == NULL) pFloor = (CFloor*)CScene::GetActor(CScene::ACTOR_FLOOR, CScene::LAYER_3DOBJECT, nCntLayer);	// ��
	}
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// ��]���擾
	// �W�����v
	if (CManager::GetKeyboard()->GetKeyboardDetach(DIK_SPACE) &&
		m_bJump == false)
	{
		CGauge *pGauge = CGame::GetGauge();

		m_bJump = true;
		m_move.x = 0;	// �ړ���
		m_move.y = 0;	// �ړ���
		m_move.z = 0;	// �ړ���
		m_nJumpCharge = pGauge->GetGaugeValue();
		//m_move.y += 20.0f;
		//m_move.y += CPlayer::GetResultJumpforce();
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSION, m_pos);
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSIONWAVE, m_pos);

		m_move.x -= sinf(m_rot.z) * GetJumpforce();
		m_move.y += cosf(m_rot.z) * GetJumpforce();

		pGauge->ResetJump();
	}

	if (CManager::GetJoy()->GetRelease(0, CJoypad::KEY_A) &&
		m_bJump == false)
	{
		CGauge *pGauge = CGame::GetGauge();

		m_bJump = true;
		m_move.x = 0;	// �ړ���
		m_move.y = 0;	// �ړ���
		m_move.z = 0;	// �ړ���
		m_nJumpCharge = pGauge->GetGaugeValue();
		//m_move.y += 20.0f;
		//m_move.y += CPlayer::GetResultJumpforce();
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSION, m_pos);
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSIONWAVE, m_pos);

		m_move.x -= sinf(m_rot.z) * GetJumpforce() / m_Status.fRateJumpforce;
		m_move.y += cosf(m_rot.z) * GetJumpforce();

		pGauge->ResetJump();
	}

	if (m_bJump == false)
	{//�W�����v���Ă��Ȃ���
		//�L�[�{�[�h�ł̏���
		// ��]

		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_J))
		{//����]
			m_rot.z -= 0.05f;
		}
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{//�E��]
			m_rot.z += 0.05f;
		}

		//�W���C�p�b�h�ł̏���
		CManager::GetJoy()->GetStickLeft(0, &nValueH, &nValueV);

		if (nValueH != 0 || nValueV != 0)
		{
			if (nValueH <= -1)
			{
				m_rot.z -= 0.05f;
			}

			else if (nValueH >= 1)
			{
				m_rot.z += 0.05f;
			}
		}

		// ��]���
		if (m_rot.z > D3DX_PI / 3)
		{
			m_rot.z = D3DX_PI / 3;
		}
		if (m_rot.z < -D3DX_PI / 3)
		{
			m_rot.z = -D3DX_PI / 3;
		}
	}
	else
	{//�W�����v���Ă��鎞
		//�L�[�{�[�h�̏���
		// ��]
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_J))
		{//����]
			m_rot.z -= m_Status.fRotforce * m_Status.fRateRotforce;
			m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
			m_move.x += PLAYER_JUMP_MOVE;
		}
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{//�E��]
			m_rot.z += m_Status.fRotforce * m_Status.fRateRotforce;
			m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
			m_move.x -= PLAYER_JUMP_MOVE;
		}

		//�W���C�p�b�h�ł̏���
		CManager::GetJoy()->GetStickLeft(0, &nValueH, &nValueV);

		if (nValueH != 0 || nValueV != 0)
		{
			if (nValueH <= -1)
			{
				//m_move.x = 0;
				m_rot.z -= m_Status.fRotforce * m_Status.fRateRotforce;
				m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
				m_move.x += PLAYER_JUMP_MOVE;
			}

			else if (nValueH >= 1)
			{
				//m_move.x = 0;
				m_rot.z += m_Status.fRotforce * m_Status.fRateRotforce;
				m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
				m_move.x -= PLAYER_JUMP_MOVE;
			}
		}
		// ���x����
		m_rot.z = CCalculation::Rot_One_Limit(m_rot.z);
	}

	// ���x����
	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);


	if (m_bJump == false)
	{
		// ��R��
		m_move.x *= PLAYER_RESISTANCE;
		//m_move.y *= PLAYER_RESISTANCE;
		m_move.z *= PLAYER_RESISTANCE;
	}
	// �d��
	m_move.y -= PLAYER_G;	// �ړ���

	m_pos += m_move;

	CScore * pScore = NULL;	// ��
							// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntLayer++)
	{
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_UI, nCntLayer);	// ��
	}

	// ���̍�������
	if (pFloor != NULL)
	{
		if (m_pos.y < pFloor->GetHeight(m_pos))
		{
			// ���n�����u��
			if (m_bJump == true)
			{
				// �X�R�A���Z
				pScore->AddScore((int)(m_fCntRot / D3DX_PI / 2) * 100 * (int)(m_fCntRot / D3DX_PI / 2));
			}
			m_bJump = false;
			m_move.x = 0;
			m_move.y = 0;
			m_move.z = 0;
			
			m_pos.y = pFloor->GetHeight(m_pos);
			MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
			pModelInfo->scal.y = 0;
			m_fCntRot = 0;
		}
	}
	// �L�k����
	ExpanAndContraProc();
	// �����蔻��ɏ������
	if (m_pCollision->Collision_Come() == true)
	{
		if (m_bJump == true)
		{
			// �X�R�A���Z
			pScore->AddScore((int)(m_fCntRot / D3DX_PI / 2) * 100 * (int)(m_fCntRot / D3DX_PI / 2));
		}

		m_bJump = false;
		m_move.x = 0;
		m_move.y = 0;
		m_move.z = 0;
		MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
		pModelInfo->scal.y = 0;
		m_fCntRot = 0;
	}

	// �������
	Restriction();
}

// ----------------------------------------
// �W�����v�͂̌v�Z����
// ----------------------------------------
float CPlayer::GetResultJumpforce(void)
{
	return m_Status.fJumpforce + (m_Status.fRateJumpforce * m_nNumJumpItem);
}

// ----------------------------------------
// ��]�͂̌v�Z����
// ----------------------------------------
float CPlayer::GetResultRotforce(void)
{
	return m_Status.fRotforce + (m_Status.fRateRotforce * m_nNumRotItem);
}

// ----------------------------------------
// �L�k����
// ----------------------------------------
void CPlayer::ExpanAndContraProc(void)
{

	// �W�����v���Ă��鎞�̎���
	if (m_bJump == false && m_JumpTime != 0)
	{
		m_JumpTime = 0;
		return;
	}
	if (m_bJump == false)
	{
		return;
	}

	// �W�����v���Ԃ��C���N�������g
	m_JumpTime++;

	// �k���`�F�b�N
	if (m_pModel[0]->GetModelInfo() == NULL)
	{
		return;
	}
	MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
	// �L�т鎞
	if (GetExtendTime() >= m_JumpTime)
	{
		float fValue = (GetExpanAndContraMagni() - PLAYER_SHRINK_DEFAU) / (float)GetExtendTime();
		pModelInfo->scal.y += fValue;
	}
	// �k�ގ�
	else if (GetShrinkTime()+ GetExtendTime() >= m_JumpTime)
	{
		float fValue = (GetExpanAndContraMagni() - PLAYER_SHRINK_DEFAU) / (float)GetShrinkTime();
		pModelInfo->scal.y -= fValue;
	}
}

// ----------------------------------------
// �L�k�̔{���̎擾
// ----------------------------------------
float CPlayer::GetExpanAndContraMagni(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FIRST_EXTEND;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SECOND_EXTEND;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_THIRD_EXTEND;
	}
	return 0.0f;
}

// ----------------------------------------
// �L�т鎞�Ԃ̎擾
// ----------------------------------------
int CPlayer::GetExtendTime(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FEXTEND_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SEXTEND_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_TEXTEND_TIME;
	}
	return 0;
}

// ----------------------------------------
// �k�ގ��Ԃ̎擾
// ----------------------------------------
int CPlayer::GetShrinkTime(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FSHRINK_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SSHRINK_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_TSHRINK_TIME;
	}
	return 0;
}

//----------------------------------------
// �Q�[�W�ʂ̔{���̎擾
// ----------------------------------------
float CPlayer::GetJumpMagni(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FJUMP_MAGNI;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SJUMP_MAGNI;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_TJUMP_MAGNI;
	}
	return 0;
}

//----------------------------------------
// �W�����v�͂̎擾
// ----------------------------------------
float CPlayer::GetJumpforce(void)
{
	return m_Status.fJumpforce * GetJumpMagni()*m_Status.fRateJumpforce;
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CPlayer::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,	// �����̓��ꕨ
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ���f��
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// �k���`�F�b�N
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Draw(m_mtxWorld);
		}
	}
}

// ----------------------------------------
// �ʒu�擾����
// ----------------------------------------
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// ��]�擾����
// ----------------------------------------
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

// ----------------------------------------
// ��������
// ----------------------------------------
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

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
void CPlayer::Load(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();
	// ���f�����̐���
	m_pModel_info = new CModel_info[PLAYERTYPE_MAX];
	// ���f���̃I�t�Z�b�g�̐���
	m_pModel_offset = new MODEL_OFFSET[PLAYERTYPE_MAX];
	// ���[�V�����̐���
	m_pMotion = new MOTION[MOTIONTYPE_MAX];
	// �e�N�X�`���[�����
	char ModelData[PLAYERTYPE_MAX][72] =
	{
		"data/MODEL/bane/01_bane.x",			// [��]01
		"data/MODEL/bane/00_hed.x",			// [��]00
	};
	// ���f���̃I�t�Z�b�g�ݒ�
	CModel_info::ModelOffset(
		m_pModel_offset,					// ���f���̃I�t�Z�b�g
		"data/MOTION/motion_banekun.txt"	// �t�@�C����
	);
	// ���[�V�����̐ݒ�
	CMotion::Load(
		m_pMotion,							// ���[�V����
		m_pModel_offset,					// �I�t�Z�b�g
		PLAYERTYPE_MAX,						// �L�[
		"data/MOTION/motion_banekun.txt"	// �t�@�C����
	);
	// ���f���̕ۑ�
	for (int nCntModelLoad = 0; nCntModelLoad < PLAYERTYPE_MAX; nCntModelLoad++)
	{
		// ���f������ǂݍ���
		m_pModel_info[nCntModelLoad].Load(
			pDevice,
			ModelData[nCntModelLoad]);
	}
}

// ----------------------------------------
// �ǂݍ��񂾏���j������
// ----------------------------------------
void CPlayer::UnLoad(void)
{
	// ���f���̕`����j��
	for (int nCntModelLoad = 0; nCntModelLoad < PLAYERTYPE_MAX; nCntModelLoad++)
	{
		// ���f�����̊J��
		m_pModel_info[nCntModelLoad].Unload();
	}
	// ���[�V�����̔j��
	CMotion::UnLoad(
		m_pMotion,					// ���[�V����
		MOTIONTYPE_MAX
	);
	// ���f�����̔j��
	delete[] m_pModel_info;
	// ���f���̃I�t�Z�b�g�̔j��
	delete[] m_pModel_offset;
}

// ----------------------------------------
// �X�e�[�^�X�̓ǂݍ���
// ----------------------------------------
void CPlayer::LoadStatus(void)
{
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���̃|�C���^
	char cRead[128];	// �ǂݍ��ݗp
	char cComp[128];	// ��r�p
	char cEmpty[128];	// �v��Ȃ����̗p
	int nCntError = 0;	// �G���[�\���J�E���g

	// �J�������ǂ���
	if ((pFile = fopen("DATA/TEXT/PlayerStatus.txt", "r")) != NULL)
	{

		while (strcmp(cComp, "SCRIPT") != 0)
		{
			// 1�s�ǂݍ���
			fgets(cRead, sizeof(cRead), pFile);
			// �ǂݍ���Ǖ�������
			sscanf(cRead, "%s", &cComp);

			// �G���[�J�E���g���C���N�������g
			nCntError++;
			if (nCntError > 1048576)
			{// �G���[
				nCntError = 0;
				MessageBox(NULL, "[SCRIP]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
			}
		}

		if (strcmp(cComp, "SCRIPT") == 0)
		{
			// END_SCRIPT�܂Ń��[�v
			while (strcmp(cComp, "END_SCRIPT") != 0)
			{
				// 1�s�ǂݍ���
				fgets(cRead, sizeof(cRead), pFile);
				// �ǂݍ���Ǖ�������
				sscanf(cRead, "%s", &cComp);

				// �G���[�J�E���g���C���N�������g
				nCntError++;
				if (nCntError > 1048576)
				{// �G���[
					nCntError = 0;
					MessageBox(NULL, "[END_SCRIPT]��������܂���\n�I�����Ă�������", "�x���I", MB_ICONWARNING);
				}
				if (strcmp(cComp, "SETSTATUS") == 0)
				{
					do
					{// ���[�v�J�n
					 // 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > 1048576)
						{// �G���[
							nCntError = 0;
						}
						if (strcmp(cComp, "JUMPFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fJumpforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "ROTFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fRotforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "RATE_JUMPFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fRateJumpforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "RATE_ROTFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fRateRotforce);
							cComp[0] = '\0';
						}
					} while ((strcmp(cComp, "END_SETSTATUS") != 0) && (strcmp(cComp, "}") != 0));

				}
			}
		}
	}
	else
	{
		MessageBox(NULL, "�v���C���[���t�@�C�����J���܂���ł���", "�x���I", MB_ICONWARNING);
	}
	fclose(pFile);
}

// ----------------------------------------
// �X�e�[�^�X�̕ۑ�
// ----------------------------------------
void CPlayer::SaveStatus(void)
{
	FILE *pFile;		// �t�@�C���̃|�C���^

	if ((pFile = fopen("DATA/TEXT/PlayerStatus.txt", "w")) != NULL)
	{
		fprintf(pFile, "//---------------------------------------------\n");
		fprintf(pFile, "// �X�e�[�^�X�̂̏��\n");
		fprintf(pFile, "//---------------------------------------------\n");
		fprintf(pFile, "SCRIPT\n\n");

		fprintf(pFile, "SETSTATUS\n");
		fprintf(pFile, "{\n");
		fprintf(pFile, "	JUMPFORCE = %.2f\n", m_Status.fJumpforce);
		fprintf(pFile, "	ROTFORCE = %.2f\n", m_Status.fRotforce);
		fprintf(pFile, "	RATE_JUMPFORCE = %.2f\n", m_Status.fRateJumpforce);
		fprintf(pFile, "	RATE_ROTFORCE = %.2f\n", m_Status.fRateRotforce);
		fprintf(pFile, "}\n\n");

		fprintf(pFile, "END_SCRIPT\n\n");
	}
	else
	{
		MessageBox(NULL, "�v���C���[���t�@�C����ۑ��ł��܂���ł�www", "�x���I", MB_ICONWARNING);
	}

	fclose(pFile);

}

// ----------------------------------------
// �X�e�[�^�X�̐ݒ�
// ----------------------------------------
void CPlayer::SetStatus(STATUS_INFO Status)
{
	m_Status = Status;
}

// ----------------------------------------
// �W�����v�㏸
// ----------------------------------------
void CPlayer::AddJump(void)
{
	// �ϐ��錾
	CItem * pItem = NULL;	// �A�C�e��
	int nMaxItem = 0;		// �ő�A�C�e��
							// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pItem == NULL) pItem = (CItem*)CScene::GetActor(CScene::ACTOR_ITEM, CScene::LAYER_3DOBJECT, nCntLayer);	// ��
	}
	// �ő吔�擾
	nMaxItem = pItem->GetLoad()->GetJump();
	// �W�����v�̃A�C�e���A�b�v
	m_nNumJumpItem++;
	// �W�����v���A�b�v
	m_Status.fRateJumpforce += (PLAYER_JUMP_UP / nMaxItem);
}

// ----------------------------------------
// ��]�㏸
// ----------------------------------------
void CPlayer::AddRot(void)
{
	// �ϐ��錾
	CItem * pItem = NULL;	// �A�C�e��
	int nMaxItem = 0;		// �ő�A�C�e��
	// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pItem == NULL) pItem = (CItem*)CScene::GetActor(CScene::ACTOR_ITEM, CScene::LAYER_3DOBJECT, nCntLayer);	// ��
	}
	// �ő吔�擾
	nMaxItem = pItem->GetLoad()->GetRot();
	// ��]���A�b�v
	m_nNumRotItem++;
	// ��]���A�b�v
	m_Status.fRateRotforce += (PLAYER_ROT_UP / nMaxItem);
}

// ----------------------------------------
// �X�e�[�^�X�̐ݒ�
// ----------------------------------------
void CPlayer::Restriction(void)
{
	// X�̐���
	if (m_pos.x > PLAYER_REST_X)
	{
		m_pos.x = PLAYER_REST_X;
	}
	else if(m_pos.x < -PLAYER_REST_X)
	{
		m_pos.x = -PLAYER_REST_X;
	}
	// Z�̐���
	if (m_pos.z > PLAYER_REST_Z)
	{
		m_pos.z = PLAYER_REST_Z;
	}
	else if (m_pos.z < -PLAYER_REST_Z)
	{
		m_pos.z = -PLAYER_REST_Z;
	}
}

// ----------------------------------------
//
// ----------------------------------------
bool CPlayer::GetbJump(void)
{
	return m_bJump;
}