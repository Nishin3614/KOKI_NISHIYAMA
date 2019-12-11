// ------------------------------------------------------------------------------------------
//
// �R�����g�����̐���[tutorialcomment.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "tutorialcomment.h"
#include "title.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define NAME_SIZE_X (500)		// �^�C�g�����T�C�Y�i���j
#define NAME_SIZE_Y (300)		// �^�C�g�����T�C�Y�i���j

#define ENTER_SIZE_X (300)		// �G���^�[���T�C�Y�i���j
#define ENTER_SIZE_Y (150)		// �G���^�[���T�C�Y�i���j

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureTutorialComment[TUTORIALCOMMENTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialComment = NULL;		// �e�N�X�`���ւ̃|�C���^
TUTORIALCOMMENT g_aTutorialComment[TUTORIALCOMMENTTYPE_MAX];							// �R�����g

// ------------------------------------------------------------------------------------------
//
// �^�C�g������������
//
// ------------------------------------------------------------------------------------------

void InitTutorialComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTutorialComment;

	// �^�C�g���R�����g�t�@�C������`
	char *paTutorialCommentFile[TUTORIALCOMMENTTYPE_MAX] =
	{
		{ "data/TEXTURE/TUTORIALS/.png" },	// �w�i
		{ "data/TEXTURE/TUTORIALS/����.png" },	// �L�[�{�[�h�p�̐���
		{ "data/TEXTURE/TUTORIALS/�W���C�p�b�h����.png" }	// �W���C�p�b�h�p�̐���
	};

	// ������
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		g_aTutorialComment[nCntTutorialComment].Type = TUTORIALCOMMENTTYPE_KEYBOARD;
		g_aTutorialComment[nCntTutorialComment].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTutorialComment[nCntTutorialComment].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aTutorialComment[nCntTutorialComment].fXSize = 0;
		g_aTutorialComment[nCntTutorialComment].fYSize = 0;
		g_aTutorialComment[nCntTutorialComment].bUse = false;

		D3DXCreateTextureFromFile(
			pDevice,
			paTutorialCommentFile[nCntTutorialComment],
			&g_pTextureTutorialComment[nCntTutorialComment]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTORIALCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialComment, 
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTutorialComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2, 
			g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2, 
			g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2,
			g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2,
			g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// �l�ڂ̒��_
		
		// ���_�쐬
		pVtx[0].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;									// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;									// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = g_aTutorialComment[nCntTutorialComment].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aTutorialComment[nCntTutorialComment].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aTutorialComment[nCntTutorialComment].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aTutorialComment[nCntTutorialComment].col;	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffTutorialComment->Unlock();

	// �`���[�g���A���R�����g�ݒ�
	// �L�[�{�[�h�p�R�����g
	if (GetButtenTitle() == false)
	{
		SetTutorialComment(
			TUTORIALCOMMENTTYPE_KEYBOARD,
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	// �W���C�p�b�h�p�R�����g
	else
	{
		SetTutorialComment(
			TUTORIALCOMMENTTYPE_JOYPAD,
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	//// �W�����v�R�����g
	//SetTutorialComment(
	//	TUTORIALCOMMENTTYPE_JUMP,
	//	D3DXVECTOR3(620, 600, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);

	//// �|�[�Y�R�����g
	//SetTutorialComment(TUTORIALCOMMENTTYPE_PAUSE, 
	//	D3DXVECTOR3(1180, 650, 0.0f), 
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	150, 100);

	//// �R���{�R�����g
	//SetTutorialComment(TUTORIALCOMMENTTYPE_FRONT,
	//	D3DXVECTOR3(150, 400, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);

	//// �U���R�����g
	//SetTutorialComment(TUTORIALCOMMENTTYPE_ATTACK,
	//	D3DXVECTOR3(1100, 400, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);

	//// �e�R�����g
	//SetTutorialComment(TUTORIALCOMMENTTYPE_SHOT,
	//	D3DXVECTOR3(1100, 200, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���I������
//
// ------------------------------------------------------------------------------------------

void UninitTutorialComment(void)
{
	int nCntTutorialComment;

	// �e�N�X�`���̊J��
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		if (g_pTextureTutorialComment[nCntTutorialComment] != NULL)
		{
			g_pTextureTutorialComment[nCntTutorialComment]->Release();
			g_pTextureTutorialComment[nCntTutorialComment] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorialComment != NULL)
	{
		g_pVtxBuffTutorialComment->Release();
		g_pVtxBuffTutorialComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateTutorialComment(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTutorialComment;
	static int nCntColor = 0;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTutorialComment->Lock(0, 0, (void **)&pVtx, 0);

	// �J�E���g�`���[�g���A��
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		// �^�C�v���Ƃɕ\��
		switch (g_aTutorialComment[nCntTutorialComment].Type)
		{
			// �ړ��R�����g
		case TUTORIALCOMMENTTYPE_KEYBOARD:

			break;
		}
		// �������[���킹
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffTutorialComment->Unlock();

}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawTutorialComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTutorialComment;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialComment, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �e�N�X�`���̐ݒ�
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		if (g_aTutorialComment[nCntTutorialComment].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTutorialComment[g_aTutorialComment[nCntTutorialComment].Type]);

			// �^�C�g���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorialComment * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���ݒ菈��
//
// ------------------------------------------------------------------------------------------
void SetTutorialComment(TUTORIALCOMMENTTYPE Type, D3DXVECTOR3 pos, D3DXCOLOR col, float fXSize, float fYSize) 
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTutorialComment;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTutorialComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		// �g�p���Ă��Ȃ���
		if (g_aTutorialComment[nCntTutorialComment].bUse == false)
		{
			g_aTutorialComment[nCntTutorialComment].Type = Type;
			g_aTutorialComment[nCntTutorialComment].pos = pos;
			g_aTutorialComment[nCntTutorialComment].col = col;
			g_aTutorialComment[nCntTutorialComment].fXSize = fXSize;
			g_aTutorialComment[nCntTutorialComment].fYSize = fYSize;
			g_aTutorialComment[nCntTutorialComment].bUse = true;

			// �f�[�^�̃|�C���^�����킹��
			pVtx += nCntTutorialComment * 4;

			// �^�C�v���Ƃɕ\��
			switch (g_aTutorialComment[nCntTutorialComment].Type)
			{
				// �ړ��R�����g
			case TUTORIALCOMMENTTYPE_KEYBOARD:

				break;
			}
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// �l�ڂ̒��_

			// �J���[�ݒ�
			pVtx[0].col = g_aTutorialComment[nCntTutorialComment].col;	// ��ڂ̐F�ݒ�
			pVtx[1].col = g_aTutorialComment[nCntTutorialComment].col;	// ��ڂ̐F�ݒ�
			pVtx[2].col = g_aTutorialComment[nCntTutorialComment].col;	// �O�ڂ̐F�ݒ�
			pVtx[3].col = g_aTutorialComment[nCntTutorialComment].col;	// �l�ڂ̐F�ݒ�

			// for���𔲂���
			break;
		}
	}
	// �A�����b�N
	g_pVtxBuffTutorialComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���擾����
//
// ------------------------------------------------------------------------------------------

TUTORIALCOMMENT *GetTutorialComment(void)
{
	return &g_aTutorialComment[0];
}