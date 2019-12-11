// ------------------------------------------------------------------------------------------
//
// �f�ޏ��� [materials.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "floor.h"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------�I�u�W�F�N�g���---------- //
typedef enum
{
	MATERIALS_OBJECT_00_WOOD_GREEN = 0,
	MATERIALS_OBJECT_01_WOOD_RED,
	MATERIALS_OBJECT_02_WOOD_ORANGE,
	MATERIALS_OBJECT_MAX
} MATERIALS_OBJECT;

// ---------�A�C�e�����---------- //
typedef enum
{
	MATERIALS_ITEM_00_APPLE = MATERIALS_OBJECT_MAX,
	MATERIALS_ITEM_01_ACTINIDIA,
	MATERIALS_ITEM_MAX
} MATERIALS_ITEM;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------

// ---------- �\�� ---------- //
typedef struct
{
	int nHPRecovery;
	int nMPRecovery;
} ABILITY;

// ---------- �f�� ---------- //
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posold;				// �O�̈ʒu
	D3DXVECTOR3 posKeyBetween;		// �L�[�Ԃ̋���
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ���݉�]��
	D3DXVECTOR3 rotLast;			// ������������
	D3DXVECTOR3 rotbetween;			// ��]�̍���
	D3DXVECTOR3 rotKeyBetween;		// �L�[�Ԃ̉�]�̍���
	D3DXVECTOR3	size;			// �T�C�Y
	D3DXVECTOR3 vtxMinMaterials;	// ���f���̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxMaterials;	// ���f���̈ʒu�̍ő�l
	D3DXMATRIX  mtxWorldMaterials;	// ���[���h�}�g���b�N�X
	FLOOR		*pFloor;			// ��
	int			nDrawType;			// �`��^�C�v
	int			nTexType;			// �e�N�X�`���[�^�C�v
	int			nSelect;			// �I�u�W�F�N�g���A�C�e����
	int			IdxShadow;			// �e��NO.
	int			nIdxDome;			// �h�[����NO.
	float		fRadius;			// ���a
	bool		bHit;				// ������������
	bool		bUse;				// �g�p��
	bool		bDisp;				// �\�����
	// �\�͕ω�
	ABILITY		ability;			// �\��
} MATERIALS;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitMaterials(void);
void UninitMaterials(void);
void UpdateMaterials(void);
void DrawMaterials(void);

// �����蔻��
int ColisionMaterials(
	D3DXVECTOR3 *pos,				// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,			// �O��̃|�W�V����
	D3DXVECTOR3 *move,				// �ړ���
	D3DXVECTOR3 *size,				// �T�C�Y
	MATERIALS	**ppMaterialsRet	// �f�ނ̃|�C���^�[�̃|�C���^�[
);

// �ݒ�
void SetMaterials(
	D3DXVECTOR3 pos,			// �ʒu
	D3DXVECTOR3 rot,			// ��]
	D3DXVECTOR3 move,			// �ړ���
	int			nDrawType,		// �`��^�C�v
	int			nHPRecovery,	// HP�p
	int			nMPRecovery		// MP�p
);

MATERIALS *GetMaterials(void);
#endif
