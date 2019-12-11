// ------------------------------------------------------------------------------------------
//
// �������� [explosion.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// �^�C�v
typedef enum
{
	EXPLOSIONTYPE_THUNDER = 0,
	EXPLOSIONTYPE_HIBANA,
	EXPLOSIONTYPE_MAX
} EXPLOSIONTYPE;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 size;
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldExplosion;
	D3DXCOLOR	col;
	float fSize;		// �T�C�Y
	float fAlpha;		// �A���t�@�l�ω�
	float fDestSize;	// �T�C�Y�ω�
	float fG;			// �d��
	int nLife;			// ���C�t
	EXPLOSIONTYPE type;	// �^�C�v
	int nAttack;		// �U����
	int nCounterAnim;	// �J�E���^
	int nPatternAnim;	// �p�^�[��No.
	bool bUse;
} EXPLOSION;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);

// �����ݒ�
void SetExplosion(
	D3DXVECTOR3 pos,	// �ʒu
	EXPLOSIONTYPE type,	// �^�C�v
	float fSize, 		// �T�C�Y
	int nAttack			// �U����
);
// �e�N�X�`���[�ݒ�
void SetTexExplosion(
	VERTEX_3D *pVtx,	// �ʒu���_���
	float fBeginTex,	// ���߂̃e�N�X�`���[
	float fEndTex		// �Ō�̃e�N�X�`���[
);
// �ʒu�̐ݒ�
void SetPosExplosion(
	VERTEX_3D *pVtx,	// �ʒu���_���
	int nCntExplosion	// �J�E���g
);

// �����̏��
EXPLOSION *GetExplosion(void);
#endif
