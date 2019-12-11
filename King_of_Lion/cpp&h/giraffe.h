// ------------------------------------------------------------------------------------------
//
// �L�������� [giraffe.h]
// Author : 
//
// ------------------------------------------------------------------------------------------
#ifndef _GIRAFFE_H_
#define _GIRAFFE_H_

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
#define GIRAFFETYPE_MAX (11)
#define	MAX_GIRAFFE (128)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	GIRAFFEMOSIONSET_NEUTRAL = 0,
	GIRAFFEMOSIONSET_MOVE,
	GIRAFFEMOSIONSET_NORMALATACK,
	GIRAFFEMOSIONSET_SHOT,
	GIRAFFEMOSIONSET_MAX
} GIRAFFEMOSIONSET;

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	GIRAFFESTATUS_NEUTRAL = 0,
	GIRAFFESTATUS_DAMAGE,
	GIRAFFESTATUS_DIE,
	GIRAFFESTATUS_MAX
} GIRAFFESTATUS;

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
	bool	bShot;								// ���ˏ��
	bool	bMove;								// �ړ����
	bool	bRot;								// ��]���
} GIRAFFE_STATUS;

// ---------- �L���� ---------- //
typedef struct
{
	MODEL			model[GIRAFFETYPE_MAX];		// �L�������
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		posold;						// �O�̈ʒu
	D3DXVECTOR3		posLast;					// �s�������ʒu
	D3DXVECTOR3		posBetween;					// �ʒu�̍���
	D3DXVECTOR3		posKeyBetween;				// �L�[�Ԃ̋���
	D3DXVECTOR3		posOrigin;					// �����ʒu
	D3DXVECTOR3		posAttack;					// �W�I
	D3DXVECTOR3		move;						// �ړ���
	D3DXVECTOR3		rot;						// ���݉�]��
	D3DXVECTOR3		rotLast;					// ������������
	D3DXVECTOR3		rotbetween;					// ��]�̍���
	D3DXVECTOR3		rotKeyBetween;				// �L�[�Ԃ̉�]�̍���
	D3DXVECTOR3		vtxMinMaterials;			// �L�����̈ʒu�̍ŏ��l
	D3DXVECTOR3		vtxMaxMaterials;			// �L�����̈ʒu�̍ő�l
	D3DXVECTOR3		size;						// �L�����̃T�C�Y
	D3DXMATRIX		mtxWorldGiraffe;			// ���[���h�}�g���b�N�X
	D3DXCOLOR		EffectCol;					// �G�t�F�N�g�p�̐F
	WALL			*pWall;						// �Ώۂ̕ǂ̃|�C���g
	FLOOR			*pMeshField;				// �Ώۂ̏��̃|�C���g
	MATERIALS		*pMaterials;				// �Ώۂ̑f�ނ̃|�C���g
	int				nMotionType;				// ���[�V�����^�C�v
	int				nMotionTypeOld;				// �O��̃��[�V�����^�C�v
	int				nParent;					// �e
	int				nIndex;						// �p�[�c
	int				nFram;						// �t���[��
	int				nCntKeySet;					// �L�[�Z�b�g�J�E���g
	int				nIdxShadow;					// �e�̔ԍ�
	int				nIdxPrediction;				// �\�����̔ԍ�
	int				nAttack;					// �A�^�b�N�J�E���g
	int				nCntDamage;					// �_���[�W
	int				nCntAnim;					// �A�j��
	float			fLength;					// ����
	bool			bUse;						// �g�p��
	bool			bDisp;						// �\�����

	// �J���[���
	int			nCntCor;						// �J���[�̏�ԃJ�E���g
	float		diffuse;						// �����

	// �X�e�[�^�X //
	GIRAFFE_STATUS	status;						// �X�e�[�^�X

	// ���՗p
	int nCntTotal;								// ���v
} GIRAFFE;

// ---------- �L�����L�[��� ---------- //
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

	KEY_INFO KeyInfo[5];						// �L�[���(���[�V������)
} MOSION_GIRAFFE;


// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitGiraffe(void);
void UninitGiraffe(void);
void UpdateGiraffe(void);
void DrawGiraffe(void);

void HitGiraffe(int nCntGiraffe,int nDamage);		// �U����������������

void ColisionGiraffe(
	D3DXVECTOR3 * pos, 
	D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size);

// �L�����̌��j��
int GetGiraffeTotal(void);

void SetGiraffe(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot);		// �U����������������

GIRAFFE *GetGiraffe(void);
#endif
