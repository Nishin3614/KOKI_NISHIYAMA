// ------------------------------------------------------------------------------------------
//
// UI�����̐���[ui.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "UI.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

#define TIMERWAKU "data/TEXTURE/timerwaku.png"		// �^�C���̘g
#define TIMENAME "data/TEXTURE/�^�C����.png"		// �^�C����
#define FRAME_SCORE "data/TEXTURE/Frame_Score2.png"	// �X�R�A�g
#define SCORECAT1 "data/TEXTURE/�X�R�A�E.png"		// �X�R�A�E
#define SCORECAT2 "data/TEXTURE/�X�R�A��.png"		// �X�R�A��
#define HPNAME "data/TEXTURE/HP��.png"				// HP��

#define UI_TIME_SIZE_X (80)		// �^�C���T�C�Y�i���j
#define UI_TIME_SIZE_Y (50)		// �^�C���T�C�Y�i���j

#define UI_SCORE_SIZE_X (150)	// �X�R�A�T�C�Y�i���j
#define UI_SCORE_SIZE_Y (50)	// �X�R�A�T�C�Y�i���j

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureUi[UITYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;	// �e�N�X�`���ւ̃|�C���^
UI g_aUi[UITYPE_MAX];								// UI

// ------------------------------------------------------------------------------------------
//
// �^�C�g������������
//
// ------------------------------------------------------------------------------------------

void InitUi(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntUi;

	// ������
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		g_aUi[nCntUi].Type = UITYPE_TIMEFRAME;
		g_aUi[nCntUi].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aUi[nCntUi].fXSize = 0;
		g_aUi[nCntUi].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, TIMERWAKU, &g_pTextureUi[0]);
	D3DXCreateTextureFromFile(pDevice, TIMENAME, &g_pTextureUi[1]);
	D3DXCreateTextureFromFile(pDevice, FRAME_SCORE, &g_pTextureUi[2]);
	D3DXCreateTextureFromFile(pDevice, SCORECAT1, &g_pTextureUi[3]);
	D3DXCreateTextureFromFile(pDevice, SCORECAT2, &g_pTextureUi[4]);
	D3DXCreateTextureFromFile(pDevice, HPNAME, &g_pTextureUi[5]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffUi, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y - UI_TIME_SIZE_Y, 0.0f);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y - UI_TIME_SIZE_Y, 0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y + UI_TIME_SIZE_Y, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y + UI_TIME_SIZE_Y, 0.0f);	// �l�ڂ̒��_
		
		// ���_�쐬
		pVtx[0].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;									// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;									// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;									// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = g_aUi[nCntUi].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aUi[nCntUi].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aUi[nCntUi].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aUi[nCntUi].col;	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffUi->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���I������
//
// ------------------------------------------------------------------------------------------

void UninitUi(void)
{
	int nCntUi;

	// �e�N�X�`���̊J��
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		if (g_pTextureUi[nCntUi] != NULL)
		{
			g_pTextureUi[nCntUi]->Release();
			g_pTextureUi[nCntUi] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateUi(void)
{
	//ENEMY *pEnemy;		// �G�l�~�[���

	////�G�l�~�[�擾
	//pEnemy = GetEnemy();

	//// �ϐ��錾
	//VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	//int nCntUi;

	//// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	//g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	//for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	//{
	//	if (g_aUi[nCntUi].bUse == true)
	//	{
	//		if (g_aUi[nCntUi].Type == UITYPE_TIMEFRAME &&
	//			g_aUi[nCntUi].Type == UITYPE_TIMENAME)
	//		{
	//			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	//			{
	//				if (pEnemy->bDisp == true)
	//				{
	//					// �G�Ƃ͈̔�
	//					if (pEnemy->pos.x + pEnemy->fSize / 2 - GetCamera()->difference.x > g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize&&
	//						pEnemy->pos.x - pEnemy->fSize / 2 - GetCamera()->difference.x < g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize &&
	//						pEnemy->pos.y > g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize &&
	//						pEnemy->pos.y - pEnemy->fSize < g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize)
	//					{
	//						// ��������
	//						g_aUi[nCntUi].col.a = 0.5f;
	//					}
	//				}

	//				// �ʏ�\����
	//				else
	//				{
	//					g_aUi[nCntUi].col.a = 1.0f;
	//				}
	//			}
	//		}

	//		// �J���[�ݒ�
	//		pVtx[0].col = g_aUi[nCntUi].col;	// ��ڂ̐F�ݒ�
	//		pVtx[1].col = g_aUi[nCntUi].col;	// ��ڂ̐F�ݒ�
	//		pVtx[2].col = g_aUi[nCntUi].col;	// �O�ڂ̐F�ݒ�
	//		pVtx[3].col = g_aUi[nCntUi].col;	// �l�ڂ̐F�ݒ�
	//	}

	//	// �|�C���g���킹
	//	pVtx += 4;
	//}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawUi(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffUi, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �e�N�X�`���̐ݒ�
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		if (g_aUi[nCntUi].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureUi[g_aUi[nCntUi].Type]);

			// �^�C�g���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���ݒ菈��
//
// ------------------------------------------------------------------------------------------
void SetUi(D3DXVECTOR3 pos, D3DXVECTOR3 move,UITYPE Type,D3DXCOLOR col ,float fXSize,float fYSize)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntUi;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		// �g�p���Ă��Ȃ���
		if (g_aUi[nCntUi].bUse == false)
		{
			g_aUi[nCntUi].Type = Type;
			g_aUi[nCntUi].pos = pos;
			g_aUi[nCntUi].move = move;
			g_aUi[nCntUi].col = col;
			g_aUi[nCntUi].bUse = true;
			g_aUi[nCntUi].fXSize = fXSize;
			g_aUi[nCntUi].fYSize = fYSize;

			// �f�[�^�̃|�C���^�����킹��
			pVtx += nCntUi * 4;

			pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);											// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);						// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);						// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);	// �l�ڂ̒��_

			// �J���[�ݒ�
			pVtx[0].col = g_aUi[nCntUi].col;	// ��ڂ̐F�ݒ�
			pVtx[1].col = g_aUi[nCntUi].col;	// ��ڂ̐F�ݒ�
			pVtx[2].col = g_aUi[nCntUi].col;	// �O�ڂ̐F�ݒ�
			pVtx[3].col = g_aUi[nCntUi].col;	// �l�ڂ̐F�ݒ�

			// for���𔲂���
			break;
		}
	}
	// �A�����b�N
	g_pVtxBuffUi->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���擾����
//
// ------------------------------------------------------------------------------------------

UI *GetUi(void)
{
	return &g_aUi[0];
}