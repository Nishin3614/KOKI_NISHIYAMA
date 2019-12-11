// ------------------------------------------------------------------------------------------
//
// �X�R�A���� [pointscore.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "point.h"

#include "camera.h"

#include "Player.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_POINTSCORE		"data/TEXTURE/PointNumber.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define POINTSCORE_SIZE (25)									// �X�R�A�{�[�h�̑傫��
#define POINTSCORE_MAX (128)									// ���l�}�b�N�X
#define POINTSCORE_TEXTURE (4)								// �e�N�X�`���[

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
void MakeVertexPointScore(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ��錾
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTexturePointScore = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPointScore = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
POINTSCORE				g_PointScore[POINTSCORE_MAX * POINTSCORE_TEXTURE];				// �|�C���g�ϐ�
int						g_nPointScore;				// �|�C���g�⏕

// ------------------------------------------------------------------------------------------
//
// ����������
//
// ------------------------------------------------------------------------------------------
void InitPointScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntPointScore;

	// ������
	for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX * POINTSCORE_TEXTURE; nCntPointScore++)
	{
		g_PointScore[nCntPointScore].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_PointScore[nCntPointScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_PointScore[nCntPointScore].nPointScore = 0;
		g_PointScore[nCntPointScore].nCntAnim = 0;
		g_PointScore[nCntPointScore].bUse = false;
	}

	// ������
	g_nPointScore = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_POINTSCORE,		// �t�@�C���̖��O
		&g_pTexturePointScore);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	MakeVertexPointScore(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// �I������
//
// ------------------------------------------------------------------------------------------
void UninitPointScore(void)
{
	// �e�N�X�`���̊J��
	if (g_pTexturePointScore != NULL)
	{
		g_pTexturePointScore->Release();
		g_pTexturePointScore = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPointScore != NULL)
	{
		g_pVtxBuffPointScore->Release();
		g_pVtxBuffPointScore = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �X�V����
//
// ------------------------------------------------------------------------------------------
void UpdatePointScore(void)
{
	//VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	//int nCntPointScore;		// �X�R�A�J�E���g
	//int nCntTexture;	// �e�N�X�g�J�E���g
	//PLAYER *pPlayer;	// �v���C�����

	//// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	//g_pVtxBuffPointScore->Lock(0, 0, (void **)&pVtx, 0);

	//// �J�E���g�|�C���g�X�R�A
	//for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX; nCntPointScore++)
	//{
	//	// �J�E���g�e�N�X�`���[
	//	for (nCntTexture = 0; nCntTexture < POINTSCORE_TEXTURE; nCntTexture++)
	//	{
	//		// �g���Ă��邩�ǂ���
	//		if (g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].bUse == true)
	//		{
	//			// �v���C���[�擾
	//			pPlayer = GetPlayer();
	//			
	//			// �J�E���g�A�j���A�b�v
	//			g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].nCntAnim++;

	//			// ���g�p��Ԃ�
	//			if (g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].nCntAnim >= 50)
	//			{
	//				g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].bUse = false;
	//			}

	//			// ���_���W�X�V
	//			pVtx[0].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);				// ��ڂ̒��_
	//			pVtx[1].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);	// ��ڂ̒��_
	//			pVtx[2].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);					// �O�ڂ̒��_
	//			pVtx[3].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);		// �l�ڂ̒��_

	//		}
	//		// �|�C���g���킹
	//		pVtx += 4;
	//	}
	//}
	//// �A�����b�N
	//g_pVtxBuffPointScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawPointScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntPointScore;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPointScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePointScore);

	for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX * POINTSCORE_TEXTURE; nCntPointScore++)
	{
		if (g_PointScore[nCntPointScore].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPointScore, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// ���_�̍쐬
//
// ------------------------------------------------------------------------------------------
void MakeVertexPointScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntPointScore;		// �X�R�A�J�E���g

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * POINTSCORE_MAX * POINTSCORE_TEXTURE, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPointScore, NULL);

	// ���_���̐ݒ�
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffPointScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX * POINTSCORE_TEXTURE; nCntPointScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore,
			g_PointScore[nCntPointScore].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);				// ��ڂ̒��_

		pVtx[1].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore + POINTSCORE_SIZE, 
			g_PointScore[nCntPointScore].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);	// ��ڂ̒��_

		pVtx[2].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore,
			g_PointScore[nCntPointScore].pos.y + POINTSCORE_SIZE / 2, 0.0f);					// �O�ڂ̒��_

		pVtx[3].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore + POINTSCORE_SIZE, 
			g_PointScore[nCntPointScore].pos.y + POINTSCORE_SIZE / 2, 0.0f);		// �l�ڂ̒��_

		pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

		pVtx[0].col = g_PointScore[nCntPointScore].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_PointScore[nCntPointScore].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_PointScore[nCntPointScore].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_PointScore[nCntPointScore].col;	// �l�ڂ̐F�ݒ�

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffPointScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �X�R�A�̕ύX
//
// ------------------------------------------------------------------------------------------
void AddPointScore(D3DXVECTOR3 pos,int nValue)
{
	//VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	//int nCntPointScore;		// �X�R�A�J�E���g
	//int nCntTexture;	// �e�N�X�g�J�E���g
	//int nPointScore;			// ���X�R�A
	//float fPointScore;		// �e�N�X�`���ݒ�
	//g_nPointScore = nValue;	// �X�R�A���Z

	//// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	//g_pVtxBuffPointScore->Lock(0, 0, (void **)&pVtx, 0);

	//// �ʒu�w��̂��߂̃��[�v
	//for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX; nCntPointScore++)
	//{
	//	// �����w��̂��߂̃��[�v
	//	for (nCntTexture = 0; nCntTexture < POINTSCORE_TEXTURE; nCntTexture++)
	//	{
	//		if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].bUse == false)
	//		{
	//			// �ʒu
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos = pos;

	//			// �X�R�A
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore = nValue;

	//			// �J�E���g�A�j��
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nCntAnim = 0;

	//			// �g�p���
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].bUse = true;

	//			// �J���[
	//			if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 200)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	//			}

	//			// �J���[
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 300)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//			}

	//			// �J���[
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 400)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	//			}

	//			// �J���[
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 500)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//			}

	//			// �J���[
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 1000)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//			}

	//			// ���̐���
	//			nPointScore = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore %
	//				(int)powf(10.0f, (float)nCntTexture + 1.0f) / 
	//				(int)powf(10.0f, (float)nCntTexture);

	//				// �e�N�X�`�����W
	//				fPointScore = (float)nPointScore / 10.0f;

	//				// �ʒu
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos += D3DXVECTOR3(POINTSCORE_SIZE * 5, 0.0f, 0.0f);

	//				// ���_���W�X�V
	//				pVtx[0].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);				// ��ڂ̒��_
	//				pVtx[1].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);	// ��ڂ̒��_
	//				pVtx[2].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);					// �O�ڂ̒��_
	//				pVtx[3].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);		// �l�ڂ̒��_

	//				// ���_�J���[�X�V
	//				pVtx[0].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// ��ڂ̐F�ݒ�
	//				pVtx[1].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// ��ڂ̐F�ݒ�
	//				pVtx[2].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// �O�ڂ̐F�ݒ�
	//				pVtx[3].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// �l�ڂ̐F�ݒ�

	//				// ���_�J���[(���C���{�[)�X�V
	//				if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore == 5000)
	//				{
	//					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
	//					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// ��ڂ̐F�ݒ�
	//					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
	//					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�
	//				}

	//				// �Ō�̐��l���O�̏ꍇ�X�L�b�v
	//				if (nCntTexture == POINTSCORE_TEXTURE - 1 &&
	//					nPointScore == 0)
	//				{
	//					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);	// ��ڂ̐F�ݒ�
	//					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);	// ��ڂ̐F�ݒ�
	//					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);	// �O�ڂ̐F�ݒ�
	//					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �l�ڂ̐F�ݒ�
	//				}

	//				// ���_�e�N�X�`���[�X�V
	//				pVtx[0].tex = D3DXVECTOR2(fPointScore, 0.0f);		// ��ڂ̃e�N�X�`��
	//				pVtx[1].tex = D3DXVECTOR2(fPointScore + 0.1f, 0.0f);	// ��ڂ̃e�N�X�`��
	//				pVtx[2].tex = D3DXVECTOR2(fPointScore, 1.0f);		// �O�ڂ̃e�N�X�`��
	//				pVtx[3].tex = D3DXVECTOR2(fPointScore + 0.1f, 1.0f);	// �l�ڂ̃e�N�X�`��
	//		}
	//		// �|�C���g���킹
	//		pVtx += 4;
	//	}

	//	// 
	//	if (nCntTexture == POINTSCORE_TEXTURE &&								// �J�E���g���ő�l�ɂȂ����ꍇ
	//		g_PointScore[nCntPointScore * POINTSCORE_TEXTURE].bUse == true &&	// 
	//		g_PointScore[nCntPointScore * POINTSCORE_TEXTURE].nCntAnim == 0)
	//	{
	//		break;
	//	}

	//}
	//// �A�����b�N
	//g_pVtxBuffPointScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �X�R�A�̎擾
//
// ------------------------------------------------------------------------------------------

POINTSCORE *GetPointScore(void)
{
	return &g_PointScore[0];
}