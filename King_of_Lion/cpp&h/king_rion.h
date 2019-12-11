// ------------------------------------------------------------------------------------------
//
// �L�������� [king_rion.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _KING_RION_H_
#define _KING_RION_H_

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
#define KING_RIONTYPE_MAX (13)
#define	MAX_KING_RION (10)
#define KING_RION_KEYSET_MAX (10)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	KING_RIONMOSIONSET_NEUTRAL = 0,
	KING_RIONMOSIONSET_MOVE,
	KING_RIONMOSIONSET_RUNMOVE,
	KING_RIONMOSIONSET_NORMALATACK,
	KING_RIONMOSIONSET_SHOT,
	KING_RIONMOSIONSET_MAX
} KING_RIONMOSIONSET;

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	KING_RIONSTATUS_NEUTRAL = 0,
	KING_RIONSTATUS_DAMAGE,
	KING_RIONSTATUS_DIE,
	KING_RIONSTATUS_MAX
} KING_RIONSTATUS;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- �S�b�̉��̃X�e�[�^�X��� ---------- //
typedef struct
{
	int		nStatus;							// �X�e�[�^�X���
	int		nCntStatus;							// �J�E���g�X�e�[�g
	int		nCntAttack;							// �U������J�E���g
	int		nLife;								// HP
	int		nAttack;							// �U����
	bool	bColition;							// ������������
	bool	bAttack;							// �U�����
	bool	bShot;								// ���ˏ��
	bool	bMove;								// �ړ����
	bool	bRot;								// ��]���
} KING_RION_STATUS;

// ---------- �L���� ---------- //
typedef struct
{
	MODEL			model[KING_RIONTYPE_MAX];	// �L�������
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
	D3DXVECTOR3		vtxMinMaterials;			// �L�����̈ʒu�̍ŏ��l
	D3DXVECTOR3		vtxMaxMaterials;			// �L�����̈ʒu�̍ő�l
	D3DXVECTOR3		size;						// �L�����̃T�C�Y
	D3DXMATRIX		mtxWorldKing_Rion;			// ���[���h�}�g���b�N�X
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
	int				nAttack;					// �A�^�b�N�J�E���g
	float			fLength;					// ����
	bool			bUse;						// �g�p��
	bool			bDisp;						// �\�����

	// �J���[���
	int			nCntCor;						// �J���[�̏�ԃJ�E���g
	float		diffuse;						// �����

	// �X�e�[�^�X //
	KING_RION_STATUS	status;					// �X�e�[�^�X

	// ���՗p
	int nCntTotal;								// ���v
} KING_RION;

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

	KEY_INFO KeyInfo[KING_RION_KEYSET_MAX];		// �L�[�Z�b�g���
} MOSION_KING_RION;


// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitKing_Rion(void);
void UninitKing_Rion(void);
void UpdateKing_Rion(void);
void DrawKing_Rion(void);

void HitKing_Rion(int nCntKing_Rion,int nDamage);		// �U����������������

void ColisionKing_Rion(
	D3DXVECTOR3 * pos,		// �ʒu
	D3DXVECTOR3 * posOld, 	// �O��̈ʒu
	D3DXVECTOR3 * move,		// �ړ���
	D3DXVECTOR3 * size		// �T�C�Y
);

// �S�b�̉��̌��j��
int GetKing_RionTotal(void);

KING_RION *GetKing_Rion(void);

MODEL *GetKing_Rion_Model(int nCntKing_Rion);
#endif
