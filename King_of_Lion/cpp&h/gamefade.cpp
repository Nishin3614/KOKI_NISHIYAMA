// ------------------------------------------------------------------------------------------
//
// �Q�[���t�F�[�h���� [gamefade.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "gamefade.h"

#include "sound.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_GAMEFADE	"data/TEXTURE/ui/mozaiku.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
void MakeVertexGameFade(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureGameFade = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameFade = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
GAMEFADE				g_gamefade;					// �Q�[���t�F�[�h���
GAMESTATE				g_gameNext;					// ���̉�ʁi���[�h�j
D3DXCOLOR				g_colorGameFade;			// �Q�[���t�F�[�h�F

// ------------------------------------------------------------------------------------------
//
// ����������
//
// ------------------------------------------------------------------------------------------
void InitGameFade(GAMESTATE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_gamefade = GAMEFADE_NONE;
	g_gameNext = modeNext;
	g_colorGameFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// ������ʁi�s�����j

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_GAMEFADE,		// �t�@�C���̖��O
								&g_pTextureGameFade);	// �ǂݍ��ރ������[

	// ���_���̐ݒ�
	MakeVertexGameFade(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// �I������
//
// ------------------------------------------------------------------------------------------
void UninitGameFade(void)
{
	// �e�N�X�`���̊J��
	if(g_pTextureGameFade != NULL)
	{
		g_pTextureGameFade->Release();
		g_pTextureGameFade = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if(g_pVtxBuffGameFade != NULL)
	{
		g_pVtxBuffGameFade->Release();
		g_pVtxBuffGameFade = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateGameFade(void)
{
	VERTEX_2D *pVtx;

	if (g_gamefade != GAMEFADE_NONE)
	{
		if (g_gamefade == GAMEFADE_IN)
		{
			g_colorGameFade.a -= 0.05f;
			if (g_colorGameFade.a <= 0.0f)
			{
				// �Q�[���t�F�[�h�I��
				g_colorGameFade.a = 0.0f;
				g_gamefade = GAMEFADE_NONE;
			}
		}
		
		else if (g_gamefade == GAMEFADE_OUT)
		{
			// �Q�[���t�F�[�h�A�E�g
			g_colorGameFade.a += 0.05f;
			if (g_colorGameFade.a >= 1.0f)
			{
				// �Q�[���t�F�[�h�I��
				g_colorGameFade.a = 1.0f;
				g_gamefade = GAMEFADE_IN;

				SetGameState(g_gameNext);
			}
		}

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffGameFade->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_colorGameFade;
		pVtx[1].col = g_colorGameFade;
		pVtx[2].col = g_colorGameFade;
		pVtx[3].col = g_colorGameFade;

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffGameFade->Unlock();
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g�����
//
// ------------------------------------------------------------------------------------------
void DrawGameFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffGameFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGameFade);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ------------------------------------------------------------------------------------------
//
// ���_�̍쐬
//
// ------------------------------------------------------------------------------------------
void MakeVertexGameFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameFade, NULL);
	
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffGameFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_colorGameFade;
	pVtx[1].col = g_colorGameFade;
	pVtx[2].col = g_colorGameFade;
	pVtx[3].col = g_colorGameFade;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffGameFade->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �Q�[���t�F�[�h�̏�Ԑݒ�
//
// ------------------------------------------------------------------------------------------
void SetGameFade(GAMESTATE modeNext)
{
	if (modeNext == GAMESTATE_BOSS)
	{
		g_gamefade = GAMEFADE_OUT;
		g_gameNext = modeNext;
		g_colorGameFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// ������ʁi����)
	}
}

// ------------------------------------------------------------------------------------------
//
// �Q�[���t�F�[�h�̏�Ԏ擾
//
// ------------------------------------------------------------------------------------------
GAMEFADE GetGameFade(void)
{
	return g_gamefade;
}