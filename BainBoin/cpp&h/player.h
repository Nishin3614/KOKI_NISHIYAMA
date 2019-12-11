//=============================================================================
//
// ���f������ [player.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"
#include "model.h"
#include "motion.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYERCOLLISION_SIZE (30.0f)

//=============================================================================
// �O���錾
//=============================================================================

//=============================================================================
// �N���X
//=============================================================================
class CPlayer : public CScene
{
public:
	// ---------���[�V�����^�C�v---------- //
	typedef enum
	{
		MOTIONTYPE_WAIT = 0,
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_LANDING_FAIL1,
		MOTIONTYPE_LANDING_FAIL2,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,
		PLAYER_STATE_DAMAGE,
		PLAYER_STATE_DIE
	} PLAYER_STATE;

	// ---------���f�����---------- //
	typedef enum
	{
		PLAYERTYPE_00_RION_BODY = 0,
		PLAYERTYPE_01_RION_HEAD,
		PLAYERTYPE_MAX
	} PLAYERTYPE;

	/* �֐� */
	CPlayer();
	~CPlayer();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);		// �ʒu�擾
	D3DXVECTOR3 GetRot(void);		// ��]�擾
	static CPlayer * Create();		// �쐬
	static void Load(void);			// �ǂݍ���
	static void UnLoad(void);		// UnLoad����
	// �X�e�[�^�X�ǂݍ���
	void LoadStatus(void);
	// �X�e�[�^�X�ۑ�
	void SaveStatus(void);
	// �X�e�[�^�X�̐ݒ�
	void SetStatus(STATUS_INFO Status);
	// �X�e�[�^�X�㏸
	void AddJump(void);
	void AddRot(void);
	// �錾���
	void Restriction(void);
	bool GetbJump(void);

protected:
private:
	/* �֐� */
	void Move(void);				// �ړ�
	float GetResultJumpforce(void);
	float GetResultRotforce(void);

	// �L�k����
	void ExpanAndContraProc(void);
	// �L�k�{���̎擾
	float GetExpanAndContraMagni(void);
	// �L�鎞�Ԃ̎擾
	int GetExtendTime(void);
	// �k�ގ��Ԃ̎擾
	int GetShrinkTime(void);
	// �Q�[�W�ʂ̔{���̎擾
	float GetJumpMagni(void);
	// �W�����v�͂̔{��
	float GetJumpforce(void);
	/* �ϐ� */
	static CModel_info			*m_pModel_info;						// ���f�����̕ۑ�
	static MODEL_OFFSET			*m_pModel_offset;					// ���f���̏����z�u
	static MOTION				*m_pMotion;							// ���[�V�������̕ۑ�
	CModel 						*m_pModel[PLAYERTYPE_MAX] = {};		// ���f��
	D3DXMATRIX					m_mtxWorld;							// �s��
	D3DXVECTOR3					m_pos;								// �ʒu
	D3DXVECTOR3					m_posold;							// �O�̈ʒu
	D3DXVECTOR3					m_move;								// �ړ���
	D3DXVECTOR3					m_rot;								// ���݉�]��
	D3DXVECTOR3					m_rotLast;							// ������������
	D3DXVECTOR3					m_rotbetween;						// ��]�̍���
	D3DXVECTOR3					m_size;								// ���f���̃T�C�Y
	int							m_nMaxModel;						// �ő僂�f����
	int							m_nMotiontype;						// ���[�V�����^�C�v
	int							m_nMotiontypeOld;					// �O��̃��[�V�����^�C�v
	int							m_nMaxMotion;						// �ő僂�[�V������
	int							m_keyinfoCnt;						// �L�[���̃J�E���g
	int							m_nFrame;							// �t���[���J�E���g
	bool						m_bJump;							// �W�����v
	float						m_fCntRot;							// ��]�J�E���g
	// �����蔻��
	CCollision					*m_pCollision;						// �����蔻����

	STATUS_INFO					m_Status;							// �X�e�[�^�X���
	int							m_nNumJumpItem;						// �l�������W�����v�A�C�e����
	int							m_nNumRotItem;						// �l��������]�A�C�e����
	D3DXVECTOR3					m_scal;								// �X�P�[��

	int							m_JumpTime;							// �W�����v���Ă��鎞�̎���
	int							m_nJumpCharge;						// �W�����v�`���[�W
	D3DXVECTOR3					m_JumpRot;
};

//=============================================================================
// �\����
//=============================================================================
// ---------- ��l���̃X�e�[�^�X��� ---------- //
typedef struct
{
	int		nState;		// ����̃X�e�[�^�X
	int		nCntState;	// �J�E���g�X�e�[�^�X
	int		nMaxLife;	// �ő�HP
	int		nLife;		// HP
	int		nLastLife;	// �ړI��HP
	int		nMaxMP;		// �ő�MP
	int		nMP;		// MP
	int		nLastMP;	// �ړI��MP
	int		nAttack;	// �U����
	float	fLength;	// �U���̓�����͈�
	bool	bColition;	// �U�����������Ă��邩�ǂ���
} PLAYER_STATUS;
#endif
