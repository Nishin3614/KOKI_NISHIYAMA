// ------------------------------------------------------------------------------------------
//
// �I�u�̑I������ [seleob.h]
// Author : koki_nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _SELEOB_H_
#define _SELEOB_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �񋓌^
//
// ------------------------------------------------------------------------------------------
// �I�����
typedef enum
{
	SELEOBTYPE_NONE = 0,	// �Ȃ�
	SELEOBTYPE_FLOOR,		// ��
	SELEOBTYPE_MATERIALS,	// �f��
	SELEOBTYPE_GIRAFFE,		// �L����
	SELEOBTYPE_BUFFALO,		// �o�b�t�@���[
	SELEOBTYPE_MEERKAT,		// �~�[�A�L���b�g
	SELEOBTYPE_CROCODILE,	// �N���R�_�C��
	SELEOBTYPE_WALL,		// ��
	SELEOBTYPE_BILLBOARD,	// �r���{�[�h
	SELEOBTYPE_MAX,
} SELEOBTYPE;

// ����
typedef enum
{
	SELEOBDICTION_X = 0,	// X
	SELEOBDICTION_Y,		// Y
	SELEOBDICTION_Z,		// Z
	SELEOBDICTION_MAX
} SELEOBDIRECTION;
// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 Worldpos;
	D3DXVECTOR3 Screenpos;
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldSeleOb;
	SELEOBTYPE	type;
	int			nNumber;
	bool		bUse;
} SELEOB;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void InitSeleob(void);
void UninitSeleob(void);
void UpdateSeleob(void);
void DrawSeleob(void);

#endif
