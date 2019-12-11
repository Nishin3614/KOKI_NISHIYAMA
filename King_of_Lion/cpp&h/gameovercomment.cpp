// ------------------------------------------------------------------------------------------
//
// �Q�[���I�[�o�[�R�����g�����̐���[gameovercomment.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------

#include "gameovercomment.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"

// ------------------------------------------------------------------------------------------
//
// �񋓌^
//
// ------------------------------------------------------------------------------------------
// ---------�^�C�v---------- //
typedef enum
{
	GAMEOVERCOMMENTTYPE_GAMEOVER = 0,
	GAMEOVERCOMMENTTYPE_MAX
} GAMEOVERCOMMENTTYPE;

// ------------------------------------------------------------------------------------------
//
// �\����
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;	// �ʒu
	D3DXVECTOR2 size;	// �T�C�Y
} GAMEOVERCOM;

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureGameoverComment[GAMEOVERCOMMENTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameoverComment = NULL;					// �e�N�X�`���ւ̃|�C���^
GAMEOVERCOM g_GameoverCom[GAMEOVERCOMMENTTYPE_MAX];							// �N���A���ߕϐ�

// �e�N�X�`���[�t�@�C������`
const char *g_paGameoverTexFile[GAMEOVERCOMMENTTYPE_MAX] =
{
	{ "data/TEXTURE/GAMEOVER/Game_Over.png" },
};

// ------------------------------------------------------------------------------------------
//
// �v���C���[����������
//
// ------------------------------------------------------------------------------------------

void InitGameoverComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g

	// �e�N�X�`���̓ǂݍ���
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GAMEOVERCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameoverComment,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffGameoverComment->Lock(0, 0, (void **)&pVtx, 0);

	// �N���A�R�����g�J�E���g
	for (int nCntCom = 0; nCntCom < GAMEOVERCOMMENTTYPE_MAX; nCntCom++)
	{
		// �e�N�X�`���[�쐬
		D3DXCreateTextureFromFile(
			pDevice,
			g_paGameoverTexFile[nCntCom],
			&g_pTextureGameoverComment[nCntCom]);

		// �N���A�̃R�����g
		if (nCntCom == GAMEOVERCOMMENTTYPE_GAMEOVER)
		{
			g_GameoverCom[nCntCom].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
			g_GameoverCom[nCntCom].size = D3DXVECTOR2(400.0f, 250.0f);
		}

		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x - g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y - g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[1].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x + g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y - g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[2].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x - g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y + g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[3].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x + g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y + g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);

		// ���_�쐬
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += NUM_VERTEX;
	}

	// �A�����b�N
	g_pVtxBuffGameoverComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�I������
//
// ------------------------------------------------------------------------------------------

void UninitGameoverComment(void)
{
	// �e�N�X�`���̊J��
	for (int nCntGameoverComment = 0; nCntGameoverComment < GAMEOVERCOMMENTTYPE_MAX; nCntGameoverComment++)
	{
		if (g_pTextureGameoverComment[nCntGameoverComment] != NULL)
		{
			g_pTextureGameoverComment[nCntGameoverComment]->Release();
			g_pTextureGameoverComment[nCntGameoverComment] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffGameoverComment != NULL)
	{
		g_pVtxBuffGameoverComment->Release();
		g_pVtxBuffGameoverComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateGameoverComment(void)
{
}

// ------------------------------------------------------------------------------------------
//
// �v���C���[�`�揈��
//
// ------------------------------------------------------------------------------------------

void DrawGameoverComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffGameoverComment, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int nCntGameoverComment = 0; nCntGameoverComment < GAMEOVERCOMMENTTYPE_MAX; nCntGameoverComment++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGameoverComment[nCntGameoverComment]);

		// �v���C���[�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameoverComment * 4, 2);
	}
}