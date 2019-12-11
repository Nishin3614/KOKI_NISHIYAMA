// ------------------------------------------------------------------------------------------
//
// STATISTICSUI�����̐���[statisticsUI.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "statisticsUI.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------�A�C�e���̎��---------- //
typedef enum
{
	STATISTICSUITYPE_BG = 0,	// �w�i
	STATISTICSUITYPE_ALL,		// ���v�p��UI
	STATISTICSUITYPE_MAX
} STATISTICSUITYPE;

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureStatisticsUI[STATISTICSUITYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStatisticsUI = NULL;					// �e�N�X�`���ւ̃|�C���^

// �ǂݍ��ރe�N�X�`��
char *g_paSTATISTICSUITexture[STATISTICSUITYPE_MAX] =
{
	{"data/TEXTURE/STATISTICS/NULL" },			// �w�i
	{"data/TEXTURE/STATISTICS/�X�R�A���v.png"}	// ���v�p��UI
};

// ------------------------------------------------------------------------------------------
//
// �^�C�g������������
//
// ------------------------------------------------------------------------------------------

void InitStatisticsUI(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���g
	int nCntStatisticsUI;

	// ������
	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			g_paSTATISTICSUITexture[nCntStatisticsUI],
			&g_pTextureStatisticsUI[nCntStatisticsUI]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * STATISTICSUITYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStatisticsUI,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffStatisticsUI->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		if (nCntStatisticsUI == STATISTICSUITYPE_BG)
		{
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				0.0f,
				0.0f,
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(
				SCREEN_WIDTH,
				0.0f,
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(
				0.0f,
				SCREEN_HEIGHT,
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				0.0f);
		}

		else
		{
			// ���_�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				0.0f,
				0.0f,
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(
				1100,
				0.0f,
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(
				0.0f,
				SCREEN_HEIGHT,
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(
				1100,
				SCREEN_HEIGHT,
				0.0f);

		}

		// ���_�쐬
		pVtx[0].rhw = 1.0f;									
		pVtx[1].rhw = 1.0f;									
		pVtx[2].rhw = 1.0f;									
		pVtx[3].rhw = 1.0f;									

		// �w�i
		if (nCntStatisticsUI == STATISTICSUITYPE_BG)
		{
			// �J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		// ���v�p��UI
		else
		{
			// �J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				

		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffStatisticsUI->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// STATISTICSUI�I������
//
// ------------------------------------------------------------------------------------------

void UninitStatisticsUI(void)
{
	int nCntStatisticsUI;

	// �e�N�X�`���̊J��
	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		if (g_pTextureStatisticsUI[nCntStatisticsUI] != NULL)
		{
			g_pTextureStatisticsUI[nCntStatisticsUI]->Release();
			g_pTextureStatisticsUI[nCntStatisticsUI] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffStatisticsUI != NULL)
	{
		g_pVtxBuffStatisticsUI->Release();
		g_pVtxBuffStatisticsUI = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���X�V����
//
// ------------------------------------------------------------------------------------------

void UpdateStatisticsUI(void)
{
}

// ------------------------------------------------------------------------------------------
//
// �^�C�g���`�揈��
//
// ------------------------------------------------------------------------------------------
void DrawStatisticsUI(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStatisticsUI;

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffStatisticsUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		pDevice->SetTexture(0, g_pTextureStatisticsUI[nCntStatisticsUI]);

		// �^�C�g���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStatisticsUI * 4, 2);
	}
}