// ------------------------------------------------------------------------------------------
//
// �Ǐ��� [wall.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "wall.h"

#include "input.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define WALL_PHONT "data/TEXTURE/woods.png"

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet);

void LoadWall(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	// ���_�e�N�X�`���[�̃|�C���^

WALL g_awall[WALL_MAX];							// ��

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		g_awall[nCntWall].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_awall[nCntWall].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_awall[nCntWall].nBlock_Depth	= 2;
		g_awall[nCntWall].nBlock_Width	= 2;

		// ���_�̌v�Z
		g_awall[nCntWall].OriginBlock	= D3DXVECTOR3(
			g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
			g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
			0.0f);

		// �����_�̌v�Z
		g_awall[nCntWall].nNumberVertexMeshField = 
			(g_awall[nCntWall].nBlock_Depth + 1) *
			(g_awall[nCntWall].nBlock_Width + 1);

		// �C���f�b�N�X�̌v�Z
		g_awall[nCntWall].nNumIndex = 
			(g_awall[nCntWall].nBlock_Width + 1) * 2 *
			g_awall[nCntWall].nBlock_Depth + 
			2 * (g_awall[nCntWall].nBlock_Depth - 1);

		// �|���S���̌v�Z
		g_awall[nCntWall].nNumPolygon = 
			g_awall[nCntWall].nBlock_Depth * 
			g_awall[nCntWall].nBlock_Width * 2 + 
			4 * (g_awall[nCntWall].nBlock_Depth - 1);

		g_awall[nCntWall].nNumber = 0;

		// �g�p�E�\����Ԃ̏�����
		g_awall[nCntWall].bUse = false;
		g_awall[nCntWall].bDisp = false;


		// �����蔻��p // 
		// ���_
		g_awall[nCntWall].BiginPos = g_awall[nCntWall].OriginBlock;

		// �ŏI�n�_
		g_awall[nCntWall].LastPos =
			g_awall[nCntWall].OriginBlock * (float)g_awall[nCntWall].nBlock_Width;

		// �ǂ̒���
		g_awall[nCntWall].Length = g_awall[nCntWall].LastPos - g_awall[nCntWall].BiginPos;

		// ����
		g_awall[nCntWall].fdistance =
			sqrtf(g_awall[nCntWall].Length.x * g_awall[nCntWall].Length.x +
				g_awall[nCntWall].Length.z * g_awall[nCntWall].Length.z);

		//// ���K�����ăx�N�g�������߂�
		//D3DXVec3Normalize(&g_awall[nCntWall].VecWall, &g_awall[nCntWall].Length);
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, WALL_PHONT, &g_pTextureWall);

	// �ǂ̃f�[�^�Ǎ�
	LoadWall();

	// ���_���̍쐬
	MakeVertexWall(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitWall(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_awall[nCntWall].pVtxBuff != NULL)
		{
			g_awall[nCntWall].pVtxBuff->Release();
			g_awall[nCntWall].pVtxBuff = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_awall[nCntWall].pIndex != NULL)
		{
			g_awall[nCntWall].pIndex->Release();
			g_awall[nCntWall].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateWall(void)
{

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// �J�E���g��
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_awall[nCntWall].mtxWorldWall);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// �����̓��ꕨ
				g_awall[nCntWall].rot.y, g_awall[nCntWall].rot.x, g_awall[nCntWall].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorldWall,	// 1
				&g_awall[nCntWall].mtxWorldWall,				// 2
				&mtxRot);							// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,												// �����̓��ꕨ
				g_awall[nCntWall].pos.x, g_awall[nCntWall].pos.y, g_awall[nCntWall].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorldWall,	// 1
				&g_awall[nCntWall].mtxWorldWall,				// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_awall[nCntWall].mtxWorldWall);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_awall[nCntWall].pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(g_awall[nCntWall].pIndex);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_awall[nCntWall].nNumberVertexMeshField, 0, g_awall[nCntWall].nNumPolygon);
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCntWall;
	int nCountDirect;
	int nCountWidth;

	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_awall[nCntWall].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

			// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_awall[nCntWall].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_awall[nCntWall].OriginBlock.x + (g_awall[nCntWall].size.x * nCountWidth),
							g_awall[nCntWall].OriginBlock.y - (g_awall[nCntWall].size.y * nCountDirect),
							0.0f);

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// �F�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`���[�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// �|�C���g���킹
					pVtx++;
				}
			}
			// �A�����b�N
			g_awall[nCntWall].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_awall[nCntWall].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth + g_awall[nCntWall].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_awall[nCntWall].nBlock_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_awall[nCntWall].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// �ǂ̓����蔻����
// ------------------------------------------------------------------------------------------
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntWall;			// �J�E���g��

	// �x�N�g���̓����蔻��
	D3DXVECTOR3 PlayerOldVec;	// �O��̃v���C���[�̃u���b�N�̎n�_����̃x�N�g��
	D3DXVECTOR3 PlayerVec;		// �v���C���[�̃u���b�N�̎n�_����̃x�N�g��

	//// �u���b�N���[�v
	//for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	//{
	//	// �g�p���
	//	if (g_awall[nCntWall].bUse == true)
	//	{
	//		// �O��̃v���C���[�̃x�N�g��
	//		PlayerOldVec = D3DXVECTOR3(
	//			posOld->x - g_awall[nCntWall].pos.x,
	//			posOld->y - g_awall[nCntWall].pos.y,
	//			posOld->z - g_awall[nCntWall].pos.z);

	//		// 
	//		// �v���C���[�̃x�N�g��
	//		PlayerVec = D3DXVECTOR3(
	//			pos->x - g_awall[nCntWall].pos.x,
	//			pos->y - g_awall[nCntWall].pos.y,
	//			pos->z - g_awall[nCntWall].pos.z);

	//		//// ���K�����ăx�N�g�������߂�
	//		//D3DXVec3Normalize(&PlayerVec, &PlayerVec);

	//		//// ���K�����ăx�N�g�������߂�
	//		//D3DXVec3Normalize(&PlayerOldVec, &PlayerOldVec);

	//		// �u���b�N��Y�͈�
	//		if (pos->y - size->y < g_awall[nCntWall].pos.y + g_awall[nCntWall].size.y &&
	//			pos->y + size->y > g_awall[nCntWall].pos.y)
	//		{
	//			// �O��(�O��-,����+)
	//			if ((PlayerOldVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerOldVec.x * g_awall[nCntWall].Length.z) < 0 &&	// �O��̊O�ς�-
	//				(PlayerVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerVec.x * g_awall[nCntWall].Length.z) > 0)			// ���݂̊O�ς�+
	//			{
	//				// �u���b�N��̍���
	//				pos->x = 0.0f;

	//				// �u���b�N��̍���
	//				pos->y = 0.0f;

	//				// �u���b�N��̍���
	//				pos->z = 0.0f;
	//			}

	//			// �O��(�O��+,����-)
	//			else if ((PlayerOldVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerOldVec.x * g_awall[nCntWall].Length.z) > 0 &&	// �O��̊O�ς�+
	//				(PlayerVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerVec.x * g_awall[nCntWall].Length.z) < 0)			// ���݂̊O�ς�-
	//			{
	//				// �u���b�N��̍���
	//				pos->x = 0.0f;

	//				// �u���b�N��̍���
 //					pos->y = 0.0f;

	//				// �u���b�N��̍���
	//				pos->z = 0.0f;

	//			}
	//		}
	//	}
	//}





	// �Ԓn��񂪂�������
	if (ppWallRet != NULL)
	{
		*ppWallRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		// �g�p���
		if (g_awall[nCntWall].bUse == true)
		{
			// �u���b�N��Y�͈�
			if (pos->y - size->y < g_awall[nCntWall].pos.y + g_awall[nCntWall].size.y &&
				pos->y + size->y > g_awall[nCntWall].pos.y)
			{
				// �u���b�N��Z�͈�
				if (pos->z + size->z > 
					g_awall[nCntWall].pos.z -
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width *sinf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < 
					g_awall[nCntWall].pos.z +
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width *sinf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��)
					if (pos->x + size->z >
						g_awall[nCntWall].pos.x&&
						posOld->x + size->z <=
						g_awall[nCntWall].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppWallRet = &g_awall[nCntWall];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_awall[nCntWall].pos.x - size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

					// �u���b�N��Z�͈�
				if (pos->z + size->z > 
					g_awall[nCntWall].pos.z + 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * sinf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z <
					g_awall[nCntWall].pos.z -
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * sinf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(�E)
					if (pos->x - size->z <
						g_awall[nCntWall].pos.x&&
						posOld->x - size->z >=
						g_awall[nCntWall].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppWallRet = &g_awall[nCntWall];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_awall[nCntWall].pos.x + size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

				// �u���b�N��X�͈�
				if (pos->x + size->z >
					g_awall[nCntWall].pos.x - 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * cosf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z <
					g_awall[nCntWall].pos.x + 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width*cosf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��O)
					if (pos->z + size->z > g_awall[nCntWall].pos.z&&
						posOld->z + size->z <= g_awall[nCntWall].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppWallRet = &g_awall[nCntWall];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_awall[nCntWall].pos.z - size->z;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}
				}

					// �u���b�N��X�͈�
				if (pos->x + size->z > 
					g_awall[nCntWall].pos.x + 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * cosf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < 
					g_awall[nCntWall].pos.x - 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * cosf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(��)
					if (pos->z - size->z < g_awall[nCntWall].pos.z&&
						posOld->z - size->z >= g_awall[nCntWall].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppWallRet = &g_awall[nCntWall];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_awall[nCntWall].pos.z + size->z;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}
				}
			}
		}
	}

	return bLand;
}

// ------------------------------------------------------------------------------------------
// �Ǔǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadWall(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntWall = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data/SAVE/TUTORIAL/wall.txt", "r");
	}

	// �{�X�p
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/wall.txt", "r");
	}

	// ����ȊO
	else
	{
		pFile = fopen("data/SAVE/wall.txt", "r");
	}

	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
		}

		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// ���s��������
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// ���f���Z�b�g��������
				else if (strcmp(cHeadText, "WALLSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_WALLSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_awall[nCntWall].pos.x, &g_awall[nCntWall].pos.y, &g_awall[nCntWall].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_awall[nCntWall].rot.x, &g_awall[nCntWall].rot.y, &g_awall[nCntWall].rot.z);
						}

						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nBlock_Depth);
						}

						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nBlock_Width);
						}

						// x�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_awall[nCntWall].size.x);
						}

						// y�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "YSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_awall[nCntWall].size.y);
						}

					}

					// ���_�̌v�Z
					g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
						g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
						g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
						0.0f);

					// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
					g_awall[nCntWall].nNumberVertexMeshField = (g_awall[nCntWall].nBlock_Depth + 1) * (g_awall[nCntWall].nBlock_Width + 1);
					g_awall[nCntWall].nNumIndex = (g_awall[nCntWall].nBlock_Width + 1) * 2 * g_awall[nCntWall].nBlock_Depth + 2 * (g_awall[nCntWall].nBlock_Depth - 1);
					g_awall[nCntWall].nNumPolygon = g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 + 4 * (g_awall[nCntWall].nBlock_Depth - 1);

					// �g�p��ԁE�\�����
					g_awall[nCntWall].bUse = true;
					g_awall[nCntWall].bDisp = true;


					// �����蔻��p // 
					// ���_
					g_awall[nCntWall].BiginPos = g_awall[nCntWall].OriginBlock;

					// �ŏI�n�_
					g_awall[nCntWall].LastPos =
						g_awall[nCntWall].OriginBlock * (float)g_awall[nCntWall].nBlock_Width;

					// �ǂ̒���
					g_awall[nCntWall].Length = g_awall[nCntWall].LastPos - g_awall[nCntWall].BiginPos;

					// ����
					g_awall[nCntWall].fdistance =
						sqrtf(g_awall[nCntWall].Length.x * g_awall[nCntWall].Length.x +
							g_awall[nCntWall].Length.z * g_awall[nCntWall].Length.z);

					//// ���K�����ăx�N�g�������߂�
					//D3DXVec3Normalize(&g_awall[nCntWall].VecWall, &g_awall[nCntWall].Length);

					// ���J�E���g�̍X�V
					nCntWall++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�ǃf�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// �Ǐ��
// ------------------------------------------------------------------------------------------
WALL *GetWall(void)
{
	return &g_awall[0];
}