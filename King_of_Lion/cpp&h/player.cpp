// ------------------------------------------------------------------------------------------
//
// モデル処理 [player.cpp]
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

// 仮のヘッダー
#include "giraffe.h"
#include "buffalo.h"
#include "game.h"
#include "king_rion.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_PLAYER (1)
#define PLAYER_ATTACK (5)
#define PLAYER_MPDAMAGE (10)
#define PLAYER_KEYMOVE (2.0f)	// キーボード移動量
#define PLAYER_JOYMOVE (4.0f)	// ジョイパッド移動量
#define PLAYER_G (0.5f)			// 重力
#define PLAYER_RESISTANCE (0.5f)// 抵抗力

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
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
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTexturePlayer[TEXTURE_PLAYER] = {};	// テクスチャへのポインタ

PLAYER				g_player;				// モデル情報

MOSION_PLAYER		g_playerKey[PLAYERMOSIONSET_MAX];		// モーション情報

int					g_nNumType = 0;							// ナンバータイプ

int					g_nPlayerMotionOld = 0;					// モーションタイプ

int					g_nIdxShadow = 0;						// 影のID

bool				g_bStop = false;						// ポーズ

bool				g_bMove;								// 移動しているかどうか

bool				g_bJump;								// ジャンプしているかどうか

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitPlayer(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	CAMERA	*pCamera = GetCamera();				// カメラの取得
	int		nCntModel;							// モデルのカウント

	// モーション用変数
	int		nCntMotion;							// モーションカウント
	int		nCntKeySet;							// モーションカウント
	int		nCntKey;							// キーカウント

	// モデルファイル名定義
	char *paPlayerFile[PLAYERTYPE_MAX] =
	{
		{ "data/MODEL/rion/00_rion_body.x" },						// [体]00
		{ "data/MODEL/rion/01_rion_head.x" },						// [頭]01
		{ "data/MODEL/rion/02_rion_ring.x" },						// [ひげ]02
		{ "data/MODEL/rion/03_rion_right.x" },						// [右手]03
		{ "data/MODEL/rion/04_rion_left.x" },						// [左手]04
		{ "data/MODEL/rion/05_rion_back_right2.x" },				// [右足]05
		{ "data/MODEL/rion/06_rion_back_left2.x" },					// [左足]06
		{ "data/MODEL/rion/07_rion_tail_joint.x" },					// [しっぽの関節]07
		{ "data/MODEL/rion/08_rion_tail.x" },						// [しっぽ]08
		{ "data/MODEL/rion/09_rion_edge.x" }						// [しっぽの先端]09
	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_PLAYER] =
	{
		{ "" }
	};

	// プレイヤーの初期設定
	{
		// チュートリアル
		if (GetMode() == MODE_TUTORIAL)
		{
			g_player.pos = D3DXVECTOR3(0.0f, 20.0f, 10.0f);
		}

		// それ以外
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

		// ステータス情報
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

		// カラーの初期化
		g_player.nCntCor = 0;
		g_player.diffuse = 1.0f;

		// モデルの初期設定
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

			// Xファイルの読み込み
			D3DXLoadMeshFromX(
				paPlayerFile[nCntModel],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_player.model[nCntModel].paBuffMat,
				NULL,
				&g_player.model[nCntModel].nNumMat,
				&g_player.model[nCntModel].paMesh);

			// モデルの頂点座標の最大値・最小値の設定
			int nNumVertex;			// 頂点数
			DWORD sizeFVF;			// 頂点フォーマットのサイズ
			BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

			// 頂点数を取得
			nNumVertex = g_player.model[nCntModel].paMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_player.model[nCntModel].paMesh->GetFVF());

			// 頂点バッファをロック
			g_player.model[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

			// 頂点数分繰り返す
			for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
			{
				// 番地情報のD3DXVE
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

				// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
				// x //
				// 最小値
				if (g_player.model[nCntModel].vtxMinMaterials.x > vtx.x)
				{
					g_player.model[nCntModel].vtxMinMaterials.x = vtx.x;
				}

				// 最大値
				if (g_player.model[nCntModel].vtxMaxMaterials.x < vtx.x)
				{
					g_player.model[nCntModel].vtxMaxMaterials.x = vtx.x;
				}

				// y //
				// 最小値
				if (g_player.model[nCntModel].vtxMinMaterials.y > vtx.y)
				{
					g_player.model[nCntModel].vtxMinMaterials.y = vtx.y;
				}

				// 最大値
				if (g_player.model[nCntModel].vtxMaxMaterials.y < vtx.y)
				{
					g_player.model[nCntModel].vtxMaxMaterials.y = vtx.y;
				}

				// z //
				// 最小値
				if (g_player.model[nCntModel].vtxMinMaterials.z > vtx.z)
				{
					g_player.model[nCntModel].vtxMinMaterials.z = vtx.z;
				}

				// 最大値
				if (g_player.model[nCntModel].vtxMaxMaterials.z < vtx.z)
				{
					g_player.model[nCntModel].vtxMaxMaterials.z = vtx.z;
				}

				// サイズポインタを進める
				pVertexBuffer += sizeFVF;
			}

			// アンロック
			g_player.model[nCntModel].paMesh->UnlockVertexBuffer();

			// 最大値と最小値の距離
			g_player.model[nCntModel].size = g_player.model[nCntModel].vtxMaxMaterials - g_player.model[nCntModel].vtxMinMaterials;
		}
	}

	for (nCntMotion = 0; nCntMotion < PLAYERMOSIONSET_MAX; nCntMotion++)
	{
		// モーションの初期設定
		g_playerKey[nCntMotion].nNumKey = 0;				// ナンバーキー
		g_playerKey[nCntMotion].nLoop = 0;					// ループの有無
		// 当たり判定用 //
		g_playerKey[nCntMotion].Collision_nParts = 0;		// パーツ番号
		g_playerKey[nCntMotion].Collision_Ofset = 
			D3DXVECTOR3(0.0f,0.0f,0.0f);					// オフセット
		g_playerKey[nCntMotion].Collision_Radius = 0.0f;	// 円の半径
		g_playerKey[nCntMotion].Collision_StartFram = 0;	// 開始フレーム数
		g_playerKey[nCntMotion].Collision_EndFram = 0;		// 終了フレーム数
		g_playerKey[nCntMotion].Collision_Damage = 0;		// ダメージ数

		// 軌跡用 //
		g_playerKey[nCntMotion].Orbit_nParts = 0;			// パーツ番号
		g_playerKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f,0.0f,0.0f);					// オフセット
		g_playerKey[nCntMotion].Orbit_Mesh = 0;				// 軌跡の線
		g_playerKey[nCntMotion].Orbit_StartFram = 0;		// 開始フレーム数
		g_playerKey[nCntMotion].Orbit_EndFram = 0;			// 終了フレーム数
		g_playerKey[nCntMotion].Orbit_nType = 0;			// 軌跡のタイプ
		g_playerKey[nCntMotion].Orbit_nIdTrajectory = -1;	// 軌跡の番号
		g_playerKey[nCntMotion].Orbit_bUse = false;			// 軌跡の使用状態

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_playerKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// キーごとに初期化
			for (nCntKey = 0; nCntKey < PLAYERTYPE_MAX; nCntKey++)
			{
				g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
				g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// グローバル変数の初期化
	g_nPlayerMotionOld = 0;
	g_nIdxShadow = 0;
	g_nNumType = 0;
	g_bMove = false;
	g_bJump = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTexturePlayer[0]);

	// モデル読込
	LoadPlayer();
	
	// プレイヤーモーション読み込み
	LoadPlayerMotion();

	for (nCntMotion = 0; nCntMotion < PLAYERMOSIONSET_MAX; nCntMotion++)
	{
		// 使用状態
		if (g_playerKey[nCntMotion].Orbit_bUse == true)
		{
			// 軌跡の設定
			g_playerKey[nCntMotion].Orbit_nIdTrajectory = SetTrajectory(
				g_playerKey[nCntMotion].Orbit_Ofset,
				g_playerKey[nCntMotion].Orbit_nParts,
				g_playerKey[nCntMotion].Orbit_Mesh,
				g_playerKey[nCntMotion].Orbit_StartFram,
				g_playerKey[nCntMotion].Orbit_EndFram,
				g_playerKey[nCntMotion].Orbit_nType);
		}
	}

	// 影の作成
	g_nIdxShadow = SetShadow(g_player.pos,
		D3DXVECTOR3(20.0f, 0.0f, 20.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitPlayer(void)
{
	int nCntModel;

	// パーツカウント
	for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_player.model[nCntModel].paMesh != NULL)
		{
			g_player.model[nCntModel].paMesh->Release();
			g_player.model[nCntModel].paMesh = NULL;
		}

		// マテリアルの開放
		if (g_player.model[nCntModel].paBuffMat != NULL)
		{
			g_player.model[nCntModel].paBuffMat->Release();
			g_player.model[nCntModel].paBuffMat = NULL;
		}
	}

	for (int nCntTexture = 0; nCntTexture < TEXTURE_PLAYER; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTexturePlayer[nCntTexture] != NULL)
		{
			g_paTexturePlayer[nCntTexture]->Release();
			g_paTexturePlayer[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	// 現在地保存
	g_player.posold = g_player.pos;

	// モーションを保存
	g_nPlayerMotionOld = g_player.nMotionType;

	// 状態処理
	Update_StatePlayer();

	// 死亡状態以外
	if (g_player.status.nState != PLAYER_STATE_DIE)
	{
		// 回転処理
		Update_RotPlayer();

		// 移動処理
		Update_MovePlayer();

		// 弾の処理
		Update_ShotPlayer();

		// 普通攻撃の処理
		Update_NormalAttackPlayer();

		// 当たり判定
		Update_ColisionPlayer();

		// モーション処理
		Update_MotionPlayer();
	}
	else if (g_player.status.nState == PLAYER_STATE_DIE)
	{
		// 床の当たり判定 //
		if (ColisionFLOOR(&g_player.pos,
			&g_player.posold,
			&g_player.move,
			&g_player.model[0].size,
			&g_player.pMeshField))
		{
			// 着地モーション
			if (g_player.bjump == true)
			{
				g_player.nMotionType = PLAYERMOSIONSET_LANDING;
				g_player.bjump = false;
			}
		}
	}
	// 影の位置を設定
	SetPositionShadow(
		g_nIdxShadow,
		g_player.pos + D3DXVECTOR3(
			sinf(-g_player.rot.y) * 10.0f,
			0.0f,
			cosf(D3DX_PI - g_player.rot.y) * 10.0f));
}

// ------------------------------------------------------------------------------------------
// 状態処理
// ------------------------------------------------------------------------------------------
void Update_StatePlayer(void)
{
	switch (g_player.status.nState)
	{
	case PLAYER_STATE_NORMAL:
		g_player.diffuse = 1.0f;
		break;

	case PLAYER_STATE_DAMAGE:
		// カラーの状態カウントアップ
		g_player.nCntCor++;

		// 表示状態
		if (g_player.nCntCor % 20 == 0)
		{
			g_player.diffuse = 1.0f;
		}

		// 非表示状態
		else if (g_player.nCntCor % 10 == 0)
		{
			g_player.diffuse = 0.5f;
		}

		if (g_player.nCntCor == 100)
		{
			// カラーの状態カウント初期化
			g_player.nCntCor = 0;

			// プレイヤーの状態をノーマルへ
			g_player.status.nState = PLAYER_STATE_NORMAL;

			// プレイヤーに攻撃が当たっているかどうか
			g_player.status.bColition = false;
		}

		break;

	case PLAYER_STATE_DIE:
		// カウントステートアップ
		g_player.status.nCntState++;

		// スローモーション
		if (g_player.status.nCntState % 10 == 0)
		{
			// 目標回転量から現在の回転量の差分
			g_player.rotbetween.z =
				(g_player.rotLast.z - g_player.rot.z) / 10;

			// 回転
			g_player.rot.z += g_player.rotbetween.z;

			// 重力
			g_player.move.y -= 0.5f;

			// 位置情報の更新
			g_player.pos.y += g_player.move.y;
		}

		// ゲームオーバー
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
// 回転処理
// ------------------------------------------------------------------------------------------
void Update_RotPlayer(void)
{

}

// ------------------------------------------------------------------------------------------
// 移動処理
// ------------------------------------------------------------------------------------------
void Update_MovePlayer(void)
{
	CAMERA *pCamera = GetCamera();	// カメラ情報取得

	// 移動 //
	// 左
	if (GetKeyboardPress(DIK_A))
	{
		// 奥
		if (GetKeyboardPress(DIK_W))
		{
			g_player.rotLast.y = -D3DX_PI * 0.25f + pCamera->rot.y;

			g_player.move.x += sinf(D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// 手前
		else if (GetKeyboardPress(DIK_S))
		{
			g_player.rotLast.y = -D3DX_PI * 0.75f + pCamera->rot.y;

			g_player.move.x += sinf(D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// 左
		else
		{
			g_player.rotLast.y = -D3DX_PI * 0.5f + pCamera->rot.y;


			g_player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
	}
	// 右
	else if (GetKeyboardPress(DIK_D))
	{
		// 奥
		if (GetKeyboardPress(DIK_W))
		{
			g_player.rotLast.y = D3DX_PI * 0.25f + pCamera->rot.y;

			g_player.move.x += sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// 手前
		else if (GetKeyboardPress(DIK_S))
		{
			g_player.rotLast.y = D3DX_PI * 0.75f + pCamera->rot.y;

			g_player.move.x += sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
		// 右
		else
		{
			g_player.rotLast.y = D3DX_PI * 0.5f + pCamera->rot.y;

			g_player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
			g_player.move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * PLAYER_KEYMOVE;
		}
	}
	// 奥に行く
	else if (GetKeyboardPress(DIK_W))
	{
		g_player.rotLast.y = D3DX_PI * 0.0f + pCamera->rot.y;
		g_player.move.x += sinf(-D3DX_PI * 1.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
		g_player.move.z += cosf(-D3DX_PI * 1.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
	}
	// 手前に行く
	else if (GetKeyboardPress(DIK_S))
	{
		g_player.rotLast.y = D3DX_PI * 1.0f + pCamera->rot.y;

		g_player.move.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
		g_player.move.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * PLAYER_KEYMOVE;
	}

	// パッド用 //
	int nValueH, nValueV;	// ゲームパッドのスティック情報の取得用
	float fMove;			// 移動速度
	float fAngle;			// スティック角度の計算用変数
	fAngle = 1.0f;

	// ゲームパッドのスティック情報を取得
	GetGamepadStickLeft(0, &nValueH, &nValueV);

	/* プレイヤー移動 */
	// ゲームパッド移動
	if (nValueH != 0 || nValueV != 0)
	{
		// 角度の計算
		fAngle = atan2f((float)nValueH, (float)nValueV);

		if (fAngle  > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2;
		}
		else if (fAngle  < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2;
		}

		// 速度の計算
		if (abs(nValueH) > abs(nValueV))
		{
			fMove = (abs(nValueH) * PLAYER_JOYMOVE) / 32768.0f;
		}
		else
		{
			fMove = (abs(nValueV) * PLAYER_JOYMOVE) / 32768.0f;
		}

		g_player.rotLast.y = fAngle + pCamera->rot.y;

		// スティックの角度によってプレイヤー移動
		g_player.move.x -= sinf(fAngle + pCamera->rot.y) * (PLAYER_JOYMOVE / 2 + fMove);
		g_player.move.z -= cosf(fAngle + pCamera->rot.y) * (PLAYER_JOYMOVE / 2 + fMove);
	}


	// ジャンプ
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
		// 移動時
		if (g_player.move.x >= 0.1f || g_player.move.x <= -0.1f ||
			g_player.move.z >= 0.1f || g_player.move.z <= -0.1f)
		{
			// 移動状態
			g_player.nMotionType = PLAYERMOSIONSET_MOVE;
		}

		// 通常時
		else
		{
			g_player.nMotionType = PLAYERMOSIONSET_NEUTRAL;
		}
	}

	// 現在回転差分
	g_player.rotbetween.y = g_player.rotLast.y - g_player.rot.y;

	// 限度調整
	g_player.rotbetween.y = Rot_One_Limit(g_player.rotbetween.y);

	// 回転移動
	g_player.rot.y += g_player.rotbetween.y * 0.1f;

	// 限度調整
	g_player.rot.y = Rot_One_Limit(g_player.rot.y);

	// 重力
	g_player.move.y -= PLAYER_G;	// 移動量
	g_player.pos += g_player.move;

	// 抵抗力
	g_player.move.x *= PLAYER_RESISTANCE;
	g_player.move.z *= PLAYER_RESISTANCE;
}

// ------------------------------------------------------------------------------------------
// モーション処理
// ------------------------------------------------------------------------------------------
void Update_MotionPlayer(void)
{
	// 変数宣言
	int	nCntModel;			// カウントモデル

	// 現在のモーションと前回のモーションを比較
	if (g_player.nMotionType != g_nPlayerMotionOld)
	{
		g_player.nFram = 0;

		g_player.nCntKeySet = 0;
	}

	// パーツカウント
	for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// フレーム数が０の場合
		if (g_player.nFram == 0)
		{
			// フレーム数の規定値が０ではない場合
			if (g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram != 0)
			{
				// 最終地点と現在地点の差分分割
				g_player.model[nCntModel].rotKeyBetween =
					(g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].rot -
						g_player.model[nCntModel].rot) /
						(float)g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram;

				// 最終地点 と現在地点の差分
				g_player.model[nCntModel].posKeyBetween =
					(g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].pos -
						g_player.model[nCntModel].pos) /
						(float)g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram;
			}

			// フレーム数の規定値が０の場合
			else
			{
				// 最終地点を現在地点に代入
				g_player.model[nCntModel].rot =
					g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].rot;

				// 最終地点を現在地点に代入
				g_player.model[nCntModel].pos =
					g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].key[nCntModel].pos;

				// 差分の初期化
				g_player.model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 差分の初期化
				g_player.model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		// 回転する
		g_player.model[nCntModel].rot +=
			g_player.model[nCntModel].rotKeyBetween;

		// 移動する
		g_player.model[nCntModel].pos +=
			g_player.model[nCntModel].posKeyBetween;
	}

	// フレーム数が規定値と同じになったら
	if (g_player.nFram ==
		g_playerKey[g_player.nMotionType].KeyInfo[g_player.nCntKeySet].nFram)
	{
		// キーセットの更新
		g_player.nCntKeySet++;

		// フレーム数初期化
		g_player.nFram = 0;

		// キーセット数が規定値と同じになったら
		if (g_player.nCntKeySet == g_playerKey[g_player.nMotionType].nNumKey)
		{
			// ループしないとき
			if (g_playerKey[g_player.nMotionType].nLoop == 0)
			{
				// キーセット数初期化
				g_player.nCntKeySet = 0;

				g_player.nMotionType = PLAYERMOSIONSET_NEUTRAL;
			}

			// ループする時
			else
			{
				// キーセット数初期化
				g_player.nCntKeySet = 0;
			}
		}
	}

	// フレーム数が規定値と同じじゃないとき
	else
	{
		// フレーム数カウントアップ
		g_player.nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// 発射処理
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

			// ダメージ計算
			g_player.status.nLastMP -= PLAYER_MPDAMAGE;

			// ゲージの更新
			ChangeHpGauge(HPGAUGESYSTEM_MP, (float)g_player.status.nMP, (float)g_player.status.nLastMP);

			// 現在のHPに代入
			g_player.status.nMP = g_player.status.nLastMP;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 普通攻撃処理
// ------------------------------------------------------------------------------------------
void Update_NormalAttackPlayer(void)
{
	if (GetKeyboardTrigger(DIK_J) ||
		GetJoyboardTrigger(0, JOYPADKEY_RIGHT_1))
	{
		// SE
		PlaySound(SOUND_LABEL_SE_ATTACK);

		// 普通攻撃モーション
		g_player.nMotionType = PLAYERMOSIONSET_NORMALATACK;

		// 軌跡の発生
		OccurrenceTrajectory(
			g_playerKey[PLAYERMOSIONSET_NORMALATACK].Orbit_nIdTrajectory, 0);
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionPlayer(void)
{
	// 変数宣言
	GIRAFFE * pGiraffe = GetGiraffe();					// キリンの情報取得	
	SHOGIRA * pShogira = GetShogira();					// キリンの情報取得	
	BUFFALO * pBuffalo = GetBuffalo();					// バッファローの情報取得
	KING_RION * pKing_Rion = GetKing_Rion();			// 百獣の王の情報取得
	MATERIALS * pMaterials = GetMaterials();			// 素材の情報取得
	D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 0.0f, 10.0f);

	int nCntGiraffe = 0;
	int nCntShogira = 0;
	int nCntBuffalo = 0;
	int nCntKing_Rion = 0;
	int nCntMatarials = 0;

	// 床の当たり判定 //
	if (ColisionFLOOR(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[0].size,
		&g_player.pMeshField))
	{
		// 着地モーション
		if (g_player.bjump == true)
		{
			g_player.nMotionType = PLAYERMOSIONSET_LANDING;
			g_player.bjump = false;
		}
	}

	// 水の当たり判定 //
	if (ColisionWATER(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[0].size,
		&g_player.pWater))
	{
		g_player.bjump = false;
	}

	// 素材の当たり判定 //
	switch (ColisionMaterials(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[0].size,
		&g_player.pMaterials))
	{
		// 上の当たり判定
	case 0:
		if (g_player.bjump == true)
		{
			g_player.nMotionType = PLAYERMOSIONSET_LANDING;
			g_player.bjump = false;
		}
		break;

		// 下の当たり判定
	case 1:
		break;

		// 前後左右の当たり判定
	case 2:
		break;

	default:
		break;
	}

	// 壁の当たり判定
	if (ColisionWall(&g_player.pos,
		&g_player.posold,
		&g_player.move,
		&g_player.model[1].size,
		&g_player.pWall))
	{

	}

	// 素材カウント
	for (nCntMatarials = 0; nCntMatarials < MATERIALS_MAX; nCntMatarials++, pMaterials++)
	{
		if (pMaterials->bUse == true)
		{
			// チュートリアル用
			if (GetMode() == MODE_TUTORIAL)
			{

				// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
				if (Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
					pMaterials->fRadius,
					g_player.fLength,
					0.0f) >=
					Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
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

			// 主人公の攻撃時の当たり判定
			if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
			{

				if (pMaterials->bHit == false)
				{
					// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
					if (Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
						pMaterials->fRadius,
						g_playerKey[g_player.nMotionType].Collision_Radius,
						0.0f) >=
						Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
							pMaterials->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41,
							pMaterials->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42,
							pMaterials->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
					{
						// 角度
						float frot;
						// 赤木
						if (pMaterials->nDrawType ==
							MATERIALS_OBJECT_01_WOOD_RED)
						{
							// プレイヤーがいる角度
							frot = atan2f(g_player.pos.x - pMaterials->pos.x, g_player.pos.z - pMaterials->pos.z);
							// アイテム設置
							SetMaterials(pMaterials->pos +
								D3DXVECTOR3(
									sinf(frot) * 50.0f,
									80.0f,
									cosf(frot) * 50.0f),	// 位置
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 回転量
								D3DXVECTOR3(5.0f, 5.0f, 5.0f),	// 移動量
								MATERIALS_ITEM_00_APPLE,									// 描画タイプ
								50,									// HP回復量
								0);									// MP回復量

						}

						// オレンジ木
						else if (pMaterials->nDrawType ==
							MATERIALS_OBJECT_02_WOOD_ORANGE)
						{
							// プレイヤーがいる角度
							frot = atan2f(g_player.pos.x - pMaterials->pos.x, g_player.pos.z - pMaterials->pos.z);
							// アイテム設置
							SetMaterials(pMaterials->pos +
								D3DXVECTOR3(
									sinf(frot) * 50.0f,
									80.0f,
									cosf(frot) * 50.0f),		// 位置
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 回転量
								D3DXVECTOR3(5.0f, 5.0f, 5.0f),	// 移動量
								MATERIALS_ITEM_01_ACTINIDIA,	// 描画タイプ
								0,								// HP回復量
								50);							// MP回復量
						}
						// 当たった判定
						pMaterials->bHit = true;
						// ドーム削除
						DeleteDome(pMaterials->nIdxDome);
					}
				}
			}

		}
	}

	// キリンカウント
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++, pGiraffe++)
	{
		if (pGiraffe->bUse == true)
		{
			// チュートリアル用
			if (GetMode() == MODE_TUTORIAL)
			{
				// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
				if (Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
					pGiraffe->fLength,
					g_player.fLength,
					0.0f) >=
					Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
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
				// 主人公の攻撃時の当たり判定
				if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
				{

					if (pGiraffe->status.bColition == false)
					{
						// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
						if (Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
							pGiraffe->fLength,
							g_playerKey[g_player.nMotionType].Collision_Radius,
							0.0f) >=
							Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
								pGiraffe->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41,
								pGiraffe->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42,
								pGiraffe->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
						{
							// キリンのダメージ処理
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

	// キリンカウント
	for (nCntShogira = 0; nCntShogira < MAX_GIRAFFE; nCntShogira++, pShogira++)
	{
		if (pShogira->bUse == true)
		{
			if (pShogira->status.nStatus != GIRAFFESTATUS_DIE)
			{
				// 主人公の攻撃時の当たり判定
				if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
				{

					if (pShogira->status.bColition == false)
					{
						// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
						if (Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
							pShogira->fLength,
							g_playerKey[g_player.nMotionType].Collision_Radius,
							0.0f) >=
							Addition_Circle(											// 円の足し算(キリンとプレイヤーの半径)
								pShogira->pos.x - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._41,
								pShogira->pos.y - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._42,
								pShogira->pos.z - g_player.model[g_playerKey[g_player.nMotionType].Collision_nParts].mtxWorldModel._43))
						{
							// キリンのダメージ処理
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

	// バッファローカウント
	for (nCntBuffalo = 0; nCntBuffalo < MAX_GIRAFFE; nCntBuffalo++, pBuffalo++)
	{
		if (pBuffalo->bUse == true)
		{
			// 主人公の攻撃時の当たり判定
			if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
			{

				if (pBuffalo->status.nStatus != BUFFALOSTATUS_DIE)
				{
					if (pBuffalo->status.bColition == false)
					{
						// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
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
							// キリンのダメージ処理
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

	// キリンカウント
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_GIRAFFE; nCntKing_Rion++, pKing_Rion++)
	{
		if (pKing_Rion->bUse == true)
		{
			// 主人公の攻撃時の当たり判定
			if (g_player.nMotionType == PLAYERMOSIONSET_NORMALATACK)
			{

				if (pKing_Rion->status.nStatus != GIRAFFESTATUS_DIE)
				{
					if (pKing_Rion->status.bColition == false)
					{
						// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
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
							// キリンのダメージ処理
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
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// 計算用マトリックス
	D3DXMATERIAL		*pMat;					// 現在のマテリアル保存
	D3DMATERIAL9		matDef;					// マテリアルデータのポインタ
	int nCntModel;

	// 使用状態
	if (g_player.bUse)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.mtxWorldPlayer);

		// 回転を反映
		// スケールを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_player.rot.y,
			g_player.rot.x,
			g_player.rot.z);

		// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer,
			&g_player.mtxWorldPlayer, &mtxRot);


		// 位置を反映 //
		// 平行移動行列作成(オフセット)
		D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
			g_player.pos.x,
			g_player.pos.y,
			g_player.pos.z);	// それぞれの移動量

		// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
		D3DXMatrixMultiply(&g_player.mtxWorldPlayer,	// 1
			&g_player.mtxWorldPlayer,					// 2
			&mtxTrans);									// 3

		// カウントモデル
		for (nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_player.model[nCntModel].mtxWorldModel);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_player.model[nCntModel].rot.y,
				g_player.model[nCntModel].rot.x,
				g_player.model[nCntModel].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,
				&g_player.model[nCntModel].mtxWorldModel, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_player.model[nCntModel].pos.x,
				g_player.model[nCntModel].pos.y,
				g_player.model[nCntModel].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,	// 1
				&g_player.model[nCntModel].mtxWorldModel,					// 2
				&mtxTrans);									// 3


			// [すべての親]じゃない場合
			if (g_player.model[nCntModel].nIdxModelParent != -1)
			{
				// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
				D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,
					&g_player.model[nCntModel].mtxWorldModel,
					&g_player.model[g_player.model[nCntModel].nIdxModelParent].mtxWorldModel);
			}

			else
			{
				// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
				D3DXMatrixMultiply(&g_player.model[nCntModel].mtxWorldModel,
					&g_player.model[nCntModel].mtxWorldModel,
					&g_player.mtxWorldPlayer);
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_player.model[nCntModel].mtxWorldModel);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_player.model[nCntModel].paBuffMat->GetBufferPointer();

			// テクスチャー設定
			pDevice->SetTexture(0, g_paTexturePlayer[0]);

			// カウントマテリアル
			for (int nCntMat = 0; nCntMat < (int)g_player.model[nCntModel].nNumMat; nCntMat++,pMat++)
			{
				// 本体のカラー情報更新
				pMat->MatD3D.Diffuse.a = g_player.diffuse;
						
				// マテリアルの設定
				pDevice->SetMaterial(&pMat->MatD3D);

				// 描画
				g_player.model[nCntModel].paMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

// ------------------------------------------------------------------------------------------
// ステータスアップ処理
// ------------------------------------------------------------------------------------------
void StatusUpPlayer(
	int nUpLife,		// HPアップ
	int nUpMp,			// MPアップ
	int nUpAttack)		// 攻撃力アップ
{
	// SE
	PlaySound(SOUND_LABEL_SE_EAT);

	// 最大HP変化
	g_player.status.nMaxLife += nUpLife;

	// 最大HPゲージの変化
	ChangeMaxHpGauge(HPGAUGESYSTEM_HP,(float)g_player.status.nMaxLife);

	// 最大MP変化
	g_player.status.nMaxMP += nUpMp;

	// 最大MPゲージの変化
	ChangeMaxHpGauge(HPGAUGESYSTEM_MP, (float)g_player.status.nMaxMP);

	// 攻撃力変化
	g_player.status.nAttack += nUpAttack;
}

// ------------------------------------------------------------------------------------------
// 回復処理
// ------------------------------------------------------------------------------------------
void RecoveryPlayer(int nRecoveryLife, int nRecoveryMP)
{
	// ダメージ計算
	g_player.status.nLastLife += nRecoveryLife;

	// 最大HPを超えたら
	if (g_player.status.nLastLife >= g_player.status.nMaxLife)
	{
		g_player.status.nLastLife = g_player.status.nMaxLife;
	}

	// ゲージの更新
	ChangeHpGauge(HPGAUGESYSTEM_HP, (float)g_player.status.nLife, (float)g_player.status.nLastLife);

	// 現在のHPに代入
	g_player.status.nLife = g_player.status.nLastLife;

	// 魔力が回復
	g_player.status.nLastMP += nRecoveryMP;

	// 最大MPを超えたら
	if (g_player.status.nLastMP >= g_player.status.nMaxMP)
	{
		g_player.status.nLastMP = g_player.status.nMaxMP;
	}

	// ゲージの更新
	ChangeHpGauge(HPGAUGESYSTEM_MP, (float)g_player.status.nMP, (float)g_player.status.nLastMP);

	// 現在のMPに代入
	g_player.status.nMP = g_player.status.nLastMP;
}

// ------------------------------------------------------------------------------------------
// 位置情報の処理
// ------------------------------------------------------------------------------------------
void PlayerPosition(void)
{
	// ボス
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		g_player.pos = D3DXVECTOR3(300.0f, 20.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

// ------------------------------------------------------------------------------------------
// ダメージを受けたときの処理
// ------------------------------------------------------------------------------------------
void HitPlayer(int fDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 攻撃が当たっている
	g_player.status.bColition = true;

	// ダメージ計算
	g_player.status.nLastLife -= fDamage;

	// ゲージの更新
  	ChangeHpGauge(HPGAUGESYSTEM_HP,(float)g_player.status.nLife, (float)g_player.status.nLastLife);

	// 現在のHPに代入
	g_player.status.nLife = g_player.status.nLastLife;

	PlaySound(SOUND_LABEL_SE_COLISION);

	// HPが0の時
	if (g_player.status.nLife <= 0)
	{
		// 死亡状態
		g_player.status.nState = PLAYER_STATE_DIE;

		// 目標回転量
		g_player.rotLast.z = D3DX_PI / 2;

		// 移動量
		g_player.move.y = 5.0f;

		for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
		{
			// X・Zの角度
			fAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// Yの角度
			fYAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// 移動方向
			move =
				D3DXVECTOR3(sinf(fAngle) * 5.0f,
					cosf(fYAngle) * 5.0f,
					cosf(fAngle) * 5.0f);

			// カラー
			col =
				D3DXCOLOR(
					0.5f + float(rand() % 50) / 100.0f,
					0.5f - float(rand() % 50) / 100.0f,
					0.0f,
					1.0f);

			// エフェクトの設定
			SetEffect(
				D3DXVECTOR3(													// 位置
					g_player.pos.x,
					g_player.pos.y,
					g_player.pos.z),

				move,															// 移動量
				col,															// 色
				float(rand() % 10),												// サイズ
				-0,																// 減少サイズ
				0.0f,															// 重力
				10,																// エフェクトライフ
				0,																// タイプ
				0);
		}
	}

	else
	{
		// ダメージ状態
		g_player.status.nState = PLAYER_STATE_DAMAGE;

		// 攻撃が当たっているかどうか
		g_player.status.bColition = true;

		for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
		{
			// X・Zの角度
			fAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// Yの角度
			fYAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// 移動方向
			move =
				D3DXVECTOR3(sinf(fAngle) * 5.0f,
					cosf(fYAngle) * 5.0f,
					cosf(fAngle) * 5.0f);

			// カラー
			col =
				D3DXCOLOR(
					0.5f + float(rand() % 50) / 100.0f,
					0.5f - float(rand() % 50) / 100.0f,
					0.0f,
					1.0f);

			// エフェクトの設定
			SetEffect(
				D3DXVECTOR3(													// 位置
					g_player.pos.x,
					g_player.pos.y,
					g_player.pos.z),

				move,															// 移動量
				col,															// 色
				float(rand() % 10),												// サイズ
				-0,																// 減少サイズ
				0.0f,															// 重力
				10,																// エフェクトライフ
				0,																// タイプ
				0);
		}
	}
}

// ------------------------------------------------------------------------------------------
// プレイヤーモーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadPlayerMotion(void)
{
	// ファイルポイント
	FILE *pFile;
	
	// 変数宣言
	int	nCntMotion = 0;		// モーションカウント
	int	nCntKeySet = 0;		// フレーム数
	int	nCntKey = 0;		// モデル数
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/PLAYER/player_mosion.txt", "r");
	
	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テキストに文字を代入
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// モーションセットが来たら
				if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// モデルの初期化
					nCntKeySet = 0;

					// エンドモーションセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_playerKey[nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_playerKey[nCntMotion].nNumKey);
						}

						// 当たり判定情報読み込み
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

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_playerKey[nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
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

							// キャラクタータイプ
							g_playerKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_PLAYER;

							// 使用状態
							g_playerKey[nCntMotion].Orbit_bUse = true;
						}

						// キー情報読み込み
						else if (strcmp(cHeadText, "KEYSET") == 0)
						{
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// フレーム情報読み込み
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].nFram);
							}

							// キーの初期化
							nCntKey = 0;

							// エンドキーセットが来るまでループ
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// キー情報読み込み
								if (strcmp(cHeadText, "KEY") == 0)
								{

									// エンドキーが来るまでループ
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);

										// 位置情報読み込み
										if (strcmp(cHeadText, "POS") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos.x,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos.y,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos.z);

											// パーツの位置情報を加える
											g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos += g_player.model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot.x,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot.y,
												&g_playerKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot.z);
										}
									}

									// カウントキー
									nCntKey++;
								}
							}

							// カウントモデル
							nCntKeySet++;
						}
					}

					// カウントプレイヤー
					nCntMotion++;
				}
			}
		}

		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "プレイヤーデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// プレイヤー読み込み処理
// ------------------------------------------------------------------------------------------
void LoadPlayer(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;
	int nCntPlayer = 0;
	int	nCntModel = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/PLAYER/player.txt", "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// モデルセットが来たら
				else if (strcmp(cHeadText, "PLAYERSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_PLAYERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "nParent") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&g_player.model[nCntModel].nIdxModelParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "pos") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_player.model[nCntModel].pos.x,
								&g_player.model[nCntModel].pos.y, 
								&g_player.model[nCntModel].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "rot") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_player.model[nCntModel].rot.x, &g_player.model[nCntModel].rot.y, &g_player.model[nCntModel].rot.z);
						}
					}
					// モデルの更新
					nCntModel++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "プレイヤーデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// プレイヤー情報処理
// ------------------------------------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_player;
}

// ------------------------------------------------------------------------------------------
// プレイヤーのモデル情報処理
// ------------------------------------------------------------------------------------------
MODEL * GetPlayerModel(void)
{
	return &g_player.model[0];
}

// ------------------------------------------------------------------------------------------
// プレイヤーの位置情報処理
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_Pos(void)
{
	return g_player.pos;
}

// ------------------------------------------------------------------------------------------
// プレイヤーの前回の位置情報処理
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_PosOld(void)
{
	return g_player.posold;
}

// ------------------------------------------------------------------------------------------
// プレイヤーの回転情報処理
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_Rot(void)
{
	return g_player.rot;
}

// ------------------------------------------------------------------------------------------
// プレイヤー半径情報処理
// ------------------------------------------------------------------------------------------
float GetPlayer_Radius(void)
{
	return g_player.status.fLength;
}

// ------------------------------------------------------------------------------------------
// プレイヤーステータス情報処理
// ------------------------------------------------------------------------------------------
PLAYER_STATUS * GetPlayerStatus(void)
{
	return &g_player.status;
}
