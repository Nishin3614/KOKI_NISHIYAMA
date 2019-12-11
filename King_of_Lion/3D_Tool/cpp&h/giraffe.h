// ------------------------------------------------------------------------------------------
//
// ���f������ [giraffe.h]
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
#define GIRAFFE_TEX1 "data/MODEL"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	GIRAFFEMOSIONSET_NEUTRAL = 0,
	GIRAFFEMOSIONSET_MOVE,
	GIRAFFEMOSIONSET_JUMP,
	GIRAFFEMOSIONSET_LANDING,
	GIRAFFEMOSIONSET_NORMALATACK,
	GIRAFFEMOSIONSET_MAX
} GIRAFFEMOSIONSET;

// ---------�f�ޏ��---------- //
typedef enum
{
	GIRAFFEDRAWTYPE_00_WOOD_GREEN = 0,
	GIRAFFEDRAWTYPE_MAX
} GIRAFFEDRAWTYPE;
// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------

// ---------- �L���� ---------- //
typedef struct
{
	MODEL		model[GIRAFFETYPE_MAX];				// ���f�����
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posold;				// �O�̈ʒu
	D3DXVECTOR3 posLast;			// �s�������ʒu
	D3DXVECTOR3 posKeyBetween;		// �L�[�Ԃ̋���
	D3DXVECTOR3	posOrigin;			// �����ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ���݉�]��
	D3DXVECTOR3 rotLast;			// ������������
	D3DXVECTOR3 rotbetween;			// ��]�̍���
	D3DXVECTOR3 rotKeyBetween;		// �L�[�Ԃ̉�]�̍���
	D3DXVECTOR3 vtxMinMaterials;	// ���f���̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxMaterials;	// ���f���̈ʒu�̍ő�l
	D3DXVECTOR3 size;				// ���f���̃T�C�Y
	D3DXMATRIX  mtxWorldGiraffe;		// ���[���h�}�g���b�N�X
	WALL		*pWall;				// �Ώۂ̕ǂ̃|�C���g
	FLOOR		*pMeshField;		// �Ώۂ̏��̃|�C���g
	MATERIALS	*pMaterials;		// �Ώۂ̑f�ނ̃|�C���g
	int			nMotionType;		// ���[�V�����^�C�v
	int			nMotionTypeOld;		// �O��̃��[�V�����^�C�v
	int			nParent;			// �e
	int			nIndex;				// �p�[�c
	int			nFram;				// �t���[��
	int			nCntKeySet;			// �L�[�Z�b�g�J�E���g
	int			nIdxShadow;			// �e�̔ԍ�
	float		fLength;			// ����
	bool		bUse;				// �g�p��
	bool		bDisp;				// �\�����
} GIRAFFE;

// ---------- ���f���L�[��� ---------- //
typedef struct
{
	int nLoop;						// ���[�v
	int nNumKey;					// �L�[���̐�
	KEY_INFO KeyInfo[5];			// �L�[���(���[�V������)
} MOSION_GIRAFFE;


// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitGiraffe(void);
void UninitGiraffe(void);
void UpdateGiraffe(void);
void DrawGiraffe(void);

// �I��
bool SeleGiraffe(int nCntGiraffe);

void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void NotDispGiraffe(void);		// �f�ނ̏����\��

// �Z�[�u
void SaveGiraffe(void);

// ���[�h
void LoadGiraffe(void);
GIRAFFE *GetGiraffe(void);
#endif
