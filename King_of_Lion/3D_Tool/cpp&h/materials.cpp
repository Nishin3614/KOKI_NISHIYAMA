// ------------------------------------------------------------------------------------------
//
// �f�ޏ��� [materials.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "materials.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "arrangement.h"
#include "mouse.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_MATERIALS (1)
#define SIZE (0.5f)

// ------------------------------------------------------------------------------------------
// �\����
// ------------------------------------------------------------------------------------------
// ---------- �g���C�G ---------- //
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
void MoveMaterials(void);
void RotMaterials(void);
void SetMaterials(int nType,D3DXVECTOR3 pos,D3DXVECTOR3 rot);
void Update_DestroyMaterials(void);
void Update_DispMaterials(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPD3DXMESH		g_paMeshMaterials[MATERIALSTYPE_MAX] = {};				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER	g_paBuffMatMaterials[MATERIALSTYPE_MAX] = {};			// �}�e���A�����ւ̃|�C���^
DWORD			g_anNummatMaterials[MATERIALSTYPE_MAX];					// �}�e���A���̐�
LPDIRECT3DTEXTURE9  g_paTextureMaterials[TEXTURE_MATERIALS] = {};	// �e�N�X�`���ւ̃|�C���^
MATERIALS_PARTS		g_materialsParts[MATERIALSTYPE_MAX];			// �p�[�c���
MATERIALS			g_materials[ALL_MAX];						// �f�ޏ��
int				g_nMaterialsNumType = 0;							// �i���o�[�^�C�v
int				g_nMaterialsIdxShadow = 0;							// �e��ID

// 3D�p�̃Z�[�u�t�@�C��
const char g_cMatarialsFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/materials.txt",
	"data/SAVE/3D/Stage_2/materials.txt",
	"data/SAVE/3D/Stage_3/materials.txt",
	"data/SAVE/3D/Stage_4/materials.txt",
	"data/SAVE/3D/Stage_5/materials.txt"
};

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
	char *paMaterialsFile[MATERIALSTYPE_MAX] =
	{
		{ "data/MODEL/materials/00_wood_green.x" },						// [�Ζ�]00
		{ "data/MODEL/materials/00_wood_green.x" },						// [�Ζ�]00
		{ "data/MODEL/materials/00_wood_green.x" },						// [�Ζ�]00

	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_MATERIALS] =
	{
		{ "" }
	};

	// �p�[�c�̏�����
	for (nCntModelType = 0; nCntModelType < MATERIALSTYPE_MAX; nCntModelType++)
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
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		g_materials[nCntMaterials].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].nDrawType = 0;
		g_materials[nCntMaterials].nTexType = 0;
		g_materials[nCntMaterials].fLength = 0.0f;
		g_materials[nCntMaterials].bUse = false;
		g_materials[nCntMaterials].bDisp = false;
	}

	// �f�ޓǍ�
	LoadMaterials();
	
	// �e�̍쐬
	g_nMaterialsIdxShadow = SetShadow(g_materials[0].pos, g_materials[0].rot,D3DXVECTOR3(20.0f,0.0f,20.0f));
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitMaterials(void)
{
	for (int nCntMaterials = 0; nCntMaterials < MATERIALSTYPE_MAX; nCntMaterials++)
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
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();
	
	LONG		pMouseX = 0;
	LONG		pMouseY = 0;

	// �}�E�X�̏��擾
	pMouseX = GetMouseX();
	pMouseY = GetMouseY();

	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		// ���ݒn�ۑ�
		g_materials[nCntMaterials].posold = g_materials[nCntMaterials].pos;

		// �e�̈ʒu��ݒ�
		SetPositionShadow(g_nMaterialsIdxShadow, g_materials[nCntMaterials].pos);
	}

	// �폜
	Update_DestroyMaterials();
	// �z�u����Ƃ��ɕ\��
	Update_DispMaterials();

	// �͈�
	if (pMouseX < 1000.0f)
	{
		// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
		if (GetMousePressTrigger(0))
		{
			SetMaterials(0, pArrangement->pos, pArrangement->rot);
		}
	}

	// �Z�[�u
	if (GetKeyboardSpeacePress(DIK_F7))
	{
		// �f�ރf�[�^�̕ۑ�
		SaveMaterials();
	}
}

// ------------------------------------------------------------------------------------------
// �폜����
// ------------------------------------------------------------------------------------------
void Update_DestroyMaterials(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true && g_materials[nCntMaterials].bDisp == true)
		{
			// �u���b�N��X�͈�
			if (pArrangement->pos.x >
				g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
				pArrangement->pos.x <
				g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x&&
				pArrangement->pos.z >
				g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
				pArrangement->pos.z <
				g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
			{
				// �폜����
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_materials[nCntMaterials].bUse = false;
					g_materials[nCntMaterials].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �f�ނ�u���Ƃ��ɕ\������
// ------------------------------------------------------------------------------------------
void Update_DispMaterials(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	int nCntMaterials;

	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == false)
		{
			g_materials[nCntMaterials].bDisp = true;

			g_materials[nCntMaterials].pos = pArrangement->pos;

			g_materials[nCntMaterials].rot = pArrangement->rot;
			break;
		}
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
	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bDisp == true)
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
bool ColisionMaterials(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 * posOld,
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size,
	MATERIALS ** ppMaterialsRet)
{
	// �ϐ��錾
	bool bLand = false;		// �߂荞��ł��邩���Ȃ���
	int  nCntMaterials;			// �J�E���g��
							// �Ԓn��񂪂�������
	if (ppMaterialsRet != NULL)
	{
		*ppMaterialsRet = NULL;		// �Ԓn�̒��g�̏����k����
	}

	// �u���b�N���[�v
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{

		// �g�p���
		if (g_materials[nCntMaterials].bUse == true)
		{

			// �u���b�N��Y�͈�
			if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
				pos->y - size->y * 0.5f < g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
			{
				// �u���b�N��Z�͈�
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
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x - size->z * 0.5f;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}

					// �����蔻��(�E)
					else if (pos->x - size->z * 0.5f< g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x&&
						posOld->x - size->z * 0.5f >= g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x + size->z * 0.5f;

						// �ړ��ʂ̏�����
						move->x = 0.0f;
					}
				}

				// �u���b�N��X�͈�
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
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z - size->z * 0.5f;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}

					// �����蔻��(��)
					else if (pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z&&
						posOld->z - size->z * 0.5f >= g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z )
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->z = g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z + size->z * 0.5f;

						// �ړ��ʂ̏�����
						move->z = 0.0f;
					}
				}
			}

			// �u���b�N��Z�͈�
			if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
				pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
			{
				// �u���b�N��X�͈�
				if (pos->x + size->x * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
					pos->x - size->x * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
				{
					// �����蔻��(��
					if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
						posOld->y + size->y * 0.5f <= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y - size->y * 0.5f;

						// �ړ��ʂ̏�����
						move->y = 0.0f;
					}

					// �����蔻��(��)
					else if (pos->y - size->y * 0.5f< g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y&&
						posOld->y - size->y * 0.5f >= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
					{
						// �Ԓn�̒��g�̏��ɑ��
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// �߂荞��ł���
						bLand = true;

						// �u���b�N��̍���
						pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y + size->y * 0.5f;

						// �ړ��ʂ̏�����
						move->y = 0.0f;
					}
				}
			}
		}
	}
	return bLand;
}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void MoveMaterials(void)
{

}

// ------------------------------------------------------------------------------------------
// ��]����
// ------------------------------------------------------------------------------------------
void RotMaterials(void)
{

}

// ------------------------------------------------------------------------------------------
// �f�ސݒu����
// ------------------------------------------------------------------------------------------
void SetMaterials(int nType,D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �J�E���g�f��
	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == false)
		{
			// �ݒ����
			g_materials[nCntMaterials].pos = pos;
			g_materials[nCntMaterials].rot = rot;
			g_materials[nCntMaterials].nDrawType = nType;

			// �ő�l
			g_materials[nCntMaterials].vtxMaxMaterials =
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;

			// �ŏ��l
			g_materials[nCntMaterials].vtxMinMaterials =
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;

			// �T�C�Y
			g_materials[nCntMaterials].size =
				g_materialsParts[g_materials[nCntMaterials].nDrawType].size;

			g_materials[nCntMaterials].bUse = true;
			g_materials[nCntMaterials].bDisp = true;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �z�u�ꏊ���킩�邽�߂ɕ`�悵�Ă������̂��\���ɏ���
// ------------------------------------------------------------------------------------------
void NotDispMaterials(void)
{
	int nCntMaterials;

	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == false &&
			g_materials[nCntMaterials].bDisp == true)
		{
			g_materials[nCntMaterials].bDisp = false;

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
	
	// �����\�����Ă�����̂���������
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true)
		{
			g_materials[nCntMaterials].bUse = false;
			g_materials[nCntMaterials].bDisp = false;
		}
	}

	// �J�E���g������
	nCntMaterials = 0;

	// �t�@�C���J
	pFile = fopen(g_cMatarialsFIleName_3D[GetGameStage()], "r");

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
								// �ő�l
								g_materials[nCntMaterials].vtxMaxMaterials =
									g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;

								// �ŏ��l
								g_materials[nCntMaterials].vtxMinMaterials =
									g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;

								// �T�C�Y
								g_materials[nCntMaterials].size = 
									g_materialsParts[g_materials[nCntMaterials].nDrawType].size;
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
// �I�����Ă���I�u�W�F�N�g��ݒ�
// ------------------------------------------------------------------------------------------
bool SeleMaterials(int nCntMaterials)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	ARRANGEMENT	*pArrangement = GetArrangement();

	// �ړ�
	if (GetMousePress(0))
	{
		// �z�u�̈ʒu�����擾
		g_materials[nCntMaterials].pos = pArrangement->pos;
	}

	// �^�C�v�̕ω�
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_materials[nCntMaterials].nDrawType++;

		// ���E�l�܂ōs������
		if (g_materials[nCntMaterials].nDrawType >= MATERIALSTYPE_MAX)
		{
			// ������
			g_materials[nCntMaterials].nDrawType = 0;
		}
	}

	// �폜
	if (GetKeyboardSpeacePress(DIK_BACK))
	{
		g_materials[nCntMaterials].bDisp = false;
		g_materials[nCntMaterials].bUse = false;
	}

	return g_materials[nCntMaterials].bUse;
}

// ------------------------------------------------------------------------------------------
// �Z�[�u��񏈗�
// ------------------------------------------------------------------------------------------
void SaveMaterials(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntMaterials = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// �o�O�΍�J�E���g
	char	cWriteText[512];		// �����Ƃ��ď������ݗp
	char	cHeadText[128];			// �����̊i�[
	char	cEqual[8] = { "=" };	// �C�R�[���p

	char	cSharpEnter[8];			// �R�����g�P
	char	cComment[64];			// �R�����g�Q
	char	cEnter[8];				// �R�����g�R

									// �悭�g���镶��
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// ���̎�ގ��
	char *cMaterialsType[MATERIALSTYPE_MAX] =
	{
		{ "    ----------[00]:��----------" },				// [��]00
		{ "    ----------[00]:��----------" },				// [��]00
		{ "    ----------[00]:��----------" },				// [��]00
	};

	// ���f�[�^�̎��
	char *cMaterialsData[] =
	{
		{ "TYPE" },				// �^�C�v
		{ "POS" },				// �ʒu
		{ "ROT" },				// ��]
		{ "END_MATERIALSSET" }	// �G���h�t���A�[
	};

	// �Z�[�u�p //
	// �t�@�C���J
	pFile = fopen(g_cMatarialsFIleName_3D[GetGameStage()], "w");

	// �J����
	if (pFile != NULL)
	{
		//------------------ �R�����g ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# �Z�[�u�f��\n");

		strcat(cWriteText, "# Author : koki nishiyama\n");

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, cComment);

		strcat(cWriteText, cEnter);

		fputs(cWriteText, pFile);

		//------------------ �R�����g�I�� ------------------//

		strcpy(cWriteText, "SCRIPT");

		fputs(cWriteText, pFile);

		// �X�N���v�g���ʂ���
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{
				// ���̍ő吔�Ȃ�Z�[�u
				if (nCntMaterials < ALL_MAX - 1)
				{
					if (g_materials[nCntMaterials].bUse == true)
					{
						fputs(cEnter, pFile);

						// �f�ނ̖��O����
						strcpy(cWriteText, cMaterialsType[g_materials[nCntMaterials].nDrawType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    MATERIALSSET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_MATERIALSSET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cMaterialsData[nCntDataLoad]);

								// �^�C�v���ۑ�
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_materials[nCntMaterials].nDrawType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// �ʒu���ۑ�
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_materials[nCntMaterials].pos.x,
										g_materials[nCntMaterials].pos.y,
										g_materials[nCntMaterials].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_materials[nCntMaterials].rot.x,
										g_materials[nCntMaterials].rot.y,
										g_materials[nCntMaterials].rot.z);

									fputs(cHeadText, pFile);

									// �J�E���g�f�[�^������
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "END_MATERIALSSET") == 0)
								{
									sprintf(cHeadText, "    %s", &cWriteText);

									fputs(cHeadText, pFile);

									// �J�E���g�f�[�^������
									nCntDataLoad = 0;
								}

							}
						}
					}
					// �f�ރJ�E���g�ύX
					nCntMaterials++;
				}

				// �G���h�X�N���v�g
				else
				{
					fputs(cEnter, pFile);

					strcpy(cWriteText, "END_SCRIPT");

					fputs(cWriteText, pFile);
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	else
	{
		MessageBox(NULL, "�f�ރf�[�^�̏������݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
		// �t�@�C����
		fclose(pFile);
	}
}

// ------------------------------------------------------------------------------------------
// �z�u���ɂ킩��₷���\�������Ă������̂��\���ɏ���
// ------------------------------------------------------------------------------------------
void NotDisp_Materials(void)
{
	int nCntMaterials;

	// �f�ނ̃J�E���g
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		// ���g���Ă������
		if (g_materials[nCntMaterials].bUse == false &&
			g_materials[nCntMaterials].bDisp == true)
		{
			g_materials[nCntMaterials].bDisp = false;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �f�ޏ�񏈗�
// ------------------------------------------------------------------------------------------
MATERIALS *GetMaterials(void)
{
	return &g_materials[0];
}