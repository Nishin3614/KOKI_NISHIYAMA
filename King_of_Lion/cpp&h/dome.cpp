// ------------------------------------------------------------------------------------------
//
// �h�[������ [dome.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "dome.h"

#include "input.h"

#include "player.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define DOME_PHONT "data/TEXTURE/Water_Texture.png"	// ���̎ʐ^
#define DOME_MAX (128)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexDome(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
DOME g_dome[DOME_MAX];									// �h�[��
LPDIRECT3DTEXTURE9	pTextureDome[DOMETYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^

														// �e�N�X�`���[�t�@�C������`
const char *g_paTexDomeFile[DOMETYPE_MAX] =
{
	{ "data/TEXTURE/Line.png" },		// 00_��
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitDome(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	// �h�[���̏�����
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// �ʒu�E��]�E�ړ��ʁE�T�C�Y�̏����ݒ�
		g_dome[nCntDome].pos = D3DXVECTOR3(0.0f, 0.0, 0.0f);
		g_dome[nCntDome].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_dome[nCntDome].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_dome[nCntDome].size = D3DXVECTOR3(25.0f, 50.0f, 25.0f);
		g_dome[nCntDome].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �u���b�N�̏�����
		g_dome[nCntDome].nBlock_Depth = 1;
		g_dome[nCntDome].nBlock_Width = 20;

		// �e�N�X�`���E�C���f�b�N�X�E�o�b�t�@�̏�����
		g_dome[nCntDome].pIndexMeshField = NULL;
		g_dome[nCntDome].pVtxBuffDome = NULL;

		// �����_���E�C���f�b�N�X�E�h�[���̏�����(�v�Z)
		g_dome[nCntDome].nNumberVertexMeshField =
			(g_dome[nCntDome].nBlock_Depth + 1) *
			(g_dome[nCntDome].nBlock_Width + 1);

		g_dome[nCntDome].nNumIndex =
			(g_dome[nCntDome].nBlock_Width + 1) * 2 *
			g_dome[nCntDome].nBlock_Depth + 2 *
			(g_dome[nCntDome].nBlock_Depth - 1);

		g_dome[nCntDome].nNumPolygon =
			g_dome[nCntDome].nBlock_Depth *
			g_dome[nCntDome].nBlock_Width * 2 +
			4 * (g_dome[nCntDome].nBlock_Depth - 1);

		// �g�p���
		g_dome[nCntDome].bUse = false;
	}

	// ���_���̍쐬
	MakeVertexDome(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitDome(void)
{
	// �h�[���̕`��̏I������
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// �e�N�X�`���̊J��
		if (pTextureDome[g_dome[nCntDome].nType] != NULL)
		{
			pTextureDome[g_dome[nCntDome].nType]->Release();
			pTextureDome[g_dome[nCntDome].nType] = NULL;
		}

		// ���_�o�b�t�@�̊J��
		if (g_dome[nCntDome].pVtxBuffDome != NULL)
		{
			g_dome[nCntDome].pVtxBuffDome->Release();
			g_dome[nCntDome].pVtxBuffDome = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_dome[nCntDome].pIndexMeshField != NULL)
		{
			g_dome[nCntDome].pIndexMeshField->Release();
			g_dome[nCntDome].pIndexMeshField = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateDome(void)
{
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// �h�[���̍X�V
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// �g�p��Ԃ̂���
		if (g_dome[nCntDome].bUse == true)
		{
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_dome[nCntDome].pVtxBuffDome->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
				{
					// �e�N�X�`���[�̐ݒ�
					pVtx[0].tex = pVtx[0].tex +
						D3DXVECTOR2(
							1.0f / g_dome[nCntDome].nBlock_Width * 0.1f,
							0.0f);

					// �|�C���g���킹
					pVtx++;
				}
			}

			// �A�����b�N
			g_dome[nCntDome].pVtxBuffDome->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

												// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// �A���t�@�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ALPHAREF, int(255 * 0.1f));				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)

																// ���ʂ�`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �h�[���̕`��
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// �g�p��Ԃ̂���
		if (g_dome[nCntDome].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_dome[nCntDome].mtxWorldDome);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				g_dome[nCntDome].rot.y, g_dome[nCntDome].rot.x, g_dome[nCntDome].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_dome[nCntDome].mtxWorldDome,	// 1
				&g_dome[nCntDome].mtxWorldDome,					// 2
				&mtxRot);										// 3

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				g_dome[nCntDome].pos.x, g_dome[nCntDome].pos.y, g_dome[nCntDome].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_dome[nCntDome].mtxWorldDome,	// 1
				&g_dome[nCntDome].mtxWorldDome,					// 2
				&mtxTrans);							// 3

													// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_dome[nCntDome].mtxWorldDome);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_dome[nCntDome].pVtxBuffDome, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(g_dome[nCntDome].pIndexMeshField);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTextureDome[g_dome[nCntDome].nType]);

			// �h�[���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_dome[nCntDome].nNumberVertexMeshField, 0, g_dome[nCntDome].nNumPolygon);
		}
	}
	// ����(�����)�ŃJ�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �A���t�@�e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexDome(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;
	float fAngle;
	float fRadian;

	for (int nCntDomeType = 0; nCntDomeType < DOMETYPE_MAX; nCntDomeType++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			g_paTexDomeFile[nCntDomeType],
			&pTextureDome[nCntDomeType]);
	}

	// �h�[���̒��_���쐬
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// �p�x�̌v�Z
		fAngle = D3DX_PI * 2 / g_dome[nCntDome].nBlock_Width;

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_dome[nCntDome].nNumberVertexMeshField,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_dome[nCntDome].pVtxBuffDome,
			NULL);

		// �C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_dome[nCntDome].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_dome[nCntDome].pIndexMeshField,
			NULL);

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

							// �C���f�b�N�X���̐ݒ�
		WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

							// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		g_dome[nCntDome].pVtxBuffDome->Lock(0, 0, (void **)&pVtx, 0);

		//���_�ݒ� //
		//�s���[�v
		for (nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth + 1; nCountDirect++)
		{
			// �񃋁[�v
			for (nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
			{
				// ���W�A���l
				fRadian = fAngle * nCountWidth;

				// -3.14����
				if (fRadian < -D3DX_PI)
				{
					fRadian += D3DX_PI * 2;
				}

				// 3.14����
				else if (fRadian > D3DX_PI)
				{
					fRadian -= D3DX_PI * 2;
				}

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(
					sinf(fRadian) * g_dome[nCntDome].size.x,
					g_dome[nCntDome].size.y * nCountDirect,
					cosf(fRadian) * g_dome[nCntDome].size.z);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = pVtx[0].pos;
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				// �F�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

				// �e�N�X�`���[�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(
					1.0f / g_dome[nCntDome].nBlock_Width * nCountWidth,
					1.0f / g_dome[nCntDome].nBlock_Depth * (g_dome[nCntDome].nBlock_Depth - nCountDirect));

				// �|�C���g���킹
				pVtx++;
			}
		}

		// �A�����b�N
		g_dome[nCntDome].pVtxBuffDome->Unlock();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		g_dome[nCntDome].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

		// �c�u���b�N��
		for (nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth; nCountDirect++)
		{
			// �Q��ڂ̃��[�v�ȍ~
			if (nCountDirect >= 1)
			{
				// �k�ރh�[�����̒��_�ǉ�
				pIdx[0] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + g_dome[nCntDome].nBlock_Width + 1;

				// �C���f�b�N�X�̃|�C���g���킹
				pIdx++;
			}

			// ���u���b�N�̒��_��
			for (nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
			{
				// �`�揇�Ԃ̃C���f�b�N�X
				pIdx[0] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + nCountWidth + g_dome[nCntDome].nBlock_Width + 1;
				pIdx[1] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + nCountWidth;

				// �C���f�b�N�X�̃|�C���g���킹
				pIdx += 2;
			}

			// �k�ރh�[�������K�v������ꍇ
			if (nCountDirect < g_dome[nCntDome].nBlock_Depth - 1)
			{
				// �k�ރh�[�����̒��_�ǉ�
				pIdx[0] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + g_dome[nCntDome].nBlock_Width;

				// �C���f�b�N�X�̃|�C���g���킹
				pIdx++;
			}
		}

		// �A�����b�N
		g_dome[nCntDome].pIndexMeshField->Unlock();
	}
}

// ------------------------------------------------------------------------------------------
// �g�p���̂��̂�������
// ------------------------------------------------------------------------------------------
void UseInitDome(void)
{
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		if (g_dome[nCntDome].bUse == true)
		{
			g_dome[nCntDome].bUse = false;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �h�[���̐ݒu
// ------------------------------------------------------------------------------------------
int SetDome(
	D3DXVECTOR3 pos,
	D3DXCOLOR	col,
	int nType)
{
	// �ϐ��錾
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// �h�[���̐ݒu
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// ���g�p�̂��̂�I��
		if (g_dome[nCntDome].bUse == false)
		{
			g_dome[nCntDome].pos = pos;
			g_dome[nCntDome].nType = nType;
			g_dome[nCntDome].col = col;
			g_dome[nCntDome].bUse = true;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_dome[nCntDome].pVtxBuffDome->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (int nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (int nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
				{
					// �e�N�X�`���[�̐ݒ�
					pVtx[0].col = g_dome[nCntDome].col;

					// �|�C���g���킹
					pVtx++;
				}
			}

			// �A�����b�N
			g_dome[nCntDome].pVtxBuffDome->Unlock();

			return nCntDome;
		}
	}
	return 0;
}

// ------------------------------------------------------------------------------------------
// �폜
// ------------------------------------------------------------------------------------------
void DeleteDome(int nIdx)
{
	g_dome[nIdx].bUse = false;
}

// ------------------------------------------------------------------------------------------
// �h�[�����
// ------------------------------------------------------------------------------------------
DOME *GetDome(void)
{
	return &g_dome[0];
}