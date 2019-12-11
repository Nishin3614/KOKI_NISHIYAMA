// ------------------------------------------------------------------------------------------
//
// �^�C������ [time.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "time.h"

#include "game.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_TIME		"data/TEXTURE/TimeNumber.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TIME_SIZE (25)								// �^�C���{�[�h�̑傫��
#define TIME_MAX (3)								// ���l

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ��錾
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_TimePos;							// �^�C���{�[�h�̃|�W�V����
D3DXCOLOR				g_TimeCol;							// �^�C���J���[
int						g_nTime;							// �^�C��
int						g_nTimer;							// �^�C�}�[
int						g_nCntTimeState;					// �Q�[���I�[�o�[�ւ̃J�E���g

// ------------------------------------------------------------------------------------------
//
// ����������
//
// ------------------------------------------------------------------------------------------
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_TimePos = D3DXVECTOR3(1220.0f, 50.0f, 0.0f);
	g_TimeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_nTime = 300;
	g_nTimer = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TIME,		// �t�@�C���̖��O
		&g_pTextureTime);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexTime(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// �I������
//
// ------------------------------------------------------------------------------------------
void UninitTime(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateTime(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTime;		// �X�R�A�J�E���g
	int nTime;			// ���X�R�A
	float fTime;		// �e�N�X�`���ݒ�

	// �^�C���A�b�v
	g_nTimer++;

	// 1�b�J�E���g
	if (g_nTimer % 60 == 0)
	{
		g_nTime--;	// �X�R�A���Z
	}

	// �ԐF
	if (g_nTime <= 100)
	{
		g_TimeCol.g = 0.0f;
	}

	// �Q�[���I�[�o�[
	if (g_nTime <= 0)
	{
		g_nCntTimeState++;

		if (g_nCntTimeState >= 30)
		{
			SetGameState(GAMESTATE_GAMEOVER);
		}
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		// ���̐���
		nTime = g_nTime % (int)powf(10.0f, (float)nCntTime + 1.0f) / (int)powf(10.0f, (float)nCntTime);
		fTime = (float)nTime / 10.0f;

		pVtx[0].col = g_TimeCol;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_TimeCol;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_TimeCol;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_TimeCol;	// �l�ڂ̐F�ݒ�

		pVtx[0].tex = D3DXVECTOR2(fTime, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(fTime + 0.1f, 0.0f);	// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(fTime, 1.0f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(fTime + 0.1f, 1.0f);	// �l�ڂ̃e�N�X�`��

		// �|�C���g���킹
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffTime->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTime;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTime, 2);
	}
}

// ------------------------------------------------------------------------------------------
//
// ���_�̍쐬
//
// ------------------------------------------------------------------------------------------
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntTime;		// �^�C���J�E���g

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTime, NULL);

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_TimePos.x + (-TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime, g_TimePos.y + (-TIME_SIZE / 2), 0.0f);				// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_TimePos.x + (TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime + TIME_SIZE, g_TimePos.y + (-TIME_SIZE / 2), 0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(g_TimePos.x + (-TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime, g_TimePos.y + TIME_SIZE / 2, 0.0f);					// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_TimePos.x + (TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime + TIME_SIZE, g_TimePos.y + TIME_SIZE / 2, 0.0f);		// �l�ڂ̒��_

		pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

		pVtx[0].col = g_TimeCol;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_TimeCol;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_TimeCol;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_TimeCol;	// �l�ڂ̐F�ݒ�

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffTime->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �^�C���̎擾
//
// ------------------------------------------------------------------------------------------

int GetTime(void)
{
	return g_nTime;
}
