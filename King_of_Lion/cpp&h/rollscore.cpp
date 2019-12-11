// ------------------------------------------------------------------------------------------
//
// �X�R�A���� [rollscorescore.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "rollscore.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_ROLLSCORE		"data/TEXTURE/ui/00_RollScore_Number.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define ROLLSCORE_SIZE (40)								// �X�R�A�{�[�h�̑傫��
#define ROLLSCORE_BETWEEN (15)							// �����Ɛ����̊�
#define ROLLSCORE_MAX (6)								// ���l

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
void MakeVertexRollScore(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ��錾
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureRollScore = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRollScore = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_RollScorePos;							// �X�R�A�{�[�h�̃|�W�V����
float					g_fRollScore = 0.0;		// �e�N�X�`���ݒ�
int						g_nRollScore;							// �X�R�A

// ------------------------------------------------------------------------------------------
//
// ����������
//
// ------------------------------------------------------------------------------------------
void InitRollScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_RollScorePos = D3DXVECTOR3(1100.0f, 50.0f, 0.0f);
	g_nRollScore = 12345;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_ROLLSCORE,		// �t�@�C���̖��O
		&g_pTextureRollScore);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexRollScore(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// �I������
//
// ------------------------------------------------------------------------------------------
void UninitRollScore(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureRollScore != NULL)
	{
		g_pTextureRollScore->Release();
		g_pTextureRollScore = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRollScore != NULL)
	{
		g_pVtxBuffRollScore->Release();
		g_pVtxBuffRollScore = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateRollScore(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntRollScore;		// �X�R�A�J�E���g

	// �c�ɉ�]����
	g_fRollScore += 0.05f;

	// ����𒴂����珉����
	if (g_fRollScore >= 1.0f)
	{
		g_fRollScore = 0.0f;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRollScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_fRollScore);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_fRollScore);	// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_fRollScore + 0.1f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_fRollScore + 0.1f);	// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffRollScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawRollScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntRollScore;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffRollScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRollScore);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRollScore, 2);
	}
}

// ------------------------------------------------------------------------------------------
//
// ���_�̍쐬
//
// ------------------------------------------------------------------------------------------
void MakeVertexRollScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntRollScore;		// �X�R�A�J�E���g

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ROLLSCORE_MAX,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffRollScore,
		NULL);

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRollScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(
			g_RollScorePos.x + (-ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN)  * nCntRollScore,
			g_RollScorePos.y + (-ROLLSCORE_SIZE / 2),
			0.0f);				// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(
			g_RollScorePos.x + (ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN) * nCntRollScore + ROLLSCORE_SIZE,
			g_RollScorePos.y + (-ROLLSCORE_SIZE / 2),
			0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(
			g_RollScorePos.x + (-ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN) * nCntRollScore,
			g_RollScorePos.y + ROLLSCORE_SIZE / 2,
			0.0f);					// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(
			g_RollScorePos.x + (ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN) * nCntRollScore + ROLLSCORE_SIZE,
			g_RollScorePos.y + ROLLSCORE_SIZE / 2,
			0.0f);		// �l�ڂ̒��_

		pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.1f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.1f);		// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffRollScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �X�R�A�̕ύX
//
// ------------------------------------------------------------------------------------------
void AddRollScore(int nValue)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntRollScore;		// �X�R�A�J�E���g
	int nRollScore;			// ���X�R�A
	float fRollScore;		// �e�N�X�`���ݒ�
	g_nRollScore += nValue;	// �X�R�A���Z

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRollScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		// ���̐���
   		nRollScore = g_nRollScore % (int)powf(10.0f, (float)nCntRollScore + 1.0f) / (int)powf(10.0f, (float)nCntRollScore);
 		fRollScore = (float)nRollScore / 10.0f;
		
		pVtx[0].tex = D3DXVECTOR2(0.0f, fRollScore);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, fRollScore);	// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, fRollScore + 0.1f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, fRollScore + 0.1f);	// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffRollScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �X�R�A�̎擾
//
// ------------------------------------------------------------------------------------------

int GetRollScore(void)
{
	return g_nRollScore;
}
