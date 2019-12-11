// ------------------------------------------------------------------------------------------
//
// �o�b�t�@���[���� [buffalo.h]
// Author : 
//
// ------------------------------------------------------------------------------------------
#ifndef _BUFFALO_H_
#define _BUFFALO_H_

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
#define BUFFALOTYPE_MAX (13)	// ���f���^�C�v
#define	MAX_BUFFALO (128)		// �o�b�t�@���[�̍ő��

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	BUFFALOMOSIONSET_NEUTRAL = 0,
	BUFFALOMOSIONSET_MOVE,
	BUFFALOMOSIONSET_ASSAULT_WAIT,
	BUFFALOMOSIONSET_ASSAULT,
	BUFFALOMOSIONSET_MAX
} BUFFALOMOSIONSET;

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	BUFFALOSTATUS_NEUTRAL = 0,
	BUFFALOSTATUS_DAMAGE,
	BUFFALOSTATUS_DIE,
	BUFFALOSTATUS_MAX
} BUFFALOSTATUS;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- �L�����̃X�e�[�^�X��� ---------- //
typedef struct
{
	int		nStatus;							// �X�e�[�^�X���
	int		nCntStatus;							// �J�E���g�X�e�[�g
	int		nLife;								// HP
	int		nAttack;							// �U����
	bool	bColition;							// ������������
	bool	bAttack;							// �U�����
	bool	bMove;								// �ړ����
	bool	bRot;								// ��]���
} BUFFALO_STATUS;

// ---------- �o�b�t�@���[ ---------- //
typedef struct
{
	MODEL			model[BUFFALOTYPE_MAX];		// �o�b�t�@���[���
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		posold;						// �O�̈ʒu
	D3DXVECTOR3		posLast;					// �s�������ʒu
	D3DXVECTOR3		posBetween;					// �ʒu�̍���
	D3DXVECTOR3		posKeyBetween;				// �L�[�Ԃ̋���
	D3DXVECTOR3		posOrigin;					// �����ʒu
	D3DXVECTOR3		move;						// �ړ���
	D3DXVECTOR3		rot;						// ���݉�]��
	D3DXVECTOR3		rotLast;					// ������������
	D3DXVECTOR3		rotbetween;					// ��]�̍���
	D3DXVECTOR3		rotKeyBetween;				// �L�[�Ԃ̉�]�̍���
	D3DXVECTOR3		vtxMinMaterials;			// �o�b�t�@���[�̈ʒu�̍ŏ��l
	D3DXVECTOR3		vtxMaxMaterials;			// �o�b�t�@���[�̈ʒu�̍ő�l
	D3DXVECTOR3		size;						// �o�b�t�@���[�̃T�C�Y
	D3DXMATRIX		mtxWorldBuffalo;			// ���[���h�}�g���b�N�X
	WALL			*pWall;						// �Ώۂ̕ǂ̃|�C���g
	FLOOR			*pMeshField;				// �Ώۂ̏��̃|�C���g
	MATERIALS		*pMaterials;				// �Ώۂ̑f�ނ̃|�C���g
	int				nMotionType;				// ���[�V�����^�C�v
	int				nMotionTypeOld;				// �O��̃��[�V�����^�C�v
	int				nCntMotion;					// ���[�V�����̐؂�ւ��J�E���g
	int				nParent;					// �e
	int				nIndex;						// �p�[�c
	int				nFram;						// �t���[��
	int				nCntKeySet;					// �L�[�Z�b�g�J�E���g
	int				nIdxShadow;					// �e�̔ԍ�
	int				nAttack;					// �A�^�b�N�J�E���g
	float			fLength;					// ����
	bool			bUse;						// �g�p��
	bool			bDisp;						// �\�����

	// �J���[���
	int			nCntCor;						// �J���[�̏�ԃJ�E���g
	float		diffuse;						// �����

	// �X�e�[�^�X //
	BUFFALO_STATUS	status;						// �X�e�[�^�X

	// ���՗p
	int nCntTotal;								// ���v
} BUFFALO;

// ---------- �o�b�t�@���[�L�[��� ---------- //
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

	KEY_INFO KeyInfo[10];						// �L�[���(���[�V������)
} MOSION_BUFFALO;


// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitBuffalo(void);
void UninitBuffalo(void);
void UpdateBuffalo(void);
void DrawBuffalo(void);

void HitBuffalo(int nCntBuffalo, int nDamage);		// �U����������������

void ColisionBuffalo(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 * posOld,
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size);

void SetBuffalo(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot);		// �U����������������

BUFFALO *GetBuffalo(void);

// �o�b�t�@���[�̌��j��
int GetBuffaloTotal(void);
#endif
