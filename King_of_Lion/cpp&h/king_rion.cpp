// ------------------------------------------------------------------------------------------
//
// �L�������� [king_rion.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "king_rion.h"
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
#include "score.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_KING_RION (1)
#define KING_RION_HP (100)
#define BUFFALO_ATTACK (20)					// �U����
#define KING_RION_NORMALATTACK_DECISION (100)
#define KING_RION_SHOT_DECISION (500)
#define KING_RION_ATTACK_COUNT (70)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Init_PartsKing_Rion(void);
void Init_King_RionInfo(void);
void Init_MotionKing_Rion(void);
void Update_StateKing_Rion(int nCntKing_Rion);
void Update_RotKing_Rion(int nCntKing_Rion);
void Update_MoveKing_Rion(int nCntKing_Rion);
void Update_MosionKing_Rion(int nCntKing_Rion);
void Update_NormalAttackKing_Rion(int nCntKing_Rion);
void Update_ShotKing_Rion(int nCntKing_Rion);
void Update_DecisionKing_Rion(int nCntKing_Rion);
void Update_ColisionKing_Rion(int nCntKing_Rion);
void LoadKing_RionModel(void);
void LoadKing_Rion(void);
void LoadKing_RionMosion(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureKing_Rion[TEXTURE_KING_RION] = {};	// �e�N�X�`���ւ̃|�C���^

KING_RION			g_king_rion[MAX_KING_RION];						// �S�b�̉����

MODEL				g_king_rionModel[KING_RIONTYPE_MAX];			// �L�������

MOSION_KING_RION	g_king_rionKey[KING_RIONMOSIONSET_MAX];			// ���[�V�������

int					g_king_rionTotal;								// �S�b�̉����j�����v

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitKing_Rion(void)
{
	g_king_rionTotal = 0;
	// �S�b�̉��p�[�c���
	Init_PartsKing_Rion();

	// �S�b�̉����
	Init_King_RionInfo();

	// �S�b�̉����[�V�������
	Init_MotionKing_Rion();
}

// ------------------------------------------------------------------------------------------
// �S�b�̉��p�[�c��񏈗�
// ------------------------------------------------------------------------------------------
void Init_PartsKing_Rion(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	int		nCntModel;							// �L�����̃J�E���g
		
	// �L�����t�@�C������`
	char *paKing_RionFile[KING_RIONTYPE_MAX] =
	{
		{ "data/MODEL/king_rion/00_rion_body.x" },				// [0]��
		{ "data/MODEL/king_rion/01_rion_head_joint.x" },		// [1]���֐�
		{ "data/MODEL/king_rion/02_rion_head.x" },				// [2]��
		{ "data/MODEL/king_rion/03_rion_right_hand_joint.x" },	// [3]�E�r�֐�
		{ "data/MODEL/king_rion/04_rion_right_hand.x" },		// [4]�E�r
		{ "data/MODEL/king_rion/05_rion_left_hand_joint.x" },	// [5]���r�֐�
		{ "data/MODEL/king_rion/06_rion_left_hand.x" },			// [6]���r
		{ "data/MODEL/king_rion/07_rion_right_leg_joint.x" },	// [7]�E���֐�
		{ "data/MODEL/king_rion/08_rion_right_leg.x" },			// [8]�E��
		{ "data/MODEL/king_rion/09_rion_left_leg_joint.x" },	// [9]�����֐�
		{ "data/MODEL/king_rion/10_rion_left_leg.x" },			// [10]����
		{ "data/MODEL/king_rion/11_rion_tail_joint.x" },		// [11]�����ۂ̊֐�
		{ "data/MODEL/king_rion/12_rion_tail.x" },				// [12]������
	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_KING_RION] =
	{
		{ "" },
	};

	// �p�[�c�̓ǂݍ��� //
	for (nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
	{
		g_king_rionModel[nCntModel].paBuffMat = NULL;
		g_king_rionModel[nCntModel].paMesh = NULL;
		g_king_rionModel[nCntModel].nNumMat = 0;
		g_king_rionModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rionModel[nCntModel].nIdxModelParent = 0;
		g_king_rionModel[nCntModel].nFram = 0;
		g_king_rionModel[nCntModel].nCntKeySet = 0;

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			paKing_RionFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_king_rionModel[nCntModel].paBuffMat,
			NULL,
			&g_king_rionModel[nCntModel].nNumMat,
			&g_king_rionModel[nCntModel].paMesh);

		// �L�����̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
		int nNumVertex;			// ���_��
		DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����擾
		nNumVertex = g_king_rionModel[nCntModel].paMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_king_rionModel[nCntModel].paMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_king_rionModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// ���_�����J��Ԃ�
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// �Ԓn����D3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
			// x //
			// �ŏ��l
			if (g_king_rionModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_king_rionModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// �ő�l
			if (g_king_rionModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_king_rionModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// �ŏ��l
			if (g_king_rionModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_king_rionModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// �ő�l
			if (g_king_rionModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_king_rionModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// �ŏ��l
			if (g_king_rionModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_king_rionModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// �ő�l
			if (g_king_rionModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_king_rionModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// �T�C�Y�|�C���^��i�߂�
			pVertexBuffer += sizeFVF;
		}

		// �A�����b�N
		g_king_rionModel[nCntModel].paMesh->UnlockVertexBuffer();

		// �ő�l�ƍŏ��l�̋���
		g_king_rionModel[nCntModel].size = g_king_rionModel[nCntModel].vtxMaxMaterials - g_king_rionModel[nCntModel].vtxMinMaterials;
	}

	// �p�[�c�Ǎ�
	LoadKing_RionModel();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureKing_Rion[0]);
}

// ------------------------------------------------------------------------------------------
// �S�b�̉���񏈗�
// ------------------------------------------------------------------------------------------
void Init_King_RionInfo(void)
{
	// �ϐ��錾
	int		nCntKing_Rion;	// �v���C���[�J�E���g
	int		nCntModel;		// �L�����̃J�E���g

	// �S�b�̉��̏����ݒ� //
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_king_rion[nCntKing_Rion].bUse == true)
			{
				DeleteShadow(g_king_rion[nCntKing_Rion].nIdxShadow);
			}
		}

		g_king_rion[nCntKing_Rion].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_king_rion[nCntKing_Rion].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].posBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_king_rion[nCntKing_Rion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_king_rion[nCntKing_Rion].nMotionType = 0;
		g_king_rion[nCntKing_Rion].nMotionTypeOld = 0;
		g_king_rion[nCntKing_Rion].nParent = 0;
		g_king_rion[nCntKing_Rion].nIndex = nCntKing_Rion;
		g_king_rion[nCntKing_Rion].nFram = 0;
		g_king_rion[nCntKing_Rion].nCntKeySet = 0;
		g_king_rion[nCntKing_Rion].nIdxShadow = nCntKing_Rion;
		g_king_rion[nCntKing_Rion].fLength = 50.0f;
		g_king_rion[nCntKing_Rion].bUse = false;
		g_king_rion[nCntKing_Rion].bDisp = false;

		// �J���[���
		g_king_rion[nCntKing_Rion].nCntCor = 0;
		g_king_rion[nCntKing_Rion].diffuse = 1.0f;

		// �X�e�[�^�X���
		g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_NEUTRAL;
		g_king_rion[nCntKing_Rion].status.nCntStatus = 0;
		g_king_rion[nCntKing_Rion].status.nAttack = 10;
		g_king_rion[nCntKing_Rion].status.nLife = KING_RION_HP;
		g_king_rion[nCntKing_Rion].status.bColition = false;
		g_king_rion[nCntKing_Rion].status.bAttack = false;
		g_king_rion[nCntKing_Rion].status.bShot = false;
		g_king_rion[nCntKing_Rion].status.bRot = false;
		g_king_rion[nCntKing_Rion].status.bMove = false;

		// �p�[�c���Ƃ̃J�E���g
		for (nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
		{
			g_king_rion[nCntKing_Rion].model[nCntModel].pos = g_king_rionModel[nCntModel].pos;
			g_king_rion[nCntKing_Rion].model[nCntModel].posLast = g_king_rionModel[nCntModel].posLast;
			g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween = g_king_rionModel[nCntModel].posKeyBetween;
			g_king_rion[nCntKing_Rion].model[nCntModel].rot = g_king_rionModel[nCntModel].rot;
			g_king_rion[nCntKing_Rion].model[nCntModel].rotLast = g_king_rionModel[nCntModel].rotLast;
			g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween = g_king_rionModel[nCntModel].rotKeyBetween;
			g_king_rion[nCntKing_Rion].model[nCntModel].size = g_king_rionModel[nCntModel].size;
			g_king_rion[nCntKing_Rion].model[nCntModel].vtxMinMaterials = g_king_rionModel[nCntModel].vtxMinMaterials;
			g_king_rion[nCntKing_Rion].model[nCntModel].vtxMaxMaterials = g_king_rionModel[nCntModel].vtxMaxMaterials;
			g_king_rion[nCntKing_Rion].model[nCntModel].nIdxModelParent = g_king_rionModel[nCntModel].nIdxModelParent;
			g_king_rion[nCntKing_Rion].model[nCntModel].nFram = g_king_rionModel[nCntModel].nFram;
			g_king_rion[nCntKing_Rion].model[nCntModel].nCntKeySet = g_king_rionModel[nCntModel].nCntKeySet;
		}
	}

	// ���[�h�S�b�̉�
	LoadKing_Rion();
}

// ------------------------------------------------------------------------------------------
// �S�b�̉����[�V��������
// ------------------------------------------------------------------------------------------
void Init_MotionKing_Rion(void)
{
	// �ϐ��錾
	int		nCntKing_Rion;						// �S�b�̉��J�E���g

												// ���[�V�����p�ϐ�
	int		nCntMotion;							// ���[�V�����J�E���g
	int		nCntKeySet;							// ���[�V�����J�E���g
	int		nCntKey;							// �L�[�J�E���g

												// ���[�V�����J�E���g //
	for (nCntMotion = 0; nCntMotion < KING_RIONMOSIONSET_MAX; nCntMotion++)
	{
		// ���[�V�����̏����ݒ�
		g_king_rionKey[nCntMotion].nNumKey = 0;		// �i���o�[�L�[
		g_king_rionKey[nCntMotion].nLoop = 0;		// ���[�v�̗L��

												// �����蔻��p //
		g_king_rionKey[nCntMotion].Collision_nParts = 0;		// �p�[�c�ԍ�
		g_king_rionKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �I�t�Z�b�g
		g_king_rionKey[nCntMotion].Collision_Radius = 0.0f;		// �~�̔��a
		g_king_rionKey[nCntMotion].Collision_StartFram = 0;		// �J�n�t���[����
		g_king_rionKey[nCntMotion].Collision_EndFram = 0;		// �I���t���[����
		g_king_rionKey[nCntMotion].Collision_Damage = 0;		// �_���[�W��

		// �O�՗p //
		g_king_rionKey[nCntMotion].Orbit_nParts = 0;			// �p�[�c�ԍ�
		g_king_rionKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �I�t�Z�b�g
		g_king_rionKey[nCntMotion].Orbit_Mesh = 0;				// �O�Ղ̐�
		g_king_rionKey[nCntMotion].Orbit_StartFram = 0;			// �J�n�t���[����
		g_king_rionKey[nCntMotion].Orbit_EndFram = 0;			// �I���t���[����
		g_king_rionKey[nCntMotion].Orbit_nType =
			TRAJECTORYTYPE_KING_RION;							// �O�Ղ̃^�C�v
		g_king_rionKey[nCntMotion].Orbit_nIdTrajectory = -1;	// �O�Ղ̔ԍ�
		g_king_rionKey[nCntMotion].Orbit_bUse = false;			// �O�Ղ̎g�p���

		for (nCntKeySet = 0; nCntKeySet < KING_RION_KEYSET_MAX; nCntKeySet++)
		{
			g_king_rionKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// �L�[���Ƃɏ�����
			for (nCntKey = 0; nCntKey < KING_RIONTYPE_MAX; nCntKey++)
			{
				g_king_rionKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_king_rionKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// �v���C���[���[�V�����ǂݍ���
	LoadKing_RionMosion();

	// �S�b�̉��̏����ݒ� //
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		if (g_king_rion[nCntKing_Rion].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < KING_RIONMOSIONSET_MAX; nCntMotion++)
			{
				// �g�p���
				if (g_king_rionKey[nCntMotion].Orbit_bUse == true)
				{
					// �O�Ղ̐ݒ�
					g_king_rionKey[nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
						g_king_rionKey[nCntMotion].Orbit_Ofset,
						g_king_rionKey[nCntMotion].Orbit_nParts,
						g_king_rionKey[nCntMotion].Orbit_Mesh,
						g_king_rionKey[nCntMotion].Orbit_StartFram,
						g_king_rionKey[nCntMotion].Orbit_EndFram,
						g_king_rionKey[nCntMotion].Orbit_nType);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitKing_Rion(void)
{
	// �L�����f�[�^�̊J��
	for (int nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
	{
		// ���b�V���̊J��
		if (g_king_rionModel[nCntModel].paMesh != NULL)
		{
			g_king_rionModel[nCntModel].paMesh->Release();
			g_king_rionModel[nCntModel].paMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_king_rionModel[nCntModel].paBuffMat != NULL)
		{
			g_king_rionModel[nCntModel].paBuffMat->Release();
			g_king_rionModel[nCntModel].paBuffMat = NULL;
		}
	}

	// �L�����e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < TEXTURE_KING_RION; nCntTexture++)
	{
		// �e�N�X�`���[�̊J��
		if (g_paTextureKing_Rion[nCntTexture] != NULL)
		{
			g_paTextureKing_Rion[nCntTexture]->Release();
			g_paTextureKing_Rion[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateKing_Rion(void)
{
	// �J�E���g�S�b�̉�
	int nCntKing_Rion;
	PLAYER *pPlayer = GetPlayer();

	// �ϐ��錾
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �S�b�̉��̃J�E���g
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		if (g_king_rion[nCntKing_Rion].bUse)
		{
			// �v���C���[�ƕS�b�̉��Ƃ̋���
			diffpos = pPlayerPos - g_king_rion[nCntKing_Rion].pos;

			// ���ݒn�ۑ�
			g_king_rion[nCntKing_Rion].posold = g_king_rion[nCntKing_Rion].pos;

			// ���[�V������ۑ�
			g_king_rion[nCntKing_Rion].nMotionTypeOld = g_king_rion[nCntKing_Rion].nMotionType;

			// ��ԏ���
			Update_StateKing_Rion(nCntKing_Rion);

			// ����ł��Ȃ����
			if (g_king_rion[nCntKing_Rion].status.nStatus != KING_RIONSTATUS_DIE)
			{
				// �U���s���͈�
				if ((KING_RION_NORMALATTACK_DECISION * KING_RION_NORMALATTACK_DECISION) >=	// �U���s���̉~�͈�
					diffpos.x * diffpos.x +												// x���̉~
					diffpos.y * diffpos.y +												// y���̉~
					diffpos.z * diffpos.z)												// z���̉~
				{
					// ���ʍU���̏���
					Update_NormalAttackKing_Rion(nCntKing_Rion);
				}

				// �v���C���[�ǐ�
				else
				{
					if (g_king_rion[nCntKing_Rion].status.nStatus !=
						KING_RIONMOSIONSET_SHOT)
					{
						// ��]����
						Update_RotKing_Rion(nCntKing_Rion);

						// �ړ�����
						Update_MoveKing_Rion(nCntKing_Rion);
					}

					// �e�̏���
					Update_ShotKing_Rion(nCntKing_Rion);
				}

				// �U���̓����蔻��
				Update_DecisionKing_Rion(nCntKing_Rion);
			}
			g_king_rion[nCntKing_Rion].move.y -= 0.5f;
			g_king_rion[nCntKing_Rion].pos.y += g_king_rion[nCntKing_Rion].move.y;

			// �����蔻��
			Update_ColisionKing_Rion(nCntKing_Rion);

			// ����ł��Ȃ����
			if (g_king_rion[nCntKing_Rion].status.nStatus != KING_RIONSTATUS_DIE)
			{
				// ���[�V��������
				Update_MosionKing_Rion(nCntKing_Rion);
			}

			// �e�̈ʒu��ݒ�
			SetPositionShadow(
				g_king_rion[nCntKing_Rion].nIdxShadow, 
				g_king_rion[nCntKing_Rion].pos +
				D3DXVECTOR3(
					sinf(-g_king_rion[nCntKing_Rion].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_king_rion[nCntKing_Rion].rot.y) * 10.0f));

			// �U�����������Ă���
			if (g_king_rion[nCntKing_Rion].status.bColition == true)
			{
				// �U�����I�������
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_king_rion[nCntKing_Rion].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ��ԏ���
// ------------------------------------------------------------------------------------------
void Update_StateKing_Rion(int nCntKing_Rion)
{
	switch (g_king_rion[nCntKing_Rion].status.nStatus)
	{
	case KING_RIONSTATUS_NEUTRAL:
		g_king_rion[nCntKing_Rion].diffuse = 1.0f;
		break;

	case KING_RIONSTATUS_DAMAGE:
		// �J���[�̏�ԃJ�E���g�A�b�v
		g_king_rion[nCntKing_Rion].nCntCor++;

		// �\�����
		if (g_king_rion[nCntKing_Rion].nCntCor % 10 == 0)
		{
			g_king_rion[nCntKing_Rion].diffuse = 1.0f;
		}

		// ��\�����
		else if (g_king_rion[nCntKing_Rion].nCntCor % 5 == 0)
		{
			g_king_rion[nCntKing_Rion].diffuse = 0.5f;
		}

		if (g_king_rion[nCntKing_Rion].nCntCor == 50)
		{
			// �J���[�̏�ԃJ�E���g������
			g_king_rion[nCntKing_Rion].nCntCor = 0;

			// �S�b�̉��̏�Ԃ��m�[�}����
			g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_NEUTRAL;
		}

		break;

	case KING_RIONSTATUS_DIE:
		// �ڕW��]�ʂ��猻�݂̉�]�ʂ̍���
		g_king_rion[nCntKing_Rion].rotbetween.z =
			(g_king_rion[nCntKing_Rion].rotLast.z - g_king_rion[nCntKing_Rion].rot.z) / 10;

		// ��]
		g_king_rion[nCntKing_Rion].rot.z += g_king_rion[nCntKing_Rion].rotbetween.z;

		// �d��
		g_king_rion[nCntKing_Rion].move.y -= 0.5f;	// �ړ���
		g_king_rion[nCntKing_Rion].pos.y += g_king_rion[nCntKing_Rion].move.y;

		// �X�e�[�^�X�̃J�E���g�A�b�v
		g_king_rion[nCntKing_Rion].status.nCntStatus++;

		// �X�e�[�^�X�̃J�E���g���K��l�ȏ�ɂȂ�����
		if (g_king_rion[nCntKing_Rion].status.nCntStatus == 70)
		{
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
						g_king_rion[nCntKing_Rion].pos.x,
						g_king_rion[nCntKing_Rion].pos.y,
						g_king_rion[nCntKing_Rion].pos.z),

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
			g_king_rion[nCntKing_Rion].bUse = false;
			g_king_rion[nCntKing_Rion].bDisp = false;

			// �e�̏���
			DeleteShadow(g_king_rion[nCntKing_Rion].nIdxShadow);

			// �g�p���̋O�Ղ̏���
			DeleteTrajectory(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Orbit_nIdTrajectory);

			// �X�e�[�^�X�̃J�E���g�̏�����
			g_king_rion[nCntKing_Rion].status.nCntStatus = 0;

			// �X�e�[�^�X��
			g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_NEUTRAL;

			// �A�C�e���o��
			SetItem(
				g_king_rion[nCntKing_Rion].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ��]
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// �ړ���
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// �T�C�Y
				20.0f,								// ���a
				0,									// �e�N�X�`���[
				100);								// �o������

			// ���j��
			g_king_rionTotal++;

			// �Q�[��������
			if (GetMode() == MODE_GAME)
			{
				AddScore(1);
			}

			// �Q�[���N���A��Ԃ�
			SetGameState(GAMESTATE_CLEAR);
		}

		break;
	default:
		break;
	}

}

// ------------------------------------------------------------------------------------------
// ��]����
// ------------------------------------------------------------------------------------------
void Update_RotKing_Rion(int nCntKing_Rion)
{
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƕS�b�̉��Ƃ̋���
	diffpos = pPlayerPos - g_king_rion[nCntKing_Rion].pos;

	// �S�b�̉����猩�ăv���C���[������p�x
	float	fAngle = (atan2f(diffpos.x, diffpos.z));

	// �ړI�̉�]��
	g_king_rion[nCntKing_Rion].rotLast.y = fAngle + D3DX_PI;

	// ��]�ʂ�����l�𒴂�����
	if (g_king_rion[nCntKing_Rion].rotLast.y > D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotLast.y += -D3DX_PI * 2;
	}
	else if (g_king_rion[nCntKing_Rion].rotLast.y < -D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotLast.y += D3DX_PI * 2;
	}

	// �ړI��]�ʂ��猻�݉�]�ʂ̍���
	g_king_rion[nCntKing_Rion].rotbetween.y =
		(g_king_rion[nCntKing_Rion].rotLast.y - g_king_rion[nCntKing_Rion].rot.y);

	// ��]�ʂ�����l�𒴂�����
	if (g_king_rion[nCntKing_Rion].rotbetween.y > D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotbetween.y += -D3DX_PI * 2;
	}
	else if (g_king_rion[nCntKing_Rion].rotbetween.y < -D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotbetween.y += D3DX_PI * 2;
	}

	g_king_rion[nCntKing_Rion].rot.y += g_king_rion[nCntKing_Rion].rotbetween.y * 0.1f;

	// ��]�ʂ�����l�𒴂�����
	if (g_king_rion[nCntKing_Rion].rot.y > D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rot.y += -D3DX_PI * 2;
	}
	else if (g_king_rion[nCntKing_Rion].rot.y < -D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rot.y += D3DX_PI * 2;
	}
}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void Update_MoveKing_Rion(int nCntKing_Rion)
{
	// �ړ����
	g_king_rion[nCntKing_Rion].status.bMove = true;

	// �ړI�ړ��ʂ��猻�݈ړ��ʂ̍���
	g_king_rion[nCntKing_Rion].pos.x += 
		g_king_rion[nCntKing_Rion].move.x * sinf(g_king_rion[nCntKing_Rion].rot.y - D3DX_PI);
	g_king_rion[nCntKing_Rion].pos.z += 
		g_king_rion[nCntKing_Rion].move.z * cosf(g_king_rion[nCntKing_Rion].rot.y - D3DX_PI);

	// �ړ���ԏI��
		//g_king_rion[nCntKing_Rion].status.bMove = false;

		//g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;

	// �ړ����
	if (g_king_rion[nCntKing_Rion].status.bMove == true)
	{
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// ���ʍU���̏���
// ------------------------------------------------------------------------------------------
void Update_NormalAttackKing_Rion(int nCntKing_Rion)
{
	// �U���J�E���g�A�b�v
	g_king_rion[nCntKing_Rion].status.nCntAttack++;

	// ���R���[�V����
	if (g_king_rion[nCntKing_Rion].status.nCntAttack <= KING_RION_ATTACK_COUNT)
	{
		// ���R�̃��[�V����
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
	}

	// �U�����[�V����
	else
	{
		// �G�t�F�N�g����


		PlaySound(SOUND_LABEL_SE_BOSSVOICE);

		// ���ʍU�����[�V����
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NORMALATACK;

		// �O�Ղ̔���
		OccurrenceTrajectory(
			g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Orbit_nIdTrajectory,
			nCntKing_Rion);
	}
}

// ------------------------------------------------------------------------------------------
// �e����
// ------------------------------------------------------------------------------------------
void Update_ShotKing_Rion(int nCntKing_Rion)
{
	// �ϐ��錾
	int	nAttackNum;				// �U���ԍ�

	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƕS�b�̉��Ƃ̋���
	diffpos = pPlayerPos - g_king_rion[nCntKing_Rion].pos;

	// �U���̊m��
	nAttackNum = rand() % 1000;

	// �U�����[�V�����؂�ւ�
	if (nAttackNum == 0)
	{
		// �G�t�F�N�g����


		// ���ʍU�����[�V����
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_SHOT;
	}

	// �e����
	if (g_king_rion[nCntKing_Rion].nMotionType == KING_RIONMOSIONSET_SHOT)
	{
		// ���˂���O
		if (g_king_rion[nCntKing_Rion].nCntKeySet >= 0 &&
			g_king_rion[nCntKing_Rion].nCntKeySet <= 2)
		{
			// �S�b�̉����猩�ăv���C���[������p�x
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// �ړI�̉�]��
			g_king_rion[nCntKing_Rion].rotLast.y = fAngle - D3DX_PI;

			// ��]�ʂ�����l�𒴂�����
			if (g_king_rion[nCntKing_Rion].rotLast.y > D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_king_rion[nCntKing_Rion].rotLast.y < -D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotLast.y += D3DX_PI * 2;
			}

			// �ړI��]�ʂ��猻�݉�]�ʂ̍���
			g_king_rion[nCntKing_Rion].rotbetween.y =
				(g_king_rion[nCntKing_Rion].rotLast.y - g_king_rion[nCntKing_Rion].rot.y);

			// ��]�ʂ�����l�𒴂�����
			if (g_king_rion[nCntKing_Rion].rotbetween.y > D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_king_rion[nCntKing_Rion].rotbetween.y < -D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotbetween.y += D3DX_PI * 2;
			}

			g_king_rion[nCntKing_Rion].rot.y += g_king_rion[nCntKing_Rion].rotbetween.y * 0.1f;

			// ��]�ʂ�����l�𒴂�����
			if (g_king_rion[nCntKing_Rion].rot.y > D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rot.y += -D3DX_PI * 2;
			}
			else if (g_king_rion[nCntKing_Rion].rot.y < -D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rot.y += D3DX_PI * 2;
			}
		}

		// ���˃^�C�~���O
		else if (g_king_rion[nCntKing_Rion].nCntKeySet == 3)
		{
			// �t���[�������O
			if (g_king_rion[nCntKing_Rion].nFram == 0)
			{
				PlaySound(SOUND_LABEL_SE_BOSSVOICE);

				SetBullet(D3DXVECTOR3(
					g_king_rion[nCntKing_Rion].model[4].mtxWorldModel._41,
					g_king_rion[nCntKing_Rion].model[4].mtxWorldModel._42,
					g_king_rion[nCntKing_Rion].model[4].mtxWorldModel._43),	// ���ˈʒu
					D3DXVECTOR3(1.0f, 0.0f, 1.0f),							// �ړ�
					g_king_rion[nCntKing_Rion].rot,							// ��]
					D3DXVECTOR3(10.0f, 10.0f, 10.0f),						// �T�C�Y
					120,													// HP
					2,														// �^�C�v
					g_king_rion[nCntKing_Rion].status.nAttack);				// �U����
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���[�V��������
// ------------------------------------------------------------------------------------------
void Update_MosionKing_Rion(int nCntKing_Rion)
{
	// �ϐ��錾
	int	nCntModel;			// �J�E���g�L����

		// ���݂̃��[�V�����ƑO��̃��[�V�������r
	if (g_king_rion[nCntKing_Rion].nMotionType != g_king_rion[nCntKing_Rion].nMotionTypeOld)
	{

		g_king_rion[nCntKing_Rion].nFram = 0;

		g_king_rion[nCntKing_Rion].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
	{
		// �t���[�������O�̏ꍇ
		if (g_king_rion[nCntKing_Rion].nFram == 0)
		{
			if (g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram != 0)
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween =
					(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].rot -
						g_king_rion[nCntKing_Rion].model[nCntModel].rot) /
						(float)g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween =
					(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].pos -
						g_king_rion[nCntKing_Rion].model[nCntModel].pos) /
						(float)g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram;
			}

			else
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_king_rion[nCntKing_Rion].model[nCntModel].rot =
					g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].rot;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_king_rion[nCntKing_Rion].model[nCntModel].pos =
					g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].pos;

				// �����̏�����
				g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �����̏�����
				g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// ��]����
		g_king_rion[nCntKing_Rion].model[nCntModel].rot +=
			g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween;

		// �ړ�����
		g_king_rion[nCntKing_Rion].model[nCntModel].pos +=
			g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween;
	}

	// �t���[�������K��l�Ɠ����ɂȂ�����
	if (g_king_rion[nCntKing_Rion].nFram ==
		g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram)
	{
		// �L�[�Z�b�g�̃J�E���g�A�b�v
		g_king_rion[nCntKing_Rion].nCntKeySet++;

		// �t���[����������
		g_king_rion[nCntKing_Rion].nFram = 0;

		// �L�[�Z�b�g�����K��l�Ɠ����ɂȂ�����
		if (g_king_rion[nCntKing_Rion].nCntKeySet == g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].nLoop == 0)
			{

				// �L�[�Z�b�g��������
				g_king_rion[nCntKing_Rion].nCntKeySet = 0;

				// �m�[�}���U�����[�V����
				if (g_king_rion[nCntKing_Rion].nMotionType == KING_RIONMOSIONSET_NORMALATACK)
				{
					// �U���J�E���g�̏�����
					g_king_rion[nCntKing_Rion].status.nCntAttack = 0;
				}

				g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
			}

			// ���[�v���鎞
			else
			{
				// �L�[�Z�b�g��������
				g_king_rion[nCntKing_Rion].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// �t���[�����J�E���g�A�b�v
		g_king_rion[nCntKing_Rion].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// �U���̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_DecisionKing_Rion(int nCntKing_Rion)
{
	// �v���C���[���擾
	PLAYER * pPlayer = GetPlayer();

	// �G�������Ă��邩�ǂ���
	if (g_king_rion[nCntKing_Rion].status.nStatus != KING_RIONSTATUS_DIE)
	{
		// �U����Ԃ��ǂ���
		if (g_king_rion[nCntKing_Rion].nMotionType == KING_RIONMOSIONSET_NORMALATACK)
		{
			// �v���C���[�̍U�����󂯂Ă��邩�ǂ���
			if (pPlayer->status.bColition == false)
			{
				// �v���C���[�̍U�����̕S�b�̉��ƃv���C���[�̓����蔻��
				if ((pPlayer->status.fLength * pPlayer->status.fLength) +
					(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_Radius * g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_Radius) >=
					(pPlayer->pos.x - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._41) *
					(pPlayer->pos.x - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._41) +
					(pPlayer->pos.y - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._42) *
					(pPlayer->pos.y - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._42) +
					(pPlayer->pos.z - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._43) *
					(pPlayer->pos.z - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._43))
				{
					// �S�b�̉��̃_���[�W����
					HitPlayer(
						g_king_rion[nCntKing_Rion].status.nAttack +
						g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionKing_Rion(int nCntKing_Rion)
{
	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_king_rion[nCntKing_Rion].pos,
		&g_king_rion[nCntKing_Rion].posold,
		&g_king_rion[nCntKing_Rion].move,
		&g_king_rion[nCntKing_Rion].model[0].size,
		&g_king_rion[nCntKing_Rion].pMeshField))
	{

	}

	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_king_rion[nCntKing_Rion].pos,
		&g_king_rion[nCntKing_Rion].posold,
		&g_king_rion[nCntKing_Rion].move,
		&g_king_rion[nCntKing_Rion].model[0].size,
		&g_king_rion[nCntKing_Rion].pMaterials) != -1)
	{
		// �ڕW�n�_�����ݒn�X��
		g_king_rion[nCntKing_Rion].posLast = g_king_rion[nCntKing_Rion].pos;

		// ���[�V���������R�֖̂߂�
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_king_rion[nCntKing_Rion].pos,
		&g_king_rion[nCntKing_Rion].posold,
		&g_king_rion[nCntKing_Rion].move,
		&g_king_rion[nCntKing_Rion].model[0].size,
		&g_king_rion[nCntKing_Rion].pWall))
	{
		// �ڕW�n�_�����ݒn�X��
		g_king_rion[nCntKing_Rion].posLast = g_king_rion[nCntKing_Rion].pos;

		// ���[�V���������R�֖̂߂�
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawKing_Rion(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;				// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;				// �}�e���A���f�[�^�̃|�C���^
	int nCntKing_Rion;
	int nCntModel;
	int nCnt = 0;

	// �J�E���g�L����
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		if (g_king_rion[nCntKing_Rion].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion);

			// ��]�𔽉f
			// �X�P�[���𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_king_rion[nCntKing_Rion].rot.y, 
				g_king_rion[nCntKing_Rion].rot.x, 
				g_king_rion[nCntKing_Rion].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion,
				&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion, &mtxRot);


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_king_rion[nCntKing_Rion].pos.x, 
				g_king_rion[nCntKing_Rion].pos.y, 
				g_king_rion[nCntKing_Rion].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion,	// 1
				&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion,					// 2
				&mtxTrans);									// 3

			// �J�E���g�L����
			for (nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_king_rion[nCntKing_Rion].model[nCntModel].rot.y,
					g_king_rion[nCntKing_Rion].model[nCntModel].rot.x, 
					g_king_rion[nCntKing_Rion].model[nCntModel].rot.z);

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
				D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
					&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel, &mtxRot);

				// �ʒu�𔽉f //
				// ���s�ړ��s��쐬(�I�t�Z�b�g)
				D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
					g_king_rion[nCntKing_Rion].model[nCntModel].pos.x,
					g_king_rion[nCntKing_Rion].model[nCntModel].pos.y, 
					g_king_rion[nCntKing_Rion].model[nCntModel].pos.z);	// ���ꂼ��̈ړ���

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
				D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,	// 1
					&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [���ׂĂ̐e]����Ȃ��ꍇ
				if (g_king_rion[nCntKing_Rion].model[nCntModel].nIdxModelParent != -1)
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].model[g_king_rion[nCntKing_Rion].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)g_king_rionModel[nCntModel].paBuffMat->GetBufferPointer();

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_king_rionModel[nCntModel].nNumMat; nCntMat++, pMat++,nCnt++)
				{
					// �{�̂̃J���[���X�V
					pMat->MatD3D.Diffuse.a = g_king_rion[nCntKing_Rion].diffuse;

					// �e�N�X�`���[����
					if (pMat->pTextureFilename != NULL)
					{
						// �e�N�X�`���[�ݒ�
						pDevice->SetTexture(0, g_paTextureKing_Rion[0]);
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
					g_king_rionModel[nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitKing_Rion(int nCntKing_Rion, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �U�����������Ă���
	g_king_rion[nCntKing_Rion].status.bColition = true;

	// �J���[�̏�ԃJ�E���g������
	g_king_rion[nCntKing_Rion].nCntCor = 0;

	// HP������
	g_king_rion[nCntKing_Rion].status.nLife -= nDamage;

	// HP��0�ɂȂ�����
	if (g_king_rion[nCntKing_Rion].status.nLife <= 0)
	{
		// ���S���
		g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_DIE;

		// �ڕW��]��
		g_king_rion[nCntKing_Rion].rotLast.z = D3DX_PI / 2;

		// �ړ���
		g_king_rion[nCntKing_Rion].move.y = 10.0f;

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
					g_king_rion[nCntKing_Rion].pos.x,
					g_king_rion[nCntKing_Rion].pos.y,
					g_king_rion[nCntKing_Rion].pos.z),

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
		g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_DAMAGE;

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
					g_king_rion[nCntKing_Rion].pos.x,
					g_king_rion[nCntKing_Rion].pos.y,
					g_king_rion[nCntKing_Rion].pos.z),
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
// �S�b�̉��̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void ColisionKing_Rion(
	D3DXVECTOR3 * pos,		// �ʒu
	D3DXVECTOR3 * posOld, 	// �O��̈ʒu
	D3DXVECTOR3 * move,		// �ړ���
	D3DXVECTOR3 * size		// �T�C�Y
)
{
}

// ------------------------------------------------------------------------------------------
// �S�b�̉��L�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadKing_RionModel(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/KING_RION/king_rion_model.txt", "r");

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

				// �L�����Z�b�g��������
				if (strcmp(cHeadText, "KING_RIONSET") == 0)
				{

					// �G���h�L�����Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_KING_RIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_king_rionModel[nCntModel].nIdxModelParent);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_king_rionModel[nCntModel].pos.x,
								&g_king_rionModel[nCntModel].pos.y, 
								&g_king_rionModel[nCntModel].pos.z);

							// �ʒu���ۑ�
							g_king_rionModel[nCntModel].posOrigin = g_king_rionModel[nCntModel].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_king_rionModel[nCntModel].rot.x, 
								&g_king_rionModel[nCntModel].rot.y,
								&g_king_rionModel[nCntModel].rot.z);
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
		MessageBox(NULL, "�S�b�̉��̃p�[�c�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �S�b�̉����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadKing_Rion(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntKing_Rion = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �{�X�p
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/GAME_BOSS/KING_RION/king_rion.txt", "r");

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
					else if (strcmp(cHeadText, "KING_RIONSET") == 0)
					{

						// �G���h�L�����Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_KING_RIONSET") != 0)
						{
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �ʒu���ǂݍ���
							if (strcmp(cHeadText, "POS") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f",
									&cDie, &cDie,
									&g_king_rion[nCntKing_Rion].pos.x,
									&g_king_rion[nCntKing_Rion].pos.y,
									&g_king_rion[nCntKing_Rion].pos.z);

								// �ړI�ʒu�ɕۑ�
								g_king_rion[nCntKing_Rion].posLast = g_king_rion[nCntKing_Rion].pos;
							}

							// ��]���ǂݍ���
							else if (strcmp(cHeadText, "ROT") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
									&g_king_rion[nCntKing_Rion].rot.x,
									&g_king_rion[nCntKing_Rion].rot.y,
									&g_king_rion[nCntKing_Rion].rot.z);

								// �g�p���
								g_king_rion[nCntKing_Rion].bUse = true;

								// �\�����
								g_king_rion[nCntKing_Rion].bDisp = true;

								// �e�̍쐬
								g_king_rion[nCntKing_Rion].nIdxShadow = SetShadow(
									g_king_rion[nCntKing_Rion].pos,
									D3DXVECTOR3(20.0f, 0.0f, 20.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
						// �L�����̍X�V
						nCntKing_Rion++;
					}
				}
			}
			// �t�@�C����
			fclose(pFile);
		}

		// �J���Ȃ�
		else
		{
			MessageBox(NULL, "�S�b�̉��̃f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �S�b�̉����[�V�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadKing_RionMosion(void)
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
	pFile = fopen("data/SAVE/KING_RION/king_rion_motion.txt", "r");

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
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_king_rionKey[nCntMotion].nLoop);
						}

						// �L�[�����ǂݍ���
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_king_rionKey[nCntMotion].nNumKey);
						}

						// �����蔻����ǂݍ���
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie,
								&g_king_rionKey[nCntMotion].Collision_nParts,
								&g_king_rionKey[nCntMotion].Collision_Ofset.x,
								&g_king_rionKey[nCntMotion].Collision_Ofset.y,
								&g_king_rionKey[nCntMotion].Collision_Ofset.z,
								&g_king_rionKey[nCntMotion].Collision_Radius,
								&g_king_rionKey[nCntMotion].Collision_StartFram,
								&g_king_rionKey[nCntMotion].Collision_EndFram);
						}

						// �_���[�W���ǂݍ���
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_king_rionKey[nCntMotion].Collision_Damage);
						}

						// �O�Տ��ǂݍ���
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&g_king_rionKey[nCntMotion].Orbit_nParts,
								&g_king_rionKey[nCntMotion].Orbit_Ofset.x,
								&g_king_rionKey[nCntMotion].Orbit_Ofset.y,
								&g_king_rionKey[nCntMotion].Orbit_Ofset.z,
								&g_king_rionKey[nCntMotion].Orbit_Mesh,
								&g_king_rionKey[nCntMotion].Orbit_StartFram,
								&g_king_rionKey[nCntMotion].Orbit_EndFram);

							// �L�����N�^�[�^�C�v
							g_king_rionKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_KING_RION;

							// �g�p���
							g_king_rionKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_king_rionKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// �����̈ʒu��񔽉f
											g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_king_rion[0].model[nCntKey].pos;
										}

										// ��]���ǂݍ���
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
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

					// �J�E���g�v���C���[-
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
		MessageBox(NULL, "�S�b�̉��̃��[�V�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �S�b�̉���񏈗�
// ------------------------------------------------------------------------------------------
KING_RION *GetKing_Rion(void)
{
	return &g_king_rion[0];
}

// ------------------------------------------------------------------------------------------
// �S�b�̉��̃L������񏈗�
// ------------------------------------------------------------------------------------------
MODEL * GetKing_Rion_Model(int nCntKing_Rion)
{
	return &g_king_rion[nCntKing_Rion].model[0];
}

// ------------------------------------------------------------------------------------------
// �S�b�̉����j������
// ------------------------------------------------------------------------------------------
int GetKing_RionTotal(void)
{
	return g_king_rionTotal;
}
