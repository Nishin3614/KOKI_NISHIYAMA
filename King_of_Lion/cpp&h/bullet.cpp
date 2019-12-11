// ------------------------------------------------------------------------------------------
//
// �e���� [bullet.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "bullet.h"
#include "input.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "sound.h"

// �����蔻��p	//
#include "player.h"
#include "giraffe.h"
#include "buffalo.h"
#include "king_rion.h"
#include "shogira.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define BULLET_MAX (128)							// �ʂ̍ő吔
#define BULLET_SIZE (10)							// �T�C�Y
#define BULLET_TEXTURE (3)							// �e�N�X�`���[��
#define BULLET_PLAYER "data/TEXTURE/sunani.png"		// �v���C���[
#define BULLET_ENEMY "data/TEXTURE/bullet000.png"	// �G
#define BULLET_KING "data/TEXTURE/sunani.png"		// �{�X

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------���---------- //
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_BOSS,
	BULLETTYPE_MAX
} BULLETTYPE;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);	// ���_��񐶐�
// �X�V
void Update_DecisionBullet(int nCntBullet);			// �U���̓����蔻��
void Update_ColisionBullet(int nCntBullet);			// �����蔻��
void Update_EndEffectBullet(int nCntBullet);		// effect

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_paTextureBullet[BULLET_TEXTURE] = {};	// ���_�e�N�X�`���[�̃|�C���^
BULLET g_aBullet[BULLET_MAX];									// �e

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		// �ʒu�E��]�̏����ݒ�
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].nIdxShadow = 0;
		g_aBullet[nCntBullet].nPattern = 0;
		g_aBullet[nCntBullet].nCntAnim = 0;
		g_aBullet[nCntBullet].nType = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nAttack = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BULLET_PLAYER, &g_paTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, BULLET_ENEMY, &g_paTextureBullet[1]);
	D3DXCreateTextureFromFile(pDevice, BULLET_KING, &g_paTextureBullet[2]);

	// ���_���̍쐬
	MakeVertexBullet(pDevice);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_TEXTURE; nCntBullet++)
	{
		// �e�N�X�`���̊J��
		if (g_paTextureBullet[nCntBullet] != NULL)
		{
			g_paTextureBullet[nCntBullet]->Release();
			g_paTextureBullet[nCntBullet] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateBullet(void)
{
	// �ϐ��錾
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			// �^�C�v��
			switch (g_aBullet[nCntBullet].nType)
			{
				// �v���C���[�e
			case BULLETTYPE_PLAYER:
			case BULLETTYPE_BOSS:
				// �J�E���g�A�j��
				g_aBullet[nCntBullet].nCntAnim++;
				if (g_aBullet[nCntBullet].nCntAnim % 20 == 0)
				{		
					g_aBullet[nCntBullet].nPattern = (g_aBullet[nCntBullet].nPattern++) % ANIMATION_FOUR;	// 8���p�^�[���n��
					// �e�N�X�`���[�ݒ�
					SetTexBullet(pVtx,
						ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern,
						ANIMATION_FOUR_FTEX + ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern);
				}
				// �ړ�����
				move =
					D3DXVECTOR3(0.0f,
						0.0f,
						0.0f);

				// �J���[
				col =
					D3DXCOLOR(
						1.0f,
						0.3f,
						0.5f,
						1.0f);

				// �G�t�F�N�g�̐ݒ�
				SetEffect(
					g_aBullet[nCntBullet].pos,				// �ʒu
					move,									// �ړ���
					col,									// �F
					g_aBullet[nCntBullet].size.x + 10,	// �T�C�Y
					-1,										// �����T�C�Y
					0.0f,									// �d��
					5,										// �G�t�F�N�g���C�t
					0,										// �^�C�v
					0);
				break;
			case BULLETTYPE_ENEMY:
				// X�EZ�̊p�x
				fAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

				// Y�̊p�x
				fYAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

				// �ړ�����
				move =
					D3DXVECTOR3(sinf(fAngle) * 5.0f,
						cosf(fYAngle) * 5.0f,
						cosf(fAngle) * 5.0f);

				// �J���[
				col =
					D3DXCOLOR(
						0.5f + float(rand() % 50) / 100.0f,
						0.5f - float(rand() % 50) / 100.0f,
						0.0f,
						1.0f);

				// �G�t�F�N�g�̐ݒ�
				SetEffect(
					D3DXVECTOR3(													// �ʒu
						sinf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.x,
						cosf(fYAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.y,
						cosf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.z),

					move,															// �ړ���
					col,															// �F
					float(rand() % 10),												// �T�C�Y
					-0,																// �����T�C�Y
					0.0f,															// �d��
					10,																// �G�t�F�N�g���C�t
					0,																// �^�C�v
					0);
			default:
				break;
			}
			// ���݂̈ʒu�ۑ�
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
			// �ړ�
			g_aBullet[nCntBullet].pos.z +=g_aBullet[nCntBullet].move.z;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;

			// ���C�t����
			g_aBullet[nCntBullet].nLife--;
			// �e�̈ʒu��ݒ�
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			// ���C�t���O�ɂȂ�����
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				// ���g�p��Ԃ�
				g_aBullet[nCntBullet].bUse = false;
				// �e�̍폜
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				// �^�C�v��
				switch (g_aBullet[nCntBullet].nType)
				{
					// �v���C���[�e
				case BULLETTYPE_PLAYER:
				case BULLETTYPE_BOSS:
					// ����effect
					Update_EndEffectBullet(nCntBullet);
					break;
				case BULLETTYPE_ENEMY:
					break;
				}
				// ������
				g_aBullet[nCntBullet].nLife = 200;
			}

			// �L�����N�^�[�̓����蔻��
			Update_DecisionBullet(nCntBullet);

			// �����蔻��
			Update_ColisionBullet(nCntBullet);
		}
		// �|�C���g�A�b�v
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffBullet->Unlock();
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	BULLET *pBullet = &g_aBullet[0];

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// �r���[�}�g���b�N�X�̑���p
	D3DXMATRIX mtxView;

	// �A���t�@�e�X�g�̐ݒ�ɕύX
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ALPHAREF,200);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&pBullet->mtxWorldBullet);

			// ���݂̃r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			g_aBullet[nCntBullet].mtxWorldBullet._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorldBullet._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorldBullet._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorldBullet._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorldBullet._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorldBullet._23 = mtxView._32;

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				pBullet->rot.y, pBullet->rot.x, pBullet->rot.z);	// ���ꂼ��̉�]��

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&pBullet->mtxWorldBullet,	// 1
				&pBullet->mtxWorldBullet,					// 2
				&mtxRot);							// 3

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				pBullet->pos.x, pBullet->pos.y, pBullet->pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&pBullet->mtxWorldBullet,	// 1
				&pBullet->mtxWorldBullet,					// 2
				&mtxTrans);							// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pBullet->mtxWorldBullet);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_paTextureBullet[g_aBullet[nCntBullet].nType]);

			// �e�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}

	// �A���t�@�e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

// ------------------------------------------------------------------------------------------
// �e�N�X�`���[�ݒ�
// ------------------------------------------------------------------------------------------
void SetTexBullet(VERTEX_3D *pVtx,float fBeginTex,float fEndTex)
{
	// �e�N�X�`���[
	pVtx[0].tex = D3DXVECTOR2(fBeginTex, 0.0f);	// ��ڂ̐F�ݒ�
	pVtx[1].tex = D3DXVECTOR2(fEndTex, 0.0f);	// ��ڂ̐F�ݒ�
	pVtx[2].tex = D3DXVECTOR2(fBeginTex, 1.0f);	// �O�ڂ̐F�ݒ�
	pVtx[3].tex = D3DXVECTOR2(fEndTex, 1.0f);	// �l�ڂ̐F�ݒ�
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void SetBullet(
	D3DXVECTOR3 pos,	// �ʒu
	D3DXVECTOR3 move ,	// �ړ���
	D3DXVECTOR3 rot,	// ��]��
	D3DXVECTOR3 size,	// �T�C�Y
	int nLife,			// ���C�t
	int nType,			// �^�C�v
	int nAttack)		// �U����
{
	VERTEX_3D * pVtx;	// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	// �e���[�v
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		// �g�p���Ă��Ȃ����
		if (!g_aBullet[nCntBullet].bUse)
		{
			// SE
			PlaySound(SOUND_LABEL_SE_BEAM);

			g_aBullet[nCntBullet].nAttack = 0;

			// ���X�V
			g_aBullet[nCntBullet].pos = pos;

			// �ړ���
			g_aBullet[nCntBullet].move =
				D3DXVECTOR3(-sinf(rot.y) * move.x, move.y, -cosf(rot.y) * move.z);

			// �T�C�Y
			g_aBullet[nCntBullet].size = size;

			// ���C�t��
			g_aBullet[nCntBullet].nLife = nLife;

			// �^�C�v
			g_aBullet[nCntBullet].nType = nType;

			// �U����
			// �v���C���[
			if (g_aBullet[nCntBullet].nType == BULLETTYPE_PLAYER)
			{
				g_aBullet[nCntBullet].nAttack += 10 + nAttack;
			}

			// �G
			else if (g_aBullet[nCntBullet].nType == BULLETTYPE_ENEMY)
			{
				g_aBullet[nCntBullet].nAttack += 5 + nAttack;
			}

			// �g�p��Ԃ�
			g_aBullet[nCntBullet].bUse = true;

			// �e�̍쐬
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(
				g_aBullet[nCntBullet].pos,
				D3DXVECTOR3(BULLET_SIZE, 0.0f, BULLET_SIZE),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ���_���̍X�V
			pVtx += nCntBullet * 4;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBullet[nCntBullet].size.x,
				g_aBullet[nCntBullet].size.y, 0);	// ��ڂ̒��_
			pVtx[1].pos = D3DXVECTOR3(
				g_aBullet[nCntBullet].size.x, 
				g_aBullet[nCntBullet].size.y, 0);	// ��ڂ̒��_
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBullet[nCntBullet].size.x,
				-g_aBullet[nCntBullet].size.y, 0);	// �O�ڂ̒��_
			pVtx[3].pos = D3DXVECTOR3(
				g_aBullet[nCntBullet].size.x, 
				-g_aBullet[nCntBullet].size.y, 0);	// �l�ڂ̒��_
			// �e�N�X�`���[�ݒ�
			// �v���C���[
			if (g_aBullet[nCntBullet].nType == BULLETTYPE_PLAYER)
			{
				SetTexBullet(pVtx,
					ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern,
					ANIMATION_FOUR_FTEX + ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern);
			}
			// ����ȊO
			else
			{
				SetTexBullet(pVtx,
					0.0f,
					1.0f);
			}
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���_���̍쐬
// ------------------------------------------------------------------------------------------
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BULLET_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			-g_aBullet[nCntBullet].size.x,
			g_aBullet[nCntBullet].size.y, 0);	// ��ڂ̒��_
		pVtx[1].pos = D3DXVECTOR3(
			g_aBullet[nCntBullet].size.x,
			g_aBullet[nCntBullet].size.y, 0);	// ��ڂ̒��_
		pVtx[2].pos = D3DXVECTOR3(
			-g_aBullet[nCntBullet].size.x,
			-g_aBullet[nCntBullet].size.y, 0);	// �O�ڂ̒��_
		pVtx[3].pos = D3DXVECTOR3(
			g_aBullet[nCntBullet].size.x,
			-g_aBullet[nCntBullet].size.y, 0);	// �l�ڂ̒��_

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// ��ڂ̒��_
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �O�ڂ̒��_
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// �l�ڂ̒��_

		// �J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// ��ڂ̐F�ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// �O�ڂ̐F�ݒ�
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// �l�ڂ̐F�ݒ�

		// �|�C���g���킹
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffBullet->Unlock();
}

// ------------------------------------------------------------------------------------------
// �L�����N�^�[�̓����蔻��
// ------------------------------------------------------------------------------------------
void Update_DecisionBullet(int nCntBullet)
{
	// �ϐ��錾
	// ���擾
	PLAYER * pPlayer = GetPlayer();			// �v���C���[
	GIRAFFE * pGiraffe = GetGiraffe();		// �L����
	SHOGIRA * pShogira = GetShogira();		// �e�L����
	BUFFALO * pBuffalo = GetBuffalo();		// �o�b�t�@���[
	KING_RION *pKing_Rion = GetKing_Rion();	// �S�b�̉�

	// �G�̒e
	if (g_aBullet[nCntBullet].nType == BULLETTYPE_ENEMY)
	{
		if (pPlayer->status.nState != PLAYER_STATE_DIE)
		{
			// �L�����̒e�̒e�ƃv���C���[�̓����蔻��
			if ((pPlayer->status.fLength * pPlayer->status.fLength) +
				(g_aBullet[nCntBullet].size.x * g_aBullet[nCntBullet].size.x) >=
				(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x) *
				(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x) +
				(pPlayer->pos.y - g_aBullet[nCntBullet].pos.y) *
				(pPlayer->pos.y - g_aBullet[nCntBullet].pos.y) +
				(pPlayer->pos.z - g_aBullet[nCntBullet].pos.z) *
				(pPlayer->pos.z - g_aBullet[nCntBullet].pos.z))
			{
				// �v���C���[�̃_���[�W����
				HitPlayer(g_aBullet[nCntBullet].nAttack);

				// ���g�p��Ԃ�
				g_aBullet[nCntBullet].bUse = false;

				//// ��������
				//SetExplosion(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].nAttack);

				// �e�̍폜
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
			}
		}
	}

	// �v���C���[�̒e
	if (g_aBullet[nCntBullet].nType == BULLETTYPE_PLAYER)
	{
		// �J�E���g�L����
		for (int nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++, pGiraffe++)
		{
			// �L�������g�p��Ԃ̏ꍇ
			if (pGiraffe->bUse == true)
			{
				if (pGiraffe->status.nStatus != GIRAFFESTATUS_DIE)
				{
					// �v���C���[�̒e�̒e�ƃL�����̓����蔻��
					if ((pGiraffe->fLength * pGiraffe->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pGiraffe->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pGiraffe->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pGiraffe->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pGiraffe->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pGiraffe->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pGiraffe->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// �v���C���[�_���[�W����
						HitGiraffe(nCntGiraffe, g_aBullet[nCntBullet].nAttack);

						// ���g�p��Ԃ�
						g_aBullet[nCntBullet].bUse = false;

						// �^�C�v��
						switch (g_aBullet[nCntBullet].nType)
						{
							// �v���C���[�e
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// ����effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}

						// �e�̍폜
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}

		// �J�E���g�e�L����
		for (int nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++, pShogira++)
		{
			// �e�L�������g�p��Ԃ̏ꍇ
			if (pShogira->bUse == true)
			{
				if (pShogira->status.nStatus != GIRAFFESTATUS_DIE)
				{
					// �v���C���[�̒e�̒e�ƒe�L�����̓����蔻��
					if ((pShogira->fLength * pShogira->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pShogira->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pShogira->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pShogira->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pShogira->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pShogira->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pShogira->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// �v���C���[�_���[�W����
						HitShogira(nCntShogira, g_aBullet[nCntBullet].nAttack);

						// ���g�p��Ԃ�
						g_aBullet[nCntBullet].bUse = false;

						// �^�C�v��
						switch (g_aBullet[nCntBullet].nType)
						{
							// �v���C���[�e
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// ����effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}

						// �e�̍폜
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}

		// �J�E���g�o�b�t�@���[
		for (int nCntBuffalo = 0; nCntBuffalo < MAX_GIRAFFE; nCntBuffalo++, pBuffalo++)
		{
			// �o�b�t�@���[���g�p��Ԃ̏ꍇ
			if (pBuffalo->bUse == true)
			{
				if (pBuffalo->status.nStatus != GIRAFFESTATUS_DIE)
				{
					// �v���C���[�̒e�̒e�ƃo�b�t�@���[�̓����蔻��
					if ((pBuffalo->fLength * pBuffalo->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pBuffalo->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pBuffalo->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pBuffalo->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pBuffalo->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pBuffalo->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pBuffalo->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// �v���C���[�_���[�W����
						HitBuffalo(nCntBuffalo, g_aBullet[nCntBullet].nAttack);

						// ���g�p��Ԃ�
						g_aBullet[nCntBullet].bUse = false;

						// �^�C�v��
						switch (g_aBullet[nCntBullet].nType)
						{
							// �v���C���[�e
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// ����effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}
						// �e�̍폜
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}

		// �J�E���g�L����
		for (int nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++, pKing_Rion++)
		{
			// �L�������g�p��Ԃ̏ꍇ
			if (pKing_Rion->bUse == true)
			{
				if (pKing_Rion->status.nStatus != KING_RIONSTATUS_DIE)
				{
					// �v���C���[�̒e�̒e�ƃL�����̓����蔻��
					if ((pKing_Rion->fLength * pKing_Rion->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pKing_Rion->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pKing_Rion->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pKing_Rion->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pKing_Rion->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pKing_Rion->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pKing_Rion->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// �v���C���[�_���[�W����
						HitKing_Rion(nCntKing_Rion, g_aBullet[nCntBullet].nAttack);

						// ���g�p��Ԃ�
						g_aBullet[nCntBullet].bUse = false;

						// �^�C�v��
						switch (g_aBullet[nCntBullet].nType)
						{
							// �v���C���[�e
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// ����effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}

						// �e�̍폜
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �e�̓����蔻��
// ------------------------------------------------------------------------------------------
void Update_ColisionBullet(int nCntBullet)
{
	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_aBullet[nCntBullet].pos,
		&g_aBullet[nCntBullet].posOld,
		&g_aBullet[nCntBullet].move,
		&g_aBullet[nCntBullet].size,
		&g_aBullet[nCntBullet].pMaterials) != -1)
	{
		// ���g�p��Ԃ�
		g_aBullet[nCntBullet].bUse = false;

		// �^�C�v��
		switch (g_aBullet[nCntBullet].nType)
		{
			// �v���C���[�e
		case BULLETTYPE_PLAYER:
		case BULLETTYPE_BOSS:
			// ����effect
			Update_EndEffectBullet(nCntBullet);
			break;
		case BULLETTYPE_ENEMY:
			break;
		}

		// �e�̍폜
		DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_aBullet[nCntBullet].pos,
		&g_aBullet[nCntBullet].posOld,
		&g_aBullet[nCntBullet].move,
		&g_aBullet[nCntBullet].size,
		&g_aBullet[nCntBullet].pWall))
	{
		// ���g�p��Ԃ�
		g_aBullet[nCntBullet].bUse = false;

		// �^�C�v��
		switch (g_aBullet[nCntBullet].nType)
		{
			// �v���C���[�e
		case BULLETTYPE_PLAYER:
		case BULLETTYPE_BOSS:
			// ����effect
			Update_EndEffectBullet(nCntBullet);
			break;
		case BULLETTYPE_ENEMY:
			break;
		}

		// �e�̍폜
		DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
	}
}

// ------------------------------------------------------------------------------------------
// ����effect���
// ------------------------------------------------------------------------------------------
void Update_EndEffectBullet(int nCntBullet)
{
		// �ϐ��錾
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �^�C�v��
	switch (g_aBullet[nCntBullet].nType)
	{
		// �v���C���[�e
	case BULLETTYPE_PLAYER:
	case BULLETTYPE_BOSS:
		// �ړ�����
		move =
			D3DXVECTOR3(0.0f,
				1.0f,
				0.0f);

		// �J���[
		col =
			D3DXCOLOR(
				1.0f,
				0.0f,
				0.0f,
				1.0f);

		for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
		{
			// X�EZ�̊p�x
			fAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// Y�̊p�x
			fYAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// �G�t�F�N�g�̐ݒ�
			SetEffect(
				g_aBullet[nCntBullet].pos +
				D3DXVECTOR3(sinf(fAngle) * BULLET_SIZE,
					cosf(fYAngle) * BULLET_SIZE,
					cosf(fAngle) * BULLET_SIZE),// �ʒu
				move,									// �ړ���
				col,									// �F
				70,		// �T�C�Y
				-1,										// �����T�C�Y
				0.0f,									// �d��
				65,										// �G�t�F�N�g���C�t
				0,										// �^�C�v
				0);
		}
		// �J���[
		col =
			D3DXCOLOR(
				1.0f,
				0.3f,
				0.1f,
				1.0f);


		for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
		{
			// X�EZ�̊p�x
			fAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// Y�̊p�x
			fYAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// �G�t�F�N�g�̐ݒ�
			SetEffect(
				g_aBullet[nCntBullet].pos +
				D3DXVECTOR3(sinf(fAngle) * BULLET_SIZE,
					cosf(fYAngle) * BULLET_SIZE,
					cosf(fAngle) * BULLET_SIZE),// �ʒu
				move,									// �ړ���
				col,									// �F
				50,		// �T�C�Y
				-1,										// �����T�C�Y
				0.0f,									// �d��
				65,										// �G�t�F�N�g���C�t
				0,										// �^�C�v
				0);
		}

		break;
	case BULLETTYPE_ENEMY:
		// X�EZ�̊p�x
		fAngle =
			float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

		// Y�̊p�x
		fYAngle =
			float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

		// �ړ�����
		move =
			D3DXVECTOR3(sinf(fAngle) * 5.0f,
				cosf(fYAngle) * 5.0f,
				cosf(fAngle) * 5.0f);

		// �J���[
		col =
			D3DXCOLOR(
				0.5f + float(rand() % 50) / 100.0f,
				0.5f - float(rand() % 50) / 100.0f,
				0.0f,
				1.0f);

		// �G�t�F�N�g�̐ݒ�
		SetEffect(
			D3DXVECTOR3(													// �ʒu
				sinf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.x,
				cosf(fYAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.y,
				cosf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.z),

			move,															// �ړ���
			col,															// �F
			float(rand() % 10),												// �T�C�Y
			-0,																// �����T�C�Y
			0.0f,															// �d��
			10,																// �G�t�F�N�g���C�t
			0,																// �^�C�v
			0);
	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------
// �e���
// ------------------------------------------------------------------------------------------
BULLET *GetBullet(void) 
{
	return &g_aBullet[0];
}