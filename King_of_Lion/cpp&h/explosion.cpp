// ------------------------------------------------------------------------------------------
//
// �������� [explosion.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "explosion.h"
#include "polygon.h"
#include "player.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define EXPLOSION_MAX (128)								// �����̍ő吔 

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void CollisionExplosion(int nCntExplosion);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureExplosion[EXPLOSIONTYPE_MAX] = {};		// ���_�e�N�X�`���[�̃|�C���^
EXPLOSION				g_aExplosion[EXPLOSION_MAX];	// �������

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntExplosion;							// �J�E���g����
	char cTexName[EXPLOSIONTYPE_MAX][36] =		// �e�N�X�`���[��
	{
		"data/TEXTURE/Thunder.png" ,
		"data/TEXTURE/bullet/hibana.png"
	};

	// �����ݒ�
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].fSize = 0.0f;
		g_aExplosion[nCntExplosion].fAlpha = 0.0f;
		g_aExplosion[nCntExplosion].fDestSize = 0;
		g_aExplosion[nCntExplosion].fG = 0;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].nLife = 0;
		g_aExplosion[nCntExplosion].type = EXPLOSIONTYPE_THUNDER;
		g_aExplosion[nCntExplosion].nAttack = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	// �e�N�X�`���[�ǂݍ���
	for (int nCntTex = 0; nCntTex < EXPLOSIONTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, cTexName[nCntTex], &g_pTextureExplosion[nCntTex]);
	}

	// ���_���̍쐬
	MakeVertexExplosion(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitExplosion(void)
{
	for (int nCntTex = 0; nCntTex < EXPLOSIONTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureExplosion[nCntTex] != NULL)
		{
			g_pTextureExplosion[nCntTex]->Release();
			g_pTextureExplosion[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateExplosion(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	int nCntExplosion;	// �J�E���g����

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		// �g�p���
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			g_aExplosion[nCntExplosion].nCounterAnim++;	// �J�E���^�[���Z

			// �d���e�N�X�`���[
			if (g_aExplosion[nCntExplosion].type == EXPLOSIONTYPE_THUNDER)
			{
				// �����蔻��
				CollisionExplosion(nCntExplosion);
				if (g_aExplosion[nCntExplosion].nCounterAnim % 6 == 0)
				{
					// �e�N�X�`���ݒ�
					SetTexExplosion(pVtx,
						ANIMATION_SIX_FTEX * g_aExplosion[nCntExplosion].nPatternAnim,
						ANIMATION_SIX_FTEX + ANIMATION_SIX_FTEX * g_aExplosion[nCntExplosion].nPatternAnim);
					g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim++) % ANIMATION_SIX;	// 6���p�^�[���n��

					// �����I��
					if (g_aExplosion[nCntExplosion].nPatternAnim % ANIMATION_SIX == 0)
					{
						g_aExplosion[nCntExplosion].bUse = false;
					}
				}
			}
			// �Ήԃe�N�X�`���[
			if (g_aExplosion[nCntExplosion].type == EXPLOSIONTYPE_HIBANA)
			{
				// �傫������
				if (g_aExplosion[nCntExplosion].nCounterAnim <= 20)
				{
					g_aExplosion[nCntExplosion].fSize++;
				}
				// ����������
				else if(g_aExplosion[nCntExplosion].nCounterAnim <= 40)
				{
					g_aExplosion[nCntExplosion].fSize--;
				}
				// �����I��
				else
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}

			}
		}
		// �|�C���g���킹
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawExplosion(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	EXPLOSION *pExplosion = &g_aExplosion[0];	// �������
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// ��]�E�ʒu�̌v�Z�p�ϐ�
	int nCntExplosion;							// �J�E���g����

	// Z�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// �󋵂ɉ�����Z�e�X�g�����s����(�e�X�g���p�X����)

	// �A���t�@�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)

	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pExplosion->mtxWorldExplosion);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			g_aExplosion[nCntExplosion].mtxWorldExplosion._11 = mtxView._11;
			g_aExplosion[nCntExplosion].mtxWorldExplosion._12 = mtxView._21;
			g_aExplosion[nCntExplosion].mtxWorldExplosion._13 = mtxView._31;

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pExplosion->rot.y, pExplosion->rot.x, pExplosion->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pExplosion->mtxWorldExplosion,	// 1
				&pExplosion->mtxWorldExplosion,					// 2
				&mtxRot);									// 3

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pExplosion->pos.x, pExplosion->pos.y, pExplosion->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pExplosion->mtxWorldExplosion,	// 1
				&pExplosion->mtxWorldExplosion,					// 2
				&mtxTrans);									// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorldExplosion);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion[pExplosion->type]);

			// �����̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}

	// Z�e�X�g�̐ݒ��ʏ��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// �󋵂ɉ�����Z�e�X�g�����s����(�V�s�N�Z�� <= ���݂̃s�N�Z��)
															
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// �A���t�@�e�X�g�̐ݒ��߂�

}

// ------------------------------------------------------------------------------------------
// �����̐ݒ�
// ------------------------------------------------------------------------------------------
void SetExplosion(
	D3DXVECTOR3 pos,	// �ʒu
	EXPLOSIONTYPE type,	// �^�C�v
	float fSize, 		// �T�C�Y
	int nAttack			// �U����
)
{
	// �ϐ��錾
	POLYGON *pPolygon = GetPolygon();	// �|���S�����
	int nCntExplosion;		// �J�E���g�o���b�g

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	// �u���b�N���[�v
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_aExplosion[nCntExplosion].bUse)
		{
			// ���X�V
			g_aExplosion[nCntExplosion].pos = pos;

			g_aExplosion[nCntExplosion].nAttack = 5 + nAttack;
			g_aExplosion[nCntExplosion].type = type;
			g_aExplosion[nCntExplosion].fSize = fSize;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;
			// �|�C���g�X�V
			pVtx += nCntExplosion * 4;
			SetPosExplosion(pVtx,nCntExplosion);

			break;
		}
	}

	// �A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntExplosion;		// �J�E���g����

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EXPLOSION_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	// �����ݒ�
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		// ���_���W�̐ݒ�					
		pVtx[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);		// �l�ڂ̒��_

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �l�ڂ̒��_

		// �J���[
		pVtx[0].col = g_aExplosion[nCntExplosion].col;	// ��ڂ̐F�ݒ�
		pVtx[1].col = g_aExplosion[nCntExplosion].col;	// ��ڂ̐F�ݒ�
		pVtx[2].col = g_aExplosion[nCntExplosion].col;	// �O�ڂ̐F�ݒ�
		pVtx[3].col = g_aExplosion[nCntExplosion].col;	// �l�ڂ̐F�ݒ�

														// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �|�C���g���킹
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

// ------------------------------------------------------------------------------------------
// �����蔻����
// ------------------------------------------------------------------------------------------
void CollisionExplosion(int nCntExplosion)
{
	// �v���C���[���擾
	PLAYER *pPlayer = GetPlayer();

	if (pPlayer->status.bColition == false)
	{
		if (SquareBottom_Judg(g_aExplosion[nCntExplosion].pos,
			D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			1, 1, pPlayer->pos))
		{
			// �o�b�t�@���[�̃_���[�W����
			HitPlayer(g_aExplosion[nCntExplosion].nAttack);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����̏��
// ------------------------------------------------------------------------------------------
EXPLOSION *GetExplosion(void)
{
	return &g_aExplosion[0];
}

// ------------------------------------------------------------------------------------------
// �e�N�X�`���[�ݒ�
// ------------------------------------------------------------------------------------------
void SetTexExplosion(
	VERTEX_3D *pVtx,	// �ʒu���_���
	float fBeginTex,	// ���߂̃e�N�X�`���[
	float fEndTex		// �Ō�̃e�N�X�`���[
)
{
	// �e�N�X�`���[
	pVtx[0].tex = D3DXVECTOR2(fBeginTex, 0.0f);	// ��ڂ̐F�ݒ�
 	pVtx[1].tex = D3DXVECTOR2(fEndTex, 0.0f);	// ��ڂ̐F�ݒ�
	pVtx[2].tex = D3DXVECTOR2(fBeginTex, 1.0f);	// �O�ڂ̐F�ݒ�
	pVtx[3].tex = D3DXVECTOR2(fEndTex, 1.0f);	// �l�ڂ̐F�ݒ�
}

// ------------------------------------------------------------------------------------------
// ���_���W�̐ݒ�
// ------------------------------------------------------------------------------------------
void SetPosExplosion(
	VERTEX_3D *pVtx,	// �ʒu���_���
	int nCntExplosion	// �J�E���g
)
{
	// ���_���W�̐ݒ�					
	pVtx[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// ��ڂ̒��_
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// ��ڂ̒��_
	pVtx[2].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);	// �O�ڂ̒��_
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);		// �l�ڂ̒��_
}
