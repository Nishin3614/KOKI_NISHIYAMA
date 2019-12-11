// ------------------------------------------------------------------------------------------
//
// �e���� [bullet.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "wall.h"
#include "materials.h"

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��̈ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ��]��
	D3DXVECTOR3 size;			// �T�C�Y
	D3DXMATRIX  mtxWorldBullet;	// �}�g���b�N�X
	WALL		*pWall;			// �Ώۂ̕ǂ̃|�C���g
	MATERIALS	*pMaterials;	// �Ώۂ̑f�ނ̃|�C���g
	int			nLife;			// �o������
	int			nPattern;		// �A�j���[�V�����p�^�[��
	int			nCntAnim;		// �A�j���[�V�����J�E���g
	int			nIdxShadow;		// �e��ID
	int			nType;			// ���
	int			nAttack;		// �U����
	bool		bUse;			// �g�p���
} BULLET;						

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
// �e�̃e�N�X�`���[�ݒ�
void SetTexBullet(VERTEX_3D *pVtx,float fBeginTex,float fEndTex);
// �e�̐ݒ�
void SetBullet(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 move ,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nLife,
	int nType,
	int nAttack);
// �e�̎擾
BULLET *GetBullet(void);
#endif
