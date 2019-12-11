// ------------------------------------------------------------------------------------------
//
// �Ǐ��� [wall.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define WALL_MAX (128)

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pIndex = NULL;			// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 pos;								// �ʒu
	D3DXVECTOR3 move;								// �ړ���
	D3DXVECTOR3 rot;								// ��]��
	D3DXVECTOR3 size;								// �T�C�Y
	D3DXMATRIX  mtxWorldWall;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 OriginBlock;						// �u���b�N�`��̌��_
	int	nNumberVertexMeshField;						// �����_��
	int nNumIndex;									// ���C���f�b�N�X
	int nNumPolygon;								// ���|���S��
	int nNumber;									// ���_
	int nBlock_Depth;								// �c�u���b�N��
	int nBlock_Width;								// ���u���b�N��
	int nType;										// �^�C�v
	bool		bUse;								// �g�p���
	bool		bDisp;								// �\�����

	// �����蔻��Ɏg���ϐ�
	D3DXVECTOR3	BiginPos;							// ���_�Ƃ���
	D3DXVECTOR3 LastPos;							// �ŏI�n�_
	D3DXVECTOR3	Length;								// �ǂ̒���
	D3DXVECTOR3 VecWall;							// �ǂ̃x�N�g��
	float		fdistance;							// ����
} WALL;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

// �����蔻��
bool ColisionWall(
	D3DXVECTOR3 *pos,		// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,	// �O��̃|�W�V����
	D3DXVECTOR3 *move,		// �ړ���
	D3DXVECTOR3 *size,		// �T�C�Y
	WALL		**ppWallRet);

WALL *GetWall(void);
#endif
