// ------------------------------------------------------------------------------------------
// ���v���� [Statipoint.cpp]
// Author : Koki Nishiyama
// ------------------------------------------------------------------------------------------
#include "statipoint.h"			// ���v�w�b�_�[
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "giraffe.h"
#include "buffalo.h"
#include "king_rion.h"
#include "time.h"
#include "ranking.h"
#include "score.h"
#include "Load_Save.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define	TEXTURE_STATIPOINT		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define STATIPOINT_SIZE (25)										// ���v�{�[�h�̑傫��
#define STATIPOINT_BETWEEN (25)										// �Ԃ̊Ԋu
#define POINT_MAX (6)												// ���l
#define TIME_BORNUS (10)											// �^�C���{�[�i�X
#define ENEMY_BORNUS (200)											// ���j�{�[�i�X

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexStatipoint(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ��錾
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureStatipoint = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStatipoint = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
STATIPOINT				g_aStatipoint[STATIPOINTTYPE_MAX];		// ���v�ϐ�
STATIPOINTTYPE			g_StatiPointType = STATIPOINTTYPE_TIME;
int						g_nStatipointScore;
int						g_nCntStatipoint;
float					g_fColStatipoint;

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitStatipoint(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	int nCntStatipoint;							// �J�E���g���v
	int nTime			= GetTime();			// �^�C��
	int nScore			= GetScore();			// ������

	// �X�R�A�Q�b�g
	g_nStatipointScore = 0;
	g_StatiPointType = STATIPOINTTYPE_TIME;

	// ���v�J�n
	for (nCntStatipoint = 0; nCntStatipoint < STATIPOINTTYPE_MAX; nCntStatipoint++)
	{
		switch (nCntStatipoint)
		{
		case STATIPOINTTYPE_TIME:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(300.0, 150.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint = nTime;
			break;
		case STATIPOINTTYPE_TIMEBORNUS:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(1200.0, 150.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint =
				g_aStatipoint[STATIPOINTTYPE_TIME].nPoint * TIME_BORNUS;
			break;
		case STATIPOINTTYPE_ENEMY:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(300.0, 400.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint = nScore;
			break;
		case STATIPOINTTYPE_ENEMYBORNUS:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(1200.0, 400.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint = 
				g_aStatipoint[STATIPOINTTYPE_ENEMY].nPoint * ENEMY_BORNUS;
			break;
		case STATIPOINTTYPE_SUM:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(1200.0, 650.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint =
				g_aStatipoint[STATIPOINTTYPE_TIMEBORNUS].nPoint + 
				g_aStatipoint[STATIPOINTTYPE_ENEMYBORNUS].nPoint;
			break;
		default:
			break;
		}
		g_aStatipoint[nCntStatipoint].nId = nCntStatipoint;
	}
	
	SaveData(g_aStatipoint[STATIPOINTTYPE_SUM].nPoint);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_STATIPOINT,				// �t�@�C���̖��O
		&g_pTextureStatipoint);			// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexStatipoint(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitStatipoint(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureStatipoint != NULL)
	{
		g_pTextureStatipoint->Release();
		g_pTextureStatipoint = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffStatipoint != NULL)
	{
		// ���_�o�b�t�@�̊J��
		g_pVtxBuffStatipoint->Release();
		g_pVtxBuffStatipoint = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateStatipoint(void)
{
	int nCntCol;

	// ���_�o�b�t�@�ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffStatipoint->Lock(0, 0, (void **)&pVtx, 0);
	g_nCntStatipoint++;

	if (g_nCntStatipoint % 14 == 0)
	{
		g_fColStatipoint = 0.0f;
	}

	else if (g_nCntStatipoint % 7 == 0)
	{
		g_fColStatipoint = 1.0f;
	}

	// �|�C���g���킹
	pVtx += STATIPOINTTYPE_SUM * POINT_MAX * 4;

	for (nCntCol = 0; nCntCol < POINT_MAX; nCntCol++)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// �l�ڂ̐F�ݒ�

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffStatipoint->Unlock();
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawStatipoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStatipoint;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffStatipoint, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureStatipoint);

	for (nCntStatipoint = 0; nCntStatipoint < POINT_MAX * STATIPOINTTYPE_MAX; nCntStatipoint++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntStatipoint, 2);
	}
}

// ------------------------------------------------------------------------------------------
// ���_�̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexStatipoint(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntStatipoint;		// ���v�J�E���g
	int nCntStatipointDate;		// ���v�J�E���g
	int nStatipoint;			// �����v
	float fStatipoint;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POINT_MAX * STATIPOINTTYPE_MAX, 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStatipoint,
		NULL);

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffStatipoint->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntStatipointDate = 0; nCntStatipointDate < STATIPOINTTYPE_MAX; nCntStatipointDate++)
	{
		for (nCntStatipoint = 0; nCntStatipoint < POINT_MAX; nCntStatipoint++)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (-STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint,
				g_aStatipoint[nCntStatipointDate].pos.y + (-STATIPOINT_SIZE / 2), 0.0f);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint + STATIPOINT_SIZE,
				g_aStatipoint[nCntStatipointDate].pos.y + (-STATIPOINT_SIZE / 2) , 0.0f);		// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (-STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint,
				g_aStatipoint[nCntStatipointDate].pos.y + (STATIPOINT_SIZE / 2), 0.0f);		// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint + STATIPOINT_SIZE,
				g_aStatipoint[nCntStatipointDate].pos.y + (STATIPOINT_SIZE / 2), 0.0f);		// �l�ڂ̒��_

			pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
			pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
			pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
			pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

			nStatipoint = g_aStatipoint[nCntStatipointDate].nPoint % (int)powf(10.0f, (float)nCntStatipoint + 1.0f) / (int)powf(10.0f, (float)nCntStatipoint);
			fStatipoint = (float)nStatipoint / 10.0f;

			pVtx[0].tex = D3DXVECTOR2(fStatipoint, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[1].tex = D3DXVECTOR2(fStatipoint + 0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[2].tex = D3DXVECTOR2(fStatipoint, 1.0f);		// �O�ڂ̃e�N�X�`��
			pVtx[3].tex = D3DXVECTOR2(fStatipoint + 0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

			pVtx += 4;
		}
	}
	// �A�����b�N
	g_pVtxBuffStatipoint->Unlock();
}

// ------------------------------------------------------------------------------------------
// ���v�̕ύX
// ------------------------------------------------------------------------------------------
void AddStatipoint(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntPoint;		// ���l�J�E���g
	int nCntPointDate;	// ���v�J�E���g
	int nStatipoint;			// �����v
	float fStatipoint;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffStatipoint->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPointDate = 0; nCntPointDate < STATIPOINTTYPE_MAX; nCntPointDate++)
	{
		for (nCntPoint = 0; nCntPoint < POINT_MAX; nCntPoint++)
		{
			nStatipoint = g_aStatipoint[nCntPointDate].nPoint % (int)powf(10.0f, (float)nCntPoint + 1.0f) / (int)powf(10.0f, (float)nCntPoint);
			fStatipoint = (float)nStatipoint / 10.0f;

			pVtx[0].tex = D3DXVECTOR2(fStatipoint, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[1].tex = D3DXVECTOR2(fStatipoint + 0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[2].tex = D3DXVECTOR2(fStatipoint, 1.0f);		// �O�ڂ̃e�N�X�`��
			pVtx[3].tex = D3DXVECTOR2(fStatipoint + 0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

			pVtx += 4;
		}
	}
	// �A�����b�N
	g_pVtxBuffStatipoint->Unlock();
}