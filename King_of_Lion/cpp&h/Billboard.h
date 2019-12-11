// ------------------------------------------------------------------------------------------
//
// �r���{�[�h���� [billboard.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "wall.h"
#include "materials.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define MATERIALS_MAX (128)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	BILLBOARDTYPE_WOOD = 0,
	BILLBOARDTYPE_ENTER,
	BILLBOARDTYPE_TREE,
	BILLBOARDTYPE_RION,
	BILLBOARDTYPE_MAX
} BILLBOARDTYPE;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��̈ʒu
	D3DXVECTOR3 rot;			// ��]��
	D3DXVECTOR3 size;			// �T�C�Y
	D3DXMATRIX  mtxWorldBillboard;	// �}�g���b�N�X
	WALL		*pWall;				// �Ώۂ̕ǂ̃|�C���g
	MATERIALS	*pMaterials;		// �Ώۂ̑f�ނ̃|�C���g
	int			nType;				// ���
	float		fAlpha;				// �A���t�@�l�ω�
	bool		bDisp;				// �\����ԂȂ�
	bool		bUse;				// �g�p���
} BILLBOARD;						

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);

void SetBillboard(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nType);

BILLBOARD *GetBillboard(void);

void SetPositionBillboard(int nType, D3DXVECTOR3 pos);
void DeleteBillboard(int nType);

#endif
