// ------------------------------------------------------------------------------------------
//
// ���ʏ����̐���[rank5.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "rank.h"
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

#define NO_1 "data/TEXTURE/ProvisionalTexture/1��.png"	// 1��
#define NO_2 "data/TEXTURE/ProvisionalTexture/2��.png"	// 2��
#define NO_3 "data/TEXTURE/ProvisionalTexture/3��.png"	// 3��
#define NO_4 "data/TEXTURE/ProvisionalTexture/4��.png"	// 4��
#define NO_5 "data/TEXTURE/ProvisionalTexture/5��.png"	// 5��

#define NO_MAX (5)					// �摜��

#define RANK5_LEFT_X (400)			// ��
#define RANK5_ON_Y (100)			// ��

#define RANK5_SIZE_X (100)			// �T�C�Y�i���j
#define RANK5_SIZE_Y (100)			// �T�C�Y�i���j


// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureRank5[NO_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank5 = NULL;		// �e�N�X�`���ւ̃|�C���^
float g_fRank5Y;									// y�̃|�W�V����

// ------------------------------------------------------------------------------------------
//
// �����L���O�w�i����������
//
// ------------------------------------------------------------------------------------------

void InitRank5(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���g
	int nCntNo;					// �i���o�[�J�E���g

	// ������
	g_fRank5Y = 90.0f;		// y�̃|�W�V����

	// �e�N�X�`���̓ǂݍ���
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, NO_1, &g_pTextureRank5[0]);
	D3DXCreateTextureFromFile(pDevice, NO_2, &g_pTextureRank5[1]);
	D3DXCreateTextureFromFile(pDevice, NO_3, &g_pTextureRank5[2]);
	D3DXCreateTextureFromFile(pDevice, NO_4, &g_pTextureRank5[3]);
	D3DXCreateTextureFromFile(pDevice, NO_5, &g_pTextureRank5[4]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NO_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRank5, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRank5->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntNo = 0; nCntNo < NO_MAX; nCntNo++)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(RANK5_LEFT_X, RANK5_ON_Y + g_fRank5Y * nCntNo, 0.0f);									// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(RANK5_LEFT_X + RANK5_SIZE_X, RANK5_ON_Y + g_fRank5Y* nCntNo, 0.0f);					// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(RANK5_LEFT_X, RANK5_ON_Y + RANK5_SIZE_Y + g_fRank5Y* nCntNo, 0.0f);					// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(RANK5_LEFT_X + RANK5_SIZE_X, RANK5_ON_Y + RANK5_SIZE_Y + g_fRank5Y* nCntNo, 0.0f);	// �l�ڂ̒��_

		// ���_�쐬
		pVtx[0].rhw = 1.0f;																// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;																// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;																// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;																// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);								// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);								// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);								// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);								// �l�ڂ̃e�N�X�`��

		pVtx += 4;	// �|�C���g���킹
	}
	// �A�����b�N
	g_pVtxBuffRank5->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �����L���O�w�i�I������
//
// ------------------------------------------------------------------------------------------

void UninitRank5(void)
{	
	for (int nCntNo = 0; nCntNo < NO_MAX; nCntNo++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureRank5[nCntNo] != NULL)
		{
			g_pTextureRank5[nCntNo]->Release();
			g_pTextureRank5[nCntNo] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRank5 != NULL)
	{
		g_pVtxBuffRank5->Release();
		g_pVtxBuffRank5 = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �����L���O�w�i�X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateRank5(void)
{

}

// ------------------------------------------------------------------------------------------
//
// �����L���O�w�i�`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawRank5(void)
{
	// �ϐ��錾
	int nCntNo;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffRank5, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntNo = 0; nCntNo < NO_MAX; nCntNo++)
	{
		pDevice->SetTexture(0, g_pTextureRank5[nCntNo]);

		// �����L���O�w�i�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntNo, 2);
	}
}