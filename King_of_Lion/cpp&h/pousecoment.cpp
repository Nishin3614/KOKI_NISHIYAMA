// ------------------------------------------------------------------------------------------
//
// �|�[�Y�R�����g�����̐���[pousecoment.cpp]
// Author : Koki Nishiyama 
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "title.h"

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "pousecoment.h"

#include "arrow.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define POUSEUI_SIZE_X (300)					// �|�[�Y�R�����g���T�C�Y�i���j
#define POUSEUI_SIZE_Y (300)					// �|�[�Y�R�����g���T�C�Y�i���j

#define POUSECOMMENTUI_SIZE_X (500)					// �|�[�Y�R�����g���T�C�Y�i���j
#define POUSECOMMENTUI_SIZE_Y (500)					// �|�[�Y�R�����g���T�C�Y�i���j
#define UI_SIZE_X (200)							// �G���^�[���T�C�Y�i���j
#define UI_SIZE_Y (200)							// �G���^�[���T�C�Y�i���j

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_PousePouseComment(void);
void Update_ColChangePouseComment(void);
void Update_VertexPouseComment(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePouseComent[POUSECOMMENT_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPouseComent = NULL;			// �e�N�X�`���ւ̃|�C���^
POUSECOMENT g_aPouseComent[POUSECOMMENT_MAX];					// �|�[�Y�R�����g
int g_nCntPcoment = 0;

// ------------------------------------------------------------------------------------------
// �|�[�Y�R�����g����������
// ------------------------------------------------------------------------------------------
void InitPouseComent(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	VERTEX_2D *pVtx;			// ���_���̔Ԓn
	int nCntPouseComent;

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[POUSECOMMENT_MAX] =
	{
		{ "data/TEXTURE/POUSE/00_pousecomment.png" },
		{ "data/TEXTURE/POUSE/01_GoGame.png" },
		{ "data/TEXTURE/POUSE/02_GoTitle.png" },
		{ "data/TEXTURE/POUSE/03_ReGame.png" }
	};

	// ������
	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
		// �|�[�Y�R�����g�̈ʒu
		if (nCntPouseComent == POUSECOMMENT_POUSE)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f);
		}

		// �Q�[���R�����g�̈ʒu
		else if(nCntPouseComent == POUSECOMMENT_GAME)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, 500.0f, 0.0f);
		}
		// �^�C�g���R�����g�̈ʒu
		else if (nCntPouseComent == POUSECOMMENT_TITLE)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f);
		}

		// ��蒼���R�����g�̈ʒu
		else if (nCntPouseComent == POUSECOMMENT_RESTART)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 500.0f, 0.0f);
		}

		g_aPouseComent[nCntPouseComent].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPouseComent[nCntPouseComent].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPouseComent[nCntPouseComent].nType = nCntPouseComent;
		g_aPouseComent[nCntPouseComent].nCntCol = 0;
		g_aPouseComent[nCntPouseComent].bUse = false;

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntPouseComent],
			&g_pTexturePouseComent[nCntPouseComent]);
	}

	// �|�[�Y�R�����g�J�E���g�̏�����
	g_nCntPcoment = POUSECOMMENT_GAME;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POUSECOMMENT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffPouseComent,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffPouseComent->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
		if (nCntPouseComent == POUSECOMMENT_POUSE)
		{
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[1].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[2].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// �O�ڂ̒��_

			pVtx[3].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// �l�ڂ̒��_
		}
		
		else
		{
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[1].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// ��ڂ̒��_

			pVtx[2].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// �O�ڂ̒��_

			pVtx[3].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// �l�ڂ̒��_
		}

		// ���_�쐬
		pVtx[0].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;									// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;									// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = g_aPouseComent[nCntPouseComent].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aPouseComent[nCntPouseComent].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aPouseComent[nCntPouseComent].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aPouseComent[nCntPouseComent].col;	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffPouseComent->Unlock();
}

// ------------------------------------------------------------------------------------------
// �|�[�Y�R�����g�I������
// ------------------------------------------------------------------------------------------
void UninitPouseComent(void)
{
	int nCntPouseComent;

	// �e�N�X�`���̊J��
	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
		if (g_pTexturePouseComent[nCntPouseComent] != NULL)
		{
			g_pTexturePouseComent[nCntPouseComent]->Release();
			g_pTexturePouseComent[nCntPouseComent] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPouseComent != NULL)
	{
		g_pVtxBuffPouseComent->Release();
		g_pVtxBuffPouseComent = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �|�[�Y�R�����g�X�V����
// ------------------------------------------------------------------------------------------

void UpdatePouseComent(void)
{
	// �I������
	Update_PousePouseComment();

	// �J���[�̕ω�����
	Update_ColChangePouseComment();

	// ���_��񏈗�
	Update_VertexPouseComment();
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void Update_PousePouseComment(void)
{
	// �X�e�B�b�N�ł̑���
	static int nCntStickLeft = 0;
	static int nCntStickRight = 0;

	// �W���C�p�b�h�p //
	int nValueH, nValueV;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p

	// �Q�[���p�b�h�̃X�e�B�b�N�����擾
	GetGamepadStickLeft(0, &nValueH, &nValueV);

	// �Q�[���p�b�h�ړ�
	if (nValueH != 0 || nValueV != 0)
	{
		// ���x�̌v�Z
		if (nValueH < 0)
		{
			nCntStickLeft++;
		}
		// ���x�̌v�Z
		else if (nValueH > 0)
		{
			nCntStickRight++;
		}
	}
	else
	{
		nCntStickLeft = 0;
		nCntStickRight = 0;
	}

	// �J�E���g����
	if (GetKeyboardTrigger(DIK_D) ||
		GetJoyboardTrigger(0, JOYPADKEY_RIGHT) ||
		nCntStickRight == 1)
	{
		// ���艹
		PlaySound(SOUND_LABEL_SE_ENTER);

		// �O��̃�����ʏ��
		g_aPouseComent[g_nCntPcoment].col.a = 1.0f;

		// �O��̃J���[�J�E���g��ʏ��
		g_aPouseComent[g_nCntPcoment].nCntCol = 0;

		// ���_��񏈗�
		Update_VertexPouseComment();

		// �J�E���g�A�b�v
		g_nCntPcoment++;

		// �X�e�B�b�N�̃J�E���g������
		nCntStickLeft = 0;
		nCntStickRight = 0;

		// �J�E���g�����E�_�𒴂�����
		if (g_nCntPcoment >= POUSECOMMENT_MAX)
		{
			g_nCntPcoment = POUSECOMMENT_GAME;
		}
	}
	else if (GetKeyboardTrigger(DIK_A) ||
		GetJoyboardTrigger(0, JOYPADKEY_LEFT) ||
		nCntStickLeft == 1)
	{
		// ���艹
		PlaySound(SOUND_LABEL_SE_ENTER);

		// �O��̃�����ʏ��
		g_aPouseComent[g_nCntPcoment].col.a = 1.0f;

		// �O��̃J���[�J�E���g��ʏ��
		g_aPouseComent[g_nCntPcoment].nCntCol = 0;

		// ���_��񏈗�
		Update_VertexPouseComment();

		// �J�E���g�_�E��
		g_nCntPcoment--;

		// �J�E���g�����E�_�����������
		if (g_nCntPcoment < POUSECOMMENT_GAME)
		{
			g_nCntPcoment = POUSECOMMENT_MAX - 1;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetKeyboardTrigger(DIK_J) ||
		GetJoyboardTrigger(0, JOYPADKEY_B) ||
		GetJoyboardTrigger(0, JOYPADKEY_A))
	{
		// �Q�[���I����
		if (g_nCntPcoment == POUSECOMMENT_GAME)
		{
			// ���艹
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetGameState(GAMESTATE_NORMAL);
		}

		// �^�C�g���I����
		else if (g_nCntPcoment == POUSECOMMENT_TITLE)
		{
			// ���艹
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetGameState(GAMESTATE_TITLE);
		}

		// ��蒼���I����
		else if (g_nCntPcoment == POUSECOMMENT_RESTART)
		{
			// ���艹
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetFade(MODE_GAME);
		}
	}

	// ���̈ʒu���X�V
	ArrowPosition(g_aPouseComent[g_nCntPcoment].pos);
}

// ------------------------------------------------------------------------------------------
// �J���[�ω�����
// ------------------------------------------------------------------------------------------
void Update_ColChangePouseComment(void)
{
	// �J�E���g�A�b�v
	g_aPouseComent[g_nCntPcoment].nCntCol++;

	if (g_aPouseComent[g_nCntPcoment].nCntCol % 20 == 0)
	{
		// �J���[�ݒ�
		g_aPouseComent[g_nCntPcoment].col.a = 1.0f;	// ��ڂ̐F�ݒ�
	}

	else if (g_aPouseComent[g_nCntPcoment].nCntCol % 10 == 0)
	{
		// �J���[�ݒ�
		g_aPouseComent[g_nCntPcoment].col.a = 0.0f;	// ��ڂ̐F�ݒ�
	}
}

// ------------------------------------------------------------------------------------------
// ���_��񏈗�
// ------------------------------------------------------------------------------------------
void Update_VertexPouseComment(void)
{
	VERTEX_2D *pVtx;			// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffPouseComent->Lock(0, 0, (void **)&pVtx, 0);

	// �|�C���g���킹
	pVtx += 4 * g_nCntPcoment;

	// �J���[�ݒ�
	pVtx[0].col = g_aPouseComent[g_nCntPcoment].col;	// ��ڂ̐F�ݒ�
	pVtx[1].col = g_aPouseComent[g_nCntPcoment].col;	// ��ڂ̐F�ݒ�
	pVtx[2].col = g_aPouseComent[g_nCntPcoment].col;	// �O�ڂ̐F�ݒ�
	pVtx[3].col = g_aPouseComent[g_nCntPcoment].col;	// �l�ڂ̐F�ݒ�

	// �A�����b�N
	g_pVtxBuffPouseComent->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �|�[�Y�R�����g�`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawPouseComent(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPouseComent;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPouseComent, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �e�N�X�`���̐ݒ�
	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
			pDevice->SetTexture(0, g_pTexturePouseComent[nCntPouseComent]);

			// �|�[�Y�R�����g�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPouseComent * 4, 2);
	}
}

// ------------------------------------------------------------------------------------------
// �|�[�Y�R�����g�擾����
// ------------------------------------------------------------------------------------------
int GetPouseCommentPouse(void)
{
	return g_nCntPcoment;
}

// ------------------------------------------------------------------------------------------
//
// �|�[�Y�R�����g�擾����
//
// ------------------------------------------------------------------------------------------
POUSECOMENT *GetPouseComent(void)
{
	return &g_aPouseComent[0];
}