// ------------------------------------------------------------------------------------------
//
// �G���� [enemy.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "enemy.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define ENEMY_PHONT "data/TEXTURE/woods.png"
#define ENEMY_SIZE (10)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	ENEMYTYPE_00_ROAD1 = 0,
	ENEMYTYPE_MAX
} ENEMYTYPE;
// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice);
bool ColisionEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, ENEMY ** ppEnemyRet);

void Update_ChangeEnemy(void);
void Update_DispEnemy(void);
void Update_DestroyEnemy(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;	// ���_�e�N�X�`���[�̃|�C���^

ENEMY g_aenemy[ALL_MAX];							// �G
ENEMYSET g_EnemySet;							// �z�u���

// 3D�p�̃Z�[�u�t�@�C��
const char g_cEnemyFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/enemy.txt",
	"data/SAVE/3D/Stage_2/enemy.txt",
	"data/SAVE/3D/Stage_3/enemy.txt",
	"data/SAVE/3D/Stage_4/enemy.txt",
	"data/SAVE/3D/Stage_5/enemy.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		g_aenemy[nCntEnemy].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aenemy[nCntEnemy].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aenemy[nCntEnemy].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aenemy[nCntEnemy].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_aenemy[nCntEnemy].nEnemy_Depth	= 1;
		g_aenemy[nCntEnemy].nEnemy_Width	= 1;

		// ���_�̌v�Z
		g_aenemy[nCntEnemy].OriginEnemy	= D3DXVECTOR3(
			g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
			g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
			0.0f);

		// �����_�̌v�Z
		g_aenemy[nCntEnemy].nNumberVertexMeshField = 
			(g_aenemy[nCntEnemy].nEnemy_Depth + 1) *
			(g_aenemy[nCntEnemy].nEnemy_Width + 1);

		// �C���f�b�N�X�̌v�Z
		g_aenemy[nCntEnemy].nNumIndex = 
			(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 *
			g_aenemy[nCntEnemy].nEnemy_Depth + 
			2 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

		// �|���S���̌v�Z
		g_aenemy[nCntEnemy].nNumPolygon = 
			g_aenemy[nCntEnemy].nEnemy_Depth * 
			g_aenemy[nCntEnemy].nEnemy_Width * 2 + 
			4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

		g_aenemy[nCntEnemy].nNumber = 0;

		// �g�p�E�\����Ԃ̏�����
		g_aenemy[nCntEnemy].bUse = false;
		g_aenemy[nCntEnemy].bDisp = false;
	}

	// �z�u���̏�����
	g_EnemySet.nEnemy_Depth = 1;
	g_EnemySet.nEnemy_Width = 1;
	g_EnemySet.nType = 0;
	g_EnemySet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_EnemySet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ENEMY_PHONT, &g_pTextureEnemy);

	// �G�̃f�[�^�Ǎ�
	LoadEnemy();

	// ���_���̍쐬
	MakeVertexEnemy(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitEnemy(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// ���_�o�b�t�@�̊J��
		if (g_aenemy[nCntEnemy].pVtxBuff != NULL)
		{
			g_aenemy[nCntEnemy].pVtxBuff->Release();
			g_aenemy[nCntEnemy].pVtxBuff = NULL;
		}

		// ���_�C���f�b�N�X�̊J��
		if (g_aenemy[nCntEnemy].pIndex != NULL)
		{
			g_aenemy[nCntEnemy].pIndex->Release();
			g_aenemy[nCntEnemy].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// �}�E�X�̏��擾
	pMouseX = GetMouseX();

	// �폜����
	Update_DestroyEnemy();

	// ���̕ω�����
	Update_ChangeEnemy();

	// �z�u���̏ꏊ�c��
	Update_DispEnemy();

	// �͈�
	if (pMouseX < 1000.0f)
	{
		// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
		if (GetMousePressTrigger(0))
		{
			SetEnemy(
				pArrangement->pos,
				pArrangement->rot,
				g_EnemySet.size,
				g_EnemySet.nEnemy_Depth,
				g_EnemySet.nEnemy_Width,
				g_EnemySet.nType);
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���̕ω�����
// ------------------------------------------------------------------------------------------
void Update_ChangeEnemy(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �c�̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_EnemySet.nEnemy_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_EnemySet.nEnemy_Depth--;
	}

	// ���̃u���b�N���𑝌�
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_EnemySet.nEnemy_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_EnemySet.nEnemy_Width--;
	}

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_EnemySet.size.y += ENEMY_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_EnemySet.size.y -= ENEMY_SIZE;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_EnemySet.size.x += ENEMY_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_EnemySet.size.x -= ENEMY_SIZE;
	}

	// ��]�ʂ𑝌�
	if (GetKeyboardSpeacePress(DIK_T))
	{
		g_EnemySet.rot.y += 1.57f;

		// -3.14�`3.14�ɉ�]�ʂ��Œ肳����
		if (g_EnemySet.rot.y < -D3DX_PI) g_EnemySet.rot.y += D3DX_PI * 2;		// -3.14����

		else if (g_EnemySet.rot.y > D3DX_PI) g_EnemySet.rot.y += -D3DX_PI * 2;	// 3.14����

	}

	else if (GetKeyboardSpeacePress(DIK_G))
	{
		g_EnemySet.rot.y -= 1.57f;

		// -3.14�`3.14�ɉ�]�ʂ��Œ肳����
		if (g_EnemySet.rot.y < -D3DX_PI) g_EnemySet.rot.y += D3DX_PI * 2;		// -3.14����

		else if (g_EnemySet.rot.y > D3DX_PI) g_EnemySet.rot.y += -D3DX_PI * 2;	// 3.14����
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_EnemySet.nType++;

		// ���E�l�܂ōs������
		if (g_EnemySet.nType >= ENEMYTYPE_MAX)
		{
			// ������
			g_EnemySet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�̎��̏ꏊ�c������
// ------------------------------------------------------------------------------------------
void Update_DispEnemy(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
												// �ϐ��錾
	int nCountDirect;
	int nCountWidth;

	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == false)
		{
			g_aenemy[nCntEnemy].nType = g_EnemySet.nType;

			// �g�p��ԁE�\�����
			g_aenemy[nCntEnemy].bDisp = true;

			// �z�u�̈ʒu�����擾
			g_aenemy[nCntEnemy].pos = pArrangement->pos;

			// ��]�����擾
			g_aenemy[nCntEnemy].rot = pArrangement->rot;

			if (g_aenemy[nCntEnemy].nEnemy_Depth != g_EnemySet.nEnemy_Depth ||
				g_aenemy[nCntEnemy].nEnemy_Width != g_EnemySet.nEnemy_Width ||
				g_aenemy[nCntEnemy].size != g_EnemySet.size)
			{
				g_aenemy[nCntEnemy].nEnemy_Depth = g_EnemySet.nEnemy_Depth;
				g_aenemy[nCntEnemy].nEnemy_Width = g_EnemySet.nEnemy_Width;
				g_aenemy[nCntEnemy].size = g_EnemySet.size;

				// ���_�̌v�Z
				g_aenemy[nCntEnemy].OriginEnemy = D3DXVECTOR3(
					g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
					g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
					0.0f);

				// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
				g_aenemy[nCntEnemy].nNumberVertexMeshField =
					(g_aenemy[nCntEnemy].nEnemy_Depth + 1) *
					(g_aenemy[nCntEnemy].nEnemy_Width + 1);

				g_aenemy[nCntEnemy].nNumIndex =
					(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 *
					g_aenemy[nCntEnemy].nEnemy_Depth + 2 *
					(g_aenemy[nCntEnemy].nEnemy_Depth - 1);

				g_aenemy[nCntEnemy].nNumPolygon =
					g_aenemy[nCntEnemy].nEnemy_Depth * g_aenemy[nCntEnemy].nEnemy_Width * 2 +
					4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_aenemy[nCntEnemy].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_aenemy[nCntEnemy].pVtxBuff, NULL);

				// �C���f�b�N�X�o�b�t�@�̐���
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_aenemy[nCntEnemy].nNumIndex,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_aenemy[nCntEnemy].pIndex,
					NULL);

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

									// �C���f�b�N�X���̐ݒ�
				WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_aenemy[nCntEnemy].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

				//���_�ݒ� //
				//�s���[�v
				for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth + 1; nCountDirect++)
				{
					// �񃋁[�v
					for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
					{
						// ���_���W�̐ݒ�
						pVtx[0].pos =
							D3DXVECTOR3(
								g_aenemy[nCntEnemy].OriginEnemy.x + 
								(g_aenemy[nCntEnemy].size.x * nCountWidth),
								g_aenemy[nCntEnemy].OriginEnemy.y - 
								(g_aenemy[nCntEnemy].size.y * nCountDirect),
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
				g_aenemy[nCntEnemy].pVtxBuff->Unlock();

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
				g_aenemy[nCntEnemy].pIndex->Lock(0, 0, (void **)&pIdx, 0);

				// �c�u���b�N��
				for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth; nCountDirect++)
				{
					// �Q��ڂ̃��[�v�ȍ~
					if (nCountDirect >= 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = 
							nCountDirect * 
							(g_aenemy[nCntEnemy].nEnemy_Width + 1) +
							g_aenemy[nCntEnemy].nEnemy_Width + 1;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}

					// ���u���b�N�̒��_��
					for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
					{
						// �`�揇�Ԃ̃C���f�b�N�X
						pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth + g_aenemy[nCntEnemy].nEnemy_Width + 1;
						pIdx[1] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx += 2;
					}

					// �k�ރ|���S�������K�v������ꍇ
					if (nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth - 1)
					{
						// �k�ރ|���S�����̒��_�ǉ�
						pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width;

						// �C���f�b�N�X�̃|�C���g���킹
						pIdx++;
					}
				}

				// �A�����b�N
				g_aenemy[nCntEnemy].pIndex->Unlock();
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void Update_DestroyEnemy(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == true && g_aenemy[nCntEnemy].bDisp == true)
		{
			// �u���b�N��X�͈�
			if (pArrangement->pos.x >
				g_aenemy[nCntEnemy].pos.x - sinf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.x * 0.5f * g_aenemy[nCntEnemy].nEnemy_Width&&
				pArrangement->pos.x <
				g_aenemy[nCntEnemy].pos.x + sinf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.x * 0.5f * g_aenemy[nCntEnemy].nEnemy_Width&&
				pArrangement->pos.z >
				g_aenemy[nCntEnemy].pos.z - cosf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.z * 0.5f * g_aenemy[nCntEnemy].nEnemy_Depth&&
				pArrangement->pos.z <
				g_aenemy[nCntEnemy].pos.z + cosf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.z * 0.5f * g_aenemy[nCntEnemy].nEnemy_Depth)
			{
				// �폜����
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_aenemy[nCntEnemy].bUse = false;
					g_aenemy[nCntEnemy].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// �J�E���g�G
	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bDisp == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_aenemy[nCntEnemy].mtxWorldEnemy);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// �����̓��ꕨ
				g_aenemy[nCntEnemy].rot.y, g_aenemy[nCntEnemy].rot.x, g_aenemy[nCntEnemy].rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_aenemy[nCntEnemy].mtxWorldEnemy,	// 1
				&g_aenemy[nCntEnemy].mtxWorldEnemy,				// 2
				&mtxRot);							// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,												// �����̓��ꕨ
				g_aenemy[nCntEnemy].pos.x, g_aenemy[nCntEnemy].pos.y, g_aenemy[nCntEnemy].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_aenemy[nCntEnemy].mtxWorldEnemy,	// 1
				&g_aenemy[nCntEnemy].mtxWorldEnemy,				// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aenemy[nCntEnemy].mtxWorldEnemy);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_aenemy[nCntEnemy].pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
			pDevice->SetIndices(g_aenemy[nCntEnemy].pIndex);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemy);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aenemy[nCntEnemy].nNumberVertexMeshField,
				0,
				g_aenemy[nCntEnemy].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	//if (GetGameSelect() == GAMESELECT_ENEMY)
	//{
	//	// ���̏��\��
	//	DrawEnemyInfo(
	//		g_EnemySet.size,
	//		g_EnemySet.nEnemy_Width,
	//		g_EnemySet.nEnemy_Depth,
	//		g_EnemySet.nType);
	//}
#endif

}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCntEnemy;
	int nCountDirect;
	int nCountWidth;

	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == true)
		{
			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aenemy[nCntEnemy].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_aenemy[nCntEnemy].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

			// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aenemy[nCntEnemy].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_aenemy[nCntEnemy].OriginEnemy.x + 
							(g_aenemy[nCntEnemy].size.x * nCountWidth),
							g_aenemy[nCntEnemy].OriginEnemy.y -
							(g_aenemy[nCntEnemy].size.y * nCountDirect),
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
			g_aenemy[nCntEnemy].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aenemy[nCntEnemy].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] =
						nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + 
						nCountWidth + g_aenemy[nCntEnemy].nEnemy_Width + 1;

					pIdx[1] =
						nCountDirect *
						(g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_aenemy[nCntEnemy].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// �G�̓����蔻����
// ------------------------------------------------------------------------------------------
bool ColisionEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, ENEMY ** ppEnemyRet)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntEnemy;			// �J�E���g��

	// �Ԓn��񂪂�������
	if (ppEnemyRet != NULL)
	{
		*ppEnemyRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// �g�p���
		if (g_aenemy[nCntEnemy].bUse == true)
		{
			
			// �u���b�N��Y�͈�
			if (pos->y - size->y < g_aenemy[nCntEnemy].pos.y + g_aenemy[nCntEnemy].size.y&&
				pos->y + size->y > g_aenemy[nCntEnemy].pos.y)
			{
				// �u���b�N��Z�͈�
				if (pos->z + size->z > g_aenemy[nCntEnemy].pos.z - (g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_aenemy[nCntEnemy].pos.z + (g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��)
					if (pos->x + size->z >
						g_aenemy[nCntEnemy].pos.x&&
						posOld->x + size->z <=
						g_aenemy[nCntEnemy].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_aenemy[nCntEnemy].pos.x - size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

					// �u���b�N��Z�͈�
				if (pos->z + size->z > g_aenemy[nCntEnemy].pos.z + (g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)) &&	// z�����̃T�C�Y
					pos->z - size->z < g_aenemy[nCntEnemy].pos.z -(g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(�E)
					if (pos->x - size->z <
						g_aenemy[nCntEnemy].pos.x&&
						posOld->x - size->z >=
						g_aenemy[nCntEnemy].pos.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_aenemy[nCntEnemy].pos.x + size->z;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

				// �u���b�N��X�͈�
				if (pos->x + size->z > g_aenemy[nCntEnemy].pos.x - (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_aenemy[nCntEnemy].pos.x + (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)))	// z�����̃T�C�Y)
				{
					// �����蔻��(��O)
					if (pos->z + size->z > g_aenemy[nCntEnemy].pos.z&&
						posOld->z + size->z <= g_aenemy[nCntEnemy].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_aenemy[nCntEnemy].pos.z - size->z;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}
				}

					// �u���b�N��X�͈�
				if (pos->x + size->z > g_aenemy[nCntEnemy].pos.x + (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)) &&	// z�����̃T�C�Y
					pos->x - size->z < g_aenemy[nCntEnemy].pos.x - (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)))	// z�����̃T�C�Y)
				{

					// �����蔻��(��)
					if (pos->z - size->z < g_aenemy[nCntEnemy].pos.z&&
						posOld->z - size->z >= g_aenemy[nCntEnemy].pos.z)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_aenemy[nCntEnemy].pos.z + size->z;

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
void NotDispEnemy(void)
{
	int nCntEnemy;

	// ���̃J�E���g
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// �z�u�p�̏�
		if (g_aenemy[nCntEnemy].bUse == false &&
			g_aenemy[nCntEnemy].bDisp == true)
		{
			g_aenemy[nCntEnemy].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u����
// ------------------------------------------------------------------------------------------
void SaveEnemy(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntEnemy = 0;
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
	char *cEnemyType[ENEMYTYPE_MAX] =
	{
		{ "    ----------[00]:�G1----------" },				// 00_���P
	};

	// ���f�[�^�̎��
	char *cEnemyData[] =
	{
		{ "TYPE" },			// �^�C�v
		{ "POS" },			// �ʒu
		{ "ROT" },			// ��]
		{ "BLOCK_DEPTH" },	// �c�u���b�N
		{ "BLOCK_WIDTH" },	// ���u���b�N
		{ "XSIZE" },		// �w�T�C�Y
		{ "YSIZE" },		// Z�T�C�Y
		{ "END_ENEMYSET" }	// �G���h�t���A�[
	};

	// �t�@�C���J
	pFile = fopen(g_cEnemyFIleName_3D[GetGameStage()], "w");

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
				if (nCntEnemy < ALL_MAX - 1)
				{
					if (g_aenemy[nCntEnemy].bUse == true)
					{
						fputs(cEnter, pFile);

						// ���f���̃p�[�c�̖��O����
						strcpy(cWriteText, cEnemyType[g_aenemy[nCntEnemy].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    ENEMYSET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_ENEMYSET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cEnemyData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_aenemy[nCntEnemy].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].pos.x,
										g_aenemy[nCntEnemy].pos.y,
										g_aenemy[nCntEnemy].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].rot.x,
										g_aenemy[nCntEnemy].rot.y,
										g_aenemy[nCntEnemy].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �c�u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].nEnemy_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ���u���b�N�����ۑ�
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].nEnemy_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �w�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Z�T�C�Y���ۑ�
								else if (strcmp(cWriteText, "YSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �G���h���Z�b�g
								else if (strcmp(cWriteText, "END_ENEMYSET") == 0)
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
					nCntEnemy++;
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
// �G�ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadEnemy(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntEnemy = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �����\�����Ă�����̂���������
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == true)
		{
			g_aenemy[nCntEnemy].bUse = false;
			g_aenemy[nCntEnemy].bDisp = false;
		}
	}

	// �J�E���g������
	nCntEnemy = 0;

	// �t�@�C���J
	pFile = fopen(g_cEnemyFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "ENEMYSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_ENEMYSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aenemy[nCntEnemy].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_aenemy[nCntEnemy].pos.x, &g_aenemy[nCntEnemy].pos.y, &g_aenemy[nCntEnemy].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_aenemy[nCntEnemy].rot.x, &g_aenemy[nCntEnemy].rot.y, &g_aenemy[nCntEnemy].rot.z);
						}

						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aenemy[nCntEnemy].nEnemy_Depth);
						}

						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aenemy[nCntEnemy].nEnemy_Width);
						}

						// x�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_aenemy[nCntEnemy].size.x);
						}

						// y�T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "YSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_aenemy[nCntEnemy].size.y);
						}

					}

					// ���_�̌v�Z
					g_aenemy[nCntEnemy].OriginEnemy = D3DXVECTOR3(
						g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
						g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
						0.0f);

					// �����_���E�C���f�b�N�X�E�|���S���̐ݒ�(�v�Z)
					g_aenemy[nCntEnemy].nNumberVertexMeshField =
						(g_aenemy[nCntEnemy].nEnemy_Depth + 1) * 
						(g_aenemy[nCntEnemy].nEnemy_Width + 1);

					g_aenemy[nCntEnemy].nNumIndex = 
						(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 * 
						g_aenemy[nCntEnemy].nEnemy_Depth + 2 *
						(g_aenemy[nCntEnemy].nEnemy_Depth - 1);

					g_aenemy[nCntEnemy].nNumPolygon =
						g_aenemy[nCntEnemy].nEnemy_Depth * g_aenemy[nCntEnemy].nEnemy_Width * 2 + 
						4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

					// �g�p��ԁE�\�����
					g_aenemy[nCntEnemy].bUse = true;
					g_aenemy[nCntEnemy].bDisp = true;

					// ���J�E���g�̍X�V
					nCntEnemy++;
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
void SetEnemy(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nEnemy_Depth,
	int	nEnemy_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	int nCntEnemy;		// �J�E���g��
	int nCountDirect;	// �c�u���b�N�̃J�E���g
	int nCountWidth;	// ���u���b�N�̃J�E���g

						// ���̐ݒu������
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// ���g�p�̏�
		if (g_aenemy[nCntEnemy].bUse == false)
		{
			// ����ϐ��ɑ��
			g_aenemy[nCntEnemy].pos = pos;
			g_aenemy[nCntEnemy].rot = rot;
			g_aenemy[nCntEnemy].size = size;
			g_aenemy[nCntEnemy].nEnemy_Depth = nEnemy_Depth;
			g_aenemy[nCntEnemy].nEnemy_Width = nEnemy_Width;
			g_aenemy[nCntEnemy].nType = nType;

			// ���_�̌v�Z
			g_aenemy[nCntEnemy].OriginEnemy = D3DXVECTOR3(
				g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
				g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
				0.0f);

			// �����_�̌v�Z
			g_aenemy[nCntEnemy].nNumberVertexMeshField =
				(g_aenemy[nCntEnemy].nEnemy_Depth + 1) *
				(g_aenemy[nCntEnemy].nEnemy_Width + 1);

			// �C���f�b�N�X�̌v�Z
			g_aenemy[nCntEnemy].nNumIndex =
				(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 *
				g_aenemy[nCntEnemy].nEnemy_Depth +
				2 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

			// �|���S���̌v�Z
			g_aenemy[nCntEnemy].nNumPolygon =
				g_aenemy[nCntEnemy].nEnemy_Depth *
				g_aenemy[nCntEnemy].nEnemy_Width * 2 +
				4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

			g_aenemy[nCntEnemy].nNumber = 0;

			// �g�p��ԁE�\�����
			g_aenemy[nCntEnemy].bUse = true;
			g_aenemy[nCntEnemy].bDisp = true;

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aenemy[nCntEnemy].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pVtxBuff, NULL);

			// �C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_aenemy[nCntEnemy].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pIndex,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

								// �C���f�b�N�X���̐ݒ�
			WORD * pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾

								// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aenemy[nCntEnemy].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//���_�ݒ� //
			//�s���[�v
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth + 1; nCountDirect++)
			{
				// �񃋁[�v
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// ���_���W�̐ݒ�
					pVtx[0].pos =
						D3DXVECTOR3(
							g_aenemy[nCntEnemy].OriginEnemy.x + (g_aenemy[nCntEnemy].size.x * nCountWidth),
							g_aenemy[nCntEnemy].OriginEnemy.y - (g_aenemy[nCntEnemy].size.y * nCountDirect),
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
			g_aenemy[nCntEnemy].pVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_aenemy[nCntEnemy].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// �c�u���b�N��
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth; nCountDirect++)
			{
				// �Q��ڂ̃��[�v�ȍ~
				if (nCountDirect >= 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width + 1;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}

				// ���u���b�N�̒��_��
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// �`�揇�Ԃ̃C���f�b�N�X
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth + g_aenemy[nCntEnemy].nEnemy_Width + 1;
					pIdx[1] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx += 2;
				}

				// �k�ރ|���S�������K�v������ꍇ
				if (nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth - 1)
				{
					// �k�ރ|���S�����̒��_�ǉ�
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width;

					// �C���f�b�N�X�̃|�C���g���킹
					pIdx++;
				}
			}

			// �A�����b�N
			g_aenemy[nCntEnemy].pIndex->Unlock();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �G���
// ------------------------------------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aenemy[0];
}