// ------------------------------------------------------------------------------------------
//
// �e���� [shadow.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "shadow.h"

//#include "polygon.h"

#include "floor.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define SHADOW_MAX (128)								// �e�̍ő吔 
#define SHADOW_SIZE (20)							// �T�C�Y
#define SHADOW_PHONT "data/TEXTURE/shadow000.jpg"	// �e�̎ʐ^

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// ���_�e�N�X�`���[�̃|�C���^
SHADOW					g_shadow[SHADOW_MAX];		// �e���

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntShadow;								// �J�E���g�e
	
	// �����ݒ�
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		g_shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_shadow[nCntShadow].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SHADOW_PHONT, &g_pTextureShadow);

	// ���_���̍쐬
	MakeVertexShadow(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitShadow(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateShadow(void)
{

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawShadow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	SHADOW *pShadow = &g_shadow[0];				// �e���
	D3DXMATRIX mtxRot, mtxTrans;				// ��]�E�ʒu�̌v�Z�p�ϐ�
	int nCntShadow;								// �J�E���g�e

	// ���Z 
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pShadow->mtxWorldShadow);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pShadow->mtxWorldShadow,	// 1
				&pShadow->mtxWorldShadow,					// 2
				&mtxRot);									// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pShadow->mtxWorldShadow,	// 1
				&pShadow->mtxWorldShadow,					// 2
				&mtxTrans);									// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorldShadow);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			// �e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ------------------------------------------------------------------------------------------
// �e�̐ݒ�
// ------------------------------------------------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	FLOOR *pFloor = GetFLOOR();	// �|���S�����
	int nCntShadow;		// �J�E���g�o���b�g

	// �u���b�N���[�v
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_shadow[nCntShadow].bUse)
		{
			// ���X�V
			g_shadow[nCntShadow].pos = pos;
			g_shadow[nCntShadow].size = size;
			g_shadow[nCntShadow].col = col;
			g_shadow[nCntShadow].bUse = true;

			// �e��y���W
			g_shadow[nCntShadow].pos.y = pFloor->pos.y;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);

			// �|�C���g���킹
			pVtx += 4 * nCntShadow;

			// ���_���W�̐ݒ�					
			pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, g_shadow[nCntShadow].size.z);		// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x, 0.1f, g_shadow[nCntShadow].size.z);		// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, -g_shadow[nCntShadow].size.z);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x, 0.1f, -g_shadow[nCntShadow].size.z);		// �l�ڂ̒��_

			// �J���[
			pVtx[0].col = g_shadow[nCntShadow].col;	// ��ڂ̐F�ݒ�
			pVtx[1].col = g_shadow[nCntShadow].col;	// ��ڂ̐F�ݒ�
			pVtx[2].col = g_shadow[nCntShadow].col;	// �O�ڂ̐F�ݒ�
			pVtx[3].col = g_shadow[nCntShadow].col;	// �l�ڂ̐F�ݒ�

			// �A�����b�N
			g_pVtxBuffShadow->Unlock();

			break;
		}
	}
	return nCntShadow;
}

// ------------------------------------------------------------------------------------------
// �e�̈ʒu�X�V
// ------------------------------------------------------------------------------------------
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	FLOOR *pFloor = GetFLOOR();	// �|���S�����
	float fWhile;						// �n�ʂƃ��f���̋���

	// �ʒu�̍X�V
	g_shadow[nIdxShadow].pos = pos;

	// ����
	fWhile = g_shadow[nIdxShadow].pos.y - pFloor->pos.y;

	g_shadow[nIdxShadow].col.a = 1.0f - fWhile / 200.0f;

	// �e��n�ʂ�
	for (int nCntFloor = 0; nCntFloor < 128; nCntFloor++, pFloor++)
	{
		// �u���b�N��X�͈�
		if (g_shadow[nIdxShadow].pos.x + g_shadow[nIdxShadow].size.x * 0.5f >
			pFloor->pos.x - pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_shadow[nIdxShadow].pos.x - g_shadow[nIdxShadow].size.x * 0.5f <
			pFloor->pos.x + pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_shadow[nIdxShadow].pos.z + g_shadow[nIdxShadow].size.z * 0.5f >
			pFloor->pos.z - pFloor->size.z * 0.5f * pFloor->nBlock_Depth&&
			g_shadow[nIdxShadow].pos.z - g_shadow[nIdxShadow].size.z * 0.5f <
			pFloor->pos.z + pFloor->size.z * 0.5f * pFloor->nBlock_Depth)
		{

			g_shadow[nIdxShadow].pos.y = pFloor->pos.y;
		}
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);

	// �f�[�^�̃|�C���^�����킹��
	pVtx += nIdxShadow * 4;

	// �J���[
	pVtx[0].col = g_shadow[nIdxShadow].col;	// ��ڂ̐F�ݒ�
	pVtx[1].col = g_shadow[nIdxShadow].col;	// ��ڂ̐F�ݒ�
	pVtx[2].col = g_shadow[nIdxShadow].col;	// �O�ڂ̐F�ݒ�
	pVtx[3].col = g_shadow[nIdxShadow].col;	// �l�ڂ̐F�ݒ�

	// �A�����b�N
	g_pVtxBuffShadow->Unlock();
}

// ------------------------------------------------------------------------------------------
// �e���ŏ���
// ------------------------------------------------------------------------------------------
void DeleteShadow(int nIdxShadow)
{
	g_shadow[nIdxShadow].bUse = false;
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntShadow;		// �J�E���g�e

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * SHADOW_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);
	
	// �����ݒ�
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		// ���_���W�̐ݒ�					
		pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, g_shadow[nCntShadow].size.z);		// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x,  0.1f, g_shadow[nCntShadow].size.z);		// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, -g_shadow[nCntShadow].size.z);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x,  0.1f, -g_shadow[nCntShadow].size.z);		// �l�ڂ̒��_

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �l�ڂ̒��_

		// �J���[
		pVtx[0].col = g_shadow[nCntShadow].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_shadow[nCntShadow].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_shadow[nCntShadow].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_shadow[nCntShadow].col;	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �|�C���g���킹
		pVtx += 4;
	}
	// �A�����b�N

	g_pVtxBuffShadow->Unlock();
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
SHADOW *GetShadow(void)
{
	return &g_shadow[0];
}