// ------------------------------------------------------------------------------------------
//
// �f�ޏ��� [materials.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "materials.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "player.h"
#include "effect.h"
#include "Billboard.h"
#include "game.h"
#include "dome.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_MATERIALS (1)
#define SIZE (0.5f)

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- �p�[�c��� ---------- //
typedef struct
{
	LPD3DXMESH		paMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	paBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD			nNumMat;			// �}�e���A���̐�
	D3DXVECTOR3		size;				// �T�C�Y
	D3DXVECTOR3		vtxMinMaterials;	// ���f���̈ʒu�̍ŏ��l
	D3DXVECTOR3		vtxMaxMaterials;	// ���f���̈ʒu�̍ő�l
} MATERIALS_PARTS;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_HitMaterials(int nCntMaterials);
void Update_PickMaterials(int nCntMaterials);
void Update_ColisionMaterials(int nCntMaterials);
void LoadMaterials(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPD3DXMESH			g_paMeshMaterials[MATERIALS_ITEM_MAX] = {};				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_paBuffMatMaterials[MATERIALS_ITEM_MAX] = {};			// �}�e���A�����ւ̃|�C���^
DWORD				g_anNummatMaterials[MATERIALS_ITEM_MAX];					// �}�e���A���̐�

LPDIRECT3DTEXTURE9  g_paTextureMaterials[TEXTURE_MATERIALS] = {};	// �e�N�X�`���ւ̃|�C���^

MATERIALS_PARTS		g_materialsParts[MATERIALS_ITEM_MAX];			// �p�[�c���

MATERIALS			g_materials[MATERIALS_MAX];						// �f�ޏ��

int					g_nMaterialsNumType = 0;							// �i���o�[�^�C�v

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitMaterials(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	CAMERA	*pCamera = GetCamera();				// �J�����̎擾
	int		nCntMaterials;						// �v���C���[�J�E���g
	int		nCntModelType;						// ���f���J�E���g
	int		nCntTexType;						// �e�N�X�`���[�J�E���g

	// �f�ރt�@�C������`
	char *paMaterialsFile[MATERIALS_ITEM_MAX] =
	{
		// �I�u�W�F�N�g�p //
		{ "data/MODEL/materials/00_wood_green.x" },		// [�Ζ�]00
		{ "data/MODEL/materials/03_wood_red.x" },		// [�Ԗ�]02
		{ "data/MODEL/materials/02_wood_orange.x" },	// [�I�����W��]01
														
		// �A�C�e���p //
		{ "data/MODEL/item/00_APPLE.x" },				// [�����S]00
		{ "data/MODEL/item/00_APPLE.x" }				// [�}�^�^�r]01

	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_MATERIALS] =
	{
		{ "" }
	};

	// �p�[�c�̏�����
	for (nCntModelType = 0; nCntModelType < MATERIALS_ITEM_MAX; nCntModelType++)
	{
		g_materialsParts[nCntModelType].paBuffMat		= NULL;
		g_materialsParts[nCntModelType].paMesh			= NULL;
		g_materialsParts[nCntModelType].nNumMat			= 0;
		g_materialsParts[nCntModelType].size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materialsParts[nCntModelType].vtxMaxMaterials	= D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
		g_materialsParts[nCntModelType].vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			paMaterialsFile[nCntModelType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_materialsParts[nCntModelType].paBuffMat,
			NULL,
			&g_materialsParts[nCntModelType].nNumMat,
			&g_materialsParts[nCntModelType].paMesh);

		// ���f���̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
		int nNumVertex;			// ���_��
		DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����擾
		nNumVertex = g_materialsParts[nCntModelType].paMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_materialsParts[nCntModelType].paMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_materialsParts[nCntModelType].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// ���_�����J��Ԃ�
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// �Ԓn����D3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
			// x //
			// �ŏ��l
			if (g_materialsParts[nCntModelType].vtxMinMaterials.x > vtx.x)
			{
				g_materialsParts[nCntModelType].vtxMinMaterials.x = vtx.x;
			}

			// �ő�l
			if (g_materialsParts[nCntModelType].vtxMaxMaterials.x < vtx.x)
			{
				g_materialsParts[nCntModelType].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// �ŏ��l
			if (g_materialsParts[nCntModelType].vtxMinMaterials.y > vtx.y)
			{
				g_materialsParts[nCntModelType].vtxMinMaterials.y = vtx.y;
			}

			// �ő�l
			if (g_materialsParts[nCntModelType].vtxMaxMaterials.y < vtx.y)
			{
				g_materialsParts[nCntModelType].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// �ŏ��l
			if (g_materialsParts[nCntModelType].vtxMinMaterials.z > vtx.z)
			{
				g_materialsParts[nCntModelType].vtxMinMaterials.z = vtx.z;
			}

			// �ő�l
			if (g_materialsParts[nCntModelType].vtxMaxMaterials.z < vtx.z)
			{
				g_materialsParts[nCntModelType].vtxMaxMaterials.z = vtx.z;
			}

			// �T�C�Y�|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
		}

		// �T�C�Y
		g_materialsParts[nCntModelType].size =
			g_materialsParts[nCntModelType].vtxMaxMaterials -
			g_materialsParts[nCntModelType].vtxMinMaterials;

		// �A�����b�N
		g_materialsParts[nCntModelType].paMesh->UnlockVertexBuffer();
	}

	// �e�N�X�`���[�Ăяo��
	for (nCntTexType = 0; nCntTexType < TEXTURE_MATERIALS; nCntTexType++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureMaterials[0]);
	}

	// �f�ނ̏����ݒ�
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_materials[nCntMaterials].bUse == true)
			{
				DeleteShadow(g_materials[nCntMaterials].IdxShadow);
			}
		}

		g_materials[nCntMaterials].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].size = g_materialsParts[0].size;
		g_materials[nCntMaterials].vtxMinMaterials = g_materialsParts[0].vtxMinMaterials;
		g_materials[nCntMaterials].vtxMaxMaterials = g_materialsParts[0].vtxMaxMaterials;
		g_materials[nCntMaterials].nDrawType = 0;
		g_materials[nCntMaterials].nTexType = 0;
		g_materials[nCntMaterials].nIdxDome = 0;
		g_materials[nCntMaterials].nSelect = 0;
		g_materials[nCntMaterials].fRadius = 50.0f;
		g_materials[nCntMaterials].bHit = false;
		g_materials[nCntMaterials].bUse = false;
		g_materials[nCntMaterials].bDisp = false;
	}

	// �f�ޓǍ�
	LoadMaterials();
	
	// �f�ނ̏����ݒ�
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true)
		{
			// �e�̍쐬
			g_materials[nCntMaterials].IdxShadow = 
				SetShadow(
					g_materials[nCntMaterials].pos,
					D3DXVECTOR3(20.0f, 0.0f, 20.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitMaterials(void)
{
	for (int nCntMaterials = 0; nCntMaterials < MATERIALS_ITEM_MAX; nCntMaterials++)
	{
		// ���b�V���̊J��
		if (g_materialsParts[nCntMaterials].paMesh != NULL)
		{
			g_materialsParts[nCntMaterials].paMesh->Release();
			g_materialsParts[nCntMaterials].paMesh = NULL;
		}	
		
		// �}�e���A���̊J��
		if (g_materialsParts[nCntMaterials].paBuffMat != NULL)
		{
			g_materialsParts[nCntMaterials].paBuffMat->Release();
			g_materialsParts[nCntMaterials].paBuffMat = NULL;
		}

	}


	for (int nCntTexture = 0; nCntTexture < TEXTURE_MATERIALS; nCntTexture++)
	{
		// �e�N�X�`���[�̊J��
		if (g_paTextureMaterials[nCntTexture] != NULL)
		{
			g_paTextureMaterials[nCntTexture]->Release();
			g_paTextureMaterials[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateMaterials(void)
{
	// �ϐ��錾
	int nCntMaterials;

	// �f�ނ̏����ݒ�
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true)
		{
			// ���ݒn�ۑ�
			g_materials[nCntMaterials].posold = g_materials[nCntMaterials].pos;

			// �A�C�e���̏ꍇ
			if (g_materials[nCntMaterials].nSelect == 1)
			{
				// �A�C�e�����E�������̏���
				Update_PickMaterials(nCntMaterials);

				// �@�������̏���
				Update_HitMaterials(nCntMaterials);
			}

			// �����蔻��
			Update_ColisionMaterials(nCntMaterials);

			// �e�̈ʒu��ݒ�
			SetPositionShadow(
				g_materials[nCntMaterials].IdxShadow,
				g_materials[nCntMaterials].pos);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �@������
// ------------------------------------------------------------------------------------------
void Update_HitMaterials(int nCntMaterials)
{
	// �ړ���
	g_materials[nCntMaterials].move.y -= 0.3f;

	// �ړ���
	g_materials[nCntMaterials].move.x *= 0.5f;
	g_materials[nCntMaterials].move.z *= 0.5f;

	// �ړ�
	g_materials[nCntMaterials].pos +=
		g_materials[nCntMaterials].move;
}

// ------------------------------------------------------------------------------------------
// �E��������
// ------------------------------------------------------------------------------------------
void Update_PickMaterials(int nCntMaterials)
{
	// �v���C���[���擾
	PLAYER *pPlayer = GetPlayer();

	// �v���C���[�ƃA�C�e���̓����蔻��
	if ((pPlayer->fLength * pPlayer->fLength) +
		(g_materials[nCntMaterials].fRadius *
			g_materials[nCntMaterials].fRadius) >=
			(pPlayer->pos.x - g_materials[nCntMaterials].pos.x) *
		(pPlayer->pos.x - g_materials[nCntMaterials].pos.x) +
		(pPlayer->pos.y - g_materials[nCntMaterials].pos.y) *
		(pPlayer->pos.y - g_materials[nCntMaterials].pos.y) +
		(pPlayer->pos.z - g_materials[nCntMaterials].pos.z) *
		(pPlayer->pos.z - g_materials[nCntMaterials].pos.z))
	{
		// �v���C���[��
		RecoveryPlayer(g_materials[nCntMaterials].ability.nHPRecovery,
			g_materials[nCntMaterials].ability.nMPRecovery);

		// ���g�p��Ԃ�
		g_materials[nCntMaterials].bUse = false;
		g_materials[nCntMaterials].bDisp = false;

		// �G�t�F�N�g
		float fAngle;
		float fYAngle;
		D3DXVECTOR3 move;
		D3DXCOLOR col;

		for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
		{
			// X�EZ�̊p�x
			fAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// Y�̊p�x
			fYAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

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
					g_materials[nCntMaterials].pos.x,
					g_materials[nCntMaterials].pos.y,
					g_materials[nCntMaterials].pos.z),

				move,															// �ړ���
				col,															// �F
				float(rand() % 10),												// �T�C�Y
				-0,																// �����T�C�Y
				0.0f,															// �d��
				10,																// �G�t�F�N�g���C�t
				0,																// �^�C�v
				0);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionMaterials(int nCntMaterials)
{
	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_materials[nCntMaterials].pos,
		&g_materials[nCntMaterials].posold,
		&g_materials[nCntMaterials].move,
		&g_materials[nCntMaterials].size,
		&g_materials[nCntMaterials].pFloor))
	{

	}

}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawMaterials(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;				// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;				// �}�e���A���f�[�^�̃|�C���^

	// �J�E���g�f��
	for (int nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_materials[nCntMaterials].mtxWorldMaterials);

			// ��]�𔽉f
			// �X�P�[���𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_materials[nCntMaterials].rot.y, g_materials[nCntMaterials].rot.x, g_materials[nCntMaterials].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_materials[nCntMaterials].mtxWorldMaterials,
				&g_materials[nCntMaterials].mtxWorldMaterials, &mtxRot);

			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_materials[nCntMaterials].pos.x, g_materials[nCntMaterials].pos.y, g_materials[nCntMaterials].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_materials[nCntMaterials].mtxWorldMaterials,	// 1
				&g_materials[nCntMaterials].mtxWorldMaterials,					// 2
				&mtxTrans);									// 3

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_materials[nCntMaterials].mtxWorldMaterials);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_materialsParts[g_materials[nCntMaterials].nDrawType].paBuffMat->GetBufferPointer();

			// �e�N�X�`���[�ݒ�
			pDevice->SetTexture(0, g_paTextureMaterials[g_materials[nCntMaterials].nTexType]);

			// �J�E���g�}�e���A��
			for (int nCntMat = 0; nCntMat < (int)g_materialsParts[g_materials[nCntMaterials].nDrawType].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �`��
				g_materialsParts[g_materials[nCntMaterials].nDrawType].paMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
int ColisionMaterials(
	D3DXVECTOR3 *pos,				// ���݂̃|�W�V����
	D3DXVECTOR3 *posOld,			// �O��̃|�W�V����
	D3DXVECTOR3 *move,				// �ړ���
	D3DXVECTOR3 *size,				// �T�C�Y
	MATERIALS	**ppMaterialsRet	// �f�ނ̃|�C���^�[�̃|�C���^�[
)
{
	// �ϐ��錾
	int nLand = -1;		// �ǂ��̓����蔻�肩
	int  nCntMaterials;	// �J�E���g��
							
	// �Ԓn��񂪂�������
	if (ppMaterialsRet != NULL)
	{
		*ppMaterialsRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �f�ރ��[�v
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		// �g�p���
		if (g_materials[nCntMaterials].bUse == true)
		{
			// �I�u�W�F�N�g�̓����蔻��
			if (g_materials[nCntMaterials].nSelect == 0)
			{
				// �f�ނ�Y�͈�
				if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
					pos->y - size->y * 0.5f < g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
				{
					// �f�ނ�Z�͈�
					if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
						pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
					{
						// �����蔻��(��)
						if (pos->x + size->z * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
							posOld->x + size->z * 0.5f <= g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x)
						{
							// �Ԓn�̒��g�̏��ɑ��
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// �߂荞��ł���
							nLand = 2;

							// �f�ޏ�̍���
							pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x - size->z * 0.5f;

							// �ړ��ʂ̏�����
							move->x = 0.0f;

						}

						// �����蔻��(�E)
						else if (pos->x - size->z * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x&&
							posOld->x - size->z * 0.5f >= g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
						{
							// �Ԓn�̒��g�̏��ɑ��
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// �߂荞��ł���
							nLand = 2;

							// �f�ޏ�̍���
							pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x + size->z * 0.5f;

							// �ړ��ʂ̏�����
							move->x = 0.0f;
						}
					}

					// �f�ނ�X�͈�
					if (pos->x + size->x * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
						pos->x - size->x * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
					{
						// �����蔻��(��O)
						if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
							posOld->z + size->z * 0.5f <= g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z)
						{
							// �Ԓn�̒��g�̏��ɑ��
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// �߂荞��ł���
							nLand = 2;

							// �f�ޏ�̍���
							pos->z = g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z - size->z * 0.5f;

							// �ړ��ʂ̏�����
							move->z = 0.0f;
						}

						// �����蔻��(��)
						else if (pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z&&
							posOld->z - size->z * 0.5f >= g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
						{
							// �Ԓn�̒��g�̏��ɑ��
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// �߂荞��ł���
							nLand = 2;

							// �f�ޏ�̍���
							pos->z = 
								g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z +
								size->z * 0.5f + 0.1f;

							// �ړ��ʂ̏�����
							move->z = 0.0f;
						}
					}
				}

				// �f�ނ�Z�͈�
				if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
					pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
				{
					// �f�ނ�X�͈�
					if (pos->x + size->x * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
						pos->x - size->x * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
					{
						// �����蔻��(��)
						if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
							posOld->y + size->y * 0.5f <= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y)
						{
							// �Ԓn�̒��g�̏��ɑ��
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// �߂荞��ł���
							nLand = 1;

							// �f�ޏ�̍���
							pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y - size->y * 0.5f;

							// �ړ��ʂ̏�����
							move->y = 0.0f;
						}

						// �����蔻��(��)
						else if (pos->y - size->y * 0.5f < g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y&&
							posOld->y - size->y * 0.5f >= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
						{
							// �Ԓn�̒��g�̏��ɑ��
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// �߂荞��ł���
							nLand = 0;

							// �f�ޏ�̍���
							pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y + size->y * 0.5f;

							// �ړ��ʂ̏�����
							move->y = 0.0f;
						}
					}
				}
			}
		}
	}
	return nLand;
}

// ------------------------------------------------------------------------------------------
// �ݒu����
// ------------------------------------------------------------------------------------------
void SetMaterials(
	D3DXVECTOR3 pos,			// �ʒu
	D3DXVECTOR3 rot,			// ��]
	D3DXVECTOR3 move,			// �ړ���
	int			nDrawType,		// �`��^�C�v
	int			nHPRecovery,	// HP�p
	int			nMPRecovery		// MP�p
)
{
	// �ϐ��錾
	int nCntMaterials;

	// �f�ރ��[�v
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		// �g�p���
		if (g_materials[nCntMaterials].bUse == false)
		{
			// ���ݒ�
			g_materials[nCntMaterials].pos = pos;										// �ʒu
			g_materials[nCntMaterials].rot = rot;										// ��]���
			g_materials[nCntMaterials].move = move;										// �ړ���
			g_materials[nCntMaterials].nDrawType = nDrawType;							// �`��^�C�v
			g_materials[nCntMaterials].vtxMinMaterials = 								// �ŏ��l
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;	
			g_materials[nCntMaterials].vtxMaxMaterials =								// �ő�l
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;	
			g_materials[nCntMaterials].ability.nHPRecovery = nHPRecovery;				// HP�񕜗�
			g_materials[nCntMaterials].ability.nMPRecovery = nMPRecovery;				// MP�񕜗�

			// �`��^�C�v���I�u�W�F�N�g�̏ꍇ
			if (g_materials[nCntMaterials].nDrawType < MATERIALS_OBJECT_MAX)
			{
				g_materials[nCntMaterials].nSelect = 0;
			}

			// �`��^�C�v���A�C�e���̏ꍇ
			else
			{
				g_materials[nCntMaterials].nSelect = 1;
			}
			g_materials[nCntMaterials].bUse = true;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �f�ޓǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadMaterials(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;
	int nCntMaterials = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/TUTORIAL/materials.txt", "r");
	}

	// �{�X�p
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/materials.txt", "r");
	}

	// ����ȊO
	else
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/materials.txt", "r");
	}


	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
		}

		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// ���s��������
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// ���f���Z�b�g��������
				else if (strcmp(cHeadText, "MATERIALSSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_MATERIALSSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_materials[nCntMaterials].nDrawType);

							// �T�C�Y
							g_materials[nCntMaterials].vtxMinMaterials =
								g_materialsParts[g_materials[nCntMaterials].nDrawType].size;

							// �ŏ��l�ƍő�l
							g_materials[nCntMaterials].vtxMinMaterials = 
								g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;
							g_materials[nCntMaterials].vtxMaxMaterials = 
								g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;

							// �`��^�C�v���I�u�W�F�N�g�̏ꍇ
							if (g_materials[nCntMaterials].nDrawType < MATERIALS_OBJECT_MAX)
							{
								g_materials[nCntMaterials].nSelect = 0;
							}

							// �`��^�C�v���A�C�e���̏ꍇ
							else
							{
								g_materials[nCntMaterials].nSelect = 1;
							}
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_materials[nCntMaterials].pos.x,
								&g_materials[nCntMaterials].pos.y,
								&g_materials[nCntMaterials].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_materials[nCntMaterials].rot.x,
								&g_materials[nCntMaterials].rot.y,
								&g_materials[nCntMaterials].rot.z);
						}
					}

					// �`���[�g���A
					if (GetMode() == MODE_TUTORIAL)
					{
						SetBillboard(D3DXVECTOR3(
							g_materials[nCntMaterials].pos.x + 100.0f,
							g_materials[nCntMaterials].pos.y + 50.0f ,
							g_materials[nCntMaterials].pos.z + 100.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							BILLBOARDTYPE_TREE);
					}

					// �Ԃ��؂̏ꍇ
					if (g_materials[nCntMaterials].nDrawType == MATERIALS_OBJECT_01_WOOD_RED)
					{
						g_materials[nCntMaterials].nIdxDome  = 
							SetDome(g_materials[nCntMaterials].pos, 
								D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
								0);
					}
					// �I�����W�؂̏ꍇ
					if (g_materials[nCntMaterials].nDrawType == MATERIALS_OBJECT_02_WOOD_ORANGE)
					{
						g_materials[nCntMaterials].nIdxDome =
							SetDome(g_materials[nCntMaterials].pos,
								D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f),
								0);
					}

					// �g�p��Ԃ�
					g_materials[nCntMaterials].bUse = true;

					// �\����Ԃ�
					g_materials[nCntMaterials].bDisp = true;

					// ���f���̍X�V
					nCntMaterials++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�f�ރf�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// �f�ޏ�񏈗�
// ------------------------------------------------------------------------------------------
MATERIALS *GetMaterials(void)
{
	return &g_materials[0];
}