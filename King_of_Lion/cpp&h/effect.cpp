// ------------------------------------------------------------------------------------------
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "effect.h"

#include "polygon.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define EFFECT_MAX (4096)								// �G�t�F�N�g�̍ő吔 
#define EFFECT_SIZE (20)							// �T�C�Y
#define EFFECT_PHONT "data/TEXTURE/shadow000.jpg"	// �G�t�F�N�g�̎ʐ^

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// ���_�e�N�X�`���[�̃|�C���^
EFFECT					g_aEffect[EFFECT_MAX];		// �G�t�F�N�g���

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntEffect;								// �J�E���g�G�t�F�N�g

												// �����ݒ�
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fAlpha = 0.0f;
		g_aEffect[nCntEffect].fDestSize = 0;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].fG = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EFFECT_PHONT, &g_pTextureEffect);

	// ���_���̍쐬
	MakeVertexEffect(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitEffect(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateEffect(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	int nCntEffect;		// �J�E���g�G�t�F�N�g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		// �g�p���
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// ���a�ω�
			g_aEffect[nCntEffect].fSize += g_aEffect[nCntEffect].fDestSize;

			// �d�͕ω�
			g_aEffect[nCntEffect].move.y += g_aEffect[nCntEffect].fG;

			// �A���t�@�l�ω�
			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fAlpha;

			// �|�W�V�����ω�
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fSize <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			// ���_���̐ݒ�
			// ���_���W�̐ݒ�					
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize,
				g_aEffect[nCntEffect].fSize, 0.0f);		// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize,
				g_aEffect[nCntEffect].fSize, 0.0f);		// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize,
				-g_aEffect[nCntEffect].fSize, 0.0f);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize,
				-g_aEffect[nCntEffect].fSize, 0.0f);		// �l�ڂ̒��_

			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;	// ��ڂ̐F�ݒ�
			pVtx[1].col = g_aEffect[nCntEffect].col;	// ��ڂ̐F�ݒ�
			pVtx[2].col = g_aEffect[nCntEffect].col;	// �O�ڂ̐F�ݒ�
			pVtx[3].col = g_aEffect[nCntEffect].col;	// �l�ڂ̐F�ݒ�

		}
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffEffect->Unlock();

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawEffect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	EFFECT *pEffect = &g_aEffect[0];			// �G�t�F�N�g���
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// ��]�E�ʒu�̌v�Z�p�ϐ�
	int nCntEffect;								// �J�E���g�G�t�F�N�g

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�ւ̒ǉ��������ݏ������݂𖳌�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// �󋵂ɉ�����Z�e�X�g�����s����(�e�X�g���p�X����)

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pEffect->mtxWorldEffect);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			g_aEffect[nCntEffect].mtxWorldEffect._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorldEffect._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorldEffect._13 = mtxView._31;

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pEffect->rot.y, pEffect->rot.x, pEffect->rot.z);	// ���ꂼ��̉�]��

																	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pEffect->mtxWorldEffect,	// 1
				&pEffect->mtxWorldEffect,					// 2
				&mtxRot);									// 3


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);	// ���ꂼ��̈ړ���

																	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pEffect->mtxWorldEffect,	// 1
				&pEffect->mtxWorldEffect,					// 2
				&mtxTrans);									// 3

															// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorldEffect);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			// �G�t�F�N�g�̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntEffect * 4,
				2);
		}
	}

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�e�X�g�̐ݒ��ʏ��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�e�X�g����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// �󋵂ɉ�����Z�e�X�g�����s����(�V�s�N�Z�� <= ���݂̃s�N�Z��)
}

// ------------------------------------------------------------------------------------------
// �G�t�F�N�g��{���̐ݒ�
// ------------------------------------------------------------------------------------------
void SetInformation(
	D3DXVECTOR3 pos,			// �ʒu
	D3DXCOLOR col,				// �F
	EFFECTAPPEARANCE Appearnce,	// �o����
	EFFECTMOVETYPE Movetype,	// �ړ��^�C�v
	int nMaxEffect,				// �ő吔
	float fSize,				// �T�C�Y
	float fSpeed, 				// ����
	int nLife					// ���C�t
)
{
	// �ϐ��錾
	D3DXVECTOR3 Effectpos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXCOLOR	Effectcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float		EffectfSize = 0;

	for (int nCntEffect = 0; nCntEffect < nMaxEffect; nCntEffect++)
	{
		switch (Appearnce)
		{
		case EFFECTAPPEARANCE_NORMAL:
			break;
		case EFFECTAPPEARANCE_RAND:
			break;
		case EFFECTAPPEARANCE_RING:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);

			break;
		}

		switch (Movetype)
		{
		case EFFECTMOVETYPE_NORMAL:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_UP:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_DOWN:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_G:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_RAND:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �G�t�F�N�g�ړ��̐ݒ�
// ------------------------------------------------------------------------------------------
void SetMove(
	D3DXVECTOR3 pos,			// �ʒu
	D3DXCOLOR col,				// �F
	EFFECTMOVETYPE Movetype,	// �ړ��^�C�v
	float fSize, 				// �T�C�Y
	float fSpeed,				// ����
	int nLife					// ���C�t
)
{
}

// ------------------------------------------------------------------------------------------
// �G�t�F�N�g�̐ݒ�
// ------------------------------------------------------------------------------------------
void SetEffect(
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR3 move,	// �ړ�
	D3DXCOLOR col,		// �F
	float fSize, 		// �T�C�Y
	float fDestSize,	// �T�C�Y�ύX
	float fG,			// �d��
	int nLife,			// ���C�t
	int nType,			// �^�C�v
	int nMaxEffect		// �ő吔
)
{
	// �ϐ��錾
	POLYGON *pPolygon = GetPolygon();	// �|���S�����
	int nCntEffect;		// �J�E���g�o���b�g

						// �u���b�N���[�v
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_aEffect[nCntEffect].bUse)
		{
			// ���X�V
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fSize = fSize;
			g_aEffect[nCntEffect].fDestSize = fDestSize;
			g_aEffect[nCntEffect].fG = fG;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntEffect;		// �J�E���g�G�t�F�N�g

						// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

						// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	// �����ݒ�
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		// ���_���W�̐ݒ�					
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize, g_aEffect[nCntEffect].fSize, 0.0f);		// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize, g_aEffect[nCntEffect].fSize, 0.0f);		// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize, -g_aEffect[nCntEffect].fSize, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize, -g_aEffect[nCntEffect].fSize, 0.0f);		// �l�ڂ̒��_

																										// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// �l�ڂ̒��_

														// �J���[
		pVtx[0].col = g_aEffect[nCntEffect].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aEffect[nCntEffect].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aEffect[nCntEffect].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aEffect[nCntEffect].col;	// �l�ڂ̐F�ݒ�

													// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

												// �|�C���g���킹
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffEffect->Unlock();
}

// ------------------------------------------------------------------------------------------
// �G�t�F�N�g�̏��
// ------------------------------------------------------------------------------------------
EFFECT *GetEffect(void)
{
	return &g_aEffect[0];
}