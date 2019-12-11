// ------------------------------------------------------------------------------------------
//
// �������� [hemisphere.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "hemisphere.h"

#include "input.h"

#include "player.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define HEMISPHERE_PHONT "data/TEXTURE/mesh/00_sky.jpg"	// ���̎ʐ^

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexHemisphere(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
HEMISPHERE g_hemisphere;				// ����

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitHemisphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�E�ړ��ʁE�T�C�Y�̏����ݒ�
	g_hemisphere.pos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
	g_hemisphere.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	g_hemisphere.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_hemisphere.size = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	// �u���b�N�̏�����
	g_hemisphere.nBlock_Depth = 20;
	g_hemisphere.nBlock_Width = 20;

	// �u���b�N�`��̌��_
	g_hemisphere.OriginBlock = D3DXVECTOR3(
		sinf(D3DX_PI * 0.0f) * g_hemisphere.size.x * -0.5f,
		0.0f,
		cosf(D3DX_PI * 0.0f) * g_hemisphere.size.z * 0.5f);

	// �e�N�X�`���E�C���f�b�N�X�E�o�b�t�@�̏�����
	g_hemisphere.pTextureHemisphere = NULL;
	g_hemisphere.pIndexMeshField = NULL;
	g_hemisphere.pVtxBuffHemisphere = NULL;

	// �����_���E�C���f�b�N�X�E�����̏�����(�v�Z)
	g_hemisphere.nNumberVertexMeshField = (g_hemisphere.nBlock_Depth + 1) * (g_hemisphere.nBlock_Width + 1);
	g_hemisphere.nNumIndex = (g_hemisphere.nBlock_Width + 1) * 2 * g_hemisphere.nBlock_Depth + 2 * (g_hemisphere.nBlock_Depth - 1);
	g_hemisphere.nNumPolygon = g_hemisphere.nBlock_Depth * g_hemisphere.nBlock_Width * 2 + 4 * (g_hemisphere.nBlock_Depth - 1);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, HEMISPHERE_PHONT, &g_hemisphere.pTextureHemisphere);

	// ���_���̍쐬
	MakeVertexHemisphere(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitHemisphere(void)
{
	// �e�N�X�`���̊J��
	if (g_hemisphere.pTextureHemisphere != NULL)
	{
		g_hemisphere.pTextureHemisphere->Release();
		g_hemisphere.pTextureHemisphere = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_hemisphere.pVtxBuffHemisphere != NULL)
	{
		g_hemisphere.pVtxBuffHemisphere->Release();
		g_hemisphere.pVtxBuffHemisphere = NULL;
	}

	// ���_�C���f�b�N�X�̊J��
	if (g_hemisphere.pIndexMeshField != NULL)
	{
		g_hemisphere.pIndexMeshField->Release();
		g_hemisphere.pIndexMeshField = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateHemisphere(void)
{
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawHemisphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&g_hemisphere.mtxWorldHemisphere);

	// ��]�𔽉f //
	// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
		g_hemisphere.rot.y, g_hemisphere.rot.x, g_hemisphere.rot.z);	// ���ꂼ��̉�]��
	
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&g_hemisphere.mtxWorldHemisphere,	// 1
		&g_hemisphere.mtxWorldHemisphere,					// 2
		&mtxRot);							// 3

	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
		g_hemisphere.pos.x, g_hemisphere.pos.y, g_hemisphere.pos.z);	// ���ꂼ��̈ړ���

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&g_hemisphere.mtxWorldHemisphere,	// 1
		&g_hemisphere.mtxWorldHemisphere,					// 2
		&mtxTrans);							// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_hemisphere.mtxWorldHemisphere);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_hemisphere.pVtxBuffHemisphere, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
	pDevice->SetIndices(g_hemisphere.pIndexMeshField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_hemisphere.pTextureHemisphere);

	// �\��(�E���)�ŃJ�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// �����̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_hemisphere.nNumberVertexMeshField, 0, g_hemisphere.nNumPolygon);

	// ����(�����)�ŃJ�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexHemisphere(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;
	float fXZAngel;
	float fXZRadian;
	float fYAngle;
	float fYRadian;

	// �p�x�̌v�Z
	fXZAngel = D3DX_PI * 2 / (g_hemisphere.nBlock_Width);
	fYAngle = D3DX_PI / g_hemisphere.nBlock_Depth;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_hemisphere.nNumberVertexMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_hemisphere.pVtxBuffHemisphere,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_hemisphere.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_hemisphere.pIndexMeshField,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// �C���f�b�N�X���̐ݒ�
	WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_hemisphere.pVtxBuffHemisphere->Lock(0, 0, (void **)&pVtx, 0);

	 //���_�ݒ� //
	 //�s���[�v
	for (nCountDirect = 0; nCountDirect < g_hemisphere.nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (nCountWidth = 0; nCountWidth < g_hemisphere.nBlock_Width + 1; nCountWidth++)
		{
			// ���W�A���l(XZ)
			fXZRadian = fXZAngel * nCountWidth;

			// ���W�A���l(Y)
			fYRadian = fYAngle * nCountDirect;

			// 
			float kake = (nCountDirect) / 1.0f;

			// -3.14����
			if (fXZRadian  < -D3DX_PI)
			{
				fXZRadian += D3DX_PI * 2;
			}

			// 3.14����
			else if (fXZRadian > D3DX_PI)
			{
				fXZRadian -= D3DX_PI * 2;
			}

			// ���_���W
			pVtx[0].pos = D3DXVECTOR3(
				g_hemisphere.OriginBlock.x + sinf(fXZRadian) * g_hemisphere.size.x * kake,
				g_hemisphere.OriginBlock.y + cosf(fYRadian) * g_hemisphere.size.y, 
				g_hemisphere.OriginBlock.z - cosf(fXZRadian) * g_hemisphere.size.z * kake);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = pVtx[0].pos;
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			// �F�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(
				1.0f / g_hemisphere.nBlock_Width * nCountWidth,
				1.0f / g_hemisphere.nBlock_Depth * nCountDirect);

			// �|�C���g���킹
			pVtx++;
		}
	}

	// �A�����b�N
	g_hemisphere.pVtxBuffHemisphere->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_hemisphere.pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

	// �c�u���b�N��
	for (nCountDirect = 0; nCountDirect < g_hemisphere.nBlock_Depth; nCountDirect++)
	{
		// �Q��ڂ̃��[�v�ȍ~
		if (nCountDirect >= 1)
		{
			// �k�ޔ������̒��_�ǉ�
			pIdx[0] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + g_hemisphere.nBlock_Width + 1;
 
			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}

		// ���u���b�N�̒��_��
		for (nCountWidth = 0; nCountWidth < g_hemisphere.nBlock_Width + 1; nCountWidth++)
		{
			// �`�揇�Ԃ̃C���f�b�N�X
			pIdx[0] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + nCountWidth + g_hemisphere.nBlock_Width + 1;
			pIdx[1] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + nCountWidth ;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx += 2;
		}

		// �k�ޔ��������K�v������ꍇ
		if (nCountDirect < g_hemisphere.nBlock_Depth -1)
		{
			// �k�ޔ������̒��_�ǉ�
			pIdx[0] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + g_hemisphere.nBlock_Width;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}
	}

	// �A�����b�N
	g_hemisphere.pIndexMeshField->Unlock();
}

// ------------------------------------------------------------------------------------------
// �������
// ------------------------------------------------------------------------------------------
HEMISPHERE *GetHemisphere(void)
{
	return &g_hemisphere;
}