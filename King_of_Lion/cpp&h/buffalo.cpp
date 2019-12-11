// ------------------------------------------------------------------------------------------
//
// �o�b�t�@���[���� [buffalo.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "buffalo.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "materials.h"
#include "trajectory.h"
#include "player.h"
#include "effect.h"
#include "item.h"
#include "game.h"
#include "sound.h"
#include "giraffe.h"
#include "score.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_BUFFALO (1)					// �e�N�X�`���[�̐�
#define BUFFALO_NORMALATTACK_DECISION (500)	// �U���͈�
#define BUFFALO_ADDMOVE (5.0f)				// �X�s�[�h
#define BUFFALO_HP (50)						// HP
#define BUFFALO_ATTACK (15)					// �U����
#define BUFFALO_FREQUENCY (100)				// �U���p�x

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
// ������
void Init_PartsBuffalo(void);					// �p�[�c
void Init_BuffaloInfo(void);					// �o�b�t�@���[
void Init_MotionBuffalo(void);					// ���[�V����
// �X�V
void Update_StateBuffalo(int nCntBuffalo);		// ���
void Update_RotBuffalo(int nCntBuffalo);		// ��]
void Update_MoveBuffalo(int nCntBuffalo);		// �ړ�
void Update_MosionBuffalo(int nCntBuffalo);		// ���[�V����
void Update_ASSAULT(int nCntBuffalo);			// �U��
void Update_DecisionBuffalo(int nCntBuffalo);	// �U�������蔻��
void Update_ColisionBuffalo(int nCntBuffalo);	// �����蔻��
// �ǂݎ��
void LoadBuffaloModel(void);					// ���f�����
void LoadBuffalo(void);							// �o�b�t�@���[���
void LoadBuffaloMosion(void);					// ���[�V�������

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureBuffalo[TEXTURE_BUFFALO] = {};	// �e�N�X�`���ւ̃|�C���^
BUFFALO				g_buffalo[MAX_BUFFALO];						// �o�b�t�@���[���
MODEL				g_buffaloModel[BUFFALOTYPE_MAX];			// �o�b�t�@���[���
MOSION_BUFFALO		g_buffaloKey[BUFFALOMOSIONSET_MAX];			// ���[�V�������
int					g_buffaloTotal;								// �o�b�t�@���[�̌��j���v
// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitBuffalo(void)
{
	// ���j��
	g_buffaloTotal = 0;
	// �o�b�t�@���[�p�[�c���
	Init_PartsBuffalo();
	// �o�b�t�@���[���
	Init_BuffaloInfo();
	// �o�b�t�@���[���[�V�������
	Init_MotionBuffalo();
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[�p�[�c��񏈗�
// ------------------------------------------------------------------------------------------
void Init_PartsBuffalo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	int		nCntModel;							// �o�b�t�@���[�̃J�E���g

	// �o�b�t�@���[�t�@�C������`
	const char *paBuffaloFile[BUFFALOTYPE_MAX] =
	{
		{ "data/MODEL/buffalo/00_Buffalo_body.x" },				// [0]��
		{ "data/MODEL/buffalo/01_Buffalo_head_joint.x" },		// [1]���̊֐�
		{ "data/MODEL/buffalo/02_Buffalo_head.x" },				// [2]��
		{ "data/MODEL/buffalo/03_Buffalo_LightHand_joint.x" },	// [3]�E��̊֐�
		{ "data/MODEL/buffalo/04_Buffalo_LightHand.x" },		// [4]�E��
		{ "data/MODEL/buffalo/05_Buffalo_LeftHand_joint.x" },	// [5]����̊֐�
		{ "data/MODEL/buffalo/06_Buffalo_LeftHand.x" },			// [6]����
		{ "data/MODEL/buffalo/07_Buffalo_LightLeg_joint.x" },	// [7]�E���̊֐�
		{ "data/MODEL/buffalo/08_Buffalo_LightLeg.x" },			// [8]�E��
		{ "data/MODEL/buffalo/09_Buffalo_LeftLeg_joint.x" },	// [9]�����̊֐�
		{ "data/MODEL/buffalo/10_Buffalo_LeftLeg.x" },			// [10]����
		{ "data/MODEL/buffalo/11_Buffalo_Tail_joint.x" },		// [11]�����ۂ̊֐�
		{ "data/MODEL/buffalo/12_Buffalo_Tail.x" }				// [12]������
	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_BUFFALO] =
	{
		{ "" },
	};

	// �p�[�c�̓ǂݍ��� //
	for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
	{
		g_buffaloModel[nCntModel].paBuffMat = NULL;
		g_buffaloModel[nCntModel].paMesh = NULL;
		g_buffaloModel[nCntModel].nNumMat = 0;
		g_buffaloModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffaloModel[nCntModel].nIdxModelParent = 0;
		g_buffaloModel[nCntModel].nFram = 0;
		g_buffaloModel[nCntModel].nCntKeySet = 0;

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			paBuffaloFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_buffaloModel[nCntModel].paBuffMat,
			NULL,
			&g_buffaloModel[nCntModel].nNumMat,
			&g_buffaloModel[nCntModel].paMesh);

		// �o�b�t�@���[�̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
		int nNumVertex;			// ���_��
		DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

								// ���_�����擾
		nNumVertex = g_buffaloModel[nCntModel].paMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_buffaloModel[nCntModel].paMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_buffaloModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// ���_�����J��Ԃ�
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// �Ԓn����D3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
			// x //
			// �ŏ��l
			if (g_buffaloModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_buffaloModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// �ő�l
			if (g_buffaloModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_buffaloModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// �ŏ��l
			if (g_buffaloModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_buffaloModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// �ő�l
			if (g_buffaloModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_buffaloModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// �ŏ��l
			if (g_buffaloModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_buffaloModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// �ő�l
			if (g_buffaloModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_buffaloModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// �T�C�Y�|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
		}

		// �A�����b�N
		g_buffaloModel[nCntModel].paMesh->UnlockVertexBuffer();

		// �ő�l�ƍŏ��l�̋���
		g_buffaloModel[nCntModel].size = g_buffaloModel[nCntModel].vtxMaxMaterials - g_buffaloModel[nCntModel].vtxMinMaterials;
	}

	// �p�[�c�Ǎ�
	LoadBuffaloModel();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureBuffalo[0]);
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[��񏈗�
// ------------------------------------------------------------------------------------------
void Init_BuffaloInfo(void)
{
	// �ϐ��錾
	int		nCntBuffalo;	// �v���C���[�J�E���g
	int		nCntModel;		// �o�b�t�@���[�̃J�E���g

	// �o�b�t�@���[�̏����ݒ� //
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_buffalo[nCntBuffalo].bUse == true)
			{
				DeleteShadow(g_buffalo[nCntBuffalo].nIdxShadow);
			}
		}

		g_buffalo[nCntBuffalo].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_buffalo[nCntBuffalo].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].posBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_buffalo[nCntBuffalo].nMotionType = 0;
		g_buffalo[nCntBuffalo].nMotionTypeOld = 0;
		g_buffalo[nCntBuffalo].nParent = 0;
		g_buffalo[nCntBuffalo].nIndex = nCntBuffalo;
		g_buffalo[nCntBuffalo].nFram = 0;
		g_buffalo[nCntBuffalo].nCntKeySet = 0;
		g_buffalo[nCntBuffalo].nIdxShadow = nCntBuffalo;
		g_buffalo[nCntBuffalo].fLength = 50.0f;
		g_buffalo[nCntBuffalo].bUse = false;
		g_buffalo[nCntBuffalo].bDisp = false;

		// �J���[���
		g_buffalo[nCntBuffalo].nCntCor = 0;
		g_buffalo[nCntBuffalo].diffuse = 1.0f;

		// �X�e�[�^�X���
		g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;
		g_buffalo[nCntBuffalo].status.nCntStatus = 0;
		g_buffalo[nCntBuffalo].status.nAttack = BUFFALO_ATTACK;
		g_buffalo[nCntBuffalo].status.nLife = BUFFALO_HP;
		g_buffalo[nCntBuffalo].status.bColition = false;
		g_buffalo[nCntBuffalo].status.bAttack = false;
		g_buffalo[nCntBuffalo].status.bRot = false;
		g_buffalo[nCntBuffalo].status.bMove = false;

		// �p�[�c���Ƃ̃J�E���g
		for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
		{
			g_buffalo[nCntBuffalo].model[nCntModel].pos = g_buffaloModel[nCntModel].pos;
			g_buffalo[nCntBuffalo].model[nCntModel].posLast = g_buffaloModel[nCntModel].posLast;
			g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween = g_buffaloModel[nCntModel].posKeyBetween;
			g_buffalo[nCntBuffalo].model[nCntModel].rot = g_buffaloModel[nCntModel].rot;
			g_buffalo[nCntBuffalo].model[nCntModel].rotLast = g_buffaloModel[nCntModel].rotLast;
			g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween = g_buffaloModel[nCntModel].rotKeyBetween;
			g_buffalo[nCntBuffalo].model[nCntModel].size = g_buffaloModel[nCntModel].size;
			g_buffalo[nCntBuffalo].model[nCntModel].vtxMinMaterials = g_buffaloModel[nCntModel].vtxMinMaterials;
			g_buffalo[nCntBuffalo].model[nCntModel].vtxMaxMaterials = g_buffaloModel[nCntModel].vtxMaxMaterials;
			g_buffalo[nCntBuffalo].model[nCntModel].nIdxModelParent = g_buffaloModel[nCntModel].nIdxModelParent;
			g_buffalo[nCntBuffalo].model[nCntModel].nFram = g_buffaloModel[nCntModel].nFram;
			g_buffalo[nCntBuffalo].model[nCntModel].nCntKeySet = g_buffaloModel[nCntModel].nCntKeySet;
		}
	}

	// ���[�h�o�b�t�@���[
	LoadBuffalo();
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[���[�V��������
// ------------------------------------------------------------------------------------------
void Init_MotionBuffalo(void)
{
	// �ϐ��錾
	int		nCntBuffalo;						// �o�b�t�@���[�J�E���g

												// ���[�V�����p�ϐ�
	int		nCntMotion;							// ���[�V�����J�E���g
	int		nCntKeySet;							// ���[�V�����J�E���g
	int		nCntKey;							// �L�[�J�E���g

												// ���[�V�����J�E���g //
	for (nCntMotion = 0; nCntMotion < BUFFALOMOSIONSET_MAX; nCntMotion++)
	{
		// ���[�V�����̏����ݒ�
		g_buffaloKey[nCntMotion].nNumKey = 0;		// �i���o�[�L�[
		g_buffaloKey[nCntMotion].nLoop = 0;		// ���[�v�̗L��

												// �����蔻��p //
		g_buffaloKey[nCntMotion].Collision_nParts = 0;		// �p�[�c�ԍ�
		g_buffaloKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
		g_buffaloKey[nCntMotion].Collision_Radius = 0.0f;	// �~�̔��a
		g_buffaloKey[nCntMotion].Collision_StartFram = 0;	// �J�n�t���[����
		g_buffaloKey[nCntMotion].Collision_EndFram = 0;		// �I���t���[����
		g_buffaloKey[nCntMotion].Collision_Damage = 0;		// �_���[�W��

															// �O�՗p //
		g_buffaloKey[nCntMotion].Orbit_nParts = 0;			// �p�[�c�ԍ�
		g_buffaloKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
		g_buffaloKey[nCntMotion].Orbit_Mesh = 0;			// �O�Ղ̐�
		g_buffaloKey[nCntMotion].Orbit_StartFram = 0;		// �J�n�t���[����
		g_buffaloKey[nCntMotion].Orbit_EndFram = 0;			// �I���t���[����
		g_buffaloKey[nCntMotion].Orbit_nType = 1;			// �O�Ղ̃^�C�v
		g_buffaloKey[nCntMotion].Orbit_nIdTrajectory = -1;	// �O�Ղ̔ԍ�
		g_buffaloKey[nCntMotion].Orbit_bUse = false;		// �O�Ղ̎g�p���

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_buffaloKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// �L�[���Ƃɏ�����
			for (nCntKey = 0; nCntKey < BUFFALOTYPE_MAX; nCntKey++)
			{
				g_buffaloKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_buffaloKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// �v���C���[���[�V�����ǂݍ���
	LoadBuffaloMosion();

	// �o�b�t�@���[�̏����ݒ� //
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < BUFFALOMOSIONSET_MAX; nCntMotion++)
			{
				// �g�p���
				if (g_buffaloKey[nCntMotion].Orbit_bUse == true)
				{
					// �O�Ղ̐ݒ�
					g_buffaloKey[nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
						g_buffaloKey[nCntMotion].Orbit_Ofset,
						g_buffaloKey[nCntMotion].Orbit_nParts,
						g_buffaloKey[nCntMotion].Orbit_Mesh,
						g_buffaloKey[nCntMotion].Orbit_StartFram,
						g_buffaloKey[nCntMotion].Orbit_EndFram,
						g_buffaloKey[nCntMotion].Orbit_nType);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitBuffalo(void)
{
	// �o�b�t�@���[�f�[�^�̊J��
	for (int nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
	{
		// ���b�V���̊J��
		if (g_buffaloModel[nCntModel].paMesh != NULL)
		{
			g_buffaloModel[nCntModel].paMesh->Release();
			g_buffaloModel[nCntModel].paMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_buffaloModel[nCntModel].paBuffMat != NULL)
		{
			g_buffaloModel[nCntModel].paBuffMat->Release();
			g_buffaloModel[nCntModel].paBuffMat = NULL;
		}
	}

	// �o�b�t�@���[�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < TEXTURE_BUFFALO; nCntTexture++)
	{
		// �e�N�X�`���[�̊J��
		if (g_paTextureBuffalo[nCntTexture] != NULL)
		{
			g_paTextureBuffalo[nCntTexture]->Release();
			g_paTextureBuffalo[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateBuffalo(void)
{
	// �J�E���g�o�b�t�@���[
	int nCntBuffalo;
	PLAYER *pPlayer = GetPlayer();

	// �o�b�t�@���[�̃J�E���g
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse)
		{
			// ���ݒn�ۑ�
			g_buffalo[nCntBuffalo].posold = g_buffalo[nCntBuffalo].pos;

			// ���[�V������ۑ�
			g_buffalo[nCntBuffalo].nMotionTypeOld = g_buffalo[nCntBuffalo].nMotionType;

			// ��ԏ���
			Update_StateBuffalo(nCntBuffalo);

			// ����ł��Ȃ����
			if (g_buffalo[nCntBuffalo].status.nStatus != BUFFALOSTATUS_DIE)
			{
				// �U����ԂłȂ��Ƃ�
				if (g_buffalo[nCntBuffalo].status.bAttack == false)
				{
					// �ړ���Ԃł͂Ȃ��Ƃ�
					if (g_buffalo[nCntBuffalo].status.bMove == false)
					{
						// ��]����
						Update_RotBuffalo(nCntBuffalo);
					}

					// �ړ���Ԃł͂Ȃ��Ƃ�
					if (g_buffalo[nCntBuffalo].status.bRot == false)
					{
						// �ړ�����
						Update_MoveBuffalo(nCntBuffalo);
					}

				}

				// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
				if (g_buffalo[nCntBuffalo].status.bMove == false)
				{
					// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
					if (g_buffalo[nCntBuffalo].status.bRot == false)
					{
						// ���ʍU���̏���
						Update_ASSAULT(nCntBuffalo);
					}
				}

				// �U���̓����蔻��
				Update_DecisionBuffalo(nCntBuffalo);
			}
			g_buffalo[nCntBuffalo].move.y -= 0.5f;
			g_buffalo[nCntBuffalo].pos.y += g_buffalo[nCntBuffalo].move.y;

			// �����蔻��
			Update_ColisionBuffalo(nCntBuffalo);

			// ����ł��Ȃ����
			if (g_buffalo[nCntBuffalo].status.nStatus != BUFFALOSTATUS_DIE)
			{
				// ���[�V��������
				Update_MosionBuffalo(nCntBuffalo);
			}
			// �e�̈ʒu��ݒ�
			SetPositionShadow(
				g_buffalo[nCntBuffalo].nIdxShadow,
				g_buffalo[nCntBuffalo].pos +
				D3DXVECTOR3(
					sinf(-g_buffalo[nCntBuffalo].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_buffalo[nCntBuffalo].rot.y) * 10.0f));

			// �U�����������Ă���
			if (g_buffalo[nCntBuffalo].status.bColition == true)
			{
				// �U�����I�������
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_buffalo[nCntBuffalo].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ��ԏ���
// ------------------------------------------------------------------------------------------
void Update_StateBuffalo(int nCntBuffalo)
{
	switch (g_buffalo[nCntBuffalo].status.nStatus)
	{
	case BUFFALOSTATUS_NEUTRAL:
		g_buffalo[nCntBuffalo].diffuse = 1.0f;
		break;

	case BUFFALOSTATUS_DAMAGE:
		// �J���[�̏�ԃJ�E���g�A�b�v
		g_buffalo[nCntBuffalo].nCntCor++;

		// �\�����
		if (g_buffalo[nCntBuffalo].nCntCor % 10 == 0)
		{
			g_buffalo[nCntBuffalo].diffuse = 1.0f;
		}

		// ��\�����
		else if (g_buffalo[nCntBuffalo].nCntCor % 5 == 0)
		{
			g_buffalo[nCntBuffalo].diffuse = 0.5f;
		}

		if (g_buffalo[nCntBuffalo].nCntCor == 50)
		{
			// �J���[�̏�ԃJ�E���g������
			g_buffalo[nCntBuffalo].nCntCor = 0;

			// �o�b�t�@���[�̏�Ԃ��m�[�}����
			g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;
		}

		break;

	case BUFFALOSTATUS_DIE:
		// �ڕW��]�ʂ��猻�݂̉�]�ʂ̍���
		g_buffalo[nCntBuffalo].rotbetween.z =
			(g_buffalo[nCntBuffalo].rotLast.z - g_buffalo[nCntBuffalo].rot.z) / 10;

		// ��]�̌��E�v�Z
		g_buffalo[nCntBuffalo].rotbetween.z =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rotbetween.z);

		// ��]
		g_buffalo[nCntBuffalo].rot.z += g_buffalo[nCntBuffalo].rotbetween.z;


		g_buffalo[nCntBuffalo].rot.z =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rot.z);

		// �d��
		g_buffalo[nCntBuffalo].move.y -= 0.5f;	// �ړ���
		g_buffalo[nCntBuffalo].pos.y += g_buffalo[nCntBuffalo].move.y;

		// �X�e�[�^�X�̃J�E���g�A�b�v
		g_buffalo[nCntBuffalo].status.nCntStatus++;

		// �X�e�[�^�X�̃J�E���g���K��l�ȏ�ɂȂ�����
		if (g_buffalo[nCntBuffalo].status.nCntStatus == 70)
		{
			if (GetMode() == MODE_TUTORIAL)
			{
				// �o�b�t�@���[�o��
				SetGiraffe(
					g_buffalo[nCntBuffalo].pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}

			// �G�t�F�N�g�p�ϐ�
			float fAngle;
			float fYAngle;
			D3DXVECTOR3 move;
			D3DXCOLOR col;

			for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
			{
				// X�EZ�̊p�x
				fAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

				// Y�̊p�x
				fYAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

				// �ړ�����
				move =
					D3DXVECTOR3(sinf(fAngle) * 5.0f,
						cosf(fAngle) * 5.0f,
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
						g_buffalo[nCntBuffalo].pos.x,
						g_buffalo[nCntBuffalo].pos.y,
						g_buffalo[nCntBuffalo].pos.z),

					move,															// �ړ���
					col,															// �F
					float(rand() % 10),												// �T�C�Y
					-0,																// �����T�C�Y
					0.0f,															// �d��
					10,																// �G�t�F�N�g���C�t
					0,																// �^�C�v
					0);
			}

			// �g�p�E�\�����OFF
			g_buffalo[nCntBuffalo].bUse = false;
			g_buffalo[nCntBuffalo].bDisp = false;

			// �e�̏���
			DeleteShadow(g_buffalo[nCntBuffalo].nIdxShadow);

			// �g�p���̋O�Ղ̏���
			DeleteTrajectory(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Orbit_nIdTrajectory);

			// �X�e�[�^�X�̃J�E���g�̏�����
			g_buffalo[nCntBuffalo].status.nCntStatus = 0;

			// �X�e�[�^�X��
			g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;

			// �A�C�e���o��
			SetItem(
				g_buffalo[nCntBuffalo].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ��]
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// �ړ���
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// �T�C�Y
				20.0f,								// ���a
				0,									// �e�N�X�`���[
				100);								// �o������

			// ���j��
			g_buffaloTotal++;

			// �Q�[��������
			if (GetMode() == MODE_GAME)
			{
				AddScore(1);
			}
		}
		break;
	default:
		break;
	}

}

// ------------------------------------------------------------------------------------------
// ��]����
// ------------------------------------------------------------------------------------------
void Update_RotBuffalo(int nCntBuffalo)
{
	// �����]���ԍ�
	int nRandNum = 0;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_buffalo[nCntBuffalo].status.bRot == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 500;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �ړI�̉�]��
			g_buffalo[nCntBuffalo].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			g_buffalo[nCntBuffalo].rotLast.y =
				Rot_One_Limit(g_buffalo[nCntBuffalo].rotLast.y);

			// ��]���
			g_buffalo[nCntBuffalo].status.bRot = true;
		}
	}

	// �ړI��]�ʂ��猻�݉�]�ʂ̍���
	g_buffalo[nCntBuffalo].rotbetween.y = (g_buffalo[nCntBuffalo].rotLast.y - g_buffalo[nCntBuffalo].rot.y) * 0.01f;

	g_buffalo[nCntBuffalo].rotbetween.y =
		Rot_One_Limit(g_buffalo[nCntBuffalo].rotbetween.y);

	g_buffalo[nCntBuffalo].rot.y += g_buffalo[nCntBuffalo].rotbetween.y;

	g_buffalo[nCntBuffalo].rot.y =
		Rot_One_Limit(g_buffalo[nCntBuffalo].rot.y);

	// ��]��ԏI��
	if (g_buffalo[nCntBuffalo].rot.y >= -0.1f ||
		g_buffalo[nCntBuffalo].rot.y <= 0.1f)
	{
		g_buffalo[nCntBuffalo].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void Update_MoveBuffalo(int nCntBuffalo)
{
	// �����]���ԍ�
	int nRandNum = 0;
	int nRotNum = 0;
	float fRandMove;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_buffalo[nCntBuffalo].status.bMove == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 200;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �����_���ňړ��ʂ����߂�
			fRandMove = float(rand() % 100);

			// �ړI��]��
			g_buffalo[nCntBuffalo].posLast.x =
				sinf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * fRandMove + g_buffalo[nCntBuffalo].pos.x;

			g_buffalo[nCntBuffalo].posLast.z =
				cosf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * fRandMove + g_buffalo[nCntBuffalo].pos.z;

			// �ړ����
			g_buffalo[nCntBuffalo].status.bMove = true;
		}
	}

	// �ړI��]�ʂ��猻�݉�]�ʂ̍���
	g_buffalo[nCntBuffalo].move.x = (g_buffalo[nCntBuffalo].posLast.x - g_buffalo[nCntBuffalo].pos.x) * 0.05f;
	g_buffalo[nCntBuffalo].move.z = (g_buffalo[nCntBuffalo].posLast.z - g_buffalo[nCntBuffalo].pos.z) * 0.05f;

	// �ʒu���̍X�V
	g_buffalo[nCntBuffalo].pos += g_buffalo[nCntBuffalo].move;

	// �ړ���ԏI��
	if (g_buffalo[nCntBuffalo].move.x >= -0.1f && g_buffalo[nCntBuffalo].move.x <= 0.1f &&
		g_buffalo[nCntBuffalo].move.z >= -0.1f && g_buffalo[nCntBuffalo].move.z <= 0.1f)
	{
		g_buffalo[nCntBuffalo].status.bMove = false;

		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
	}

	// �ړ����
	if (g_buffalo[nCntBuffalo].status.bMove == true)
	{
		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// ���[�V��������
// ------------------------------------------------------------------------------------------
void Update_MosionBuffalo(int nCntBuffalo)
{
	// �ϐ��錾
	int	nCntModel;			// �J�E���g�o�b�t�@���[

	// ���݂̃��[�V�����ƑO��̃��[�V�������r
	if (g_buffalo[nCntBuffalo].nMotionType != g_buffalo[nCntBuffalo].nMotionTypeOld)
	{
 		g_buffalo[nCntBuffalo].nFram = 0;
		g_buffalo[nCntBuffalo].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
	{
		// �t���[�������O�̏ꍇ
		if (g_buffalo[nCntBuffalo].nFram == 0)
		{
			if (g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram != 0)
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween =
					(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].rot -
						g_buffalo[nCntBuffalo].model[nCntModel].rot) /
						(float)g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram;

				// ��]�̌��E�v�Z
				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.x =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.x);

				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.y =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.y);

				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.z =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.z);

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween =
					(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].pos -
						g_buffalo[nCntBuffalo].model[nCntModel].pos) /
						(float)g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram;
			}

			else
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_buffalo[nCntBuffalo].model[nCntModel].rot =
					g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].rot;

				// ��]�̌��E�v�Z
				g_buffalo[nCntBuffalo].model[nCntModel].rot.x =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.x);

				g_buffalo[nCntBuffalo].model[nCntModel].rot.y =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.y);

				g_buffalo[nCntBuffalo].model[nCntModel].rot.z =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.z);

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_buffalo[nCntBuffalo].model[nCntModel].pos =
					g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].pos;

				// �����̏�����
				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �����̏�����
				g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}
		// ��]����
		g_buffalo[nCntBuffalo].model[nCntModel].rot +=
			g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween;

		// ��]�̌��E�v�Z
		g_buffalo[nCntBuffalo].model[nCntModel].rot.x =
			Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.x);

		g_buffalo[nCntBuffalo].model[nCntModel].rot.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.y);

		g_buffalo[nCntBuffalo].model[nCntModel].rot.z =
			Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.z);


		// �ړ�����
		g_buffalo[nCntBuffalo].model[nCntModel].pos +=
			g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween;
	}

	// �t���[�������K��l�Ɠ����ɂȂ�����
	if (g_buffalo[nCntBuffalo].nFram ==
		g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram)
	{
		// �L�[�Z�b�g�̃J�E���g�A�b�v
		g_buffalo[nCntBuffalo].nCntKeySet++;

		// �t���[����������
		g_buffalo[nCntBuffalo].nFram = 0;

		// �L�[�Z�b�g�����K��l�Ɠ����ɂȂ�����
		if (g_buffalo[nCntBuffalo].nCntKeySet == g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].nLoop == 0)
			{
				// �L�[�Z�b�g��������
				g_buffalo[nCntBuffalo].nCntKeySet = 0;

				// ���[�V�����؂�ւ�
				// �ːi�ҋ@��
				if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT_WAIT)
				{
					g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_ASSAULT;

					// �؂�ւ��J�E���g�̏�����
					g_buffalo[nCntBuffalo].nCntMotion = 0;

					for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
					{
						// ��]�ʂ̏�����
						g_buffalo[nCntBuffalo].model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_buffalo[nCntBuffalo].model[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}

				// ����ȊO
				else
				{
					g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
				}
			}
			// ���[�v���鎞
			else
			{
				// �L�[�Z�b�g��������
				g_buffalo[nCntBuffalo].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// �t���[�����J�E���g�A�b�v
		g_buffalo[nCntBuffalo].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// �ːi�U���̏���
// ------------------------------------------------------------------------------------------
void Update_ASSAULT(int nCntBuffalo)
{
	int	nAttackNum;	// �U���ԍ�
	D3DXVECTOR3 diffpos;						// ����
	D3DXVECTOR3 pPlayerPos = GetPlayer_Pos();	// �v���C���[���擾

	// ����
	diffpos = pPlayerPos - g_buffalo[nCntBuffalo].pos;

	// �U���s���͈�
	if ((BUFFALO_NORMALATTACK_DECISION * BUFFALO_NORMALATTACK_DECISION) >=	// �U���s���̉~�͈�
		diffpos.x * diffpos.x +												// x���̉~
		diffpos.y * diffpos.y +												// y���̉~
		diffpos.z * diffpos.z)												// z���̉~
	{
		// �U���̊m��
		nAttackNum = rand() % BUFFALO_FREQUENCY;

		// �U��
		if (nAttackNum == 0 &&
			g_buffalo[nCntBuffalo].status.bAttack == false)
		{
			// �G�t�F�N�g����


			// ���ʍU�����[�V����
			g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_ASSAULT_WAIT;

			// �U�����ON
			g_buffalo[nCntBuffalo].status.bAttack = true;
		}
	}

	// �ːi�ҋ@���[�V������������
	if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT_WAIT)
	{
		// �o�b�t�@���[���猩�ăv���C���[������p�x
		float	fAngle = atan2f(diffpos.x, diffpos.z);

		// �ړI�̉�]��
		g_buffalo[nCntBuffalo].rotLast.y = fAngle - D3DX_PI;

		g_buffalo[nCntBuffalo].rotLast.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rotLast.y);

		// �ړI��]�ʂ��猻�݉�]�ʂ̍���
		g_buffalo[nCntBuffalo].rotbetween.y = 
			(g_buffalo[nCntBuffalo].rotLast.y - g_buffalo[nCntBuffalo].rot.y);

		g_buffalo[nCntBuffalo].rotbetween.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rotbetween.y);

		g_buffalo[nCntBuffalo].rot.y += g_buffalo[nCntBuffalo].rotbetween.y * 0.1f;
		
		g_buffalo[nCntBuffalo].rot.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rot.y);
	}

	// �ːi���[�V������������
	if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT)
	{
		// �ړI�ړ���
		g_buffalo[nCntBuffalo].move.x =
			sinf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * BUFFALO_ADDMOVE;

		g_buffalo[nCntBuffalo].move.z =
			cosf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * BUFFALO_ADDMOVE;

		
		// �ʒu���̍X�V
		g_buffalo[nCntBuffalo].pos.x += g_buffalo[nCntBuffalo].move.x;

		// �ʒu���̍X�V
		g_buffalo[nCntBuffalo].pos.z += g_buffalo[nCntBuffalo].move.z;

		// �ړI�ʒu���̍X�V
		g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

		// �؂�ւ��J�E���g�A�b�v
		g_buffalo[nCntBuffalo].nCntMotion++;

		// �T�b��������؂�ւ�
		if (g_buffalo[nCntBuffalo].nCntMotion == 300)
		{
			// ���[�V���������R�֖̂߂�
			g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;

			// �؂�ւ��J�E���g�̏�����
 			g_buffalo[nCntBuffalo].nCntMotion = 0;
		}
	}

	// ���[�V�������I�������
	if (g_buffalo[nCntBuffalo].nMotionType != BUFFALOMOSIONSET_ASSAULT_WAIT &&
		g_buffalo[nCntBuffalo].nMotionType != BUFFALOMOSIONSET_ASSAULT)
	{
		// �U�����OFF
		g_buffalo[nCntBuffalo].status.bAttack = false;
	}
}

// ------------------------------------------------------------------------------------------
// �U���̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_DecisionBuffalo(int nCntBuffalo)
{
	// �v���C���[���擾
	PLAYER * pPlayer = GetPlayer();

	// �G�������Ă��邩�ǂ���
	if (g_buffalo[nCntBuffalo].status.nStatus != BUFFALOSTATUS_DIE)
	{
		// �U����Ԃ��ǂ���
		if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT)
		{
			// �v���C���[�̍U�����󂯂Ă��邩�ǂ���
			if (pPlayer->status.bColition == false)
			{
				// �v���C���[�̍U�����̃o�b�t�@���[�ƃv���C���[�̓����蔻��
				if ((pPlayer->status.fLength * pPlayer->status.fLength) +
					(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_Radius *
					g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_Radius) >=
					(pPlayer->pos.x - g_buffalo[nCntBuffalo].model[g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_nParts].mtxWorldModel._41) *
					(pPlayer->pos.x - g_buffalo[nCntBuffalo].model[g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_nParts].mtxWorldModel._41) +
					(pPlayer->pos.y - g_buffalo[nCntBuffalo].model[g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_nParts].mtxWorldModel._42) *
					(pPlayer->pos.y - g_buffalo[nCntBuffalo].model[g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_nParts].mtxWorldModel._42) +
					(pPlayer->pos.z - g_buffalo[nCntBuffalo].model[g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_nParts].mtxWorldModel._43) *
					(pPlayer->pos.z - g_buffalo[nCntBuffalo].model[g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_nParts].mtxWorldModel._43))
				{
					// �o�b�t�@���[�̃_���[�W����
					HitPlayer(
						g_buffalo[nCntBuffalo].status.nAttack +
						g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionBuffalo(int nCntBuffalo)
{
	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_buffalo[nCntBuffalo].pos,
		&g_buffalo[nCntBuffalo].posold,
		&g_buffalo[nCntBuffalo].move,
		&g_buffalo[nCntBuffalo].model[0].size,
		&g_buffalo[nCntBuffalo].pMeshField))
	{

	}

	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_buffalo[nCntBuffalo].pos,
		&g_buffalo[nCntBuffalo].posold,
		&g_buffalo[nCntBuffalo].move,
		&g_buffalo[nCntBuffalo].model[0].size,
		&g_buffalo[nCntBuffalo].pMaterials) != -1)
	{
		// �ڕW�n�_�����ݒn�X��
		g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

		// ���[�V���������R�֖̂߂�
		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_buffalo[nCntBuffalo].pos,
		&g_buffalo[nCntBuffalo].posold,
		&g_buffalo[nCntBuffalo].move,
		&g_buffalo[nCntBuffalo].model[0].size,
		&g_buffalo[nCntBuffalo].pWall))
	{
		// �ڕW�n�_�����ݒn�X��
		g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

		// ���[�V���������R�֖̂߂�
		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawBuffalo(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;				// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;				// �}�e���A���f�[�^�̃|�C���^
	int nCntBuffalo;
	int nCntModel;
	int nCnt = 0;

	// �J�E���g�o�b�t�@���[
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_buffalo[nCntBuffalo].mtxWorldBuffalo);

			// ��]�𔽉f
			// �X�P�[���𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_buffalo[nCntBuffalo].rot.y,
				g_buffalo[nCntBuffalo].rot.x,
				g_buffalo[nCntBuffalo].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].mtxWorldBuffalo,
				&g_buffalo[nCntBuffalo].mtxWorldBuffalo, &mtxRot);


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_buffalo[nCntBuffalo].pos.x,
				g_buffalo[nCntBuffalo].pos.y,
				g_buffalo[nCntBuffalo].pos.z);	// ���ꂼ��̈ړ���

												// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].mtxWorldBuffalo,	// 1
				&g_buffalo[nCntBuffalo].mtxWorldBuffalo,					// 2
				&mtxTrans);									// 3

															// �J�E���g�o�b�t�@���[
			for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_buffalo[nCntBuffalo].model[nCntModel].rot.y,
					g_buffalo[nCntBuffalo].model[nCntModel].rot.x,
					g_buffalo[nCntBuffalo].model[nCntModel].rot.z);

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
				D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
					&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel, &mtxRot);

				// �ʒu�𔽉f //
				// ���s�ړ��s��쐬(�I�t�Z�b�g)
				D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
					g_buffalo[nCntBuffalo].model[nCntModel].pos.x,
					g_buffalo[nCntBuffalo].model[nCntModel].pos.y,
					g_buffalo[nCntBuffalo].model[nCntModel].pos.z);	// ���ꂼ��̈ړ���

																	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
				D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,	// 1
					&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


																// [���ׂĂ̐e]����Ȃ��ꍇ
				if (g_buffalo[nCntBuffalo].model[nCntModel].nIdxModelParent != -1)
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].model[g_buffalo[nCntBuffalo].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].mtxWorldBuffalo);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)g_buffaloModel[nCntModel].paBuffMat->GetBufferPointer();

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_buffaloModel[nCntModel].nNumMat; nCntMat++, pMat++, nCnt++)
				{
					// �{�̂̃J���[���X�V
					pMat->MatD3D.Diffuse.a = g_buffalo[nCntBuffalo].diffuse;

					// �e�N�X�`���[����
					if (pMat->pTextureFilename != NULL)
					{
						// �e�N�X�`���[�ݒ�
						pDevice->SetTexture(0, g_paTextureBuffalo[0]);
					}

					// �e�N�X�`���[�Ȃ�
					else
					{
						// �e�N�X�`���[�ݒ�
						pDevice->SetTexture(0, NULL);
					}

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat->MatD3D);

					// �`��
					g_buffaloModel[nCntModel].paMesh->DrawSubset(nCntMat);
				}

				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �U�����󂯂��Ƃ��̏���
// ------------------------------------------------------------------------------------------
void HitBuffalo(int nCntBuffalo, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �U�����������Ă���
	g_buffalo[nCntBuffalo].status.bColition = true;

	// �J���[�̏�ԃJ�E���g������
	g_buffalo[nCntBuffalo].nCntCor = 0;

	// HP������
	g_buffalo[nCntBuffalo].status.nLife -= nDamage;
	
	PlaySound(SOUND_LABEL_SE_COLISION);

	// HP��0�ɂȂ�����
	if (g_buffalo[nCntBuffalo].status.nLife <= 0)
	{
		// ���S���
		g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_DIE;

		// �ڕW��]��
		g_buffalo[nCntBuffalo].rotLast.z = D3DX_PI / 2;

		// �ړ���
		g_buffalo[nCntBuffalo].move.y = 5.0f;

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
					g_buffalo[nCntBuffalo].pos.x,
					g_buffalo[nCntBuffalo].pos.y,
					g_buffalo[nCntBuffalo].pos.z),

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

	else
	{
		// �_���[�W���
		g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_DAMAGE;

		for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
		{
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
					g_buffalo[nCntBuffalo].pos.x,
					g_buffalo[nCntBuffalo].pos.y,
					g_buffalo[nCntBuffalo].pos.z),
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
// �o�b�t�@���[�̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void ColisionBuffalo(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[�ݒ菈��
// ------------------------------------------------------------------------------------------
void SetBuffalo(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �J�E���g�o�b�t�@���[
	for (int nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse == false)
		{
			// �ʒu���
			g_buffalo[nCntBuffalo].pos = pos;


			// �ړI�ʒu�ɕۑ�
			g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

			// ��]���
			g_buffalo[nCntBuffalo].rot = rot;

			// �g�p���
			g_buffalo[nCntBuffalo].bUse = true;

			// �\�����
			g_buffalo[nCntBuffalo].bDisp = true;

			// �e�̍쐬
			g_buffalo[nCntBuffalo].nIdxShadow = SetShadow(
				g_buffalo[nCntBuffalo].pos,
				D3DXVECTOR3(20.0f, 0.0f, 20.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �X�e�[�^�X���
			g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;
			g_buffalo[nCntBuffalo].status.nCntStatus = 0;
			g_buffalo[nCntBuffalo].status.nAttack = BUFFALO_ATTACK;
			g_buffalo[nCntBuffalo].status.nLife = BUFFALO_HP;
			g_buffalo[nCntBuffalo].status.bColition = false;
			g_buffalo[nCntBuffalo].status.bAttack = false;
			g_buffalo[nCntBuffalo].status.bRot = false;
			g_buffalo[nCntBuffalo].status.bMove = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[�o�b�t�@���[�̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadBuffaloModel(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/BUFFALO/buffalo_model.txt", "r");

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

				// �o�b�t�@���[�Z�b�g��������
				else if (strcmp(cHeadText, "PARTSSET") == 0)
				{

					// �G���h�o�b�t�@���[�Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_PARTSSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_buffaloModel[nCntModel].nIdxModelParent);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_buffaloModel[nCntModel].pos.x,
								&g_buffaloModel[nCntModel].pos.y,
								&g_buffaloModel[nCntModel].pos.z);

							// �ʒu���ۑ�
							g_buffaloModel[nCntModel].posOrigin = g_buffaloModel[nCntModel].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_buffaloModel[nCntModel].rot.x,
								&g_buffaloModel[nCntModel].rot.y,
								&g_buffaloModel[nCntModel].rot.z);
						}
					}
					// �o�b�t�@���[�̍X�V
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
		MessageBox(NULL, "�o�b�t�@���[�̃p�[�c�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[���̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadBuffalo(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntBuffalo = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �{�X�p
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/BUFFALO/buffalo.txt", "r");
	}

	// �`���[�g���A��
	else if (GetMode() == MODE_TUTORIAL)
	{
		return;
	}

	// ����ȊO
	else
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/BUFFALO/buffalo.txt", "r");
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

				// �o�b�t�@���[�Z�b�g��������
				else if (strcmp(cHeadText, "BUFFALOSET") == 0)
				{

					// �G���h�o�b�t�@���[�Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_BUFFALOSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ʒu���ǂݍ���
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_buffalo[nCntBuffalo].pos.x,
								&g_buffalo[nCntBuffalo].pos.y,
								&g_buffalo[nCntBuffalo].pos.z);

							// �ړI�ʒu�ɕۑ�
							g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_buffalo[nCntBuffalo].rot.x,
								&g_buffalo[nCntBuffalo].rot.y,
								&g_buffalo[nCntBuffalo].rot.z);

							// �g�p���
							g_buffalo[nCntBuffalo].bUse = true;

							// �\�����
							g_buffalo[nCntBuffalo].bDisp = true;

							// �e�̍쐬
							g_buffalo[nCntBuffalo].nIdxShadow = SetShadow(
								g_buffalo[nCntBuffalo].pos,
								D3DXVECTOR3(20.0f, 0.0f, 20.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}
					// �o�b�t�@���[�̍X�V
					nCntBuffalo++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�o�b�t�@���[�̃f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[���[�V�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadBuffaloMosion(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntMotion = 0;							// �v���C���[�J�E���g
	int	nCntModel = 0;							// �o�b�t�@���[�J�E���g
	int	nCntKeySet = 0;							// ���[�V�����J�E���g
	int	nCntKey = 0;							// �L�[�J�E���g
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

							// �t�@�C���J
	pFile = fopen("data/SAVE/BUFFALO/buffalo_motion.txt", "r");

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
					// �o�b�t�@���[�̏�����
					nCntModel = 0;

					// �G���h�o�b�t�@���[�Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ���[�v���邩�ǂ������ǂݍ���
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_buffaloKey[nCntMotion].nLoop);
						}

						// �L�[�����ǂݍ���
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_buffaloKey[nCntMotion].nNumKey);
						}

						// �����蔻����ǂݍ���
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie,
								&g_buffaloKey[nCntMotion].Collision_nParts,
								&g_buffaloKey[nCntMotion].Collision_Ofset.x,
								&g_buffaloKey[nCntMotion].Collision_Ofset.y,
								&g_buffaloKey[nCntMotion].Collision_Ofset.z,
								&g_buffaloKey[nCntMotion].Collision_Radius,
								&g_buffaloKey[nCntMotion].Collision_StartFram,
								&g_buffaloKey[nCntMotion].Collision_EndFram);
						}

						// �_���[�W���ǂݍ���
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_buffaloKey[nCntMotion].Collision_Damage);
						}

						// �O�Տ��ǂݍ���
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&g_buffaloKey[nCntMotion].Orbit_nParts,
								&g_buffaloKey[nCntMotion].Orbit_Ofset.x,
								&g_buffaloKey[nCntMotion].Orbit_Ofset.y,
								&g_buffaloKey[nCntMotion].Orbit_Ofset.z,
								&g_buffaloKey[nCntMotion].Orbit_Mesh,
								&g_buffaloKey[nCntMotion].Orbit_StartFram,
								&g_buffaloKey[nCntMotion].Orbit_EndFram);

							// �L�����N�^�[�^�C�v
							g_buffaloKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_BUFFALO;

							// �g�p���
							g_buffaloKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_buffaloKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_buffalo[0].model[nCntKey].pos;
										}

										// ��]���ǂݍ���
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// �J�E���g�L�[
									nCntKey++;
								}
							}

							// �J�E���g�o�b�t�@���[
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
		MessageBox(NULL, "�o�b�t�@���[�̃��[�V�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[��񏈗�
// ------------------------------------------------------------------------------------------
BUFFALO *GetBuffalo(void)
{
	return &g_buffalo[0];
}

// ------------------------------------------------------------------------------------------
// �o�b�t�@���[���j������
// ------------------------------------------------------------------------------------------
int GetBuffaloTotal(void)
{
	return g_buffaloTotal;
}
