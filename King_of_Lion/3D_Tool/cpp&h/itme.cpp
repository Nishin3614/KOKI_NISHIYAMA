// ------------------------------------------------------------------------------------------
//
// �A�C�e������ [item.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "itme.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define ITME_PHONT "data/TEXTURE/woods.png"
#define ITME_SIZE (10)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	ITMETYPE_00_ROAD1 = 0,
	ITMETYPE_MAX
} ITMETYPE;
// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
bool ColisionItem(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, ITME ** ppItemRet);

void Update_ChangeItem(void);
void Update_DispItem(void);
void Update_DestroyItem(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;	// ���_�e�N�X�`���[�̃|�C���^

ITME g_aitem[ALL_MAX];							// �A�C�e��
ITMESET g_ItemSet;							// �z�u���

// 3D�p�̃Z�[�u�t�@�C��
const char g_cItemFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/item.txt",
	"data/SAVE/3D/Stage_2/item.txt",
	"data/SAVE/3D/Stage_3/item.txt",
	"data/SAVE/3D/Stage_4/item.txt",
	"data/SAVE/3D/Stage_5/item.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		g_aitem[nCntItem].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aitem[nCntItem].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aitem[nCntItem].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aitem[nCntItem].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_aitem[nCntItem].nBlock_Depth	= 1;
		g_aitem[nCntItem].nBlock_Width	= 1;

		// ���_�̌v�Z
		g_aitem[nCntItem].OriginBlock	= D3DXVECTOR3(
			g_aitem[nCntItem].size.x * -0.5f * g_aitem[nCntItem].nBlock_Width,
			g_aitem[nCntItem].size.y * g_aitem[nCntItem].nBlock_Depth,
			0.0f);

		// �����_�̌v�Z
		g_aitem[nCntItem].nNumberVertexMeshField = 
			(g_aitem[nCntItem].nBlock_Depth + 1) *
			(g_aitem[nCntItem].nBlock_Width + 1);

		// �C���f�b�N�X�̌v�Z
		g_aitem[nCntItem].nNumIndex = 
			(g_aitem[nCntItem].nBlock_Width + 1) * 2 *
			g_aitem[nCntItem].nBlock_Depth + 
			2 * (g_aitem[nCntItem].nBlock_Depth - 1);

		// �|���S���̌v�Z
		g_aitem[nCntItem].nNumPolygon = 
			g_aitem[nCntItem].nBlock_Depth * 
			g_aitem[nCntItem].nBlock_Width * 2 + 
			4 * (g_aitem[nCntItem].nBlock_Depth - 1);

		g_aitem[nCntItem].nNumber = 0;

		// �g�p�E�\����Ԃ̏�����
		g_aitem[nCntItem].bUse = false;
		g_aitem[nCntItem].bDisp = false;
	}

	// �z�u���̏�����
	g_ItemSet.nBlock_Depth = 1;
	g_ItemSet.nBlock_Width = 1;
	g_ItemSet.nType = 0;
	g_ItemSet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_ItemSet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ITME_PHONT, &g_pTextureItem);

	// �A�C�e���̃f�[�^�Ǎ�
	LoadItem();

	// ���_���̍쐬
	MakeVertexItem(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitItem(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	for (int nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_aitem[nCntItem].pVtxBuff != NULL)
		{
			g_aitem[nCntItem].pVtxBuff->Release();
			g_aitem[nCntItem].pVtxBuff = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_aitem[nCntItem].pIndex != NULL)
		{
			g_aitem[nCntItem].pIndex->Release();
			g_aitem[nCntItem].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateItem(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// �}�E�X�̏��擾
	pMouseX = GetMouseX();

	// �폜����
	Update_DestroyItem();

	// ���̕ω�����
	Update_ChangeItem();

	// �z�u���̏ꏊ�c��
	Update_DispItem();

	// �͈�
	if (pMouseX < 1000.0f)
	{
		// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
		if (GetMousePressTrigger(0))
		{
			SetItem(
				pArrangement->pos,
				pArrangement->rot,
				g_ItemSet.size,
				g_ItemSet.nBlock_Depth,
				g_ItemSet.nBlock_Width,
				g_ItemSet.nType);
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���̕ω�����
// ------------------------------------------------------------------------------------------
void Update_ChangeItem(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �c�̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_ItemSet.nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_ItemSet.nBlock_Depth--;
	}

	// ���̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_ItemSet.nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_ItemSet.nBlock_Width--;
	}

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_ItemSet.size.y += ITME_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_ItemSet.size.y -= ITME_SIZE;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_ItemSet.size.x += ITME_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_ItemSet.size.x -= ITME_SIZE;
	}

	// ��]�ʂ𑝌�
	if (GetKeyboardSpeacePress(DIK_T))
	{
		g_ItemSet.rot.y += 1.57f;

		// -3.14�`3.14�ɉ�]�ʂ��Œ肳����
		if (g_ItemSet.rot.y < -D3DX_PI) g_ItemSet.rot.y += D3DX_PI * 2;		// -3.14����

		else if (g_ItemSet.rot.y > D3DX_PI) g_ItemSet.rot.y += -D3DX_PI * 2;	// 3.14����

	}

	else if (GetKeyboardSpeacePress(DIK_G))
	{
		g_ItemSet.rot.y -= 1.57f;

		// -3.14�`3.14�ɉ�]�ʂ��Œ肳����
		if (g_ItemSet.rot.y < -D3DX_PI) g_ItemSet.rot.y += D3DX_PI * 2;		// -3.14����

		else if (g_ItemSet.rot.y > D3DX_PI) g_ItemSet.rot.y += -D3DX_PI * 2;	// 3.14����
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_ItemSet.nType++;

		// ���E�l�܂ōs������
		if (g_ItemSet.nType >= ITMETYPE_MAX)
		{
			// ������
			g_ItemSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�̎��̏ꏊ�c������
// ------------------------------------------------------------------------------------------
void Update_DispItem(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
												// �ϐ��錾
	int nCountDirect;
	int nCountWidth;

	int nCntItem;

	for (nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		if (g_aitem[nCntItem].bUse == false)
		{
			g_aitem[nCntItem].nType = g_ItemSet.nType;

			// �g�p��ԁE�\�����
			g_aitem[nCntItem].bDisp = true;

			// �z�u�̈ʒu�����擾
			g_aitem[nCntItem].pos = pArrangement->pos;

			// ��]�����擾
			g_aitem[nCntItem].rot = pArrangement->rot;

			if (g_aitem[nCntItem].nBlock_Depth != g_ItemSet.nBlock_Depth ||
				g_aitem[nCntItem].nBlock_Width != g_ItemSet.nBlock_Width ||
				g_aitem[nCntItem].size != g_ItemSet.size)
			{
				g_aitem[nCntItem].nBlock_Depth = g_ItemSet.nBlock_Depth;
				g_aitem[nCntItem].nBlock_Width = g_ItemSet.nBlock_Width;
				g_aitem[nCntItem].size = g_ItemSet.size;

				// ���_�̌v�Z
				g_aitem[nCntItem].OriginBlock = D3DXVECTOR3(
					g_aitem[nCntItem].size.x * -0.5f * g_aitem[nCntItem].nBlock_Width,
					g_aitem[nCntItem].size.y * g_aitem[nCntItem].nBlock_Depth,
					0.0f);

				// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
				g_aitem[nCntItem].nNumberVertexMeshField =
					(g_aitem[nCntItem].nBlock_Depth + 1) *
					(g_aitem[nCntItem].nBlock_Width + 1);

				g_aitem[nCntItem].nNumIndex =
					(g_aitem[nCntItem].nBlock_Width + 1) * 2 *
					g_aitem[nCntItem].nBlock_Depth + 2 *
					(g_aitem[nCntItem].nBlock_Depth - 1);

				g_aitem[nCntItem].nNumPolygon =
					g_aitem[nCntItem].nBlock_Depth * g_aitem[nCntItem].nBlock_Width * 2 +
					4 * (g_aitem[nCntItem].nBlock_Depth - 1);

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_aitem[nCntItem].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_aitem[nCntItem].pVtxBuff, NULL);

				// �C���f�b�N�X�o�b�t�@�̐���
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_aitem[nCntItem].nNumIndex,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_aitem[nCntItem].pIndex,
					NULL);

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

									// �C���f�b�N�X���̐ݒ�
				WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_aitem[nCntItem].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

				//���_�ݒ� //
				//�s���[�v
				for (nCountDirect = 0; nCountDirect < g_aitem[nCntItem].nBlock_Depth + 1; nCountDirect++)
				{
					// �񃋁[�v
					for (nCountWidth = 0; nCountWidth < g_aitem[nCntItem].nBlock_Width + 1; nCountWidth++)
					{
						// ���_���W�̐ݒ�
						pVtx[0].pos =
							D3DXVECTOR3(
								g_aitem[nCntItem].OriginBlock.x + 
								(g_aitem[nCntItem].size.x * nCountWidth),
								g_aitem[nCntItem].OriginBlock.y - 
								(g_aitem[nCntItem].size.y * nCountDirect),
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
				g_aitem[nCntItem].pVtxBuff->Unlock();

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_aitem[nCntItem].pIndex->Lock(0, 0, (void **)&pIdx, 0);

				// �c�u���b�N��
				for (nCountDirect = 0; nCountDirect < g_aitem[nCntItem].nBlock_Depth; nCountDirect++)
				{
					// �Q��ڂ̃��[�v�ȍ~
					if (nCountDirect >= 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = 
							nCountDirect * 
							(g_aitem[nCntItem].nBlock_Width + 1) +
							g_aitem[nCntItem].nBlock_Width + 1;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}

					// ���u���b�N�̒��_��
					for (nCountWidth = 0; nCountWidth < g_aitem[nCntItem].nBlock_Width + 1; nCountWidth++)
					{
						// �`�揇�Ԃ̃C���f�b�N�X
						pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + nCountWidth + g_aitem[nCntItem].nBlock_Width + 1;
						pIdx[1] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + nCountWidth;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx += 2;
					}

					// �k�ރ|���S�������K�v������ꍇ
					if (nCountDirect < g_aitem[nCntItem].nBlock_Depth - 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + g_aitem[nCntItem].nBlock_Width;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}
				}

				// �A�����b�N
				g_aitem[nCntItem].pIndex->Unlock();
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void Update_DestroyItem(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		if (g_aitem[nCntItem].bUse == true && g_aitem[nCntItem].bDisp == true)
		{
			// �u���b�N��X�͈�
			if (pArrangement->pos.x >
				g_aitem[nCntItem].pos.x - sinf(g_aitem[nCntItem].rot.y) * g_aitem[nCntItem].size.x * 0.5f * g_aitem[nCntItem].nBlock_Width&&
				pArrangement->pos.x <
				g_aitem[nCntItem].pos.x + sinf(g_aitem[nCntItem].rot.y) * g_aitem[nCntItem].size.x * 0.5f * g_aitem[nCntItem].nBlock_Width&&
				pArrangement->pos.z >
				g_aitem[nCntItem].pos.z - cosf(g_aitem[nCntItem].rot.y) * g_aitem[nCntItem].size.z * 0.5f * g_aitem[nCntItem].nBlock_Depth&&
				pArrangement->pos.z <
				g_aitem[nCntItem].pos.z + cosf(g_aitem[nCntItem].rot.y) * g_aitem[nCntItem].size.z * 0.5f * g_aitem[nCntItem].nBlock_Depth)
			{
				// �폜����
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_aitem[nCntItem].bUse = false;
					g_aitem[nCntItem].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// �J�E���g�A�C�e��
	for (int nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		if (g_aitem[nCntItem].bDisp == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_aitem[nCntItem].mtxWorldItem);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// �����̓��ꕨ
				g_aitem[nCntItem].rot.y, g_aitem[nCntItem].rot.x, g_aitem[nCntItem].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_aitem[nCntItem].mtxWorldItem,	// 1
				&g_aitem[nCntItem].mtxWorldItem,				// 2
				&mtxRot);							// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,												// �����̓��ꕨ
				g_aitem[nCntItem].pos.x, g_aitem[nCntItem].pos.y, g_aitem[nCntItem].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_aitem[nCntItem].mtxWorldItem,	// 1
				&g_aitem[nCntItem].mtxWorldItem,				// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aitem[nCntItem].mtxWorldItem);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_aitem[nCntItem].pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(g_aitem[nCntItem].pIndex);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItem);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aitem[nCntItem].nNumberVertexMeshField,
				0,
				g_aitem[nCntItem].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	//if (GetGameSelect() == GAMESELECT_ITME)
	//{
	//	// ���̏��\��
	//	DrawItemInfo(
	//		g_ItemSet.size,
	//		g_ItemSet.nBlock_Width,
	//		g_ItemSet.nBlock_Depth,
	//		g_ItemSet.nType);
	//}
#endif

}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCntItem;
	int nCountDirect;
	int nCountWidth;

	for (nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		if (g_aitem[nCntItem].bUse == true)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aitem[nCntItem].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aitem[nCntItem].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_aitem[nCntItem].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aitem[nCntItem].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

			// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aitem[nCntItem].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_aitem[nCntItem].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_aitem[nCntItem].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_aitem[nCntItem].OriginBlock.x + 
							(g_aitem[nCntItem].size.x * nCountWidth),
							g_aitem[nCntItem].OriginBlock.y -
							(g_aitem[nCntItem].size.y * nCountDirect),
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
			g_aitem[nCntItem].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aitem[nCntItem].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_aitem[nCntItem].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + g_aitem[nCntItem].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_aitem[nCntItem].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] =
						nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + 
						nCountWidth + g_aitem[nCntItem].nBlock_Width + 1;

					pIdx[1] =
						nCountDirect *
						(g_aitem[nCntItem].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_aitem[nCntItem].nBlock_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + g_aitem[nCntItem].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_aitem[nCntItem].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// �A�C�e���̓����蔻����
// ------------------------------------------------------------------------------------------
bool ColisionItem(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, ITME ** ppItemRet)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntItem;			// �J�E���g��

	// �Ԓn��񂪂�������
	if (ppItemRet != NULL)
	{
		*ppItemRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		// �g�p���
		if (g_aitem[nCntItem].bUse == true)
		{
			
			// �u���b�N��Y�͈�
			if (pos->y - size->y < g_aitem[nCntItem].pos.y + g_aitem[nCntItem].size.y&&
				pos->y + size->y > g_aitem[nCntItem].pos.y)
			{
				// �u���b�N��Z�͈�
				if (pos->z + size->z > g_aitem[nCntItem].pos.z - (g_aitem[nCntItem].size.x * sinf(g_aitem[nCntItem].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_aitem[nCntItem].pos.z + (g_aitem[nCntItem].size.x * sinf(g_aitem[nCntItem].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��)
					if (pos->x + size->z >
						g_aitem[nCntItem].pos.x&&
						posOld->x + size->z <=
						g_aitem[nCntItem].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppItemRet = &g_aitem[nCntItem];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_aitem[nCntItem].pos.x - size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

					// �u���b�N��Z�͈�
				if (pos->z + size->z > g_aitem[nCntItem].pos.z + (g_aitem[nCntItem].size.x * sinf(g_aitem[nCntItem].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_aitem[nCntItem].pos.z -(g_aitem[nCntItem].size.x * sinf(g_aitem[nCntItem].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(�E)
					if (pos->x - size->z <
						g_aitem[nCntItem].pos.x&&
						posOld->x - size->z >=
						g_aitem[nCntItem].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppItemRet = &g_aitem[nCntItem];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_aitem[nCntItem].pos.x + size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

				// �u���b�N��X�͈�
				if (pos->x + size->z > g_aitem[nCntItem].pos.x - (g_aitem[nCntItem].size.x * cosf(g_aitem[nCntItem].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_aitem[nCntItem].pos.x + (g_aitem[nCntItem].size.x * cosf(g_aitem[nCntItem].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��O)
					if (pos->z + size->z > g_aitem[nCntItem].pos.z&&
						posOld->z + size->z <= g_aitem[nCntItem].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppItemRet = &g_aitem[nCntItem];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_aitem[nCntItem].pos.z - size->z;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}
				}

					// �u���b�N��X�͈�
				if (pos->x + size->z > g_aitem[nCntItem].pos.x + (g_aitem[nCntItem].size.x * cosf(g_aitem[nCntItem].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_aitem[nCntItem].pos.x - (g_aitem[nCntItem].size.x * cosf(g_aitem[nCntItem].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(��)
					if (pos->z - size->z < g_aitem[nCntItem].pos.z&&
						posOld->z - size->z >= g_aitem[nCntItem].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppItemRet = &g_aitem[nCntItem];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_aitem[nCntItem].pos.z + size->z;

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
void NotDispItem(void)
{
	int nCntItem;

	// ���̃J�E���g
	for (nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		// �z�u�p�̏�
		if (g_aitem[nCntItem].bUse == false &&
			g_aitem[nCntItem].bDisp == true)
		{
			g_aitem[nCntItem].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u����
// ------------------------------------------------------------------------------------------
void SaveItem(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntItem = 0;
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
	char *cItemType[ITMETYPE_MAX] =
	{
		{ "    ----------[00]:�A�C�e��1----------" },				// 00_���P
	};

	// ���f�[�^�̎��
	char *cItemData[] =
	{
		{ "TYPE" },			// �^�C�v
		{ "POS" },			// �ʒu
		{ "ROT" },			// ��]
		{ "BLOCK_DEPTH" },	// �c�u���b�N
		{ "BLOCK_WIDTH" },	// ���u���b�N
		{ "XSIZE" },		// �w�T�C�Y
		{ "YSIZE" },		// Z�T�C�Y
		{ "END_ITMESET" }	// �G���h�t���A�[
	};

	// �t�@�C���J
	pFile = fopen(g_cItemFIleName_3D[GetGameStage()], "w");

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
				if (nCntItem < ALL_MAX - 1)
				{
					if (g_aitem[nCntItem].bUse == true)
					{
						fputs(cEnter, pFile);

						// ���f���̃p�[�c�̖��O����
						strcpy(cWriteText, cItemType[g_aitem[nCntItem].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    ITMESET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_ITMESET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cItemData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_aitem[nCntItem].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aitem[nCntItem].pos.x,
										g_aitem[nCntItem].pos.y,
										g_aitem[nCntItem].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aitem[nCntItem].rot.x,
										g_aitem[nCntItem].rot.y,
										g_aitem[nCntItem].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �c�u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_aitem[nCntItem].nBlock_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ���u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_aitem[nCntItem].nBlock_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �w�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_aitem[nCntItem].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Z�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "YSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_aitem[nCntItem].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �G���h���Z�b�g
								else if (strcmp(cWriteText, "END_ITMESET") == 0)
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
					nCntItem++;
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
// �A�C�e���ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadItem(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntItem = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �����\�����Ă�����̂���������
	for (nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		if (g_aitem[nCntItem].bUse == true)
		{
			g_aitem[nCntItem].bUse = false;
			g_aitem[nCntItem].bDisp = false;
		}
	}

	// �J�E���g������
	nCntItem = 0;

	// �t�@�C���J
	pFile = fopen(g_cItemFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "ITMESET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_ITMESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aitem[nCntItem].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_aitem[nCntItem].pos.x, &g_aitem[nCntItem].pos.y, &g_aitem[nCntItem].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_aitem[nCntItem].rot.x, &g_aitem[nCntItem].rot.y, &g_aitem[nCntItem].rot.z);
						}

						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aitem[nCntItem].nBlock_Depth);
						}

						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aitem[nCntItem].nBlock_Width);
						}

						// x�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_aitem[nCntItem].size.x);
						}

						// y�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "YSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_aitem[nCntItem].size.y);
						}

					}

					// ���_�̌v�Z
					g_aitem[nCntItem].OriginBlock = D3DXVECTOR3(
						g_aitem[nCntItem].size.x * -0.5f * g_aitem[nCntItem].nBlock_Width,
						g_aitem[nCntItem].size.y * g_aitem[nCntItem].nBlock_Depth,
						0.0f);

					// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
					g_aitem[nCntItem].nNumberVertexMeshField =
						(g_aitem[nCntItem].nBlock_Depth + 1) * 
						(g_aitem[nCntItem].nBlock_Width + 1);

					g_aitem[nCntItem].nNumIndex = 
						(g_aitem[nCntItem].nBlock_Width + 1) * 2 * 
						g_aitem[nCntItem].nBlock_Depth + 2 *
						(g_aitem[nCntItem].nBlock_Depth - 1);

					g_aitem[nCntItem].nNumPolygon =
						g_aitem[nCntItem].nBlock_Depth * g_aitem[nCntItem].nBlock_Width * 2 + 
						4 * (g_aitem[nCntItem].nBlock_Depth - 1);

					// �g�p��ԁE�\�����
					g_aitem[nCntItem].bUse = true;
					g_aitem[nCntItem].bDisp = true;

					// ���J�E���g�̍X�V
					nCntItem++;
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
void SetItem(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	int nCntItem;		// �J�E���g��
	int nCountDirect;	// �c�u���b�N�̃J�E���g
	int nCountWidth;	// ���u���b�N�̃J�E���g

						// ���̐ݒu������
	for (nCntItem = 0; nCntItem < ALL_MAX; nCntItem++)
	{
		// ���g�p�̏�
		if (g_aitem[nCntItem].bUse == false)
		{
			// ����ϐ��ɑ��
			g_aitem[nCntItem].pos = pos;
			g_aitem[nCntItem].rot = rot;
			g_aitem[nCntItem].size = size;
			g_aitem[nCntItem].nBlock_Depth = nBlock_Depth;
			g_aitem[nCntItem].nBlock_Width = nBlock_Width;
			g_aitem[nCntItem].nType = nType;

			// ���_�̌v�Z
			g_aitem[nCntItem].OriginBlock = D3DXVECTOR3(
				g_aitem[nCntItem].size.x * -0.5f * g_aitem[nCntItem].nBlock_Width,
				g_aitem[nCntItem].size.y * g_aitem[nCntItem].nBlock_Depth,
				0.0f);

			// �����_�̌v�Z
			g_aitem[nCntItem].nNumberVertexMeshField =
				(g_aitem[nCntItem].nBlock_Depth + 1) *
				(g_aitem[nCntItem].nBlock_Width + 1);

			// �C���f�b�N�X�̌v�Z
			g_aitem[nCntItem].nNumIndex =
				(g_aitem[nCntItem].nBlock_Width + 1) * 2 *
				g_aitem[nCntItem].nBlock_Depth +
				2 * (g_aitem[nCntItem].nBlock_Depth - 1);

			// �|���S���̌v�Z
			g_aitem[nCntItem].nNumPolygon =
				g_aitem[nCntItem].nBlock_Depth *
				g_aitem[nCntItem].nBlock_Width * 2 +
				4 * (g_aitem[nCntItem].nBlock_Depth - 1);

			g_aitem[nCntItem].nNumber = 0;

			// �g�p��ԁE�\�����
			g_aitem[nCntItem].bUse = true;
			g_aitem[nCntItem].bDisp = true;

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aitem[nCntItem].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aitem[nCntItem].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_aitem[nCntItem].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aitem[nCntItem].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

								// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

								// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aitem[nCntItem].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_aitem[nCntItem].nBlock_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_aitem[nCntItem].nBlock_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_aitem[nCntItem].OriginBlock.x + (g_aitem[nCntItem].size.x * nCountWidth),
							g_aitem[nCntItem].OriginBlock.y - (g_aitem[nCntItem].size.y * nCountDirect),
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
			g_aitem[nCntItem].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aitem[nCntItem].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_aitem[nCntItem].nBlock_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + g_aitem[nCntItem].nBlock_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_aitem[nCntItem].nBlock_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + nCountWidth + g_aitem[nCntItem].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_aitem[nCntItem].nBlock_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aitem[nCntItem].nBlock_Width + 1) + g_aitem[nCntItem].nBlock_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_aitem[nCntItem].pIndex->Unlock();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �A�C�e�����
// ------------------------------------------------------------------------------------------
ITME *GetItem(void)
{
	return &g_aitem[0];
}