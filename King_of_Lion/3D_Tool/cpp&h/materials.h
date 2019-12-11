// ------------------------------------------------------------------------------------------
//
// �f�ޏ��� [materials.h]
// Author : 
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

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define MATERIALS_MAX (128)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------�f�ޏ��---------- //
typedef enum
{
	MATERIALSTYPE_00_WOOD_GREEN = 0,
	MATERIALSTYPE_00_WOOD_GREE1,
	MATERIALSTYPE_00_WOOD_GREE12,
	MATERIALSTYPE_MAX
} MATERIALSTYPE;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------

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
	D3DXVECTOR3	size;				// �T�C�Y
	D3DXVECTOR3 vtxMinMaterials;	// ���f���̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxMaterials;	// ���f���̈ʒu�̍ő�l
	D3DXMATRIX  mtxWorldMaterials;	// ���[���h�}�g���b�N�X
	int			nDrawType;			// �`��^�C�v
	int			nTexType;			// �e�N�X�`���[�^�C�v
	float		fLength;			// ����
	bool		bUse;				// �g�p��
	bool		bDisp;				// �\�����
} MATERIALS;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitMaterials(void);
void UninitMaterials(void);
void UpdateMaterials(void);
void DrawMaterials(void);

// �����蔻��
bool ColisionMaterials(
	D3DXVECTOR3 *pos,		// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,	// �O��̃|�W�V����
	D3DXVECTOR3 *move,		// �ړ���
	D3DXVECTOR3 *size,		// �T�C�Y
	MATERIALS		**ppMaterialsRet);

void NotDisp_Materials(void);		// �f�ނ̏����\��
MATERIALS *GetMaterials(void);

// �I��
bool SeleMaterials(int nCntMaterials);

// �Z�[�u
void SaveMaterials(void);

// ���[�h
void LoadMaterials(void);

#endif
