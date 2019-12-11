// ------------------------------------------------------------------------------------------
//
// �L�������� [giraffe.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "giraffe.h"
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
#define TEXTURE_GIRAFFE (1)
#define GIRAFFE_HP (30)
#define GIRAFFE_ATTACK (10)					// �U����
#define GIRAFFE_FREQUENCY (100)				// �U���p�x
#define GIRAFFE_NORMALATTACK_DECISION (100)
#define GIRAFFE_SHOT_DECISION (500)
#define GIRAFFE_G (0.5f)			// �d��
#define GIRAFFE_RESISTANCE (0.5f)// ��R��

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void);
void Init_GiraffeInfo(void);
void Init_MotionGiraffe(void);
void Update_StateGiraffe(int nCntGiraffe);
void Update_RotGiraffe(int nCntGiraffe);
void Update_MoveGiraffe(int nCntGiraffe);
void Update_MosionGiraffe(int nCntGiraffe);
void Update_NormalAttackGiraffe(int nCntGiraffe);
void Update_ShotGiraffe(int nCntGiraffe);
void Update_DecisionGiraffe(int nCntGiraffe);
void Update_ColisionGiraffe(int nCntGiraffe);
void LoadGiraffeModel(void);
void LoadGiraffe(void);
void LoadGiraffeMosion(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureGiraffe[TEXTURE_GIRAFFE] = {};	// �e�N�X�`���ւ̃|�C���^

GIRAFFE				g_giraffe[MAX_GIRAFFE];						// �L�������

MODEL				g_giraffeModel[GIRAFFETYPE_MAX];			// �L�������

MOSION_GIRAFFE		g_giraffeKey[GIRAFFEMOSIONSET_MAX];			// ���[�V�������

int					g_giraffeTotal;								// �L�������j�����v

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitGiraffe(void)
{
	g_giraffeTotal = 0;
	// �L�����p�[�c���
	Init_PartsGiraffe();

	// �L�������
	Init_GiraffeInfo();

	// �L�������[�V�������
	Init_MotionGiraffe();
}

// ------------------------------------------------------------------------------------------
// �L�����p�[�c��񏈗�
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	int		nCntModel;							// �L�����̃J�E���g

												// �L�����t�@�C������`
	char *paGiraffeFile[GIRAFFETYPE_MAX] =
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
	char *paTextureFile[TEXTURE_GIRAFFE] =
	{
		{ "data/TEXTURE/giraffe/00_giraffe_Texture.png" },
	};

	// �p�[�c�̓ǂݍ��� //
	for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		g_giraffeModel[nCntModel].paBuffMat = NULL;
		g_giraffeModel[nCntModel].paMesh = NULL;
		g_giraffeModel[nCntModel].nNumMat = 0;
		g_giraffeModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].nIdxModelParent = 0;
		g_giraffeModel[nCntModel].nFram = 0;
		g_giraffeModel[nCntModel].nCntKeySet = 0;

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

		// �L�����̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
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

			// �T�C�Y�|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
		}

		// �A�����b�N
		g_giraffeModel[nCntModel].paMesh->UnlockVertexBuffer();

		// �ő�l�ƍŏ��l�̋���
		g_giraffeModel[nCntModel].size = g_giraffeModel[nCntModel].vtxMaxMaterials - g_giraffeModel[nCntModel].vtxMinMaterials;
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
	int		nCntModel;		// �L�����̃J�E���g

	// �L�����̏����ݒ� //
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_giraffe[nCntGiraffe].bUse == true)
			{
				DeleteShadow(g_giraffe[nCntGiraffe].nIdxShadow);
			}
		}

		g_giraffe[nCntGiraffe].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_giraffe[nCntGiraffe].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posAttack = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].EffectCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_giraffe[nCntGiraffe].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].nMotionType = 0;
		g_giraffe[nCntGiraffe].nMotionTypeOld = 0;
		g_giraffe[nCntGiraffe].nParent = 0;
		g_giraffe[nCntGiraffe].nIndex = nCntGiraffe;
		g_giraffe[nCntGiraffe].nFram = 0;
		g_giraffe[nCntGiraffe].nCntKeySet = 0;
		g_giraffe[nCntGiraffe].nCntDamage = 0;
		g_giraffe[nCntGiraffe].nCntAnim = 0;
		g_giraffe[nCntGiraffe].nIdxShadow = nCntGiraffe;
		g_giraffe[nCntGiraffe].nIdxPrediction = 0;
		g_giraffe[nCntGiraffe].fLength = 50.0f;
		g_giraffe[nCntGiraffe].bUse = false;
		g_giraffe[nCntGiraffe].bDisp = false;

		// �J���[���
		g_giraffe[nCntGiraffe].nCntCor = 0;
		g_giraffe[nCntGiraffe].diffuse = 1.0f;

		// �X�e�[�^�X���
		g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;
		g_giraffe[nCntGiraffe].status.nCntStatus = 0;
		g_giraffe[nCntGiraffe].status.nAttack = GIRAFFE_ATTACK;
		g_giraffe[nCntGiraffe].status.nLife = GIRAFFE_HP;
		g_giraffe[nCntGiraffe].status.bColition = false;
		g_giraffe[nCntGiraffe].status.bAttack = false;
		g_giraffe[nCntGiraffe].status.bShot = false;
		g_giraffe[nCntGiraffe].status.bRot = false;
		g_giraffe[nCntGiraffe].status.bMove = false;

		// �p�[�c���Ƃ̃J�E���g
		for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
		{
			g_giraffe[nCntGiraffe].model[nCntModel].pos = g_giraffeModel[nCntModel].pos;
			g_giraffe[nCntGiraffe].model[nCntModel].posLast = g_giraffeModel[nCntModel].posLast;
			g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween = g_giraffeModel[nCntModel].posKeyBetween;
			g_giraffe[nCntGiraffe].model[nCntModel].rot = g_giraffeModel[nCntModel].rot;
			g_giraffe[nCntGiraffe].model[nCntModel].rotLast = g_giraffeModel[nCntModel].rotLast;
			g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween = g_giraffeModel[nCntModel].rotKeyBetween;
			g_giraffe[nCntGiraffe].model[nCntModel].size = g_giraffeModel[nCntModel].size;
			g_giraffe[nCntGiraffe].model[nCntModel].vtxMinMaterials = g_giraffeModel[nCntModel].vtxMinMaterials;
			g_giraffe[nCntGiraffe].model[nCntModel].vtxMaxMaterials = g_giraffeModel[nCntModel].vtxMaxMaterials;
			g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent = g_giraffeModel[nCntModel].nIdxModelParent;
			g_giraffe[nCntGiraffe].model[nCntModel].nFram = g_giraffeModel[nCntModel].nFram;
			g_giraffe[nCntGiraffe].model[nCntModel].nCntKeySet = g_giraffeModel[nCntModel].nCntKeySet;
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
	int		nCntGiraffe;						// �L�����J�E���g

												// ���[�V�����p�ϐ�
	int		nCntMotion;							// ���[�V�����J�E���g
	int		nCntKeySet;							// ���[�V�����J�E���g
	int		nCntKey;							// �L�[�J�E���g

												// ���[�V�����J�E���g //
	for (nCntMotion = 0; nCntMotion < GIRAFFEMOSIONSET_MAX; nCntMotion++)
	{
		// ���[�V�����̏����ݒ�
		g_giraffeKey[nCntMotion].nNumKey = 0;		// �i���o�[�L�[
		g_giraffeKey[nCntMotion].nLoop = 0;		// ���[�v�̗L��

												// �����蔻��p //
		g_giraffeKey[nCntMotion].Collision_nParts = 0;		// �p�[�c�ԍ�
		g_giraffeKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
		g_giraffeKey[nCntMotion].Collision_Radius = 0.0f;	// �~�̔��a
		g_giraffeKey[nCntMotion].Collision_StartFram = 0;	// �J�n�t���[����
		g_giraffeKey[nCntMotion].Collision_EndFram = 0;		// �I���t���[����
		g_giraffeKey[nCntMotion].Collision_Damage = 0;		// �_���[�W��

															// �O�՗p //
		g_giraffeKey[nCntMotion].Orbit_nParts = 0;			// �p�[�c�ԍ�
		g_giraffeKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
		g_giraffeKey[nCntMotion].Orbit_Mesh = 0;			// �O�Ղ̐�
		g_giraffeKey[nCntMotion].Orbit_StartFram = 0;		// �J�n�t���[����
		g_giraffeKey[nCntMotion].Orbit_EndFram = 0;			// �I���t���[����
		g_giraffeKey[nCntMotion].Orbit_nType = 1;			// �O�Ղ̃^�C�v
		g_giraffeKey[nCntMotion].Orbit_nIdTrajectory = -1;	// �O�Ղ̔ԍ�
		g_giraffeKey[nCntMotion].Orbit_bUse = false;		// �O�Ղ̎g�p���

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

	// �L�����̏����ݒ� //
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < GIRAFFEMOSIONSET_MAX; nCntMotion++)
			{
				// �g�p���
				if (g_giraffeKey[nCntMotion].Orbit_bUse == true)
				{
					// �O�Ղ̐ݒ�
					g_giraffeKey[nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
						g_giraffeKey[nCntMotion].Orbit_Ofset,
						g_giraffeKey[nCntMotion].Orbit_nParts,
						g_giraffeKey[nCntMotion].Orbit_Mesh,
						g_giraffeKey[nCntMotion].Orbit_StartFram,
						g_giraffeKey[nCntMotion].Orbit_EndFram,
						g_giraffeKey[nCntMotion].Orbit_nType);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitGiraffe(void)
{
	// �L�����f�[�^�̊J��
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

	// �L�����e�N�X�`���̊J��
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
	// �J�E���g�L����
	int nCntGiraffe;
	PLAYER *pPlayer = GetPlayer();

	// �L�����̃J�E���g
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse)
		{
			// ���ݒn�ۑ�
			g_giraffe[nCntGiraffe].posold = g_giraffe[nCntGiraffe].pos;

			// ���[�V������ۑ�
			g_giraffe[nCntGiraffe].nMotionTypeOld = g_giraffe[nCntGiraffe].nMotionType;

			// ��ԏ���
			Update_StateGiraffe(nCntGiraffe);

			// ����ł��Ȃ����
			if (g_giraffe[nCntGiraffe].status.nStatus != GIRAFFESTATUS_DIE)
			{
				if (GetMode() != MODE_TUTORIAL)
				{
					// �U����ԂłȂ��Ƃ�
					if (g_giraffe[nCntGiraffe].status.bAttack == false)
					{
						// ���ˏ�ԂłȂ��Ƃ�
						if (g_giraffe[nCntGiraffe].status.bShot == false)
						{
							// �ړ���Ԃł͂Ȃ��Ƃ�
							if (g_giraffe[nCntGiraffe].status.bMove == false)
							{
								// ��]����
								Update_RotGiraffe(nCntGiraffe);
							}

							// �ړ���Ԃł͂Ȃ��Ƃ�
							if (g_giraffe[nCntGiraffe].status.bRot == false)
							{
								// �ړ�����
								Update_MoveGiraffe(nCntGiraffe);
							}
						}
					}

					// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
					if (g_giraffe[nCntGiraffe].status.bMove == false)
					{
						// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
						if (g_giraffe[nCntGiraffe].status.bRot == false)
						{
							if (g_giraffe[nCntGiraffe].status.bShot == false)
							{
								// ���ʍU���̏���
								Update_NormalAttackGiraffe(nCntGiraffe);
							}

							if (g_giraffe[nCntGiraffe].status.bAttack == false)
							{
								// �e�̏���
								Update_ShotGiraffe(nCntGiraffe);
							}
						}
					}
				}
				// �U���̓����蔻��
				Update_DecisionGiraffe(nCntGiraffe);

			}

			g_giraffe[nCntGiraffe].move.y -= 0.5f;
			g_giraffe[nCntGiraffe].pos.y += g_giraffe[nCntGiraffe].move.y;

			// �����蔻��
			Update_ColisionGiraffe(nCntGiraffe);

			// ����ł��Ȃ����
			if (g_giraffe[nCntGiraffe].status.nStatus != GIRAFFESTATUS_DIE)
			{
				// ���[�V��������
				Update_MosionGiraffe(nCntGiraffe);
			}

			// �e�̈ʒu��ݒ�
			SetPositionShadow(
				g_giraffe[nCntGiraffe].nIdxShadow,
				g_giraffe[nCntGiraffe].pos +
				D3DXVECTOR3(
					sinf(-g_giraffe[nCntGiraffe].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_giraffe[nCntGiraffe].rot.y) * 10.0f));

			// �U�����������Ă���
			if (g_giraffe[nCntGiraffe].status.bColition == true)
			{
				// �U�����I�������
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_giraffe[nCntGiraffe].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ��ԏ���
// ------------------------------------------------------------------------------------------
void Update_StateGiraffe(int nCntGiraffe)
{
	switch (g_giraffe[nCntGiraffe].status.nStatus)
	{
	case GIRAFFESTATUS_NEUTRAL:
		g_giraffe[nCntGiraffe].diffuse = 1.0f;
		break;

	case GIRAFFESTATUS_DAMAGE:
		// �J���[�̏�ԃJ�E���g�A�b�v
		g_giraffe[nCntGiraffe].nCntCor++;

		// �\�����
		if (g_giraffe[nCntGiraffe].nCntCor % 10 == 0)
		{
			g_giraffe[nCntGiraffe].diffuse = 1.0f;
		}

		// ��\�����
		else if (g_giraffe[nCntGiraffe].nCntCor % 5 == 0)
		{
			g_giraffe[nCntGiraffe].diffuse = 0.5f;
		}

		if (g_giraffe[nCntGiraffe].nCntCor == 50)
		{
			// �J���[�̏�ԃJ�E���g������
			g_giraffe[nCntGiraffe].nCntCor = 0;

			// �L�����̏�Ԃ��m�[�}����
			g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;
		}

		break;

	case GIRAFFESTATUS_DIE:
		// �ڕW��]�ʂ��猻�݂̉�]�ʂ̍���
		g_giraffe[nCntGiraffe].rotbetween.z =
			(g_giraffe[nCntGiraffe].rotLast.z - g_giraffe[nCntGiraffe].rot.z) / 10;

		// ��]
		g_giraffe[nCntGiraffe].rot.z += g_giraffe[nCntGiraffe].rotbetween.z;

		// �d��
		g_giraffe[nCntGiraffe].move.y -= 0.5f;	// �ړ���
		g_giraffe[nCntGiraffe].pos.y += g_giraffe[nCntGiraffe].move.y;

		// �X�e�[�^�X�̃J�E���g�A�b�v
		g_giraffe[nCntGiraffe].status.nCntStatus++;

		DeleteBillboard(BILLBOARDTYPE_ENTER);
		// �X�e�[�^�X�̃J�E���g���K��l�ȏ�ɂȂ�����
		if (g_giraffe[nCntGiraffe].status.nCntStatus == 70)
		{
			if (GetMode() == MODE_TUTORIAL)
			{
				// �o�b�t�@���[�o��
				SetBuffalo(
					g_giraffe[nCntGiraffe].pos,
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
						g_giraffe[nCntGiraffe].pos.x,
						g_giraffe[nCntGiraffe].pos.y,
						g_giraffe[nCntGiraffe].pos.z),

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
			g_giraffe[nCntGiraffe].bUse = false;
			g_giraffe[nCntGiraffe].bDisp = false;

			// �e�̏���
			DeleteShadow(g_giraffe[nCntGiraffe].nIdxShadow);

			// �\�����̏���
			DeletePrediction(g_giraffe[nCntGiraffe].nIdxPrediction);

			// �g�p���̋O�Ղ̏���
			DeleteTrajectory(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Orbit_nIdTrajectory);

			// �X�e�[�^�X�̃J�E���g�̏�����
			g_giraffe[nCntGiraffe].status.nCntStatus = 0;

			// �X�e�[�^�X��
			g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;

			// �A�C�e���o��
			SetItem(
				g_giraffe[nCntGiraffe].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ��]
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// �ړ���
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// �T�C�Y
				20.0f,								// ���a
				1,									// �e�N�X�`���[
				100);								// �o������

			// ���j��
			g_giraffeTotal++;

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
void Update_RotGiraffe(int nCntGiraffe)
{
	// �����]���ԍ�
	int nRandNum = 0;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_giraffe[nCntGiraffe].status.bRot == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 500;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �ړI�̉�]��
			g_giraffe[nCntGiraffe].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// ��]�ʂ�����l�𒴂�����
			if (g_giraffe[nCntGiraffe].rotLast.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotLast.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += D3DX_PI * 2;
			}

			// ��]���
			g_giraffe[nCntGiraffe].status.bRot = true;
		}
	}

	// �ړI��]�ʂ��猻�݉�]�ʂ̍���
	g_giraffe[nCntGiraffe].rotbetween.y = (g_giraffe[nCntGiraffe].rotLast.y - g_giraffe[nCntGiraffe].rot.y) * 0.01f;
	g_giraffe[nCntGiraffe].rot.y += g_giraffe[nCntGiraffe].rotbetween.y;

	// ��]��ԏI��
	if (g_giraffe[nCntGiraffe].rot.y >= -0.1f ||
		g_giraffe[nCntGiraffe].rot.y <= 0.1f)
	{
		g_giraffe[nCntGiraffe].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void Update_MoveGiraffe(int nCntGiraffe)
{
	// �����]���ԍ�
	int nRandNum = 0;
	int nRotNum = 0;
	float fRandMove;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_giraffe[nCntGiraffe].status.bMove == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 200;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �����_���ňړ��ʂ����߂�
			fRandMove = float(rand() % 100);

			// �ړI�ړ���
			g_giraffe[nCntGiraffe].posLast.x = 
				sinf(g_giraffe[nCntGiraffe].rot.y - D3DX_PI) * fRandMove + g_giraffe[nCntGiraffe].pos.x;

			g_giraffe[nCntGiraffe].posLast.z = 
				cosf(g_giraffe[nCntGiraffe].rot.y - D3DX_PI) * fRandMove + g_giraffe[nCntGiraffe].pos.z;
		
			// �ړ����
			g_giraffe[nCntGiraffe].status.bMove = true;
		}
	}

	// �ړ����
	if (g_giraffe[nCntGiraffe].status.bMove == true)
	{
		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_MOVE;
		// �ړI�ړ��ʂ��猻�݈ړ��ʂ̍���
		g_giraffe[nCntGiraffe].move.x = (g_giraffe[nCntGiraffe].posLast.x - g_giraffe[nCntGiraffe].pos.x) * 0.05f;
		g_giraffe[nCntGiraffe].move.z = (g_giraffe[nCntGiraffe].posLast.z - g_giraffe[nCntGiraffe].pos.z) * 0.05f;
	}

	// �ʒu���̍X�V
	// �d��
	g_giraffe[nCntGiraffe].move.y -= GIRAFFE_G;	// �ړ���
	g_giraffe[nCntGiraffe].pos += g_giraffe[nCntGiraffe].move;

	// �ړ���ԏI��
	if (g_giraffe[nCntGiraffe].move.x >= -0.1f && g_giraffe[nCntGiraffe].move.x <= 0.1f &&
		g_giraffe[nCntGiraffe].move.z >= -0.1f && g_giraffe[nCntGiraffe].move.z <= 0.1f)
	{
		g_giraffe[nCntGiraffe].status.bMove = false;

		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// ���[�V��������
// ------------------------------------------------------------------------------------------
void Update_MosionGiraffe(int nCntGiraffe)
{
	// �ϐ��錾
	int	nCntModel;			// �J�E���g�L����

		// ���݂̃��[�V�����ƑO��̃��[�V�������r
	if (g_giraffe[nCntGiraffe].nMotionType != g_giraffe[nCntGiraffe].nMotionTypeOld)
	{
		g_giraffe[nCntGiraffe].nFram = 0;
		g_giraffe[nCntGiraffe].nCntKeySet = 0;
		g_giraffe[nCntGiraffe].nCntDamage = 0;
	}

	for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		// �t���[�������O�̏ꍇ
		if (g_giraffe[nCntGiraffe].nFram == 0)
		{
			if (g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram != 0)
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween =
					(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].rot -
						g_giraffe[nCntGiraffe].model[nCntModel].rot) /
						(float)g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween =
					(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].pos -
						g_giraffe[nCntGiraffe].model[nCntModel].pos) /
						(float)g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram;
			}

			else
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_giraffe[nCntGiraffe].model[nCntModel].rot =
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].rot;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_giraffe[nCntGiraffe].model[nCntModel].pos =
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].pos;

				// �����̏�����
				g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �����̏�����
				g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// ��]����
		g_giraffe[nCntGiraffe].model[nCntModel].rot +=
			g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween;

		// �ړ�����
		g_giraffe[nCntGiraffe].model[nCntModel].pos +=
			g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween;
	}

	// �t���[�������K��l�Ɠ����ɂȂ�����
	if (g_giraffe[nCntGiraffe].nFram ==
		g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram)
	{
		// �L�[�Z�b�g�̃J�E���g�A�b�v
		g_giraffe[nCntGiraffe].nCntKeySet++;

		// �t���[����������
		g_giraffe[nCntGiraffe].nFram = 0;

		// �L�[�Z�b�g�����K��l�Ɠ����ɂȂ�����
		if (g_giraffe[nCntGiraffe].nCntKeySet == g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].nLoop == 0)
			{

				// �L�[�Z�b�g��������
				g_giraffe[nCntGiraffe].nCntKeySet = 0;

				g_giraffe[nCntGiraffe].nMotionType = 0;
			}

			// ���[�v���鎞
			else
			{
				// �L�[�Z�b�g��������
				g_giraffe[nCntGiraffe].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// �t���[�����J�E���g�A�b�v
		g_giraffe[nCntGiraffe].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// ���ʍU���̏���
// ------------------------------------------------------------------------------------------
void Update_NormalAttackGiraffe(int nCntGiraffe)
{
	// �ϐ��錾
	int	nAttackNum;				// �U���ԍ�
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƃL�����Ƃ̋���
	diffpos = pPlayerPos - g_giraffe[nCntGiraffe].pos;

	// �U���s���͈�
	if ((GIRAFFE_NORMALATTACK_DECISION * GIRAFFE_NORMALATTACK_DECISION) >=	// �U���s���̉~�͈�
		diffpos.x * diffpos.x +												// x���̉~
		diffpos.y * diffpos.y +												// y���̉~
		diffpos.z * diffpos.z)												// z���̉~
	{
		// �U���̊m��
		nAttackNum = rand() % GIRAFFE_FREQUENCY;

		// �U��
		if (nAttackNum == 0 &&
			g_giraffe[nCntGiraffe].status.bAttack == false)
		{
			PlaySound(SOUND_LABEL_SE_ATTACK);

			// �G�t�F�N�g����

			// ���ʍU�����[�V����
			g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NORMALATACK;

			// �O�Ղ̔���
			OccurrenceTrajectory(
				g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Orbit_nIdTrajectory,
				nCntGiraffe);

			// �U�����ON
			g_giraffe[nCntGiraffe].status.bAttack = true;

			// �o�b�t�@���[���猩�ăv���C���[������p�x
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// �ړI�̉�]��
			g_giraffe[nCntGiraffe].rotLast.y = fAngle - D3DX_PI;

			// ��]�ʂ�����l�𒴂�����
			if (g_giraffe[nCntGiraffe].rotLast.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotLast.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += D3DX_PI * 2;
			}

			// �ړI��]�ʂ��猻�݉�]�ʂ̍���
			g_giraffe[nCntGiraffe].rotbetween.y =
				(g_giraffe[nCntGiraffe].rotLast.y - g_giraffe[nCntGiraffe].rot.y) * 0.1f;

			g_giraffe[nCntGiraffe].rot.y += g_giraffe[nCntGiraffe].rotbetween.y;

			// ��]�ʂ�����l�𒴂�����
			if (g_giraffe[nCntGiraffe].rot.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rot.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rot.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rot.y += D3DX_PI * 2;
			}
		}
	}

	// ���[�V�������I�������
	if (g_giraffe[nCntGiraffe].status.bAttack == true &&
		g_giraffe[nCntGiraffe].nMotionType != GIRAFFEMOSIONSET_NORMALATACK)
	{
		// �U�����OFF
		g_giraffe[nCntGiraffe].status.bAttack = false;
	}
}

// ------------------------------------------------------------------------------------------
// �e����
// ------------------------------------------------------------------------------------------
void Update_ShotGiraffe(int nCntGiraffe)
{
	// �ϐ��錾
	int	nAttackNum;				// �U���ԍ�
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƃL�����Ƃ̋���
	diffpos = pPlayerPos - g_giraffe[nCntGiraffe].pos;

	// �U���s���͈�
	if ((GIRAFFE_SHOT_DECISION * GIRAFFE_SHOT_DECISION) >=	// �U���s���̉~�͈�
		diffpos.x * diffpos.x +								// x���̉~
		diffpos.y * diffpos.y +								// y���̉~
		diffpos.z * diffpos.z)								// z���̉~
	{
		// �U���̊m��
		nAttackNum = rand() % GIRAFFE_FREQUENCY;

		// �U��
		if (nAttackNum == 0 &&
			g_giraffe[nCntGiraffe].status.bShot == false)
		{
			// �v���C���[�̈ʒu���擾
			g_giraffe[nCntGiraffe].posAttack = GetPlayer()->pos;
			g_giraffe[nCntGiraffe].EffectCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_giraffe[nCntGiraffe].nCntAnim = 0;
			// �\�����̔���
			g_giraffe[nCntGiraffe].nIdxPrediction =
				SetPrediction(
					g_giraffe[nCntGiraffe].posAttack + D3DXVECTOR3(0.0f,20.0f,0.0f),
					D3DXVECTOR3(20.0f, 20.0f, 20.0f),
					D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

			// ���ʍU�����[�V����
			g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_SHOT;

			// �U�����ON
			g_giraffe[nCntGiraffe].status.bShot = true;
		}
	}

	// �e����
	if (g_giraffe[nCntGiraffe].nMotionType == GIRAFFEMOSIONSET_SHOT)
	{
		// ���˂���O
		if (g_giraffe[nCntGiraffe].nCntKeySet >= 0 &&
			g_giraffe[nCntGiraffe].nCntKeySet <= 2)
		{
			// �΂̂���̌���
			g_giraffe[nCntGiraffe].EffectCol.g -= 1.0f /
				(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[0].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[1].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[2].nFram);
			// �̂���̌���
			g_giraffe[nCntGiraffe].EffectCol.b -= 1.0f /
				(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[0].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[1].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[2].nFram);

			g_giraffe[nCntGiraffe].nCntAnim++;
			// �L�������猩�ăv���C���[������p�x
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// �ړI�̉�]��
			g_giraffe[nCntGiraffe].rotLast.y = fAngle - D3DX_PI;

			// ��]�ʂ�����l�𒴂�����
			if (g_giraffe[nCntGiraffe].rotLast.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotLast.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += D3DX_PI * 2;
			}

			// �ړI��]�ʂ��猻�݉�]�ʂ̍���
			g_giraffe[nCntGiraffe].rotbetween.y =
				(g_giraffe[nCntGiraffe].rotLast.y - g_giraffe[nCntGiraffe].rot.y);

			// ��]�ʂ�����l�𒴂�����
			if (g_giraffe[nCntGiraffe].rotbetween.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotbetween.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotbetween.y += D3DX_PI * 2;
			}

			g_giraffe[nCntGiraffe].rot.y += g_giraffe[nCntGiraffe].rotbetween.y * 0.1f;

			// ��]�ʂ�����l�𒴂�����
			if (g_giraffe[nCntGiraffe].rot.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rot.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rot.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rot.y += D3DX_PI * 2;
			}
			// �v���C���[�̈ʒu���擾
			g_giraffe[nCntGiraffe].posAttack = GetPlayer()->pos;
			// �\�����̈ʒu���ݒ�
			SetPositionPrediction(g_giraffe[nCntGiraffe].nIdxPrediction, g_giraffe[nCntGiraffe].posAttack + D3DXVECTOR3(0.0f, 20.0f, 0.0f));

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
						g_giraffe[nCntGiraffe].EffectCol.g,
						g_giraffe[nCntGiraffe].EffectCol.b,
						1.0f);

				// �G�t�F�N�g�̐ݒ�
				SetEffect(
					D3DXVECTOR3(													// �ʒu
						g_giraffe[nCntGiraffe].pos.x + sinf(fXZAngle) * 20.0f,
						0.0f,
						g_giraffe[nCntGiraffe].pos.z + cosf(fXZAngle) * 20.0f),
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
		else if (g_giraffe[nCntGiraffe].nCntKeySet == 3)
		{
			// ���ˏ�Ԃł͂Ȃ��ꍇ
			if (g_giraffe[nCntGiraffe].status.bShot == true)
			{
				g_giraffe[nCntGiraffe].nCntDamage++;
				// �Z�̔�������
				if (g_giraffe[nCntGiraffe].nCntDamage >= 20)
				{
					PlaySound(SOUND_LABEL_SE_BEAM);

					// �����ݒ�
					SetExplosion(
						g_giraffe[nCntGiraffe].posAttack,
						EXPLOSIONTYPE_THUNDER,
						50.0f,
						g_giraffe[nCntGiraffe].status.nAttack);
					// ���ˏ��
					g_giraffe[nCntGiraffe].status.bShot = false;
					g_giraffe[nCntGiraffe].nCntDamage = 0;
					// �\�����̍폜
					DeletePrediction(g_giraffe[nCntGiraffe].nIdxPrediction);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �U���̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_DecisionGiraffe(int nCntGiraffe)
{
	// �v���C���[���擾
	PLAYER * pPlayer = GetPlayer();

	// ��������
	if (g_giraffe[nCntGiraffe].status.nStatus != GIRAFFESTATUS_DIE &&			// �G�������Ă��邩�ǂ���
		g_giraffe[nCntGiraffe].nMotionType == GIRAFFEMOSIONSET_NORMALATACK &&	// �U����Ԃ��ǂ���
		pPlayer->status.bColition == false)										// �v���C���[�̍U�����󂯂Ă��邩�ǂ���
	{
		// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
		if ((pPlayer->status.fLength * pPlayer->status.fLength) +
			(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_Radius *
				g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_Radius) >=
				(pPlayer->pos.x - g_giraffe[nCntGiraffe].model[g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_nParts].mtxWorldModel._41) *
			(pPlayer->pos.x - g_giraffe[nCntGiraffe].model[g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_nParts].mtxWorldModel._41) +
			(pPlayer->pos.y - g_giraffe[nCntGiraffe].model[g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_nParts].mtxWorldModel._42) *
			(pPlayer->pos.y - g_giraffe[nCntGiraffe].model[g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_nParts].mtxWorldModel._42) +
			(pPlayer->pos.z - g_giraffe[nCntGiraffe].model[g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_nParts].mtxWorldModel._43) *
			(pPlayer->pos.z - g_giraffe[nCntGiraffe].model[g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_nParts].mtxWorldModel._43))
		{
			// �L�����̃_���[�W����
			HitPlayer(
				g_giraffe[nCntGiraffe].status.nAttack +
				g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_Damage);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionGiraffe(int nCntGiraffe)
{
	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_giraffe[nCntGiraffe].pos,
		&g_giraffe[nCntGiraffe].posold,
		&g_giraffe[nCntGiraffe].move,
		&g_giraffe[nCntGiraffe].model[0].size,
		&g_giraffe[nCntGiraffe].pMeshField))
	{

	}

	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_giraffe[nCntGiraffe].pos,
		&g_giraffe[nCntGiraffe].posold,
		&g_giraffe[nCntGiraffe].move,
		&g_giraffe[nCntGiraffe].model[0].size,
		&g_giraffe[nCntGiraffe].pMaterials) != -1)
	{
		// �ڕW�n�_�����ݒn�X��
		g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;

		// ���[�V���������R�֖̂߂�
		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NEUTRAL;
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_giraffe[nCntGiraffe].pos,
		&g_giraffe[nCntGiraffe].posold,
		&g_giraffe[nCntGiraffe].move,
		&g_giraffe[nCntGiraffe].model[0].size,
		&g_giraffe[nCntGiraffe].pWall))
	{
		// �ڕW�n�_�����ݒn�X��
		g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;

		// ���[�V���������R�֖̂߂�
		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NEUTRAL;
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
	int nCnt = 0;

	// �J�E���g�L����
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse)
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

			// �J�E���g�L����
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

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_giraffeModel[nCntModel].nNumMat; nCntMat++, pMat++,nCnt++)
				{
					// �{�̂̃J���[���X�V
					pMat->MatD3D.Diffuse.a = g_giraffe[nCntGiraffe].diffuse;

					// �e�N�X�`���[����
					if (pMat->pTextureFilename != NULL)
					{
						// �e�N�X�`���[�ݒ�
						pDevice->SetTexture(0, g_paTextureGiraffe[0]);
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
					g_giraffeModel[nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitGiraffe(int nCntGiraffe, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �U�����������Ă���
	g_giraffe[nCntGiraffe].status.bColition = true;

	// �J���[�̏�ԃJ�E���g������
	g_giraffe[nCntGiraffe].nCntCor = 0;

	// HP������
	g_giraffe[nCntGiraffe].status.nLife -= nDamage;

	// HP��0�ɂȂ�����
	if (g_giraffe[nCntGiraffe].status.nLife <= 0)
	{
		// ���S���
		g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_DIE;

		// �ڕW��]��
		g_giraffe[nCntGiraffe].rotLast.z = D3DX_PI / 2;

		// �ړ���
		g_giraffe[nCntGiraffe].move.y = 5.0f;

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
					g_giraffe[nCntGiraffe].pos.x,
					g_giraffe[nCntGiraffe].pos.y,
					g_giraffe[nCntGiraffe].pos.z),

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
		g_giraffe[nCntGiraffe].move.y = 5.0f;

		// �_���[�W���
		g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_DAMAGE;

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
					g_giraffe[nCntGiraffe].pos.x,
					g_giraffe[nCntGiraffe].pos.y,
					g_giraffe[nCntGiraffe].pos.z),
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
void ColisionGiraffe(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// �L�����L�����̓ǂݍ��ݏ���
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

				// �L�����Z�b�g��������
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// �G���h�L�����Z�b�g������܂Ń��[�v
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

							// �ʒu���ۑ�
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
					// �L�����̍X�V
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
void LoadGiraffe(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntGiraffe = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/TUTORIAL/GIRAFFE/giraffe.txt", "r");
	}

	// �{�X�p
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/GIRAFFE/giraffe.txt", "r");
	}

	// ����ȊO
	else
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GIRAFFE/giraffe.txt", "r");
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

				// �L�����Z�b�g��������
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// �G���h�L�����Z�b�g������܂Ń��[�v
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

							// �ړI�ʒu�ɕۑ�
							g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffe[nCntGiraffe].rot.x,
								&g_giraffe[nCntGiraffe].rot.y,
								&g_giraffe[nCntGiraffe].rot.z);

							// �e�̍쐬
							g_giraffe[nCntGiraffe].nIdxShadow = SetShadow(
								g_giraffe[nCntGiraffe].pos,
								D3DXVECTOR3(20.0f, 0.0f, 20.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}

					if (GetMode() == MODE_TUTORIAL)
					{
						SetBillboard(D3DXVECTOR3(
							g_giraffe[nCntGiraffe].pos.x,
							g_giraffe[nCntGiraffe].pos.y,
							g_giraffe[nCntGiraffe].pos.z),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							1);
					}

					// �g�p���
					g_giraffe[nCntGiraffe].bUse = true;

					// �\�����
					g_giraffe[nCntGiraffe].bDisp = true;

					// �L�����̍X�V
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
		MessageBox(NULL, "�L�����̃f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
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
	int	nCntModel = 0;							// �L�����J�E���g
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
					// �L�����̏�����
					nCntModel = 0;

					// �G���h�L�����Z�b�g������܂Ń��[�v
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

						// �����蔻����ǂݍ���
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie,
								&g_giraffeKey[nCntMotion].Collision_nParts,
								&g_giraffeKey[nCntMotion].Collision_Ofset.x,
								&g_giraffeKey[nCntMotion].Collision_Ofset.y,
								&g_giraffeKey[nCntMotion].Collision_Ofset.z,
								&g_giraffeKey[nCntMotion].Collision_Radius,
								&g_giraffeKey[nCntMotion].Collision_StartFram,
								&g_giraffeKey[nCntMotion].Collision_EndFram);
						}

						// �_���[�W���ǂݍ���
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_giraffeKey[nCntMotion].Collision_Damage);
						}

						// �O�Տ��ǂݍ���
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&g_giraffeKey[nCntMotion].Orbit_nParts,
								&g_giraffeKey[nCntMotion].Orbit_Ofset.x,
								&g_giraffeKey[nCntMotion].Orbit_Ofset.y,
								&g_giraffeKey[nCntMotion].Orbit_Ofset.z,
								&g_giraffeKey[nCntMotion].Orbit_Mesh,
								&g_giraffeKey[nCntMotion].Orbit_StartFram,
								&g_giraffeKey[nCntMotion].Orbit_EndFram);

							// �L�����N�^�[�^�C�v
							g_giraffeKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_GIRAFFE;

							// �g�p���
							g_giraffeKey[nCntMotion].Orbit_bUse = true;
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

							// �J�E���g�L����
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
		MessageBox(NULL, "�L�����̃��[�V�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �L������񏈗�
// ------------------------------------------------------------------------------------------
GIRAFFE *GetGiraffe(void)
{
	return &g_giraffe[0];
}


// ------------------------------------------------------------------------------------------
// �L�������j������
// ------------------------------------------------------------------------------------------
int GetGiraffeTotal(void)
{
	return g_giraffeTotal;
}

// ------------------------------------------------------------------------------------------
// �L�����ݒu����
// ------------------------------------------------------------------------------------------
void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �J�E���g�L����
	for (int nCntGiraffe = 0; nCntGiraffe < MAX_BUFFALO; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false)
		{
			// �ʒu���
			g_giraffe[nCntGiraffe].pos = pos;


			// �ړI�ʒu�ɕۑ�
			g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;

			// ��]���
			g_giraffe[nCntGiraffe].rot = rot;

			// �g�p���
			g_giraffe[nCntGiraffe].bUse = true;

			// �\�����
			g_giraffe[nCntGiraffe].bDisp = true;

			// �e�̍쐬
			g_giraffe[nCntGiraffe].nIdxShadow = SetShadow(
				g_giraffe[nCntGiraffe].pos,
				D3DXVECTOR3(20.0f, 0.0f, 20.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// �X�e�[�^�X���
			g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;
			g_giraffe[nCntGiraffe].status.nCntStatus = 0;
			g_giraffe[nCntGiraffe].status.nAttack = GIRAFFE_ATTACK;
			g_giraffe[nCntGiraffe].status.nLife = GIRAFFE_HP;
			g_giraffe[nCntGiraffe].status.bColition = false;
			g_giraffe[nCntGiraffe].status.bAttack = false;
			g_giraffe[nCntGiraffe].status.bShot = false;
			g_giraffe[nCntGiraffe].status.bRot = false;
			g_giraffe[nCntGiraffe].status.bMove = false;

			break;
		}
	}

}
