// ------------------------------------------------------------------------------------------
//
// �|���S������ [floor.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffFLOOR = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;									// �ʒu
	D3DXVECTOR3 move;									// �ړ���
	D3DXVECTOR3 rot;									// ��]��
	D3DXVECTOR3 size;									// �T�C�Y
	D3DXVECTOR3 OriginBlock;							// �u���b�N�`��̌��_
	D3DXMATRIX  mtxWorldFLOOR;							// ���[���h�}�g���b�N�X
	D3DXCOLOR	col;									// �J���[
	int	nNumberVertexMeshField;							// �����_��
	int nNumIndex;										// ���C���f�b�N�X
	int nNumPolygon;									// ���|���S��
	int nNumber;										// ���_
	int nBlock_Depth;									// �c�u���b�N��
	int nBlock_DepthOld;								// �O��̏c�u���b�N��
	int nBlock_Width;									// ���u���b�N��
	int nBlock_WidthOld;								// �O��̉��u���b�N��
	int nType;											// ���
	bool bUse;											// �g�p���
	bool bDisp;											// �\�����
	bool bSelect;										// �I�����
} FLOOR;

typedef struct
{
	D3DXVECTOR3 size;									// �T�C�Y
	int nBlock_Depth;									// �c�u���b�N��
	int nBlock_Width;									// ���u���b�N��
	int nType;											// �^�C�v
} FLOORSET;


// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);
// ���̐ݒu
void SetFloor(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType);

// �����蔻��
bool ColisionFloor(
	D3DXVECTOR3 *pos,		// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,	// �O��̃|�W�V����
	D3DXVECTOR3 *move,		// �ړ���
	D3DXVECTOR3 *size,		// �T�C�Y
	FLOOR		**ppFloorRet);

// �\���ؑ֎��ɕ\����
void NotDispFLoor(void);	

// �Z�[�u
void SaveFloor(void);
// ���[�h
void LoadFloor(void);

// �I��
bool SeleFloor(int nCntFloor);

// ���̏��擾
FLOOR *GetFloor(void);

#endif
