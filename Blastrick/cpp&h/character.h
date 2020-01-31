// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �L�����N�^�[���� [character.h]
// Author : koki_nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCollision;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCharacter : public CScene
{
public:
	/* �񋓌^ */
	// �X�e�[�^�X
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_BEFORE_AT,
		STATE_ATTACK,
		STATE_AFTER_AT,
		STATE_DAMAGE,
		STATE_DIE
	} STATE;
	// �L�����N�^�[
	typedef enum
	{
		CHARACTER_PLAYER = 0,
		CHARACTER_ROTHUMAN,
		CHARACTER_ENEMY,
		CHARACTER_MAX
	} CHARACTER;
	// �����������̔���
	typedef enum
	{
		BLUST_STOP = 0,
		BLUST_SLASH,
		BLUST_UP,
		BLUST_NEAR,
		BLUST_MAX
	} BLUST;
	/* �\���� */
	//-------------------------------------------------------------------------------------------------------------
	// �X�e�[�^�X���
	// 1�X�e�[�^�X�A2�I�t�Z�b�g�n�_�A3�I�t�Z�b�g�I�_�A4�F�A5�^�C�v�A6�T�C�Y�A7���̌��A8�t���[����
	//-------------------------------------------------------------------------------------------------------------
	typedef struct STATUS
	{
		/* �֐� */
		// ������
		STATUS()
		{
			nMaxLife = 0;				// �ő�HP
			nMaxMP = 0;					// �ő�MP
			nAttack = 0;				// �U����
			nInviTime = 0;				// ���G����
			fLength = 0;				// �U���̓�����͈�
			State = STATE_NORMAL;		// ����̃X�e�[�^�X
			nCntState = 0;				// �J�E���g�X�e�[�^�X
			nLife = 0;					// HP
			nLastLife = 0;				// �ړI��HP
			nMP = 0;					// MP
			nLastMP = 0;				// �ړI��MP
			fAlpha = 1.0f;				// �A���t�@�l
			bAttack = false;			// �U�����
			bDefense = false;			// �h����
			bInvincible = false;		// ���G���
		}
		/* �ϐ� */
		STATE		State;			// ����̃X�e�[�^�X
		int			nCntState;		// �J�E���g�X�e�[�^�X
		int			nMaxLife;		// �ő�HP
		int			nLife;			// HP
		int			nLastLife;		// �ړI��HP
		int			nMaxMP;			// �ő�MP
		int			nMP;			// MP
		int			nLastMP;		// �ړI��MP
		int			nAttack;		// �U����
		float		fLength;		// �U���̓�����͈�
		float		fAlpha;			// �A���t�@�l
		bool		bAttack;		// �U�����
		bool		bDefense;		// �h����
		bool		bInvincible;	// ���G���
		int			nInviTime;		// ���G����
	} STATUS,*PSTATUS;
	/* �֐� */
	CCharacter();
	CCharacter(ACTOR actor);
	~CCharacter();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// �K�v�ɉ���������
	void AplayDamage(
		D3DXVECTOR3 const &pos,
		BLUST		const &bBlust,
		int			const &nDamage
	);			// �_���[�W
	
	// �ݒ�
	void SetCharacter(CHARACTER const character);	// �L�����N�^�[
	void SetMotion(int const nMotiontype);			// ���[�V����
	void SetPos(D3DXVECTOR3 const &pos);			// �ʒu
	void SetMove(D3DXVECTOR3 const &move);			// �ړ���
	void SetRot(D3DXVECTOR3 const &rot);			// ��]
	void SetAttack(bool bAttack);					// �U�����
	void SetState(STATE const &state);				// ���
	void SetInvi(bool const bInvi);					// ���G���
	// �擾
	CHARACTER	GetCharacter(void);					// �L�����N�^�[
	D3DXVECTOR3 &GetPos(void);						// �ʒu
	D3DXVECTOR3 &GetMove(void);						// �ړ����
	D3DXVECTOR3 &GetRot(void);						// ��]
	D3DXVECTOR3 *GetPartsRot(int const nModelID);	// �e�q�֌W�̑S��]
	D3DXVECTOR3 * GetPartsPos(int const nModelID);	// �e�q�֌W�̑S�ʒu
	D3DXMATRIX *GetMatrix(int const nModelID);		// �}�g���b�N�X
	int GetMotion(void);							// ���[�V�����ԍ�
	int GetKeyInfoCnt(void);						// �L�[�J�E���g
	STATUS &GetStatus(void);						// �X�e�[�^�X
	STATE GetState(void);							// ���
	bool GetAttack(void);							// �U�����	
	bool GetFloorHeight(void);						// ���̍���
	// ���[�V�����̃t���[�����擾����
	int GetMaxFrame(
		CHARACTER character,					// �L�����N�^�[
		int nMotionID,							// ���[�V����ID
		int nNowKeyCnt = -1						// ���݂̃L�[�J�E���g
	);												
	// �J�������擾
	int GetCameraType(void);					// �J�������擾
	// �쐬
	static CCharacter * Create(				
		int const nMaxMotion,
		CHARACTER const character
		);
	// ���[�h�E�A�����[�h
	static void Load(
		CHARACTER const character,		// �L�����N�^�[
		int const nMaxMotion,			// �ő僂�[�V����
		const char * file_name			// �t�@�C����
		);			
	static HRESULT LoadStatus(void);	// �X�e�[�^�X���
	static void UnLoad(
		CHARACTER const character,		// �L�����N�^�[
		int const nMaxkey				// �ő�L�[
		);	
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
protected:
	void FagGravity(void);							// �d��
	void SetRotDest(D3DXVECTOR3 const &rotDest);		// �ڕW��]�ʐݒ�
	D3DXVECTOR3 GetRotDest(void);						// �ڕW��]�ʎ擾
	// ��
	bool				m_bDamage;						// �_���[�W��������������
private:
	/* �֐� */
	void NextKeyMotion(void);							// ���̃��[�V����
	void Move(void);									// �ړ�
	void Motion(void);									// ���[�V����
	void AttackCollision(void);							// �U���̔���
	void InviTime(void);								// ���G����
	void ModelUpdate(void);								// ���f���̍X�V
	// ��Ԍ���
	void State(void);									// ���
	void NormalState(void);								// ���ʂ̏��
	void Before_ATState(void);							// �U���O�̏��
	void AttackState(void);								// �U�����
	void After_ATState(void);							// �U����̏��
	void DamageState(void);								// �_���[�W���
	void DieState(void);								// ���̏��

	void NockBack(void);								// �m�b�N�o�b�N
	void Dead(void);									// ��
	/* �ϐ� */
	/* �\���̂̃X�^�e�B�b�N�ɂ��� */
	static MODEL_ALL	*m_modelAll[CHARACTER_MAX];		// ���f���S�̂̏��
	static CModel_info	*m_model_info[CHARACTER_MAX];	// ���f�����
	static int			m_NumModel[CHARACTER_MAX];		// �ő僂�f����
	static STATUS		m_sStatus[CHARACTER_MAX];		// �L�����N�^�[���ׂẴX�^�[�^�X���
	CModel 				*m_pModel;						// ���f��
	CHARACTER			m_character;					// �L�����N�^�[
	D3DXMATRIX			m_mtxWorld;						// �s��
	D3DXVECTOR3			m_pos;							// �ʒu
	D3DXVECTOR3			m_posold;						// �O�̈ʒu
	D3DXVECTOR3			m_move;							// �ړ���
	D3DXVECTOR3			m_BlustMove;					// ������΂�
	D3DXVECTOR3			m_rot;							// ���݉�]��
	D3DXVECTOR3			m_rotLast;						// ������������
	D3DXVECTOR3			m_rotbetween;					// ��]�̍���
	D3DXVECTOR3			m_size;							// �L�����N�^�[�̃T�C�Y
	int					m_nMotiontype;					// ���[�V�����^�C�v
	int					m_nMotiontypeOld;				// �O��̃��[�V�����^�C�v
	int					m_nMaxMotion;					// �ő僂�[�V������
	int					m_keyinfoCnt;					// �L�[���̃J�E���g
	int					m_nFrame;						// �t���[���J�E���g
	STATUS				m_Status = {};					// �X�e�[�^�X���
	int					m_nIdAttackKey;					// �U���p�̃L�[ID
	int					m_nSlow;						// �X���[
	int					m_nIDWho;						// �G��������
	BLUST				m_blust;						// ������ѕ�
	CCollision			*m_pCollision;					// �����蔻��
};

#endif
