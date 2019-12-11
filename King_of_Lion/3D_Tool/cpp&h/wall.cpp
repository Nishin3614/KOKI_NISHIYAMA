// ------------------------------------------------------------------------------------------
//
// �Ǐ��� [wall.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "wall.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define WALL_PHONT "data/TEXTURE/woods.png"
#define WALL_SIZE (10)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	WALLTYPE_00_ROAD1 = 0,
	WALLTYPE_MAX
} WALLTYPE;
// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet);

void Update_ChangeWall(void);
void Update_DispWall(void);
void Update_DestroyWall(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	// ���_�e�N�X�`���[�̃|�C���^

WALL g_awall[ALL_MAX];							// ��
WALLSET g_WallSet;							// �z�u���

// 3D�p�̃Z�[�u�t�@�C��
const char g_cWallFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/wall.txt",
	"data/SAVE/3D/Stage_2/wall.txt",
	"data/SAVE/3D/Stage_3/wall.txt",
	"data/SAVE/3D/Stage_4/wall.txt",
	"data/SAVE/3D/Stage_5/wall.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
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
	}

	// �z�u���̏�����
	g_WallSet.nBlock_Depth = 1;
	g_WallSet.nBlock_Width = 1;
	g_WallSet.nType = 0;
	g_WallSet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_WallSet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
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
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �폜����
	Update_DestroyWall();

	// ���̕ω�����
	Update_ChangeWall();

	// �z�u���̏ꏊ�c��
	Update_DispWall();

	// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
	if (GetMousePressTrigger(0))
	{
		SetWall(
			pArrangement->pos,
			pArrangement->rot,
			g_WallSet.size,
			g_WallSet.nBlock_Depth,
			g_WallSet.nBlock_Width,
			g_WallSet.nType);
	}
}

// ------------------------------------------------------------------------------------------
// ���̕ω�����
// ------------------------------------------------------------------------------------------
void Update_ChangeWall(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �c�̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_WallSet.nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_WallSet.nBlock_Depth--;
	}

	// ���̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_WallSet.nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_WallSet.nBlock_Width--;
	}

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_WallSet.size.y += WALL_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_WallSet.size.y -= WALL_SIZE;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_WallSet.size.x += WALL_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_WallSet.size.x -= WALL_SIZE;
	}

	// ��]�ʂ𑝌�
	if (GetKeyboardSpeacePress(DIK_T))
	{
		g_WallSet.rot.y += 1.57f;

		// -3.14�`3.14�ɉ�]�ʂ��Œ肳����
		if (g_WallSet.rot.y < -D3DX_PI) g_WallSet.rot.y += D3DX_PI * 2;		// -3.14����

		else if (g_WallSet.rot.y > D3DX_PI) g_WallSet.rot.y += -D3DX_PI * 2;	// 3.14����

	}

	else if (GetKeyboardSpeacePress(DIK_G))
	{
		g_WallSet.rot.y -= 1.57f;

		// -3.14�`3.14�ɉ�]�ʂ��Œ肳����
		if (g_WallSet.rot.y < -D3DX_PI) g_WallSet.rot.y += D3DX_PI * 2;		// -3.14����

		else if (g_WallSet.rot.y > D3DX_PI) g_WallSet.rot.y += -D3DX_PI * 2;	// 3.14����
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_WallSet.nType++;

		// ���E�l�܂ōs������
		if (g_WallSet.nType >= WALLTYPE_MAX)
		{
			// ������
			g_WallSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�̎��̏ꏊ�c������
// ------------------------------------------------------------------------------------------
void Update_DispWall(void)
{
	// �ϐ��錾												
	int nCountDirect;
	int nCountWidth;
	int nCntWall;
	// ���擾
	ARRANGEMENT *pArrangement = GetArrangement();	// �z��
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X

	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == false)
		{
			g_awall[nCntWall].nType = g_WallSet.nType;

			// �g�p��ԁE�\�����
			g_awall[nCntWall].bDisp = true;

			// �z�u�̈ʒu�����擾
			g_awall[nCntWall].pos = pArrangement->pos;

			// ��]�����擾
			g_awall[nCntWall].rot = pArrangement->rot;

			if (g_awall[nCntWall].nBlock_Depth != g_WallSet.nBlock_Depth ||
				g_awall[nCntWall].nBlock_Width != g_WallSet.nBlock_Width ||
				g_awall[nCntWall].size != g_WallSet.size)
			{
				g_awall[nCntWall].nBlock_Depth = g_WallSet.nBlock_Depth;
				g_awall[nCntWall].nBlock_Width = g_WallSet.nBlock_Width;
				g_awall[nCntWall].size = g_WallSet.size;

				// ���_�̌v�Z
				g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
					g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
					g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
					0.0f);

				// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
				g_awall[nCntWall].nNumberVertexMeshField =
					(g_awall[nCntWall].nBlock_Depth + 1) *
					(g_awall[nCntWall].nBlock_Width + 1);

				g_awall[nCntWall].nNumIndex =
					(g_awall[nCntWall].nBlock_Width + 1) * 2 *
					g_awall[nCntWall].nBlock_Depth + 2 *
					(g_awall[nCntWall].nBlock_Depth - 1);

				g_awall[nCntWall].nNumPolygon =
					g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 +
					4 * (g_awall[nCntWall].nBlock_Depth - 1);

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_awall[nCntWall].pVtxBuff, NULL);

				// �C���f�b�N�X�o�b�t�@�̐���
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_awall[nCntWall].nNumIndex,
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
								g_awall[nCntWall].OriginBlock.x + 
								(g_awall[nCntWall].size.x * nCountWidth),
								g_awall[nCntWall].OriginBlock.y - 
								(g_awall[nCntWall].size.y * nCountDirect),
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
						pIdx[0] = 
							nCountDirect * 
							(g_awall[nCntWall].nBlock_Width + 1) +
							g_awall[nCntWall].nBlock_Width + 1;

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

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void Update_DestroyWall(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true && g_awall[nCntWall].bDisp == true)
		{
			// �u���b�N��X�͈�
			if (pArrangement->pos.x >
				g_awall[nCntWall].pos.x - sinf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width&&
				pArrangement->pos.x <
				g_awall[nCntWall].pos.x + sinf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width&&
				pArrangement->pos.z >
				g_awall[nCntWall].pos.z - cosf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.z * 0.5f * g_awall[nCntWall].nBlock_Depth&&
				pArrangement->pos.z <
				g_awall[nCntWall].pos.z + cosf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.z * 0.5f * g_awall[nCntWall].nBlock_Depth)
			{
				// �폜����
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_awall[nCntWall].bUse = false;
					g_awall[nCntWall].bDisp = false;
				}
			}
		}
	}
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
	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bDisp == true)
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
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_awall[nCntWall].nNumberVertexMeshField,
				0,
				g_awall[nCntWall].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	if (GetGameSelect() == GAMESELECT_WALL)
	{
		// ���̏��\��
		Debug_Wall();
	}
#endif

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

	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true)
		{
			// ���_�̌v�Z
			g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
				g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
				g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
				0.0f);

			// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
			g_awall[nCntWall].nNumberVertexMeshField =
				(g_awall[nCntWall].nBlock_Depth + 1) *
				(g_awall[nCntWall].nBlock_Width + 1);

			g_awall[nCntWall].nNumIndex =
				(g_awall[nCntWall].nBlock_Width + 1) * 2 *
				g_awall[nCntWall].nBlock_Depth + 2 *
				(g_awall[nCntWall].nBlock_Depth - 1);

			g_awall[nCntWall].nNumPolygon =
				g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 +
				4 * (g_awall[nCntWall].nBlock_Depth - 1);

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(
				sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(
				sizeof(WORD) * g_awall[nCntWall].nNumIndex,
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
							g_awall[nCntWall].OriginBlock.x +
							(g_awall[nCntWall].size.x * nCountWidth),
							g_awall[nCntWall].OriginBlock.y -
							(g_awall[nCntWall].size.y * nCountDirect),
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
					pIdx[0] =
						nCountDirect *
						(g_awall[nCntWall].nBlock_Width + 1) +
						g_awall[nCntWall].nBlock_Width + 1;

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

	// �Ԓn��񂪂�������
	if (ppWallRet != NULL)
	{
		*ppWallRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// �g�p���
		if (g_awall[nCntWall].bUse == true)
		{
			
			// �u���b�N��Y�͈�
			if (pos->y - size->y < g_awall[nCntWall].pos.y + g_awall[nCntWall].size.y&&
				pos->y + size->y > g_awall[nCntWall].pos.y)
			{
				// �u���b�N��Z�͈�
				if (pos->z + size->z > g_awall[nCntWall].pos.z - (g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_awall[nCntWall].pos.z + (g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
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
				if (pos->z + size->z > g_awall[nCntWall].pos.z + (g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_awall[nCntWall].pos.z -(g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
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
				if (pos->x + size->z > g_awall[nCntWall].pos.x - (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_awall[nCntWall].pos.x + (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
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
				if (pos->x + size->z > g_awall[nCntWall].pos.x + (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_awall[nCntWall].pos.x - (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)))	// z�����̃T�C�Y)
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
// �z�u�ꏊ���킩�邽�߂ɕ`�悵�Ă������̂��\���ɏ���
// ------------------------------------------------------------------------------------------
void NotDispWall(void)
{
	int nCntWall;

	// ���̃J�E���g
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// �z�u�p�̏�
		if (g_awall[nCntWall].bUse == false &&
			g_awall[nCntWall].bDisp == true)
		{
			g_awall[nCntWall].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u����
// ------------------------------------------------------------------------------------------
void SaveWall(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntWall = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// �o�O�΍�J�E���g
	char	cWriteText[512];		// �����Ƃ��ď������ݗp
	char	cHeadText[128];			// �����̊i�[
	char	cEqual[8] = { "=" };	// �C�R�[���p

	char	cSharpEnter[8];			// �R�����g�P
	char	cComment[64];			// �R�����g�Q
	char	cEnter[8];				// �R�����g�R

									// �悭�g���镶��
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// ���̎�ގ��
	char *cWallType[WALLTYPE_MAX] =
	{
		{ "    ----------[00]:��1----------" },				// 00_���P
	};

	// ���f�[�^�̎��
	char *cWallData[] =
	{
		{ "TYPE" },			// �^�C�v
		{ "POS" },			// �ʒu
		{ "ROT" },			// ��]
		{ "BLOCK_DEPTH" },	// �c�u���b�N
		{ "BLOCK_WIDTH" },	// ���u���b�N
		{ "XSIZE" },		// �w�T�C�Y
		{ "YSIZE" },		// Z�T�C�Y
		{ "END_WALLSET" }	// �G���h�t���A�[
	};

	// �t�@�C���J
	pFile = fopen(g_cWallFIleName_3D[GetGameStage()], "w");

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
				if (nCntWall < ALL_MAX - 1)
				{
					if (g_awall[nCntWall].bUse == true)
					{
						fputs(cEnter, pFile);

						// ���f���̃p�[�c�̖��O����
						strcpy(cWriteText, cWallType[g_awall[nCntWall].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    WALLSET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_WALLSET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cWallData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_awall[nCntWall].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].pos.x,
										g_awall[nCntWall].pos.y,
										g_awall[nCntWall].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].rot.x,
										g_awall[nCntWall].rot.y,
										g_awall[nCntWall].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �c�u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_awall[nCntWall].nBlock_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ���u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_awall[nCntWall].nBlock_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �w�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Z�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "YSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �G���h���Z�b�g
								else if (strcmp(cWriteText, "END_WALLSET") == 0)
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
					nCntWall++;
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
		MessageBox(NULL, "���f���f�[�^�̏������݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
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

	// �����\�����Ă�����̂���������
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true)
		{
			g_awall[nCntWall].bUse = false;
			g_awall[nCntWall].bDisp = false;
		}
	}

	// �J�E���g������
	nCntWall = 0;

	// �t�@�C���J
	pFile = fopen(g_cWallFIleName_3D[GetGameStage()], "r");

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
					g_awall[nCntWall].nNumberVertexMeshField =
						(g_awall[nCntWall].nBlock_Depth + 1) * 
						(g_awall[nCntWall].nBlock_Width + 1);

					g_awall[nCntWall].nNumIndex = 
						(g_awall[nCntWall].nBlock_Width + 1) * 2 * 
						g_awall[nCntWall].nBlock_Depth + 2 *
						(g_awall[nCntWall].nBlock_Depth - 1);

					g_awall[nCntWall].nNumPolygon =
						g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 + 
						4 * (g_awall[nCntWall].nBlock_Depth - 1);

					// �g�p��ԁE�\�����
					g_awall[nCntWall].bUse = true;
					g_awall[nCntWall].bDisp = true;

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
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// ���̐ݒu
// ------------------------------------------------------------------------------------------
void SetWall(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	int nCntWall;		// �J�E���g��
	int nCountDirect;	// �c�u���b�N�̃J�E���g
	int nCountWidth;	// ���u���b�N�̃J�E���g

						// ���̐ݒu������
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// ���g�p�̏�
		if (g_awall[nCntWall].bUse == false)
		{
			// ����ϐ��ɑ��
			g_awall[nCntWall].pos = pos;
			g_awall[nCntWall].rot = rot;
			g_awall[nCntWall].size = size;
			g_awall[nCntWall].nBlock_Depth = nBlock_Depth;
			g_awall[nCntWall].nBlock_Width = nBlock_Width;
			g_awall[nCntWall].nType = nType;

			// ���_�̌v�Z
			g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
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

			// �g�p��ԁE�\�����
			g_awall[nCntWall].bUse = true;
			g_awall[nCntWall].bDisp = true;

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
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Ǐ��
// ------------------------------------------------------------------------------------------
WALL *GetWall(void)
{
	return &g_awall[0];
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------------------------------------------------------
void Debug_Wall(void)
{
	PrintDebug("/*---------<�Ǐ��>----------*/\n");
	PrintDebug("�T�C�Y�ύX(x:[I][K],z:[O][L])\n");
	PrintDebug("�u���b�N�ύX(��:[Y][H],�c:[U][J])\n");
	PrintDebug("�^�C�v�ύX[4]\n");
	PrintDebug("�T�C�Y:(%0.3f,%0.3f,%0.3f)\n",
		g_WallSet.size.x,
		g_WallSet.size.y,
		g_WallSet.size.z);
	PrintDebug("�u���b�N(��:%d,�c:%d)\n",
		g_WallSet.nBlock_Width,
		g_WallSet.nBlock_Depth
	);
	PrintDebug("�^�C�v(%d)\n",
		g_WallSet.nType
	);
	PrintDebug("");
}
#endif // _DEBUG