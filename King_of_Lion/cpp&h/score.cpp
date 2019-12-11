// ------------------------------------------------------------------------------------------
//
// �X�R�A���� [score.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "score.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_SCORE	"data/TEXTURE/PointNumber.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define SCORE_SIZE (25)								// �X�R�A�{�[�h�̑傫��
#define SCORE_MAX (3)								// ���l

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ��錾
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_ScorePos;				// �X�R�A�{�[�h�̃|�W�V����
int						g_nScore;				// �X�R�A

// ------------------------------------------------------------------------------------------
//
// ����������
//
// ------------------------------------------------------------------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_ScorePos = D3DXVECTOR3(1200, 140.0f, 0.0f);
	g_nScore = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_SCORE,		// �t�@�C���̖��O
		&g_pTextureScore);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexScore(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// �I������
//
// ------------------------------------------------------------------------------------------
void UninitScore(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateScore(void)
{

}

// ------------------------------------------------------------------------------------------
//
// �`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < SCORE_MAX; nCntScore++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

// ------------------------------------------------------------------------------------------
//
// ���_�̍쐬
//
// ------------------------------------------------------------------------------------------
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntScore;		// �X�R�A�J�E���g

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL);

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < SCORE_MAX; nCntScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_ScorePos.x + (-SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore, g_ScorePos.y + (-SCORE_SIZE / 2), 0.0f);				// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_ScorePos.x + (SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore + SCORE_SIZE, g_ScorePos.y + (-SCORE_SIZE / 2), 0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(g_ScorePos.x + (-SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore, g_ScorePos.y + SCORE_SIZE / 2, 0.0f);					// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_ScorePos.x + (SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore + SCORE_SIZE, g_ScorePos.y + SCORE_SIZE / 2, 0.0f);		// �l�ڂ̒��_

		pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �X�R�A�̕ύX
//
// ------------------------------------------------------------------------------------------
void AddScore(int nValue)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntScore;		// �X�R�A�J�E���g
	int nScore;			// ���X�R�A
	float fScore;		// �e�N�X�`���ݒ�
	g_nScore += nValue;	// �X�R�A���Z

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < SCORE_MAX; nCntScore++)
	{
		// ���̐���
   		nScore = g_nScore % (int)powf(10.0f, (float)nCntScore + 1.0f) / (int)powf(10.0f, (float)nCntScore);
 		fScore = (float)nScore / 10.0f;
		
		pVtx[0].tex = D3DXVECTOR2(fScore, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(fScore + 0.1f, 0.0f);	// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(fScore, 1.0f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(fScore + 0.1f, 1.0f);	// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �X�R�A�̎擾
//
// ------------------------------------------------------------------------------------------

int GetScore(void)
{
	return g_nScore;
}
