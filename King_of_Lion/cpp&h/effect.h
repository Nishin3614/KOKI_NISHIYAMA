// ------------------------------------------------------------------------------------------
//
// �G�t�F�N�g���� [effect.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------�o�����̌`---------- //
typedef enum
{
	EFFECTAPPEARANCE_NEUTRAL = 0,
	EFFECTAPPEARANCE_NORMAL,		// �ʒu�͂��̂܂�
	EFFECTAPPEARANCE_RAND,			// �ʒu�o���o��
	EFFECTAPPEARANCE_RING,			// �ʒu�ւ���
	EFFECTAPPEARANCE_MAX
} EFFECTAPPEARANCE;

// ---------�o����̓���---------- //
typedef enum
{
	EFFECTMOVETYPE_NEUTRAL = 0,	
	EFFECTMOVETYPE_NORMAL,		// ����
	EFFECTMOVETYPE_UP,			// �㏸
	EFFECTMOVETYPE_DOWN,		// ���~
	EFFECTMOVETYPE_G,			// �d��
	EFFECTMOVETYPE_RAND,		// �����_��
	EFFECTMOVETYPE_MAX
} EFFECTMOVETYPE;

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{								
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 size;			// �T�C�Y
	D3DXVECTOR3 rot;			// ��]
	D3DXMATRIX  mtxWorldEffect;	// �s��
	D3DXCOLOR	col;			// �F
	EFFECTMOVETYPE MoveType;	// �ړ��^�C�v
	float fSize;				// �T�C�Y
	float fAlpha;				// �A���t�@�l�ω�
	float fDestSize;			// �T�C�Y�ω�
	int nLife;					// ���C�t
	int nType;					// �^�C�v
	float fG;					// �d��
	bool bUse;			// �g�p���
} EFFECT;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

// �ݒ�
// �G�t�F�N�g��{���̐ݒ�
void SetInformation(
	D3DXVECTOR3 pos,			// �ʒu
	D3DXCOLOR col,				// �F
	EFFECTAPPEARANCE Appearnce,	// �o����
	EFFECTMOVETYPE Movetype ,	// �ړ��^�C�v
	int nMaxEffect,				// �ő吔
	float fSize,				// �T�C�Y
	float fSpeed, 				// ����
	int nLife					// ���C�t
	);

void SetMove(
	D3DXVECTOR3 pos,			// �ʒu
	D3DXCOLOR col,				// �F
	EFFECTMOVETYPE Movetype,	// �ړ��^�C�v
	float fSize, 				// �T�C�Y
	float fSpeed,				// ����
	int nLife					// ���C�t
);

// effect�̐ݒ�
void SetEffect(
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR3 move,	// �ړ�
	D3DXCOLOR col,		// �F
	float fSize, 		// �T�C�Y
	float fDestSize,	// �T�C�Y�ύX
	float fG,			// �d��
	int nLife,			// ���C�t
	int nType,			// �^�C�v
	int nMaxEffect		// �ő吔
);
// �G�t�F�N�g�̏��
EFFECT *GetEffect(void);
#endif
