// ------------------------------------------------------------------------------------------
//
// �������� [range.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "range.h"

#include "player.h"

#include "game.h"

#include "fade.h"

#include "Calculation.h"

#include "effect.h"

#include "gamefade.h"

#include "tutorial.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define RANGE_MAX (128)								// �����̍ő吔 
#define RANGE_SIZE (50)									// �T�C�Y
#define RANGE_PHONT "data/TEXTURE/range000.png"	// �����̎ʐ^
#define ANIMATION_EXP 1/8

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���[�V�����Z�b�g---------- //
typedef enum
{
	RANGETYPE_00_TUTORIAL = 0,
	RANGETYPE_01_BOSS,
	RANGETYPE_MAX
} RANGETYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexRange(LPDIRECT3DDEVICE9 pDevice);
void ColisionRange(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRange = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureRange = NULL;	// ���_�e�N�X�`���[�̃|�C���^
RANGE					g_aRange[RANGE_MAX];		// �������

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitRange(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntRange;								// �J�E���g����

												// �����ݒ�
	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		g_aRange[nCntRange].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRange[nCntRange].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRange[nCntRange].size = D3DXVECTOR3(100.0f, 3.0f, 0.0f);
		g_aRange[nCntRange].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRange[nCntRange].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);
		g_aRange[nCntRange].nType = RANGETYPE_00_TUTORIAL;
		g_aRange[nCntRange].bUse = false;
		g_aRange[nCntRange].bRange = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, RANGE_PHONT, &g_pTextureRange);

	// ���_���̍쐬
	MakeVertexRange(pDevice);

	// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		SetRange(
			D3DXVECTOR3(0.0f, 40.0f, -200.0f),
			D3DXVECTOR3(0.0f,0.0f,0.0f),
			RANGETYPE_00_TUTORIAL);
	}

	// �Q�[���p
	else if(GetGameStete() != GAMESTATE_BOSS)
	{
		SetRange(D3DXVECTOR3(-3200.0f, 40.0f, -3400.0f),
			D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),
			RANGETYPE_01_BOSS);
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitRange(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureRange != NULL)
	{
		g_pTextureRange->Release();
		g_pTextureRange = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRange != NULL)
	{
		g_pVtxBuffRange->Release();
		g_pVtxBuffRange = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateRange(void)
{
	ColisionRange();
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawRange(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	RANGE *pRange = &g_aRange[0];	// �������
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// ��]�E�ʒu�̌v�Z�p�ϐ�
	int nCntRange;							// �J�E���g����

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O����

	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++, pRange++)
	{
		if (pRange->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pRange->mtxWorldRange);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pRange->rot.y, pRange->rot.x, pRange->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pRange->mtxWorldRange,	// 1
				&pRange->mtxWorldRange,					// 2
				&mtxRot);									// 3

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pRange->pos.x, pRange->pos.y, pRange->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pRange->mtxWorldRange,	// 1
				&pRange->mtxWorldRange,					// 2
				&mtxTrans);									// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pRange->mtxWorldRange);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffRange, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRange);

			// �����̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRange * 4, 2);
		}
	}
	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
}

// ------------------------------------------------------------------------------------------
// �����̐ݒ�
// ------------------------------------------------------------------------------------------
void SetRange(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int			nType)
{
	// �ϐ��錾
	int nCntRange;		// �J�E���g�o���b�g

						// �u���b�N���[�v
	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_aRange[nCntRange].bUse)
		{
			// ���X�V
			g_aRange[nCntRange].pos = pos;
			g_aRange[nCntRange].rot = rot;
			g_aRange[nCntRange].nType = nType;
			g_aRange[nCntRange].bUse = true;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �v���C���[�������o��
// ------------------------------------------------------------------------------------------
void PlayerPos_Range(void)
{
	// �v���C���[���擾
	PLAYER *pPlayer = GetPlayer();

	// �J�E���g�͈�
	for (int nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// �͈͂ɓ����Ă���
		if (g_aRange[nCntRange].bRange == true)
		{
			// �ʒu���X�V
			pPlayer->pos = g_aRange[nCntRange].posOld;
			
			// �ړ��ʏ�����
			pPlayer->move.x = 0.0f;
			pPlayer->move.z = 0.0f;
			g_aRange[nCntRange].bRange = false;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexRange(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntRange;		// �J�E���g����

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * RANGE_MAX, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRange, NULL);
	
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffRange->Lock(0, 0, (void **)&pVtx, 0);

	// �����ݒ�
	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// ���_���W�̐ݒ�					
		pVtx[0].pos = D3DXVECTOR3(-g_aRange[nCntRange].size.x, g_aRange[nCntRange].size.y, 0.0f);		// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_aRange[nCntRange].size.x, g_aRange[nCntRange].size.y, 0.0f);		// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(-g_aRange[nCntRange].size.x, -g_aRange[nCntRange].size.y, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_aRange[nCntRange].size.x, -g_aRange[nCntRange].size.y, 0.0f);		// �l�ڂ̒��_

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �l�ڂ̒��_

		// �J���[
		pVtx[0].col = g_aRange[nCntRange].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aRange[nCntRange].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aRange[nCntRange].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aRange[nCntRange].col;	// �l�ڂ̐F�ݒ�

														// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �|�C���g���킹
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffRange->Unlock();
}

// ------------------------------------------------------------------------------------------
// �͈͂̓����蔻����
// ------------------------------------------------------------------------------------------
void ColisionRange(void)
{
	// �x�N�g���̓����蔻��
	D3DXVECTOR3 RangeLine;	// �O��̃v���C���[�̃u���b�N�̎n�_����̃x�N�g��
	D3DXVECTOR3 PlayerLine;	// �v���C���[�̃u���b�N�̎n�_����̃x�N�g��
	D3DXVECTOR3 PlayerOldLine;	// �O��̃v���C���[�̃u���b�N�̎n�_����̃x�N�g��
	D3DXVECTOR3 RangeBegin;
	D3DXVECTOR3 RangeLast;

	// �v���C���[���擾
	PLAYER		*pPlayer = GetPlayer();

	// �u���b�N���[�v
	for (int nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// �g�p���
		if (g_aRange[nCntRange].bUse == true)
		{
			RangeBegin =
				g_aRange[nCntRange].pos +
				D3DXVECTOR3(
					g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
					0.0f,
					g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y));

			RangeLast =
				g_aRange[nCntRange].pos +
				D3DXVECTOR3(
					-g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
					0.0f,
					-g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y));

			// �v���C���[�̐�
			PlayerLine =
				Difference_Between(
					pPlayer->pos,
					g_aRange[nCntRange].pos + D3DXVECTOR3(
						g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
						0.0f,
						g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y)));


			// �O��v���C���[�̐�
			PlayerOldLine =
				Difference_Between(
					pPlayer->posold,
					g_aRange[nCntRange].pos + D3DXVECTOR3(
						g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
						0.0f,
						g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y)));

			// �͈͂̐�
			RangeLine =
				Difference_Between(
					RangeLast,
					RangeBegin);

			float fCross;
			float fCrossOld;

			// X�EZ�̊p�x
			float fAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// Y�̊p�x
			float fYAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			fCross = Cross_product(
				PlayerLine,
				RangeLine);

			fCrossOld = Cross_product(
				PlayerOldLine,
				RangeLine);

			// �Q�[��
			if (GetMode() == MODE_GAME)
			{
				// �O��(����-)
				if (fCross <= 0 &&
					fCrossOld > 0)		// �O��̊O�ς�+
				{
					SetGameState(GAMESTATE_SELECT);
					g_aRange[nCntRange].bRange = true;
					g_aRange[nCntRange].posOld = pPlayer->posold;
				}
			}

			// �`���[�g���A��
			else
			{
				// �O��(����-)
				if (fCross >= 0 &&
					fCrossOld < 0)		// �O��̊O�ς�+
				{
					SetTutorialState(TUTORIALSSTATE_SELECT);
					g_aRange[nCntRange].bRange = true;
					g_aRange[nCntRange].posOld = pPlayer->posold;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����̏��
// ------------------------------------------------------------------------------------------
RANGE *GetRange(void)
{
	return &g_aRange[0];
}