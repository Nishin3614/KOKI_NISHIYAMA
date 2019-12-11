// ------------------------------------------------------------------------------------------
//
// ��󏈗��̐���[arrow.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "arrow.h"
#include "pousecoment.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define ARROWTEX "data/TEXTURE/99_arrow.png"	// �w�i
#define POUSENAME_SIZE_X (100)					// �^�C�g�����T�C�Y�i���j
#define POUSENAME_SIZE_Y (100)					// �^�C�g�����T�C�Y�i���j

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureArrow = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffArrow = NULL;	// �e�N�X�`���ւ̃|�C���^
ARROW g_aArrow;									// ���

// ------------------------------------------------------------------------------------------
// �^�C�g������������
// ------------------------------------------------------------------------------------------
void InitArrow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g

	// ������
	g_aArrow.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aArrow.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aArrow.bUse = false;

	// �e�N�X�`���̓ǂݍ���
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, ARROWTEX, &g_pTextureArrow);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffArrow, NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffArrow->Lock(0, 0, (void **)&pVtx, 0);
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// ��ڂ̒��_
	pVtx[1].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// ��ڂ̒��_
	pVtx[2].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// �O�ڂ̒��_
	pVtx[3].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// �l�ڂ̒��_
	// ���_�쐬
	pVtx[0].rhw = 1.0f;									// ��ڂ̒��_�쐬
	pVtx[1].rhw = 1.0f;									// ��ڂ̒��_�쐬
	pVtx[2].rhw = 1.0f;									// �O�ڂ̒��_�쐬
	pVtx[3].rhw = 1.0f;									// �l�ڂ̒��_�쐬
	// �J���[�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// ��ڂ̐F�ݒ�
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// ��ڂ̐F�ݒ�
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// �O�ڂ̐F�ݒ�
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// �l�ڂ̐F�ݒ�
	// �e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// ��ڂ̃e�N�X�`��
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// ��ڂ̃e�N�X�`��
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// �O�ڂ̃e�N�X�`��
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// �l�ڂ̃e�N�X�`��
	// �A�����b�N
	g_pVtxBuffArrow->Unlock();
}

// ------------------------------------------------------------------------------------------
// �^�C�g���I������
// ------------------------------------------------------------------------------------------

void UninitArrow(void)
{
	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �^�C�g���X�V����
// ------------------------------------------------------------------------------------------
void UpdateArrow(void)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g

	// ���X�Ɉړ�
	g_aArrow.pos += (g_aArrow.posLast - g_aArrow.pos) * 0.5f;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffArrow->Lock(0, 0, (void **)&pVtx, 0);
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// ��ڂ̒��_
	pVtx[1].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// ��ڂ̒��_
	pVtx[2].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// �O�ڂ̒��_
	pVtx[3].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// �l�ڂ̒��_
	// �A�����b�N
	g_pVtxBuffArrow->Unlock();

}

// ------------------------------------------------------------------------------------------
// �^�C�g���`�揈��
// ------------------------------------------------------------------------------------------
void DrawArrow(void)
{
	// �g�p��Ԃ̏ꍇ
	if (g_aArrow.bUse == true)
	{
		// �ϐ��錾
		LPDIRECT3DDEVICE9 pDevice;
		// �f�o�C�X���擾����
		pDevice = GetDevice();
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffArrow, 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���[�ݒ�
		pDevice->SetTexture(0, g_pTextureArrow);
		// �^�C�g���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

// ------------------------------------------------------------------------------------------
// ���̈ʒu����
// ------------------------------------------------------------------------------------------
void ArrowPosition(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g

	// ��\����Ԃ���\����ԂɂȂ�O
	if (g_aArrow.bUse == false)
	{
		g_aArrow.pos = pos;
	}

	// �ʒu���擾
	g_aArrow.posLast = pos;
	g_aArrow.posLast.y -= 100.0f;

	// �g�p��Ԃ�
	g_aArrow.bUse = true;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffArrow->Lock(0, 0, (void **)&pVtx, 0);
	// ���_�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// ��ڂ̒��_
	pVtx[1].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// ��ڂ̒��_
	pVtx[2].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// �O�ڂ̒��_
	pVtx[3].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// �l�ڂ̒��_
	// �A�����b�N
	g_pVtxBuffArrow->Unlock();
}

// ------------------------------------------------------------------------------------------
// ���𖢎g�p��ԏ���
// ------------------------------------------------------------------------------------------
void DeleteArrow(void)
{
	g_aArrow.bUse = false;
}

// ------------------------------------------------------------------------------------------
// �^�C�g���擾����
// ------------------------------------------------------------------------------------------
ARROW *GetArrow(void)
{
	return &g_aArrow;
}