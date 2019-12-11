// ------------------------------------------------------------------------------------------
//
// �J�������� [camera.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "camera.h"
#include "input.h"
#include "joypad.h"
#include "player.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_Title_MoveCamera(void);
void Update_Tutorial_MoveCamera(void);
void Update_Game_MoveCamera(void);
void Update_Game_Compliance(PLAYER *pPlayer);
void Update_Game_ViewMove(PLAYER *pPlayer);
void Update_Clear_MoveCamera(void);
void Update_GameOver_MoveCamera(void);
void Update_Rank_MoveCamera(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
CAMERA g_Camera;	// �J�������

// ------------------------------------------------------------------------------------------
// �J�����̏���������
// ------------------------------------------------------------------------------------------
void InitCamera(void)
{
	// �^�C�g����
	if (GetMode() == MODE_TITLE)
	{
		// �J�����̏�����
		g_Camera.posV = D3DXVECTOR3(30.0f, 50.0f, 500.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// �`���[�g���A����
	if (GetMode() == MODE_TUTORIAL)
	{
		// �J�����̏�����
		g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, 300.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// �Q�[����
	else if (GetMode() == MODE_GAME)
	{
		// �J�����̏�����
		g_Camera.posV = D3DXVECTOR3(0.0f, 75.0f, 750.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// �N���A��
	else if (GetMode() == MODE_CLEAR)
	{
		// �J�����̏�����
		g_Camera.posV = D3DXVECTOR3(-12.0f, 75.0f, 150.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// �Q�[���I�[�o�[��
	else if (GetMode() == MODE_GAMEOVER)
	{
		// �J�����̏�����
		g_Camera.posV = D3DXVECTOR3(-12.0f, 75.0f, 150.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// �����L���O��
	else if (
		GetMode() == MODE_RANK ||
		GetMode() == MODE_STATI)
	{
		// �J�����̏�����
		g_Camera.posV = D3DXVECTOR3(-12.0f, 75.0f, 150.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

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
	PLAYER * pPlayer = GetPlayer(); // �v���C���[���擾

	// �^�C�g����
	if (GetMode() == MODE_TITLE)
	{
		// �J�����ړ�
		Update_Title_MoveCamera();
	}

	// �`���[�g���A����
	else if (GetMode() == MODE_TUTORIAL)
	{
		// �J�����ړ�
		Update_Game_MoveCamera();

		// ���_�E�����_�̈ړ�
		Update_Game_ViewMove(pPlayer);
	}

	// �Q�[����
	else if (GetMode() == MODE_GAME)
	{
		// �J�����ړ�
		Update_Game_MoveCamera();

		// ���_�E�����_�̈ړ�
		Update_Game_ViewMove(pPlayer);
	}

	// �N���A��
	else if (GetMode() == MODE_CLEAR)
	{
		// �J�����ړ�
		Update_Clear_MoveCamera();
	}

	// �Q�[���I�[�o�[��
	else if (GetMode() == MODE_GAMEOVER)
	{
		// �J�����ړ�
		Update_GameOver_MoveCamera();
	}

	// �����L���O��
	else if (GetMode() == MODE_RANK ||
		GetMode() == MODE_STATI)
	{
		// �J�����ړ�
		Update_Rank_MoveCamera();
	}
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
		20000.0f);									// FarZ�l(�����r���[���ʂ�Z�l)�����l��1000

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
// �J�����̈ʒu��񏈗�
// ------------------------------------------------------------------------------------------
void CameraPosition(void)
{
	// �{�X
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		g_Camera.posV = D3DXVECTOR3(0.0f, 20.0f, 30.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

// ------------------------------------------------------------------------------------------
// �^�C�g���̃J�����ړ�����
// ------------------------------------------------------------------------------------------
void Update_Title_MoveCamera(void)
{
	// �J�����ړ�
	g_Camera.posR.z -= g_Camera.move.z;

	//// �J�����̎��_��]
	//g_Camera.rot.y -= 0.01f;

	// ���_�X�V
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// �`���[�g���A���̃J�����ړ�����
// ------------------------------------------------------------------------------------------
void Update_Tutorial_MoveCamera(void)
{

}

// ------------------------------------------------------------------------------------------
// �Q�[���̃J�����ړ�����
// ------------------------------------------------------------------------------------------
void Update_Game_MoveCamera(void)
{
	// �����_���ɉ�] //
	// �����v���
	if (GetKeyboardPress(DIK_L))
	{
		g_Camera.rot.y += 0.03f;

		// -3.14�`3.14�ɉ�]�������Œ肳����
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// ���_�X�V
		g_Camera.posVDest.x = g_Camera.posRDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posVDest.z = g_Camera.posRDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		// �J�E���g�^�C��������
		g_Camera.nCntTime = 0;

		// ���ʂ�������off��
		g_Camera.bfront = false;
	}

	// ���v���
	else if (GetKeyboardPress(DIK_K))
	{
		g_Camera.rot.y -= 0.03f;

		// -3.14�`3.14�ɉ�]�������Œ肳����
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// ���_�X�V
		g_Camera.posVDest.x = g_Camera.posRDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posVDest.z = g_Camera.posRDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	
		// �J�E���g�^�C��������
		g_Camera.nCntTime = 0;

		// ���ʂ�������off��
		g_Camera.bfront = false;
	}

	// �W���C�p�b�h�p //
	// �p�b�h�p //
	//GetGamepadStickLeft
	int nValueH, nValueV;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fRot;			// �ړ����x

	// �Q�[���p�b�h�̃X�e�B�b�N�����擾
	GetGamepadStickRight(0, &nValueH, &nValueV);

	// ================================
	// �v���C���[�ړ�
	// ================================
	// �Q�[���p�b�h�ړ�
	if (nValueH != 0 || nValueV != 0)
	{
		// ���x�̌v�Z
		if (nValueH < 0)
		{
			fRot = (abs(nValueH) * 0.8f) / 32768.0f;
			g_Camera.rot.y -= fRot;

		}
		// ���x�̌v�Z
		else if (nValueH > 0)
		{
			fRot = (abs(nValueH) * 0.8f) / 32768.0f;
			g_Camera.rot.y += fRot;

		}

		// -3.14�`3.14�ɉ�]�������Œ肳����
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// ���_�X�V
		g_Camera.posVDest.x = g_Camera.posRDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posVDest.z = g_Camera.posRDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		// �J�E���g�^�C��������
		g_Camera.nCntTime = 0;

		// ���ʂ�������off��
		g_Camera.bfront = false;
	}
}

// ------------------------------------------------------------------------------------------
// �Q�[���̒Ǐ]����
// ------------------------------------------------------------------------------------------
void Update_Game_Compliance(PLAYER *pPlayer)
{
	// �J�E���g�^�C���A�b�v
	g_Camera.nCntTime++;

	// ���ʂ�����
	if (g_Camera.bfront == true)
	{
		//-------------------- ���_�ړ� --------------------//
		// �Q�[���̃J�����̉�]�̖ړI�n
		g_Camera.rotDest.y = pPlayer->rot.y;

		// ��]��������
		g_Camera.rotBetween.y = (g_Camera.rotDest.y - g_Camera.rot.y);

		// -3.14�`3.14�ɉ�]�������Œ肳����
		g_Camera.rotBetween.y = Rot_One_Limit(g_Camera.rotBetween.y);

		// �Q�[���̃J�����̉�]�̊���
		g_Camera.rot.y += g_Camera.rotBetween.y * 0.1f;

		// -3.14�`3.14�ɉ�]�������Œ肳����
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);
	}

	//-------------------- �����_�ړ� --------------------//
	// �v���C���[�������Ă���Ƃ�
	if (pPlayer->move.x <= -0.1 || pPlayer->move.x >= 0.1f ||
		pPlayer->move.y <= -0.1 || pPlayer->move.y >= 0.1f ||
		pPlayer->move.z <= -0.1 || pPlayer->move.z >= 0.1f)
	{
		// �J�E���g�^�C��������
		g_Camera.nCntTime = 0;

		// ���ʂ�������off��
		g_Camera.bfront = false;
	}

	// �v���C���[���~�܂���1�b�o�߂�����
	if (g_Camera.nCntTime >= 30)
	{
		g_Camera.bfront = true;
	}

	// -3.14�`3.14�ɉ�]�������Œ肳����
	g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);
}

// ------------------------------------------------------------------------------------------
// ���_�E�����_�̈ړ�����
// ------------------------------------------------------------------------------------------
void Update_Game_ViewMove(PLAYER *pPlayer)
{
	//-------------------- �����_�ړ� --------------------//
	// �ړI�̒����_
	g_Camera.posRDest.x = pPlayer->pos.x + sinf(g_Camera.rot.y) * 10.0f;
	g_Camera.posRDest.y = pPlayer->pos.y;
	g_Camera.posRDest.z = pPlayer->pos.z + cosf(g_Camera.rot.y) * 10.0f;

	// �ړI�n���猻�ݒn�̋���(�����_)
	g_Camera.posRDiff = g_Camera.posRDest - g_Camera.posR;

	// �����_�ړ�
	g_Camera.posR.x += g_Camera.posRDiff.x * 0.9f;
	g_Camera.posR.y += g_Camera.posRDiff.y * 0.9f;
	g_Camera.posR.z += g_Camera.posRDiff.z * 0.9f;

	//-------------------- ���_�ړ� --------------------//
	// �ړI�̎��_
	g_Camera.posVDest.x =
		g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posVDest.y =
		g_Camera.posR.y + 75.0f;
	g_Camera.posVDest.z =
		g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

	// �ړI�n���猻�ݒn�̋���(�����_)
	g_Camera.posVDiff =
		g_Camera.posVDest - g_Camera.posV;

	// �����_�Ɋ���
	g_Camera.posV.x += g_Camera.posVDiff.x * 0.1f;
	g_Camera.posV.y += g_Camera.posVDiff.y * 0.1f;
	g_Camera.posV.z += g_Camera.posVDiff.z * 0.1f;
}

// ------------------------------------------------------------------------------------------
// �N���A�̃J�����ړ�����
// ------------------------------------------------------------------------------------------
void Update_Clear_MoveCamera(void)
{
	// �J�����̎��_��]
	g_Camera.rot.y -= 0.01f;

	// ���_�X�V
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// �Q�[���I�[�o�[�̃J�����ړ�����
// ------------------------------------------------------------------------------------------
void Update_GameOver_MoveCamera(void)
{
	// �J�����̎��_��]
	g_Camera.rot.y -= 0.01f;

	// ���_�X�V
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// �����L���O�̃J�����ړ�����
// ------------------------------------------------------------------------------------------
void Update_Rank_MoveCamera(void)
{
	// �J�����̎��_��]
	g_Camera.rot.y -= 0.01f;

	// ���_�X�V
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// �J������񏈗�
// ------------------------------------------------------------------------------------------
CAMERA *GetCamera(void)
{
	return &g_Camera;
}