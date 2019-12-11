      // ------------------------------------------------------------------------------------------
//
// ������ [water.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "water.h"

#include "input.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	WATERTYPE_02_WATER = 0,
	WATERTYPE_MAX
} WATERTYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexWATER(LPDIRECT3DDEVICE9 pDevice);
void LoadWater(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
WATER				g_water[WATER_MAX];						// ��
LPDIRECT3DTEXTURE9	g_apTextureWater[WATERTYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitWATER(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntWater;

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[WATERTYPE_MAX] =
	{
		{ "data/TEXTURE/mesh/02_water3.jpg" },		// 02_��
	};

	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		// �ʒu�E��]�E�ړ��ʁE�T�C�Y�̏����ݒ�
		g_water[nCntWater].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_water[nCntWater].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_water[nCntWater].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_water[nCntWater].size = D3DXVECTOR3(500.0f, 0.0f, 500.0f);

		// �g�p��ԁE�\����Ԃ̏����ݒ�
		g_water[nCntWater].bUse = false;
		g_water[nCntWater].bDisp = false;

		// �u���b�N�̏����ݒ�
		g_water[nCntWater].nBlock_Depth = 2;
		g_water[nCntWater].nBlock_Width = 2;

		// �u���b�N�`��̌��_�̏����ݒ�
		g_water[nCntWater].OriginBlock = D3DXVECTOR3(
			g_water[nCntWater].size.x * -0.5f * g_water[nCntWater].nBlock_Width,
			0.0f,
			g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth);

		// �C���f�b�N�X�E�o�b�t�@�̏����ݒ�
		g_water[nCntWater].pIndexMeshField = NULL;
		g_water[nCntWater].pVtxBuffWATER = NULL;

		// �����_���E�C���f�b�N�X�E���̏����ݒ�(�v�Z)
		g_water[nCntWater].nNumberVertexMeshField = 
			(g_water[nCntWater].nBlock_Depth + 1) * (g_water[nCntWater].nBlock_Width + 1);
		g_water[nCntWater].nNumIndex = 
			(g_water[nCntWater].nBlock_Width + 1) * 2 * g_water[nCntWater].nBlock_Depth +
			2 * (g_water[nCntWater].nBlock_Depth - 1);
			g_water[nCntWater].nNumPolygon = 
				g_water[nCntWater].nBlock_Depth * g_water[nCntWater].nBlock_Width * 2 + 
			4 * (g_water[nCntWater].nBlock_Depth - 1);
	}

	// �e�N�X�`���[
	for (int nCntTex = 0; nCntTex < WATERTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntTex],
			&g_apTextureWater[nCntTex]);
	}

	// ������ǂݍ���
	LoadWater();

	// ���_���̍쐬
	MakeVertexWATER(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitWATER(void)
{
	for (int nCntTex = 0; nCntTex < WATERTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̊J��
		if (g_apTextureWater[nCntTex] != NULL)
		{
			g_apTextureWater[nCntTex]->Release();
			g_apTextureWater[nCntTex] = NULL;
		}
	}

	for (int nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_water[nCntWater].pVtxBuffWATER != NULL)
		{
			g_water[nCntWater].pVtxBuffWATER->Release();
			g_water[nCntWater].pVtxBuffWATER = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_water[nCntWater].pIndexMeshField != NULL)
		{
			g_water[nCntWater].pIndexMeshField->Release();
			g_water[nCntWater].pIndexMeshField = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateWATER(void)
{
	for (int nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		if (g_water[nCntWater].bUse == true)
		{
			if (g_water[nCntWater].nType == WATERTYPE_02_WATER)
			{
				// ���_���̐ݒ�
				VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_water[nCntWater].pVtxBuffWATER->Lock(0, 0, (void **)&pVtx, 0);

				//���_�ݒ� //
				//�s���[�v
				for (int nCountDirect = 0; nCountDirect < g_water[nCntWater].nBlock_Depth + 1; nCountDirect++)
				{
					// �񃋁[�v
					for (int nCountWidth = 0; nCountWidth < g_water[nCntWater].nBlock_Width + 1; nCountWidth++)
					{
						// �e�N�X�`���[�̐ݒ�
						pVtx[0].tex += D3DXVECTOR2(0.0f, 0.001f);

						// �|�C���g���킹
						pVtx++;
					}
				}

				// �A�����b�N
				g_water[nCntWater].pIndexMeshField->Unlock();
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawWATER(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntWater;

	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		if (g_water[nCntWater].bUse == true)
		{

			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_water[nCntWater].mtxWorldWATER);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				g_water[nCntWater].rot.y, g_water[nCntWater].rot.x, g_water[nCntWater].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_water[nCntWater].mtxWorldWATER,	// 1
				&g_water[nCntWater].mtxWorldWATER,					// 2
				&mtxRot);							// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				g_water[nCntWater].pos.x, g_water[nCntWater].pos.y, g_water[nCntWater].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_water[nCntWater].mtxWorldWATER,	// 1
				&g_water[nCntWater].mtxWorldWATER,					// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_water[nCntWater].mtxWorldWATER);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_water[nCntWater].pVtxBuffWATER, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(
				g_water[nCntWater].pIndexMeshField);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(
				0, 
				g_apTextureWater[g_water[nCntWater].nType]);

			if (g_water[nCntWater].bDisp == true)
			{
				// ���̕`��
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLESTRIP,
					0,
					0, 
					g_water[nCntWater].nNumberVertexMeshField,
					0, 
					g_water[nCntWater].nNumPolygon);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexWATER(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;
	int nCntWater;

	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		if (g_water[nCntWater].bUse == true)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *
				g_water[nCntWater].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_water[nCntWater].pVtxBuffWATER,
				NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * 
				g_water[nCntWater].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_water[nCntWater].pIndexMeshField,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

			// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_water[nCntWater].pVtxBuffWATER->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_water[nCntWater].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_water[nCntWater].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_water[nCntWater].OriginBlock.x +
							(g_water[nCntWater].size.x * nCountWidth),
							0.0f,
							g_water[nCntWater].OriginBlock.z -
							(g_water[nCntWater].size.z * nCountDirect));

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

					// �J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,0.5f);

					// �e�N�X�`���[�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// �|�C���g���킹
					pVtx++;
				}
			}

			// �A�����b�N
			g_water[nCntWater].pVtxBuffWATER->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_water[nCntWater].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_water[nCntWater].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ސ����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + g_water[nCntWater].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_water[nCntWater].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + nCountWidth + g_water[nCntWater].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ސ������K�v������ꍇ
				if (nCountDirect < g_water[nCntWater].nBlock_Depth - 1)
				{
					// �k�ސ����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + g_water[nCntWater].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_water[nCntWater].pIndexMeshField->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadWater(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntWater = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �`���[�g���A���̏ꍇ
	if (GetMode() == MODE_TUTORIAL)
	{
		return;
	}

	pFile = fopen("data/SAVE/water.txt", "r");

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

				// ���f���Z�b�g��������
				if (strcmp(cHeadText, "WATERSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_WATERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_water[nCntWater].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_water[nCntWater].pos.x, &g_water[nCntWater].pos.y, &g_water[nCntWater].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_water[nCntWater].rot.x, &g_water[nCntWater].rot.y, &g_water[nCntWater].rot.z);
						}

						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_water[nCntWater].nBlock_Depth);
						}

						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_water[nCntWater].nBlock_Width);
						}

						// x�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_water[nCntWater].size.x);
						}

						// y�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "ZSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_water[nCntWater].size.z);
						}

					}

					// �u���b�N�`��̌��_�̐ݒ�
					g_water[nCntWater].OriginBlock = D3DXVECTOR3(
						g_water[nCntWater].size.x * -0.5f * g_water[nCntWater].nBlock_Width,
						0.0f,
						g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth);

					// �����_���E�C���f�b�N�X�E���̐ݒ�(�v�Z)
					g_water[nCntWater].nNumberVertexMeshField = (g_water[nCntWater].nBlock_Depth + 1) * (g_water[nCntWater].nBlock_Width + 1);
					g_water[nCntWater].nNumIndex = (g_water[nCntWater].nBlock_Width + 1) * 2 * g_water[nCntWater].nBlock_Depth + 2 * (g_water[nCntWater].nBlock_Depth - 1);
					g_water[nCntWater].nNumPolygon = g_water[nCntWater].nBlock_Depth * g_water[nCntWater].nBlock_Width * 2 + 4 * (g_water[nCntWater].nBlock_Depth - 1);

					// �g�p��ԁE�\�����
					g_water[nCntWater].bUse = true;
					g_water[nCntWater].bDisp = true;

					// ���J�E���g�̍X�V
					nCntWater++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
bool ColisionWATER(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, WATER ** ppWaterRet)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntWater;			// �J�E���g��

	// �Ԓn��񂪂�������
	if (ppWaterRet != NULL)
	{
		*ppWaterRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		// �g�p���
		if (g_water[nCntWater].bUse == true)
		{
			// �u���b�N��X�͈�
			if (pos->x + size->x * 0.5f >
				g_water[nCntWater].pos.x - g_water[nCntWater].size.x * 0.5f * g_water[nCntWater].nBlock_Width&&
				pos->x - size->x * 0.5f <
				g_water[nCntWater].pos.x + g_water[nCntWater].size.x * 0.5f * g_water[nCntWater].nBlock_Width&&
				pos->z + size->z * 0.5f >
				g_water[nCntWater].pos.z - g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth&&
				pos->z - size->z * 0.5f <
				g_water[nCntWater].pos.z + g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth)
			{
				// ��
				if (g_water[nCntWater].nType == WATERTYPE_02_WATER)
				{
					// �����蔻��(��)
					if (pos->y < g_water[nCntWater].pos.y)
					{
						move->y = 0;
						//move->x = 0;
						move->z += 1.1f;
						bLand = true;
					}

				}
			}
		}
	}
	return bLand;
}

// ------------------------------------------------------------------------------------------
// ���̏��
// ------------------------------------------------------------------------------------------
WATER *GetWATER(void)
{
	return &g_water[0];
}