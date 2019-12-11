// ------------------------------------------------------------------------------------------
//
// �I�������̐���[select.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "select.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define SELECT1 "data/TEXTURE/SELECT001.png"	// �I��
#define SELECT_LEFT_X (0)					// ��
#define SELECT_ON_Y (0)						// ��
#define SELECT_SIZE_X (1280)					// �T�C�Y�i���j
#define SELECT_SIZE_Y (720)					// �T�C�Y�i��)

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureSelect = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	// �e�N�X�`���ւ̃|�C���^

// ------------------------------------------------------------------------------------------
// �I������������
// ------------------------------------------------------------------------------------------
void InitSelect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���g

	// �e�N�X�`���̓ǂݍ���
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, SELECT1, &g_pTextureSelect);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelect, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffSelect->Lock(0, 0, (void **)&pVtx, 0);

	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(SELECT_LEFT_X, SELECT_ON_Y, 0.0f);									// ��ڂ̒��_
	pVtx[1].pos = D3DXVECTOR3(SELECT_LEFT_X + SELECT_SIZE_X, SELECT_ON_Y, 0.0f);					// ��ڂ̒��_
	pVtx[2].pos = D3DXVECTOR3(SELECT_LEFT_X, SELECT_ON_Y + SELECT_SIZE_Y, 0.0f);					// �O�ڂ̒��_
	pVtx[3].pos = D3DXVECTOR3(SELECT_LEFT_X + SELECT_SIZE_X, SELECT_ON_Y + SELECT_SIZE_Y, 0.0f);	// �l�ڂ̒��_

	// ���_�쐬
	pVtx[0].rhw = 1.0f;																// ��ڂ̒��_�쐬
	pVtx[1].rhw = 1.0f;																// ��ڂ̒��_�쐬
	pVtx[2].rhw = 1.0f;																// �O�ڂ̒��_�쐬
	pVtx[3].rhw = 1.0f;																// �l�ڂ̒��_�쐬

	// �J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// ��ڂ̐F�ݒ�
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// ��ڂ̐F�ݒ�
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// �O�ڂ̐F�ݒ�
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// �l�ڂ̐F�ݒ�

	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);								// ��ڂ̃e�N�X�`��
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);								// ��ڂ̃e�N�X�`��
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);								// �O�ڂ̃e�N�X�`��
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);								// �l�ڂ̃e�N�X�`��

	// �A�����b�N
	g_pVtxBuffSelect->Unlock();
}

// ------------------------------------------------------------------------------------------
// �I���I������
// ------------------------------------------------------------------------------------------
void UninitSelect(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureSelect != NULL)
	{
		g_pTextureSelect->Release();
		g_pTextureSelect = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �I���X�V����
// ------------------------------------------------------------------------------------------
void UpdateSelect(void)
{
}

// ------------------------------------------------------------------------------------------
// �I��`�揈��
// ------------------------------------------------------------------------------------------
void DrawSelect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureSelect);

	// �I��`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}