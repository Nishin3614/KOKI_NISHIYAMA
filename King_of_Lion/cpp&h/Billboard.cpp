// ------------------------------------------------------------------------------------------
//
// �r���{�[�h���� [billboard.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "Billboard.h"
#include "input.h"
#include "game.h"
#include "camera.h"
// �����蔻��p	//
#include "player.h"
#include "giraffe.h"
#include "buffalo.h"
#include "king_rion.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define BILLBOARD_MAX (128)
#define BILLBOARD_SIZE (10)
#define BILLBOARD_DISTANCE (0.0f)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
void Update_MakeVerTexBillboard(void);
void LoadBillboard(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_paTextureBillboard[BILLBOARDTYPE_MAX] = {};	// ���_�e�N�X�`���[�̃|�C���^
BILLBOARD g_aBillboard[BILLBOARD_MAX];									// �r���{�[�h

// �e�N�X�`���[�t�@�C������`
const char *g_paBillboardTexFile[BILLBOARDTYPE_MAX] =
{
	{ "data/TEXTURE/GAME/tree.png" },
	{ "data/TEXTURE/ui/�G��|���Ƃǂ��Ȃ邩.png" },
	{ "data/TEXTURE/ui/�؂��������Ƃǂ��Ȃ邩.png" },
	{ "data/TEXTURE/ui/�q���C�I������̐���.png" }
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// �ʒu�E��]�̏����ݒ�
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].nType = BILLBOARDTYPE_WOOD;
		g_aBillboard[nCntBillboard].fAlpha = 1.0f;
		g_aBillboard[nCntBillboard].bUse = false;
		g_aBillboard[nCntBillboard].bDisp = false;
	}

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
	CAMERA		*pCamera = GetCamera();			// �J�������擾
	D3DXVECTOR3	pPlayer_Pos = GetPlayer_Pos();	// �v���C���[���擾
	D3DXVECTOR3	diff[2];				// ���ꂼ��̒��_����
	float		fDiff[2];				// ����

	// �J�E���g�r���{�[�h
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// �g�p���
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			if (g_aBillboard[nCntBillboard].nType == BILLBOARDTYPE_WOOD)
			{
				// �J�����̎��_�ƃr���{�[�h�̋��� //
				// ���_���ꂼ��̋���
				diff[0] = g_aBillboard[nCntBillboard].pos - pCamera->posV;

				// �����̌v�Z
				fDiff[0] = sqrtf(
					diff[0].x * diff[0].x +
					diff[0].y * diff[0].y +
					diff[0].z * diff[0].z);

				// �J�����̎��_�ƃv���C���[�̋��� //
				// ���_���ꂼ��̋���
				diff[1] = pPlayer_Pos - pCamera->posV;

				// �����̌v�Z
				fDiff[1] = sqrtf(
					diff[1].x * diff[1].x +
					diff[1].y * diff[1].y +
					diff[1].z * diff[1].z);

				// �v���C���[�̂ق������ɕ`�悳��Ă���Ƃ�
				if (fDiff[0] - fDiff[1] < 0)
				{
					g_aBillboard[nCntBillboard].fAlpha = 0.8f;
				}

				// ����ȊO
				else
				{
					g_aBillboard[nCntBillboard].fAlpha = 1.0f;
				}
			}

			else
			{

			}
		}
	}

	// ���_���X�V
	Update_MakeVerTexBillboard();
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
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ���C�e�B���O���[�h�L��

	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bDisp)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pBillboard->mtxWorldBillboard);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			g_aBillboard[nCntBillboard].mtxWorldBillboard._11 = mtxView._11;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._13 = mtxView._31;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._21 = mtxView._12;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._22 = mtxView._22;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._23 = mtxView._32;

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

			// �r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}

	// �A���t�@�e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��

}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * BILLBOARD_MAX, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffBillboard,
		NULL);

	for (int nCntBillboardTexture = 0; nCntBillboardTexture < BILLBOARDTYPE_MAX; nCntBillboardTexture++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			g_paBillboardTexFile[nCntBillboardTexture],
			&g_paTextureBillboard[nCntBillboardTexture]);
	}

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
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
// �r���{�[�h�̒��_���X�V
// ------------------------------------------------------------------------------------------
void Update_MakeVerTexBillboard(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// �J�E���g�r���{�[�h
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// �g�p��Ԃ̏ꍇ
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// �J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// ��ڂ̐F�ݒ�
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// ��ڂ̐F�ݒ�
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// �O�ڂ̐F�ݒ�
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// �l�ڂ̐F�ݒ�
		}

		// ���_���X�V
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBillboard->Unlock();

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
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
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

															// �J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// ��ڂ̐F�ݒ�
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// ��ڂ̐F�ݒ�
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �O�ڂ̐F�ݒ�
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �l�ڂ̐F�ݒ�

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �r���{�[�h�̈ʒu�X�V
// ------------------------------------------------------------------------------------------
void SetPositionBillboard(int nType, D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// �J�E���g�r���{�[�h
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// �g�p���
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// �^�C�v
			if (g_aBillboard[nCntBillboard].nType == nType)
			{
				// ���X�V
				g_aBillboard[nCntBillboard].pos = pos;


				g_aBillboard[nCntBillboard].bDisp = true;

				// �J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�
			}
		}

		// �|�C���g���킹
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBillboard->Unlock();
}

// ------------------------------------------------------------------------------------------
// �r���{�[�h���ŏ���
// ------------------------------------------------------------------------------------------
void DeleteBillboard(int nType)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// �J�E���g�r���{�[�h
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// �g�p���
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// �^�C�v
			if (g_aBillboard[nCntBillboard].nType == nType)
			{
				g_aBillboard[nCntBillboard].bDisp = false;

				// �J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// ��ڂ̐F�ݒ�
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// ��ڂ̐F�ݒ�
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �O�ڂ̐F�ݒ�
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �l�ڂ̐F�ݒ�

				break;
			}
		}

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

	// �t�@�C���J
	// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data/SAVE/TUTORIAL/billboard.txt", "r");
	}

	// �{�X�p
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/billboard.txt", "r");
	}

	// ����ȊO
	else
	{
		pFile = fopen("data/SAVE/billboard.txt", "r");
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
		MessageBox(NULL, "�r���{�[�h�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �e���
// ------------------------------------------------------------------------------------------
BILLBOARD *GetBillboard(void) 
{
	return &g_aBillboard[0];
}