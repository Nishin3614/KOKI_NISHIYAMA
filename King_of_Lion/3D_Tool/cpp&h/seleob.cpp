// ------------------------------------------------------------------------------------------
//
// �I�u�̑I������ [seleob.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "seleob.h"
#include "input.h"
#include "camera.h"
#include "mouse.h"
#include "floor.h"
#include "materials.h"
#include "giraffe.h"
#include "wall.h"
#include "Billboard.h"
#include "game.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG


// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_SELEOB (1)
#define SELEOB_MOVE (2.0f)
#define SELEOB_SIZE_X (100.0f)
#define SELEOB_SIZE_Y (200.0f)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexSeleOb(LPDIRECT3DDEVICE9 pDevice);

#ifdef _DEBUG
void Debug_SeleOb(void);
#endif // _DEBUG
// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSeleOb = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureSeleOb = NULL;	// ���_�e�N�X�`���[�̃|�C���^
SELEOB g_SeleOb;

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitSeleob(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �f�ރt�@�C������`
	char *paSeleObFile[1] =
	{
		{ "data/MODEL/���.x" }						// [�Ζ�]00
	};


	// ������
	g_SeleOb.Screenpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SeleOb.Worldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SeleOb.bUse = false;
	g_SeleOb.type = SELEOBTYPE_NONE;
	g_SeleOb.nNumber = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"", &g_pTextureSeleOb);

	// ���_�쐬
	MakeVertexSeleOb(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitSeleob(void)
{
	// �e�N�X�`���[�̊J��
	if (g_pTextureSeleOb != NULL)
	{
		g_pTextureSeleOb->Release();
		g_pTextureSeleOb = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateSeleob(void)
{
	CAMERA		*pCamera = GetCamera();						// �J�������擾
	GIRAFFE		*pGiraffe = GetGiraffe();					// �L�������擾
	FLOOR		*pFloor = GetFloor();						// �����擾
	WALL		*pWall = GetWall();							// �Ǐ��擾
	BILLBOARD	*pBillboard = GetBillboard();				// �r���{�[�h���擾
	MATERIALS	*pMaterials = GetMaterials();				// �}�e���A�����擾
	float MouseposX = (float)GetMouseX();					// �}�E�X��x���W���擾
	float MouseposZ = (float)GetMouseY();					// �}�E�X��y���W���擾
	D3DXVECTOR3 WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[���h���W

	// ���_���̐ݒ�
	for (int nCntSeleob = ALL_MAX; nCntSeleob >= 0; nCntSeleob--)
	{
		// �}�e���A��
		if (pMaterials[nCntSeleob].bUse == true && pMaterials[nCntSeleob].bDisp == true)
		{
			// �}�E�X�̓_���X�N���[�����W���烏�[���h���W��(�I�u�W�F�N�g�̑��݂��镽��)
			CalcScreenToXZ(
				&WorldPos,
				MouseposX,
				MouseposZ,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				&pCamera->mtxView,
				&pCamera->mtxProjection,
				pMaterials[nCntSeleob].pos + D3DXVECTOR3(0.0f, pMaterials[nCntSeleob].size.y / 2, 0.0f)
			);

			// �~�̓����蔻��
			if (Collision_Circle(
				WorldPos, 4.0f,
				pMaterials[nCntSeleob].pos +
				D3DXVECTOR3(0.0f, pMaterials[nCntSeleob].size.y / 2, 0.0f), pMaterials[nCntSeleob].size.x))
			{
				// ���N���b�N�őI��
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.Worldpos = pMaterials[nCntSeleob].pos + D3DXVECTOR3(0.0, 20.0f, 0.0f);	// �����I�u�W�F�N�g���W
					g_SeleOb.type = SELEOBTYPE_MATERIALS;							// �I�����
					g_SeleOb.nNumber = nCntSeleob;								// �ύX�\�ȑI���I�u�W�F�N�g�̔ԍ�
					g_SeleOb.bUse = true;										// �����I�u�W�F�N�g�o��
					return;
				}
			}
			else
			{
				// �I������
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.type = SELEOBTYPE_NONE;	// �I����ԉ���
					g_SeleOb.nNumber = 0;				// �ύX�\�ȑI���I�u�W�F�N�g�̔ԍ���������
					g_SeleOb.bUse = false;				// �����I�u�W�F�N�g�𖢎g�p��Ԃ�
				}
			}
		}
	}
	// ���_���̐ݒ�
	for (int nCntSeleob = ALL_MAX; nCntSeleob >= 0; nCntSeleob--)
	{

		// �L����(�G�l�~�[)
		if (pGiraffe[nCntSeleob].bUse == true && pGiraffe[nCntSeleob].bDisp == true)
		{
			// �}�E�X�̓_���X�N���[�����W���烏�[���h���W��(�I�u�W�F�N�g�̑��݂��镽��)
			CalcScreenToXZ(
				&WorldPos,
				MouseposX,
				MouseposZ,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				&pCamera->mtxView,
				&pCamera->mtxProjection,
				pGiraffe[nCntSeleob].pos + D3DXVECTOR3(0.0f, pGiraffe[nCntSeleob].size.y / 2, 0.0f)
			);

			// �~�̓����蔻��
			if (Collision_Circle(
				WorldPos, 4.0f,
				pGiraffe[nCntSeleob].pos + D3DXVECTOR3(0.0f, pGiraffe[nCntSeleob].size.y / 2, 0.0f), pGiraffe[nCntSeleob].size.x))
			{
				// ���N���b�N�őI��
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.Worldpos = pGiraffe[nCntSeleob].pos + D3DXVECTOR3(0.0, pGiraffe[nCntSeleob].size.y, 0.0f);	// �����I�u�W�F�N�g���W
					g_SeleOb.type = SELEOBTYPE_GIRAFFE;							// �I�����
					g_SeleOb.nNumber = nCntSeleob;								// �ύX�\�ȑI���I�u�W�F�N�g�̔ԍ�
					g_SeleOb.bUse = true;										// �����I�u�W�F�N�g�o��
					return;
				}
			}
			else
			{
				// �I������
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.type = SELEOBTYPE_NONE;	// �I����ԉ���
					g_SeleOb.nNumber = 0;				// �ύX�\�ȑI���I�u�W�F�N�g�̔ԍ���������
					g_SeleOb.bUse = false;				// �����I�u�W�F�N�g�𖢎g�p��Ԃ�
				}
			}
		}
	}
	// ���_���̐ݒ�
	for (int nCntSeleob = ALL_MAX; nCntSeleob >= 0; nCntSeleob--)
	{
		// ��
		if (pFloor[nCntSeleob].bUse == true && pFloor[nCntSeleob].bDisp == true)
		{
			// �}�E�X�̓_���X�N���[�����W���烏�[���h���W��(�I�u�W�F�N�g�̑��݂��镽��)
			CalcScreenToXZ(
				&WorldPos,
				MouseposX,
				MouseposZ,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				&pCamera->mtxView,
				&pCamera->mtxProjection,
				pFloor[nCntSeleob].pos
			);

			// ����
			if (SquareBottom_Judg(
				pFloor[nCntSeleob].pos,
				pFloor[nCntSeleob].size,
				pFloor[nCntSeleob].rot,
				pFloor[nCntSeleob].nBlock_Width,
				pFloor[nCntSeleob].nBlock_Depth,
				WorldPos))
			{
				// ���N���b�N�őI��
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.Worldpos = pFloor[nCntSeleob].pos + D3DXVECTOR3(0.0, 20.0f, 0.0f);	// �����I�u�W�F�N�g���W
					g_SeleOb.type = SELEOBTYPE_FLOOR;							// �I�����
					g_SeleOb.nNumber = nCntSeleob;								// �ύX�\�ȑI���I�u�W�F�N�g�̔ԍ�
					g_SeleOb.bUse = true;										// �����I�u�W�F�N�g�o��
					return;
				}
			}
			else
			{
				// �I������
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.type = SELEOBTYPE_NONE;	// �I����ԉ���
					g_SeleOb.nNumber = 0;				// �ύX�\�ȑI���I�u�W�F�N�g�̔ԍ���������
					g_SeleOb.bUse = false;				// �����I�u�W�F�N�g�𖢎g�p��Ԃ�
				}
			}
		}
	}
	// �I����Ԃɂ���I�u�W�F�N�g
	if (g_SeleOb.type == SELEOBTYPE_MATERIALS)
	{
		// �I���ʒu
		g_SeleOb.Worldpos = pMaterials[g_SeleOb.nNumber].pos + D3DXVECTOR3(0.0, pMaterials[g_SeleOb.nNumber].size.y, 0.0f);	// �����I�u�W�F�N�g���W

		// �I�������I�u�W�F�N�g�̐ݒ�
		if (!SeleMaterials(g_SeleOb.nNumber))
		{
			g_SeleOb.bUse = false;
			g_SeleOb.type = SELEOBTYPE_NONE;
		}
	}

	// �I����Ԃɂ���I�u�W�F�N�g
	else if (g_SeleOb.type == SELEOBTYPE_GIRAFFE)
	{
		// �I���ʒu
		g_SeleOb.Worldpos = pGiraffe[g_SeleOb.nNumber].pos + D3DXVECTOR3(0.0, pGiraffe[g_SeleOb.nNumber].size.y, 0.0f);	// �����I�u�W�F�N�g���W

		// �I�������I�u�W�F�N�g�̐ݒ�
		if (!SeleGiraffe(g_SeleOb.nNumber))
		{
			g_SeleOb.bUse = false;
			g_SeleOb.type = SELEOBTYPE_NONE;
		}
	}

	// �I����Ԃɂ���I�u�W�F�N�g
	else if (g_SeleOb.type == SELEOBTYPE_FLOOR)
	{
		// �I���ʒu
		g_SeleOb.Worldpos = pFloor[g_SeleOb.nNumber].pos + D3DXVECTOR3(0.0, 20.0f, 0.0f);	// �����I�u�W�F�N�g���W

		// �I�������I�u�W�F�N�g�̐ݒ�
		if (!SeleFloor(g_SeleOb.nNumber))
		{
			g_SeleOb.bUse = false;
			g_SeleOb.type = SELEOBTYPE_NONE;
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	Debug_SeleOb();
#endif // _DEBUG

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawSeleob(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	FLOOR	*pFloor = GetFloor();						// �����擾

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffSeleOb, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �g�p���
	if (g_SeleOb.bUse == true)
	{
		// �e�N�X�`���[���
		pDevice->SetTexture(0, g_pTextureSeleOb);
		// �`�揈��
		for (int nCntDraw = 0; nCntDraw < SELEOBDICTION_MAX; nCntDraw++)
		{
			// �^�C�g���`��
			//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDraw * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexSeleOb(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���g
	int nCntSeleOb;						// �J�E���g
	D3DXCOLOR col[SELEOBDICTION_MAX] =	// �J���[
	{
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),	// X(��)
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),	// Y(��)
		D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),	// Z(��)
	};

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * SELEOBDICTION_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED,
		&g_pVtxBuffSeleOb,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffSeleOb->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntSeleOb = 0; nCntSeleOb < SELEOBDICTION_MAX; nCntSeleOb++)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x - SELEOB_SIZE_X, g_SeleOb.Worldpos.y - SELEOB_SIZE_Y, 0.0f);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x + SELEOB_SIZE_X, g_SeleOb.Worldpos.y - SELEOB_SIZE_Y, 0.0f);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x - SELEOB_SIZE_X, g_SeleOb.Worldpos.y + SELEOB_SIZE_Y, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x + SELEOB_SIZE_X, g_SeleOb.Worldpos.y + SELEOB_SIZE_Y, 0.0f);	// �l�ڂ̒��_

		// ���_�쐬
		pVtx[0].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[1].rhw = 1.0f;	// ��ڂ̒��_�쐬
		pVtx[2].rhw = 1.0f;	// �O�ڂ̒��_�쐬
		pVtx[3].rhw = 1.0f;	// �l�ڂ̒��_�쐬

		// �J���[�ݒ�
		pVtx[0].col = col[nCntSeleOb];	// ��ڂ̐F�ݒ�
		pVtx[1].col = col[nCntSeleOb];	// ��ڂ̐F�ݒ�
		pVtx[2].col = col[nCntSeleOb];	// �O�ڂ̐F�ݒ�
		pVtx[3].col = col[nCntSeleOb];	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̃e�N�X�`��
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̃e�N�X�`��
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̃e�N�X�`��
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̃e�N�X�`��

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffSeleOb->Unlock();
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------------------------------------------------------
void Debug_SeleOb(void)
{
	PrintDebug("/*----------<�I�����>----------*/\n");
	PrintDebug("�I�u�W�F�N�g�폜:BACKSPACE\n");
}
#endif // _DEBUG
