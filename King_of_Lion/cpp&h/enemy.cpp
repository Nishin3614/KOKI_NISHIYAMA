// ------------------------------------------------------------------------------------------
//
// �G���� [enemy.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "enemy.h"

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

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define	MAX_ENEMY (128)		// �ő吔
#define ENEMY_HP (30)
#define ENEMY_ATTACK (10)					// �U����
#define ENEMY_FREQUENCY (100)				// �U���p�x
#define ENEMY_NORMALATTACK_DECISION (100)
#define ENEMY_SHOT_DECISION (500)

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Init_PartsEnemy(void);
void Init_EnemyInfo(void);
void Init_MotionEnemy(void);
void Update_StateEnemy(int nCntEnemy);
void Update_RotEnemy(int nCntEnemy);
void Update_MoveEnemy(int nCntEnemy);
void Update_MosionEnemy(int nCntEnemy);
void Update_NormalAttackEnemy(int nCntEnemy);
void Update_ShotEnemy(int nCntEnemy);
void Update_DecisionEnemy(int nCntEnemy);
void Update_ColisionEnemy(int nCntEnemy);
void LoadEnemy(void);
void LoadEnemyMosion(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
ENEMY				g_enemy[MAX_ENEMY];						// �G���

ENEMYMODEL			g_enemyModel[ENEMY_TYPE][ENEMY_PARTS];			// �G���

MOSION_ENEMY		g_enemyKey[ENEMY_TYPE][ENEMY_MOTION];			// ���[�V�������

int					g_enemyTotal;								// �G���j�����v


// �G��p�ϐ�
ENEMYMODELINFO		g_enemyModelInfo[ENEMY_TYPE];						// �G�̊�{���

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitEnemy(void)
{
	g_enemyTotal = 0;
	// �G�p�[�c���
	Init_PartsEnemy();

	// �G���
	Init_EnemyInfo();

	// �G���[�V�������
	Init_MotionEnemy();
}

// ------------------------------------------------------------------------------------------
// �G�p�[�c��񏈗�
// ------------------------------------------------------------------------------------------
void Init_PartsEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	int					nCntModel;				// �G�̃J�E���g
	int					nCntType;				// �L�����N�^�[�̎��
	D3DXMATERIAL		*pMat;					// ���݂̃}�e���A���ۑ�

	// �L�����N�^�[�̏����� //
	for (nCntType = 0; nCntType < ENEMY_TYPE; nCntType++)
	{
		// �p�[�c�̏����� //
		for (nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
		{
			// ���������Ȃ���΂Ȃ�Ȃ�
			g_enemyModel[nCntType][nCntModel].paBuffMat = NULL;
			g_enemyModel[nCntType][nCntModel].paMesh = NULL;
			g_enemyModel[nCntType][nCntModel].nNumMat = 0;
			g_enemyModel[nCntType][nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// ��{�I�ɓ������
			g_enemyModel[nCntType][nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].nIdxModelParent = 0;
			g_enemyModel[nCntType][nCntModel].nFram = 0;
			g_enemyModel[nCntType][nCntModel].nTexType = 0;
			g_enemyModel[nCntType][nCntModel].nCntKeySet = 0;
		}

		// ���[�V�����̏�����
		for (int nCntMotion = 0; nCntMotion < ENEMY_MOTION; nCntMotion++)
		{
			// ���[�V�����̏����ݒ�
			g_enemyKey[nCntType][nCntMotion].nNumKey = 0;		// �i���o�[�L�[
			g_enemyKey[nCntType][nCntMotion].nLoop = 0;		// ���[�v�̗L��

															// �����蔻��p //
			g_enemyKey[nCntType][nCntMotion].Collision_nParts = 0;		// �p�[�c�ԍ�
			g_enemyKey[nCntType][nCntMotion].Collision_Ofset =
				D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
			g_enemyKey[nCntType][nCntMotion].Collision_Radius = 0.0f;	// �~�̔��a
			g_enemyKey[nCntType][nCntMotion].Collision_StartFram = 0;	// �J�n�t���[����
			g_enemyKey[nCntType][nCntMotion].Collision_EndFram = 0;		// �I���t���[����
			g_enemyKey[nCntType][nCntMotion].Collision_Damage = 0;		// �_���[�W��

																		// �O�՗p //
			g_enemyKey[nCntType][nCntMotion].Orbit_nParts = 0;			// �p�[�c�ԍ�
			g_enemyKey[nCntType][nCntMotion].Orbit_Ofset =
				D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �I�t�Z�b�g
			g_enemyKey[nCntType][nCntMotion].Orbit_Mesh = 0;			// �O�Ղ̐�
			g_enemyKey[nCntType][nCntMotion].Orbit_StartFram = 0;		// �J�n�t���[����
			g_enemyKey[nCntType][nCntMotion].Orbit_EndFram = 0;			// �I���t���[����
			g_enemyKey[nCntType][nCntMotion].Orbit_nType = 1;			// �O�Ղ̃^�C�v
			g_enemyKey[nCntType][nCntMotion].Orbit_nIdTrajectory = -1;	// �O�Ղ̔ԍ�
			g_enemyKey[nCntType][nCntMotion].Orbit_bUse = false;		// �O�Ղ̎g�p���

			for (int nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
			{
				g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

				// �L�[���Ƃɏ�����
				for (int nCntKey = 0; nCntKey < ENEMY_PARTS; nCntKey++)
				{
					g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	// �G�E���[�V�����̓ǂݍ���
	LoadEnemyMosion();

	// �L�����N�^�[�̓ǂݍ��� //
	for (nCntType = 0; nCntType < 1; nCntType++)
	{
		// �p�[�c�̓ǂݍ��� //
		for (nCntModel = 0; nCntModel < g_enemyModelInfo[nCntType].Num_Model; nCntModel++)
		{
			if (strcmp(&g_enemyModelInfo[nCntType].aFileModel[nCntModel][0], "") != 0)
			{

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				&g_enemyModelInfo[nCntType].aFileModel[nCntModel][0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_enemyModel[nCntType][nCntModel].paBuffMat,
				NULL,
				&g_enemyModel[nCntType][nCntModel].nNumMat,
				&g_enemyModel[nCntType][nCntModel].paMesh);

				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat =
					(D3DXMATERIAL*)g_enemyModel[nCntType][nCntModel].paBuffMat->GetBufferPointer();

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_enemyModel[nCntType][nCntModel].nNumMat; nCntMat++, pMat++)
				{
					// �e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(
						pDevice,
						pMat->pTextureFilename,
						&g_enemyModel[nCntType][nCntModel].paTextureEnemy[nCntMat]);
				}


				// �G�̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
				int nNumVertex;			// ���_��
				DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
				BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

										// ���_�����擾
				nNumVertex = g_enemyModel[nCntType][nCntModel].paMesh->GetNumVertices();

				// ���_�t�H�[�}�b�g�̃T�C�Y���擾
				sizeFVF = D3DXGetFVFVertexSize(g_enemyModel[nCntType][nCntModel].paMesh->GetFVF());

				// ���_�o�b�t�@�����b�N
				g_enemyModel[nCntType][nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

				// ���_�����J��Ԃ�
				for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
				{
					// �Ԓn����D3DXVE
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

					// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
					// x //
					// �ŏ��l
					if (g_enemyModel[nCntType][nCntModel].vtxMinMaterials.x > vtx.x)
					{
						g_enemyModel[nCntType][nCntModel].vtxMinMaterials.x = vtx.x;
					}

					// �ő�l
					if (g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.x < vtx.x)
					{
						g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.x = vtx.x;
					}

					// y //
					// �ŏ��l
					if (g_enemyModel[nCntType][nCntModel].vtxMinMaterials.y > vtx.y)
					{
						g_enemyModel[nCntType][nCntModel].vtxMinMaterials.y = vtx.y;
					}

					// �ő�l
					if (g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.y < vtx.y)
					{
						g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.y = vtx.y;
					}

					// z //
					// �ŏ��l
					if (g_enemyModel[nCntType][nCntModel].vtxMinMaterials.z > vtx.z)
					{
						g_enemyModel[nCntType][nCntModel].vtxMinMaterials.z = vtx.z;
					}

					// �ő�l
					if (g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.z < vtx.z)
					{
						g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.z = vtx.z;
					}

					// �T�C�Y�|�C���^��i�߂�
					pVertexBuffer += sizeFVF;
				}

				// �A�����b�N
				g_enemyModel[nCntType][nCntModel].paMesh->UnlockVertexBuffer();

				// �ő�l�ƍŏ��l�̋���
				g_enemyModel[nCntType][nCntModel].size = g_enemyModel[nCntType][nCntModel].vtxMaxMaterials - g_enemyModel[nCntType][nCntModel].vtxMinMaterials;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �G��񏈗�
// ------------------------------------------------------------------------------------------
void Init_EnemyInfo(void)
{
	// �ϐ��錾
	int					nCntEnemy;	// �v���C���[�J�E���g
	int					nCntModel;	// �G�̃J�E���g

	// �G�̏����ݒ� //
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_enemy[nCntEnemy].bUse == true)
			{
				DeleteShadow(g_enemy[nCntEnemy].nIdxShadow);
			}
		}

		g_enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_enemy[nCntEnemy].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].posBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].nMotionType = 0;
		g_enemy[nCntEnemy].nMotionTypeOld = 0;
		g_enemy[nCntEnemy].nParent = 0;
		g_enemy[nCntEnemy].nIndex = nCntEnemy;
		g_enemy[nCntEnemy].nFram = 0;
		g_enemy[nCntEnemy].nCntKeySet = 0;
		g_enemy[nCntEnemy].nIdxShadow = nCntEnemy;
		g_enemy[nCntEnemy].nType = 0;
		g_enemy[nCntEnemy].nTexType = 0;
		g_enemy[nCntEnemy].fLength = 50.0f;
		g_enemy[nCntEnemy].bUse = true;
		g_enemy[nCntEnemy].bDisp = true;

		// �J���[���
		g_enemy[nCntEnemy].nCntCor = 0;
		g_enemy[nCntEnemy].diffuse = 1.0f;

		// �X�e�[�^�X���
		g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_NEUTRAL;
		g_enemy[nCntEnemy].status.nCntStatus = 0;
		g_enemy[nCntEnemy].status.nAttack = ENEMY_ATTACK;
		g_enemy[nCntEnemy].status.nLife = ENEMY_HP;
		g_enemy[nCntEnemy].status.bColition = false;
		g_enemy[nCntEnemy].status.bAttack = false;
		g_enemy[nCntEnemy].status.bShot = false;
		g_enemy[nCntEnemy].status.bRot = false;
		g_enemy[nCntEnemy].status.bMove = false;
	}

	// ���[�h�G
	LoadEnemy();


	// �G�̏����ݒ� //
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �{�X���
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// �g�p���̉e������
			if (g_enemy[nCntEnemy].bUse == true)
			{
				DeleteShadow(g_enemy[nCntEnemy].nIdxShadow);
			}
		}

		// �p�[�c���Ƃ̃J�E���g
		for (nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
		{
			g_enemy[nCntEnemy].model[nCntModel].pos =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].pos;
			g_enemy[nCntEnemy].model[nCntModel].posLast =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].posLast;
			g_enemy[nCntEnemy].model[nCntModel].posKeyBetween =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].posKeyBetween;
			g_enemy[nCntEnemy].model[nCntModel].rot =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].rot;
			g_enemy[nCntEnemy].model[nCntModel].rotLast =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].rotLast;
			g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].rotKeyBetween;
			g_enemy[nCntEnemy].model[nCntModel].size =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].size;
			g_enemy[nCntEnemy].model[nCntModel].vtxMinMaterials =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].vtxMinMaterials;
			g_enemy[nCntEnemy].model[nCntModel].vtxMaxMaterials =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].vtxMaxMaterials;
			g_enemy[nCntEnemy].model[nCntModel].nIdxModelParent =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].nIdxModelParent;
			g_enemy[nCntEnemy].model[nCntModel].nFram =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].nFram;
			g_enemy[nCntEnemy].model[nCntModel].nCntKeySet =
				g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].nCntKeySet;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �G���[�V��������
// ------------------------------------------------------------------------------------------
void Init_MotionEnemy(void)
{
	// �ϐ��錾
	int		nCntEnemy;		// �G�J�E���g

	// ���[�V�����p�ϐ�
	int		nCntMotion;		// ���[�V�����J�E���g

	// �G�̏����ݒ� //
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true)
		{
			for (nCntMotion = 0; nCntMotion < ENEMY_MOTION; nCntMotion++)
			{
				// �g�p���
				if (g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_bUse == true)
				{
					// �O�Ղ̐ݒ�
					g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
						g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_Ofset,
						g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_nParts,
						g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_Mesh,
						g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_StartFram,
						g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_EndFram,
						g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_nType);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitEnemy(void)
{
	// �G�f�[�^�̊J��
	for (int nCntType = 0; nCntType < ENEMY_TYPE; nCntType++)
	{
		// �G�f�[�^�̊J��
		for (int nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
		{
			// ���b�V���̊J��
			if (g_enemyModel[nCntType][nCntModel].paMesh != NULL)
			{
				g_enemyModel[nCntType][nCntModel].paMesh->Release();
				g_enemyModel[nCntType][nCntModel].paMesh = NULL;
			}

			// �}�e���A���̊J��
			if (g_enemyModel[nCntType][nCntModel].paBuffMat != NULL)
			{
				g_enemyModel[nCntType][nCntModel].paBuffMat->Release();
				g_enemyModel[nCntType][nCntModel].paBuffMat = NULL;
			}

			// �G�e�N�X�`���̊J��
			for (int nCntMat = 0; nCntMat < ENEMY_MAT; nCntMat++)
			{
				// �e�N�X�`���[�̊J��
				if (g_enemyModel[nCntType][nCntModel].paTextureEnemy[nCntMat] != NULL)
				{
					g_enemyModel[nCntType][nCntModel].paTextureEnemy[nCntMat]->Release();
					g_enemyModel[nCntType][nCntModel].paTextureEnemy[nCntMat] = NULL;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	//// �J�E���g�G
	//int nCntEnemy;
	//PLAYER *pPlayer = GetPlayer();

	//// �G�̃J�E���g
	//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	if (g_enemy[nCntEnemy].bUse)
	//	{
	//		// ���ݒn�ۑ�
	//		g_enemy[nCntEnemy].posold = g_enemy[nCntEnemy].pos;

	//		// ���[�V������ۑ�
	//		g_enemy[nCntEnemy].nMotionTypeOld = g_enemy[nCntEnemy].nMotionType;

	//		// ��ԏ���
	//		Update_StateEnemy(nCntEnemy);

	//		if (GetMode() != MODE_TUTORIAL)
	//		{
	//			// ����ł��Ȃ����
	//			if (g_enemy[nCntEnemy].status.nStatus != ENEMYSTATUS_DIE)
	//			{
	//				// �U����ԂłȂ��Ƃ�
	//				if (g_enemy[nCntEnemy].status.bAttack == false)
	//				{
	//					// ���ˏ�ԂłȂ��Ƃ�
	//					if (g_enemy[nCntEnemy].status.bShot == false)
	//					{
	//						// �ړ���Ԃł͂Ȃ��Ƃ�
	//						if (g_enemy[nCntEnemy].status.bMove == false)
	//						{
	//							// ��]����
	//							Update_RotEnemy(nCntEnemy);
	//						}

	//						// �ړ���Ԃł͂Ȃ��Ƃ�
	//						if (g_enemy[nCntEnemy].status.bRot == false)
	//						{
	//							// �ړ�����
	//							Update_MoveEnemy(nCntEnemy);
	//						}
	//					}
	//				}

	//				// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
	//				if (g_enemy[nCntEnemy].status.bMove == false)
	//				{
	//					// �ړ��E��]��Ԃł͂Ȃ��Ƃ�
	//					if (g_enemy[nCntEnemy].status.bRot == false)
	//					{
	//						if (g_enemy[nCntEnemy].status.bShot == false)
	//						{
	//							// ���ʍU���̏���
	//							Update_NormalAttackEnemy(nCntEnemy);
	//						}

	//						if (g_enemy[nCntEnemy].status.bAttack == false)
	//						{
	//							if (GetMode() == MODE_GAME)
	//							{
	//								// �e�̏���
	//								Update_ShotEnemy(nCntEnemy);
	//							}
	//						}
	//					}
	//				}

	//				// �U���̓����蔻��
	//				Update_DecisionEnemy(nCntEnemy);
	//			}
	//		}

	//		// �����蔻��
	//		Update_ColisionEnemy(nCntEnemy);

	//		// ����ł��Ȃ����
	//		if (g_enemy[nCntEnemy].status.nStatus != ENEMYSTATUS_DIE)
	//		{
	//			// ���[�V��������
	//			Update_MosionEnemy(nCntEnemy);
	//		}

	//		// �e�̈ʒu��ݒ�
	//		SetPositionShadow(
	//			g_enemy[nCntEnemy].nIdxShadow, 
	//			g_enemy[nCntEnemy].pos +
	//			D3DXVECTOR3(
	//				sinf(-g_enemy[nCntEnemy].rot.y) * 10.0f,
	//				0.0f,
	//				cosf(D3DX_PI - g_enemy[nCntEnemy].rot.y) * 10.0f));

	//		// �U�����������Ă���
	//		if (g_enemy[nCntEnemy].status.bColition == true)
	//		{
	//			// �U�����I�������
	//			if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
	//			{
	//				g_enemy[nCntEnemy].status.bColition = false;
	//			}
	//		}
	//	}
	//}
}

// ------------------------------------------------------------------------------------------
// ��ԏ���
// ------------------------------------------------------------------------------------------
void Update_StateEnemy(int nCntEnemy)
{
	switch (g_enemy[nCntEnemy].status.nStatus)
	{
	case ENEMYSTATUS_NEUTRAL:
		g_enemy[nCntEnemy].diffuse = 1.0f;
		break;

	case ENEMYSTATUS_DAMAGE:
		// �J���[�̏�ԃJ�E���g�A�b�v
		g_enemy[nCntEnemy].nCntCor++;

		// �\�����
		if (g_enemy[nCntEnemy].nCntCor % 10 == 0)
		{
			g_enemy[nCntEnemy].diffuse = 1.0f;
		}

		// ��\�����
		else if (g_enemy[nCntEnemy].nCntCor % 5 == 0)
		{
			g_enemy[nCntEnemy].diffuse = 0.5f;
		}

		if (g_enemy[nCntEnemy].nCntCor == 50)
		{
			// �J���[�̏�ԃJ�E���g������
			g_enemy[nCntEnemy].nCntCor = 0;

			// �G�̏�Ԃ��m�[�}����
			g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_NEUTRAL;
		}

		break;

	case ENEMYSTATUS_DIE:
		// �ڕW��]�ʂ��猻�݂̉�]�ʂ̍���
		g_enemy[nCntEnemy].rotbetween.z =
			(g_enemy[nCntEnemy].rotLast.z - g_enemy[nCntEnemy].rot.z) / 10;

		// ��]
		g_enemy[nCntEnemy].rot.z += g_enemy[nCntEnemy].rotbetween.z;

		// �d��
		g_enemy[nCntEnemy].move.y -= 0.5f;	// �ړ���
		g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;

		// �X�e�[�^�X�̃J�E���g�A�b�v
		g_enemy[nCntEnemy].status.nCntStatus++;

		// �X�e�[�^�X�̃J�E���g���K��l�ȏ�ɂȂ�����
		if (g_enemy[nCntEnemy].status.nCntStatus == 70)
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
						g_enemy[nCntEnemy].pos.x,
						g_enemy[nCntEnemy].pos.y,
						g_enemy[nCntEnemy].pos.z),

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
			g_enemy[nCntEnemy].bUse = false;
			g_enemy[nCntEnemy].bDisp = false;

			// �e�̏���
			DeleteShadow(g_enemy[nCntEnemy].nIdxShadow);

			//// �g�p���̋O�Ղ̏���
			//DeleteTrajectory(g_enemyKey[g_enemy[nCntEnemy].nMotionType].Orbit_nIdTrajectory);

			// �X�e�[�^�X�̃J�E���g�̏�����
			g_enemy[nCntEnemy].status.nCntStatus = 0;

			// �X�e�[�^�X��
			g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_NEUTRAL;

			// �A�C�e���o��
			SetItem(
				g_enemy[nCntEnemy].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ��]
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// �ړ���
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// �T�C�Y
				20.0f,								// ���a
				1,									// �e�N�X�`���[
				100);								// �o������

			// ���j��
			g_enemyTotal++;

		}

		break;
	default:
		break;
	}

}

// ------------------------------------------------------------------------------------------
// ��]����
// ------------------------------------------------------------------------------------------
void Update_RotEnemy(int nCntEnemy)
{
	// �����]���ԍ�
	int nRandNum = 0;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_enemy[nCntEnemy].status.bRot == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 500;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �ړI�̉�]��
			g_enemy[nCntEnemy].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// ��]�ʂ�����l�𒴂�����
			if (g_enemy[nCntEnemy].rotLast.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotLast.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += D3DX_PI * 2;
			}

			// ��]���
			g_enemy[nCntEnemy].status.bRot = true;
		}
	}

	// �ړI��]�ʂ��猻�݉�]�ʂ̍���
	g_enemy[nCntEnemy].rotbetween.y = (g_enemy[nCntEnemy].rotLast.y - g_enemy[nCntEnemy].rot.y) * 0.01f;
	g_enemy[nCntEnemy].rot.y += g_enemy[nCntEnemy].rotbetween.y;

	// ��]��ԏI��
	if (g_enemy[nCntEnemy].rot.y >= -0.1f ||
		g_enemy[nCntEnemy].rot.y <= 0.1f)
	{
		g_enemy[nCntEnemy].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void Update_MoveEnemy(int nCntEnemy)
{
	// �����]���ԍ�
	int nRandNum = 0;
	int nRotNum = 0;
	float fRandMove;

	// �ړ���Ԃł͂Ȃ��Ƃ�
	if (g_enemy[nCntEnemy].status.bMove == false)
	{
		// �����_���Ő��l�����߂�
		nRandNum = rand() % 200;

		// �����]���ԍ���0�Ȃ�
		if (nRandNum == 0)
		{
			// �����_���ňړ��ʂ����߂�
			fRandMove = float(rand() % 100);

			// �ړI�ړ���
			g_enemy[nCntEnemy].posLast.x = 
				sinf(g_enemy[nCntEnemy].rot.y - D3DX_PI) * fRandMove + g_enemy[nCntEnemy].pos.x;

			g_enemy[nCntEnemy].posLast.z = 
				cosf(g_enemy[nCntEnemy].rot.y - D3DX_PI) * fRandMove + g_enemy[nCntEnemy].pos.z;
		
			// �ړ����
			g_enemy[nCntEnemy].status.bMove = true;
		}
	}

	// �ړI�ړ��ʂ��猻�݈ړ��ʂ̍���
	g_enemy[nCntEnemy].move.x = (g_enemy[nCntEnemy].posLast.x - g_enemy[nCntEnemy].pos.x) * 0.05f;
	g_enemy[nCntEnemy].move.z = (g_enemy[nCntEnemy].posLast.z - g_enemy[nCntEnemy].pos.z) * 0.05f;

	// �ʒu���̍X�V
	g_enemy[nCntEnemy].pos += g_enemy[nCntEnemy].move;

	// �ړ���ԏI��
	if (g_enemy[nCntEnemy].move.x >= -0.1f && g_enemy[nCntEnemy].move.x <= 0.1f &&
		g_enemy[nCntEnemy].move.z >= -0.1f && g_enemy[nCntEnemy].move.z <= 0.1f)
	{
		g_enemy[nCntEnemy].status.bMove = false;

		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NEUTRAL;
	}

	// �ړ����
	if (g_enemy[nCntEnemy].status.bMove == true)
	{
		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// ���[�V��������
// ------------------------------------------------------------------------------------------
void Update_MosionEnemy(int nCntEnemy)
{
	// �ϐ��錾
	int	nCntModel;			// �J�E���g�G

		// ���݂̃��[�V�����ƑO��̃��[�V�������r
	if (g_enemy[nCntEnemy].nMotionType != g_enemy[nCntEnemy].nMotionTypeOld)
	{

		g_enemy[nCntEnemy].nFram = 0;

		g_enemy[nCntEnemy].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
	{
		// �t���[�������O�̏ꍇ
		if (g_enemy[nCntEnemy].nFram == 0)
		{
			if (g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram != 0)
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween =
					(g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].rot -
						g_enemy[nCntEnemy].model[nCntModel].rot) /
						(float)g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_enemy[nCntEnemy].model[nCntModel].posKeyBetween =
					(g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].pos -
						g_enemy[nCntEnemy].model[nCntModel].pos) /
						(float)g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram;
			}

			else
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_enemy[nCntEnemy].model[nCntModel].rot =
					g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].rot;

				// �ŏI�n�_�ƌ��ݒn�_�̍���
				g_enemy[nCntEnemy].model[nCntModel].pos =
					g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].pos;

				// �����̏�����
				g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �����̏�����
				g_enemy[nCntEnemy].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// ��]����
		g_enemy[nCntEnemy].model[nCntModel].rot +=
			g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween;

		// �ړ�����
		g_enemy[nCntEnemy].model[nCntModel].pos +=
			g_enemy[nCntEnemy].model[nCntModel].posKeyBetween;
	}

	// �t���[�������K��l�Ɠ����ɂȂ�����
	if (g_enemy[nCntEnemy].nFram ==
		g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram)
	{
		// �L�[�Z�b�g�̃J�E���g�A�b�v
		g_enemy[nCntEnemy].nCntKeySet++;

		// �t���[����������
		g_enemy[nCntEnemy].nFram = 0;

		// �L�[�Z�b�g�����K��l�Ɠ����ɂȂ�����
		if (g_enemy[nCntEnemy].nCntKeySet == g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].nLoop == 0)
			{

				// �L�[�Z�b�g��������
				g_enemy[nCntEnemy].nCntKeySet = 0;

				g_enemy[nCntEnemy].nMotionType = 0;
			}

			// ���[�v���鎞
			else
			{
				// �L�[�Z�b�g��������
				g_enemy[nCntEnemy].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// �t���[�����J�E���g�A�b�v
		g_enemy[nCntEnemy].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// ���ʍU���̏���
// ------------------------------------------------------------------------------------------
void Update_NormalAttackEnemy(int nCntEnemy)
{
	// �ϐ��錾
	int	nAttackNum;				// �U���ԍ�
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƓG�Ƃ̋���
	diffpos = pPlayerPos - g_enemy[nCntEnemy].pos;

	// �U���s���͈�
	if ((ENEMY_NORMALATTACK_DECISION * ENEMY_NORMALATTACK_DECISION) >=	// �U���s���̉~�͈�
		diffpos.x * diffpos.x +												// x���̉~
		diffpos.y * diffpos.y +												// y���̉~
		diffpos.z * diffpos.z)												// z���̉~
	{
		// �U���̊m��
		nAttackNum = rand() % ENEMY_FREQUENCY;

		// �U��
		if (nAttackNum == 0 &&
			g_enemy[nCntEnemy].status.bAttack == false)
		{
			PlaySound(SOUND_LABEL_SE_ATTACK);
			// ���ʍU�����[�V����
			g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NORMALATACK;

			// �O�Ղ̔���
			OccurrenceTrajectory(
				g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Orbit_nIdTrajectory,
				nCntEnemy);

			// �U�����ON
			g_enemy[nCntEnemy].status.bAttack = true;

			// �o�b�t�@���[���猩�ăv���C���[������p�x
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// �ړI�̉�]��
			g_enemy[nCntEnemy].rotLast.y = fAngle - D3DX_PI;

			// ��]�ʂ�����l�𒴂�����
			if (g_enemy[nCntEnemy].rotLast.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotLast.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += D3DX_PI * 2;
			}

			// �ړI��]�ʂ��猻�݉�]�ʂ̍���
			g_enemy[nCntEnemy].rotbetween.y =
				(g_enemy[nCntEnemy].rotLast.y - g_enemy[nCntEnemy].rot.y) * 0.1f;

			g_enemy[nCntEnemy].rot.y += g_enemy[nCntEnemy].rotbetween.y;

			// ��]�ʂ�����l�𒴂�����
			if (g_enemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}
		}
	}

	// ���[�V�������I�������
	if (g_enemy[nCntEnemy].status.bAttack == true &&
		g_enemy[nCntEnemy].nMotionType != ENEMYMOSIONSET_NORMALATACK)
	{
		// �U�����OFF
		g_enemy[nCntEnemy].status.bAttack = false;
	}
}

// ------------------------------------------------------------------------------------------
// �e����
// ------------------------------------------------------------------------------------------
void Update_ShotEnemy(int nCntEnemy)
{
	// �ϐ��錾
	int	nAttackNum;				// �U���ԍ�
	D3DXVECTOR3 diffpos;		// �v���C���[�܂ł̋���
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// �v���C���[���擾

	// �v���C���[�ƓG�Ƃ̋���
	diffpos = pPlayerPos - g_enemy[nCntEnemy].pos;

	// �U���s���͈�
	if ((ENEMY_SHOT_DECISION * ENEMY_SHOT_DECISION) >=	// �U���s���̉~�͈�
		diffpos.x * diffpos.x +								// x���̉~
		diffpos.y * diffpos.y +								// y���̉~
		diffpos.z * diffpos.z)								// z���̉~
	{
		// �U���̊m��
		nAttackNum = rand() % ENEMY_FREQUENCY;

		// �U��
		if (nAttackNum == 0 &&
			g_enemy[nCntEnemy].status.bShot == false)
		{
			// ���ʍU�����[�V����
			g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_SHOT;

			// �U�����ON
			g_enemy[nCntEnemy].status.bShot = true;
		}
	}

	// �e����
	if (g_enemy[nCntEnemy].nMotionType == ENEMYMOSIONSET_SHOT)
	{
		// ���˂���O
		if (g_enemy[nCntEnemy].nCntKeySet >= 0 &&
			g_enemy[nCntEnemy].nCntKeySet <= 2)
		{
			// �G���猩�ăv���C���[������p�x
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// �ړI�̉�]��
			g_enemy[nCntEnemy].rotLast.y = fAngle - D3DX_PI;

			// ��]�ʂ�����l�𒴂�����
			if (g_enemy[nCntEnemy].rotLast.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotLast.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += D3DX_PI * 2;
			}

			// �ړI��]�ʂ��猻�݉�]�ʂ̍���
			g_enemy[nCntEnemy].rotbetween.y =
				(g_enemy[nCntEnemy].rotLast.y - g_enemy[nCntEnemy].rot.y);

			// ��]�ʂ�����l�𒴂�����
			if (g_enemy[nCntEnemy].rotbetween.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotbetween.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotbetween.y += D3DX_PI * 2;
			}

			g_enemy[nCntEnemy].rot.y += g_enemy[nCntEnemy].rotbetween.y * 0.1f;

			// ��]�ʂ�����l�𒴂�����
			if (g_enemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}
		}

		// ���˃^�C�~���O
		else if (g_enemy[nCntEnemy].nCntKeySet == 3)
		{
			// ���ˏ�Ԃł͂Ȃ��ꍇ
			if (g_enemy[nCntEnemy].status.bShot == true)
			{
				PlaySound(SOUND_LABEL_SE_BEAM);

				SetBullet(D3DXVECTOR3(
					g_enemy[nCntEnemy].model[4].mtxWorldModel._41,
					g_enemy[nCntEnemy].model[4].mtxWorldModel._42,
					g_enemy[nCntEnemy].model[4].mtxWorldModel._43),	// ���ˈʒu
					D3DXVECTOR3(4.0f, 0.0f, 4.0f),						// �ړ�
					g_enemy[nCntEnemy].rot,							// ��]
					D3DXVECTOR3(15.0f, 15.0f,15.0f),						// �T�C�Y
					200,												// HP
					1,													// �^�C�v
					g_enemy[nCntEnemy].status.nAttack);				// �U����

				// ���ˏ��
				g_enemy[nCntEnemy].status.bShot = false;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �U���̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_DecisionEnemy(int nCntEnemy)
{
	// �v���C���[���擾
	PLAYER * pPlayer = GetPlayer();

	// �G�������Ă��邩�ǂ���
	if (g_enemy[nCntEnemy].status.nStatus != ENEMYSTATUS_DIE)
	{
		// �U����Ԃ��ǂ���
		if (g_enemy[nCntEnemy].nMotionType == ENEMYMOSIONSET_NORMALATACK)
		{
			// �v���C���[�̍U�����󂯂Ă��邩�ǂ���
			if (pPlayer->status.bColition == false)
			{
				// �v���C���[�̍U�����̓G�ƃv���C���[�̓����蔻��
				if ((pPlayer->status.fLength * pPlayer->status.fLength) +
					(g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_Radius *
						g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_Radius) >=
						(pPlayer->pos.x - g_enemy[nCntEnemy].model[g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_nParts].mtxWorldModel._41) *
					(pPlayer->pos.x - g_enemy[nCntEnemy].model[g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_nParts].mtxWorldModel._41) +
					(pPlayer->pos.y - g_enemy[nCntEnemy].model[g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_nParts].mtxWorldModel._42) *
					(pPlayer->pos.y - g_enemy[nCntEnemy].model[g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_nParts].mtxWorldModel._42) +
					(pPlayer->pos.z - g_enemy[nCntEnemy].model[g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_nParts].mtxWorldModel._43) *
					(pPlayer->pos.z - g_enemy[nCntEnemy].model[g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_nParts].mtxWorldModel._43))
				{
					// �G�̃_���[�W����
					HitPlayer(
						g_enemy[nCntEnemy].status.nAttack +
						g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionEnemy(int nCntEnemy)
{
	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_enemy[nCntEnemy].pos,
		&g_enemy[nCntEnemy].posold,
		&g_enemy[nCntEnemy].move,
		&g_enemy[nCntEnemy].model[0].size,
		&g_enemy[nCntEnemy].pMeshField))
	{

	}

	// �f�ނ̓����蔻�� //
	if (ColisionMaterials(&g_enemy[nCntEnemy].pos,
		&g_enemy[nCntEnemy].posold,
		&g_enemy[nCntEnemy].move,
		&g_enemy[nCntEnemy].model[0].size,
		&g_enemy[nCntEnemy].pMaterials) != -1)
	{
		// �ڕW�n�_�����ݒn�X��
		g_enemy[nCntEnemy].posLast = g_enemy[nCntEnemy].pos;

		// ���[�V���������R�֖̂߂�
		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NEUTRAL;
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_enemy[nCntEnemy].pos,
		&g_enemy[nCntEnemy].posold,
		&g_enemy[nCntEnemy].move,
		&g_enemy[nCntEnemy].model[0].size,
		&g_enemy[nCntEnemy].pWall))
	{
		// �ڕW�n�_�����ݒn�X��
		g_enemy[nCntEnemy].posLast = g_enemy[nCntEnemy].pos;

		// ���[�V���������R�֖̂߂�
		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;				// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;				// �}�e���A���f�[�^�̃|�C���^
	int nCntEnemy;
	int nCntModel;

	// �J�E���g�G
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_enemy[nCntEnemy].mtxWorldEnemy);

			// ��]�𔽉f
			// �X�P�[���𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_enemy[nCntEnemy].rot.y, 
				g_enemy[nCntEnemy].rot.x, 
				g_enemy[nCntEnemy].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].mtxWorldEnemy,
				&g_enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_enemy[nCntEnemy].pos.x, 
				g_enemy[nCntEnemy].pos.y, 
				g_enemy[nCntEnemy].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].mtxWorldEnemy,	// 1
				&g_enemy[nCntEnemy].mtxWorldEnemy,					// 2
				&mtxTrans);									// 3

			// �J�E���g�G
			for (
				nCntModel = 0;
				nCntModel < g_enemyModelInfo[g_enemy[nCntEnemy].nType].Num_Model; 
				nCntModel++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_enemy[nCntEnemy].model[nCntModel].rot.y,
					g_enemy[nCntEnemy].model[nCntModel].rot.x, 
					g_enemy[nCntEnemy].model[nCntModel].rot.z);

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
					&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel, &mtxRot);

				// �ʒu�𔽉f //
				// ���s�ړ��s��쐬(�I�t�Z�b�g)
				D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
					g_enemy[nCntEnemy].model[nCntModel].pos.x,
					g_enemy[nCntEnemy].model[nCntModel].pos.y, 
					g_enemy[nCntEnemy].model[nCntModel].pos.z);	// ���ꂼ��̈ړ���

				// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,	// 1
					&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [���ׂĂ̐e]����Ȃ��ꍇ
				if (g_enemy[nCntEnemy].model[nCntModel].nIdxModelParent != -1)
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].model[g_enemy[nCntEnemy].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
					D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].mtxWorldEnemy);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);
				
				// �}�e���A�����ɑ΂���|�C���^���擾
				pMat = (D3DXMATERIAL*)g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].paBuffMat->GetBufferPointer();

				// �J�E���g�}�e���A��
				for (int nCntMat = 0; nCntMat < (int)g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].nNumMat; nCntMat++, pMat++)
				{
					// �{�̂̃J���[���X�V
					pMat->MatD3D.Diffuse.a = g_enemy[nCntEnemy].diffuse;

					// �e�N�X�`���[�ݒ�
					pDevice->SetTexture(
						0, 
						g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].paTextureEnemy[nCntMat]);

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat->MatD3D);

					// �`��
					g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitEnemy(int nCntEnemy, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �U�����������Ă���
	g_enemy[nCntEnemy].status.bColition = true;

	// �J���[�̏�ԃJ�E���g������
	g_enemy[nCntEnemy].nCntCor = 0;

	// HP������
	g_enemy[nCntEnemy].status.nLife -= nDamage;

	// HP��0�ɂȂ�����
	if (g_enemy[nCntEnemy].status.nLife <= 0)
	{
		// ���S���
		g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_DIE;

		// �ڕW��]��
		g_enemy[nCntEnemy].rotLast.z = D3DX_PI / 2;

		// �ړ���
		g_enemy[nCntEnemy].move.y = 5.0f;

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
					g_enemy[nCntEnemy].pos.x,
					g_enemy[nCntEnemy].pos.y,
					g_enemy[nCntEnemy].pos.z),

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
		g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_DAMAGE;

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
					g_enemy[nCntEnemy].pos.x,
					g_enemy[nCntEnemy].pos.y,
					g_enemy[nCntEnemy].pos.z),
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
// �G�̓����蔻�菈��
// ------------------------------------------------------------------------------------------
void ColisionEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// �G���̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadEnemy(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntEnemy = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

							// �`���[�g���A���p
	if (GetMode() == MODE_TUTORIAL)
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/TUTORIAL/ENEMY/enemy.txt", "r");
	}

	//// �{�X�p
	//else if (GetGameStete() == GAMESTATE_BOSS)
	//{
	//	// �t�@�C���J
	//	pFile = fopen("data/SAVE/GAME_BOSS/ENEMY/enemy.txt", "r");
	//}

	// ����ȊO
	else
	{
		// �t�@�C���J
		pFile = fopen("data/SAVE/ENEMY/enemy.txt", "r");
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

				// �G�Z�b�g��������
				else if (strcmp(cHeadText, "ENEMYSET") == 0)
				{

					// �G���h�G�Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_ENEMYSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ʒu���ǂݍ���
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_enemy[nCntEnemy].pos.x,
								&g_enemy[nCntEnemy].pos.y,
								&g_enemy[nCntEnemy].pos.z);

							// �ړI�ʒu�ɕۑ�
							g_enemy[nCntEnemy].posLast = g_enemy[nCntEnemy].pos;
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_enemy[nCntEnemy].rot.x,
								&g_enemy[nCntEnemy].rot.y,
								&g_enemy[nCntEnemy].rot.z);

							// �g�p���
							g_enemy[nCntEnemy].bUse = true;

							// �\�����
							g_enemy[nCntEnemy].bDisp = true;

							// �e�̍쐬
							g_enemy[nCntEnemy].nIdxShadow = SetShadow(
								g_enemy[nCntEnemy].pos,
								D3DXVECTOR3(20.0f, 0.0f, 20.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						}
					}

					if (GetMode() == MODE_TUTORIAL)
					{
						SetBillboard(D3DXVECTOR3(
							g_enemy[nCntEnemy].pos.x,
							g_enemy[nCntEnemy].pos.y,
							g_enemy[nCntEnemy].pos.z),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							1);

					}


					// �G�̍X�V
					nCntEnemy++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�G�̃f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �G���[�V�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadEnemyMosion(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int nCntType = 0;							// �^�C�v�J�E���g
	int nCntPartsFile = 0;						// x�t�@�C���ǂݍ��ݗp�J�E���g
	int	nCntMotion = 0;							// �v���C���[�J�E���g
	int	nCntModel = 0;							// �G�J�E���g
	int	nCntKeySet = 0;							// ���[�V�����J�E���g
	int	nCntKey = 0;							// �L�[�J�E���g
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/ENEMY/enemy_motion.txt", "r");

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
			//----------SCRIPT�`END_SCRIPT�܂�----------//
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// �G���̓ǂݍ���
				if (strcmp(cHeadText, "NUM_MODEL") == 0)
				{
					sscanf(cRaedText, "%s %s %d",
						&cDie, &cDie, 
						&g_enemyModelInfo[nCntType].Num_Model);
				}

				// �G�t�@�C�����̓ǂݍ���
				else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
				{
					sscanf(cRaedText, "%s %s %s",
						&cDie, &cDie,
						&g_enemyModelInfo[nCntType].aFileModel[nCntPartsFile]);

					// �J�E���g�A�b�v
					nCntPartsFile++;
				}

					// �X�N���v�g��������
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					//----------CHARACTERSET�`END_CHARACTERSET�܂�----------//
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ړ��ʓǂݍ���
						if (strcmp(cHeadText, "MOVE") == 0)
						{
							sscanf(cRaedText, "%s %s %f",
								&cDie, &cDie,
								&g_enemyModelInfo[nCntType].move);
						}

						// �����Փǂݍ���
						else if (strcmp(cHeadText, "JUMP") == 0)
						{
							sscanf(cRaedText, "%s %s %f",
								&cDie, &cDie,
								&g_enemyModelInfo[nCntType].fJump);
						}

						// ���a�ǂݍ���
						else if (strcmp(cHeadText, "RADIUS") == 0)
						{
							sscanf(cRaedText, "%s %s %f",
								&cDie, &cDie,
								&g_enemyModelInfo[nCntType].fRadius);
						}

						// �G�Z�b�g��������
						else if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							//----------PARTSSET�`END_PARTSSET�܂�----------//
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// �e���ǂݍ���
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cRaedText, "%s %s %d",
										&cDie, &cDie,
										&g_enemyModel[nCntType][nCntModel].nIdxModelParent);
								}

								// �ʒu���ǂݍ���
								else if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f",
										&cDie, &cDie,
										&g_enemyModel[nCntType][nCntModel].pos.x,
										&g_enemyModel[nCntType][nCntModel].pos.y,
										&g_enemyModel[nCntType][nCntModel].pos.z);

									// �ʒu���ۑ�
									g_enemyModel[nCntType][nCntModel].posOrigin = 
										g_enemyModel[nCntType][nCntModel].pos;
								}

								// ��]���ǂݍ���
								else if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&g_enemyModel[nCntType][nCntModel].rot.x,
										&g_enemyModel[nCntType][nCntModel].rot.y,
										&g_enemyModel[nCntType][nCntModel].rot.z);
								}
							}
							//----------PARTSSET�`END_PARTSSET�I��----------//

							// �G�̍X�V
							nCntModel++;
						}
					}
					//----------CHARACTERSET�`END_CHARACTERSET�I��----------//
				}
				
				// ���[�V�����Z�b�g��������
				if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// �G�̏�����
					nCntModel = 0;

					//----------MOTIONSET�`END_MOTIONSET�܂�----------//
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ���[�v���邩�ǂ������ǂݍ���
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", 
								&cDie, &cDie, 
								&g_enemyKey[nCntType][nCntMotion].nLoop);
						}

						// �L�[�����ǂݍ���
						else if (strcmp(cHeadText, "NUM_ENEMYKEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_enemyKey[nCntType][nCntMotion].nNumKey);
						}

						// �����蔻����ǂݍ���
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie,
								&g_enemyKey[nCntType][nCntMotion].Collision_nParts,
								&g_enemyKey[nCntType][nCntMotion].Collision_Ofset.x,
								&g_enemyKey[nCntType][nCntMotion].Collision_Ofset.y,
								&g_enemyKey[nCntType][nCntMotion].Collision_Ofset.z,
								&g_enemyKey[nCntType][nCntMotion].Collision_Radius,
								&g_enemyKey[nCntType][nCntMotion].Collision_StartFram,
								&g_enemyKey[nCntType][nCntMotion].Collision_EndFram);
						}

						// �_���[�W���ǂݍ���
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_enemyKey[nCntType][nCntMotion].Collision_Damage);
						}

						// �O�Տ��ǂݍ���
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&g_enemyKey[nCntType][nCntMotion].Orbit_nParts,
								&g_enemyKey[nCntType][nCntMotion].Orbit_Ofset.x,
								&g_enemyKey[nCntType][nCntMotion].Orbit_Ofset.y,
								&g_enemyKey[nCntType][nCntMotion].Orbit_Ofset.z,
								&g_enemyKey[nCntType][nCntMotion].Orbit_Mesh,
								&g_enemyKey[nCntType][nCntMotion].Orbit_StartFram,
								&g_enemyKey[nCntType][nCntMotion].Orbit_EndFram);

							// �L�����N�^�[�^�C�v
							g_enemyKey[nCntType][nCntMotion].Orbit_nType = TRAJECTORYTYPE_GIRAFFE;

							// �g�p���
							g_enemyKey[nCntType][nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].nFram);
							}

							// �L�[�̏�����
							nCntKey = 0;

							//----------KEYSET�`END_KEYSET�܂�----------//
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// �L�[���ǂݍ���
								if (strcmp(cHeadText, "KEY") == 0)
								{

									//----------KEY�`END_KEY�܂�----------//
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
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_enemy[0].model[nCntKey].pos;
										}

										// ��]���ǂݍ���
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}
									//----------KEY�`END_KEY�I��----------//

									// �J�E���g�L�[
									nCntKey++;
								}
							}
							//----------KEYSET�`END_KEYSET�I��----------//

							// �J�E���g�G
							nCntModel++;
						}
					}
					//----------MOTIONSET�`END_MOTIONSET�I��----------//

					// �J�E���g�v���C���[
					nCntMotion++;
				}
			}
			//----------SCRIPT�`END_SCRIPT�I��----------//
		}

		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�G�̃��[�V�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �G��񏈗�
// ------------------------------------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_enemy[0];
}


// ------------------------------------------------------------------------------------------
// �G���j������
// ------------------------------------------------------------------------------------------
int GetEnemyTotal(void)
{
	return g_enemyTotal;
}
