// ------------------------------------------------------------------------------------------
//
// �z�u���� [arrangement.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "arrangement.h"
#include "input.h"
#include "camera.h"
#include "mouse.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define ARRANGEMENT_MOVE (2.0f)				// �ړ���
#define ARRANGEMENT_MOVE_Y (50)				// y�̈ړ���
#define ARRANGEMENT_ROUND (100)				// �l�̌ܓ��͈�
#define ARRANGEMENT_ROUND_Y (50)			// y�̒l�l�̌ܓ��͈�
#define ARRANGEMENT_ROT_ROUND (D3DX_PI / 2)	// �p�x�̎l�̌ܓ��͈�

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_RotArrangement(void);
void Update_MoveArrangement(void);
void Update_RoundOffArrangement(void);

#ifdef _DEBUG
void Debug_Arrangement(void);
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
ARRANGEMENT				g_arrangement;				// �z�u���

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitArrangement(void)
{
	// �z�u�̏����ݒ�
	g_arrangement.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_arrangement.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitArrangement(void)
{

}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateArrangement(void)
{
	// ��]
	Update_RotArrangement();
	// �ړ�
	Update_MoveArrangement();
	// �l�̌ܓ�
	Update_RoundOffArrangement();
}

// ------------------------------------------------------------------------------------------
// ��]����
// ------------------------------------------------------------------------------------------
void Update_RotArrangement(void)
{
	CAMERA		*pCamera = GetCamera();						// �J�������擾
	float MouseposX = (float)GetMouseX();					// �}�E�X��x���W���擾
	float MouseposZ = (float)GetMouseY();					// �}�E�X��y���W���擾
	D3DXVECTOR3 WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[���h���W

	g_arrangement.rot = pCamera->rot;
	// �}�E�X�̓_���X�N���[�����W���烏�[���h���W��(�I�u�W�F�N�g�̑��݂��镽��)
	CalcScreenToXZ(
		&g_arrangement.pos,
		MouseposX,
		MouseposZ,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		&pCamera->mtxView,
		&pCamera->mtxProjection,
		D3DXVECTOR3(0.0f, g_arrangement.pos.y, 0.0f)	// ���ʂ����߂�(Y���W��ς���)
	);
}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void Update_MoveArrangement(void)
{
	CAMERA *pCamera = GetCamera();	// �J�������擾

	if (GetKeyboardPress(DIK_A))
	{
		g_arrangement.pos.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_D))
	{
		g_arrangement.pos.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_W))
	{
		g_arrangement.pos.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_S))
	{
		g_arrangement.pos.x += sinf(D3DX_PI * 1.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(D3DX_PI * 1.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}
	
	if (GetKeyboardPress(DIK_E))
	{
		g_arrangement.pos.y += ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_C))
	{
		g_arrangement.pos.y -= ARRANGEMENT_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// �l�̌ܓ�
// ------------------------------------------------------------------------------------------
void Update_RoundOffArrangement(void)
{
	// �ʒu //
	// x
	g_arrangement.pos.x /= ARRANGEMENT_ROUND;
	g_arrangement.pos.x = roundf(g_arrangement.pos.x);
	g_arrangement.pos.x *= ARRANGEMENT_ROUND;
	// y
	g_arrangement.pos.y /= ARRANGEMENT_ROUND_Y;
	g_arrangement.pos.y = roundf(g_arrangement.pos.y);
	g_arrangement.pos.y *= ARRANGEMENT_ROUND_Y;
	// z
	g_arrangement.pos.z /= ARRANGEMENT_ROUND;
	g_arrangement.pos.z = roundf(g_arrangement.pos.z);
	g_arrangement.pos.z *= ARRANGEMENT_ROUND;
	// �p�x //
	// x
	g_arrangement.rot.x /= ARRANGEMENT_ROT_ROUND;
	g_arrangement.rot.x = roundf(g_arrangement.rot.x);
	g_arrangement.rot.x *= ARRANGEMENT_ROT_ROUND;
	// y
	g_arrangement.rot.y /= ARRANGEMENT_ROT_ROUND;
	g_arrangement.rot.y = roundf(g_arrangement.rot.y);
	g_arrangement.rot.y *= ARRANGEMENT_ROT_ROUND;
	// z
	g_arrangement.rot.z /= ARRANGEMENT_ROT_ROUND;
	g_arrangement.rot.z = roundf(g_arrangement.rot.z);
	g_arrangement.rot.z *= ARRANGEMENT_ROT_ROUND;
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// �f�o�b�O�\������
// ------------------------------------------------------------------------------------------
void Debug_Arrangement(void)
{
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawArrangement(void)
{

}

// ------------------------------------------------------------------------------------------
// �z�u��񏈗�
// ------------------------------------------------------------------------------------------
ARRANGEMENT *GetArrangement(void)
{
	return &g_arrangement;
}