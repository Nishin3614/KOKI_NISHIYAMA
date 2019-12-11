// ------------------------------------------------------------------------------------------
//
// �|���S������ [polygon.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "polygon.h"

#include "input.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define POLYGON_SIZE (100)
#define POLYGON_PHONT "data/TEXTURE/soil.jpg"
// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;	// ���_�e�N�X�`���[�̃|�C���^
POLYGON g_polygon;				// �|���S��

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	g_polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_polygon.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	g_polygon.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_PHONT, &g_pTexturePolygon);

	// ���_���̍쐬
	MakeVertexPolygon(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitPolygon(void)
{
	// �e�N�X�`���̊J��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdatePolygon(void)
{
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&g_polygon.mtxWorldPolygon);

	// ��]�𔽉f //
	// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
		g_polygon.rot.y, g_polygon.rot.x, g_polygon.rot.z);	// ���ꂼ��̉�]��
	
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&g_polygon.mtxWorldPolygon,	// 1
		&g_polygon.mtxWorldPolygon,					// 2
		&mtxRot);							// 3


	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
		g_polygon.pos.x, g_polygon.pos.y, g_polygon.pos.z);	// ���ꂼ��̈ړ���

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&g_polygon.mtxWorldPolygon,	// 1
		&g_polygon.mtxWorldPolygon,					// 2
		&mtxTrans);							// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_polygon.mtxWorldPolygon);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygon);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffPolygon->Lock(0, 0, (void **)&pVtx, 0);
									
	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-POLYGON_SIZE, 0, POLYGON_SIZE);	// ��ڂ̒��_
	pVtx[1].pos = D3DXVECTOR3(POLYGON_SIZE, 0, POLYGON_SIZE);	// ��ڂ̒��_
	pVtx[2].pos = D3DXVECTOR3(-POLYGON_SIZE, 0, -POLYGON_SIZE);	// �O�ڂ̒��_
	pVtx[3].pos = D3DXVECTOR3(POLYGON_SIZE, 0, -POLYGON_SIZE);	// �l�ڂ̒��_


	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); 	// ��ڂ̒��_
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); 		// ��ڂ̒��_
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // �O�ڂ̒��_
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // �l�ڂ̒��_

	// �J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

	// �e�N�X�`���[
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̐F�ݒ�
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̐F�ݒ�
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

	// �A�����b�N
	g_pVtxBuffPolygon->Unlock();
}

// ------------------------------------------------------------------------------------------
// �|���S�����
// ------------------------------------------------------------------------------------------
POLYGON *GetPolygon(void)
{
	return &g_polygon;
}