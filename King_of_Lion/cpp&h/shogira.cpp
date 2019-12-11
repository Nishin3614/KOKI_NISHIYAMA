// ------------------------------------------------------------------------------------------
//
// �e�L�������� [shogira.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "shogira.h"
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
#include "Billboard.h"
#include "game.h"
#include "sound.h"
#include "buffalo.h"
#include "score.h"
#include "explosion.h"
#include "prediction.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_SHOGIRA (1)
#define SHOGIRA_HP (30)
#define SHOGIRA_ATTACK (10)					// �U����
#define SHOGIRA_FREQUENCY (100)				// �U���p�x
#define SHOGIRA_NORMALATTACK_DECISION (100)
#define SHOGIRA_SHOT_DECISION (1000)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Init_PartsShogira(void);
void Init_ShogiraInfo(void);
void Init_MotionShogira(void);
void Update_StateShogira(int nCntShogira);
void Update_RotShogira(int nCntShogira);
void Update_MosionShogira(int nCntShogira);
void Update_ShotShogira(int nCntShogira);
void Update_DecisionShogira(int nCntShogira);
void Update_ColisionShogira(int nCntShogira);
void LoadShogiraModel(void);
void LoadShogira(void);
void LoadShogiraMosion(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureShogira[TEXTURE_SHOGIRA] = {};	// �e�N�X�`���ւ̃|�C���^
SHOGIRA				g_shogira[MAX_SHOGIRA];						// �L�������
MODEL				g_shogiraModel[SHOGIRATYPE_MAX];			// �e�L�������
MOSION_SHOGIRA		g_shogiraKey[SHOGIRAMOSIONSET_MAX];			// ���[�V�������
int					g_shogiraTotal;								// �L�������j�����v

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitShogira(void)
{
	g_shogiraTotal = 0;
	// �L�����p�[�c���
	Init_PartsShogira();

	// �L�������
	Init_ShogiraInfo();

	// �L�������[�V�������
	Init_MotionShogira();
}

// ------------------------------------------------------------------------------------------
// �L�����p�[�c��񏈗�
// ------------------------------------------------------------------------------------------
void Init_PartsShogira(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	int		nCntModel;							// �e�L�����̃J�E���g

												// �e�L�����t�@�C������`
	char *paShogiraFile[SHOGIRATYPE_MAX] =
	{
		{ "data/MODEL/giraffe/00_Giraffe_body.x" },				// [0]��
		{ "data/MODEL/giraffe/01_Giraffe_JointRoot_Neck.x" },	// [1]��̍����̊֐�
		{ "data/MODEL/giraffe/02_Giraffe_Neck.x" },				// [2]��
		{ "data/MODEL/giraffe/03_Giraffe_JointTip_Neck.x" },	// [3]��̐�[�̊֐�
		{ "data/MODEL/giraffe/04_Giraffe_Head.x" },				// [4]��
		{ "data/MODEL/giraffe/05_Giraffe_RightHand.x" },		// [5]�E��
		{ "data/MODEL/giraffe/06_Giraffe_LeftHand.x" },			// [6]����
		{ "data/MODEL/giraffe/07_Giraffe_RightLeg.x" },			// [7]�E��
		{ "data/MODEL/giraffe/08_Giraffe_LeftLeg.x" },			// [8]����
		{ "data/MODEL/giraffe/09_Giraffe_Joint_Tail.x" },		// [9]�����ۂ̊֐�
		{ "data/MODEL/giraffe/10_Giraffe_Tail.x" }				// [10]������
	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_SHOGIRA] =
	{
		{ "data/TEXTURE/giraffe/00_giraffe_Texture.png" },
	};

	// �p�[�c�̓ǂݍ��� //
	for (nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
	{
		g_shogiraModel[nCntModel].paBuffMat = NULL;
		g_shogiraModel[nCntModel].paMesh = NULL;
		g_shogiraModel[nCntModel].nNumMat = 0;
		g_shogiraModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogiraModel[nCntModel].nIdxModelParent = 0;
		g_shogiraModel[nCntModel].nFram = 0;
		g_shogiraModel[nCntModel].nCntKeySet = 0;

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			paShogiraFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_shogiraModel[nCntModel].paBuffMat,
			NULL,
			&g_shogiraModel[nCntModel].nNumMat,
			&g_shogiraModel[nCntModel].paMesh);

		// �e�L�����̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
		int nNumVertex;			// ���_��
		DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

								// ���_�����擾
		nNumVertex = g_shogiraModel[nCntModel].paMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_shogiraModel[nCntModel].paMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_shogiraModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// ���_�����J��Ԃ�
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// �Ԓn����D3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
			// x //
			// �ŏ��l
			if (g_shogiraModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_shogiraModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// �ő�l
			if (g_shogiraModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_shogiraModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// �ŏ��l
			if (g_shogiraModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_shogiraModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// �ő�l
			if (g_shogiraModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_shogiraModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// �ŏ��l
			if (g_shogiraModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_shogiraModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// �ő�l
			if (g_shogiraModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_shogiraModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// �T�C�Y�|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
		}

		// �A�����b�N
		g_shogiraModel[nCntModel].paMesh->UnlockVertexBuffer();

		// �ő�l�ƍŏ��l�̋���
		g_shogiraModel[nCntModel].size = g_shogiraModel[nCntModel].vtxMaxMaterials - g_shogiraModel[nCntModel].vtxMinMaterials;
	}

	// �p�[�c�Ǎ�
	LoadShogiraModel();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureShogira[0]);
}

// ------------------------------------------------------------------------------------------
// �L������񏈗�
// ------------------------------------------------------------------------------------------
void Init_ShogiraInfo(void)
{
	// �ϐ��錾
	int		nCntShogira;	// �v���C���[�J�E���g
	int		nCntModel;		// �e�L�����̃J�E���g

	// �L�����̏����ݒ� //
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_shogira[nCntShogira].bUse == true)
			{
				DeleteShadow(g_shogira[nCntShogira].nIdxShadow);
			}
		}

		g_shogira[nCntShogira].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_shogira[nCntShogira].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].posBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].posAttack = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].EffectCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_shogira[nCntShogira].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shogira[nCntShogira].nMotionType = 0;
		g_shogira[nCntShogira].nMotionTypeOld = 0;
		g_shogira[nCntShogira].nParent = 0;
		g_shogira[nCntShogira].nIndex = nCntShogira;
		g_shogira[nCntShogira].nFram = 0;
		g_shogira[nCntShogira].nCntKeySet = 0;
		g_shogira[nCntShogira].nCntDamage = 0;
		g_shogira[nCntShogira].nIdxShadow = nCntShogira;
		g_shogira[nCntShogira].nIdxPrediction = 0;
		g_shogira[nCntShogira].fLength = 50.0f;
		g_shogira[nCntShogira].bUse = false;
		g_shogira[nCntShogira].bDisp = false;

		// �J���[���
		g_shogira[nCntShogira].nCntCor = 0;
		g_shogira[nCntShogira].diffuse = 1.0f;

		// �X�e�[�^�X���
		g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;
		g_shogira[nCntShogira].status.nCntStatus = 0;
		g_shogira[nCntShogira].status.nAttack = SHOGIRA_ATTACK;
		g_shogira[nCntShogira].status.nLife = SHOGIRA_HP;
		g_shogira[nCntShogira].status.bColition = false;
		g_shogira[nCntShogira].status.bAttack = false;
		g_shogira[nCntShogira].status.bShot = false;
		g_shogira[nCntShogira].status.bRot = false;
		g_shogira[nCntShogira].status.bMove = false;

		// �p�[�c���Ƃ̃J�E���g
		for (nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
		{
			g_shogira[nCntShogira].model[nCntModel].pos = g_shogiraModel[nCntModel].pos;
			g_shogira[nCntShogira].model[nCntModel].posLast = g_shogiraModel[nCntModel].posLast;
			g_shogira[nCntShogira].model[nCntModel].posKeyBetween = g_shogiraModel[nCntModel].posKeyBetween;
			g_shogira[nCntShogira].model[nCntModel].rot = g_shogiraModel[nCntModel].rot;
			g_shogira[nCntShogira].model[nCntModel].rotLast = g_shogiraModel[nCntModel].rotLast;
			g_shogira[nCntShogira].model[nCntModel].rotKeyBetween = g_shogiraModel[nCntModel].rotKeyBetween;
			g_shogira[nCntShogira].model[nCntModel].size = g_shogiraModel[nCntModel].size;
			g_shogira[nCntShogira].model[nCntModel].vtxMinMaterials = g_shogiraModel[nCntModel].vtxMinMaterials;
			g_shogira[nCntShogira].model[nCntModel].vtxMaxMaterials = g_shogiraModel[nCntModel].vtxMaxMaterials;
			g_shogira[nCntShogira].model[nCntModel].nIdxModelParent = g_shogiraModel[nCntModel].nIdxModelParent;
			g_shogira[nCntShogira].model[nCntModel].nFram = g_shogiraModel[nCntModel].nFram;
			g_shogira[nCntShogira].model[nCntModel].nCntKeySet = g_shogiraModel[nCntModel].nCntKeySet;
		}
	}

	// ���[�h�L����
	LoadShogira();
}

// ------------------------------------------------------------------------------------------
// �L�������[�V��������
// ------------------------------------------------------------------------------------------
void Init_MotionShogira(void)
{
	// �ϐ��錾
	int		nCntShogira;						// �L�����J�E���g

												// ���[�V�����p�ϐ�
	int		nCntMotion;							// ���[�V�����J�E���g
	int		nCntKeySet;							// ���[�V�����J�E���g
	int		nCntKey;							// �L�[�J�E���g

												// ���[�V�����J�E���g //
	for (nCntMotion = 0; nCntMotion < SHOGIRAMOSIONSET_MAX; nCntMotion++)
	{
		// ���[�V�����̏����ݒ�
		g_shogiraKey[nCntMotion].nNumKey = 0;		// �i���o�[�L�[
		g_shogiraKey[nCntMotion].nLoop = 0;		// ���[�v�̗L��

												// �����蔻��p //
		g_shogiraKey[nCntMotion].Collision_nParts = 0;		// �p�[�c�ԍ�
		g_shogiraKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
		g_shogiraKey[nCntMotion].Collision_Radius = 0.0f;	// �~�̔��a
		g_shogiraKey[nCntMotion].Collision_StartFram = 0;	// �J�n�t���[����
		g_shogiraKey[nCntMotion].Collision_EndFram = 0;		// �I���t���[����
		g_shogiraKey[nCntMotion].Collision_Damage = 0;		// �_���[�W��

															// �O�՗p //
		g_shogiraKey[nCntMotion].Orbit_nParts = 0;			// �p�[�c�ԍ�
		g_shogiraKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
		g_shogiraKey[nCntMotion].Orbit_Mesh = 0;			// �O�Ղ̐�
		g_shogiraKey[nCntMotion].Orbit_StartFram = 0;		// �J�n�t���[����
		g_shogiraKey[nCntMotion].Orbit_EndFram = 0;			// �I���t���[����
		g_shogiraKey[nCntMotion].Orbit_nType = 1;			// �O�Ղ̃^�C�v
		g_shogiraKey[nCntMotion].Orbit_nIdTrajectory = -1;	// �O�Ղ̔ԍ�
		g_shogiraKey[nCntMotion].Orbit_bUse = false;		// �O�Ղ̎g�p���

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_shogiraKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// �L�[���Ƃɏ�����
			for (nCntKey = 0; nCntKey < SHOGIRATYPE_MAX; nCntKey++)
			{
				g_shogiraKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_shogiraKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// �v���C���[���[�V�����ǂݍ���
	LoadShogiraMosion();

	// �L�����̏����ݒ� //
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < SHOGIRAMOSIONSET_MAX; nCntMotion++)
			{
				// �g�p���
				if (g_shogiraKey[nCntMotion].Orbit_bUse == true)
				{
					// �O�Ղ̐ݒ�
					g_shogiraKey[nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
						g_shogiraKey[nCntMotion].Orbit_Ofset,
						g_shogiraKey[nCntMotion].Orbit_nParts,
						g_shogiraKey[nCntMotion].Orbit_Mesh,
						g_shogiraKey[nCntMotion].Orbit_StartFram,
						g_shogiraKey[nCntMotion].Orbit_EndFram,
						g_shogiraKey[nCntMotion].Orbit_nType);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitShogira(void)
{
	// �e�L�����f�[�^�̊J��
	for (int nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
	{
		// ���b�V���̊J��
		if (g_shogiraModel[nCntModel].paMesh != NULL)
		{
			g_shogiraModel[nCntModel].paMesh->Release();
			g_shogiraModel[nCntModel].paMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_shogiraModel[nCntModel].paBuffMat != NULL)
		{
			g_shogiraModel[nCntModel].paBuffMat->Release();
			g_shogiraModel[nCntModel].paBuffMat = NULL;
		}
	}

	// �e�L�����e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < TEXTURE_SHOGIRA; nCntTexture++)
	{
		// �e�N�X�`���[�̊J��
		if (g_paTextureShogira[nCntTexture] != NULL)
		{
			g_paTextureShogira[nCntTexture]->Release();
			g_paTextureShogira[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateShogira(void)
{
	// �J�E���g�L����
	int nCntShogira;
	PLAYER *pPlayer = GetPlayer();

	// �L�����̃J�E���g
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse)
		{
			// ���ݒn�ۑ�
			g_shogira[nCntShogira].posold = g_shogira[nCntShogira].pos;

			// ���[�V������ۑ�
			g_shogira[nCntShogira].nMotionTypeOld = g_shogira[nCntShogira].nMotionType;

			// ��ԏ���
			Update_StateShogira(nCntShogira);

			// ����ł��Ȃ����
			if (g_shogira[nCntShogira].status.nStatus != SHOGIRASTATUS_DIE)
			{
				if (GetMode() != MODE_TUTORIAL)
				{
					// �U����ԂłȂ��Ƃ�
					if (g_shogira[nCntShogira].status.bAttack == false)
					{
						// ���ˏ�ԂłȂ��Ƃ�
						if (g_shogira[nCntShogira].status.bShot == false)
						{
							// �ړ���Ԃł͂Ȃ��Ƃ�
							if (g_shogira[nCntShogira].status.bMove == false)
							{
								// ��]����
								Update_RotShogira(nCntShogira);
							}
						}
					}

					// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
					if (g_shogira[nCntShogira].status.bMove == false)
					{
						// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
						if (g_shogira[nCntShogira].status.bRot == false)
						{
							if (g_shogira[nCntShogira].status.bAttack == false)
							{
								// �e�̏���
								Update_ShotShogira(nCntShogira);
							}
						}
					}
				}
				// �U���̓����蔻��
				Update_DecisionShogira(nCntShogira);

			}

			g_shogira[nCntShogira].move.y -= 0.5f;
			g_shogira[nCntShogira].pos.y += g_shogira[nCntShogira].move.y;

			// �����蔻��
			Update_ColisionShogira(nCntShogira);

			// ����ł��Ȃ����
			if (g_shogira[nCntShogira].status.nStatus != SHOGIRASTATUS_DIE)
			{
				// ���[�V��������
				Update_MosionShogira(nCntShogira);
			}

			// �e�̈ʒu��ݒ�
			SetPositionShadow(
				g_shogira[nCntShogira].nIdxShadow,
				g_shogira[nCntShogira].pos +
				D3DXVECTOR3(
					sinf(-g_shogira[nCntShogira].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_shogira[nCntShogira].rot.y) * 10.0f));

			// �U�����������Ă���
			if (g_shogira[nCntShogira].status.bColition == true)
			{
				// �U�����I�������
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_shogira[nCntShogira].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ��ԏ���
// ------------------------------------------------------------------------------------------
void Update_StateShogira(int nCntShogira)
{
	switch (g_shogira[nCntShogira].status.nStatus)
	{
	case SHOGIRASTATUS_NEUTRAL:
		g_shogira[nCntShogira].diffuse = 1.0f;
		break;

	case SHOGIRASTATUS_DAMAGE:
		// �J���[�̏�ԃJ�E���g�A�b�v
		g_shogira[nCntShogira].nCntCor++;

		// �\�����
		if (g_shogira[nCntShogira].nCntCor % 10 == 0)
		{
			g_shogira[nCntShogira].diffuse = 1.0f;
		}

		// ��\�����
		else if (g_shogira[nCntShogira].nCntCor % 5 == 0)
		{
			g_shogira[nCntShogira].diffuse = 0.5f;
		}

		if (g_shogira[nCntShogira].nCntCor == 50)
		{
			// �J���[�̏�ԃJ�E���g������
			g_shogira[nCntShogira].nCntCor = 0;

			// �L�����̏�Ԃ��m�[�}����
			g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;
		}

		break;

	case SHOGIRASTATUS_DIE:
		// �ڕW��]�ʂ��猻�݂̉�]�ʂ̍���
		g_shogira[nCntShogira].rotbetween.z =
			(g_shogira[nCntShogira].rotLast.z - g_shogira[nCntShogira].rot.z) / 10;

		// ��]
		g_shogira[nCntShogira].rot.z += g_shogira[nCntShogira].rotbetween.z;

		// �d��
		g_shogira[nCntShogira].move.y -= 0.5f;	// �ړ���
		g_shogira[nCntShogira].pos.y += g_shogira[nCntShogira].move.y;

		// �X�e�[�^�X�̃J�E���g�A�b�v
		g_shogira[nCntShogira].status.nCntStatus++;

		DeleteBillboard(BILLBOARDTYPE_ENTER);
		// �X�e�[�^�X�̃J�E���g���K��l�ȏ�ɂȂ�����
		if (g_shogira[nCntShogira].status.nCntStatus == 70)
		{
			if (GetMode() == MODE_TUTORIAL)
			{
				// �o�b�t�@���[�o��
				SetBuffalo(
					g_shogira[nCntShogira].pos,
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
						g_shogira[nCntShogira].pos.x,
						g_shogira[nCntShogira].pos.y,
						g_shogira[nCntShogira].pos.z),

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
			g_shogira[nCntShogira].bUse = false;
			g_shogira[nCntShogira].bDisp = false;

			// �e�̏���
			DeleteShadow(g_shogira[nCntShogira].nIdxShadow);
			// �\�����̏���
			DeletePrediction(g_shogira[nCntShogira].nIdxPrediction);
			// �g�p���̋O�Ղ̏���
			DeleteTrajectory(g_shogiraKey[g_shogira[nCntShogira].nMotionType].Orbit_nIdTrajectory);

			// �X�e�[�^�X�̃J�E���g�̏�����
			g_shogira[nCntShogira].status.nCntStatus = 0;

			// �X�e�[�^�X��
			g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;

			// �A�C�e���o��
			SetItem(
				g_shogira[nCntShogira].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ��]
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// �ړ���
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// �T�C�Y
				20.0f,								// ���a
				1,									// �e�N�X�`���[
				100);								// �o������

			// ���j��
			g_shogiraTotal++;

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
void Update_RotShogira(int nCntShogira)
{
	// �����]���ԍ�
	int nRandNum = 0;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_shogira[nCntShogira].status.bRot == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 500;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �ړI�̉�]��
			g_shogira[nCntShogira].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// ��]�ʂ�����l�𒴂�����
			if (g_shogira[nCntShogira].rotLast.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rotLast.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += D3DX_PI * 2;
			}

			// ��]���
			g_shogira[nCntShogira].status.bRot = true;
		}
	}

	// �ړI��]�ʂ��猻�݉�]�ʂ̍���
	g_shogira[nCntShogira].rotbetween.y = (g_shogira[nCntShogira].rotLast.y - g_shogira[nCntShogira].rot.y) * 0.01f;
	g_shogira[nCntShogira].rot.y += g_shogira[nCntShogira].rotbetween.y;

	// ��]��ԏI��
	if (g_shogira[nCntShogira].rot.y >= -0.1f ||
		g_shogira[nCntShogira].rot.y <= 0.1f)
	{
		g_shogira[nCntShogira].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// ���[�V��������
// ------------------------------------------------------------------------------------------
void Update_MosionShogira(int nCntShogira)
{
	// �ϐ��錾
	int	nCntModel;			// �J�E���g�e�L����

		// ���݂̃��[�V�����ƑO��̃��[�V�������r
	if (g_shogira[nCntShogira].nMotionType != g_shogira[nCntShogira].nMotionTypeOld)
	{

		g_shogira[nCntShogira].nFram = 0;

		g_shogira[nCntShogira].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
	{
		// �t���[�������O�̏ꍇ
		if (g_shogira[nCntShogira].nFram == 0)
		{
			if (g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram != 0)
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_shogira[nCntShogira].model[nCntModel].rotKeyBetween =
					(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].rot -
						g_shogira[nCntShogira].model[nCntModel].rot) /
						(float)g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_shogira[nCntShogira].model[nCntModel].posKeyBetween =
					(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].pos -
						g_shogira[nCntShogira].model[nCntModel].pos) /
						(float)g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram;
			}

			else
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_shogira[nCntShogira].model[nCntModel].rot =
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].rot;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_shogira[nCntShogira].model[nCntModel].pos =
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].pos;

				// �����̏�����
				g_shogira[nCntShogira].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �����̏�����
				g_shogira[nCntShogira].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// ��]����
		g_shogira[nCntShogira].model[nCntModel].rot +=
			g_shogira[nCntShogira].model[nCntModel].rotKeyBetween;

		// �ړ�����
		g_shogira[nCntShogira].model[nCntModel].pos +=
			g_shogira[nCntShogira].model[nCntModel].posKeyBetween;
	}

	// �t���[�������K��l�Ɠ����ɂȂ�����
	if (g_shogira[nCntShogira].nFram ==
		g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram)
	{
		// �L�[�Z�b�g�̃J�E���g�A�b�v
		g_shogira[nCntShogira].nCntKeySet++;

		// �t���[����������
		g_shogira[nCntShogira].nFram = 0;

		// �L�[�Z�b�g�����K��l�Ɠ����ɂȂ�����
		if (g_shogira[nCntShogira].nCntKeySet == g_shogiraKey[g_shogira[nCntShogira].nMotionType].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (g_shogiraKey[g_shogira[nCntShogira].nMotionType].nLoop == 0)
			{

				// �L�[�Z�b�g��������
				g_shogira[nCntShogira].nCntKeySet = 0;

				g_shogira[nCntShogira].nMotionType = 0;
			}

			// ���[�v���鎞
			else
			{
				// �L�[�Z�b�g��������
				g_shogira[nCntShogira].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// �t���[�����J�E���g�A�b�v
		g_shogira[nCntShogira].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// �e����
// ------------------------------------------------------------------------------------------
void Update_ShotShogira(int nCntShogira)
{
	// �ϐ��錾
	int	nAttackNum;				// �U���ԍ�
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƃL�����Ƃ̋���
	diffpos = pPlayerPos - g_shogira[nCntShogira].pos;

	// �U���s���͈�
	if ((SHOGIRA_SHOT_DECISION * SHOGIRA_SHOT_DECISION) >=	// �U���s���̉~�͈�
		diffpos.x * diffpos.x +								// x���̉~
		diffpos.y * diffpos.y +								// y���̉~
		diffpos.z * diffpos.z)								// z���̉~
	{
		// �U���̊m��
		nAttackNum = rand() % SHOGIRA_FREQUENCY;

		// �U��
		if (nAttackNum == 0 &&
			g_shogira[nCntShogira].status.bShot == false)
		{
			g_shogira[nCntShogira].EffectCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �v���C���[�̈ʒu���擾
			g_shogira[nCntShogira].posAttack = GetPlayer()->pos;

			// �\��������
			g_shogira[nCntShogira].nIdxPrediction = SetPrediction(
				g_shogira[nCntShogira].posAttack,
				D3DXVECTOR3(20.0f,20.0f,20.0f) ,
				D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

			// ���ʍU�����[�V����
			g_shogira[nCntShogira].nMotionType = SHOGIRAMOSIONSET_SHOT;

			// �U�����ON
			g_shogira[nCntShogira].status.bShot = true;
		}
	}

	// �e����
	if (g_shogira[nCntShogira].nMotionType == SHOGIRAMOSIONSET_SHOT)
	{
		// ���˂���O
		if (g_shogira[nCntShogira].nCntKeySet >= 0 &&
			g_shogira[nCntShogira].nCntKeySet <= 2)
		{
			// �΂̂���̌���
			g_shogira[nCntShogira].EffectCol.g -= 1.0f /
				(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[0].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[1].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[2].nFram);
			// �̂���̌���
			g_shogira[nCntShogira].EffectCol.b -= 1.0f /
				(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[0].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[1].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[2].nFram);

			// �L�������猩�ăv���C���[������p�x
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// �ړI�̉�]��
			g_shogira[nCntShogira].rotLast.y = fAngle - D3DX_PI;

			// ��]�ʂ�����l�𒴂�����
			if (g_shogira[nCntShogira].rotLast.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rotLast.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += D3DX_PI * 2;
			}

			// �ړI��]�ʂ��猻�݉�]�ʂ̍���
			g_shogira[nCntShogira].rotbetween.y =
				(g_shogira[nCntShogira].rotLast.y - g_shogira[nCntShogira].rot.y);

			// ��]�ʂ�����l�𒴂�����
			if (g_shogira[nCntShogira].rotbetween.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rotbetween.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rotbetween.y += D3DX_PI * 2;
			}

			g_shogira[nCntShogira].rot.y += g_shogira[nCntShogira].rotbetween.y * 0.1f;

			// ��]�ʂ�����l�𒴂�����
			if (g_shogira[nCntShogira].rot.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rot.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rot.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rot.y += D3DX_PI * 2;
			}
			// �v���C���[�̈ʒu���擾
			g_shogira[nCntShogira].posAttack = GetPlayer()->pos;
			// �\�����̈ʒu���ݒ�
			SetPositionPrediction(g_shogira[nCntShogira].nIdxPrediction, g_shogira[nCntShogira].posAttack);

			for (int nCntEffect = 0; nCntEffect < 5; nCntEffect++)
			{
				float fXZAngle, fYAngle;
				D3DXVECTOR3 move;
				D3DXCOLOR col;
				// X�EZ�̊p�x
				fXZAngle =
					float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

				// Y�̊p�x
				fYAngle =
					float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

				// �ړ�����
				move =
					D3DXVECTOR3(0.0f,
						0.5f,
						0.0f);

				// �J���[
				col =
					D3DXCOLOR(
						1.0f,
						g_shogira[nCntShogira].EffectCol.g,
						g_shogira[nCntShogira].EffectCol.b,
						1.0f);

				// �G�t�F�N�g�̐ݒ�
				SetEffect(
					D3DXVECTOR3(													// �ʒu
						g_shogira[nCntShogira].pos.x + sinf(fXZAngle) * 20.0f,
						0.0f,
						g_shogira[nCntShogira].pos.z + cosf(fXZAngle) * 20.0f),
					move,															// �ړ���
					col,															// �F
					5,																// �T�C�Y
					-0.1f,																// �����T�C�Y
					0.0f,															// �d��
					50,																// �G�t�F�N�g���C�t
					0,																// �^�C�v
					0);
			}
		}

		// ���˃^�C�~���O
		else if (g_shogira[nCntShogira].nCntKeySet == 3)
		{
			// ���ˏ�Ԃł͂Ȃ��ꍇ
			if (g_shogira[nCntShogira].status.bShot == true)
			{
				// �J�E���g�_���[�W
				g_shogira[nCntShogira].nCntDamage++;

				// �Z�̔�������
				if (g_shogira[nCntShogira].nCntDamage >= 20)
				{
					PlaySound(SOUND_LABEL_SE_BEAM);

					SetExplosion(
						g_shogira[nCntShogira].posAttack,
						EXPLOSIONTYPE_THUNDER,
						50.0f,
						g_shogira[nCntShogira].status.nAttack);

					// ���ˏ��
					g_shogira[nCntShogira].status.bShot = false;
					g_shogira[nCntShogira].nCntDamage = 0;
					// �\�����̍폜
					DeletePrediction(g_shogira[nCntShogira].nIdxPrediction);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �U���̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_DecisionShogira(int nCntShogira)
{
	// �v���C���[���擾
	PLAYER * pPlayer = GetPlayer();

	// �G�������Ă��邩�ǂ���
	if (g_shogira[nCntShogira].status.nStatus != SHOGIRASTATUS_DIE)
	{
		// �U����Ԃ��ǂ���
		if (g_shogira[nCntShogira].nMotionType == SHOGIRAMOSIONSET_NORMALATACK)
		{
			// �v���C���[�̍U�����󂯂Ă��邩�ǂ���
			if (pPlayer->status.bColition == false)
			{
				// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
				if ((pPlayer->status.fLength * pPlayer->status.fLength) +
					(g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_Radius *
						g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_Radius) >=
						(pPlayer->pos.x - g_shogira[nCntShogira].model[g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_nParts].mtxWorldModel._41) *
					(pPlayer->pos.x - g_shogira[nCntShogira].model[g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_nParts].mtxWorldModel._41) +
					(pPlayer->pos.y - g_shogira[nCntShogira].model[g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_nParts].mtxWorldModel._42) *
					(pPlayer->pos.y - g_shogira[nCntShogira].model[g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_nParts].mtxWorldModel._42) +
					(pPlayer->pos.z - g_shogira[nCntShogira].model[g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_nParts].mtxWorldModel._43) *
					(pPlayer->pos.z - g_shogira[nCntShogira].model[g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_nParts].mtxWorldModel._43))
				{
					// �L�����̃_���[�W����
					HitPlayer(
						g_shogira[nCntShogira].status.nAttack +
						g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionShogira(int nCntShogira)
{
	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_shogira[nCntShogira].pos,
		&g_shogira[nCntShogira].posold,
		&g_shogira[nCntShogira].move,
		&g_shogira[nCntShogira].model[0].size,
		&g_shogira[nCntShogira].pMeshField))
	{

	}

	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_shogira[nCntShogira].pos,
		&g_shogira[nCntShogira].posold,
		&g_shogira[nCntShogira].move,
		&g_shogira[nCntShogira].model[0].size,
		&g_shogira[nCntShogira].pMaterials) != -1)
	{
		// �ڕW�n�_�����ݒn�X��
		g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;

		// ���[�V���������R�֖̂߂�
		g_shogira[nCntShogira].nMotionType = SHOGIRAMOSIONSET_NEUTRAL;
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_shogira[nCntShogira].pos,
		&g_shogira[nCntShogira].posold,
		&g_shogira[nCntShogira].move,
		&g_shogira[nCntShogira].model[0].size,
		&g_shogira[nCntShogira].pWall))
	{
		// �ڕW�n�_�����ݒn�X��
		g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;

		// ���[�V���������R�֖̂߂�
		g_shogira[nCntShogira].nMotionType = SHOGIRAMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawShogira(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;				// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;				// �}�e���A���f�[�^�̃|�C���^
	int nCntShogira;
	int nCntModel;
	int nCnt = 0;

	// �J�E���g�e�L����
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_shogira[nCntShogira].mtxWorldShogira);

			// ��]�𔽉f
			// �X�P�[���𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_shogira[nCntShogira].rot.y, 
				g_shogira[nCntShogira].rot.x, 
				g_shogira[nCntShogira].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_shogira[nCntShogira].mtxWorldShogira,
				&g_shogira[nCntShogira].mtxWorldShogira, &mtxRot);


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_shogira[nCntShogira].pos.x, 
				g_shogira[nCntShogira].pos.y, 
				g_shogira[nCntShogira].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_shogira[nCntShogira].mtxWorldShogira,	// 1
				&g_shogira[nCntShogira].mtxWorldShogira,					// 2
				&mtxTrans);									// 3

			// �J�E���g�e�L����
			for (nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_shogira[nCntShogira].model[nCntModel].rot.y,
					g_shogira[nCntShogira].model[nCntModel].rot.x, 
					g_shogira[nCntShogira].model[nCntModel].rot.z);

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
				D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
					&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel, &mtxRot);

				// �ʒu�𔽉f //
				// ���s�ړ��s��쐬(�I�t�Z�b�g)
				D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
					g_shogira[nCntShogira].model[nCntModel].pos.x,
					g_shogira[nCntShogira].model[nCntModel].pos.y, 
					g_shogira[nCntShogira].model[nCntModel].pos.z);	// ���ꂼ��̈ړ���

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
				D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,	// 1
					&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [���ׂĂ̐e]����Ȃ��ꍇ
				if (g_shogira[nCntShogira].model[nCntModel].nIdxModelParent != -1)
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].model[g_shogira[nCntShogira].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].mtxWorldShogira);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_shogira[nCntShogira].model[nCntModel].mtxWorldModel);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)g_shogiraModel[nCntModel].paBuffMat->GetBufferPointer();

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_shogiraModel[nCntModel].nNumMat; nCntMat++, pMat++,nCnt++)
				{
					// �{�̂̃J���[���X�V
					pMat->MatD3D.Diffuse.a = g_shogira[nCntShogira].diffuse;

					// �e�N�X�`���[����
					if (pMat->pTextureFilename != NULL)
					{
						// �e�N�X�`���[�ݒ�
						pDevice->SetTexture(0, g_paTextureShogira[0]);
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
					g_shogiraModel[nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitShogira(int nCntShogira, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �U�����������Ă���
	g_shogira[nCntShogira].status.bColition = true;

	// �J���[�̏�ԃJ�E���g������
	g_shogira[nCntShogira].nCntCor = 0;

	// HP������
	g_shogira[nCntShogira].status.nLife -= nDamage;

	// HP��0�ɂȂ�����
	if (g_shogira[nCntShogira].status.nLife <= 0)
	{
		// ���S���
		g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_DIE;

		// �ڕW��]��
		g_shogira[nCntShogira].rotLast.z = D3DX_PI / 2;

		// �ړ���
		g_shogira[nCntShogira].move.y = 5.0f;

		PlaySound(SOUND_LABEL_SE_COLISION);
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
					g_shogira[nCntShogira].pos.x,
					g_shogira[nCntShogira].pos.y,
					g_shogira[nCntShogira].pos.z),

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
		// ����
		g_shogira[nCntShogira].move.y = 5.0f;

		// �_���[�W���
		g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_DAMAGE;

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
					g_shogira[nCntShogira].pos.x,
					g_shogira[nCntShogira].pos.y,
					g_shogira[nCntShogira].pos.z),
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
// �L�����̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void ColisionShogira(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// �L�����e�L�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadShogiraModel(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/GIRAFFE/giraffe_model.txt", "r");

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

				// �e�L�����Z�b�g��������
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// �G���h�e�L�����Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_shogiraModel[nCntModel].nIdxModelParent);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_shogiraModel[nCntModel].pos.x,
								&g_shogiraModel[nCntModel].pos.y, 
								&g_shogiraModel[nCntModel].pos.z);

							// �ʒu���ۑ�
							g_shogiraModel[nCntModel].posOrigin = g_shogiraModel[nCntModel].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_shogiraModel[nCntModel].rot.x, 
								&g_shogiraModel[nCntModel].rot.y,
								&g_shogiraModel[nCntModel].rot.z);
						}
					}
					// �e�L�����̍X�V
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
		MessageBox(NULL, "�L�����̃p�[�c�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �L�������̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadShogira(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntShogira = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �{�X�p
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		return;
	}

	// ����ȊO
	else
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/SHOGIRA/shogira.txt", "r");
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

				// �e�L�����Z�b�g��������
				else if (strcmp(cHeadText, "SHOGIRASET") == 0)
				{

					// �G���h�e�L�����Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_SHOGIRASET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ʒu���ǂݍ���
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_shogira[nCntShogira].pos.x,
								&g_shogira[nCntShogira].pos.y,
								&g_shogira[nCntShogira].pos.z);

							// �ړI�ʒu�ɕۑ�
							g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_shogira[nCntShogira].rot.x,
								&g_shogira[nCntShogira].rot.y,
								&g_shogira[nCntShogira].rot.z);

							// �e�̍쐬
							g_shogira[nCntShogira].nIdxShadow = SetShadow(
								g_shogira[nCntShogira].pos,
								D3DXVECTOR3(20.0f, 20.0f, 20.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}

					if (GetMode() == MODE_TUTORIAL)
					{
						SetBillboard(D3DXVECTOR3(
							g_shogira[nCntShogira].pos.x,
							g_shogira[nCntShogira].pos.y,
							g_shogira[nCntShogira].pos.z),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							1);
					}

					// �g�p���
					g_shogira[nCntShogira].bUse = true;

					// �\�����
					g_shogira[nCntShogira].bDisp = true;

					// �e�L�����̍X�V
					nCntShogira++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�e�L�����̃f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �L�������[�V�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadShogiraMosion(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntMotion = 0;							// �v���C���[�J�E���g
	int	nCntModel = 0;							// �e�L�����J�E���g
	int	nCntKeySet = 0;							// ���[�V�����J�E���g
	int	nCntKey = 0;							// �L�[�J�E���g
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/GIRAFFE/giraffe_motion.txt", "r");

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
					// �e�L�����̏�����
					nCntModel = 0;

					// �G���h�e�L�����Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ���[�v���邩�ǂ������ǂݍ���
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_shogiraKey[nCntMotion].nLoop);
						}

						// �L�[�����ǂݍ���
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_shogiraKey[nCntMotion].nNumKey);
						}

						// �����蔻����ǂݍ���
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie,
								&g_shogiraKey[nCntMotion].Collision_nParts,
								&g_shogiraKey[nCntMotion].Collision_Ofset.x,
								&g_shogiraKey[nCntMotion].Collision_Ofset.y,
								&g_shogiraKey[nCntMotion].Collision_Ofset.z,
								&g_shogiraKey[nCntMotion].Collision_Radius,
								&g_shogiraKey[nCntMotion].Collision_StartFram,
								&g_shogiraKey[nCntMotion].Collision_EndFram);
						}

						// �_���[�W���ǂݍ���
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_shogiraKey[nCntMotion].Collision_Damage);
						}

						// �O�Տ��ǂݍ���
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&g_shogiraKey[nCntMotion].Orbit_nParts,
								&g_shogiraKey[nCntMotion].Orbit_Ofset.x,
								&g_shogiraKey[nCntMotion].Orbit_Ofset.y,
								&g_shogiraKey[nCntMotion].Orbit_Ofset.z,
								&g_shogiraKey[nCntMotion].Orbit_Mesh,
								&g_shogiraKey[nCntMotion].Orbit_StartFram,
								&g_shogiraKey[nCntMotion].Orbit_EndFram);

							// �L�����N�^�[�^�C�v
							g_shogiraKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_GIRAFFE;

							// �g�p���
							g_shogiraKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_shogiraKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_shogira[0].model[nCntKey].pos;
										}

										// ��]���ǂݍ���
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// �J�E���g�L�[
									nCntKey++;
								}
							}

							// �J�E���g�e�L����
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
		MessageBox(NULL, "�e�L�����̃��[�V�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �L������񏈗�
// ------------------------------------------------------------------------------------------
SHOGIRA *GetShogira(void)
{
	return &g_shogira[0];
}


// ------------------------------------------------------------------------------------------
// �L�������j������
// ------------------------------------------------------------------------------------------
int GetShogiraTotal(void)
{
	return g_shogiraTotal;
}

// ------------------------------------------------------------------------------------------
// �L�����ݒu����
// ------------------------------------------------------------------------------------------
void SetShogira(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �J�E���g�e�L����
	for (int nCntShogira = 0; nCntShogira < MAX_BUFFALO; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse == false)
		{
			// �ʒu���
			g_shogira[nCntShogira].pos = pos;


			// �ړI�ʒu�ɕۑ�
			g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;

			// ��]���
			g_shogira[nCntShogira].rot = rot;

			// �g�p���
			g_shogira[nCntShogira].bUse = true;

			// �\�����
			g_shogira[nCntShogira].bDisp = true;

			// �e�̍쐬
			g_shogira[nCntShogira].nIdxShadow = SetShadow(
				g_shogira[nCntShogira].pos,
				D3DXVECTOR3(20.0f,0.0f,20.0f),
				D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

			// �X�e�[�^�X���
			g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;
			g_shogira[nCntShogira].status.nCntStatus = 0;
			g_shogira[nCntShogira].status.nAttack = SHOGIRA_ATTACK;
			g_shogira[nCntShogira].status.nLife = SHOGIRA_HP;
			g_shogira[nCntShogira].status.bColition = false;
			g_shogira[nCntShogira].status.bAttack = false;
			g_shogira[nCntShogira].status.bShot = false;
			g_shogira[nCntShogira].status.bRot = false;
			g_shogira[nCntShogira].status.bMove = false;

			break;
		}
	}
}
