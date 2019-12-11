// ------------------------------------------------------------------------------------------
//
// �\�������� [prediction.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "prediction.h"

//#include "polygon.h"

#include "floor.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define PRIDICTION_MAX (128)								// �\�����̍ő吔 
#define PRIDICTION_SIZE (20)							// �T�C�Y
#define PRIDICTION_PHONT "data/TEXTURE/shadow000.jpg"	// �\�����̎ʐ^

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexPrediction(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPrediction = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTexturePrediction = NULL;	// ���_�e�N�X�`���[�̃|�C���^
PRIDICTION				g_prediction[PRIDICTION_MAX];	// �\�������

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitPrediction(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntPrediction;								// �J�E���g�\����
	
	// �����ݒ�
	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++)
	{
		g_prediction[nCntPrediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_prediction[nCntPrediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_prediction[nCntPrediction].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_prediction[nCntPrediction].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PRIDICTION_PHONT, &g_pTexturePrediction);

	// ���_���̍쐬
	MakeVertexPrediction(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitPrediction(void)
{
	// �e�N�X�`���̊J��
	if (g_pTexturePrediction != NULL)
	{
		g_pTexturePrediction->Release();
		g_pTexturePrediction = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPrediction != NULL)
	{
		g_pVtxBuffPrediction->Release();
		g_pVtxBuffPrediction = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdatePrediction(void)
{

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawPrediction(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	PRIDICTION *pPrediction = &g_prediction[0];				// �\�������
	D3DXMATRIX mtxRot, mtxTrans;				// ��]�E�ʒu�̌v�Z�p�ϐ�
	int nCntPrediction;								// �J�E���g�\����

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++,pPrediction++)
	{
		if (pPrediction->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pPrediction->mtxWorldPrediction);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pPrediction->rot.y, pPrediction->rot.x, pPrediction->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pPrediction->mtxWorldPrediction,	// 1
				&pPrediction->mtxWorldPrediction,					// 2
				&mtxRot);									// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pPrediction->pos.x, pPrediction->pos.y, pPrediction->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pPrediction->mtxWorldPrediction,	// 1
				&pPrediction->mtxWorldPrediction,					// 2
				&mtxTrans);									// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pPrediction->mtxWorldPrediction);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffPrediction, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePrediction);

			// �\�����̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPrediction * 4, 2);
		}
	}
	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ------------------------------------------------------------------------------------------
// �\�����̐ݒ�
// ------------------------------------------------------------------------------------------
int SetPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	FLOOR *pFloor = GetFLOOR();	// �|���S�����
	int nCntPrediction;		// �J�E���g�o���b�g

	// �u���b�N���[�v
	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_prediction[nCntPrediction].bUse)
		{
			// ���X�V
			g_prediction[nCntPrediction].pos = pos;
			g_prediction[nCntPrediction].size = size;
			g_prediction[nCntPrediction].col = col;
			g_prediction[nCntPrediction].bUse = true;

			// �\������y���W
			g_prediction[nCntPrediction].pos.y = pFloor->pos.y;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_pVtxBuffPrediction->Lock(0, 0, (void **)&pVtx, 0);

			// �|�C���g���킹
			pVtx += 4 * nCntPrediction;

			// ���_���W�̐ݒ�					
			pVtx[0].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, g_prediction[nCntPrediction].size.z);		// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x, 0.1f, g_prediction[nCntPrediction].size.z);		// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, -g_prediction[nCntPrediction].size.z);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x, 0.1f, -g_prediction[nCntPrediction].size.z);		// �l�ڂ̒��_

			// �J���[
			pVtx[0].col = g_prediction[nCntPrediction].col;	// ��ڂ̐F�ݒ�
			pVtx[1].col = g_prediction[nCntPrediction].col;	// ��ڂ̐F�ݒ�
			pVtx[2].col = g_prediction[nCntPrediction].col;	// �O�ڂ̐F�ݒ�
			pVtx[3].col = g_prediction[nCntPrediction].col;	// �l�ڂ̐F�ݒ�

			// �A�����b�N
			g_pVtxBuffPrediction->Unlock();

			break;
		}
	}
	return nCntPrediction;
}

// ------------------------------------------------------------------------------------------
// �\�����̈ʒu�X�V
// ------------------------------------------------------------------------------------------
void SetPositionPrediction(int nIdxPrediction, D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	FLOOR *pFloor = GetFLOOR();	// �|���S�����
	float fWhile;						// �n�ʂƃ��f���̋���

	// �ʒu�̍X�V
	g_prediction[nIdxPrediction].pos = pos;

	// ����
	fWhile = g_prediction[nIdxPrediction].pos.y - pFloor->pos.y;

	g_prediction[nIdxPrediction].col.a = 1.0f - fWhile / 200.0f;

	// �\������n�ʂ�
	for (int nCntFloor = 0; nCntFloor < 128; nCntFloor++, pFloor++)
	{
		// �u���b�N��X�͈�
		if (g_prediction[nIdxPrediction].pos.x + g_prediction[nIdxPrediction].size.x * 0.5f >
			pFloor->pos.x - pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_prediction[nIdxPrediction].pos.x - g_prediction[nIdxPrediction].size.x * 0.5f <
			pFloor->pos.x + pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_prediction[nIdxPrediction].pos.z + g_prediction[nIdxPrediction].size.z * 0.5f >
			pFloor->pos.z - pFloor->size.z * 0.5f * pFloor->nBlock_Depth&&
			g_prediction[nIdxPrediction].pos.z - g_prediction[nIdxPrediction].size.z * 0.5f <
			pFloor->pos.z + pFloor->size.z * 0.5f * pFloor->nBlock_Depth)
		{

			g_prediction[nIdxPrediction].pos.y = pFloor->pos.y;
		}
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffPrediction->Lock(0, 0, (void **)&pVtx, 0);

	// �f�[�^�̃|�C���^�����킹��
	pVtx += nIdxPrediction * 4;

	// �J���[
	pVtx[0].col = g_prediction[nIdxPrediction].col;	// ��ڂ̐F�ݒ�
	pVtx[1].col = g_prediction[nIdxPrediction].col;	// ��ڂ̐F�ݒ�
	pVtx[2].col = g_prediction[nIdxPrediction].col;	// �O�ڂ̐F�ݒ�
	pVtx[3].col = g_prediction[nIdxPrediction].col;	// �l�ڂ̐F�ݒ�

	// �A�����b�N
	g_pVtxBuffPrediction->Unlock();
}

// ------------------------------------------------------------------------------------------
// �\�������ŏ���
// ------------------------------------------------------------------------------------------
void DeletePrediction(int nIdxPrediction)
{
	g_prediction[nIdxPrediction].bUse = false;
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexPrediction(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntPrediction;		// �J�E���g�\����

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * PRIDICTION_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPrediction, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffPrediction->Lock(0, 0, (void **)&pVtx, 0);
	
	// �����ݒ�
	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++)
	{
		// ���_���W�̐ݒ�					
		pVtx[0].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, g_prediction[nCntPrediction].size.z);		// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x,  0.1f, g_prediction[nCntPrediction].size.z);		// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, -g_prediction[nCntPrediction].size.z);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x,  0.1f, -g_prediction[nCntPrediction].size.z);		// �l�ڂ̒��_

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �l�ڂ̒��_

		// �J���[
		pVtx[0].col = g_prediction[nCntPrediction].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_prediction[nCntPrediction].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_prediction[nCntPrediction].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_prediction[nCntPrediction].col;	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �|�C���g���킹
		pVtx += 4;
	}
	// �A�����b�N

	g_pVtxBuffPrediction->Unlock();
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
PRIDICTION *GetPrediction(void)
{
	return &g_prediction[0];
}