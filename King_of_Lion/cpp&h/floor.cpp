// ------------------------------------------------------------------------------------------
//
// ������ [floor.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "floor.h"

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
	FLOORTYPE_00_ROAD1 = 0,
	FLOORTYPE_01_ROAD2,
	FLOORTYPE_MAX
} FLOORTYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexFLOOR(LPDIRECT3DDEVICE9 pDevice);
void LoadFloor(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
FLOOR				g_floor[FLOOR_MAX];						// ��
LPDIRECT3DTEXTURE9	g_apTextureFloor[FLOORTYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitFLOOR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntFloor;

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[FLOORTYPE_MAX] =
	{
		{ "data/TEXTURE/mesh/00_road.jpg" },		// 00_��1
		{ "data/TEXTURE/mesh/00_greenfloor.jpg" },	// 01_���Q
	};

	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		// �ʒu�E��]�E�ړ��ʁE�T�C�Y�̏����ݒ�
		g_floor[nCntFloor].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].size = D3DXVECTOR3(500.0f, 0.0f, 500.0f);

		// �g�p��ԁE�\����Ԃ̏����ݒ�
		g_floor[nCntFloor].bUse = false;
		g_floor[nCntFloor].bDisp = false;

		// �u���b�N�̏����ݒ�
		g_floor[nCntFloor].nBlock_Depth = 2;
		g_floor[nCntFloor].nBlock_Width = 2;

		// �u���b�N�`��̌��_�̏����ݒ�
		g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
			g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
			0.0f,
			g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

		// �C���f�b�N�X�E�o�b�t�@�̏����ݒ�
		g_floor[nCntFloor].pIndexMeshField = NULL;
		g_floor[nCntFloor].pVtxBuffFLOOR = NULL;

		// �����_���E�C���f�b�N�X�E���̏����ݒ�(�v�Z)
		g_floor[nCntFloor].nNumberVertexMeshField = 
			(g_floor[nCntFloor].nBlock_Depth + 1) * (g_floor[nCntFloor].nBlock_Width + 1);
		g_floor[nCntFloor].nNumIndex = 
			(g_floor[nCntFloor].nBlock_Width + 1) * 2 * g_floor[nCntFloor].nBlock_Depth +
			2 * (g_floor[nCntFloor].nBlock_Depth - 1);
			g_floor[nCntFloor].nNumPolygon = 
				g_floor[nCntFloor].nBlock_Depth * g_floor[nCntFloor].nBlock_Width * 2 + 
			4 * (g_floor[nCntFloor].nBlock_Depth - 1);
	}

	for (int nCntTex = 0; nCntTex < FLOORTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntTex],
			&g_apTextureFloor[nCntTex]);
	}

	// ������ǂݍ���
	LoadFloor();

	// ���_���̍쐬
	MakeVertexFLOOR(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitFLOOR(void)
{
	for (int nCntTex = 0; nCntTex < FLOORTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̊J��
		if (g_apTextureFloor[nCntTex] != NULL)
		{
			g_apTextureFloor[nCntTex]->Release();
			g_apTextureFloor[nCntTex] = NULL;
		}
	}

	for (int nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_floor[nCntFloor].pVtxBuffFLOOR != NULL)
		{
			g_floor[nCntFloor].pVtxBuffFLOOR->Release();
			g_floor[nCntFloor].pVtxBuffFLOOR = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_floor[nCntFloor].pIndexMeshField != NULL)
		{
			g_floor[nCntFloor].pIndexMeshField->Release();
			g_floor[nCntFloor].pIndexMeshField = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateFLOOR(void)
{

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawFLOOR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == true)
		{

			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_floor[nCntFloor].mtxWorldFLOOR);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				g_floor[nCntFloor].rot.y, g_floor[nCntFloor].rot.x, g_floor[nCntFloor].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_floor[nCntFloor].mtxWorldFLOOR,	// 1
				&g_floor[nCntFloor].mtxWorldFLOOR,					// 2
				&mtxRot);							// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				g_floor[nCntFloor].pos.x, g_floor[nCntFloor].pos.y, g_floor[nCntFloor].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_floor[nCntFloor].mtxWorldFLOOR,	// 1
				&g_floor[nCntFloor].mtxWorldFLOOR,					// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_floor[nCntFloor].mtxWorldFLOOR);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_floor[nCntFloor].pVtxBuffFLOOR, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(
				g_floor[nCntFloor].pIndexMeshField);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(
				0, 
				g_apTextureFloor[g_floor[nCntFloor].nType]);

			if (g_floor[nCntFloor].bDisp == true)
			{
				// ���̕`��
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLESTRIP,
					0, 
					0, 
					g_floor[nCntFloor].nNumberVertexMeshField, 
					0, 
					g_floor[nCntFloor].nNumPolygon);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexFLOOR(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;
	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == true)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *
				g_floor[nCntFloor].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_floor[nCntFloor].pVtxBuffFLOOR,
				NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * 
				g_floor[nCntFloor].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_floor[nCntFloor].pIndexMeshField,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

			// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_floor[nCntFloor].pVtxBuffFLOOR->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_floor[nCntFloor].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_floor[nCntFloor].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_floor[nCntFloor].OriginBlock.x +
							(g_floor[nCntFloor].size.x * nCountWidth),
							0.0f,
							g_floor[nCntFloor].OriginBlock.z -
							(g_floor[nCntFloor].size.z * nCountDirect));

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

					// �J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);

					// �e�N�X�`���[�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// �|�C���g���킹
					pVtx++;
				}
			}

			// �A�����b�N
			g_floor[nCntFloor].pVtxBuffFLOOR->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_floor[nCntFloor].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_floor[nCntFloor].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ޏ����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_floor[nCntFloor].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + nCountWidth + g_floor[nCntFloor].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ޏ������K�v������ꍇ
				if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
				{
					// �k�ޏ����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_floor[nCntFloor].pIndexMeshField->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadFloor(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntFloor = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data/SAVE/TUTORIAL/floor.txt", "r");
	}

	// �{�X�p
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/floor.txt", "r");
	}

	// ����ȊO
	else
	{
		pFile = fopen("data/SAVE/floor.txt", "r");
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

				// ���f���Z�b�g��������
				if (strcmp(cHeadText, "FLOORSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_FLOORSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_floor[nCntFloor].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_floor[nCntFloor].pos.x, &g_floor[nCntFloor].pos.y, &g_floor[nCntFloor].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_floor[nCntFloor].rot.x, &g_floor[nCntFloor].rot.y, &g_floor[nCntFloor].rot.z);
						}

						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_floor[nCntFloor].nBlock_Depth);
						}

						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_floor[nCntFloor].nBlock_Width);
						}

						// x�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_floor[nCntFloor].size.x);
						}

						// y�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "ZSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_floor[nCntFloor].size.z);
						}

					}

					// �u���b�N�`��̌��_�̐ݒ�
					g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
						g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
						0.0f,
						g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

					// �����_���E�C���f�b�N�X�E���̐ݒ�(�v�Z)
					g_floor[nCntFloor].nNumberVertexMeshField = (g_floor[nCntFloor].nBlock_Depth + 1) * (g_floor[nCntFloor].nBlock_Width + 1);
					g_floor[nCntFloor].nNumIndex = (g_floor[nCntFloor].nBlock_Width + 1) * 2 * g_floor[nCntFloor].nBlock_Depth + 2 * (g_floor[nCntFloor].nBlock_Depth - 1);
					g_floor[nCntFloor].nNumPolygon = g_floor[nCntFloor].nBlock_Depth * g_floor[nCntFloor].nBlock_Width * 2 + 4 * (g_floor[nCntFloor].nBlock_Depth - 1);

					// �g�p��ԁE�\�����
					g_floor[nCntFloor].bUse = true;
					g_floor[nCntFloor].bDisp = true;

					// ���J�E���g�̍X�V
					nCntFloor++;
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
bool ColisionFLOOR(
	D3DXVECTOR3 *pos,			// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,		// �O��̃|�W�V����
	D3DXVECTOR3 *move,			// �ړ���
	D3DXVECTOR3 *size,			// �T�C�Y
	FLOOR		**ppFloorRet	// ���̃A�h���X���̃A�h���X
)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntFloor;			// �J�E���g��

	// �Ԓn��񂪂�������
	if (ppFloorRet != NULL)
	{
		*ppFloorRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		// �g�p���
		if (g_floor[nCntFloor].bUse == true)
		{
			// �u���b�N��X�͈�
			if (pos->x + size->x * 0.5f >
				g_floor[nCntFloor].pos.x - g_floor[nCntFloor].size.x * 0.5f * g_floor[nCntFloor].nBlock_Width&&
				pos->x - size->x * 0.5f <
				g_floor[nCntFloor].pos.x + g_floor[nCntFloor].size.x * 0.5f * g_floor[nCntFloor].nBlock_Width&&
				pos->z + size->z * 0.5f >
				g_floor[nCntFloor].pos.z - g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth&&
				pos->z - size->z * 0.5f <
				g_floor[nCntFloor].pos.z + g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth)
			{
				// �����蔻��(��)
				if (pos->y < g_floor[nCntFloor].pos.y &&
					posOld->y >= g_floor[nCntFloor].pos.y)
				{
					// �Ԓn�̒��g�̏��ɑ��
					*ppFloorRet = &g_floor[nCntFloor];

					// �߂荞��ł���
					bLand = true;

					// �u���b�N��̏��
					pos->y = g_floor[nCntFloor].pos.y;

					// �ړ��ʂ̏�����
					move->y = 0.0f;
				}

				// �����蔻��(��)
				if (pos->y + size->y > g_floor[nCntFloor].pos.y &&
					posOld->y + size->y <= g_floor[nCntFloor].pos.y)
				{
					// �u���b�N��̉���
					pos->y = g_floor[nCntFloor].pos.y - size->y;

					// �ړ��ʂ̏�����
					move->y = 0.0f;
				}

			}
		}
	}
	return bLand;
}

// ------------------------------------------------------------------------------------------
// ���̏��
// ------------------------------------------------------------------------------------------
FLOOR *GetFLOOR(void)
{
	return &g_floor[0];
}