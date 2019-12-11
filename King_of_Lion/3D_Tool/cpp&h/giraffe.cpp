// ------------------------------------------------------------------------------------------
//
// ���f������ [giraffe.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "giraffe.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "materials.h"
#include "arrangement.h"
#include "mouse.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_GIRAFFE (1)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void);
void Init_GiraffeInfo(void);
void Init_MotionGiraffe(void);
void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Update_DispGiraffe(void);
void LoadGiraffeModel(void);
void LoadGiraffeMosion(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureGiraffe[TEXTURE_GIRAFFE] = {};	// �e�N�X�`���ւ̃|�C���^
GIRAFFE				g_giraffe[ALL_MAX];							// �L�������
MODEL				g_giraffeModel[GIRAFFETYPE_MAX];			// ���f�����
MOSION_GIRAFFE		g_giraffeKey[GIRAFFEMOSIONSET_MAX];			// ���[�V�������
D3DXVECTOR3			g_gireffe_vtxMax;							// �ő�l
D3DXVECTOR3			g_gireffe_vtxMin;							// �ŏ��l
D3DXVECTOR3			g_gireffe_size;								// �T�C�Y

// 3D�p�̃Z�[�u�t�@�C��
const char g_cGiraffeFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/giraffe.txt",
	"data/SAVE/3D/Stage_2/giraffe.txt",
	"data/SAVE/3D/Stage_3/giraffe.txt",
	"data/SAVE/3D/Stage_4/giraffe.txt",
	"data/SAVE/3D/Stage_5/giraffe.txt"
};

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitGiraffe(void)
{
	// �L�����p�[�c���
	Init_PartsGiraffe();

	// �L�������
	Init_GiraffeInfo();

	// �L�������[�V�������
	Init_MotionGiraffe();
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitGiraffe(void)
{
	// ���f���f�[�^�̊J��
	for (int nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		// ���b�V���̊J��
		if (g_giraffeModel[nCntModel].paMesh != NULL)
		{
			g_giraffeModel[nCntModel].paMesh->Release();
			g_giraffeModel[nCntModel].paMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_giraffeModel[nCntModel].paBuffMat != NULL)
		{
			g_giraffeModel[nCntModel].paBuffMat->Release();
			g_giraffeModel[nCntModel].paBuffMat = NULL;
		}
	}

	// ���f���e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < TEXTURE_GIRAFFE; nCntTexture++)
	{
		// �e�N�X�`���[�̊J��
		if (g_paTextureGiraffe[nCntTexture] != NULL)
		{
			g_paTextureGiraffe[nCntTexture]->Release();
			g_paTextureGiraffe[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateGiraffe(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	// �z�u����Ƃ��ɕ\��
	Update_DispGiraffe();

	// ��ʓ������N���b�N�ŁA�J�[�\���̈ʒu�ɓG�z�u
	if (GetMousePressTrigger(0))
	{
		SetGiraffe(pArrangement->pos, pArrangement->rot);
	}

	// �Z�[�u
	if (GetKeyboardSpeacePress(DIK_F7))
	{
		// �f�ރf�[�^�̕ۑ�
		SaveGiraffe();
	}
}

// ------------------------------------------------------------------------------------------
// �f�ނ�u���Ƃ��ɕ\������
// ------------------------------------------------------------------------------------------
void Update_DispGiraffe(void)
{
	// �z�u���
	ARRANGEMENT *pArrangement = GetArrangement();

	int nCntGiraffe;

	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false)
		{
			g_giraffe[nCntGiraffe].bDisp = true;

			g_giraffe[nCntGiraffe].pos = pArrangement->pos;

			g_giraffe[nCntGiraffe].rot = pArrangement->rot;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawGiraffe(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;				// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;				// �}�e���A���f�[�^�̃|�C���^
	int nCntGiraffe;
	int nCntModel;

	// �J�E���g���f��
	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bDisp)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_giraffe[nCntGiraffe].mtxWorldGiraffe);

			// ��]�𔽉f
			// �X�P�[���𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_giraffe[nCntGiraffe].rot.y, 
				g_giraffe[nCntGiraffe].rot.x, 
				g_giraffe[nCntGiraffe].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].mtxWorldGiraffe,
				&g_giraffe[nCntGiraffe].mtxWorldGiraffe, &mtxRot);


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_giraffe[nCntGiraffe].pos.x, 
				g_giraffe[nCntGiraffe].pos.y, 
				g_giraffe[nCntGiraffe].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].mtxWorldGiraffe,	// 1
				&g_giraffe[nCntGiraffe].mtxWorldGiraffe,					// 2
				&mtxTrans);									// 3

			// �J�E���g���f��
			for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_giraffe[nCntGiraffe].model[nCntModel].rot.y,
					g_giraffe[nCntGiraffe].model[nCntModel].rot.x, 
					g_giraffe[nCntGiraffe].model[nCntModel].rot.z);

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
				D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
					&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel, &mtxRot);

				// �ʒu�𔽉f //
				// ���s�ړ��s��쐬(�I�t�Z�b�g)
				D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
					g_giraffe[nCntGiraffe].model[nCntModel].pos.x,
					g_giraffe[nCntGiraffe].model[nCntModel].pos.y, 
					g_giraffe[nCntGiraffe].model[nCntModel].pos.z);	// ���ꂼ��̈ړ���

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
				D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,	// 1
					&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [���ׂĂ̐e]����Ȃ��ꍇ
				if (g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent != -1)
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].mtxWorldGiraffe);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)g_giraffeModel[nCntModel].paBuffMat->GetBufferPointer();

				// �e�N�X�`���[�ݒ�
				pDevice->SetTexture(0, g_paTextureGiraffe[0]);

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_giraffeModel[nCntModel].nNumMat; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �`��
					g_giraffeModel[nCntModel].paMesh->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I�����Ă���I�u�W�F�N�g��ݒ�
// ------------------------------------------------------------------------------------------
bool SeleGiraffe(int nCntGiraffe)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	ARRANGEMENT	*pArrangement = GetArrangement();

	// �ړ�
	if (GetMousePress(0))
	{
		// �z�u�̈ʒu�����擾
		g_giraffe[nCntGiraffe].pos = pArrangement->pos;
	}

	// �폜
	if (GetKeyboardSpeacePress(DIK_BACK))
	{
		g_giraffe[nCntGiraffe].bDisp = false;
		g_giraffe[nCntGiraffe].bUse = false;
	}

	return g_giraffe[nCntGiraffe].bUse;
}

// ------------------------------------------------------------------------------------------
// �L�����ݒu����
// ------------------------------------------------------------------------------------------
void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �J�E���g�f��
	for (int nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false)
		{
			// �ݒ����
			g_giraffe[nCntGiraffe].pos = pos;
			g_giraffe[nCntGiraffe].rot = rot;

			g_giraffe[nCntGiraffe].bUse = true;
			g_giraffe[nCntGiraffe].bDisp = true;
			break;
		}
	}
}


// ------------------------------------------------------------------------------------------
// �z�u�ꏊ���킩�邽�߂ɕ`�悵�Ă������̂��\���ɏ���
// ------------------------------------------------------------------------------------------
void NotDispGiraffe(void)
{
	int nCntGiraffe;

	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false &&
			g_giraffe[nCntGiraffe].bDisp == true)
		{
			g_giraffe[nCntGiraffe].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �L�����p�[�c��񏈗�
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	int		nCntModel;							// ���f���̃J�E���g

	// ���f���t�@�C������`
	char *paGiraffeFile[GIRAFFETYPE_MAX] =
	{
		{ GIRAFFE_TEX1"/giraffe/00_Giraffe_body.x" },			// [0]��
		{ GIRAFFE_TEX1"/giraffe/01_Giraffe_JointRoot_Neck.x" },	// [1]��̍����̊֐�
		{ GIRAFFE_TEX1"/giraffe/02_Giraffe_Neck.x" },			// [2]��
		{ GIRAFFE_TEX1"/giraffe/03_Giraffe_JointTip_Neck.x" },	// [3]��̐�[�̊֐�
		{ GIRAFFE_TEX1"/giraffe/04_Giraffe_Head.x" },			// [4]��
		{ GIRAFFE_TEX1"/giraffe/05_Giraffe_RightHand.x" },		// [5]�E��
		{ GIRAFFE_TEX1"/giraffe/06_Giraffe_LeftHand.x" },		// [6]����
		{ GIRAFFE_TEX1"/giraffe/07_Giraffe_RightLeg.x" },		// [7]�E��
		{ GIRAFFE_TEX1"/giraffe/08_Giraffe_LeftLeg.x" },		// [8]����
		{ GIRAFFE_TEX1"/giraffe/09_Giraffe_Joint_Tail.x" },		// [9]�����ۂ̊֐�
		{ GIRAFFE_TEX1"/giraffe/10_Giraffe_JTail.x" }			// [10]������
	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_GIRAFFE] =
	{
		{ "data/TEXTURE/giraffe/00_giraffe_Texture.png" },
	};

	// �p�[�c�̓ǂݍ��� //
	for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		g_giraffeModel[nCntModel].paBuffMat			= NULL;
		g_giraffeModel[nCntModel].paMesh			= NULL;
		g_giraffeModel[nCntModel].nNumMat			= 0;
		g_giraffeModel[nCntModel].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].posLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].posKeyBetween		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rotLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rotKeyBetween		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].size				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].vtxMinMaterials	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].vtxMaxMaterials	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].nIdxModelParent	= 0;
		g_giraffeModel[nCntModel].nFram				= 0;
		g_giraffeModel[nCntModel].nCntKeySet		= 0;

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			paGiraffeFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_giraffeModel[nCntModel].paBuffMat,
			NULL,
			&g_giraffeModel[nCntModel].nNumMat,
			&g_giraffeModel[nCntModel].paMesh);

		// ���f���̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
		int nNumVertex;			// ���_��
		DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

								// ���_�����擾
		nNumVertex = g_giraffeModel[nCntModel].paMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_giraffeModel[nCntModel].paMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_giraffeModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// ���_�����J��Ԃ�
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// �Ԓn����D3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
			// x //
			// �ŏ��l
			if (g_giraffeModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// �ő�l
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// x //
			// �ŏ��l
			if (g_gireffe_vtxMin.x > g_giraffeModel[nCntModel].vtxMinMaterials.x)
			{
				g_gireffe_vtxMin.x = g_giraffeModel[nCntModel].vtxMinMaterials.x;
			}

			// �ő�l
			if (g_gireffe_vtxMax.x < g_giraffeModel[nCntModel].vtxMaxMaterials.x)
			{
				g_gireffe_vtxMax.x = g_giraffeModel[nCntModel].vtxMaxMaterials.x;
			}

			// y //
			// �ŏ��l
			if (g_giraffeModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// �ő�l
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// y //
			// �ŏ��l
			if (g_gireffe_vtxMin.y > g_giraffeModel[nCntModel].vtxMinMaterials.y)
			{
				g_gireffe_vtxMin.y = g_giraffeModel[nCntModel].vtxMinMaterials.y;
			}

			// �ő�l
			if (g_gireffe_vtxMax.y < g_giraffeModel[nCntModel].vtxMaxMaterials.y)
			{
				g_gireffe_vtxMax.y = g_giraffeModel[nCntModel].vtxMaxMaterials.y;
			}

			// z //
			// �ŏ��l
			if (g_giraffeModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// �ő�l
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// z //
			// �ŏ��l
			if (g_gireffe_vtxMin.z > g_giraffeModel[nCntModel].vtxMinMaterials.z)
			{
				g_gireffe_vtxMin.z = g_giraffeModel[nCntModel].vtxMinMaterials.z;
			}

			// �ő�l
			if (g_gireffe_vtxMax.z < g_giraffeModel[nCntModel].vtxMaxMaterials.z)
			{
				g_gireffe_vtxMax.z = g_giraffeModel[nCntModel].vtxMaxMaterials.z;
			}

			// �T�C�Y�|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
		}

		// �A�����b�N
		g_giraffeModel[nCntModel].paMesh->UnlockVertexBuffer();

		// �ő�l�ƍŏ��l�̋���
		g_giraffeModel[nCntModel].size = g_giraffeModel[nCntModel].vtxMaxMaterials - g_giraffeModel[nCntModel].vtxMinMaterials;
		g_gireffe_size = g_gireffe_vtxMax - g_gireffe_vtxMin;
	}

	// �p�[�c�Ǎ�
	LoadGiraffeModel();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureGiraffe[0]);
}

// ------------------------------------------------------------------------------------------
// �L������񏈗�
// ------------------------------------------------------------------------------------------
void Init_GiraffeInfo(void)
{
	// �ϐ��錾
	int		nCntGiraffe;	// �v���C���[�J�E���g
	int		nCntModel;		// ���f���̃J�E���g

	// �L�����̏����ݒ� //
	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		g_giraffe[nCntGiraffe].pos				= D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_giraffe[nCntGiraffe].posold			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posKeyBetween	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posOrigin		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].move				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotbetween		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotKeyBetween	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].vtxMinMaterials	= g_gireffe_vtxMin;
		g_giraffe[nCntGiraffe].vtxMaxMaterials	= g_gireffe_vtxMax;
		g_giraffe[nCntGiraffe].size				= g_gireffe_size;
		g_giraffe[nCntGiraffe].nMotionType		= 0;
		g_giraffe[nCntGiraffe].nMotionTypeOld	= 0;
		g_giraffe[nCntGiraffe].nParent			= 0;
		g_giraffe[nCntGiraffe].nIndex			= nCntGiraffe;
		g_giraffe[nCntGiraffe].nFram			= 0;
		g_giraffe[nCntGiraffe].nCntKeySet		= 0;
		g_giraffe[nCntGiraffe].nIdxShadow		= nCntGiraffe;
		g_giraffe[nCntGiraffe].fLength			= 0.0f;
		g_giraffe[nCntGiraffe].bUse				= false;
		g_giraffe[nCntGiraffe].bDisp			= false;

		// �p�[�c���Ƃ̃J�E���g
		for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
		{
			g_giraffe[nCntGiraffe].model[nCntModel].pos				= g_giraffeModel[nCntModel].pos;
			g_giraffe[nCntGiraffe].model[nCntModel].posLast			= g_giraffeModel[nCntModel].posLast;
			g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween	= g_giraffeModel[nCntModel].posKeyBetween;
			g_giraffe[nCntGiraffe].model[nCntModel].rot				= g_giraffeModel[nCntModel].rot;
			g_giraffe[nCntGiraffe].model[nCntModel].rotLast			= g_giraffeModel[nCntModel].rotLast;
			g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween	= g_giraffeModel[nCntModel].rotKeyBetween;
			g_giraffe[nCntGiraffe].model[nCntModel].size			= g_giraffeModel[nCntModel].size;
			g_giraffe[nCntGiraffe].model[nCntModel].vtxMinMaterials = g_giraffeModel[nCntModel].vtxMinMaterials;
			g_giraffe[nCntGiraffe].model[nCntModel].vtxMaxMaterials = g_giraffeModel[nCntModel].vtxMaxMaterials;
			g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent = g_giraffeModel[nCntModel].nIdxModelParent;
			g_giraffe[nCntGiraffe].model[nCntModel].nFram			= g_giraffeModel[nCntModel].nFram;
			g_giraffe[nCntGiraffe].model[nCntModel].nCntKeySet		= g_giraffeModel[nCntModel].nCntKeySet;
		}
	}
	// ���[�h�L����
	LoadGiraffe();
}

// ------------------------------------------------------------------------------------------
// �L�������[�V��������
// ------------------------------------------------------------------------------------------
void Init_MotionGiraffe(void)
{
	// �ϐ��錾
	// ���[�V�����p�ϐ�
	int		nCntMotion;							// ���[�V�����J�E���g
	int		nCntKeySet;							// ���[�V�����J�E���g
	int		nCntKey;							// �L�[�J�E���g

												// ���[�V�����J�E���g //
	for (nCntMotion = 0; nCntMotion < GIRAFFEMOSIONSET_MAX; nCntMotion++)
	{
		// ���[�V�����̏����ݒ�
		g_giraffeKey[nCntMotion].nNumKey	= 0;		// �i���o�[�L�[
		g_giraffeKey[nCntMotion].nLoop		= 0;		// ���[�v�̗L��
		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// �L�[���Ƃɏ�����
			for (nCntKey = 0; nCntKey < GIRAFFETYPE_MAX; nCntKey++)
			{
				g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// �v���C���[���[�V�����ǂݍ���
	LoadGiraffeMosion();
}

// ------------------------------------------------------------------------------------------
// �L�������f���̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadGiraffeModel(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/LOAD/GIRAFFE/giraffe_model.txt", "r");

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
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeModel[nCntModel].nIdxModelParent);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_giraffeModel[nCntModel].pos.x,
								&g_giraffeModel[nCntModel].pos.y, 
								&g_giraffeModel[nCntModel].pos.z);

							// �ʒu���ǂݎ��
							g_giraffeModel[nCntModel].posOrigin = g_giraffeModel[nCntModel].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffeModel[nCntModel].rot.x, 
								&g_giraffeModel[nCntModel].rot.y,
								&g_giraffeModel[nCntModel].rot.z);
						}
					}
					// ���f���̍X�V
					nCntModel++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �L�������̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadGiraffe(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntGiraffe = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �����\�����Ă�����̂���������
	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == true)
		{
			g_giraffe[nCntGiraffe].bUse = false;
			g_giraffe[nCntGiraffe].bDisp = false;
		}
	}

	// �J�E���g������
	nCntGiraffe = 0;

	// �t�@�C���J
	pFile = fopen(g_cGiraffeFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ʒu���ǂݍ���
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_giraffe[nCntGiraffe].pos.x,
								&g_giraffe[nCntGiraffe].pos.y,
								&g_giraffe[nCntGiraffe].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffe[nCntGiraffe].rot.x,
								&g_giraffe[nCntGiraffe].rot.y,
								&g_giraffe[nCntGiraffe].rot.z);

							// �g�p���
							g_giraffe[nCntGiraffe].bUse = true;

							// �\�����
							g_giraffe[nCntGiraffe].bDisp = true;

							// �e�̍쐬
							g_giraffe[nCntGiraffe].nIdxShadow = SetShadow(
								g_giraffe[nCntGiraffe].pos,
								g_giraffe[nCntGiraffe].rot,
								D3DXVECTOR3(20.0f, 0.0f, 20.0f));
						}
					}
					// ���f���̍X�V
					nCntGiraffe++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �L�������[�V�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadGiraffeMosion(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntMotion = 0;							// �v���C���[�J�E���g
	int	nCntModel = 0;							// ���f���J�E���g
	int	nCntKeySet = 0;							// ���[�V�����J�E���g
	int	nCntKey = 0;							// �L�[�J�E���g
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen(g_cGiraffeFIleName_3D[GetGameStage()], "r");

	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�L�X�g�ɕ�������
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

				// ���[�V�����Z�b�g��������
				else if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// ���f���̏�����
					nCntModel = 0;

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ���[�v���邩�ǂ������ǂݍ���
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeKey[nCntMotion].nLoop);
						}

						// �L�[�����ǂݍ���
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeKey[nCntMotion].nNumKey);
						}

						// �L�[���ǂݍ���
						else if (strcmp(cHeadText, "KEYSET") == 0)
						{
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �t���[�����ǂݍ���
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie, &g_giraffeKey[nCntMotion].KeyInfo[nCntModel].nFram);
							}

							// �L�[�̏�����
							nCntKey = 0;

							// �G���h�L�[�Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// �L�[���ǂݍ���
								if (strcmp(cHeadText, "KEY") == 0)
								{

									// �G���h�L�[������܂Ń��[�v
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);

										// �ʒu���ǂݍ���
										if (strcmp(cHeadText, "POS") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_giraffe[0].model[nCntKey].pos;
										}

										// ��]���ǂݍ���
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// �J�E���g�L�[
									nCntKey++;
								}
							}

							// �J�E���g���f��
							nCntModel++;
						}
					}

					// �J�E���g�v���C���[
					nCntMotion++;
				}
			}
		}

		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �Z�[�u��񏈗�
// ------------------------------------------------------------------------------------------
void SaveGiraffe(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int		nCntGiraffe = 0;
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

	// �L�����f�[�^�̎��
	char cGiraffeData[3][128] =
	{
		{ "POS" },				// �ʒu
		{ "ROT" },				// ��]
		{ "END_GIRAFFESET" }	// �G���h�t���A�[
	};

	// �Z�[�u�p //
	// �t�@�C���J
	pFile = fopen(g_cGiraffeFIleName_3D[GetGameStage()], "w");

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
				if (nCntGiraffe < ALL_MAX - 1)
				{
					if (g_giraffe[nCntGiraffe].bUse == true)
					{
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "	GIRAFFESET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_GIRAFFESET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cGiraffeData[nCntDataLoad]);

								// �ʒu���ۑ�
								if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n",
										&cWriteText, 
										&cEqual,
										g_giraffe[nCntGiraffe].pos.x,
										g_giraffe[nCntGiraffe].pos.y,
										g_giraffe[nCntGiraffe].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_giraffe[nCntGiraffe].rot.x,
										g_giraffe[nCntGiraffe].rot.y,
										g_giraffe[nCntGiraffe].rot.z);

									fputs(cHeadText, pFile);
									// �J�E���g�f�[�^�A�b�v
									nCntDataLoad++;
								}

								// �G���h����������
								else if (strcmp(cWriteText, "END_GIRAFFESET") == 0)
								{
									sprintf(cHeadText, "    %s", &cWriteText);

									fputs(cHeadText, pFile);

									// �J�E���g�f�[�^�A�b�v
									nCntDataLoad = 0;
								}
							}
						}
					}
					// �f�ރJ�E���g�ύX
					nCntGiraffe++;
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
		MessageBox(NULL, "�Z�[�u�p�L�����f�[�^�̏������݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);

		// �t�@�C����
		fclose(pFile);
	}

	// �J�E���g�̏�����
	nCntGiraffe = 0;
	nCntDataLoad = 0;
	nCntBug = 0;

	// ���[�h�p //
	// �t�@�C���J
	pFile = fopen("data/LOAD/GIRAFFE/giraffe.txt", "w");

	// �J����
	if (pFile != NULL)
	{
		//------------------ �R�����g ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# ���[�h�f��\n");

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
				if (nCntGiraffe < ALL_MAX - 1)
				{
					if (g_giraffe[nCntGiraffe].bUse == true)
					{
						fputs(cEnter, pFile);

						// �G���h�X�N���v�g�ł͂Ȃ�
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    GIRAFFESET\n");
							fputs(cWriteText, pFile);

							// �G���h���f���Z�b�g������܂Ń��[�v
							while (strcmp(cWriteText, "END_GIRAFFESET") != 0)
							{
								// ��������R�s�[����
								strcpy(cWriteText, cGiraffeData[nCntDataLoad]);

								// �ʒu���ۑ�
								if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_giraffe[nCntGiraffe].pos.x,
										g_giraffe[nCntGiraffe].pos.y,
										g_giraffe[nCntGiraffe].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_giraffe[nCntGiraffe].rot.x,
										g_giraffe[nCntGiraffe].rot.y,
										g_giraffe[nCntGiraffe].rot.z);

									fputs(cHeadText, pFile);

									// �J�E���g�f�[�^������
									nCntDataLoad++;
								}

								// ��]���ۑ�
								else if (strcmp(cWriteText, "END_GIRAFFESET") == 0)
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
					nCntGiraffe++;
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
		MessageBox(NULL, "���[�h�p�f�ރf�[�^�̏������݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);

		// �t�@�C����
		fclose(pFile);
	}
}

// ------------------------------------------------------------------------------------------
// ���f����񏈗�
// ------------------------------------------------------------------------------------------
GIRAFFE *GetGiraffe(void)
{
	return &g_giraffe[0];
}