// ------------------------------------------------------------------------------------------
//
// �|���S������ [billboard.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "Billboard.h"
#include "input.h"
#include "arrangement.h"
#include "mouse.h"
#include "game.h"

// �����蔻��p	//
#include "giraffe.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define BILLBOARD_SIZE (10)
#define BILLBOARD_WOOD "data/TEXTURE/materials/green_tree000.png"

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	BILLBOARDTYPE_WOOD = 0,
	BILLBOARDTYPE_MAX
} BILLBOARDTYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
void Update_ChangeBillboard(void);
void Update_DispBillboard(void);
void Update_DestroyBillboard(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_paTextureBillboard[BILLBOARDTYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^
BILLBOARD g_aBillboard[ALL_MAX];									// �r���{�[�h
BILLBOARDSET g_BillboardSet;											// �r���{�[�h�̐ݒ�

// 3D�p�̃Z�[�u�t�@�C��
const char g_cBillboardFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/billboard.txt",
	"data/SAVE/3D/Stage_2/billboard.txt",
	"data/SAVE/3D/Stage_3/billboard.txt",
	"data/SAVE/3D/Stage_4/billboard.txt",
	"data/SAVE/3D/Stage_5/billboard.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// �ʒu�E��]�̏����ݒ�
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].nType = BILLBOARDTYPE_WOOD;
		g_aBillboard[nCntBillboard].bUse = false;
		g_aBillboard[nCntBillboard].bDisp = false;
	}

	// �z�u���̏�����
	g_BillboardSet.nType = 0;
	g_BillboardSet.size = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BILLBOARD_WOOD, &g_paTextureBillboard[0]);

	// �r���{�[�h��ǂݍ���
	LoadBillboard();

	// ���_���̍쐬
	MakeVertexBillboard(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitBillboard(void)
{
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARDTYPE_MAX; nCntBillboard++)
	{
		// �e�N�X�`���̊J��
		if (g_paTextureBillboard[nCntBillboard] != NULL)
		{
			g_paTextureBillboard[nCntBillboard]->Release();
			g_paTextureBillboard[nCntBillboard] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateBillboard(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// �}�E�X�̏��擾
	pMouseX = GetMouseX();

	// �폜����
	Update_DestroyBillboard();

	// ���̕ω�����
	Update_ChangeBillboard();

	// �z�u���̏ꏊ�c��
	Update_DispBillboard();

	// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
	if (GetMousePressTrigger(0))
	{
		SetBillboard(
			pArrangement->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			g_BillboardSet.size,
			g_BillboardSet.nType);
	}
}

// ------------------------------------------------------------------------------------------
// ���̕ω�����
// ------------------------------------------------------------------------------------------
void Update_ChangeBillboard(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �c�̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_BillboardSet.size.y += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_BillboardSet.size.y -= 50.0f;
	}

	// ���̃T�C�Y�𑝌�
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_BillboardSet.size.x += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_BillboardSet.size.x -= 50.0f;
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_BillboardSet.nType++;

		// ���E�l�܂ōs������
		if (g_BillboardSet.nType >= BILLBOARDTYPE_MAX)
		{
			// ������
			g_BillboardSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�̎��̏ꏊ�c������
// ------------------------------------------------------------------------------------------
void Update_DispBillboard(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// �ϐ��錾
	int nCntBillboard;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{
			g_aBillboard[nCntBillboard].nType = g_BillboardSet.nType;
			g_aBillboard[nCntBillboard].size = g_BillboardSet.size;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// �l�ڂ̒��_

			g_aBillboard[nCntBillboard].bDisp = true;

			// �z�u�̈ʒu�����擾
			g_aBillboard[nCntBillboard].pos = pArrangement->pos;

			break;

		}
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBillboard->Unlock();

}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void Update_DestroyBillboard(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true && g_aBillboard[nCntBillboard].bDisp == true)
		{
			// �u���b�N��X�͈�
			if (pArrangement->pos.x >
				g_aBillboard[nCntBillboard].pos.x - g_aBillboard[nCntBillboard].size.x * 0.5f &&
				pArrangement->pos.x <														  
				g_aBillboard[nCntBillboard].pos.x + g_aBillboard[nCntBillboard].size.x * 0.5f &&
				pArrangement->pos.z >														  
				g_aBillboard[nCntBillboard].pos.z - g_aBillboard[nCntBillboard].size.z * 0.5f &&
				pArrangement->pos.z <														  
				g_aBillboard[nCntBillboard].pos.z + g_aBillboard[nCntBillboard].size.z * 0.5f)
			{
				// �폜����
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_aBillboard[nCntBillboard].bUse = false;
					g_aBillboard[nCntBillboard].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	BILLBOARD *pBillboard = &g_aBillboard[0];

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// �r���[�}�g���b�N�X�̑���p
	D3DXMATRIX mtxView;

	// �A���t�@�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)

	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bDisp == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pBillboard->mtxWorldBillboard);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			pBillboard->mtxWorldBillboard._11 = mtxView._11;
			pBillboard->mtxWorldBillboard._12 = mtxView._21;
			pBillboard->mtxWorldBillboard._13 = mtxView._31;
			pBillboard->mtxWorldBillboard._21 = mtxView._12;
			pBillboard->mtxWorldBillboard._22 = mtxView._22;
			pBillboard->mtxWorldBillboard._23 = mtxView._32;

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pBillboard->rot.y, pBillboard->rot.x, pBillboard->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pBillboard->mtxWorldBillboard,	// 1
				&pBillboard->mtxWorldBillboard,					// 2
				&mtxRot);							// 3

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pBillboard->mtxWorldBillboard,	// 1
				&pBillboard->mtxWorldBillboard,					// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorldBillboard);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_paTextureBillboard[g_aBillboard[nCntBillboard].nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}

	// �A���t�@�e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void SetBillboard(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nType)
{
	VERTEX_3D * pVtx;	// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// �e���[�v
	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_aBillboard[nCntBillboard].bUse)
		{
			// ���X�V
			g_aBillboard[nCntBillboard].pos = pos;

			// �T�C�Y
			g_aBillboard[nCntBillboard].size = size;

			// �^�C�v
			g_aBillboard[nCntBillboard].nType = nType;

			// �g�p��Ԃ�
			g_aBillboard[nCntBillboard].bUse = true;

			// ���_���̍X�V
			pVtx += nCntBillboard * 4;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x, 
				g_aBillboard[nCntBillboard].size.y, 0);	// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x, 
				-g_aBillboard[nCntBillboard].size.y, 0);	// �l�ڂ̒��_

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * ALL_MAX, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffBillboard,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			-g_aBillboard[nCntBillboard].size.x,
			g_aBillboard[nCntBillboard].size.y, 0);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(
			g_aBillboard[nCntBillboard].size.x,
			g_aBillboard[nCntBillboard].size.y, 0);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(
			-g_aBillboard[nCntBillboard].size.x,
			-g_aBillboard[nCntBillboard].size.y, 0);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(
			g_aBillboard[nCntBillboard].size.x,
			-g_aBillboard[nCntBillboard].size.y, 0);	// �l�ڂ̒��_

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �l�ڂ̒��_

		// �J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �|�C���g���킹
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBillboard->Unlock();
}

// ------------------------------------------------------------------------------------------
// �r���{�[�h�ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadBillboard(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;		// �J�E���g���[�h
	int nCntBillboard = 0;		// �J�E���g��
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

							// �����\�����Ă�����̂���������
	for (nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			g_aBillboard[nCntBillboard].bUse = false;
			g_aBillboard[nCntBillboard].bDisp = false;
		}
	}

	// �J�E���g������
	nCntBillboard = 0;

	// �t�@�C���J
	pFile = fopen(g_cBillboardFIleName_3D[GetGameStage()], "r");

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
				if (strcmp(cHeadText, "BILLBOARDSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_BILLBOARDSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ��ޏ��ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].nType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].pos.x, 
								&g_aBillboard[nCntBillboard].pos.y,
								&g_aBillboard[nCntBillboard].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_aBillboard[nCntBillboard].rot.x,
								&g_aBillboard[nCntBillboard].rot.y,
								&g_aBillboard[nCntBillboard].rot.z);
						}

						// �T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].size.x, 
								&g_aBillboard[nCntBillboard].size.y,
								&g_aBillboard[nCntBillboard].size.z);
						}
					}

					// �g�p��ԁE�\�����
					g_aBillboard[nCntBillboard].bUse = true;
					g_aBillboard[nCntBillboard].bDisp = true;
					// ���J�E���g�̍X�V
					nCntBillboard++;
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
void NotDispBillboard(void)
{
	int nCntBillboard;

	// ���̃J�E���g
	for (nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// �z�u�p�̏�
		if (g_aBillboard[nCntBillboard].bUse == false &&
			g_aBillboard[nCntBillboard].bDisp == true)
		{
			g_aBillboard[nCntBillboard].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u����
// ------------------------------------------------------------------------------------------
void SaveBillboard(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntBillboard = 0;
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
	char *cBillboardType[BILLBOARDTYPE_MAX] =
	{
		{ "    ----------[00]:�؂P----------" },				// 00_���P
	};

	// ���f�[�^�̎��
	char *cBillboardData[] =
	{
		{ "TYPE" },			// �^�C�v
		{ "POS" },			// �ʒu
		{ "ROT" },			// ��]
		{ "SIZE" },		// �w�T�C�Y
		{ "END_BILLBOARDSET" }	// �G���h�t���A�[
	};

	// �t�@�C���J
	pFile = fopen(g_cBillboardFIleName_3D[GetGameStage()], "w");

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
				if (nCntBillboard < ALL_MAX - 1)
				{
					if (g_aBillboard[nCntBillboard].bUse == true)
					{
						fputs(cEnter, pFile);

						// ���f���̃p�[�c�̖��O����
						strcpy(cWriteText, cBillboardType[g_aBillboard[nCntBillboard].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    BILLBOARDSET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_BILLBOARDSET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cBillboardData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_aBillboard[nCntBillboard].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aBillboard[nCntBillboard].pos.x,
										g_aBillboard[nCntBillboard].pos.y,
										g_aBillboard[nCntBillboard].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aBillboard[nCntBillboard].rot.x,
										g_aBillboard[nCntBillboard].rot.y,
										g_aBillboard[nCntBillboard].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �T�C�Y���ۑ�
								else if (strcmp(cWriteText, "SIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aBillboard[nCntBillboard].size.x,
										g_aBillboard[nCntBillboard].size.y,
										g_aBillboard[nCntBillboard].size.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �G���h���Z�b�g
								else if (strcmp(cWriteText, "END_BILLBOARDSET") == 0)
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
					nCntBillboard++;
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
// �e���
// ------------------------------------------------------------------------------------------
BILLBOARD *GetBillboard(void) 
{
	return &g_aBillboard[0];
}