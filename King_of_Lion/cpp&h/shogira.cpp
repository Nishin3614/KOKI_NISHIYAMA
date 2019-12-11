// ------------------------------------------------------------------------------------------
//
// 弾キリン処理 [shogira.cpp]
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
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_SHOGIRA (1)
#define SHOGIRA_HP (30)
#define SHOGIRA_ATTACK (10)					// 攻撃力
#define SHOGIRA_FREQUENCY (100)				// 攻撃頻度
#define SHOGIRA_NORMALATTACK_DECISION (100)
#define SHOGIRA_SHOT_DECISION (1000)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
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
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureShogira[TEXTURE_SHOGIRA] = {};	// テクスチャへのポインタ
SHOGIRA				g_shogira[MAX_SHOGIRA];						// キリン情報
MODEL				g_shogiraModel[SHOGIRATYPE_MAX];			// 弾キリン情報
MOSION_SHOGIRA		g_shogiraKey[SHOGIRAMOSIONSET_MAX];			// モーション情報
int					g_shogiraTotal;								// キリン撃破数合計

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitShogira(void)
{
	g_shogiraTotal = 0;
	// キリンパーツ情報
	Init_PartsShogira();

	// キリン情報
	Init_ShogiraInfo();

	// キリンモーション情報
	Init_MotionShogira();
}

// ------------------------------------------------------------------------------------------
// キリンパーツ情報処理
// ------------------------------------------------------------------------------------------
void Init_PartsShogira(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	int		nCntModel;							// 弾キリンのカウント

												// 弾キリンファイル名定義
	char *paShogiraFile[SHOGIRATYPE_MAX] =
	{
		{ "data/MODEL/giraffe/00_Giraffe_body.x" },				// [0]体
		{ "data/MODEL/giraffe/01_Giraffe_JointRoot_Neck.x" },	// [1]首の根元の関節
		{ "data/MODEL/giraffe/02_Giraffe_Neck.x" },				// [2]首
		{ "data/MODEL/giraffe/03_Giraffe_JointTip_Neck.x" },	// [3]首の先端の関節
		{ "data/MODEL/giraffe/04_Giraffe_Head.x" },				// [4]頭
		{ "data/MODEL/giraffe/05_Giraffe_RightHand.x" },		// [5]右手
		{ "data/MODEL/giraffe/06_Giraffe_LeftHand.x" },			// [6]左手
		{ "data/MODEL/giraffe/07_Giraffe_RightLeg.x" },			// [7]右足
		{ "data/MODEL/giraffe/08_Giraffe_LeftLeg.x" },			// [8]左足
		{ "data/MODEL/giraffe/09_Giraffe_Joint_Tail.x" },		// [9]しっぽの関節
		{ "data/MODEL/giraffe/10_Giraffe_Tail.x" }				// [10]しっぽ
	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_SHOGIRA] =
	{
		{ "data/TEXTURE/giraffe/00_giraffe_Texture.png" },
	};

	// パーツの読み込み //
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

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			paShogiraFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_shogiraModel[nCntModel].paBuffMat,
			NULL,
			&g_shogiraModel[nCntModel].nNumMat,
			&g_shogiraModel[nCntModel].paMesh);

		// 弾キリンの頂点座標の最大値・最小値の設定
		int nNumVertex;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

								// 頂点数を取得
		nNumVertex = g_shogiraModel[nCntModel].paMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_shogiraModel[nCntModel].paMesh->GetFVF());

		// 頂点バッファをロック
		g_shogiraModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// 頂点数分繰り返す
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// 番地情報のD3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
			// x //
			// 最小値
			if (g_shogiraModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_shogiraModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// 最大値
			if (g_shogiraModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_shogiraModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// 最小値
			if (g_shogiraModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_shogiraModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// 最大値
			if (g_shogiraModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_shogiraModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// 最小値
			if (g_shogiraModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_shogiraModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// 最大値
			if (g_shogiraModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_shogiraModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// サイズポインタを進める
			pVertexBuffer += sizeFVF;
		}

		// アンロック
		g_shogiraModel[nCntModel].paMesh->UnlockVertexBuffer();

		// 最大値と最小値の距離
		g_shogiraModel[nCntModel].size = g_shogiraModel[nCntModel].vtxMaxMaterials - g_shogiraModel[nCntModel].vtxMinMaterials;
	}

	// パーツ読込
	LoadShogiraModel();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureShogira[0]);
}

// ------------------------------------------------------------------------------------------
// キリン情報処理
// ------------------------------------------------------------------------------------------
void Init_ShogiraInfo(void)
{
	// 変数宣言
	int		nCntShogira;	// プレイヤーカウント
	int		nCntModel;		// 弾キリンのカウント

	// キリンの初期設定 //
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
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

		// カラー情報
		g_shogira[nCntShogira].nCntCor = 0;
		g_shogira[nCntShogira].diffuse = 1.0f;

		// ステータス情報
		g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;
		g_shogira[nCntShogira].status.nCntStatus = 0;
		g_shogira[nCntShogira].status.nAttack = SHOGIRA_ATTACK;
		g_shogira[nCntShogira].status.nLife = SHOGIRA_HP;
		g_shogira[nCntShogira].status.bColition = false;
		g_shogira[nCntShogira].status.bAttack = false;
		g_shogira[nCntShogira].status.bShot = false;
		g_shogira[nCntShogira].status.bRot = false;
		g_shogira[nCntShogira].status.bMove = false;

		// パーツごとのカウント
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

	// ロードキリン
	LoadShogira();
}

// ------------------------------------------------------------------------------------------
// キリンモーション処理
// ------------------------------------------------------------------------------------------
void Init_MotionShogira(void)
{
	// 変数宣言
	int		nCntShogira;						// キリンカウント

												// モーション用変数
	int		nCntMotion;							// モーションカウント
	int		nCntKeySet;							// モーションカウント
	int		nCntKey;							// キーカウント

												// モーションカウント //
	for (nCntMotion = 0; nCntMotion < SHOGIRAMOSIONSET_MAX; nCntMotion++)
	{
		// モーションの初期設定
		g_shogiraKey[nCntMotion].nNumKey = 0;		// ナンバーキー
		g_shogiraKey[nCntMotion].nLoop = 0;		// ループの有無

												// 当たり判定用 //
		g_shogiraKey[nCntMotion].Collision_nParts = 0;		// パーツ番号
		g_shogiraKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
		g_shogiraKey[nCntMotion].Collision_Radius = 0.0f;	// 円の半径
		g_shogiraKey[nCntMotion].Collision_StartFram = 0;	// 開始フレーム数
		g_shogiraKey[nCntMotion].Collision_EndFram = 0;		// 終了フレーム数
		g_shogiraKey[nCntMotion].Collision_Damage = 0;		// ダメージ数

															// 軌跡用 //
		g_shogiraKey[nCntMotion].Orbit_nParts = 0;			// パーツ番号
		g_shogiraKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
		g_shogiraKey[nCntMotion].Orbit_Mesh = 0;			// 軌跡の線
		g_shogiraKey[nCntMotion].Orbit_StartFram = 0;		// 開始フレーム数
		g_shogiraKey[nCntMotion].Orbit_EndFram = 0;			// 終了フレーム数
		g_shogiraKey[nCntMotion].Orbit_nType = 1;			// 軌跡のタイプ
		g_shogiraKey[nCntMotion].Orbit_nIdTrajectory = -1;	// 軌跡の番号
		g_shogiraKey[nCntMotion].Orbit_bUse = false;		// 軌跡の使用状態

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_shogiraKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// キーごとに初期化
			for (nCntKey = 0; nCntKey < SHOGIRATYPE_MAX; nCntKey++)
			{
				g_shogiraKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_shogiraKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// プレイヤーモーション読み込み
	LoadShogiraMosion();

	// キリンの初期設定 //
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < SHOGIRAMOSIONSET_MAX; nCntMotion++)
			{
				// 使用状態
				if (g_shogiraKey[nCntMotion].Orbit_bUse == true)
				{
					// 軌跡の設定
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
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitShogira(void)
{
	// 弾キリンデータの開放
	for (int nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_shogiraModel[nCntModel].paMesh != NULL)
		{
			g_shogiraModel[nCntModel].paMesh->Release();
			g_shogiraModel[nCntModel].paMesh = NULL;
		}

		// マテリアルの開放
		if (g_shogiraModel[nCntModel].paBuffMat != NULL)
		{
			g_shogiraModel[nCntModel].paBuffMat->Release();
			g_shogiraModel[nCntModel].paBuffMat = NULL;
		}
	}

	// 弾キリンテクスチャの開放
	for (int nCntTexture = 0; nCntTexture < TEXTURE_SHOGIRA; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTextureShogira[nCntTexture] != NULL)
		{
			g_paTextureShogira[nCntTexture]->Release();
			g_paTextureShogira[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateShogira(void)
{
	// カウントキリン
	int nCntShogira;
	PLAYER *pPlayer = GetPlayer();

	// キリンのカウント
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse)
		{
			// 現在地保存
			g_shogira[nCntShogira].posold = g_shogira[nCntShogira].pos;

			// モーションを保存
			g_shogira[nCntShogira].nMotionTypeOld = g_shogira[nCntShogira].nMotionType;

			// 状態処理
			Update_StateShogira(nCntShogira);

			// 死んでいない状態
			if (g_shogira[nCntShogira].status.nStatus != SHOGIRASTATUS_DIE)
			{
				if (GetMode() != MODE_TUTORIAL)
				{
					// 攻撃状態でないとき
					if (g_shogira[nCntShogira].status.bAttack == false)
					{
						// 発射状態でないとき
						if (g_shogira[nCntShogira].status.bShot == false)
						{
							// 移動状態ではないとき
							if (g_shogira[nCntShogira].status.bMove == false)
							{
								// 回転処理
								Update_RotShogira(nCntShogira);
							}
						}
					}

					// 移動・回転状態ではないとき
					if (g_shogira[nCntShogira].status.bMove == false)
					{
						// 移動・回転状態ではないとき
						if (g_shogira[nCntShogira].status.bRot == false)
						{
							if (g_shogira[nCntShogira].status.bAttack == false)
							{
								// 弾の処理
								Update_ShotShogira(nCntShogira);
							}
						}
					}
				}
				// 攻撃の当たり判定
				Update_DecisionShogira(nCntShogira);

			}

			g_shogira[nCntShogira].move.y -= 0.5f;
			g_shogira[nCntShogira].pos.y += g_shogira[nCntShogira].move.y;

			// 当たり判定
			Update_ColisionShogira(nCntShogira);

			// 死んでいない状態
			if (g_shogira[nCntShogira].status.nStatus != SHOGIRASTATUS_DIE)
			{
				// モーション処理
				Update_MosionShogira(nCntShogira);
			}

			// 影の位置を設定
			SetPositionShadow(
				g_shogira[nCntShogira].nIdxShadow,
				g_shogira[nCntShogira].pos +
				D3DXVECTOR3(
					sinf(-g_shogira[nCntShogira].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_shogira[nCntShogira].rot.y) * 10.0f));

			// 攻撃が当たっている
			if (g_shogira[nCntShogira].status.bColition == true)
			{
				// 攻撃が終わったら
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_shogira[nCntShogira].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 状態処理
// ------------------------------------------------------------------------------------------
void Update_StateShogira(int nCntShogira)
{
	switch (g_shogira[nCntShogira].status.nStatus)
	{
	case SHOGIRASTATUS_NEUTRAL:
		g_shogira[nCntShogira].diffuse = 1.0f;
		break;

	case SHOGIRASTATUS_DAMAGE:
		// カラーの状態カウントアップ
		g_shogira[nCntShogira].nCntCor++;

		// 表示状態
		if (g_shogira[nCntShogira].nCntCor % 10 == 0)
		{
			g_shogira[nCntShogira].diffuse = 1.0f;
		}

		// 非表示状態
		else if (g_shogira[nCntShogira].nCntCor % 5 == 0)
		{
			g_shogira[nCntShogira].diffuse = 0.5f;
		}

		if (g_shogira[nCntShogira].nCntCor == 50)
		{
			// カラーの状態カウント初期化
			g_shogira[nCntShogira].nCntCor = 0;

			// キリンの状態をノーマルへ
			g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;
		}

		break;

	case SHOGIRASTATUS_DIE:
		// 目標回転量から現在の回転量の差分
		g_shogira[nCntShogira].rotbetween.z =
			(g_shogira[nCntShogira].rotLast.z - g_shogira[nCntShogira].rot.z) / 10;

		// 回転
		g_shogira[nCntShogira].rot.z += g_shogira[nCntShogira].rotbetween.z;

		// 重力
		g_shogira[nCntShogira].move.y -= 0.5f;	// 移動量
		g_shogira[nCntShogira].pos.y += g_shogira[nCntShogira].move.y;

		// ステータスのカウントアップ
		g_shogira[nCntShogira].status.nCntStatus++;

		DeleteBillboard(BILLBOARDTYPE_ENTER);
		// ステータスのカウントが規定値以上になったら
		if (g_shogira[nCntShogira].status.nCntStatus == 70)
		{
			if (GetMode() == MODE_TUTORIAL)
			{
				// バッファロー出現
				SetBuffalo(
					g_shogira[nCntShogira].pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}

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
						g_shogira[nCntShogira].pos.x,
						g_shogira[nCntShogira].pos.y,
						g_shogira[nCntShogira].pos.z),

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
			g_shogira[nCntShogira].bUse = false;
			g_shogira[nCntShogira].bDisp = false;

			// 影の消去
			DeleteShadow(g_shogira[nCntShogira].nIdxShadow);
			// 予測線の消去
			DeletePrediction(g_shogira[nCntShogira].nIdxPrediction);
			// 使用中の軌跡の消去
			DeleteTrajectory(g_shogiraKey[g_shogira[nCntShogira].nMotionType].Orbit_nIdTrajectory);

			// ステータスのカウントの初期化
			g_shogira[nCntShogira].status.nCntStatus = 0;

			// ステータス状況
			g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_NEUTRAL;

			// アイテム出現
			SetItem(
				g_shogira[nCntShogira].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// 移動量
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// サイズ
				20.0f,								// 半径
				1,									// テクスチャー
				100);								// 出現時間

			// 撃破数
			g_shogiraTotal++;

			// ゲーム時だけ
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
// 回転処理
// ------------------------------------------------------------------------------------------
void Update_RotShogira(int nCntShogira)
{
	// 方向転換番号
	int nRandNum = 0;

	// 移動状態ではないとき
	if (g_shogira[nCntShogira].status.bRot == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 500;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// 目的の回転量
			g_shogira[nCntShogira].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// 回転量が上限値を超えたら
			if (g_shogira[nCntShogira].rotLast.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rotLast.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += D3DX_PI * 2;
			}

			// 回転状態
			g_shogira[nCntShogira].status.bRot = true;
		}
	}

	// 目的回転量から現在回転量の差分
	g_shogira[nCntShogira].rotbetween.y = (g_shogira[nCntShogira].rotLast.y - g_shogira[nCntShogira].rot.y) * 0.01f;
	g_shogira[nCntShogira].rot.y += g_shogira[nCntShogira].rotbetween.y;

	// 回転状態終了
	if (g_shogira[nCntShogira].rot.y >= -0.1f ||
		g_shogira[nCntShogira].rot.y <= 0.1f)
	{
		g_shogira[nCntShogira].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// モーション処理
// ------------------------------------------------------------------------------------------
void Update_MosionShogira(int nCntShogira)
{
	// 変数宣言
	int	nCntModel;			// カウント弾キリン

		// 現在のモーションと前回のモーションを比較
	if (g_shogira[nCntShogira].nMotionType != g_shogira[nCntShogira].nMotionTypeOld)
	{

		g_shogira[nCntShogira].nFram = 0;

		g_shogira[nCntShogira].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
	{
		// フレーム数が０の場合
		if (g_shogira[nCntShogira].nFram == 0)
		{
			if (g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram != 0)
			{
				// 最終地点と現在地点の差分分割
				g_shogira[nCntShogira].model[nCntModel].rotKeyBetween =
					(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].rot -
						g_shogira[nCntShogira].model[nCntModel].rot) /
						(float)g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram;

				// 最終地点と現在地点の差分
				g_shogira[nCntShogira].model[nCntModel].posKeyBetween =
					(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].pos -
						g_shogira[nCntShogira].model[nCntModel].pos) /
						(float)g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram;
			}

			else
			{
				// 最終地点と現在地点の差分分割
				g_shogira[nCntShogira].model[nCntModel].rot =
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].rot;

				// 最終地点と現在地点の差分
				g_shogira[nCntShogira].model[nCntModel].pos =
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].key[nCntModel].pos;

				// 差分の初期化
				g_shogira[nCntShogira].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 差分の初期化
				g_shogira[nCntShogira].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// 回転する
		g_shogira[nCntShogira].model[nCntModel].rot +=
			g_shogira[nCntShogira].model[nCntModel].rotKeyBetween;

		// 移動する
		g_shogira[nCntShogira].model[nCntModel].pos +=
			g_shogira[nCntShogira].model[nCntModel].posKeyBetween;
	}

	// フレーム数が規定値と同じになったら
	if (g_shogira[nCntShogira].nFram ==
		g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[g_shogira[nCntShogira].nCntKeySet].nFram)
	{
		// キーセットのカウントアップ
		g_shogira[nCntShogira].nCntKeySet++;

		// フレーム数初期化
		g_shogira[nCntShogira].nFram = 0;

		// キーセット数が規定値と同じになったら
		if (g_shogira[nCntShogira].nCntKeySet == g_shogiraKey[g_shogira[nCntShogira].nMotionType].nNumKey)
		{
			// ループしないとき
			if (g_shogiraKey[g_shogira[nCntShogira].nMotionType].nLoop == 0)
			{

				// キーセット数初期化
				g_shogira[nCntShogira].nCntKeySet = 0;

				g_shogira[nCntShogira].nMotionType = 0;
			}

			// ループする時
			else
			{
				// キーセット数初期化
				g_shogira[nCntShogira].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// フレーム数カウントアップ
		g_shogira[nCntShogira].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// 弾処理
// ------------------------------------------------------------------------------------------
void Update_ShotShogira(int nCntShogira)
{
	// 変数宣言
	int	nAttackNum;				// 攻撃番号
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーとキリンとの距離
	diffpos = pPlayerPos - g_shogira[nCntShogira].pos;

	// 攻撃行動範囲
	if ((SHOGIRA_SHOT_DECISION * SHOGIRA_SHOT_DECISION) >=	// 攻撃行動の円範囲
		diffpos.x * diffpos.x +								// x軸の円
		diffpos.y * diffpos.y +								// y軸の円
		diffpos.z * diffpos.z)								// z軸の円
	{
		// 攻撃の確立
		nAttackNum = rand() % SHOGIRA_FREQUENCY;

		// 攻撃
		if (nAttackNum == 0 &&
			g_shogira[nCntShogira].status.bShot == false)
		{
			g_shogira[nCntShogira].EffectCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// プレイヤーの位置を取得
			g_shogira[nCntShogira].posAttack = GetPlayer()->pos;

			// 予測線発生
			g_shogira[nCntShogira].nIdxPrediction = SetPrediction(
				g_shogira[nCntShogira].posAttack,
				D3DXVECTOR3(20.0f,20.0f,20.0f) ,
				D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

			// 普通攻撃モーション
			g_shogira[nCntShogira].nMotionType = SHOGIRAMOSIONSET_SHOT;

			// 攻撃状態ON
			g_shogira[nCntShogira].status.bShot = true;
		}
	}

	// 弾処理
	if (g_shogira[nCntShogira].nMotionType == SHOGIRAMOSIONSET_SHOT)
	{
		// 発射する前
		if (g_shogira[nCntShogira].nCntKeySet >= 0 &&
			g_shogira[nCntShogira].nCntKeySet <= 2)
		{
			// 緑のいろの減少
			g_shogira[nCntShogira].EffectCol.g -= 1.0f /
				(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[0].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[1].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[2].nFram);
			// 青のいろの減少
			g_shogira[nCntShogira].EffectCol.b -= 1.0f /
				(g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[0].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[1].nFram +
					g_shogiraKey[g_shogira[nCntShogira].nMotionType].KeyInfo[2].nFram);

			// キリンから見てプレイヤーがいる角度
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// 目的の回転量
			g_shogira[nCntShogira].rotLast.y = fAngle - D3DX_PI;

			// 回転量が上限値を超えたら
			if (g_shogira[nCntShogira].rotLast.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rotLast.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rotLast.y += D3DX_PI * 2;
			}

			// 目的回転量から現在回転量の差分
			g_shogira[nCntShogira].rotbetween.y =
				(g_shogira[nCntShogira].rotLast.y - g_shogira[nCntShogira].rot.y);

			// 回転量が上限値を超えたら
			if (g_shogira[nCntShogira].rotbetween.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rotbetween.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rotbetween.y += D3DX_PI * 2;
			}

			g_shogira[nCntShogira].rot.y += g_shogira[nCntShogira].rotbetween.y * 0.1f;

			// 回転量が上限値を超えたら
			if (g_shogira[nCntShogira].rot.y > D3DX_PI)
			{
				g_shogira[nCntShogira].rot.y += -D3DX_PI * 2;
			}
			else if (g_shogira[nCntShogira].rot.y < -D3DX_PI)
			{
				g_shogira[nCntShogira].rot.y += D3DX_PI * 2;
			}
			// プレイヤーの位置を取得
			g_shogira[nCntShogira].posAttack = GetPlayer()->pos;
			// 予測線の位置情報設定
			SetPositionPrediction(g_shogira[nCntShogira].nIdxPrediction, g_shogira[nCntShogira].posAttack);

			for (int nCntEffect = 0; nCntEffect < 5; nCntEffect++)
			{
				float fXZAngle, fYAngle;
				D3DXVECTOR3 move;
				D3DXCOLOR col;
				// X・Zの角度
				fXZAngle =
					float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

				// Yの角度
				fYAngle =
					float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

				// 移動方向
				move =
					D3DXVECTOR3(0.0f,
						0.5f,
						0.0f);

				// カラー
				col =
					D3DXCOLOR(
						1.0f,
						g_shogira[nCntShogira].EffectCol.g,
						g_shogira[nCntShogira].EffectCol.b,
						1.0f);

				// エフェクトの設定
				SetEffect(
					D3DXVECTOR3(													// 位置
						g_shogira[nCntShogira].pos.x + sinf(fXZAngle) * 20.0f,
						0.0f,
						g_shogira[nCntShogira].pos.z + cosf(fXZAngle) * 20.0f),
					move,															// 移動量
					col,															// 色
					5,																// サイズ
					-0.1f,																// 減少サイズ
					0.0f,															// 重力
					50,																// エフェクトライフ
					0,																// タイプ
					0);
			}
		}

		// 発射タイミング
		else if (g_shogira[nCntShogira].nCntKeySet == 3)
		{
			// 発射状態ではない場合
			if (g_shogira[nCntShogira].status.bShot == true)
			{
				// カウントダメージ
				g_shogira[nCntShogira].nCntDamage++;

				// 技の発動時間
				if (g_shogira[nCntShogira].nCntDamage >= 20)
				{
					PlaySound(SOUND_LABEL_SE_BEAM);

					SetExplosion(
						g_shogira[nCntShogira].posAttack,
						EXPLOSIONTYPE_THUNDER,
						50.0f,
						g_shogira[nCntShogira].status.nAttack);

					// 発射状態
					g_shogira[nCntShogira].status.bShot = false;
					g_shogira[nCntShogira].nCntDamage = 0;
					// 予測線の削除
					DeletePrediction(g_shogira[nCntShogira].nIdxPrediction);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 攻撃の当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_DecisionShogira(int nCntShogira)
{
	// プレイヤー情報取得
	PLAYER * pPlayer = GetPlayer();

	// 敵が生きているかどうか
	if (g_shogira[nCntShogira].status.nStatus != SHOGIRASTATUS_DIE)
	{
		// 攻撃状態かどうか
		if (g_shogira[nCntShogira].nMotionType == SHOGIRAMOSIONSET_NORMALATACK)
		{
			// プレイヤーの攻撃を受けているかどうか
			if (pPlayer->status.bColition == false)
			{
				// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
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
					// キリンのダメージ処理
					HitPlayer(
						g_shogira[nCntShogira].status.nAttack +
						g_shogiraKey[g_shogira[nCntShogira].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionShogira(int nCntShogira)
{
	// 床の当たり判定 //
	if (ColisionFLOOR(&g_shogira[nCntShogira].pos,
		&g_shogira[nCntShogira].posold,
		&g_shogira[nCntShogira].move,
		&g_shogira[nCntShogira].model[0].size,
		&g_shogira[nCntShogira].pMeshField))
	{

	}

	// 素材の当たり判定 //
	if (ColisionMaterials(&g_shogira[nCntShogira].pos,
		&g_shogira[nCntShogira].posold,
		&g_shogira[nCntShogira].move,
		&g_shogira[nCntShogira].model[0].size,
		&g_shogira[nCntShogira].pMaterials) != -1)
	{
		// 目標地点を現在地店へ
		g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;

		// モーションを自然体へ戻す
		g_shogira[nCntShogira].nMotionType = SHOGIRAMOSIONSET_NEUTRAL;
	}

	// 壁の当たり判定
	if (ColisionWall(&g_shogira[nCntShogira].pos,
		&g_shogira[nCntShogira].posold,
		&g_shogira[nCntShogira].move,
		&g_shogira[nCntShogira].model[0].size,
		&g_shogira[nCntShogira].pWall))
	{
		// 目標地点を現在地店へ
		g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;

		// モーションを自然体へ戻す
		g_shogira[nCntShogira].nMotionType = SHOGIRAMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawShogira(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ
	int nCntShogira;
	int nCntModel;
	int nCnt = 0;

	// カウント弾キリン
	for (nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_shogira[nCntShogira].mtxWorldShogira);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_shogira[nCntShogira].rot.y, 
				g_shogira[nCntShogira].rot.x, 
				g_shogira[nCntShogira].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_shogira[nCntShogira].mtxWorldShogira,
				&g_shogira[nCntShogira].mtxWorldShogira, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_shogira[nCntShogira].pos.x, 
				g_shogira[nCntShogira].pos.y, 
				g_shogira[nCntShogira].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_shogira[nCntShogira].mtxWorldShogira,	// 1
				&g_shogira[nCntShogira].mtxWorldShogira,					// 2
				&mtxTrans);									// 3

			// カウント弾キリン
			for (nCntModel = 0; nCntModel < SHOGIRATYPE_MAX; nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_shogira[nCntShogira].model[nCntModel].rot.y,
					g_shogira[nCntShogira].model[nCntModel].rot.x, 
					g_shogira[nCntShogira].model[nCntModel].rot.z);

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
				D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
					&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel, &mtxRot);

				// 位置を反映 //
				// 平行移動行列作成(オフセット)
				D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
					g_shogira[nCntShogira].model[nCntModel].pos.x,
					g_shogira[nCntShogira].model[nCntModel].pos.y, 
					g_shogira[nCntShogira].model[nCntModel].pos.z);	// それぞれの移動量

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
				D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,	// 1
					&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [すべての親]じゃない場合
				if (g_shogira[nCntShogira].model[nCntModel].nIdxModelParent != -1)
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].model[g_shogira[nCntShogira].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].model[nCntModel].mtxWorldModel,
						&g_shogira[nCntShogira].mtxWorldShogira);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_shogira[nCntShogira].model[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_shogiraModel[nCntModel].paBuffMat->GetBufferPointer();

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_shogiraModel[nCntModel].nNumMat; nCntMat++, pMat++,nCnt++)
				{
					// 本体のカラー情報更新
					pMat->MatD3D.Diffuse.a = g_shogira[nCntShogira].diffuse;

					// テクスチャーあり
					if (pMat->pTextureFilename != NULL)
					{
						// テクスチャー設定
						pDevice->SetTexture(0, g_paTextureShogira[0]);
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
					g_shogiraModel[nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitShogira(int nCntShogira, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 攻撃が当たっている
	g_shogira[nCntShogira].status.bColition = true;

	// カラーの状態カウント初期化
	g_shogira[nCntShogira].nCntCor = 0;

	// HPが減少
	g_shogira[nCntShogira].status.nLife -= nDamage;

	// HPが0になったら
	if (g_shogira[nCntShogira].status.nLife <= 0)
	{
		// 死亡状態
		g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_DIE;

		// 目標回転量
		g_shogira[nCntShogira].rotLast.z = D3DX_PI / 2;

		// 移動量
		g_shogira[nCntShogira].move.y = 5.0f;

		PlaySound(SOUND_LABEL_SE_COLISION);
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
					g_shogira[nCntShogira].pos.x,
					g_shogira[nCntShogira].pos.y,
					g_shogira[nCntShogira].pos.z),

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
		// 反動
		g_shogira[nCntShogira].move.y = 5.0f;

		// ダメージ状態
		g_shogira[nCntShogira].status.nStatus = SHOGIRASTATUS_DAMAGE;

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
					g_shogira[nCntShogira].pos.x,
					g_shogira[nCntShogira].pos.y,
					g_shogira[nCntShogira].pos.z),
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
// キリンの当たり判定処理
// ------------------------------------------------------------------------------------------
void ColisionShogira(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// キリン弾キリンの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadShogiraModel(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/GIRAFFE/giraffe_model.txt", "r");

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

				// 弾キリンセットが来たら
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// エンド弾キリンセットが来るまでループ
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_shogiraModel[nCntModel].nIdxModelParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_shogiraModel[nCntModel].pos.x,
								&g_shogiraModel[nCntModel].pos.y, 
								&g_shogiraModel[nCntModel].pos.z);

							// 位置情報保存
							g_shogiraModel[nCntModel].posOrigin = g_shogiraModel[nCntModel].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_shogiraModel[nCntModel].rot.x, 
								&g_shogiraModel[nCntModel].rot.y,
								&g_shogiraModel[nCntModel].rot.z);
						}
					}
					// 弾キリンの更新
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
		MessageBox(NULL, "キリンのパーツデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// キリン情報の読み込み処理
// ------------------------------------------------------------------------------------------
void LoadShogira(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntShogira = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ボス用
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		return;
	}

	// それ以外
	else
	{
		// ファイル開
		pFile = fopen("data/SAVE/SHOGIRA/shogira.txt", "r");
	}

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

				// 弾キリンセットが来たら
				else if (strcmp(cHeadText, "SHOGIRASET") == 0)
				{

					// エンド弾キリンセットが来るまでループ
					while (strcmp(cHeadText, "END_SHOGIRASET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_shogira[nCntShogira].pos.x,
								&g_shogira[nCntShogira].pos.y,
								&g_shogira[nCntShogira].pos.z);

							// 目的位置に保存
							g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_shogira[nCntShogira].rot.x,
								&g_shogira[nCntShogira].rot.y,
								&g_shogira[nCntShogira].rot.z);

							// 影の作成
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

					// 使用状態
					g_shogira[nCntShogira].bUse = true;

					// 表示状態
					g_shogira[nCntShogira].bDisp = true;

					// 弾キリンの更新
					nCntShogira++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "弾キリンのデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// キリンモーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadShogiraMosion(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntMotion = 0;							// プレイヤーカウント
	int	nCntModel = 0;							// 弾キリンカウント
	int	nCntKeySet = 0;							// モーションカウント
	int	nCntKey = 0;							// キーカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/GIRAFFE/giraffe_motion.txt", "r");

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
					// 弾キリンの初期化
					nCntModel = 0;

					// エンド弾キリンセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_shogiraKey[nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_shogiraKey[nCntMotion].nNumKey);
						}

						// 当たり判定情報読み込み
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

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_shogiraKey[nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
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

							// キャラクタータイプ
							g_shogiraKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_GIRAFFE;

							// 使用状態
							g_shogiraKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_shogiraKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_shogira[0].model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_shogiraKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// カウントキー
									nCntKey++;
								}
							}

							// カウント弾キリン
							nCntModel++;
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
		MessageBox(NULL, "弾キリンのモーションデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// キリン情報処理
// ------------------------------------------------------------------------------------------
SHOGIRA *GetShogira(void)
{
	return &g_shogira[0];
}


// ------------------------------------------------------------------------------------------
// キリン撃破数処理
// ------------------------------------------------------------------------------------------
int GetShogiraTotal(void)
{
	return g_shogiraTotal;
}

// ------------------------------------------------------------------------------------------
// キリン設置処理
// ------------------------------------------------------------------------------------------
void SetShogira(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// カウント弾キリン
	for (int nCntShogira = 0; nCntShogira < MAX_BUFFALO; nCntShogira++)
	{
		if (g_shogira[nCntShogira].bUse == false)
		{
			// 位置情報
			g_shogira[nCntShogira].pos = pos;


			// 目的位置に保存
			g_shogira[nCntShogira].posLast = g_shogira[nCntShogira].pos;

			// 回転情報
			g_shogira[nCntShogira].rot = rot;

			// 使用状態
			g_shogira[nCntShogira].bUse = true;

			// 表示状態
			g_shogira[nCntShogira].bDisp = true;

			// 影の作成
			g_shogira[nCntShogira].nIdxShadow = SetShadow(
				g_shogira[nCntShogira].pos,
				D3DXVECTOR3(20.0f,0.0f,20.0f),
				D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

			// ステータス情報
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
