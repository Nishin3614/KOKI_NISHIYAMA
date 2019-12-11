// ------------------------------------------------------------------------------------------
//
// �G���� [enemy.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "floor.h"

#include "materials.h"

#include "model.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define ENEMY_TYPE (10)		// �^�C�v��
#define ENEMY_PARTS (20)	// �p�[�c��
#define ENEMY_KEY_SET (10)	// �L�[�Z�b�g��
#define ENEMY_MOTION (10)	// ���[�V������
#define ENEMY_MAT (10)		// �}�e���A����

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	ENEMYMOSIONSET_NEUTRAL = 0,
	ENEMYMOSIONSET_MOVE,
	ENEMYMOSIONSET_NORMALATACK,
	ENEMYMOSIONSET_SHOT,
	ENEMYMOSIONSET_MAX
} ENEMYMOSIONSET;

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	ENEMYSTATUS_NEUTRAL = 0,
	ENEMYSTATUS_DAMAGE,
	ENEMYSTATUS_DIE,
	ENEMYSTATUS_MAX
} ENEMYSTATUS;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- �g���C�G ---------- //
typedef struct
{
	// �G��p�ϐ�
	int				Num_Model;								// �G��
	char			aFileModel[ENEMY_PARTS][128];

	// �L�����N�^�[�̌l�X�e�[�^�X
	float			move;				// �ړ���
	float			fJump;				// �W�����v��
	float			fRadius;			// ���a

} ENEMYMODELINFO;

// ---------- �g���C�G ---------- //
typedef struct
{
	LPD3DXMESH			paMesh;							// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		paBuffMat;						// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;						// �}�e���A���̐�
	LPDIRECT3DTEXTURE9	paTextureEnemy[ENEMY_MAT];		// �e�N�X�`���ւ̃|�C���^
	D3DXMATRIX			mtxWorldModel;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3			pos;							// �ʒu
	D3DXVECTOR3			posLast;						// �s�������ʒu
	D3DXVECTOR3			posKeyBetween;					// �L�[�Ԃ̋���
	D3DXVECTOR3			posOrigin;						// �����ʒu
	D3DXVECTOR3			rot;							// ���݉�]��
	D3DXVECTOR3			rotLast;						// �s����������
	D3DXVECTOR3			rotKeyBetween;					// �L�[�Ԃ̉�]��
	D3DXVECTOR3			size;							// �T�C�Y
	D3DXVECTOR3			vtxMinMaterials;				// �G�̈ʒu�̍ŏ��l
	D3DXVECTOR3			vtxMaxMaterials;				// �G�̈ʒu�̍ő�l
	int					nIdxModelParent;				// �e�G�̃C���f�b�N�X
	int					nFram;							// �t���[��
	int					nTexType;						// �e�N�X�`���[�^�C�v
	int					nCntKeySet;						// �L�[�Z�b�g�J�E���g

	char				aFileTex[ENEMY_MAT][128];		// �e�N�X�`���[�f�[�^
} ENEMYMODEL;

// ---------- �L�[�v�f ---------- //
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} ENEMYKEY;

// ---------- �L�[��� ---------- //
typedef struct
{
	int nFram;
	ENEMYKEY key[ENEMY_PARTS];	// �p�[�c��
} ENEMYKEY_INFO;

// ---------- �G�L�[��� ---------- //
typedef struct
{
	int nLoop;									// ���[�v
	int nNumKey;								// �L�[���̐�

												// �����蔻��p //
	int			Collision_nParts;				// �p�[�c�ԍ�
	D3DXVECTOR3	Collision_Ofset;				// �I�t�Z�b�g
	float		Collision_Radius;				// �~�̔��a
	int			Collision_StartFram;			// �J�n�t���[����
	int			Collision_EndFram;				// �I���t���[����
	int			Collision_Damage;				// �_���[�W��

												// �O�՗p //
	int			Orbit_nParts;					// �p�[�c�ԍ�
	D3DXVECTOR3	Orbit_Ofset;					// �I�t�Z�b�g
	int			Orbit_Mesh;						// �O�Ղ̐�
	int			Orbit_StartFram;				// �J�n�t���[����
	int			Orbit_EndFram;					// �I���t���[����
	int			Orbit_nType;					// �^�C�v
	int			Orbit_nIdTrajectory;			// �O�Ղ̔ԍ�
	bool		Orbit_bUse;						// �O�Ղ̎g�p���

	ENEMYKEY_INFO KeyInfo[ENEMY_KEY_SET];						// �L�[���(���[�V������)
} MOSION_ENEMY;

// ---------- �G�̃X�e�[�^�X��� ---------- //
typedef struct
{
	int		nStatus;							// �X�e�[�^�X���
	int		nCntStatus;							// �J�E���g�X�e�[�g
	int		nLife;								// HP
	int		nAttack;							// �U����
	bool	bColition;							// ������������
	bool	bAttack;							// �U�����
	bool	bShot;								// ���ˏ��
	bool	bMove;								// �ړ����
	bool	bRot;								// ��]���
} ENEMY_STATUS;

// ---------- �G ---------- //
typedef struct
{
	ENEMYMODEL		model[ENEMY_PARTS];		// �G���
	D3DXVECTOR3		pos;					// �ʒu
	D3DXVECTOR3		posold;					// �O�̈ʒu
	D3DXVECTOR3		posLast;				// �s�������ʒu
	D3DXVECTOR3		posBetween;				// �ʒu�̍���
	D3DXVECTOR3		move;					// �ړ���
	D3DXVECTOR3		rot;					// ���݉�]��
	D3DXVECTOR3		rotLast;				// ������������
	D3DXVECTOR3		rotbetween;				// ��]�̍���
	D3DXVECTOR3		vtxMinMaterials;		// �G�̈ʒu�̍ŏ��l
	D3DXVECTOR3		vtxMaxMaterials;		// �G�̈ʒu�̍ő�l
	D3DXVECTOR3		size;					// �G�̃T�C�Y
	D3DXMATRIX		mtxWorldEnemy;			// ���[���h�}�g���b�N�X
	WALL			*pWall;					// �Ώۂ̕ǂ̃|�C���g
	FLOOR			*pMeshField;			// �Ώۂ̏��̃|�C���g
	MATERIALS		*pMaterials;			// �Ώۂ̑f�ނ̃|�C���g
	int				nMotionType;			// ���[�V�����^�C�v
	int				nMotionTypeOld;			// �O��̃��[�V�����^�C�v
	int				nParent;				// �e
	int				nIndex;					// �p�[�c
	int				nFram;					// �t���[��
	int				nCntKeySet;				// �L�[�Z�b�g�J�E���g
	int				nIdxShadow;				// �e�̔ԍ�
	int				nType;					// �L�����N�^�[�̎��
	int				nTexType;				// �e�N�X�`���[���
	float			fLength;				// ����
	bool			bUse;					// �g�p��
	bool			bDisp;					// �\�����

	// �J���[���
	int			nCntCor;					// �J���[�̏�ԃJ�E���g
	float		diffuse;					// �����

	// �X�e�[�^�X //
	ENEMY_STATUS	status;					// �X�e�[�^�X

	// ���՗p
	int nCntTotal;							// ���v
} ENEMY;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void HitEnemy(int nCntEnemy,int nDamage);		// �U����������������

void ColisionEnemy(
	D3DXVECTOR3 * pos, 
	D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size);

// �G�̌��j��
int GetEnemyTotal(void);

ENEMY *GetEnemy(void);
#endif
