// ------------------------------------------------------------------------------------------
//
// ���f������ [player.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "wall.h"
#include "floor.h"
#include "materials.h"
#include "model.h"
#include "water.h"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
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
	PLAYERTYPE_02_RION_RING,
	PLAYERTYPE_03_RION_RIGHT,
	PLAYERTYPE_04_RION_LEFT,
	PLAYERTYPE_05_RION_BACK_RIGHT2,
	PLAYERTYPE_06_RION_BACK_LEFT2,
	PLAYERTYPE_07_RION_TAIL_TAIL_JOINT,
	PLAYERTYPE_08_RION_TAIL_TAIL,
	PLAYERTYPE_09_RION_TAIL_TAIL_EDGE,
	PLAYERTYPE_MAX
} PLAYERTYPE;

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	PLAYERMOSIONSET_NEUTRAL = 0,
	PLAYERMOSIONSET_MOVE,
	PLAYERMOSIONSET_JUMP,
	PLAYERMOSIONSET_LANDING,
	PLAYERMOSIONSET_NORMALATACK,
	PLAYERMOSIONSET_SHOT,
	PLAYERMOSIONSET_MAX
} PLAYERMOSIONSET;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- ��l���̃X�e�[�^�X��� ---------- //
typedef struct
{
	int		nState;			// ����̃X�e�[�^�X
	int		nCntState;		// �J�E���g�X�e�[�^�X
	int		nMaxLife;		// �ő�HP
	int		nLife;			// HP
	int		nLastLife;		// �ړI��HP
	int		nMaxMP;			// �ő�MP
	int		nMP;			// MP
	int		nLastMP;		// �ړI��MP
	int		nAttack;		// �U����
	float	fLength;	// �U���̓�����͈�
	bool	bColition;	// �U�����������Ă��邩�ǂ���
} PLAYER_STATUS;

// ---------- ���f�� ---------- //
typedef struct
{
	// �v���C���[�̊�{��� //
	MODEL		model[PLAYERTYPE_MAX];	// ���f�����
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 posold;					// �O�̈ʒu
	D3DXVECTOR3 posLast;				// �s�������ʒu
	D3DXVECTOR3 posKeyBetween;			// �L�[�Ԃ̋���
	D3DXVECTOR3	posOrigin;				// �����ʒu
	D3DXVECTOR3 move;					// �ړ���
	D3DXVECTOR3 rot;					// ���݉�]��
	D3DXVECTOR3 rotLast;				// ������������
	D3DXVECTOR3 rotbetween;				// ��]�̍���
	D3DXVECTOR3 rotKeyBetween;			// �L�[�Ԃ̉�]�̍���
	D3DXVECTOR3 vtxMinMaterials;		// ���f���̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxMaterials;		// ���f���̈ʒu�̍ő�l
	D3DXVECTOR3 size;					// ���f���̃T�C�Y
	D3DXMATRIX  mtxWorldPlayer;			// ���[���h�}�g���b�N�X
	WALL		*pWall;					// �Ώۂ̕ǂ̃|�C���g
	FLOOR		*pMeshField;			// �Ώۂ̏��̃|�C���g
	MATERIALS	*pMaterials;			// �Ώۂ̑f�ނ̃|�C���g
	WATER		*pWater;				// �Ώۂ̐��̃|�C���g
	int			nMotionType;			// ���[�V�����^�C�v
	int			nMotionTypeOld;			// �O��̃��[�V�����^�C�v
	int			nParent;				// �e
	int			nIndex;					// �p�[�c
	int			nFram;					// �t���[��
	int			nCntKeySet;				// �L�[�Z�b�g�J�E���g
	int			nIdxShadow;				// �e�̔ԍ�
	
	// �J���[���
	int			nCntCor;				// �J���[�̏�ԃJ�E���g
	float		diffuse;			// �����

	// ��l���̃X�e�[�^�X��� //
	PLAYER_STATUS status;

	float		fLength;				// ����
	bool		bUse;					// �g�p��
	bool		bjump;					// �W�����v���
} PLAYER;

// ---------- ���[�W������� ---------- //
typedef struct
{
	int			nLoop;					// ���[�v
	int			nNumKey;				// �L�[���̐�
	
	// �����蔻��p //
	int			Collision_nParts;		// �p�[�c�ԍ�
	D3DXVECTOR3	Collision_Ofset;		// �I�t�Z�b�g
	float		Collision_Radius;		// �~�̔��a
	int			Collision_StartFram;	// �J�n�t���[����
	int			Collision_EndFram;		// �I���t���[����
	int			Collision_Damage;		// �_���[�W��

	// �O�՗p //
	int			Orbit_nParts;			// �p�[�c�ԍ�
	D3DXVECTOR3	Orbit_Ofset;			// �I�t�Z�b�g
	int			Orbit_Mesh;				// �O�Ղ̐�
	int			Orbit_StartFram;		// �J�n�t���[����
	int			Orbit_EndFram;			// �I���t���[����
	int			Orbit_nType;			// �^�C�v
	int			Orbit_nIdTrajectory;	// �O�Ղ̔ԍ�
	bool		Orbit_bUse;				// �O�Ղ̎g�p���
	KEY_INFO	KeyInfo[5];				// �L�[���(���[�V������)
} MOSION_PLAYER;


// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

// �_���[�W���󂯂��Ƃ�
void HitPlayer(int fDamage);

// �v���C���[�X�e�[�^�X�X�V
void StatusUpPlayer(
	int nUpLife,
	int nUpMp,
	int nUpAttack);

// �v���C���[��
void RecoveryPlayer(
	int nRecoveryLife,
	int nRecoveryMP);

void PlayerPosition(void);

// �v���C���[���
PLAYER *GetPlayer(void);

// �v���C���[�̃��f�����
MODEL *GetPlayerModel(void);

// �v���C���[�̈ʒu���
D3DXVECTOR3 GetPlayer_Pos(void);

// �v���C���[�̑O��̈ʒu���
D3DXVECTOR3 GetPlayer_PosOld(void);

// �v���C���[�̉�]���
D3DXVECTOR3 GetPlayer_Rot(void);

// �v���C���[�̈ʒu���
float GetPlayer_Radius(void);

// �v���C���[�̏�ԏ��
PLAYER_STATUS * GetPlayerStatus(void);
#endif
