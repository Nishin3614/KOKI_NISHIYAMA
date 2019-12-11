// ------------------------------------------------------------------------------------------
//
// �N���A�R�����g��ʏ����̐���[clearcomment.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "clearcomment.h"

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
	CLEARCOMMENTTYPE_CLEAR = 0,
	CLEARCOMMENTTYPE_THANK,
	CLEARCOMMENTTYPE_MAX
} CLEARCOMMENTTYPE;

// ------------------------------------------------------------------------------------------
//
// �\����
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;	// �ʒu
	D3DXVECTOR2 size;	// �T�C�Y
} CLEARCOM;

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureClearComment[CLEARCOMMENTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClearComment = NULL;					// �o�b�t�@�ւ̃|�C���^
CLEARCOM g_ClearCom[CLEARCOMMENTTYPE_MAX];								// �N���A���ߕϐ�

// �e�N�X�`���[�t�@�C������`
const char *g_paClearTexFile[CLEARCOMMENTTYPE_MAX] =
{
	{ "data/TEXTURE/CLEAR/Clear_name.png" },
	{ "data/TEXTURE/CLEAR/Thank_You_for_Playing2.png" }
};


// ------------------------------------------------------------------------------------------
//
// �N���A�R�����g����������
//
// ------------------------------------------------------------------------------------------

void InitClearComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���g

	// �e�N�X�`���̓ǂݍ���
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CLEARCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffClearComment,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffClearComment->Lock(0, 0, (void **)&pVtx, 0);

	// �N���A�R�����g�J�E���g
	for (int nCntCom = 0; nCntCom < CLEARCOMMENTTYPE_MAX; nCntCom++)
	{
		// �e�N�X�`���[�쐬
		D3DXCreateTextureFromFile(
			pDevice,
			g_paClearTexFile[nCntCom],
			&g_pTextureClearComment[nCntCom]);

		// �N���A�̃R�����g
		if (nCntCom == CLEARCOMMENTTYPE_CLEAR)
		{
			g_ClearCom[nCntCom].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
			g_ClearCom[nCntCom].size = D3DXVECTOR2(350.0f, 250.0f);
		}

		// �T���L���[�̃R�����g
		else if (nCntCom == CLEARCOMMENTTYPE_THANK)
		{
			g_ClearCom[nCntCom].pos = D3DXVECTOR2(900.0f, 600.0f);
			g_ClearCom[nCntCom].size = D3DXVECTOR2(500.0f, 130.0f);
		}

		// ���_�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x - g_ClearCom[nCntCom].size.x * 0.5f,
			g_ClearCom[nCntCom].pos.y - g_ClearCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[1].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x + g_ClearCom[nCntCom].size.x * 0.5f, 
			g_ClearCom[nCntCom].pos.y - g_ClearCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[2].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x - g_ClearCom[nCntCom].size.x * 0.5f,
			g_ClearCom[nCntCom].pos.y + g_ClearCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[3].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x + g_ClearCom[nCntCom].size.x * 0.5f, 
			g_ClearCom[nCntCom].pos.y + g_ClearCom[nCntCom].size.y * 0.5f,
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
	g_pVtxBuffClearComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// �N���A�R�����g�I������
//
// ------------------------------------------------------------------------------------------
void UninitClearComment(void)
{
	// �e�N�X�`���̊J��
	// �N���A�R�����g�J�E���g
	for (int nCntCom = 0; nCntCom < CLEARCOMMENTTYPE_MAX; nCntCom++)
	{
		if (g_pTextureClearComment[nCntCom] != NULL)
		{
			g_pTextureClearComment[nCntCom]->Release();
			g_pTextureClearComment[nCntCom] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffClearComment != NULL)
	{
		g_pVtxBuffClearComment->Release();
		g_pVtxBuffClearComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �N���A�R�����g�X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateClearComment(void)
{
}

// ------------------------------------------------------------------------------------------
//
// �N���A�R�����g�`�揈��
//
// ------------------------------------------------------------------------------------------

void DrawClearComment(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffClearComment, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �N���A�R�����g�J�E���g
	for (int nCntCom = 0; nCntCom < CLEARCOMMENTTYPE_MAX; nCntCom++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureClearComment[nCntCom]);

		// �N���A�R�����g�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 
			nCntCom * NUM_VERTEX, 
			NUM_POLYGON);
	}
}