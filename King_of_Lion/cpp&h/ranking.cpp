// ------------------------------------------------------------------------------------------
// �����L���O���� [ranking.cpp]
// Author : Koki Nishiyama
// ------------------------------------------------------------------------------------------
#include "ranking.h"			// �����L���O�w�b�_�[

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "score.h"

#include "Load_Save.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define	TEXTURE_RANKING		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define RANKING_SIZE (30)								// �����L���O�{�[�h�̑傫��
#define RANKING_BETWEEN (30)
#define POINT_MAX (6)									// ���l
#define RANKING_DATE (5)								// �����L���O��
#define SAVE_FILE_NAME ("text.txt") // �Z�[�u�t�@�C��

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ��錾
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureRanking = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_RankingPos;							// �����L���O�{�[�h�̃|�W�V����
RANKING					g_aRanking[RANKING_DATE];	// �����L���O�ϐ�
int						g_nRankScore;
int						g_nCntRanking;
float					g_fColRanking;
int						g_Num;

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitRanking(void)
{
	// �ϐ��錾
	int nCntRank;										// �J�E���g�����L���O
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�擾
	g_RankingPos = D3DXVECTOR3(850.0f, 150.0f, 0.0f);	// �|�W�V����
	LOAD_SAVE * pLoad_Save = GetLoad_Save();

	// �X�R�A�Q�b�g
	g_nRankScore = 0;

	for (nCntRank = 0; nCntRank < RANKING_DATE; nCntRank++,pLoad_Save++)
	{
		g_aRanking[nCntRank].nPoint = pLoad_Save->nPoint;
		g_aRanking[nCntRank].nId = nCntRank;
	}
	

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RANKING,		// �t�@�C���̖��O
		&g_pTextureRanking);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexRanking(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitRanking(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRanking != NULL)
	{
		// ���_�o�b�t�@�̊J��
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateRanking(void)
{
	int nCntRanking;
	int nCntCol;

	// ���_�o�b�t�@�ւ̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < RANKING_DATE; nCntRanking++)
	{
		if (g_aRanking[nCntRanking].nPoint == g_nRankScore)
		{
			g_nCntRanking++;

			if (g_nCntRanking % 14 == 0)
			{
				g_fColRanking = 0.0f;
			}

			else if (g_nCntRanking % 7 == 0)
			{
				g_fColRanking = 1.0f;
			}

			// �|�C���g���킹
			pVtx += nCntRanking * POINT_MAX * 4;

			for (nCntCol = 0; nCntCol < POINT_MAX; nCntCol++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// �l�ڂ̐F�ݒ�

				pVtx += 4;
			}

			// �|�C���g�߂�
			pVtx -= nCntRanking * POINT_MAX * 4 + POINT_MAX * 4;

		}
	}
	// �A�����b�N
	g_pVtxBuffRanking->Unlock();
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntRanking;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking);

	for (nCntRanking = 0; nCntRanking < POINT_MAX * RANKING_DATE; nCntRanking++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

// ------------------------------------------------------------------------------------------
// ���_�̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntRanking;		// �����L���O�J�E���g
	int nCntRankingDate;		// �����L���O�J�E���g

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POINT_MAX * RANKING_DATE, 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRankingDate = 0; nCntRankingDate < RANKING_DATE; nCntRankingDate++)
	{
		for (nCntRanking = 0; nCntRanking < POINT_MAX; nCntRanking++)
		{
			pVtx[0].pos = D3DXVECTOR3(g_RankingPos.x + (-RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking, 
				g_RankingPos.y + (-RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate, 0.0f);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_RankingPos.x + (RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking + RANKING_SIZE, 
				g_RankingPos.y + (-RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate, 0.0f);		// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(g_RankingPos.x + (-RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking, 
				g_RankingPos.y + (RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate + RANKING_SIZE, 0.0f);		// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_RankingPos.x + (RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking + RANKING_SIZE, 
				g_RankingPos.y + (RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate + RANKING_SIZE, 0.0f);		// �l�ڂ̒��_

			pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
			pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
			pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
			pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

			if (nCntRankingDate == 0)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// �l�ڂ̐F�ݒ�
			}
			else if (nCntRankingDate == 1)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�
			}

			else if (nCntRankingDate == 2)
			{
				pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// �l�ڂ̐F�ݒ�
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// �l�ڂ̐F�ݒ�
			}

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// �O�ڂ̃e�N�X�`��
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

			pVtx += 4;
		}
	}
	// �A�����b�N
	g_pVtxBuffRanking->Unlock();
}

// ------------------------------------------------------------------------------------------
// �����L���O�̕ύX
// ------------------------------------------------------------------------------------------
void AddRanking(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntPoint;		// ���l�J�E���g
	int nCntPointDate;	// �����L���O�J�E���g
	int nRanking;			// �������L���O
	float fRanking;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPointDate = 0; nCntPointDate < RANKING_DATE; nCntPointDate++)
	{
		for (nCntPoint = 0; nCntPoint < POINT_MAX; nCntPoint++)
		{
			nRanking = g_aRanking[nCntPointDate].nPoint % (int)powf(10.0f, (float)nCntPoint + 1.0f) / (int)powf(10.0f, (float)nCntPoint);
			fRanking = (float)nRanking / 10.0f;

			pVtx[0].tex = D3DXVECTOR2(fRanking, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[1].tex = D3DXVECTOR2(fRanking + 0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
			pVtx[2].tex = D3DXVECTOR2(fRanking, 1.0f);		// �O�ڂ̃e�N�X�`��
			pVtx[3].tex = D3DXVECTOR2(fRanking + 0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

			pVtx += 4;
		}
	}
	// �A�����b�N
	g_pVtxBuffRanking->Unlock();
}