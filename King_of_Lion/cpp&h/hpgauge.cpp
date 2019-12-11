// ------------------------------------------------------------------------------------------
// 
// �Q�[�W�̏��� [HpGauge.cpp]
// Author:Koki Nishiyama
// 
// ------------------------------------------------------------------------------------------
#include "hpgauge.h"
#include "player.h"

// ------------------------------------------------------------------------------------------
// 
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define MAX_HPGAUGE				(10)
#define MAX_HPMULTI				(5.0f)
#define HPGAUGE_TEXTURE_NAME		"data/TEXTURE/hpgauge000.png"
#define HPGAUGE_SIZE_X			(500)
#define HPGAUGE_SIZE_Y			(20)
#define HPGAUGE_WHITE_X			(1280)
#define HPGAUGE_WHITE_Y			(40)

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	HPGAUGETYPE_WHITE = 0,
	HPGAUGETYPE_BLOCK,
	HPGAUGETYPE_RED,
	HPGAUGETYPE_GREEN,
	HPGAUGETYPE_MAX
} HPGAUGETYPE;

// ------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3	pos;					// ���W
	D3DXCOLOR	col[HPGAUGETYPE_MAX];	// �F
	int			nType;					// ���
	float		fSize[HPGAUGETYPE_MAX];	// ���݃Q�[�W�̃T�C�Y
	float		fSizeLast;				// �ړI�Q�[�W�̃T�C�Y
	float		fBetSize;				// �����T�C�Y
	float		fSize_y;				// �Q�[�W�̃T�C�Y(y)
	bool		bUse[HPGAUGETYPE_MAX];	// �g���Ă邩�ǂ���
} HPGAUGE;

// ------------------------------------------------------------------------------------------
// 
// �O���[�o���ϐ�
// 
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureHpGauge = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHpGauge = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
HPGAUGE					g_aHpGauge[HPGAUGESYSTEM_MAX];

// ------------------------------------------------------------------------------------------
// 
// ����������
// 
// ------------------------------------------------------------------------------------------
void InitHpGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;
	int nCntType;		// �^�C�v�J�E���g
	int nCntHpGauge;	// HP�J�E���g

	// �Q�[�W�̏��̏�����
	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		g_aHpGauge[nCntHpGauge].nType = 0;								// ���
		g_aHpGauge[nCntHpGauge].fSizeLast = HPGAUGE_SIZE_X;				// �ړI�T�C�Y
		g_aHpGauge[nCntHpGauge].fBetSize = 0;							// �����T�C�Y
		g_aHpGauge[nCntHpGauge].fSize_y = 10;							// �T�C�Y(Y)

		// ���ꂼ��̒���
		for (nCntType = 0; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			g_aHpGauge[nCntHpGauge].fSize[nCntType] = g_aHpGauge[nCntHpGauge].fSizeLast;	// �T�C�Y(X)
			g_aHpGauge[nCntHpGauge].bUse[nCntType] = true;									// �g���Ă邩�ǂ���
		}

		// �Q�[�W�̐F
		g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// ���g
		g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_BLOCK] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// ���̃Q�[�W
		g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_RED] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �Ԃ̃Q�[�W

		// HP�Q�[�W
		if (nCntHpGauge == HPGAUGESYSTEM_HP)
		{
			g_aHpGauge[nCntHpGauge].pos = D3DXVECTOR3(50.0f, 30.0f, 0.0f);						// ���W

			g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_GREEN] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// �΂̃Q�[�W
		}

		// MP�Q�[�W
		else
		{
			g_aHpGauge[nCntHpGauge].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);						// ���W

			g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_GREEN] = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);	// �̃Q�[�W

			g_aHpGauge[nCntHpGauge].bUse[HPGAUGETYPE_WHITE] = false;							// �g���Ă邩�ǂ���
		}
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		HPGAUGE_TEXTURE_NAME,
		&g_pTextureHpGauge);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * HPGAUGESYSTEM_MAX * HPGAUGETYPE_MAX - 1,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffHpGauge,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffHpGauge->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		// ���g�쐬 //
		// ���_�̍��W
		pVtx[0].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(-100.0f, -40.0f, 0.0f);
		pVtx[1].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(HPGAUGE_WHITE_X, -40.0f, 0.0f);
		pVtx[2].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(-100.0f, HPGAUGE_WHITE_Y, 0.0f);
		pVtx[3].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(HPGAUGE_WHITE_X, HPGAUGE_WHITE_Y, 0.0f);

		// ���_�̓������W	
		pVtx[0].rhw = 1.0f;									// 1.0f�ŌŒ�
		pVtx[1].rhw = 1.0f;									// 1.0f�ŌŒ�
		pVtx[2].rhw = 1.0f;									// 1.0f�ŌŒ�
		pVtx[3].rhw = 1.0f;									// 1.0f�ŌŒ�

		// ���_�̐F
		pVtx[0].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (��,��,��,�����x)
		pVtx[1].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (��,��,��,�����x)
		pVtx[2].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (��,��,��,�����x)
		pVtx[3].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (��,��,��,�����x)

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// (�e�N�X�`����UV���W)
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// (�e�N�X�`����UV���W)
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// (�e�N�X�`����UV���W)
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// (�e�N�X�`����UV���W)

		pVtx += 4;

		for (nCntType = HPGAUGETYPE_BLOCK; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			// ���_�̍��W
			pVtx[0].pos = g_aHpGauge[nCntHpGauge].pos;
			pVtx[1].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], 0.0f, 0.0f);
			pVtx[2].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(0.0f, g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);
			pVtx[3].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);

			// ���_�̓������W	
			pVtx[0].rhw = 1.0f;									// 1.0f�ŌŒ�
			pVtx[1].rhw = 1.0f;									// 1.0f�ŌŒ�
			pVtx[2].rhw = 1.0f;									// 1.0f�ŌŒ�
			pVtx[3].rhw = 1.0f;									// 1.0f�ŌŒ�

			// �Q�[�W�{��
			if (nCntType == HPGAUGETYPE_GREEN)
			{
				// ���_�̐F
				pVtx[0].col = g_aHpGauge[nCntHpGauge].col[nCntType] + D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.5f);				// (��,��,��,�����x)
				pVtx[1].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (��,��,��,�����x)
				pVtx[2].col = g_aHpGauge[nCntHpGauge].col[nCntType] + D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.5f);				// (��,��,��,�����x)
				pVtx[3].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (��,��,��,�����x)

			}

			// �ԁE���̃Q�[�W
			else 
			{
				// ���_�̐F
				pVtx[0].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (��,��,��,�����x)
				pVtx[1].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (��,��,��,�����x)
				pVtx[2].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (��,��,��,�����x)
				pVtx[3].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (��,��,��,�����x)
			}

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// (�e�N�X�`����UV���W)
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// (�e�N�X�`����UV���W)
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// (�e�N�X�`����UV���W)
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// (�e�N�X�`����UV���W)

			pVtx += 4;
		}
	}

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffHpGauge->Unlock();
}

// ------------------------------------------------------------------------------------------
// 
// �I������
// 
// ------------------------------------------------------------------------------------------
void UninitHpGauge(void)
{
	
	// �e�N�X�`���̊J��
	if(g_pTextureHpGauge != NULL)
	{
		g_pTextureHpGauge->Release();
		g_pTextureHpGauge = NULL;
	}
	
	// ���_�o�b�t�@�̊J��
	if(g_pVtxBuffHpGauge != NULL)
	{
		g_pVtxBuffHpGauge->Release();
		g_pVtxBuffHpGauge = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 
// �X�V����
// 
// ------------------------------------------------------------------------------------------
void UpdateHpGauge(void)
{
	VERTEX_2D * pVtx;	// ���_���
	int nCntHpGauge;	// HP�Q�[�W�J�E���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffHpGauge->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		// �v���C���[��HP���ς������
		if (g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] != g_aHpGauge[nCntHpGauge].fSizeLast)
		{
			// �����������Z
			g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] -= g_aHpGauge[nCntHpGauge].fBetSize;

			// �ړI�̃T�C�Y�ʂɍ��킹��
			if (g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] < g_aHpGauge[nCntHpGauge].fSizeLast + MAX_HPMULTI)
			{
				g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] = g_aHpGauge[nCntHpGauge].fSizeLast;
			}
		}

		pVtx += 4;

		// �J�E���g�^�C�v
		for (int nCntType = HPGAUGETYPE_BLOCK; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			// ���_�̍��W
			pVtx[0].pos = g_aHpGauge[nCntHpGauge].pos;
			pVtx[1].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], 0.0f, 0.0f);
			pVtx[2].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(0.0f, g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);
			pVtx[3].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);

			pVtx += 4;
		}
	}
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffHpGauge->Unlock();
}

// ------------------------------------------------------------------------------------------
// 
// �`�揈��
// 
// ------------------------------------------------------------------------------------------
void DrawHpGauge(void)
{
	int nCntHpGauge;
	int nCntType;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffHpGauge, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHpGauge);

	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		for (nCntType = 0; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			if (g_aHpGauge[nCntHpGauge].bUse[nCntType] == true)
			{
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				// �v���~�e�B�u�̎��
					(nCntHpGauge * HPGAUGETYPE_MAX * 4) + nCntType * 4,		// �J�n����C���f�b�N�X(���_)  
					2);					// �|���S���̖���
			}

		}
	}
}

// ------------------------------------------------------------------------------------------
// 
// HP�Q�[�W�̕ω�
// 
// ------------------------------------------------------------------------------------------
void ChangeHpGauge(
	int nCntHpGauge,	// �J�E���g
	float fLife,		// ���C�t
	float fLastLife		// ������C�t
)
{
	// �T�C�Y�̍X�V
	g_aHpGauge[nCntHpGauge].fSizeLast = MAX_HPMULTI * fLastLife;

	// ���݂̃T�C�Y�ɑ��(��)
	g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_GREEN] = g_aHpGauge[nCntHpGauge].fSizeLast;

	// ��������
	g_aHpGauge[nCntHpGauge].fBetSize = 
		(g_aHpGauge[nCntHpGauge].fSizeLast - g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_BLOCK]) /
		(fLastLife - fLife);
}

// ------------------------------------------------------------------------------------------
// 
// �ő�HP�Q�[�W�̕ω�
// 
// ------------------------------------------------------------------------------------------
void ChangeMaxHpGauge(
	int nCntHpGauge,	// �J�E���g
	float fMaxHP		// �ő�HP
)
{
	// �ő�HP�Q�[�W�̕ύX
	g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_BLOCK] = MAX_HPMULTI * fMaxHP;
}
