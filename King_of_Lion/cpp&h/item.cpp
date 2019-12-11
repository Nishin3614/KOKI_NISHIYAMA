// ------------------------------------------------------------------------------------------
//
// �A�C�e������ [item.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "item.h"

#include "input.h"

#include "shadow.h"

#include "explosion.h"

#include "effect.h"

#include "wall.h"

#include "floor.h"

#include "materials.h"

#include "player.h"

#include "game.h"

#include "hpgauge.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define ITEM_MAX (128)
#define ITEM_SIZE (20)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------�A�C�e���̎��---------- //
typedef enum
{
	ITEMTYPE_MEAT = 0,
	ITEMTYPE_MATATABI,
	ITEMTYPE_MAX
} ITEMTYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Init_MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void Init_LoadItem(void);
void Update_MoveItem(int nCntItem);
void Update_ColisionItem(int nCntItem);
void Update_PickUpItem(int nCntItem);
void Update_EffectItem(int nCntItem);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_paTextureItem[ITEMTYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^
ITEM g_aItem[ITEM_MAX];									// �A�C�e��

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	char *paItemTexture[ITEMTYPE_MAX] =
	{
		{ "data/TEXTURE/item/000_meat.png" },		// ��
		{ "data/TEXTURE/item/01_matatabi.png" },	// �}�^�^�r
	};

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		// �ʒu�E��]�̏����ݒ�
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].posold = g_aItem[nCntItem].pos;
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fRadius = 50.0f;
		g_aItem[nCntItem].nLife = 0;
		g_aItem[nCntItem].nIdxShadow = 0;
		g_aItem[nCntItem].nType = 0;
		g_aItem[nCntItem].nAttack = 0;
		g_aItem[nCntItem].bUse = false;
	}

	// �e�N�X�`���[���ǂݍ���
	for (int nCntText = 0; nCntText < ITEMTYPE_MAX; nCntText++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, paItemTexture[nCntText], &g_paTextureItem[nCntText]);
	}

	// �A�C�e���̓ǂݍ���
	Init_LoadItem();

	// ���_���̍쐬
	Init_MakeVertexItem(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		// �e�N�X�`���̊J��
		if (g_paTextureItem[nCntItem] != NULL)
		{
			g_paTextureItem[nCntItem]->Release();
			g_paTextureItem[nCntItem] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].posold = g_aItem[nCntItem].pos;

			// ���C�t����
			//g_aItem[nCntItem].nLife--;
			
			// �ʒu���̍X�V
			Update_MoveItem(nCntItem);

			// ���C�t���O�ɂȂ�����
			if (g_aItem[nCntItem].nLife <= 0)
			{
				// ���g�p��Ԃ�
				g_aItem[nCntItem].bUse = false;

				// �e�̍폜
				DeleteShadow(g_aItem[nCntItem].nIdxShadow);
			}

			// �E�������̏���
			Update_PickUpItem(nCntItem);

			// �����蔻�� 
			Update_ColisionItem(nCntItem);

			// �G�t�F�N�g����
			Update_EffectItem(nCntItem);

			// �e�̈ʒu��ݒ�
			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, g_aItem[nCntItem].pos);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �ʒu���̍X�V����
// ------------------------------------------------------------------------------------------
void Update_MoveItem(int nCntItem)
{
	// �ړ���
	g_aItem[nCntItem].pos += g_aItem[nCntItem].move;

	// �d��
	g_aItem[nCntItem].move.y -= 1.0f;
}

// ------------------------------------------------------------------------------------------
// �G�t�F�N�g����
// ------------------------------------------------------------------------------------------
void Update_EffectItem(int nCntItem)
{
	// �ϐ��錾
	float fAngle;		// x�̊p�x
	float fYAngle;		// y�̊p�x
	D3DXVECTOR3 move;	// �ړ���
	D3DXCOLOR col;		// �J���[

						// X�EZ�̊p�x
	fAngle =
		float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

	// Y�̊p�x
	fYAngle =
		float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

	// �ړ�����
	move =
		D3DXVECTOR3(sinf(fAngle) * 5.0f,
			cosf(fYAngle) * 5.0f,
			cosf(fAngle) * 5.0f);

	// �J���[
	col =
		D3DXCOLOR(
			0.5f + float(rand() % 50) / 100.0f,
			0.5f - float(rand() % 50) / 100.0f,
			0.0f,
			1.0f);

	// �G�t�F�N�g�̐ݒ�
	SetEffect(
		D3DXVECTOR3(													// �ʒu
			sinf(fAngle) * g_aItem[nCntItem].size.x + g_aItem[nCntItem].pos.x,
			cosf(fYAngle) * g_aItem[nCntItem].size.y + g_aItem[nCntItem].pos.y,
			cosf(fAngle) * g_aItem[nCntItem].size.z + g_aItem[nCntItem].pos.z),
		move,															// �ړ���
		col,															// �F
		float(rand() % 10),												// �T�C�Y
		-0,																// �����T�C�Y
		0.0f,															// �d��
		10,																// �G�t�F�N�g���C�t
		0,																// �^�C�v
		0);
}

// ------------------------------------------------------------------------------------------
// ������������
// ------------------------------------------------------------------------------------------
void Update_ColisionItem(int nCntItem)
{
	// �ϐ��錾
	int nCntFloor;
	FLOOR * pFloor = GetFLOOR();

	// ���̓����蔻�� //
	// �u���b�N���[�v
	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++, pFloor++)
	{
		// �g�p���
		if (pFloor->bUse == true)
		{
			// �u���b�N��X�͈�
			if (g_aItem[nCntItem].pos.x > pFloor->pos.x - pFloor->size.x * pFloor->nBlock_Width&&
				g_aItem[nCntItem].pos.x < pFloor->pos.x + pFloor->size.x * pFloor->nBlock_Width&&
				g_aItem[nCntItem].pos.z > pFloor->pos.z - pFloor->size.z * pFloor->nBlock_Depth&&
				g_aItem[nCntItem].pos.z < pFloor->pos.z + pFloor->size.z * pFloor->nBlock_Depth)
			{
				// �����蔻��(��)

				if (g_aItem[nCntItem].pos.y - g_aItem[nCntItem].size.y - 10.0f< pFloor->pos.y &&
					g_aItem[nCntItem].posold.y - g_aItem[nCntItem].size.y - 10.0f >= pFloor->pos.y)
				{
					// �u���b�N��̏��
					g_aItem[nCntItem].pos.y = pFloor->pos.y + g_aItem[nCntItem].size.y + 10.0f;

					// �ړ��ʂ̏�����
					g_aItem[nCntItem].move.y = 0.0f;
				}

				// �����蔻��(��)
				if (g_aItem[nCntItem].pos.y + g_aItem[nCntItem].size.y > pFloor->pos.y &&
					g_aItem[nCntItem].posold.y + g_aItem[nCntItem].size.y <= pFloor->pos.y)
				{
					// �u���b�N��̉���
					g_aItem[nCntItem].pos.y = pFloor->pos.y - g_aItem[nCntItem].size.y;

					// �ړ��ʂ̏�����
					g_aItem[nCntItem].move.y = 0.0f;
				}
			}
		}
	}

	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_aItem[nCntItem].pos,
		&g_aItem[nCntItem].posold,
		&g_aItem[nCntItem].move,
		&g_aItem[nCntItem].size,
		&g_aItem[nCntItem].pMaterials))
	{

	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_aItem[nCntItem].pos,
		&g_aItem[nCntItem].posold,
		&g_aItem[nCntItem].move,
		&g_aItem[nCntItem].size,
		&g_aItem[nCntItem].pWall))
	{

	}
}

// ------------------------------------------------------------------------------------------
// �A�C�e���E�����Ƃ�
// ------------------------------------------------------------------------------------------
void Update_PickUpItem(int nCntItem)
{
	// �v���C���[�̈ʒu���
	D3DXVECTOR3 Player_pos = GetPlayer_Pos();

	// �v���C���[�̔��a���
	float Player_Radius = GetPlayer_Radius();

	// �A�C�e���̓����蔻��
	if ((g_aItem[nCntItem].fRadius * g_aItem[nCntItem].fRadius) +
		(Player_Radius * Player_Radius) >=
		(g_aItem[nCntItem].pos.x - Player_pos.x) * (g_aItem[nCntItem].pos.x - Player_pos.x) +
		(g_aItem[nCntItem].pos.y - Player_pos.y) * (g_aItem[nCntItem].pos.y - Player_pos.y) +
		(g_aItem[nCntItem].pos.z - Player_pos.z) * (g_aItem[nCntItem].pos.z - Player_pos.z))
	{
		switch (g_aItem[nCntItem].nType)
		{
		case ITEMTYPE_MEAT:
			StatusUpPlayer(
				10,			// HP�A�b�v
				0,			// MP�A�b�v
				0);			// �U���̓A�b�v

			break;

		case ITEMTYPE_MATATABI:
			StatusUpPlayer(
				0,			// HP�A�b�v
				10,			// MP�A�b�v
				0);			// �U���̓A�b�v

			break;

		default:
			break;
		}

		g_aItem[nCntItem].bUse = false;
		DeleteShadow(g_aItem[nCntItem].nIdxShadow);
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	ITEM *pItem = &g_aItem[0];

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// �r���[�}�g���b�N�X�̑���p
	D3DXMATRIX mtxView;

	// �A���t�@�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ALPHAREF,200);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ���C�e�B���O���[�h�L��

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++, pItem++)
	{
		if (pItem->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pItem->mtxWorldItem);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			g_aItem[nCntItem].mtxWorldItem._11 = mtxView._11;
			g_aItem[nCntItem].mtxWorldItem._12 = mtxView._21;
			g_aItem[nCntItem].mtxWorldItem._13 = mtxView._31;
			g_aItem[nCntItem].mtxWorldItem._21 = mtxView._12;
			g_aItem[nCntItem].mtxWorldItem._22 = mtxView._22;
			g_aItem[nCntItem].mtxWorldItem._23 = mtxView._32;
			//g_aItem[nCntItem].mtxWorldItem._31 = mtxView._13;
			//g_aItem[nCntItem].mtxWorldItem._32 = mtxView._23;
			//g_aItem[nCntItem].mtxWorldItem._33 = mtxView._33;

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pItem->rot.y, pItem->rot.x, pItem->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pItem->mtxWorldItem,	// 1
				&pItem->mtxWorldItem,					// 2
				&mtxRot);							// 3

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pItem->pos.x, pItem->pos.y, pItem->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pItem->mtxWorldItem,	// 1
				&pItem->mtxWorldItem,					// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pItem->mtxWorldItem);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_paTextureItem[g_aItem[nCntItem].nType]);

			// �A�C�e���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// �A���t�@�e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ���C�e�B���O���[�h�L��
}

// ------------------------------------------------------------------------------------------
// �A�C�e���̐ݒ�
// ------------------------------------------------------------------------------------------
void SetItem(
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR3 rot,	// ��]
	D3DXVECTOR3 move,	// �ړ���
	D3DXVECTOR3 size,	// �T�C�Y
	float	fRadius,	// ���a
	int nType,			// �^�C�v
	int nLife			// ���C�t
)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffItem->Lock(0, 0, (void **)&pVtx, 0);

	// �ǃ��[�v
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_aItem[nCntItem].bUse)
		{
			// ���X�V //
			// �ʒu���
			g_aItem[nCntItem].pos = pos;

			// �ړ���
			g_aItem[nCntItem].move = move;

			// �T�C�Y
			g_aItem[nCntItem].size = size;

			// ���a
			g_aItem[nCntItem].fRadius = fRadius;

			// �^�C�v
			g_aItem[nCntItem].nType = nType;

			// ���C�t��
			g_aItem[nCntItem].nLife = nLife;

			// �g�p��Ԃ�
			g_aItem[nCntItem].bUse = true;

			// �e�̍쐬
			g_aItem[nCntItem].nIdxShadow = SetShadow(
				g_aItem[nCntItem].pos,
				D3DXVECTOR3(20.0f, 0.0f, 20.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �|�C���g���킹
			pVtx += nCntItem * 4;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);		// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// �l�ڂ̒��_

			break;
		}
	}

	// �A�����b�N
	g_pVtxBuffItem->Unlock();
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void Init_MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * ITEM_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffItem->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);		// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// �l�ڂ̒��_


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
	g_pVtxBuffItem->Unlock();
}

// ------------------------------------------------------------------------------------------
// �A�C�e�������擾
// ------------------------------------------------------------------------------------------
void Init_LoadItem(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;
	int nCntItem = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

							// �t�@�C���J
	pFile = fopen("data/SAVE/ITEM/item.txt", "r");

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

				// �A�C�e���Z�b�g��������
				else if (strcmp(cHeadText, "ITEMSET") == 0)
				{
					// �g�p��Ԃ�
					g_aItem[nCntItem].bUse = true;
					// �G���h�A�C�e���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_ITEMSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ʒu���ǂݍ���
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_aItem[nCntItem].pos.x,
								&g_aItem[nCntItem].pos.y,
								&g_aItem[nCntItem].pos.z);
						}

						// �T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_aItem[nCntItem].size.x,
								&g_aItem[nCntItem].size.y,
								&g_aItem[nCntItem].size.z);
						}

						// ��ޏ��ǂݍ���
						else if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_aItem[nCntItem].nType);
						}

						// HP���ǂݍ���
						else if (strcmp(cHeadText, "LIFE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_aItem[nCntItem].nLife);
						}
					}
					// �A�C�e���̍X�V
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
		MessageBox(NULL, "�A�C�e���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// �A�C�e�����\��
// ------------------------------------------------------------------------------------------
void InforItem(void)
{

}

// ------------------------------------------------------------------------------------------
// �A�C�e�����
// ------------------------------------------------------------------------------------------
ITEM *GetItem(void) 
{
	return &g_aItem[0];
}