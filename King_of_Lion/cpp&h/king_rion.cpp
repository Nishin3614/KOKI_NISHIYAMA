// ------------------------------------------------------------------------------------------
//
// キリン処理 [king_rion.cpp]
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
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_KING_RION (1)
#define KING_RION_HP (100)
#define BUFFALO_ATTACK (20)					// 攻撃力
#define KING_RION_NORMALATTACK_DECISION (100)
#define KING_RION_SHOT_DECISION (500)
#define KING_RION_ATTACK_COUNT (70)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
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
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureKing_Rion[TEXTURE_KING_RION] = {};	// テクスチャへのポインタ

KING_RION			g_king_rion[MAX_KING_RION];						// 百獣の王情報

MODEL				g_king_rionModel[KING_RIONTYPE_MAX];			// キリン情報

MOSION_KING_RION	g_king_rionKey[KING_RIONMOSIONSET_MAX];			// モーション情報

int					g_king_rionTotal;								// 百獣の王撃破数合計

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitKing_Rion(void)
{
	g_king_rionTotal = 0;
	// 百獣の王パーツ情報
	Init_PartsKing_Rion();

	// 百獣の王情報
	Init_King_RionInfo();

	// 百獣の王モーション情報
	Init_MotionKing_Rion();
}

// ------------------------------------------------------------------------------------------
// 百獣の王パーツ情報処理
// ------------------------------------------------------------------------------------------
void Init_PartsKing_Rion(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	int		nCntModel;							// キリンのカウント
		
	// キリンファイル名定義
	char *paKing_RionFile[KING_RIONTYPE_MAX] =
	{
		{ "data/MODEL/king_rion/00_rion_body.x" },				// [0]体
		{ "data/MODEL/king_rion/01_rion_head_joint.x" },		// [1]頭関節
		{ "data/MODEL/king_rion/02_rion_head.x" },				// [2]頭
		{ "data/MODEL/king_rion/03_rion_right_hand_joint.x" },	// [3]右腕関節
		{ "data/MODEL/king_rion/04_rion_right_hand.x" },		// [4]右腕
		{ "data/MODEL/king_rion/05_rion_left_hand_joint.x" },	// [5]左腕関節
		{ "data/MODEL/king_rion/06_rion_left_hand.x" },			// [6]左腕
		{ "data/MODEL/king_rion/07_rion_right_leg_joint.x" },	// [7]右足関節
		{ "data/MODEL/king_rion/08_rion_right_leg.x" },			// [8]右足
		{ "data/MODEL/king_rion/09_rion_left_leg_joint.x" },	// [9]左足関節
		{ "data/MODEL/king_rion/10_rion_left_leg.x" },			// [10]左足
		{ "data/MODEL/king_rion/11_rion_tail_joint.x" },		// [11]しっぽの関節
		{ "data/MODEL/king_rion/12_rion_tail.x" },				// [12]しっぽ
	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_KING_RION] =
	{
		{ "" },
	};

	// パーツの読み込み //
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

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			paKing_RionFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_king_rionModel[nCntModel].paBuffMat,
			NULL,
			&g_king_rionModel[nCntModel].nNumMat,
			&g_king_rionModel[nCntModel].paMesh);

		// キリンの頂点座標の最大値・最小値の設定
		int nNumVertex;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

		// 頂点数を取得
		nNumVertex = g_king_rionModel[nCntModel].paMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_king_rionModel[nCntModel].paMesh->GetFVF());

		// 頂点バッファをロック
		g_king_rionModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// 頂点数分繰り返す
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// 番地情報のD3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
			// x //
			// 最小値
			if (g_king_rionModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_king_rionModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// 最大値
			if (g_king_rionModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_king_rionModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// 最小値
			if (g_king_rionModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_king_rionModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// 最大値
			if (g_king_rionModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_king_rionModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// 最小値
			if (g_king_rionModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_king_rionModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// 最大値
			if (g_king_rionModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_king_rionModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// サイズポインタを進める
			pVertexBuffer += sizeFVF;
		}

		// アンロック
		g_king_rionModel[nCntModel].paMesh->UnlockVertexBuffer();

		// 最大値と最小値の距離
		g_king_rionModel[nCntModel].size = g_king_rionModel[nCntModel].vtxMaxMaterials - g_king_rionModel[nCntModel].vtxMinMaterials;
	}

	// パーツ読込
	LoadKing_RionModel();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureKing_Rion[0]);
}

// ------------------------------------------------------------------------------------------
// 百獣の王情報処理
// ------------------------------------------------------------------------------------------
void Init_King_RionInfo(void)
{
	// 変数宣言
	int		nCntKing_Rion;	// プレイヤーカウント
	int		nCntModel;		// キリンのカウント

	// 百獣の王の初期設定 //
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
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

		// カラー情報
		g_king_rion[nCntKing_Rion].nCntCor = 0;
		g_king_rion[nCntKing_Rion].diffuse = 1.0f;

		// ステータス情報
		g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_NEUTRAL;
		g_king_rion[nCntKing_Rion].status.nCntStatus = 0;
		g_king_rion[nCntKing_Rion].status.nAttack = 10;
		g_king_rion[nCntKing_Rion].status.nLife = KING_RION_HP;
		g_king_rion[nCntKing_Rion].status.bColition = false;
		g_king_rion[nCntKing_Rion].status.bAttack = false;
		g_king_rion[nCntKing_Rion].status.bShot = false;
		g_king_rion[nCntKing_Rion].status.bRot = false;
		g_king_rion[nCntKing_Rion].status.bMove = false;

		// パーツごとのカウント
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

	// ロード百獣の王
	LoadKing_Rion();
}

// ------------------------------------------------------------------------------------------
// 百獣の王モーション処理
// ------------------------------------------------------------------------------------------
void Init_MotionKing_Rion(void)
{
	// 変数宣言
	int		nCntKing_Rion;						// 百獣の王カウント

												// モーション用変数
	int		nCntMotion;							// モーションカウント
	int		nCntKeySet;							// モーションカウント
	int		nCntKey;							// キーカウント

												// モーションカウント //
	for (nCntMotion = 0; nCntMotion < KING_RIONMOSIONSET_MAX; nCntMotion++)
	{
		// モーションの初期設定
		g_king_rionKey[nCntMotion].nNumKey = 0;		// ナンバーキー
		g_king_rionKey[nCntMotion].nLoop = 0;		// ループの有無

												// 当たり判定用 //
		g_king_rionKey[nCntMotion].Collision_nParts = 0;		// パーツ番号
		g_king_rionKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// オフセット
		g_king_rionKey[nCntMotion].Collision_Radius = 0.0f;		// 円の半径
		g_king_rionKey[nCntMotion].Collision_StartFram = 0;		// 開始フレーム数
		g_king_rionKey[nCntMotion].Collision_EndFram = 0;		// 終了フレーム数
		g_king_rionKey[nCntMotion].Collision_Damage = 0;		// ダメージ数

		// 軌跡用 //
		g_king_rionKey[nCntMotion].Orbit_nParts = 0;			// パーツ番号
		g_king_rionKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// オフセット
		g_king_rionKey[nCntMotion].Orbit_Mesh = 0;				// 軌跡の線
		g_king_rionKey[nCntMotion].Orbit_StartFram = 0;			// 開始フレーム数
		g_king_rionKey[nCntMotion].Orbit_EndFram = 0;			// 終了フレーム数
		g_king_rionKey[nCntMotion].Orbit_nType =
			TRAJECTORYTYPE_KING_RION;							// 軌跡のタイプ
		g_king_rionKey[nCntMotion].Orbit_nIdTrajectory = -1;	// 軌跡の番号
		g_king_rionKey[nCntMotion].Orbit_bUse = false;			// 軌跡の使用状態

		for (nCntKeySet = 0; nCntKeySet < KING_RION_KEYSET_MAX; nCntKeySet++)
		{
			g_king_rionKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// キーごとに初期化
			for (nCntKey = 0; nCntKey < KING_RIONTYPE_MAX; nCntKey++)
			{
				g_king_rionKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_king_rionKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// プレイヤーモーション読み込み
	LoadKing_RionMosion();

	// 百獣の王の初期設定 //
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		if (g_king_rion[nCntKing_Rion].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < KING_RIONMOSIONSET_MAX; nCntMotion++)
			{
				// 使用状態
				if (g_king_rionKey[nCntMotion].Orbit_bUse == true)
				{
					// 軌跡の設定
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
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitKing_Rion(void)
{
	// キリンデータの開放
	for (int nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_king_rionModel[nCntModel].paMesh != NULL)
		{
			g_king_rionModel[nCntModel].paMesh->Release();
			g_king_rionModel[nCntModel].paMesh = NULL;
		}

		// マテリアルの開放
		if (g_king_rionModel[nCntModel].paBuffMat != NULL)
		{
			g_king_rionModel[nCntModel].paBuffMat->Release();
			g_king_rionModel[nCntModel].paBuffMat = NULL;
		}
	}

	// キリンテクスチャの開放
	for (int nCntTexture = 0; nCntTexture < TEXTURE_KING_RION; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTextureKing_Rion[nCntTexture] != NULL)
		{
			g_paTextureKing_Rion[nCntTexture]->Release();
			g_paTextureKing_Rion[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateKing_Rion(void)
{
	// カウント百獣の王
	int nCntKing_Rion;
	PLAYER *pPlayer = GetPlayer();

	// 変数宣言
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// 百獣の王のカウント
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		if (g_king_rion[nCntKing_Rion].bUse)
		{
			// プレイヤーと百獣の王との距離
			diffpos = pPlayerPos - g_king_rion[nCntKing_Rion].pos;

			// 現在地保存
			g_king_rion[nCntKing_Rion].posold = g_king_rion[nCntKing_Rion].pos;

			// モーションを保存
			g_king_rion[nCntKing_Rion].nMotionTypeOld = g_king_rion[nCntKing_Rion].nMotionType;

			// 状態処理
			Update_StateKing_Rion(nCntKing_Rion);

			// 死んでいない状態
			if (g_king_rion[nCntKing_Rion].status.nStatus != KING_RIONSTATUS_DIE)
			{
				// 攻撃行動範囲
				if ((KING_RION_NORMALATTACK_DECISION * KING_RION_NORMALATTACK_DECISION) >=	// 攻撃行動の円範囲
					diffpos.x * diffpos.x +												// x軸の円
					diffpos.y * diffpos.y +												// y軸の円
					diffpos.z * diffpos.z)												// z軸の円
				{
					// 普通攻撃の処理
					Update_NormalAttackKing_Rion(nCntKing_Rion);
				}

				// プレイヤー追跡
				else
				{
					if (g_king_rion[nCntKing_Rion].status.nStatus !=
						KING_RIONMOSIONSET_SHOT)
					{
						// 回転処理
						Update_RotKing_Rion(nCntKing_Rion);

						// 移動処理
						Update_MoveKing_Rion(nCntKing_Rion);
					}

					// 弾の処理
					Update_ShotKing_Rion(nCntKing_Rion);
				}

				// 攻撃の当たり判定
				Update_DecisionKing_Rion(nCntKing_Rion);
			}
			g_king_rion[nCntKing_Rion].move.y -= 0.5f;
			g_king_rion[nCntKing_Rion].pos.y += g_king_rion[nCntKing_Rion].move.y;

			// 当たり判定
			Update_ColisionKing_Rion(nCntKing_Rion);

			// 死んでいない状態
			if (g_king_rion[nCntKing_Rion].status.nStatus != KING_RIONSTATUS_DIE)
			{
				// モーション処理
				Update_MosionKing_Rion(nCntKing_Rion);
			}

			// 影の位置を設定
			SetPositionShadow(
				g_king_rion[nCntKing_Rion].nIdxShadow, 
				g_king_rion[nCntKing_Rion].pos +
				D3DXVECTOR3(
					sinf(-g_king_rion[nCntKing_Rion].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_king_rion[nCntKing_Rion].rot.y) * 10.0f));

			// 攻撃が当たっている
			if (g_king_rion[nCntKing_Rion].status.bColition == true)
			{
				// 攻撃が終わったら
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_king_rion[nCntKing_Rion].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 状態処理
// ------------------------------------------------------------------------------------------
void Update_StateKing_Rion(int nCntKing_Rion)
{
	switch (g_king_rion[nCntKing_Rion].status.nStatus)
	{
	case KING_RIONSTATUS_NEUTRAL:
		g_king_rion[nCntKing_Rion].diffuse = 1.0f;
		break;

	case KING_RIONSTATUS_DAMAGE:
		// カラーの状態カウントアップ
		g_king_rion[nCntKing_Rion].nCntCor++;

		// 表示状態
		if (g_king_rion[nCntKing_Rion].nCntCor % 10 == 0)
		{
			g_king_rion[nCntKing_Rion].diffuse = 1.0f;
		}

		// 非表示状態
		else if (g_king_rion[nCntKing_Rion].nCntCor % 5 == 0)
		{
			g_king_rion[nCntKing_Rion].diffuse = 0.5f;
		}

		if (g_king_rion[nCntKing_Rion].nCntCor == 50)
		{
			// カラーの状態カウント初期化
			g_king_rion[nCntKing_Rion].nCntCor = 0;

			// 百獣の王の状態をノーマルへ
			g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_NEUTRAL;
		}

		break;

	case KING_RIONSTATUS_DIE:
		// 目標回転量から現在の回転量の差分
		g_king_rion[nCntKing_Rion].rotbetween.z =
			(g_king_rion[nCntKing_Rion].rotLast.z - g_king_rion[nCntKing_Rion].rot.z) / 10;

		// 回転
		g_king_rion[nCntKing_Rion].rot.z += g_king_rion[nCntKing_Rion].rotbetween.z;

		// 重力
		g_king_rion[nCntKing_Rion].move.y -= 0.5f;	// 移動量
		g_king_rion[nCntKing_Rion].pos.y += g_king_rion[nCntKing_Rion].move.y;

		// ステータスのカウントアップ
		g_king_rion[nCntKing_Rion].status.nCntStatus++;

		// ステータスのカウントが規定値以上になったら
		if (g_king_rion[nCntKing_Rion].status.nCntStatus == 70)
		{
			// エフェクト用変数
			float fAngle;
			float fYAngle;
			D3DXVECTOR3 move;
			D3DXCOLOR col;

			for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
			{
				// X・Zの角度
				fAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

				// Yの角度
				fYAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

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
						g_king_rion[nCntKing_Rion].pos.x,
						g_king_rion[nCntKing_Rion].pos.y,
						g_king_rion[nCntKing_Rion].pos.z),

					move,															// 移動量
					col,															// 色
					float(rand() % 10),												// サイズ
					-0,																// 減少サイズ
					0.0f,															// 重力
					10,																// エフェクトライフ
					0,																// タイプ
					0);
			}

			// 使用・表示状態OFF
			g_king_rion[nCntKing_Rion].bUse = false;
			g_king_rion[nCntKing_Rion].bDisp = false;

			// 影の消去
			DeleteShadow(g_king_rion[nCntKing_Rion].nIdxShadow);

			// 使用中の軌跡の消去
			DeleteTrajectory(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Orbit_nIdTrajectory);

			// ステータスのカウントの初期化
			g_king_rion[nCntKing_Rion].status.nCntStatus = 0;

			// ステータス状況
			g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_NEUTRAL;

			// アイテム出現
			SetItem(
				g_king_rion[nCntKing_Rion].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// 移動量
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// サイズ
				20.0f,								// 半径
				0,									// テクスチャー
				100);								// 出現時間

			// 撃破数
			g_king_rionTotal++;

			// ゲーム時だけ
			if (GetMode() == MODE_GAME)
			{
				AddScore(1);
			}

			// ゲームクリア状態へ
			SetGameState(GAMESTATE_CLEAR);
		}

		break;
	default:
		break;
	}

}

// ------------------------------------------------------------------------------------------
// 回転処理
// ------------------------------------------------------------------------------------------
void Update_RotKing_Rion(int nCntKing_Rion)
{
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーと百獣の王との距離
	diffpos = pPlayerPos - g_king_rion[nCntKing_Rion].pos;

	// 百獣の王から見てプレイヤーがいる角度
	float	fAngle = (atan2f(diffpos.x, diffpos.z));

	// 目的の回転量
	g_king_rion[nCntKing_Rion].rotLast.y = fAngle + D3DX_PI;

	// 回転量が上限値を超えたら
	if (g_king_rion[nCntKing_Rion].rotLast.y > D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotLast.y += -D3DX_PI * 2;
	}
	else if (g_king_rion[nCntKing_Rion].rotLast.y < -D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotLast.y += D3DX_PI * 2;
	}

	// 目的回転量から現在回転量の差分
	g_king_rion[nCntKing_Rion].rotbetween.y =
		(g_king_rion[nCntKing_Rion].rotLast.y - g_king_rion[nCntKing_Rion].rot.y);

	// 回転量が上限値を超えたら
	if (g_king_rion[nCntKing_Rion].rotbetween.y > D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotbetween.y += -D3DX_PI * 2;
	}
	else if (g_king_rion[nCntKing_Rion].rotbetween.y < -D3DX_PI)
	{
		g_king_rion[nCntKing_Rion].rotbetween.y += D3DX_PI * 2;
	}

	g_king_rion[nCntKing_Rion].rot.y += g_king_rion[nCntKing_Rion].rotbetween.y * 0.1f;

	// 回転量が上限値を超えたら
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
// 移動処理
// ------------------------------------------------------------------------------------------
void Update_MoveKing_Rion(int nCntKing_Rion)
{
	// 移動状態
	g_king_rion[nCntKing_Rion].status.bMove = true;

	// 目的移動量から現在移動量の差分
	g_king_rion[nCntKing_Rion].pos.x += 
		g_king_rion[nCntKing_Rion].move.x * sinf(g_king_rion[nCntKing_Rion].rot.y - D3DX_PI);
	g_king_rion[nCntKing_Rion].pos.z += 
		g_king_rion[nCntKing_Rion].move.z * cosf(g_king_rion[nCntKing_Rion].rot.y - D3DX_PI);

	// 移動状態終了
		//g_king_rion[nCntKing_Rion].status.bMove = false;

		//g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;

	// 移動状態
	if (g_king_rion[nCntKing_Rion].status.bMove == true)
	{
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// 普通攻撃の処理
// ------------------------------------------------------------------------------------------
void Update_NormalAttackKing_Rion(int nCntKing_Rion)
{
	// 攻撃カウントアップ
	g_king_rion[nCntKing_Rion].status.nCntAttack++;

	// 自然モーション
	if (g_king_rion[nCntKing_Rion].status.nCntAttack <= KING_RION_ATTACK_COUNT)
	{
		// 自然体モーション
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
	}

	// 攻撃モーション
	else
	{
		// エフェクト発生


		PlaySound(SOUND_LABEL_SE_BOSSVOICE);

		// 普通攻撃モーション
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NORMALATACK;

		// 軌跡の発生
		OccurrenceTrajectory(
			g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Orbit_nIdTrajectory,
			nCntKing_Rion);
	}
}

// ------------------------------------------------------------------------------------------
// 弾処理
// ------------------------------------------------------------------------------------------
void Update_ShotKing_Rion(int nCntKing_Rion)
{
	// 変数宣言
	int	nAttackNum;				// 攻撃番号

	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーと百獣の王との距離
	diffpos = pPlayerPos - g_king_rion[nCntKing_Rion].pos;

	// 攻撃の確立
	nAttackNum = rand() % 1000;

	// 攻撃モーション切り替え
	if (nAttackNum == 0)
	{
		// エフェクト発生


		// 普通攻撃モーション
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_SHOT;
	}

	// 弾処理
	if (g_king_rion[nCntKing_Rion].nMotionType == KING_RIONMOSIONSET_SHOT)
	{
		// 発射する前
		if (g_king_rion[nCntKing_Rion].nCntKeySet >= 0 &&
			g_king_rion[nCntKing_Rion].nCntKeySet <= 2)
		{
			// 百獣の王から見てプレイヤーがいる角度
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// 目的の回転量
			g_king_rion[nCntKing_Rion].rotLast.y = fAngle - D3DX_PI;

			// 回転量が上限値を超えたら
			if (g_king_rion[nCntKing_Rion].rotLast.y > D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_king_rion[nCntKing_Rion].rotLast.y < -D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotLast.y += D3DX_PI * 2;
			}

			// 目的回転量から現在回転量の差分
			g_king_rion[nCntKing_Rion].rotbetween.y =
				(g_king_rion[nCntKing_Rion].rotLast.y - g_king_rion[nCntKing_Rion].rot.y);

			// 回転量が上限値を超えたら
			if (g_king_rion[nCntKing_Rion].rotbetween.y > D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_king_rion[nCntKing_Rion].rotbetween.y < -D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rotbetween.y += D3DX_PI * 2;
			}

			g_king_rion[nCntKing_Rion].rot.y += g_king_rion[nCntKing_Rion].rotbetween.y * 0.1f;

			// 回転量が上限値を超えたら
			if (g_king_rion[nCntKing_Rion].rot.y > D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rot.y += -D3DX_PI * 2;
			}
			else if (g_king_rion[nCntKing_Rion].rot.y < -D3DX_PI)
			{
				g_king_rion[nCntKing_Rion].rot.y += D3DX_PI * 2;
			}
		}

		// 発射タイミング
		else if (g_king_rion[nCntKing_Rion].nCntKeySet == 3)
		{
			// フレーム数が０
			if (g_king_rion[nCntKing_Rion].nFram == 0)
			{
				PlaySound(SOUND_LABEL_SE_BOSSVOICE);

				SetBullet(D3DXVECTOR3(
					g_king_rion[nCntKing_Rion].model[4].mtxWorldModel._41,
					g_king_rion[nCntKing_Rion].model[4].mtxWorldModel._42,
					g_king_rion[nCntKing_Rion].model[4].mtxWorldModel._43),	// 発射位置
					D3DXVECTOR3(1.0f, 0.0f, 1.0f),							// 移動
					g_king_rion[nCntKing_Rion].rot,							// 回転
					D3DXVECTOR3(10.0f, 10.0f, 10.0f),						// サイズ
					120,													// HP
					2,														// タイプ
					g_king_rion[nCntKing_Rion].status.nAttack);				// 攻撃力
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// モーション処理
// ------------------------------------------------------------------------------------------
void Update_MosionKing_Rion(int nCntKing_Rion)
{
	// 変数宣言
	int	nCntModel;			// カウントキリン

		// 現在のモーションと前回のモーションを比較
	if (g_king_rion[nCntKing_Rion].nMotionType != g_king_rion[nCntKing_Rion].nMotionTypeOld)
	{

		g_king_rion[nCntKing_Rion].nFram = 0;

		g_king_rion[nCntKing_Rion].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
	{
		// フレーム数が０の場合
		if (g_king_rion[nCntKing_Rion].nFram == 0)
		{
			if (g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram != 0)
			{
				// 最終地点と現在地点の差分分割
				g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween =
					(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].rot -
						g_king_rion[nCntKing_Rion].model[nCntModel].rot) /
						(float)g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram;

				// 最終地点と現在地点の差分
				g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween =
					(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].pos -
						g_king_rion[nCntKing_Rion].model[nCntModel].pos) /
						(float)g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram;
			}

			else
			{
				// 最終地点と現在地点の差分分割
				g_king_rion[nCntKing_Rion].model[nCntModel].rot =
					g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].rot;

				// 最終地点と現在地点の差分
				g_king_rion[nCntKing_Rion].model[nCntModel].pos =
					g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].key[nCntModel].pos;

				// 差分の初期化
				g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 差分の初期化
				g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// 回転する
		g_king_rion[nCntKing_Rion].model[nCntModel].rot +=
			g_king_rion[nCntKing_Rion].model[nCntModel].rotKeyBetween;

		// 移動する
		g_king_rion[nCntKing_Rion].model[nCntModel].pos +=
			g_king_rion[nCntKing_Rion].model[nCntModel].posKeyBetween;
	}

	// フレーム数が規定値と同じになったら
	if (g_king_rion[nCntKing_Rion].nFram ==
		g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].KeyInfo[g_king_rion[nCntKing_Rion].nCntKeySet].nFram)
	{
		// キーセットのカウントアップ
		g_king_rion[nCntKing_Rion].nCntKeySet++;

		// フレーム数初期化
		g_king_rion[nCntKing_Rion].nFram = 0;

		// キーセット数が規定値と同じになったら
		if (g_king_rion[nCntKing_Rion].nCntKeySet == g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].nNumKey)
		{
			// ループしないとき
			if (g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].nLoop == 0)
			{

				// キーセット数初期化
				g_king_rion[nCntKing_Rion].nCntKeySet = 0;

				// ノーマル攻撃モーション
				if (g_king_rion[nCntKing_Rion].nMotionType == KING_RIONMOSIONSET_NORMALATACK)
				{
					// 攻撃カウントの初期化
					g_king_rion[nCntKing_Rion].status.nCntAttack = 0;
				}

				g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
			}

			// ループする時
			else
			{
				// キーセット数初期化
				g_king_rion[nCntKing_Rion].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// フレーム数カウントアップ
		g_king_rion[nCntKing_Rion].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// 攻撃の当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_DecisionKing_Rion(int nCntKing_Rion)
{
	// プレイヤー情報取得
	PLAYER * pPlayer = GetPlayer();

	// 敵が生きているかどうか
	if (g_king_rion[nCntKing_Rion].status.nStatus != KING_RIONSTATUS_DIE)
	{
		// 攻撃状態かどうか
		if (g_king_rion[nCntKing_Rion].nMotionType == KING_RIONMOSIONSET_NORMALATACK)
		{
			// プレイヤーの攻撃を受けているかどうか
			if (pPlayer->status.bColition == false)
			{
				// プレイヤーの攻撃時の百獣の王とプレイヤーの当たり判定
				if ((pPlayer->status.fLength * pPlayer->status.fLength) +
					(g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_Radius * g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_Radius) >=
					(pPlayer->pos.x - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._41) *
					(pPlayer->pos.x - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._41) +
					(pPlayer->pos.y - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._42) *
					(pPlayer->pos.y - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._42) +
					(pPlayer->pos.z - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._43) *
					(pPlayer->pos.z - g_king_rion[nCntKing_Rion].model[g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_nParts].mtxWorldModel._43))
				{
					// 百獣の王のダメージ処理
					HitPlayer(
						g_king_rion[nCntKing_Rion].status.nAttack +
						g_king_rionKey[g_king_rion[nCntKing_Rion].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionKing_Rion(int nCntKing_Rion)
{
	// 床の当たり判定 //
	if (ColisionFLOOR(&g_king_rion[nCntKing_Rion].pos,
		&g_king_rion[nCntKing_Rion].posold,
		&g_king_rion[nCntKing_Rion].move,
		&g_king_rion[nCntKing_Rion].model[0].size,
		&g_king_rion[nCntKing_Rion].pMeshField))
	{

	}

	// 素材の当たり判定 //
	if (ColisionMaterials(&g_king_rion[nCntKing_Rion].pos,
		&g_king_rion[nCntKing_Rion].posold,
		&g_king_rion[nCntKing_Rion].move,
		&g_king_rion[nCntKing_Rion].model[0].size,
		&g_king_rion[nCntKing_Rion].pMaterials) != -1)
	{
		// 目標地点を現在地店へ
		g_king_rion[nCntKing_Rion].posLast = g_king_rion[nCntKing_Rion].pos;

		// モーションを自然体へ戻す
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
	}

	// 壁の当たり判定
	if (ColisionWall(&g_king_rion[nCntKing_Rion].pos,
		&g_king_rion[nCntKing_Rion].posold,
		&g_king_rion[nCntKing_Rion].move,
		&g_king_rion[nCntKing_Rion].model[0].size,
		&g_king_rion[nCntKing_Rion].pWall))
	{
		// 目標地点を現在地店へ
		g_king_rion[nCntKing_Rion].posLast = g_king_rion[nCntKing_Rion].pos;

		// モーションを自然体へ戻す
		g_king_rion[nCntKing_Rion].nMotionType = KING_RIONMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawKing_Rion(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ
	int nCntKing_Rion;
	int nCntModel;
	int nCnt = 0;

	// カウントキリン
	for (nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++)
	{
		if (g_king_rion[nCntKing_Rion].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_king_rion[nCntKing_Rion].rot.y, 
				g_king_rion[nCntKing_Rion].rot.x, 
				g_king_rion[nCntKing_Rion].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion,
				&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_king_rion[nCntKing_Rion].pos.x, 
				g_king_rion[nCntKing_Rion].pos.y, 
				g_king_rion[nCntKing_Rion].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion,	// 1
				&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion,					// 2
				&mtxTrans);									// 3

			// カウントキリン
			for (nCntModel = 0; nCntModel < KING_RIONTYPE_MAX; nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_king_rion[nCntKing_Rion].model[nCntModel].rot.y,
					g_king_rion[nCntKing_Rion].model[nCntModel].rot.x, 
					g_king_rion[nCntKing_Rion].model[nCntModel].rot.z);

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
				D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
					&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel, &mtxRot);

				// 位置を反映 //
				// 平行移動行列作成(オフセット)
				D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
					g_king_rion[nCntKing_Rion].model[nCntModel].pos.x,
					g_king_rion[nCntKing_Rion].model[nCntModel].pos.y, 
					g_king_rion[nCntKing_Rion].model[nCntModel].pos.z);	// それぞれの移動量

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
				D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,	// 1
					&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [すべての親]じゃない場合
				if (g_king_rion[nCntKing_Rion].model[nCntModel].nIdxModelParent != -1)
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].model[g_king_rion[nCntKing_Rion].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel,
						&g_king_rion[nCntKing_Rion].mtxWorldKing_Rion);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_king_rion[nCntKing_Rion].model[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_king_rionModel[nCntModel].paBuffMat->GetBufferPointer();

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_king_rionModel[nCntModel].nNumMat; nCntMat++, pMat++,nCnt++)
				{
					// 本体のカラー情報更新
					pMat->MatD3D.Diffuse.a = g_king_rion[nCntKing_Rion].diffuse;

					// テクスチャーあり
					if (pMat->pTextureFilename != NULL)
					{
						// テクスチャー設定
						pDevice->SetTexture(0, g_paTextureKing_Rion[0]);
					}

					// テクスチャーなし
					else
					{
						// テクスチャー設定
						pDevice->SetTexture(0, NULL);
					}

					// マテリアルの設定
					pDevice->SetMaterial(&pMat->MatD3D);

					// 描画
					g_king_rionModel[nCntModel].paMesh->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 攻撃を受けたときの処理
// ------------------------------------------------------------------------------------------
void HitKing_Rion(int nCntKing_Rion, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 攻撃が当たっている
	g_king_rion[nCntKing_Rion].status.bColition = true;

	// カラーの状態カウント初期化
	g_king_rion[nCntKing_Rion].nCntCor = 0;

	// HPが減少
	g_king_rion[nCntKing_Rion].status.nLife -= nDamage;

	// HPが0になったら
	if (g_king_rion[nCntKing_Rion].status.nLife <= 0)
	{
		// 死亡状態
		g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_DIE;

		// 目標回転量
		g_king_rion[nCntKing_Rion].rotLast.z = D3DX_PI / 2;

		// 移動量
		g_king_rion[nCntKing_Rion].move.y = 10.0f;

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
					g_king_rion[nCntKing_Rion].pos.x,
					g_king_rion[nCntKing_Rion].pos.y,
					g_king_rion[nCntKing_Rion].pos.z),

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
		g_king_rion[nCntKing_Rion].status.nStatus = KING_RIONSTATUS_DAMAGE;

		for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
		{
			// X・Zの角度
			fAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// Yの角度
			fYAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

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
					g_king_rion[nCntKing_Rion].pos.x,
					g_king_rion[nCntKing_Rion].pos.y,
					g_king_rion[nCntKing_Rion].pos.z),
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
// 百獣の王の当たり判定処理
// ------------------------------------------------------------------------------------------
void ColisionKing_Rion(
	D3DXVECTOR3 * pos,		// 位置
	D3DXVECTOR3 * posOld, 	// 前回の位置
	D3DXVECTOR3 * move,		// 移動量
	D3DXVECTOR3 * size		// サイズ
)
{
}

// ------------------------------------------------------------------------------------------
// 百獣の王キリンの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadKing_RionModel(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/KING_RION/king_rion_model.txt", "r");

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

				// キリンセットが来たら
				if (strcmp(cHeadText, "KING_RIONSET") == 0)
				{

					// エンドキリンセットが来るまでループ
					while (strcmp(cHeadText, "END_KING_RIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_king_rionModel[nCntModel].nIdxModelParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_king_rionModel[nCntModel].pos.x,
								&g_king_rionModel[nCntModel].pos.y, 
								&g_king_rionModel[nCntModel].pos.z);

							// 位置情報保存
							g_king_rionModel[nCntModel].posOrigin = g_king_rionModel[nCntModel].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_king_rionModel[nCntModel].rot.x, 
								&g_king_rionModel[nCntModel].rot.y,
								&g_king_rionModel[nCntModel].rot.z);
						}
					}
					// キリンの更新
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
		MessageBox(NULL, "百獣の王のパーツデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 百獣の王情報の読み込み処理
// ------------------------------------------------------------------------------------------
void LoadKing_Rion(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntKing_Rion = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ボス用
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/KING_RION/king_rion.txt", "r");

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

					// キリンセットが来たら
					else if (strcmp(cHeadText, "KING_RIONSET") == 0)
					{

						// エンドキリンセットが来るまでループ
						while (strcmp(cHeadText, "END_KING_RIONSET") != 0)
						{
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// 位置情報読み込み
							if (strcmp(cHeadText, "POS") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f",
									&cDie, &cDie,
									&g_king_rion[nCntKing_Rion].pos.x,
									&g_king_rion[nCntKing_Rion].pos.y,
									&g_king_rion[nCntKing_Rion].pos.z);

								// 目的位置に保存
								g_king_rion[nCntKing_Rion].posLast = g_king_rion[nCntKing_Rion].pos;
							}

							// 回転情報読み込み
							else if (strcmp(cHeadText, "ROT") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
									&g_king_rion[nCntKing_Rion].rot.x,
									&g_king_rion[nCntKing_Rion].rot.y,
									&g_king_rion[nCntKing_Rion].rot.z);

								// 使用状態
								g_king_rion[nCntKing_Rion].bUse = true;

								// 表示状態
								g_king_rion[nCntKing_Rion].bDisp = true;

								// 影の作成
								g_king_rion[nCntKing_Rion].nIdxShadow = SetShadow(
									g_king_rion[nCntKing_Rion].pos,
									D3DXVECTOR3(20.0f, 0.0f, 20.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
						// キリンの更新
						nCntKing_Rion++;
					}
				}
			}
			// ファイル閉
			fclose(pFile);
		}

		// 開けない
		else
		{
			MessageBox(NULL, "百獣の王のデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 百獣の王モーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadKing_RionMosion(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntMotion = 0;							// プレイヤーカウント
	int	nCntModel = 0;							// キリンカウント
	int	nCntKeySet = 0;							// モーションカウント
	int	nCntKey = 0;							// キーカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/KING_RION/king_rion_motion.txt", "r");

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

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// モーションセットが来たら
				else if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// キリンの初期化
					nCntModel = 0;

					// エンドキリンセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_king_rionKey[nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_king_rionKey[nCntMotion].nNumKey);
						}

						// 当たり判定情報読み込み
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

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_king_rionKey[nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
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

							// キャラクタータイプ
							g_king_rionKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_KING_RION;

							// 使用状態
							g_king_rionKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_king_rionKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 初期の位置情報反映
											g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_king_rion[0].model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_king_rionKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// カウントキー
									nCntKey++;
								}
							}

							// カウントキリン
							nCntModel++;
						}
					}

					// カウントプレイヤー-
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
		MessageBox(NULL, "百獣の王のモーションデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 百獣の王情報処理
// ------------------------------------------------------------------------------------------
KING_RION *GetKing_Rion(void)
{
	return &g_king_rion[0];
}

// ------------------------------------------------------------------------------------------
// 百獣の王のキリン情報処理
// ------------------------------------------------------------------------------------------
MODEL * GetKing_Rion_Model(int nCntKing_Rion)
{
	return &g_king_rion[nCntKing_Rion].model[0];
}

// ------------------------------------------------------------------------------------------
// 百獣の王撃破数処理
// ------------------------------------------------------------------------------------------
int GetKing_RionTotal(void)
{
	return g_king_rionTotal;
}
