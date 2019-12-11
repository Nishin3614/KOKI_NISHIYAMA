      // ------------------------------------------------------------------------------------------
//
// �|���S������ [floor.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "floor.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "arrangement.h"
#include "game.h"
#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG


// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define FLOOR_PHONT "data/TEXTURE/soil.jpg"	// ���̎ʐ^

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	FLOORTYPE_00_ROAD1 = 0,
	FLOORTYPE_01_ROAD2,
	FLOORTYPE_02_WATER,
	FLOORTYPE_MAX
} FLOORTYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexFloor(LPDIRECT3DDEVICE9 pDevice);
void Update_ChangeFloor(void);
void Update_DispFloor(void);
#ifdef _DEBUG			
// ���̏��
void Debug_Floor(void);
#endif


// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
FLOOR g_floor[ALL_MAX];						// �|���S��
FLOORSET g_FloorSet;											// �z�u���
LPDIRECT3DTEXTURE9		g_apTextureFloor[FLOORTYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^

// 3D�p�̃Z�[�u�t�@�C��
const char g_cFloorFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/floor.txt",
	"data/SAVE/3D/Stage_2/floor.txt",
	"data/SAVE/3D/Stage_3/floor.txt",
	"data/SAVE/3D/Stage_4/floor.txt",
	"data/SAVE/3D/Stage_5/floor.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntFloor;

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[FLOORTYPE_MAX] =
	{
		{ "data/TEXTURE/mesh/00_road.jpg" },			// 00_��1
		{ "data/TEXTURE/mesh/01_Savannah_field.jpg" },	// 01_���Q
		{ "data/TEXTURE/soil.jpg" },					// 02_��
	};

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// �ʒu�E��]�E�ړ��ʁE�T�C�Y�̏����ݒ�
		g_floor[nCntFloor].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

		// �g�p��ԁE�\����Ԃ̏����ݒ�
		g_floor[nCntFloor].bUse = false;
		g_floor[nCntFloor].bDisp = false;
		g_floor[nCntFloor].bSelect = false;

		// �u���b�N�̏����ݒ�
		g_floor[nCntFloor].nBlock_Depth = 1;
		g_floor[nCntFloor].nBlock_Width = 1;

		// �u���b�N�`��̌��_�̐ݒ�
		g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
			g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
			0.0f,
			g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

		// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
		g_floor[nCntFloor].nNumberVertexMeshField =
			(g_floor[nCntFloor].nBlock_Depth + 1) *
			(g_floor[nCntFloor].nBlock_Width + 1);
		g_floor[nCntFloor].nNumIndex =
			(g_floor[nCntFloor].nBlock_Width + 1) * 2
			* g_floor[nCntFloor].nBlock_Depth +
			2 * (g_floor[nCntFloor].nBlock_Depth - 1);
		g_floor[nCntFloor].nNumPolygon =
			g_floor[nCntFloor].nBlock_Depth *
			g_floor[nCntFloor].nBlock_Width * 2 +
			4 * (g_floor[nCntFloor].nBlock_Depth - 1);

		// �C���f�b�N�X�E�o�b�t�@�̏����ݒ�
		g_floor[nCntFloor].pIndexMeshField = NULL;
		g_floor[nCntFloor].pVtxBuffFLOOR = NULL;
	}

	// �z�u���̏�����
	g_FloorSet.nBlock_Depth = 1;
	g_FloorSet.nBlock_Width = 1;
	g_FloorSet.nType = 0;
	g_FloorSet.size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

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
	MakeVertexFloor(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitFloor(void)
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

	for (int nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
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
void UpdateFloor(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// ���̕ω�����
	Update_ChangeFloor();

	// �z�u���̏ꏊ�c��
	Update_DispFloor();

	// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
	if (GetMousePressTrigger(0))
	{
		SetFloor(
			pArrangement->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			g_FloorSet.size,
			g_FloorSet.nBlock_Depth,
			g_FloorSet.nBlock_Width,
			g_FloorSet.nType);
	}

	// �Z�[�u
	if (GetKeyboardSpeacePress(DIK_F6))
	{
		SaveFloor();
	}
#ifdef _DEBUG	
	if (GetGameSelect() == GAMESELECT_FLOOR)
	{
		// ���̏��\��
		Debug_Floor();
	}
#endif
}

// ------------------------------------------------------------------------------------------
// ���̕ω�����
// ------------------------------------------------------------------------------------------
void Update_ChangeFloor(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �c�̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_FloorSet.nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_FloorSet.nBlock_Depth--;
	}

	// ���̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_FloorSet.nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_FloorSet.nBlock_Width--;
	}

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_FloorSet.size.z += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_FloorSet.size.z -= 50.0f;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_FloorSet.size.x += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_FloorSet.size.x -= 50.0f;
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_FloorSet.nType++;

		// ���E�l�܂ōs������
		if (g_FloorSet.nType >= FLOORTYPE_MAX)
		{
			// ������
			g_FloorSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�̎��̏ꏊ�c������
// ------------------------------------------------------------------------------------------
void Update_DispFloor(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;

	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == false)
		{
			g_floor[nCntFloor].nType = g_FloorSet.nType;

			if (g_floor[nCntFloor].nBlock_Depth != g_FloorSet.nBlock_Depth ||
				g_floor[nCntFloor].nBlock_Width != g_FloorSet.nBlock_Width ||
				g_floor[nCntFloor].size != g_FloorSet.size)
			{
				g_floor[nCntFloor].nBlock_Depth = g_FloorSet.nBlock_Depth;
				g_floor[nCntFloor].nBlock_Width = g_FloorSet.nBlock_Width;
				g_floor[nCntFloor].size = g_FloorSet.size;

				// �u���b�N�`��̌��_�̐ݒ�
				g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
					g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
					0.0f,
					g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

				// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
				g_floor[nCntFloor].nNumberVertexMeshField =
					(g_floor[nCntFloor].nBlock_Depth + 1) *
					(g_floor[nCntFloor].nBlock_Width + 1);
				g_floor[nCntFloor].nNumIndex =
					(g_floor[nCntFloor].nBlock_Width + 1) * 2
					* g_floor[nCntFloor].nBlock_Depth +
					2 * (g_floor[nCntFloor].nBlock_Depth - 1);
				g_floor[nCntFloor].nNumPolygon =
					g_floor[nCntFloor].nBlock_Depth *
					g_floor[nCntFloor].nBlock_Width * 2 +
					4 * (g_floor[nCntFloor].nBlock_Depth - 1);

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_floor[nCntFloor].pVtxBuffFLOOR,
					NULL);

				// �C���f�b�N�X�o�b�t�@�̐���
				pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
								g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
								0.0f,
								g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

						// �@���x�N�g���̐ݒ�
						pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

						if (g_floor[nCntFloor].bUse == false)
						{
							// �F�̐ݒ�
							pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
						}

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
						// �k�ރ|���S�����̒��_�ǉ�
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

					// �k�ރ|���S�������K�v������ꍇ
					if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}
				}

				// �A�����b�N
				g_floor[nCntFloor].pIndexMeshField->Unlock();
			}

			g_floor[nCntFloor].bDisp = true;
			// �z�u�̈ʒu�����擾
			g_floor[nCntFloor].pos = pArrangement->pos;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bDisp == true)
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
			pDevice->SetIndices(g_floor[nCntFloor].pIndexMeshField);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(
				0,
				g_apTextureFloor[g_floor[nCntFloor].nType]);

			// �|���S���̕`��
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

// ------------------------------------------------------------------------------------------
// ���̐ݒu
// ------------------------------------------------------------------------------------------
void SetFloor(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	int nCntFloor;		// �J�E���g��
	int nCountDirect;	// �c�u���b�N�̃J�E���g
	int nCountWidth;	// ���u���b�N�̃J�E���g

	// ���̐ݒu������
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// ���g�p�̏�
		if (g_floor[nCntFloor].bUse == false)
		{
			// ����ϐ��ɑ��
			g_floor[nCntFloor].pos = pos;
			g_floor[nCntFloor].rot = rot;
			g_floor[nCntFloor].size = size;
			g_floor[nCntFloor].nBlock_Depth = nBlock_Depth;
			g_floor[nCntFloor].nBlock_Width = nBlock_Width;
			g_floor[nCntFloor].nType = nType;

			// �u���b�N�`��̌��_�̐ݒ�
			g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
				g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
				0.0f,
				g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

			// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
			g_floor[nCntFloor].nNumberVertexMeshField =
				(g_floor[nCntFloor].nBlock_Depth + 1) *
				(g_floor[nCntFloor].nBlock_Width + 1);
			g_floor[nCntFloor].nNumIndex =
				(g_floor[nCntFloor].nBlock_Width + 1) * 2
				* g_floor[nCntFloor].nBlock_Depth +
				2 * (g_floor[nCntFloor].nBlock_Depth - 1);
			g_floor[nCntFloor].nNumPolygon =
				g_floor[nCntFloor].nBlock_Depth *
				g_floor[nCntFloor].nBlock_Width * 2 +
				4 * (g_floor[nCntFloor].nBlock_Depth - 1);

			// �g�p��ԁE�\�����
			g_floor[nCntFloor].bUse = true;
			g_floor[nCntFloor].bDisp = true;

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_floor[nCntFloor].pVtxBuffFLOOR,
				NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
							g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
							0.0f,
							g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// �F�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
					// �k�ރ|���S�����̒��_�ǉ�
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

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_floor[nCntFloor].pIndexMeshField->Unlock();

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexFloor(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCountDirect;
	int nCountWidth;
	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_floor[nCntFloor].pVtxBuffFLOOR,
			NULL);

		// �C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
						g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
						0.0f,
						g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				if (g_floor[nCntFloor].bUse == true)
				{
					// �F�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				else
				{
					// �F�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				}

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
				// �k�ރ|���S�����̒��_�ǉ�
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

			// �k�ރ|���S�������K�v������ꍇ
			if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
			{
				// �k�ރ|���S�����̒��_�ǉ�
				pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

				// �C���f�b�N�X�̃|�C���g���킹
				pIdx++;
			}
		}

		// �A�����b�N
		g_floor[nCntFloor].pIndexMeshField->Unlock();
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

	// �����\�����Ă�����̂���������
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == true)
		{
			g_floor[nCntFloor].bUse = false;
			g_floor[nCntFloor].bDisp = false;
		}
	}

	// �J�E���g������
	nCntFloor = 0;

	// �t�@�C���J
	pFile = fopen(g_cFloorFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "FLOORSET") == 0)
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

					// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
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
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// �z�u�ꏊ���킩�邽�߂ɕ`�悵�Ă������̂��\���ɏ���
// ------------------------------------------------------------------------------------------
void NotDispFLoor(void)
{
	int nCntFloor;

	// ���̃J�E���g
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// �z�u�p�̏�
		if (g_floor[nCntFloor].bUse == false &&
			g_floor[nCntFloor].bDisp == true)
		{
			g_floor[nCntFloor].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u����
// ------------------------------------------------------------------------------------------
void SaveFloor(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntFloor = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// �o�O�΍�J�E���g
	char	cWriteText[512];		// �����Ƃ��ď������ݗp
	char	cHeadText[128];			// �����̊i�[
	char	cEqual[8] = { "=" };	// �C�R�[���p

	char	cSharpEnter[8];			// �R�����g�P
	char	cComment[64];			// �R�����g�Q
	char	cEnter[8];				// �R�����g�R
	char	cFilename[128];			// �t�@�C���l�[��

	// �悭�g���镶��
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// ���̎�ގ��
	char *cFloorType[FLOORTYPE_MAX] = 
	{
		{ "    ----------[00]:���P----------" },				// 00_���P
		{ "    ----------[01]:���Q----------" },				// 01_���Q
		{ "    ----------[02]:�΂P----------" },				// 00_�΂P
	};

	// ���f�[�^�̎��
	char *cFloorData[] =
	{
		{ "TYPE"},			// �^�C�v
		{ "POS" },			// �ʒu
		{ "ROT" },			// ��]
		{ "BLOCK_DEPTH" },	// �c�u���b�N
		{ "BLOCK_WIDTH" },	// ���u���b�N
		{ "XSIZE" },		// �w�T�C�Y
		{ "ZSIZE" },		// Z�T�C�Y
		{ "END_FLOORSET" }	// �G���h�t���A�[
	};
	strcpy(cFilename, FILENAME_DATE);
	strcat(cFilename, FILENAME_FORMAT);

	// �t�@�C���J
	pFile = fopen(g_cFloorFIleName_3D[GetGameStage()], "w");

	// �J����
	if (pFile != NULL)
	{
		//------------------ �R�����g ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# �Z�[�u���f��\n");

		strcat(cWriteText, "# Author : koki nishiyama\n");

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, cComment);

		strcat(cWriteText, cEnter);

		fputs(cWriteText, pFile);

		//------------------ �R�����g�I�� ------------------//

		strcpy(cWriteText, "SCRIPT");

		fputs(cWriteText, pFile);

		// �X�N���v�g���ʂ���
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{
				// ���̍ő吔�Ȃ�Z�[�u
				if (nCntFloor < ALL_MAX - 1)
				{
					if (g_floor[nCntFloor].bUse == true)
					{
						fputs(cEnter, pFile);

						// ���f���̃p�[�c�̖��O����
						strcpy(cWriteText, cFloorType[g_floor[nCntFloor].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    FLOORSET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_FLOORSET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cFloorData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_floor[nCntFloor].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].pos.x,
										g_floor[nCntFloor].pos.y,
										g_floor[nCntFloor].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].rot.x,
										g_floor[nCntFloor].rot.y,
										g_floor[nCntFloor].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �c�u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].nBlock_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ���u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].nBlock_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �w�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Z�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "ZSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].size.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �G���h���Z�b�g
								else if (strcmp(cWriteText, "END_FLOORSET") == 0)
								{
									sprintf(cHeadText, "    %s", cWriteText);
									fputs(cHeadText, pFile);

									// �J�E���g�f�[�^������
									nCntDataLoad = 0;
								}
							}
						}
					}
					// ���f���p�[�c�ύX
					nCntFloor++;
				}

				// �G���h�X�N���v�g
				else
				{
					fputs(cEnter, pFile);

					strcpy(cWriteText, "END_SCRIPT");

					fputs(cWriteText, pFile);
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	else
	{
		MessageBox(NULL, "���̏������݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
bool ColisionFloor(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move,D3DXVECTOR3 * size, FLOOR ** ppFloorRet)
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
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// �g�p���
		if (g_floor[nCntFloor].bUse == true)
		{
			// �u���b�N��X�͈�
			if (pos->x + size->x * 0.5f > g_floor[nCntFloor].pos.x - g_floor[nCntFloor].size.x&&
				pos->x - size->x * 0.5f < g_floor[nCntFloor].pos.x + g_floor[nCntFloor].size.x&&
				pos->z + size->z * 0.5f > g_floor[nCntFloor].pos.z - g_floor[nCntFloor].size.z&&
				pos->z - size->z * 0.5f < g_floor[nCntFloor].pos.z + g_floor[nCntFloor].size.z)
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
// �I�𒆂̏�
// ------------------------------------------------------------------------------------------
bool SeleFloor(int nCntFloor)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	ARRANGEMENT	*pArrangement = GetArrangement();
	int nCountDirect;	// �c�u���b�N�̃J�E���g
	int nCountWidth;	// ���u���b�N�̃J�E���g

	// �ړ�
	if (GetMousePress(0))
	{
		// �z�u�̈ʒu�����擾
		g_floor[nCntFloor].pos = pArrangement->pos;
	}

	// �c�̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_floor[nCntFloor].nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_floor[nCntFloor].nBlock_Depth--;
	}

	// ���̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_floor[nCntFloor].nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_floor[nCntFloor].nBlock_Width--;
	}

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_floor[nCntFloor].size.z += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_floor[nCntFloor].size.z -= 50.0f;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_floor[nCntFloor].size.x += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_floor[nCntFloor].size.x -= 50.0f;
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_floor[nCntFloor].nType++;

		// ���E�l�܂ōs������
		if (g_floor[nCntFloor].nType >= FLOORTYPE_MAX)
		{
			// ������
			g_floor[nCntFloor].nType = 0;
		}
	}

	// �폜
	if (GetKeyboardSpeacePress(DIK_BACK))
	{
		g_floor[nCntFloor].bDisp = false;
		g_floor[nCntFloor].bUse = false;
	}

	// �u���b�N�`��̌��_�̐ݒ�
	g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
		g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
		0.0f,
		g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

	// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
	g_floor[nCntFloor].nNumberVertexMeshField =
		(g_floor[nCntFloor].nBlock_Depth + 1) *
		(g_floor[nCntFloor].nBlock_Width + 1);
	g_floor[nCntFloor].nNumIndex =
		(g_floor[nCntFloor].nBlock_Width + 1) * 2
		* g_floor[nCntFloor].nBlock_Depth +
		2 * (g_floor[nCntFloor].nBlock_Depth - 1);
	g_floor[nCntFloor].nNumPolygon =
		g_floor[nCntFloor].nBlock_Depth *
		g_floor[nCntFloor].nBlock_Width * 2 +
		4 * (g_floor[nCntFloor].nBlock_Depth - 1);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_floor[nCntFloor].pVtxBuffFLOOR,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
					g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
					0.0f,
					g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
			// �k�ރ|���S�����̒��_�ǉ�
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

		// �k�ރ|���S�������K�v������ꍇ
		if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}
	}

	// �A�����b�N
	g_floor[nCntFloor].pIndexMeshField->Unlock();

	// ���邩�Ȃ���
	return g_floor[nCntFloor].bUse;
}

// ------------------------------------------------------------------------------------------
// �|���S�����
// ------------------------------------------------------------------------------------------
FLOOR *GetFloor(void)
{
	return &g_floor[0];
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------------------------------------------------------
void Debug_Floor(void)
{
	PrintDebug("/*---------<�����>----------*/\n");
	PrintDebug("�T�C�Y�ύX(x:[I][K],z:[O][L])\n");
	PrintDebug("�u���b�N�ύX(��:[Y][H],�c:[U][J])\n");
	PrintDebug("�^�C�v�ύX[1]\n");
	PrintDebug("�T�C�Y:(%0.3f,%0.3f,%0.3f)\n",
		g_FloorSet.size.x,
		g_FloorSet.size.y,
		g_FloorSet.size.z);
	PrintDebug("�u���b�N(��:%d,�c:%d)\n",
		g_FloorSet.nBlock_Width,
		g_FloorSet.nBlock_Depth
	);
	PrintDebug("�^�C�v(%d)\n",
		g_FloorSet.nType
	);
}
#endif // _DEBUG