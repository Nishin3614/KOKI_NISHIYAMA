// ------------------------------------------------------------------------------------------
//
// �u���b�N���� [block.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "block.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define BLOCK_PHONT "data/TEXTURE/woods.png"
#define BLOCK_SIZE (10)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	BLOCKTYPE_00_ROAD1 = 0,
	BLOCKTYPE_MAX
} BLOCKTYPE;
// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
bool ColisionBlock(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, BLOCK ** ppBlockRet);

void Update_ChangeBlock(void);
void Update_DispBlock(void);
void Update_DestroyBlock(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// ���_�e�N�X�`���[�̃|�C���^

BLOCK g_ablock[ALL_MAX];							// �u���b�N
BLOCKSET g_BlockSet;							// �z�u���

// 3D�p�̃Z�[�u�t�@�C��
const char g_cBlockFIleName_3D[5][128] =
{
	"data/SAVE/2D/Stage_1/block.txt",
	"data/SAVE/2D/Stage_2/block.txt",
	"data/SAVE/2D/Stage_3/block.txt",
	"data/SAVE/2D/Stage_4/block.txt",
	"data/SAVE/2D/Stage_5/block.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		g_ablock[nCntBlock].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ablock[nCntBlock].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ablock[nCntBlock].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_ablock[nCntBlock].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_ablock[nCntBlock].nBlock_Depth	= 1;
		g_ablock[nCntBlock].nBlock_Width	= 1;

		// ���_�̌v�Z
		g_ablock[nCntBlock].OriginBlock	= D3DXVECTOR3(
			g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
			g_ablock[nCntBlock].size.y * 0.5f * g_ablock[nCntBlock].nBlock_Depth,
			0.0f);

		// �����_�̌v�Z
		g_ablock[nCntBlock].nNumberVertexMeshField = 
			(g_ablock[nCntBlock].nBlock_Depth + 1) *
			(g_ablock[nCntBlock].nBlock_Width + 1);

		// �C���f�b�N�X�̌v�Z
		g_ablock[nCntBlock].nNumIndex = 
			(g_ablock[nCntBlock].nBlock_Width + 1) * 2 *
			g_ablock[nCntBlock].nBlock_Depth + 
			2 * (g_ablock[nCntBlock].nBlock_Depth - 1);

		// �|���S���̌v�Z
		g_ablock[nCntBlock].nNumPolygon = 
			g_ablock[nCntBlock].nBlock_Depth * 
			g_ablock[nCntBlock].nBlock_Width * 2 + 
			4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

		g_ablock[nCntBlock].nNumber = 0;

		// �g�p�E�\����Ԃ̏�����
		g_ablock[nCntBlock].bUse = false;
		g_ablock[nCntBlock].bDisp = false;
	}

	// �z�u���̏�����
	g_BlockSet.nBlock_Depth = 1;
	g_BlockSet.nBlock_Width = 1;
	g_BlockSet.nType = 0;
	g_BlockSet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_BlockSet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BLOCK_PHONT, &g_pTextureBlock);

	// �u���b�N�̃f�[�^�Ǎ�
	LoadBlock();

	// ���_���̍쐬
	MakeVertexBlock(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitBlock(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_ablock[nCntBlock].pVtxBuff != NULL)
		{
			g_ablock[nCntBlock].pVtxBuff->Release();
			g_ablock[nCntBlock].pVtxBuff = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_ablock[nCntBlock].pIndex != NULL)
		{
			g_ablock[nCntBlock].pIndex->Release();
			g_ablock[nCntBlock].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateBlock(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// �}�E�X�̏��擾
	pMouseX = GetMouseX();

	// �폜����
	Update_DestroyBlock();

	// ���̕ω�����
	Update_ChangeBlock();

	// �z�u���̏ꏊ�c��
	Update_DispBlock();

	// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
	if (GetMousePressTrigger(0))
	{
		SetBlock(
			pArrangement->pos,
			pArrangement->rot,
			g_BlockSet.size,
			g_BlockSet.nType);
	}
}

// ------------------------------------------------------------------------------------------
// ���̕ω�����
// ------------------------------------------------------------------------------------------
void Update_ChangeBlock(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_BlockSet.size.y += BLOCK_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_BlockSet.size.y -= BLOCK_SIZE;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_BlockSet.size.x += BLOCK_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_BlockSet.size.x -= BLOCK_SIZE;
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_BlockSet.nType++;

		// ���E�l�܂ōs������
		if (g_BlockSet.nType >= BLOCKTYPE_MAX)
		{
			// ������
			g_BlockSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�̎��̏ꏊ�c������
// ------------------------------------------------------------------------------------------
void Update_DispBlock(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
												// �ϐ��錾
	int nCountDirect;
	int nCountWidth;

	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == false)
		{
			g_ablock[nCntBlock].nType = g_BlockSet.nType;

			// �g�p��ԁE�\�����
			g_ablock[nCntBlock].bDisp = true;

			// �z�u�̈ʒu�����擾
			g_ablock[nCntBlock].pos = pArrangement->pos;

			// ��]�����擾
			g_ablock[nCntBlock].rot = pArrangement->rot;

			if (g_ablock[nCntBlock].nBlock_Depth != g_BlockSet.nBlock_Depth ||
				g_ablock[nCntBlock].nBlock_Width != g_BlockSet.nBlock_Width ||
				g_ablock[nCntBlock].size != g_BlockSet.size)
			{
				g_ablock[nCntBlock].nBlock_Depth = g_BlockSet.nBlock_Depth;
				g_ablock[nCntBlock].nBlock_Width = g_BlockSet.nBlock_Width;
				g_ablock[nCntBlock].size = g_BlockSet.size;

				// ���_�̌v�Z
				g_ablock[nCntBlock].OriginBlock = D3DXVECTOR3(
					g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
					g_ablock[nCntBlock].size.y * 0.5f * g_ablock[nCntBlock].nBlock_Depth,
					0.0f);

				// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
				g_ablock[nCntBlock].nNumberVertexMeshField =
					(g_ablock[nCntBlock].nBlock_Depth + 1) *
					(g_ablock[nCntBlock].nBlock_Width + 1);

				g_ablock[nCntBlock].nNumIndex =
					(g_ablock[nCntBlock].nBlock_Width + 1) * 2 *
					g_ablock[nCntBlock].nBlock_Depth + 2 *
					(g_ablock[nCntBlock].nBlock_Depth - 1);

				g_ablock[nCntBlock].nNumPolygon =
					g_ablock[nCntBlock].nBlock_Depth * g_ablock[nCntBlock].nBlock_Width * 2 +
					4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_ablock[nCntBlock].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_ablock[nCntBlock].pVtxBuff, NULL);

				// �C���f�b�N�X�o�b�t�@�̐���
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_ablock[nCntBlock].nNumIndex,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_ablock[nCntBlock].pIndex,
					NULL);

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

									// �C���f�b�N�X���̐ݒ�
				WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_ablock[nCntBlock].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

				//���_�ݒ� //
				//�s���[�v
				for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth + 1; nCountDirect++)
				{
					// �񃋁[�v
					for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
					{
						// ���_���W�̐ݒ�
						pVtx[0].pos =
							D3DXVECTOR3(
								g_ablock[nCntBlock].OriginBlock.x + 
								(g_ablock[nCntBlock].size.x * nCountWidth),
								g_ablock[nCntBlock].OriginBlock.y - 
								(g_ablock[nCntBlock].size.y * nCountDirect),
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
				g_ablock[nCntBlock].pVtxBuff->Unlock();

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_ablock[nCntBlock].pIndex->Lock(0, 0, (void **)&pIdx, 0);

				// �c�u���b�N��
				for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth; nCountDirect++)
				{
					// �Q��ڂ̃��[�v�ȍ~
					if (nCountDirect >= 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = 
							nCountDirect * 
							(g_ablock[nCntBlock].nBlock_Width + 1) +
							g_ablock[nCntBlock].nBlock_Width + 1;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}

					// ���u���b�N�̒��_��
					for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
					{
						// �`�揇�Ԃ̃C���f�b�N�X
						pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth + g_ablock[nCntBlock].nBlock_Width + 1;
						pIdx[1] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx += 2;
					}

					// �k�ރ|���S�������K�v������ꍇ
					if (nCountDirect < g_ablock[nCntBlock].nBlock_Depth - 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}
				}

				// �A�����b�N
				g_ablock[nCntBlock].pIndex->Unlock();
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void Update_DestroyBlock(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == true && g_ablock[nCntBlock].bDisp == true)
		{
			// �u���b�N��X�͈�
			if (pArrangement->pos.x >
				g_ablock[nCntBlock].pos.x - sinf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.x * 0.5f * g_ablock[nCntBlock].nBlock_Width&&
				pArrangement->pos.x <
				g_ablock[nCntBlock].pos.x + sinf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.x * 0.5f * g_ablock[nCntBlock].nBlock_Width&&
				pArrangement->pos.z >
				g_ablock[nCntBlock].pos.z - cosf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.z * 0.5f * g_ablock[nCntBlock].nBlock_Depth&&
				pArrangement->pos.z <
				g_ablock[nCntBlock].pos.z + cosf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.z * 0.5f * g_ablock[nCntBlock].nBlock_Depth)
			{
				// �폜����
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_ablock[nCntBlock].bUse = false;
					g_ablock[nCntBlock].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// �J�E���g�u���b�N
	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bDisp == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_ablock[nCntBlock].mtxWorldBlock);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// �����̓��ꕨ
				g_ablock[nCntBlock].rot.y, g_ablock[nCntBlock].rot.x, g_ablock[nCntBlock].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_ablock[nCntBlock].mtxWorldBlock,	// 1
				&g_ablock[nCntBlock].mtxWorldBlock,				// 2
				&mtxRot);							// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,												// �����̓��ꕨ
				g_ablock[nCntBlock].pos.x, g_ablock[nCntBlock].pos.y, g_ablock[nCntBlock].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_ablock[nCntBlock].mtxWorldBlock,	// 1
				&g_ablock[nCntBlock].mtxWorldBlock,				// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_ablock[nCntBlock].mtxWorldBlock);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_ablock[nCntBlock].pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(g_ablock[nCntBlock].pIndex);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_ablock[nCntBlock].nNumberVertexMeshField,
				0,
				g_ablock[nCntBlock].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	//if (GetGameSelect() == GAMESELECT_BLOCK)
	//{
	//	// ���̏��\��
	//	DrawBlockInfo(
	//		g_BlockSet.size,
	//		g_BlockSet.nBlock_Width,
	//		g_BlockSet.nBlock_Depth,
	//		g_BlockSet.nType);
	//}
#endif

}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCntBlock;
	int nCountDirect;
	int nCountWidth;

	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == true)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ablock[nCntBlock].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_ablock[nCntBlock].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

			// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_ablock[nCntBlock].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_ablock[nCntBlock].OriginBlock.x + 
							(g_ablock[nCntBlock].size.x * nCountWidth),
							g_ablock[nCntBlock].OriginBlock.y -
							(g_ablock[nCntBlock].size.y * nCountDirect),
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
			g_ablock[nCntBlock].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_ablock[nCntBlock].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] =
						nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + 
						nCountWidth + g_ablock[nCntBlock].nBlock_Width + 1;

					pIdx[1] =
						nCountDirect *
						(g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_ablock[nCntBlock].nBlock_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_ablock[nCntBlock].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// �u���b�N�̓����蔻����
// ------------------------------------------------------------------------------------------
bool ColisionBlock(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, BLOCK ** ppBlockRet)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntBlock;			// �J�E���g��

	// �Ԓn��񂪂�������
	if (ppBlockRet != NULL)
	{
		*ppBlockRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// �g�p���
		if (g_ablock[nCntBlock].bUse == true)
		{
			
			// �u���b�N��Y�͈�
			if (pos->y - size->y < g_ablock[nCntBlock].pos.y + g_ablock[nCntBlock].size.y&&
				pos->y + size->y > g_ablock[nCntBlock].pos.y)
			{
				// �u���b�N��Z�͈�
				if (pos->z + size->z > g_ablock[nCntBlock].pos.z - (g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_ablock[nCntBlock].pos.z + (g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��)
					if (pos->x + size->z >
						g_ablock[nCntBlock].pos.x&&
						posOld->x + size->z <=
						g_ablock[nCntBlock].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppBlockRet = &g_ablock[nCntBlock];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_ablock[nCntBlock].pos.x - size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

					// �u���b�N��Z�͈�
				if (pos->z + size->z > g_ablock[nCntBlock].pos.z + (g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_ablock[nCntBlock].pos.z -(g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(�E)
					if (pos->x - size->z <
						g_ablock[nCntBlock].pos.x&&
						posOld->x - size->z >=
						g_ablock[nCntBlock].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppBlockRet = &g_ablock[nCntBlock];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_ablock[nCntBlock].pos.x + size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

				// �u���b�N��X�͈�
				if (pos->x + size->z > g_ablock[nCntBlock].pos.x - (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_ablock[nCntBlock].pos.x + (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��O)
					if (pos->z + size->z > g_ablock[nCntBlock].pos.z&&
						posOld->z + size->z <= g_ablock[nCntBlock].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppBlockRet = &g_ablock[nCntBlock];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_ablock[nCntBlock].pos.z - size->z;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}
				}

					// �u���b�N��X�͈�
				if (pos->x + size->z > g_ablock[nCntBlock].pos.x + (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_ablock[nCntBlock].pos.x - (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(��)
					if (pos->z - size->z < g_ablock[nCntBlock].pos.z&&
						posOld->z - size->z >= g_ablock[nCntBlock].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppBlockRet = &g_ablock[nCntBlock];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_ablock[nCntBlock].pos.z + size->z;

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
void NotDispBlock(void)
{
	int nCntBlock;

	// ���̃J�E���g
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// �z�u�p�̏�
		if (g_ablock[nCntBlock].bUse == false &&
			g_ablock[nCntBlock].bDisp == true)
		{
			g_ablock[nCntBlock].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u����
// ------------------------------------------------------------------------------------------
void SaveBlock(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntBlock = 0;
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
	char *cBlockType[BLOCKTYPE_MAX] =
	{
		{ "    ----------[00]:�u���b�N1----------" },				// 00_���P
	};

	// ���f�[�^�̎��
	char *cBlockData[] =
	{
		{ "TYPE" },			// �^�C�v
		{ "POS" },			// �ʒu
		{ "SIZE" },			// ��]
		{ "END_BLOCKSET" }	// �G���h�t���A�[
	};

	// �t�@�C���J
	pFile = fopen(g_cBlockFIleName_3D[GetGameStage()], "w");

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
				if (nCntBlock < ALL_MAX - 1)
				{
					if (g_ablock[nCntBlock].bUse == true)
					{
						fputs(cEnter, pFile);

						// ���f���̃p�[�c�̖��O����
						strcpy(cWriteText, cBlockType[g_ablock[nCntBlock].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    BLOCKSET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_BLOCKSET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cBlockData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_ablock[nCntBlock].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_ablock[nCntBlock].pos.x,
										g_ablock[nCntBlock].pos.y,
										g_ablock[nCntBlock].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �T�C�Y���ۑ�
								else if (strcmp(cWriteText, "SIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f\n", 
										&cWriteText, &cEqual,
										g_ablock[nCntBlock].size.x,
										g_ablock[nCntBlock].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �G���h���Z�b�g
								else if (strcmp(cWriteText, "END_BLOCKSET") == 0)
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
					nCntBlock++;
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
		MessageBox(NULL, "�u���b�N�f�[�^�̏������݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �u���b�N�ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadBlock(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntBlock = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �����\�����Ă�����̂���������
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == true)
		{
			g_ablock[nCntBlock].bUse = false;
			g_ablock[nCntBlock].bDisp = false;
		}
	}

	// �J�E���g������
	nCntBlock = 0;

	// �t�@�C���J
	pFile = fopen(g_cBlockFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "BLOCKSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_BLOCKSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_ablock[nCntBlock].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_ablock[nCntBlock].pos.x, &g_ablock[nCntBlock].pos.y, &g_ablock[nCntBlock].pos.z);
						}

						// �T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f", 
								&cDie, &cDie,
								&g_ablock[nCntBlock].size.x,
								&g_ablock[nCntBlock].size.y);
						}

					}

					// ���_�̌v�Z
					g_ablock[nCntBlock].OriginBlock = D3DXVECTOR3(
						g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
						g_ablock[nCntBlock].size.y * 0.5f * g_ablock[nCntBlock].nBlock_Depth,
						0.0f);

					// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
					g_ablock[nCntBlock].nNumberVertexMeshField =
						(g_ablock[nCntBlock].nBlock_Depth + 1) * 
						(g_ablock[nCntBlock].nBlock_Width + 1);

					g_ablock[nCntBlock].nNumIndex = 
						(g_ablock[nCntBlock].nBlock_Width + 1) * 2 * 
						g_ablock[nCntBlock].nBlock_Depth + 2 *
						(g_ablock[nCntBlock].nBlock_Depth - 1);

					g_ablock[nCntBlock].nNumPolygon =
						g_ablock[nCntBlock].nBlock_Depth * g_ablock[nCntBlock].nBlock_Width * 2 + 
						4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

					// �g�p��ԁE�\�����
					g_ablock[nCntBlock].bUse = true;
					g_ablock[nCntBlock].bDisp = true;

					// ���J�E���g�̍X�V
					nCntBlock++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�u���b�N�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// ���̐ݒu
// ------------------------------------------------------------------------------------------
void SetBlock(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	int nCntBlock;		// �J�E���g��
	int nCountDirect;	// �c�u���b�N�̃J�E���g
	int nCountWidth;	// ���u���b�N�̃J�E���g

						// ���̐ݒu������
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// ���g�p�̏�
		if (g_ablock[nCntBlock].bUse == false)
		{
			// ����ϐ��ɑ��
			g_ablock[nCntBlock].pos = pos;
			g_ablock[nCntBlock].rot = rot;
			g_ablock[nCntBlock].size = size;
			g_ablock[nCntBlock].nType = nType;

			// ���_�̌v�Z
			g_ablock[nCntBlock].OriginBlock = D3DXVECTOR3(
				g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
				g_ablock[nCntBlock].size.y * 0.5f *g_ablock[nCntBlock].nBlock_Depth,
				0.0f);

			// �����_�̌v�Z
			g_ablock[nCntBlock].nNumberVertexMeshField =
				(g_ablock[nCntBlock].nBlock_Depth + 1) *
				(g_ablock[nCntBlock].nBlock_Width + 1);

			// �C���f�b�N�X�̌v�Z
			g_ablock[nCntBlock].nNumIndex =
				(g_ablock[nCntBlock].nBlock_Width + 1) * 2 *
				g_ablock[nCntBlock].nBlock_Depth +
				2 * (g_ablock[nCntBlock].nBlock_Depth - 1);

			// �|���S���̌v�Z
			g_ablock[nCntBlock].nNumPolygon =
				g_ablock[nCntBlock].nBlock_Depth *
				g_ablock[nCntBlock].nBlock_Width * 2 +
				4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

			g_ablock[nCntBlock].nNumber = 0;

			// �g�p��ԁE�\�����
			g_ablock[nCntBlock].bUse = true;
			g_ablock[nCntBlock].bDisp = true;

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ablock[nCntBlock].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_ablock[nCntBlock].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

								// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

								// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_ablock[nCntBlock].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_ablock[nCntBlock].OriginBlock.x + (g_ablock[nCntBlock].size.x * nCountWidth),
							g_ablock[nCntBlock].OriginBlock.y - (g_ablock[nCntBlock].size.y * nCountDirect),
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
			g_ablock[nCntBlock].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_ablock[nCntBlock].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth + g_ablock[nCntBlock].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_ablock[nCntBlock].nBlock_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_ablock[nCntBlock].pIndex->Unlock();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �u���b�N���
// ------------------------------------------------------------------------------------------
BLOCK *GetBlock(void)
{
	return &g_ablock[0];
}