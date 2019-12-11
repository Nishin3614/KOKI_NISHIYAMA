// ------------------------------------------------------------------------------------------
//
// �R�����g�����̐���[titlecomment.cpp]
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
#include "titlecomment.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureTitleComment[TITLECOMMENTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleComment = NULL;		// �e�N�X�`���ւ̃|�C���^
TITLECOMMENT g_aTitleComment[TITLECOMMENTTYPE_MAX];							// �R�����g

// ------------------------------------------------------------------------------------------
//
// �^�C�g������������
//
// ------------------------------------------------------------------------------------------

void InitTitleComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTitleComment;

	// �^�C�g���R�����g�t�@�C������`
	char *paTitleCommentFile[TITLECOMMENTTYPE_MAX] =
	{
		{ "data/TEXTURE/TITLE/Title_Log.png" },		// �^�C�g����
		{ "data/TEXTURE/TITLE/00_rion_shadow.png" },	// ���C�I��
	};

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TITLECOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffTitleComment,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTitleComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		// �^�C�g����
		if (nCntTitleComment == TITLECOMMENTTYPE_NAME)
		{
			g_aTitleComment[nCntTitleComment].pos = D3DXVECTOR3(640.0f, -350.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].size = D3DXVECTOR2(350.0f, 350.0f);
			g_aTitleComment[nCntTitleComment].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���C�I��
		else if (nCntTitleComment == TITLECOMMENTTYPE_RION)
		{
			g_aTitleComment[nCntTitleComment].pos = D3DXVECTOR3(1000.0f, 200.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].size = D3DXVECTOR2(0.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].tex = D3DXVECTOR2(0.0f, 0.0f);
		}

		g_aTitleComment[nCntTitleComment].bUse = true;

		// �ړI�n�ɓ������Ă��邩�ǂ���
		g_aTitleComment[nCntTitleComment].bPut = false;

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			paTitleCommentFile[nCntTitleComment],
			&g_pTextureTitleComment[nCntTitleComment]);

		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// �l�ڂ̒��_
		
		// ���_�쐬
		pVtx[0].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;									// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;									// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(
			0.0f, 
			0.0f);				// ��ڂ̃e�N�X�`��

		pVtx[1].tex = D3DXVECTOR2(
			g_aTitleComment[nCntTitleComment].tex.x,
			0.0f);				// ��ڂ̃e�N�X�`��

		pVtx[2].tex = D3DXVECTOR2(
			0.0f, 
			g_aTitleComment[nCntTitleComment].tex.y);				// �O�ڂ̃e�N�X�`��

		pVtx[3].tex = D3DXVECTOR2(
			g_aTitleComment[nCntTitleComment].tex.x,
			g_aTitleComment[nCntTitleComment].tex.y);				// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffTitleComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���I������
//
// ------------------------------------------------------------------------------------------

void UninitTitleComment(void)
{
	int nCntTitleComment;

	// �e�N�X�`���̊J��
	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		if (g_pTextureTitleComment[nCntTitleComment] != NULL)
		{
			g_pTextureTitleComment[nCntTitleComment]->Release();
			g_pTextureTitleComment[nCntTitleComment] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitleComment != NULL)
	{
		g_pVtxBuffTitleComment->Release();
		g_pVtxBuffTitleComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateTitleComment(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTitleComment;
	static int nCntColor = 0;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTitleComment->Lock(0, 0, (void **)&pVtx, 0);

	// �J�E���g�^�C�g���R�����g
	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		// �ړI�n�ɂ��Ă��邩�ǂ���
		if (g_aTitleComment[nCntTitleComment].bPut == false)
		{
			// �^�C�g����
			if (nCntTitleComment == TITLECOMMENTTYPE_NAME)
			{
				g_aTitleComment[nCntTitleComment].pos.y += g_aTitleComment[nCntTitleComment].move.y;

				if (g_aTitleComment[nCntTitleComment].pos.y >= TITLECOMENT_ON_Y)
				{
					g_aTitleComment[nCntTitleComment].pos.y = TITLECOMENT_ON_Y;

					// �ړI�n�ɓ������Ă��邩�ǂ���
					g_aTitleComment[nCntTitleComment].bPut = true;
				}

				if (GetKeyboardTrigger(DIK_RETURN) ||
					GetKeyboardTrigger(DIK_J) ||
					GetJoyboardTrigger(0, JOYPADKEY_B))
				{
					g_aTitleComment[nCntTitleComment].pos.y = TITLECOMENT_ON_Y;

					// �ړI�n�ɓ������Ă��邩�ǂ���
					g_aTitleComment[nCntTitleComment].bPut = true;
				}

				// ���_�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// ��ڂ̒��_
				pVtx[1].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// ��ڂ̒��_
				pVtx[2].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// �O�ڂ̒��_
				pVtx[3].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// �l�ڂ̒��_
			}

			// ���C�I��
			if (nCntTitleComment == TITLECOMMENTTYPE_RION)
			{
				// �^�C�g�������ړI�n�ɓ��B������
				if (g_aTitleComment[TITLECOMMENTTYPE_NAME].bPut == true)
				{
					// �e�N�X�`�������X�ɏo������
					g_aTitleComment[nCntTitleComment].tex.x += 0.01f;
					g_aTitleComment[nCntTitleComment].tex.y += 0.01f;

					// �T�C�Y�����X�ɑ傫���Ȃ�
					g_aTitleComment[nCntTitleComment].size.x += 1.0f;
					g_aTitleComment[nCntTitleComment].size.y += 1.0f;


					// �e�N�X�`�����K��l��
					if (g_aTitleComment[nCntTitleComment].tex.y >= 1.0f ||
						g_aTitleComment[nCntTitleComment].tex.x >= 1.0f)
					{
						g_aTitleComment[nCntTitleComment].tex.x = 1.0f;

						g_aTitleComment[nCntTitleComment].tex.y = 1.0f;

						// �ړI�̃e�N�X�`���ɂȂ�����
						g_aTitleComment[nCntTitleComment].bPut = true;
					}

					// ���_�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// ��ڂ̒��_
					pVtx[1].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// ��ڂ̒��_
					pVtx[2].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// �O�ڂ̒��_
					pVtx[3].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// �l�ڂ̒��_

					// �e�N�X�`���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(
						0.0f,
						0.0f);				// ��ڂ̃e�N�X�`��

					pVtx[1].tex = D3DXVECTOR2(
						g_aTitleComment[nCntTitleComment].tex.x,
						0.0f);				// ��ڂ̃e�N�X�`��

					pVtx[2].tex = D3DXVECTOR2(
						0.0f,
						g_aTitleComment[nCntTitleComment].tex.y);				// �O�ڂ̃e�N�X�`��

					pVtx[3].tex = D3DXVECTOR2(
						g_aTitleComment[nCntTitleComment].tex.x,
						g_aTitleComment[nCntTitleComment].tex.y);				// �l�ڂ̃e�N�X�`��
				}
			}
		}
		// �|�C���g���킹
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffTitleComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawTitleComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTitleComment;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTitleComment, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �e�N�X�`���̐ݒ�
	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		if (g_aTitleComment[nCntTitleComment].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTitleComment[nCntTitleComment]);

			// �^�C�g���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitleComment * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���擾����
//
// ------------------------------------------------------------------------------------------

TITLECOMMENT *GetTitleComment(int nCntTitleComment)
{
	return &g_aTitleComment[nCntTitleComment];
}