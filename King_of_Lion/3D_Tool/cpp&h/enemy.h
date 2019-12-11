// ------------------------------------------------------------------------------------------
//
// �G���� [enemy.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------

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
	D3DXMATRIX  mtxWorldEnemy;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 OriginEnemy;						// �u���b�N�`��̌��_
	int	nNumberVertexMeshField;						// �����_��
	int nNumIndex;									// ���C���f�b�N�X
	int nNumPolygon;								// ���|���S��
	int nNumber;									// ���_
	int nEnemy_Depth;								// �c�u���b�N��
	int nEnemy_Width;								// ���u���b�N��
	int nType;										// �^�C�v
	bool		bUse;								// �g�p���
	bool		bDisp;								// �\�����
} ENEMY;

typedef struct
{
	D3DXVECTOR3 size;								// �T�C�Y
	D3DXVECTOR3 rot;								// ��]��
	int nEnemy_Depth;								// �c�u���b�N��
	int nEnemy_Width;								// ���u���b�N��
	int nType;										// �^�C�v
} ENEMYSET;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nEnemy_Depth,
	int	nEnemy_Width,
	int	nType);

// �����蔻��
bool ColisionEnemy(
	D3DXVECTOR3 *pos,		// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,	// �O��̃|�W�V����
	D3DXVECTOR3 *move,		// �ړ���
	D3DXVECTOR3 *size,		// �T�C�Y
	ENEMY		**ppEnemyRet);

void NotDispEnemy(void);	// �\���ؑ֎��ɕ\����

// �Z�[�u
void SaveEnemy(void);

// ���[�h
void LoadEnemy(void);

ENEMY *GetEnemy(void);

#ifdef _DEBUG			
// ���̏��
//void DrawEnemyInfo(
//	D3DXVECTOR3 size,
//	int nEnemy_W,
//	int nEnemy_H,
//	int nType);
#endif

#endif
