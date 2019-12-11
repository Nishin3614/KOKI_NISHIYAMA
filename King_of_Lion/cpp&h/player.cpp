// ------------------------------------------------------------------------------------------
//
// ���f������ [player.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "player.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "shogira.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "materials.h"
#include "hpgauge.h"
#include "trajectory.h"
#include "effect.h"
#include "sound.h"
#include "Billboard.h"
#include "joypad.h"
#include "dome.h"

// ���̃w�b�_�[
#include "giraffe.h"
#include "buffalo.h"
#include "game.h"
#include "king_rion.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TEXTURE_PLAYER (1)
#define PLAYER_ATTACK (5)
#define PLAYER_MPDAMAGE (10)
#define PLAYER_KEYMOVE (2.0f)	// �L�[�{�[�h�ړ���
#define PLAYER_JOYMOVE (4.0f)	// �W���C�p�b�h�ړ���
#define PLAYER_G (0.5f)			// �d��
#define PLAYER_RESISTANCE (0.5f)// ��R��

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
void Update_StatePlayer(void);
void Update_RotPlayer(void);
void Update_MovePlayer(void);
void Update_MotionPlayer(void);
void Update_ShotPlayer(void);
void Update_NormalAttackPlayer(void);
void Update_ColisionPlayer(void);
void LoadPlayer(void);
void LoadPlayerMotion(void);

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTexturePlayer[TEXTURE_PLAYER] = {};	// �e�N�X�`���ւ̃|�C���^

PLAYER				g_player;				// ���f�����

MOSION_PLAYER		g_playerKey[PLAYERMOSIONSET_MAX];		// ���[�V�������

int					g_nNumType = 0;							// �i���o�[�^�C�v

int					g_nPlayerMotionOld = 0;					// ���[�V�����^�C�v

int					g_nIdxShadow = 0;						// �e��ID

bool				g_bStop = false;						// �|�[�Y

bool				g_bMove;								// �ړ����Ă��邩�ǂ���

bool				g_bJump;								// �W�����v���Ă��邩�ǂ���

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitPlayer(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// �f�o�C�X�̎擾
	CAMERA	*pCamera = GetCamera();				// �J�����̎擾
	int		nCntModel;							// ���f���̃J�E���g

	// ���[�V�����p�ϐ�
	int		nCntMotion;							// ���[�V�����J�E���g
	int		nCntKeySet;							// ���[�V�����J�E���g
	int		nCntKey;							// �L�[�J�E���g

	// ���f���t�@�C������`
	char *paPlayerFile[PLAYERTYPE_MAX] =
	{
		{ "data/MODEL/rion/00_rion_body.x" },						// [��]00
		{ "data/MODEL/rion/01_rion_head.x" },						// [��]01
		{ "data/MODEL/rion/02_rion_ring.x" },						// [�Ђ�]02
		{ "data/MODEL/rion/03_rion_right.x" },						// [�E��]03
		{ "data/MODEL/rion/04_rion_left.x" },						// [����]04
		{ "data/MODEL/rion/05_rion_back_right2.x" },				// [�E��]05
		{ "data/MODEL/rion/06_rion_back_left2.x" },					// [����]06
		{ "data/MODEL/rion/07_rion_tail_joint.x" },					// [�����ۂ̊֐�]07
		{ "data/MODEL/rion/08_rion_tail.x" },						// [������]08
		{ "data/MODEL/rion/09_rion_edge.x" }						// [�����ۂ̐�[]09
	};

	// �e�N�X�`���[�t�@�C������`
	char *paTextureFile[TEXTURE_PLAYER] =
	{
		{ "" }
	};

	// �v���C���[�̏����ݒ�
	{
		// �`���[�g���A��
		if (GetMode() == MODE_TUTORIAL)
		{
			g_player.pos = D3DXVECTOR3(0.0f, 20.0f, 10.0f);
		}

		// ����ȊO
		else
		{
			g_player.pos = D3DXVECTOR3(0.0f, 20.0f,400.0f);
		}

		g_player.posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.nMotionType = 0;
		g_player.nParent = 0;
		g_player.nIndex = 0;
		g_player.nFram = 0;
		g_player.nCntKeySet = 0;
		g_player.fLength = 50.0f;
		g_player.bUse = true;
		g_player.bjump = false;

		// �X�e�[�^�X���
		g_player.status.nState = PLAYER_STATE_NORMAL;
		g_player.status.nCntState = 0;
		g_player.status.nMaxLife = 100;
		g_player.status.nLife = g_player.status.nMaxLife;
		g_player.status.nLastLife = g_player.status.nMaxLife;
		g_player.status.nMaxMP = 100;
		g_player.status.nMP = g_player.status.nMaxMP;
		g_player.status.nLastMP = g_player.status.nMaxMP;
		g_player.status.nAttack = 1;
		g_player.status.fLength = 50;
		g_player.status.bColition = false;

		// �J���[�̏�����
		g_player.nCntCor = 0;
		g_player.diffuse = 1.0f;

		// ���f���̏����ݒ�
		for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
		{
			g_player.model[nCntModel].paBuffMat = NULL;
			g_player.model[nCntModel].paMesh = NULL;
			g_player.model[nCntModel].nNumMat = 0;
			g_player.model[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].posLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.model[nCntModel].nIdxModelParent = 0;
			g_player.model[nCntModel].nFram = 0;
			g_player.model[nCntModel].nCntKeySet = 0;

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(
				paPlayerFile[nCntModel],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_player.model[nCntModel].paBuffMat,
				NULL,
				&g_player.model[nCntModel].nNumMat,
				&g_player.model[nCntModel].paMesh);

			// ���f���̒��_���W�̍ő�l�E�ŏ��l�̐ݒ�
			int nNumVertex;			// ���_��
			DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVertexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����擾
			nNumVertex = g_player.model[nCntModel].paMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_player.model[nCntModel].paMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			g_player.model[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

			// ���_�����J��Ԃ�
			for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
			{
				// �Ԓn����D3DXVE
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

				// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
				// x //
				// �ŏ��l
				if (g_player.model[nCntModel].vtxMinMaterials.x > vtx.x)
				{
					g_player.model[nCntModel].vtxMinMaterials.x = vtx.x;
				}

				// �ő�l
				if (g_player.model[nCntModel].vtxMaxMaterials.x < vtx.x)
				{
					g_player.model[nCntModel].vtxMaxMaterials.x = vtx.x;
				}

				// y //
				// �ŏ��l
				if (g_player.model[nCntModel].vtxMinMaterials.y > vtx.y)
				{
					g_player.model[nCntModel].vtxMinMaterials.y = vtx.y;
				}

				// �ő�l
				if (g_player.model[nCntModel].vtxMaxMaterials.y < vtx.y)
				{
					g_player.model[nCntModel].vtxMaxMaterials.y = vtx.y;
				}

				// z //
				// �ŏ��l
				if (g_player.model[nCntModel].vtxMinMaterials.z > vtx.z)
				{
					g_player.model[nCntModel].vtxMinMaterials.z = vtx.z;
				}

				// �ő�l
				if (g_player.model[nCntModel].vtxMaxMaterials.z < vtx.z)
				{
					g_player.model[nCntModel].vtxMaxMaterials.z = vtx.z;
				}

				// �T�C�Y�|�C���^��i�߂�
				pVertexBuffer += sizeFVF;
			}

			// �A�����b�N
			g_player.model[nCntModel].paMesh->UnlockVertexBuffer();

			// �ő�l�ƍŏ��l�̋���
			g_player.model[nCntModel].size = g_player.model[nCntModel].vtxMaxMaterials - g_player.model[nCntModel].vtxMinMaterials;
		}
	}

	for (nCntMotion = 0; nCntMotion < PLAYERMOSIONSET_MAX; nCntMotion++)
	{
		// ���[�V�����̏����ݒ�
		g_playerKey[nCntMotion].nNumKey = 0;				// �i���o�[�L�[
		g_playerKey[nCntMotion].nLoop = 0;					// ���[�v�̗L��
		// �����蔻��p //
		g_playerKey[nCntMotion].Collision_nParts = 0;		// �p�[�c�ԍ�
		g_playerKey[nCntMotion].Collision_Ofset = 
			D3DXVECTOR3(0.0f,0.0f,0.0f);					// �I�t�Z�b�g
		g_playerKey[nCntMotion].Collision_Radius = 0.0f;	// �~�̔��a
		g_playerKey[nCntMotion].Collision_StartFram = 0;	// �J�n�t���[����
		g_playerKey[nCntMotion].Collision_EndFram = 0;		// �I���t���[����
		g_playerKey[nCntMotion].Collision_Damage = 0;		// �_���[�W��

		// �O�՗p //
		g_playerKey[nCntMotion].Orbit_nParts = 0;			// �p�[�c�ԍ�
		g_playerKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f,0.0f,0.0f);					// �I�t�Z�b�g
		g_playerKey[nCntMotion].Orbit_Mesh = 0;				// �O�Ղ̐�
		g_playerKey[nCntMotion].Orbit_StartFram = 0;		// �J�n�t���[����
		g_playerKey[nCntMotion].Orbit_EndFram = 0;			// �I���t���[����
		g_playerKey[nCntMotion].Orbit_nType = 0;			// �O�Ղ̃^�C�v
		g_playerKey[nCntMotion].Orbit_nIdTrajectory = -1;	// �O�Ղ̔ԍ�
		g_playerKey[nCntMotion].Orbit_bUse = false;			// �O�Ղ̎g�p���

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_playerKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// �L�[���Ƃɏ�����
			for (nCntKey = 0; nCntKey < PLAYERTYPE_MAX; nCntKey++)
			{
				g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
				g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// �O���[�o���ϐ��̏�����
	g_nPlayerMotionOld = 0;
	g_nIdxShadow = 0;
	g_nNumType = 0;
	g_bMove = false;
	g_bJump = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTexturePlayer[0]);

	// ���f���Ǎ�
	LoadPlayer();
	
	// �v���C���[���[�V�����ǂݍ���
	LoadPlayerMotion();

	for (nCntMotion = 0; nCntMotion < PLAYERMOSIONSET_MAX; nCntMotion++)
	{
		// �g�p���
		if (g_playerKey[nCntMotion].Orbit_bUse == true)
		{
			// �O�Ղ̐ݒ�
			g_playerKey[nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
				g_playerKey[nCntMotion].Orbit_Ofset,
				g_playerKey[nCntMotion].Orbit_nParts,
				g_playerKey[nCntMotion].Orbit_Mesh,
				g_playerKey[nCntMotion].Orbit_StartFram,
				g_playerKey[nCntMotion].Orbit_EndFram,
				g_playerKey[nCntMotion].Orbit_nType);
		}
	}

	// �e�̍쐬
	g_nIdxShadow = SetShadow(g_player.pos,
		D3DXVECTOR3(20.0f, 0.0f, 20.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitPlayer(void)
{
	int nCntModel;

	// �p�[�c�J�E���g
	for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// ���b�V���̊J��
		if (g_player.model[nCntModel].paMesh != NULL)
		{
			g_player.model[nCntModel].paMesh->Release();
			g_player.model[nCntModel].paMesh = NULL;
		}

		// �}�e���A���̊J��
		if (g_player.model[nCntModel].paBuffMat != NULL)
		{
			g_player.model[nCntModel].paBuffMat->Release();
			g_player.model[nCntModel].paBuffMat = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < TEXTURE_PLAYER; nCntTexture++)
	{
		// �e�N�X�`���[�̊J��
		if (g_paTexturePlayer[nCntTexture] != NULL)
		{
			g_paTexturePlayer[nCntTexture]->Release();
			g_paTexturePlayer[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	// ���ݒn�ۑ�
	g_player.posold = g_player.pos;

	// ���[�V������ۑ�
	g_nPlayerMotionOld = g_player.nMotionType;

	// ��ԏ���
	Update_StatePlayer();

	// ���S��ԈȊO
	if (g_player.status.nState != PLAYER_STATE_DIE)
	{
		// ��]����
		Update_RotPlayer();

		// �ړ�����
		Update_MovePlayer();

		// �e�̏���
		Update_ShotPlayer();

		// ���ʍU���̏���
		Update_NormalAttackPlayer();

		// �����蔻��
		Update_ColisionPlayer();

		// ���[�V��������
		Update_MotionPlayer();
	}
	else if (g_player.status.nState == PLAYER_STATE_DIE)
	{
		// ���̓����蔻�� //
		if (ColisionFLOOR(&g_player.pos,
			&g_player.posold,
			&g_player.move,
			&g_player.model[0].size,
			&g_player.pMeshField))
		{
			// ���n���[�V����
			if (g_player.bjump == true)
			{
				g_player.nMotionType = PLAYERMOSIONSET_LANDING;
				g_player.bjump = false;
			}
		}
	}
	// �e�̈ʒu��ݒ�
	SetPositionShadow(
		g_nIdxShadow,
		g_player.pos + D3DXVECTOR3(
			sinf(-g_player.rot.y) * 10.0f,
			0.0f,
			cosf(D3DX_PI - g_player.rot.y) * 10.0f));
}

// ------------------------------------------------------------------------------------------
// ��ԏ���
// ------------------------------------------------------------------------------------------
void Update_StatePlayer(void)
{
	switch (g_player.status.nState)
	{
	case PLAYER_STATE_NORMAL:
		g_player.diffuse = 1.0f;
		break;

	case PLAYER_STATE_DAMAGE:
		// �J���[�̏�ԃJ�E���g�A�b�v
		g_player.nCntCor++;

		// �\�����
		if (g_player.nCntCor % 20 == 0)
		{
			g_player.diffuse = 1.0f;
		}

		// ��\�����
		else if (g_player.nCntCor % 10 == 0)
		{
			g_player.diffuse = 0.5f;
		}

		if (g_player.nCntCor == 100)
		{
			// �J���[�̏�ԃJ�E���g������
			g_player.nCntCor = 0;

			// �v���C���[�̏�Ԃ��m�[�}����
			g_player.status.nState = PLAYER_STATE_NORMAL;

			// �v���C���[�ɍU�����������Ă��邩�ǂ���
			g_player.status.bColition = false;
		}

		break;

	case PLAYER_STATE_DIE:
		// �J�E���g�X�e�[�g�A�b�v
		g_player.status.nCntState++;

		// �X���[���[�V����
		if (g_player.status.nCntState % 10 == 0)
		{
			// �ڕW��]�ʂ��猻�݂̉�]�ʂ̍���
			g_player.rotbetween.z =
				(g_player.rotLast.z - g_player.rot.z) / 10;

			// ��]
			g_player.rot.z += g_player.rotbetween.z;

			// �d��
			g_player.move.y -= 0.5f;

			// �ʒu���̍X�V
			g_player.pos.y += g_player.move.y;
		}

		// �Q�[���I�[�o�[
		if (g_player.status.nCntState == 200)
		{
			SetGameState(GAMESTATE_GAMEOVER);
		}
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------
// ��]����
// ------------------------------------------------------------------------------------------
void Update_RotPlayer(void)
{

}

// ------------------------------------------------------------------------------------------
// �ړ�����
// ------------------------------------------------------------------------------------------
void Update_MovePlayer(void)
{
	CAMERA *pCamera = GetCamera();	// �J�������擾

	// �ړ� //
	// ��
	if (GetKeyboardPress(DIK_A))
	{
		// ��
		if (GetKeyboardPress(DIK_W))
		{
			g_player.rotLast.y = -D3DX_PI * 0.25f + pCamera->rot.y;

			g_player.move.x += sinf(D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// ��O
		else if (GetKeyboardPress(DIK_S))
		{
			g_player.rotLast.y = -D3DX_PI * 0.75f + pCamera->rot.y;

			g_player.move.x += sinf(D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// ��
		else
		{
			g_player.rotLast.y = -D3DX_PI * 0.5f + pCamera->rot.y;


			g_player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
	}
	// �E
	else if (GetKeyboardPress(DIK_D))
	{
		// ��
		if (GetKeyboardPress(DIK_W))
		{
			g_player.rotLast.y = D3DX_PI * 0.25f + pCamera->rot.y;

			g_player.move.x += sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// ��O
		else if (GetKeyboardPress(DIK_S))
		{
			g_player.rotLast.y = D3DX_PI * 0.75f + pCamera->rot.y;

			g_player.move.x += sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// �E
		else
		{
			g_player.rotLast.y = D3DX_PI * 0.5f + pCamera->rot.y;

			g_player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
	}
	// ���ɍs��
	else if (GetKeyboardPress(DIK_W))
	{
		g_player.rotLast.y = D3DX_PI * 0.0f + pCamera->rot.y;
		g_player.move.x += sinf(-D3DX_PI * 1.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
		g_player.move.z += cosf(-D3DX_PI * 1.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
	}
	// ��O�ɍs��
	else if (GetKeyboardPress(DIK_S))
	{
		g_player.rotLast.y = D3DX_PI * 1.0f + pCamera->rot.y;

		g_player.move.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
		g_player.move.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
	}

	// �p�b�h�p //
	int nValueH, nValueV;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fMove;			// �ړ����x
	float fAngle;			// �X�e�B�b�N�p�x�̌v�Z�p�ϐ�
	fAngle = 1.0f;

	// �Q�[���p�b�h�̃X�e�B�b�N�����擾
	GetGamepadStickLeft(0, &nValueH, &nValueV);

	/* �v���C���[�ړ� */
	// �Q�[���p�b�h�ړ�
	if (nValueH != 0 || nValueV != 0)
	{
		// �p�x�̌v�Z
		fAngle = atan2f((float)nValueH, (float)nValueV);

		if (fAngle  > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2;
		}
		else if (fAngle  < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2;
		}

		// ���x�̌v�Z
		if (abs(nValueH) > abs(nValueV))
		{
			fMove = (abs(nValueH) * PLAYER_JOYMOVE) / 32768.0f;
		}
		else
		{
			fMove = (abs(nValueV) * PLAYER_JOYMOVE) / 32768.0f;
		}

		g_player.rotLast.y = fAngle + pCamera->rot.y;

		// �X�e�B�b�N�̊p�x�ɂ���ăv���C���[�ړ�
		g_player.move.x -= sinf(fAngle + pCamera->rot.y) * (PLAYER_JOYMOVE / 2 + fMove);
		g_player.move.z -= cosf(fAngle + pCamera->rot.y) * (PLAYER_JOYMOVE / 2 + fMove);
	}


	// �W�����v
	if (g_player.bjump == false)
	{
		if (GetKeyboardTrigger(DIK_SPACE) || 
			GetJoyboardTrigger(0, JOYPADKEY_A))
		{
			g_player.move.y = 8.0f;
			g_player.bjump = true;
		}
	}

	if (g_playerKey[g_player.nMotionType].nLoop != 0)
	{
		// �ړ���
		if (g_player.move.x >= 0.1f || g_player.move.x <= -0.1f ||
			g_player.move.z >= 0.1f || g_player.move.z <= -0.1f)
		{
			// �ړ����
			g_player.nMotionType = PLAYERMOSIONSET_MOVE;
		}

		// �ʏ펞
		else
		{
			g_player.nMotionType = PLAYERMOSIONSET_NEUTRAL;
		}
	}

	// ���݉�]����
	g_player.rotbetween.y = g_player.rotLast.y - g_player.rot.y;

	// ���x����
	g_player.rotbetween.y = Rot_One_Limit(g_player.rotbetween.y);

	// ��]�ړ�
	g_player.rot.y += g_player.rotbetween.y * 0.1f;

	// ���x����
	g_player.rot.y = Rot_One_Limit(g_player.rot.y);

	// �d��
	g_player.move.y -= PLAYER_G;	// �ړ���
	g_player.pos += g_player.move;

	// ��R��
	g_player.move.x *= PLAYER_RESISTANCE;
	g_player.move.z *= PLAYER_RESISTANCE;
}

// ------------------------------------------------------------------------------------------
// ���[�V��������
// ------------------------------------------------------------------------------------------
void Update_MotionPlayer(void)
{
	// �ϐ��錾
	int	nCntModel;			// �J�E���g���f��

	// ���݂̃��[�V�����ƑO��̃��[�V�������r
	if (g_player.nMotionType != g_nPlayerMotionOld)
	{
		g_player.nFram = 0;

		g_player.nCntKeySet = 0;
	}

	// �p�[�c�J�E���g
	for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// �t���[�������O�̏ꍇ
		if (g_player.nFram == 0)
		{
			// �t���[�����̋K��l���O�ł͂Ȃ��ꍇ
			if (g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram != 0)
			{
				// �ŏI�n�_�ƌ��ݒn�_�̍�������
				g_player.model[nCntModel].rotKeyBetween =
					(g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].rot -
						g_player.model[nCntModel].rot) /
						(float)g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram;

				// �ŏI�n�_ �ƌ��ݒn�_�̍���
				g_player.model[nCntModel].posKeyBetween =
					(g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].pos -
						g_player.model[nCntModel].pos) /
						(float)g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram;
			}

			// �t���[�����̋K��l���O�̏ꍇ
			else
			{
				// �ŏI�n�_�����ݒn�_�ɑ��
				g_player.model[nCntModel].rot =
					g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].rot;

				// �ŏI�n�_�����ݒn�_�ɑ��
				g_player.model[nCntModel].pos =
					g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].pos;

				// �����̏�����
				g_player.model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �����̏�����
				g_player.model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		// ��]����
		g_player.model[nCntModel].rot +=
			g_player.model[nCntModel].rotKeyBetween;

		// �ړ�����
		g_player.model[nCntModel].pos +=
			g_player.model[nCntModel].posKeyBetween;
	}

	// �t���[�������K��l�Ɠ����ɂȂ�����
	if (g_player.nFram ==
		g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram)
	{
		// �L�[�Z�b�g�̍X�V
		g_player.nCntKeySet++;

		// �t���[����������
		g_player.nFram = 0;

		// �L�[�Z�b�g�����K��l�Ɠ����ɂȂ�����
		if (g_player.nCntKeySet == g_playerKey[g_player.nMotionType].nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (g_playerKey[g_player.nMotionType].nLoop == 0)
			{
				// �L�[�Z�b�g��������
				g_player.nCntKeySet = 0;

				g_player.nMotionType = PLAYERMOSIONSET_NEUTRAL;
			}

			// ���[�v���鎞
			else
			{
				// �L�[�Z�b�g��������
				g_player.nCntKeySet = 0;
			}
		}
	}

	// �t���[�������K��l�Ɠ�������Ȃ��Ƃ�
	else
	{
		// �t���[�����J�E���g�A�b�v
		g_player.nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// ���ˏ���
// ------------------------------------------------------------------------------------------
void Update_ShotPlayer(void)
{
	if (GetKeyboardTrigger(DIK_U) ||
		GetJoyboardTrigger(0,JOYPADKEY_LEFT_1))
	{
		if (g_player.status.nMP > 0)
		{
			SetBullet(
				D3DXVECTOR3(
					g_player.model[PLAYERTYPE_01_RION_HEAD].mtxWorldModel._41,
					g_player.model[PLAYERTYPE_01_RION_HEAD].mtxWorldModel._42,
					g_player.model[PLAYERTYPE_01_RION_HEAD].mtxWorldModel._43),
				D3DXVECTOR3(4.0f, 0.0f, 4.0f),
				g_player.rot,
				D3DXVECTOR3(15.0f, 15.0f, 15.0f),
				200,
				0,
				g_player.status.nAttack);

			// �_���[�W�v�Z
			g_player.status.nLastMP -= PLAYER_MPDAMAGE;

			// �Q�[�W�̍X�V
			ChangeHpGauge(HPGAUGESYSTEM_MP, (float)g_player.status.nMP, (float)g_player.status.nLastMP);

			// ���݂�HP�ɑ��
			g_player.status.nMP = g_player.status.nLastMP;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ���ʍU������
// ------------------------------------------------------------------------------------------
void Update_NormalAttackPlayer(void)
{
	if (GetKeyboardTrigger(DIK_J) ||
		GetJoyboardTrigger(0, JOYPADKEY_RIGHT_1))
	{
		// SE
		PlaySound(SOUND_LABEL_SE_ATTACK);

		// ���ʍU�����[�V����
		g_player.nMotionType = PLAYERMOSIONSET_NORMALATACK;

		// �O�Ղ̔���
		OccurrenceTrajectory(
			g_playerKey[PLAYERMOSIONSET_NORMALATACK].Orbit_nIdTrajectory, 0);
	}
}

// ------------------------------------------------------------------------------------------
// �����蔻�菈��
// ------------------------------------------------------------------------------------------
void Update_ColisionPlayer(void)
{
	// �ϐ��錾
	GIRAFFE * pGiraffe = GetGiraffe();					// �L�����̏��擾	
	SHOGIRA * pShogira = GetShogira();					// �L�����̏��擾	
	BUFFALO * pBuffalo = GetBuffalo();					// �o�b�t�@���[�̏��擾
	KING_RION * pKing_Rion = GetKing_Rion();			// �S�b�̉��̏��擾
	MATERIALS * pMaterials = GetMaterials();			// �f�ނ̏��擾
	D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 0.0f, 10.0f);

	int nCntGiraffe = 0;
	int nCntShogira = 0;
	int nCntBuffalo = 0;
	int nCntKing_Rion = 0;
	int nCntMatarials = 0;

	// ���̓����蔻�� //
	if (ColisionFLOOR(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[0].size,
		&g_player.pMeshField))
	{
		// ���n���[�V����
		if (g_player.bjump == true)
		{
			g_player.nMotionType = PLAYERMOSIONSET_LANDING;
			g_player.bjump = false;
		}
	}

	// ���̓����蔻�� //
	if (ColisionWATER(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[0].size,
		&g_player.pWater))
	{
		g_player.bjump = false;
	}

	// �f�ނ̓����蔻�� //
	switch (ColisionMaterials(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[0].size,
		&g_player.pMaterials))
	{
		// ��̓����蔻��
	case 0:
		if (g_player.bjump == true)
		{
			g_player.nMotionType = PLAYERMOSIONSET_LANDING;
			g_player.bjump = false;
		}
		break;

		// ���̓����蔻��
	case 1:
		break;

		// �O�㍶�E�̓����蔻��
	case 2:
		break;

	default:
		break;
	}

	// �ǂ̓����蔻��
	if (ColisionWall(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[1].size,
		&g_player.pWall))
	{

	}

	// �f�ރJ�E���g
	for (nCntMatarials = 0; nCntMatarials < MATERIALS_MAX; nCntMatarials++, pMaterials++)
	{
		if (pMaterials->bUse == true)
		{
			// �`���[�g���A���p
			if (GetMode() == MODE_TUTORIAL)
			{

				// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
				if (Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
					pMaterials->fRadius,
					g_player.fLength,
					0.0f) >=
					Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
						pMaterials->pos.x - g_player.pos.x,
						pMaterials->pos.y - g_player.pos.y,
						pMaterials->pos.z - g_player.pos.z))
				{
					SetPositionBillboard(
						BILLBOARDTYPE_TREE,
						D3DXVECTOR3(
							pMaterials->pos.x + -30.0f,
							pMaterials->pos.y + 30.0f,
							pMaterials->pos.z + 30.0f));
				}

				else
				{
					DeleteBillboard(BILLBOARDTYPE_TREE);
				}
			}

			// ��l���̍U�����̓����蔻��
			if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
			{

				if (pMaterials->bHit == false)
				{
					// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
					if (Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
						pMaterials->fRadius,
						g_playerKey[g_player.nMotionType].Collision_Radius,
						0.0f) >=
						Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
							pMaterials->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41,
							pMaterials->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42,
							pMaterials->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
					{
						// �p�x
						float frot;
						// �Ԗ�
						if (pMaterials->nDrawType ==
							MATERIALS_OBJECT_01_WOOD_RED)
						{
							// �v���C���[������p�x
							frot = atan2f(g_player.pos.x - pMaterials->pos.x, g_player.pos.z - pMaterials->pos.z);
							// �A�C�e���ݒu
							SetMaterials(pMaterials->pos +
								D3DXVECTOR3(
									sinf(frot) * 50.0f,
									80.0f,
									cosf(frot) * 50.0f),	// �ʒu
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// ��]��
								D3DXVECTOR3(5.0f, 5.0f, 5.0f),	// �ړ���
								MATERIALS_ITEM_00_APPLE,									// �`��^�C�v
								50,									// HP�񕜗�
								0);									// MP�񕜗�

						}

						// �I�����W��
						else if (pMaterials->nDrawType ==
							MATERIALS_OBJECT_02_WOOD_ORANGE)
						{
							// �v���C���[������p�x
							frot = atan2f(g_player.pos.x - pMaterials->pos.x, g_player.pos.z - pMaterials->pos.z);
							// �A�C�e���ݒu
							SetMaterials(pMaterials->pos +
								D3DXVECTOR3(
									sinf(frot) * 50.0f,
									80.0f,
									cosf(frot) * 50.0f),		// �ʒu
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// ��]��
								D3DXVECTOR3(5.0f, 5.0f, 5.0f),	// �ړ���
								MATERIALS_ITEM_01_ACTINIDIA,	// �`��^�C�v
								0,								// HP�񕜗�
								50);							// MP�񕜗�
						}
						// ������������
						pMaterials->bHit = true;
						// �h�[���폜
						DeleteDome(pMaterials->nIdxDome);
					}
				}
			}

		}
	}

	// �L�����J�E���g
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++, pGiraffe++)
	{
		if (pGiraffe->bUse == true)
		{
			// �`���[�g���A���p
			if (GetMode() == MODE_TUTORIAL)
			{
				// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
				if (Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
					pGiraffe->fLength,
					g_player.fLength,
					0.0f) >=
					Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
						pGiraffe->pos.x - g_player.pos.x,
						pGiraffe->pos.y - g_player.pos.y,
						pGiraffe->pos.z - g_player.pos.z))
				{
					SetPositionBillboard(
						BILLBOARDTYPE_ENTER,
						D3DXVECTOR3(
							pGiraffe->pos.x,
							pGiraffe->pos.y + 50.0f,
							pGiraffe->pos.z));
				}

				else
				{
					DeleteBillboard(BILLBOARDTYPE_ENTER);
				}

			}

			if (pGiraffe->status.nStatus != GIRAFFESTATUS_DIE)
			{
				// ��l���̍U�����̓����蔻��
				if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
				{

					if (pGiraffe->status.bColition == false)
					{
						// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
						if (Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
							pGiraffe->fLength,
							g_playerKey[g_player.nMotionType].Collision_Radius,
							0.0f) >=
							Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
								pGiraffe->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41,
								pGiraffe->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42,
								pGiraffe->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
						{
							// �L�����̃_���[�W����
							HitGiraffe(
								nCntGiraffe,
								g_player.status.nAttack +
								g_playerKey[g_player.nMotionType].Collision_Damage);
						}
					}
				}
			}

		}
	}

	// �L�����J�E���g
	for (nCntShogira = 0; nCntShogira < MAX_GIRAFFE; nCntShogira++, pShogira++)
	{
		if (pShogira->bUse == true)
		{
			if (pShogira->status.nStatus != GIRAFFESTATUS_DIE)
			{
				// ��l���̍U�����̓����蔻��
				if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
				{

					if (pShogira->status.bColition == false)
					{
						// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
						if (Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
							pShogira->fLength,
							g_playerKey[g_player.nMotionType].Collision_Radius,
							0.0f) >=
							Addition_Circle(											// �~�̑����Z(�L�����ƃv���C���[�̔��a)
								pShogira->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41,
								pShogira->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42,
								pShogira->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
						{
							// �L�����̃_���[�W����
							HitShogira(
								nCntShogira,
								g_player.status.nAttack +
								g_playerKey[g_player.nMotionType].Collision_Damage);
						}
					}
				}
			}

		}
	}

	// �o�b�t�@���[�J�E���g
	for (nCntBuffalo = 0; nCntBuffalo < MAX_GIRAFFE; nCntBuffalo++, pBuffalo++)
	{
		if (pBuffalo->bUse == true)
		{
			// ��l���̍U�����̓����蔻��
			if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
			{

				if (pBuffalo->status.nStatus != BUFFALOSTATUS_DIE)
				{
					if (pBuffalo->status.bColition == false)
					{
						// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
						if ((pBuffalo->fLength * pBuffalo->fLength) +
							(g_playerKey[g_player.nMotionType].Collision_Radius *
								g_playerKey[g_player.nMotionType].Collision_Radius) >=
								(pBuffalo->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41) *
							(pBuffalo->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41) +
							(pBuffalo->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42) *
							(pBuffalo->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42) +
							(pBuffalo->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43) *
							(pBuffalo->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
						{
							// �L�����̃_���[�W����
							HitBuffalo(
								nCntBuffalo,
								g_player.status.nAttack +
								g_playerKey[g_player.nMotionType].Collision_Damage);
						}
					}
				}
			}
		}
	}

	// �L�����J�E���g
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_GIRAFFE; nCntKing_Rion++, pKing_Rion++)
	{
		if (pKing_Rion->bUse == true)
		{
			// ��l���̍U�����̓����蔻��
			if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
			{

				if (pKing_Rion->status.nStatus != GIRAFFESTATUS_DIE)
				{
					if (pKing_Rion->status.bColition == false)
					{
						// �v���C���[�̍U�����̃L�����ƃv���C���[�̓����蔻��
						if ((pKing_Rion->fLength * pKing_Rion->fLength) +
							(g_playerKey[g_player.nMotionType].Collision_Radius *
								g_playerKey[g_player.nMotionType].Collision_Radius) >=
								(pKing_Rion->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41) *
							(pKing_Rion->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41) +
							(pKing_Rion->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42) *
							(pKing_Rion->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42) +
							(pKing_Rion->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43) *
							(pKing_Rion->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
						{
							// �L�����̃_���[�W����
							HitKing_Rion(
								nCntKing_Rion,
								g_player.status.nAttack +
								g_playerKey[g_player.nMotionType].Collision_Damage);
						}
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;					// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;					// �}�e���A���f�[�^�̃|�C���^
	int nCntModel;

	// �g�p���
	if (g_player.bUse)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.mtxWorldPlayer);

		// ��]�𔽉f
		// �X�P�[���𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_player.rot.y,
			g_player.rot.x,
			g_player.rot.z);

		// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer,
			&g_player.mtxWorldPlayer, &mtxRot);


		// �ʒu�𔽉f //
		// ���s�ړ��s��쐬(�I�t�Z�b�g)
		D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
			g_player.pos.x,
			g_player.pos.y,
			g_player.pos.z);	// ���ꂼ��̈ړ���

		// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer,	// 1
			&g_player.mtxWorldPlayer,					// 2
			&mtxTrans);									// 3

		// �J�E���g���f��
		for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_player.model[nCntModel].mtxWorldModel);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_player.model[nCntModel].rot.y,
				g_player.model[nCntModel].rot.x,
				g_player.model[nCntModel].rot.z);

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,
				&g_player.model[nCntModel].mtxWorldModel, &mtxRot);


			// �ʒu�𔽉f //
			// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
				g_player.model[nCntModel].pos.x,
				g_player.model[nCntModel].pos.y,
				g_player.model[nCntModel].pos.z);	// ���ꂼ��̈ړ���

			// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,	// 1
				&g_player.model[nCntModel].mtxWorldModel,					// 2
				&mtxTrans);									// 3


			// [���ׂĂ̐e]����Ȃ��ꍇ
			if (g_player.model[nCntModel].nIdxModelParent != -1)
			{
				// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
				D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,
					&g_player.model[nCntModel].mtxWorldModel,
					&g_player.model[g_player.model[nCntModel].nIdxModelParent].mtxWorldModel);
			}

			else
			{
				// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
				D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,
					&g_player.model[nCntModel].mtxWorldModel,
					&g_player.mtxWorldPlayer);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_player.model[nCntModel].mtxWorldModel);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.model[nCntModel].paBuffMat->GetBufferPointer();

			// �e�N�X�`���[�ݒ�
			pDevice->SetTexture(0, g_paTexturePlayer[0]);

			// �J�E���g�}�e���A��
			for (int nCntMat = 0; nCntMat < (int)g_player.model[nCntModel].nNumMat; nCntMat++,pMat++)
			{
				// �{�̂̃J���[���X�V
				pMat->MatD3D.Diffuse.a = g_player.diffuse;
						
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat->MatD3D);

				// �`��
				g_player.model[nCntModel].paMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�e�[�^�X�A�b�v����
// ------------------------------------------------------------------------------------------
void StatusUpPlayer(
	int nUpLife,		// HP�A�b�v
	int nUpMp,			// MP�A�b�v
	int nUpAttack)		// �U���̓A�b�v
{
	// SE
	PlaySound(SOUND_LABEL_SE_EAT);

	// �ő�HP�ω�
	g_player.status.nMaxLife += nUpLife;

	// �ő�HP�Q�[�W�̕ω�
	ChangeMaxHpGauge(HPGAUGESYSTEM_HP,(float)g_player.status.nMaxLife);

	// �ő�MP�ω�
	g_player.status.nMaxMP += nUpMp;

	// �ő�MP�Q�[�W�̕ω�
	ChangeMaxHpGauge(HPGAUGESYSTEM_MP, (float)g_player.status.nMaxMP);

	// �U���͕ω�
	g_player.status.nAttack += nUpAttack;
}

// ------------------------------------------------------------------------------------------
// �񕜏���
// ------------------------------------------------------------------------------------------
void RecoveryPlayer(int nRecoveryLife, int nRecoveryMP)
{
	// �_���[�W�v�Z
	g_player.status.nLastLife += nRecoveryLife;

	// �ő�HP�𒴂�����
	if (g_player.status.nLastLife >= g_player.status.nMaxLife)
	{
		g_player.status.nLastLife = g_player.status.nMaxLife;
	}

	// �Q�[�W�̍X�V
	ChangeHpGauge(HPGAUGESYSTEM_HP, (float)g_player.status.nLife, (float)g_player.status.nLastLife);

	// ���݂�HP�ɑ��
	g_player.status.nLife = g_player.status.nLastLife;

	// ���͂���
	g_player.status.nLastMP += nRecoveryMP;

	// �ő�MP�𒴂�����
	if (g_player.status.nLastMP >= g_player.status.nMaxMP)
	{
		g_player.status.nLastMP = g_player.status.nMaxMP;
	}

	// �Q�[�W�̍X�V
	ChangeHpGauge(HPGAUGESYSTEM_MP, (float)g_player.status.nMP, (float)g_player.status.nLastMP);

	// ���݂�MP�ɑ��
	g_player.status.nMP = g_player.status.nLastMP;
}

// ------------------------------------------------------------------------------------------
// �ʒu���̏���
// ------------------------------------------------------------------------------------------
void PlayerPosition(void)
{
	// �{�X
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		g_player.pos = D3DXVECTOR3(300.0f, 20.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

// ------------------------------------------------------------------------------------------
// �_���[�W���󂯂��Ƃ��̏���
// ------------------------------------------------------------------------------------------
void HitPlayer(int fDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// �U�����������Ă���
	g_player.status.bColition = true;

	// �_���[�W�v�Z
	g_player.status.nLastLife -= fDamage;

	// �Q�[�W�̍X�V
  	ChangeHpGauge(HPGAUGESYSTEM_HP,(float)g_player.status.nLife, (float)g_player.status.nLastLife);

	// ���݂�HP�ɑ��
	g_player.status.nLife = g_player.status.nLastLife;

	PlaySound(SOUND_LABEL_SE_COLISION);

	// HP��0�̎�
	if (g_player.status.nLife <= 0)
	{
		// ���S���
		g_player.status.nState = PLAYER_STATE_DIE;

		// �ڕW��]��
		g_player.rotLast.z = D3DX_PI / 2;

		// �ړ���
		g_player.move.y = 5.0f;

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
					g_player.pos.x,
					g_player.pos.y,
					g_player.pos.z),

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
		g_player.status.nState = PLAYER_STATE_DAMAGE;

		// �U�����������Ă��邩�ǂ���
		g_player.status.bColition = true;

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
					g_player.pos.x,
					g_player.pos.y,
					g_player.pos.z),

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
// �v���C���[���[�V�����̓ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadPlayerMotion(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;
	
	// �ϐ��錾
	int	nCntMotion = 0;		// ���[�V�����J�E���g
	int	nCntKeySet = 0;		// �t���[����
	int	nCntKey = 0;		// ���f����
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/PLAYER/player_mosion.txt", "r");
	
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

				// ���[�V�����Z�b�g��������
				if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// ���f���̏�����
					nCntKeySet = 0;

					// �G���h���[�V�����Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ���[�v���邩�ǂ������ǂݍ���
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_playerKey[nCntMotion].nLoop);
						}

						// �L�[�����ǂݍ���
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_playerKey[nCntMotion].nNumKey);
						}

						// �����蔻����ǂݍ���
						else if (strcmp(cHeadText, "COLLISION") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
								&cDie, &cDie, 
								&g_playerKey[nCntMotion].Collision_nParts,
								&g_playerKey[nCntMotion].Collision_Ofset.x,
								&g_playerKey[nCntMotion].Collision_Ofset.y,
								&g_playerKey[nCntMotion].Collision_Ofset.z,
								&g_playerKey[nCntMotion].Collision_Radius,
								&g_playerKey[nCntMotion].Collision_StartFram,
								&g_playerKey[nCntMotion].Collision_EndFram);
						}

						// �_���[�W���ǂݍ���
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_playerKey[nCntMotion].Collision_Damage);
						}

						// �O�Տ��ǂݍ���
						else if (strcmp(cHeadText, "ORBIT") == 0)
						{
							sscanf(cRaedText, "%s %s %d %f %f %f %d %d %d",
								&cDie, &cDie,
								&g_playerKey[nCntMotion].Orbit_nParts,
								&g_playerKey[nCntMotion].Orbit_Ofset.x,
								&g_playerKey[nCntMotion].Orbit_Ofset.y,
								&g_playerKey[nCntMotion].Orbit_Ofset.z,
								&g_playerKey[nCntMotion].Orbit_Mesh,
								&g_playerKey[nCntMotion].Orbit_StartFram,
								&g_playerKey[nCntMotion].Orbit_EndFram);

							// �L�����N�^�[�^�C�v
							g_playerKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_PLAYER;

							// �g�p���
							g_playerKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie,
									&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].nFram);
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
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos.x,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos.y,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos.z);

											// �p�[�c�̈ʒu����������
											g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos += g_player.model[nCntKey].pos;
										}

										// ��]���ǂݍ���
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// �t���[����
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot.x,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot.y,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot.z);
										}
									}

									// �J�E���g�L�[
									nCntKey++;
								}
							}

							// �J�E���g���f��
							nCntKeySet++;
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
		MessageBox(NULL, "�v���C���[�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �v���C���[�ǂݍ��ݏ���
// ------------------------------------------------------------------------------------------
void LoadPlayer(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;
	int nCntPlayer = 0;
	int	nCntModel = 0;
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen("data/SAVE/PLAYER/player.txt", "r");

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
				else if (strcmp(cHeadText, "PLAYERSET") == 0)
				{

					// �G���h���f���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_PLAYERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "nParent") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&g_player.model[nCntModel].nIdxModelParent);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "pos") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_player.model[nCntModel].pos.x,
								&g_player.model[nCntModel].pos.y, 
								&g_player.model[nCntModel].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "rot") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_player.model[nCntModel].rot.x, &g_player.model[nCntModel].rot.y, &g_player.model[nCntModel].rot.z);
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
		MessageBox(NULL, "�v���C���[�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// �v���C���[��񏈗�
// ------------------------------------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_player;
}

// ------------------------------------------------------------------------------------------
// �v���C���[�̃��f����񏈗�
// ------------------------------------------------------------------------------------------
MODEL * GetPlayerModel(void)
{
	return &g_player.model[0];
}

// ------------------------------------------------------------------------------------------
// �v���C���[�̈ʒu��񏈗�
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_Pos(void)
{
	return g_player.pos;
}

// ------------------------------------------------------------------------------------------
// �v���C���[�̑O��̈ʒu��񏈗�
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_PosOld(void)
{
	return g_player.posold;
}

// ------------------------------------------------------------------------------------------
// �v���C���[�̉�]��񏈗�
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_Rot(void)
{
	return g_player.rot;
}

// ------------------------------------------------------------------------------------------
// �v���C���[���a��񏈗�
// ------------------------------------------------------------------------------------------
float GetPlayer_Radius(void)
{
	return g_player.status.fLength;
}

// ------------------------------------------------------------------------------------------
// �v���C���[�X�e�[�^�X��񏈗�
// ------------------------------------------------------------------------------------------
PLAYER_STATUS * GetPlayerStatus(void)
{
	return &g_player.status;
}
