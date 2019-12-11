// ------------------------------------------------------------------------------------------
//
// �z�u���� [gridshape.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "gridshape.h"
#include "input.h"
#include "camera.h"
#include "mouse.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_GRIDSHAPE (1)
#define GRIDSHAPE_MOVE (2.0f)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexGridshape(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 pVtxBuffGridshape = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	pIndexGridshape = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^
D3DXMATRIX				mtxWorldGridshape;			// ���[���h�}�g���b�N�X

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitGridshape(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
												// ���_���̍쐬
	MakeVertexGridshape(pDevice);

}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitGridshape(void)
{
	if (pVtxBuffGridshape != NULL)
	{
		pVtxBuffGridshape->Release();
		pVtxBuffGridshape = NULL;
	}

	// ���_�C���f�b�N�X�̊J��
	if (pIndexGridshape != NULL)
	{
		pIndexGridshape->Release();
		pIndexGridshape = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateGridshape(void)
{

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawGridshape(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	//											// ��]�E�ʒu�̌v�Z�p�ϐ�
	//D3DXMATRIX mtxRot, mtxTrans;

	//int nCntGridshape;
			//// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			//D3DXMatrixIdentity(&g_floor[nCntGridshape].mtxWorldFLOOR);

			//// ��]�𔽉f //
			//// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
			//	g_floor[nCntGridshape].rot.y, g_floor[nCntGridshape].rot.x, g_floor[nCntGridshape].rot.z);	// ���ꂼ��̉�]��

			//																					// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			//D3DXMatrixMultiply(&g_floor[nCntGridshape].mtxWorldFLOOR,	// 1
			//	&g_floor[nCntGridshape].mtxWorldFLOOR,					// 2
			//	&mtxRot);							// 3


			//										// �ʒu�𔽉f //
			//										// ���s�ړ��s��쐬(�I�t�Z�b�g)
			//D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
			//	g_floor[nCntGridshape].pos.x, g_floor[nCntGridshape].pos.y, g_floor[nCntGridshape].pos.z);	// ���ꂼ��̈ړ���

			//																					// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			//D3DXMatrixMultiply(&g_floor[nCntGridshape].mtxWorldFLOOR,	// 1
			//	&g_floor[nCntGridshape].mtxWorldFLOOR,					// 2
			//	&mtxTrans);							// 3

			//										// ���[���h�}�g���b�N�X�̐ݒ�
			//pDevice->SetTransform(D3DTS_WORLD, &g_floor[nCntGridshape].mtxWorldFLOOR);



			//// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			//pDevice->SetStreamSource(0,pVtxBuffGridshape, 0, sizeof(VERTEX_3D));

			//// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			//pDevice->SetIndices(pIndexGridshape);

			//// ���_�t�H�[�}�b�g�̐ݒ�
			//pDevice->SetFVF(FVF_VERTEX_3D);

			//// �|���S���̕`��
			//pDevice->DrawIndexedPrimitive(
			//	D3DPT_TRIANGLESTRIP,
			//	0,
			//	0,
			//	g_floor[nCntGridshape].nNumberVertexMeshField,
			//	0,
			//	g_floor[nCntGridshape].nNumPolygon);
}


// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexGridshape(LPDIRECT3DDEVICE9 pDevice)
{
	//// �ϐ��錾
	//int nCountDirect;
	//int nCountWidth;
	//int nCntGridshape;

	//for (nCntGridshape = 0; nCntGridshape < ALL_MAX; nCntGridshape++)
	//{
	//	// ���_�o�b�t�@�̐���
	//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 1000,
	//		D3DUSAGE_WRITEONLY,
	//		FVF_VERTEX_3D,
	//		D3DPOOL_MANAGED,
	//		&pVtxBuffGridshape,
	//		NULL);

	//	// ���_���̐ݒ�
	//	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	//						// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	//	pVtxBuffGridshape->Lock(0, 0, (void **)&pVtx, 0);

	//		// ���_���W�̐ݒ�
	//		pVtx[0].pos =
	//			D3DXVECTOR3(
	//				g_floor[nCntGridshape].OriginBlock.x + (g_floor[nCntGridshape].size.x * nCountWidth),
	//				0.0f,
	//				g_floor[nCntGridshape].OriginBlock.z - (g_floor[nCntGridshape].size.z * nCountDirect));

	//		// �@���x�N�g���̐ݒ�
	//		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//		if (g_floor[nCntGridshape].bUse == true)
	//		{
	//			// �F�̐ݒ�
	//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		}

	//		else
	//		{
	//			// �F�̐ݒ�
	//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//		}

	//		// �e�N�X�`���[�̐ݒ�
	//		pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

	//		// �|�C���g���킹
	//		pVtx++;

	//	// �A�����b�N
	//	g_floor[nCntGridshape].pVtxBuffFLOOR->Unlock();

	//	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	//	g_floor[nCntGridshape].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

	//	// �c�u���b�N��
	//	for (nCountDirect = 0; nCountDirect < g_floor[nCntGridshape].nBlock_Depth; nCountDirect++)
	//	{
	//		// �Q��ڂ̃��[�v�ȍ~
	//		if (nCountDirect >= 1)
	//		{
	//			// �k�ރ|���S�����̒��_�ǉ�
	//			pIdx[0] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + g_floor[nCntGridshape].nBlock_Width + 1;

	//			// �C���f�b�N�X�̃|�C���g���킹
	//			pIdx++;
	//		}

	//		// ���u���b�N�̒��_��
	//		for (nCountWidth = 0; nCountWidth < g_floor[nCntGridshape].nBlock_Width + 1; nCountWidth++)
	//		{
	//			// �`�揇�Ԃ̃C���f�b�N�X
	//			pIdx[0] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + nCountWidth + g_floor[nCntGridshape].nBlock_Width + 1;
	//			pIdx[1] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + nCountWidth;

	//			// �C���f�b�N�X�̃|�C���g���킹
	//			pIdx += 2;
	//		}

	//		// �k�ރ|���S�������K�v������ꍇ
	//		if (nCountDirect < g_floor[nCntGridshape].nBlock_Depth - 1)
	//		{
	//			// �k�ރ|���S�����̒��_�ǉ�
	//			pIdx[0] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + g_floor[nCntGridshape].nBlock_Width;

	//			// �C���f�b�N�X�̃|�C���g���킹
	//			pIdx++;
	//		}
	//	}

	//	// �A�����b�N
	//	g_floor[nCntGridshape].pIndexMeshField->Unlock();
	//}
}