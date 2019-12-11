// ------------------------------------------------------------------------------------------
//
// �J�������� [camera.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "camera.h"
#include "input.h"
#include "arrangement.h"
#include "mouse.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define CAMERA_MOVE (10.0f)
#define CAMERA_ROT (0.05f)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MoveCamera(void);
void RotCamera(void);
void Update_Result(void);

// ------------------------------------------------------------------------------------------
// �J�����̉�]����
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
CAMERA g_Camera;	// �J�������

// ------------------------------------------------------------------------------------------
// �J�����̏���������
// ------------------------------------------------------------------------------------------
void InitCamera(void)
{
	// �J�����̏�����
	g_Camera.posV = D3DXVECTOR3(0.0f, 400.0f, -300.0f);
	g_Camera.posVDest = g_Camera.posV;
	g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.move = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = 0;
	g_Camera.fY = 400.0f;
	g_Camera.mouse = D3DXVECTOR2(0.0f, 0.0f);
	g_Camera.mouseold = D3DXVECTOR2(0.0f, 0.0f);

	// ���_�ƒ����_�Ƃ̋����v�Z
	D3DXVECTOR3 diffPos = g_Camera.posV - g_Camera.posR;

	// �����v�Z
	g_Camera.fLength = sqrtf(diffPos.x*diffPos.x + diffPos.z * diffPos.z);

}

// ------------------------------------------------------------------------------------------
// �J�����̏I������
// ------------------------------------------------------------------------------------------
void UninitCamera(void)
{
}

// ------------------------------------------------------------------------------------------
// �J�����̍X�V����
// ------------------------------------------------------------------------------------------
void UpdateCamera(void)
{
	// �}�E�X���W����
	g_Camera.mouse = D3DXVECTOR2((float)GetMouseX(), (float)GetMouseY());	// �}�E�X�̍��W

	// ���݂Ɖߋ��̃}�E�X��
	g_Camera.mouseDiff = g_Camera.mouse - g_Camera.mouseold;

	// �J������]
	RotCamera();

	// �J�����ړ�
	MoveCamera();

	// �J�����̒ǐ�
	Update_Result();

	// �ϐ��錾
	g_Camera.mouseold = g_Camera.mouse;	// �}�E�X�̍��W��ۑ�
}

// ------------------------------------------------------------------------------------------
// �J�����̐ݒ菈��
// ------------------------------------------------------------------------------------------
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X //
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(
		&g_Camera.mtxProjection,					// (�e���r���_)
		D3DXToRadian(45.0f),						// ����p(��p)
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,										// MearZ�l(�߂��r���[���ʂ�Z�l)
		20000.0);									// FarZ�l(�����r���[���ʂ�Z�l)

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// �r���[�}�g���b�N�X //
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(
		&g_Camera.mtxView,	// (�J�����}�����_)
		&g_Camera.posV,		// �J�����̎��_
		&g_Camera.posR,		// �J�����̒��ړ_
		&g_Camera.posU);	// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

// ------------------------------------------------------------------------------------------
// �J�����̈ړ�����
// ------------------------------------------------------------------------------------------
void MoveCamera(void)
{
	// �ϐ��錾
	float fAngle;

	// �}�E�X��](���ӁFy�́���-�A����+)
	// �J�[�\���N���b�N
	if (GetMousePress(2))
	{
		// �x�N�g���̊p�x
		fAngle = atan2f(g_Camera.mouseDiff.x, g_Camera.mouseDiff.y);

		// �K��l�ɗ}����
		fAngle = Rot_One_Limit(fAngle);

		// �����Ă���Ƃ�
		if (fAngle != 0)
		{
			g_Camera.posVDest.x += sinf(D3DX_PI - fAngle + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.y += cosf(D3DX_PI - fAngle) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI - fAngle + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.y += cosf(D3DX_PI - fAngle) * CAMERA_MOVE;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}
	}

	//-------------------- x���ړ� --------------------//
	// ��
	if (GetKeyboardPress(DIK_A))
	{
		// ��
		if (GetKeyboardPress(DIK_W))
		{
			g_Camera.posVDest.x += sinf(-D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(-D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		}

		// �O
		else if (GetKeyboardPress(DIK_S))
		{
			g_Camera.posVDest.x += sinf(-D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(-D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}

		else
		{
			g_Camera.posVDest.x += sinf(-D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(-D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}
	}

	// �E
	else if (GetKeyboardPress(DIK_D))
	{
		// ��
		if (GetKeyboardPress(DIK_W))
		{
			g_Camera.posVDest.x += sinf(D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		}

		// �O
		else if (GetKeyboardPress(DIK_S))
		{
			g_Camera.posVDest.x += sinf(D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}

		else
		{
			g_Camera.posVDest.x += sinf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}
	}

	//-------------------- z���ړ� --------------------//
	// ��
	else if (GetKeyboardPress(DIK_W))
	{
		g_Camera.posVDest.x += sinf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posVDest.z += cosf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	}

	// �O
	else if (GetKeyboardPress(DIK_S))
	{
		g_Camera.posVDest.x += sinf(D3DX_PI * 1.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posVDest.z += cosf(D3DX_PI * 1.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	}

	//-------------------- y���ړ� --------------------//
	// ���
	if (GetKeyboardPress(DIK_R))
	{
		g_Camera.posVDest.y += cosf(D3DX_PI * 0.0f) * CAMERA_MOVE;
	}

	// ����
	else if (GetKeyboardPress(DIK_F))
	{
		g_Camera.posVDest.y += cosf(D3DX_PI * 1.0f) * CAMERA_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// �J�����̉�]����
// ------------------------------------------------------------------------------------------
void RotCamera(void)
{
	// �}�E�X��](���ӁFy�́���-�A����+)
	// �E�N���b�N
	if (GetMousePress(1))
	{
		if (g_Camera.mouseDiff.x < 0)
		{
			// ��]
			g_Camera.rot.y -= CAMERA_ROT;
		}
		else if (g_Camera.mouseDiff.x > 0)
		{
			// ��]
			g_Camera.rot.y += CAMERA_ROT;
		}

		if (g_Camera.mouseDiff.y < 0)
		{
			// ��]
			g_Camera.posRDest.y += 10.0f;

		}
		else if (g_Camera.mouseDiff.y > 0)
		{
			// ��]
			g_Camera.posRDest.y -= 10.0f;

		}
		// �K��l�ɗ}����
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// �����_�̈ʒu
		g_Camera.posRDest.x = g_Camera.posV.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posRDest.z = g_Camera.posV.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

	}


	// �K��l�ɗ}����
	g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

	// y�̈ʒu�𑝌�
	if (GetKeyboardSpeacePress(DIK_Q))
	{
		g_Camera.fY += 100.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_Z))
	{
		g_Camera.fY -= 100.0f;
	}
}

// ------------------------------------------------------------------------------------------
// �J�����̍ŏI���ʏ���
// ------------------------------------------------------------------------------------------
void Update_Result(void)
{
	// �ړI�n���猻�ݒn�̋���(�����_)
	g_Camera.posRDiff = g_Camera.posRDest - g_Camera.posR;

	// �����_�ړ�
	g_Camera.posR.x += g_Camera.posRDiff.x * 0.1f;
	g_Camera.posR.y += g_Camera.posRDiff.y * 0.1f;
	g_Camera.posR.z += g_Camera.posRDiff.z * 0.1f;

	// �ړI�n���猻�ݒn�̋���(�����_)
	g_Camera.posVDiff = g_Camera.posVDest - g_Camera.posV;

	// �����_�Ɋ���
	g_Camera.posV.x += g_Camera.posVDiff.x * 0.1f;
	g_Camera.posV.y += g_Camera.posVDiff.y * 0.1f;
	g_Camera.posV.z += g_Camera.posVDiff.z * 0.1f;
}

// ------------------------------------------------------------------------------------------
// �J������񏈗�
// ------------------------------------------------------------------------------------------
CAMERA *GetCamera(void)
{
	return &g_Camera;
}


// ------------------------------------------------------------------------------------------
// �J�����̎��_��񏈗�
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPosV(void)
{
	return g_Camera.posV;
}

// ------------------------------------------------------------------------------------------
// �J�����̒����_��񏈗�
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPosR(void)
{
	return g_Camera.posR;
}
