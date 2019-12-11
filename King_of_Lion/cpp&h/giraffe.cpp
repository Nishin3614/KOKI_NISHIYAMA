// ------------------------------------------------------------------------------------------
//
// キリン処理 [giraffe.cpp]
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
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_GIRAFFE (1)
#define GIRAFFE_HP (30)
#define GIRAFFE_ATTACK (10)					// 攻撃力
#define GIRAFFE_FREQUENCY (100)				// 攻撃頻度
#define GIRAFFE_NORMALATTACK_DECISION (100)
#define GIRAFFE_SHOT_DECISION (500)
#define GIRAFFE_G (0.5f)			// 重力
#define GIRAFFE_RESISTANCE (0.5f)// 抵抗力

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
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
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureGiraffe[TEXTURE_GIRAFFE] = {};	// テクスチャへのポインタ

GIRAFFE				g_giraffe[MAX_GIRAFFE];						// キリン情報

MODEL				g_giraffeModel[GIRAFFETYPE_MAX];			// キリン情報

MOSION_GIRAFFE		g_giraffeKey[GIRAFFEMOSIONSET_MAX];			// モーション情報

int					g_giraffeTotal;								// キリン撃破数合計

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitGiraffe(void)
{
	g_giraffeTotal = 0;
	// キリンパーツ情報
	Init_PartsGiraffe();

	// キリン情報
	Init_GiraffeInfo();

	// キリンモーション情報
	Init_MotionGiraffe();
}

// ------------------------------------------------------------------------------------------
// キリンパーツ情報処理
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	int		nCntModel;							// キリンのカウント

												// キリンファイル名定義
	char *paGiraffeFile[GIRAFFETYPE_MAX] =
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
	char *paTextureFile[TEXTURE_GIRAFFE] =
	{
		{ "data/TEXTURE/giraffe/00_giraffe_Texture.png" },
	};

	// パーツの読み込み //
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

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			paGiraffeFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_giraffeModel[nCntModel].paBuffMat,
			NULL,
			&g_giraffeModel[nCntModel].nNumMat,
			&g_giraffeModel[nCntModel].paMesh);

		// キリンの頂点座標の最大値・最小値の設定
		int nNumVertex;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

								// 頂点数を取得
		nNumVertex = g_giraffeModel[nCntModel].paMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_giraffeModel[nCntModel].paMesh->GetFVF());

		// 頂点バッファをロック
		g_giraffeModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// 頂点数分繰り返す
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// 番地情報のD3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
			// x //
			// 最小値
			if (g_giraffeModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// 最大値
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// 最小値
			if (g_giraffeModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// 最大値
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// 最小値
			if (g_giraffeModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// 最大値
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// サイズポインタを進める
			pVertexBuffer += sizeFVF;
		}

		// アンロック
		g_giraffeModel[nCntModel].paMesh->UnlockVertexBuffer();

		// 最大値と最小値の距離
		g_giraffeModel[nCntModel].size = g_giraffeModel[nCntModel].vtxMaxMaterials - g_giraffeModel[nCntModel].vtxMinMaterials;
	}

	// パーツ読込
	LoadGiraffeModel();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureGiraffe[0]);
}

// ------------------------------------------------------------------------------------------
// キリン情報処理
// ------------------------------------------------------------------------------------------
void Init_GiraffeInfo(void)
{
	// 変数宣言
	int		nCntGiraffe;	// プレイヤーカウント
	int		nCntModel;		// キリンのカウント

	// キリンの初期設定 //
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
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

		// カラー情報
		g_giraffe[nCntGiraffe].nCntCor = 0;
		g_giraffe[nCntGiraffe].diffuse = 1.0f;

		// ステータス情報
		g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;
		g_giraffe[nCntGiraffe].status.nCntStatus = 0;
		g_giraffe[nCntGiraffe].status.nAttack = GIRAFFE_ATTACK;
		g_giraffe[nCntGiraffe].status.nLife = GIRAFFE_HP;
		g_giraffe[nCntGiraffe].status.bColition = false;
		g_giraffe[nCntGiraffe].status.bAttack = false;
		g_giraffe[nCntGiraffe].status.bShot = false;
		g_giraffe[nCntGiraffe].status.bRot = false;
		g_giraffe[nCntGiraffe].status.bMove = false;

		// パーツごとのカウント
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

	// ロードキリン
	LoadGiraffe();
}

// ------------------------------------------------------------------------------------------
// キリンモーション処理
// ------------------------------------------------------------------------------------------
void Init_MotionGiraffe(void)
{
	// 変数宣言
	int		nCntGiraffe;						// キリンカウント

												// モーション用変数
	int		nCntMotion;							// モーションカウント
	int		nCntKeySet;							// モーションカウント
	int		nCntKey;							// キーカウント

												// モーションカウント //
	for (nCntMotion = 0; nCntMotion < GIRAFFEMOSIONSET_MAX; nCntMotion++)
	{
		// モーションの初期設定
		g_giraffeKey[nCntMotion].nNumKey = 0;		// ナンバーキー
		g_giraffeKey[nCntMotion].nLoop = 0;		// ループの有無

												// 当たり判定用 //
		g_giraffeKey[nCntMotion].Collision_nParts = 0;		// パーツ番号
		g_giraffeKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
		g_giraffeKey[nCntMotion].Collision_Radius = 0.0f;	// 円の半径
		g_giraffeKey[nCntMotion].Collision_StartFram = 0;	// 開始フレーム数
		g_giraffeKey[nCntMotion].Collision_EndFram = 0;		// 終了フレーム数
		g_giraffeKey[nCntMotion].Collision_Damage = 0;		// ダメージ数

															// 軌跡用 //
		g_giraffeKey[nCntMotion].Orbit_nParts = 0;			// パーツ番号
		g_giraffeKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
		g_giraffeKey[nCntMotion].Orbit_Mesh = 0;			// 軌跡の線
		g_giraffeKey[nCntMotion].Orbit_StartFram = 0;		// 開始フレーム数
		g_giraffeKey[nCntMotion].Orbit_EndFram = 0;			// 終了フレーム数
		g_giraffeKey[nCntMotion].Orbit_nType = 1;			// 軌跡のタイプ
		g_giraffeKey[nCntMotion].Orbit_nIdTrajectory = -1;	// 軌跡の番号
		g_giraffeKey[nCntMotion].Orbit_bUse = false;		// 軌跡の使用状態

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// キーごとに初期化
			for (nCntKey = 0; nCntKey < GIRAFFETYPE_MAX; nCntKey++)
			{
				g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// プレイヤーモーション読み込み
	LoadGiraffeMosion();

	// キリンの初期設定 //
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < GIRAFFEMOSIONSET_MAX; nCntMotion++)
			{
				// 使用状態
				if (g_giraffeKey[nCntMotion].Orbit_bUse == true)
				{
					// 軌跡の設定
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
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitGiraffe(void)
{
	// キリンデータの開放
	for (int nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_giraffeModel[nCntModel].paMesh != NULL)
		{
			g_giraffeModel[nCntModel].paMesh->Release();
			g_giraffeModel[nCntModel].paMesh = NULL;
		}

		// マテリアルの開放
		if (g_giraffeModel[nCntModel].paBuffMat != NULL)
		{
			g_giraffeModel[nCntModel].paBuffMat->Release();
			g_giraffeModel[nCntModel].paBuffMat = NULL;
		}
	}

	// キリンテクスチャの開放
	for (int nCntTexture = 0; nCntTexture < TEXTURE_GIRAFFE; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTextureGiraffe[nCntTexture] != NULL)
		{
			g_paTextureGiraffe[nCntTexture]->Release();
			g_paTextureGiraffe[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateGiraffe(void)
{
	// カウントキリン
	int nCntGiraffe;
	PLAYER *pPlayer = GetPlayer();

	// キリンのカウント
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse)
		{
			// 現在地保存
			g_giraffe[nCntGiraffe].posold = g_giraffe[nCntGiraffe].pos;

			// モーションを保存
			g_giraffe[nCntGiraffe].nMotionTypeOld = g_giraffe[nCntGiraffe].nMotionType;

			// 状態処理
			Update_StateGiraffe(nCntGiraffe);

			// 死んでいない状態
			if (g_giraffe[nCntGiraffe].status.nStatus != GIRAFFESTATUS_DIE)
			{
				if (GetMode() != MODE_TUTORIAL)
				{
					// 攻撃状態でないとき
					if (g_giraffe[nCntGiraffe].status.bAttack == false)
					{
						// 発射状態でないとき
						if (g_giraffe[nCntGiraffe].status.bShot == false)
						{
							// 移動状態ではないとき
							if (g_giraffe[nCntGiraffe].status.bMove == false)
							{
								// 回転処理
								Update_RotGiraffe(nCntGiraffe);
							}

							// 移動状態ではないとき
							if (g_giraffe[nCntGiraffe].status.bRot == false)
							{
								// 移動処理
								Update_MoveGiraffe(nCntGiraffe);
							}
						}
					}

					// 移動・回転状態ではないとき
					if (g_giraffe[nCntGiraffe].status.bMove == false)
					{
						// 移動・回転状態ではないとき
						if (g_giraffe[nCntGiraffe].status.bRot == false)
						{
							if (g_giraffe[nCntGiraffe].status.bShot == false)
							{
								// 普通攻撃の処理
								Update_NormalAttackGiraffe(nCntGiraffe);
							}

							if (g_giraffe[nCntGiraffe].status.bAttack == false)
							{
								// 弾の処理
								Update_ShotGiraffe(nCntGiraffe);
							}
						}
					}
				}
				// 攻撃の当たり判定
				Update_DecisionGiraffe(nCntGiraffe);

			}

			g_giraffe[nCntGiraffe].move.y -= 0.5f;
			g_giraffe[nCntGiraffe].pos.y += g_giraffe[nCntGiraffe].move.y;

			// 当たり判定
			Update_ColisionGiraffe(nCntGiraffe);

			// 死んでいない状態
			if (g_giraffe[nCntGiraffe].status.nStatus != GIRAFFESTATUS_DIE)
			{
				// モーション処理
				Update_MosionGiraffe(nCntGiraffe);
			}

			// 影の位置を設定
			SetPositionShadow(
				g_giraffe[nCntGiraffe].nIdxShadow,
				g_giraffe[nCntGiraffe].pos +
				D3DXVECTOR3(
					sinf(-g_giraffe[nCntGiraffe].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_giraffe[nCntGiraffe].rot.y) * 10.0f));

			// 攻撃が当たっている
			if (g_giraffe[nCntGiraffe].status.bColition == true)
			{
				// 攻撃が終わったら
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_giraffe[nCntGiraffe].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 状態処理
// ------------------------------------------------------------------------------------------
void Update_StateGiraffe(int nCntGiraffe)
{
	switch (g_giraffe[nCntGiraffe].status.nStatus)
	{
	case GIRAFFESTATUS_NEUTRAL:
		g_giraffe[nCntGiraffe].diffuse = 1.0f;
		break;

	case GIRAFFESTATUS_DAMAGE:
		// カラーの状態カウントアップ
		g_giraffe[nCntGiraffe].nCntCor++;

		// 表示状態
		if (g_giraffe[nCntGiraffe].nCntCor % 10 == 0)
		{
			g_giraffe[nCntGiraffe].diffuse = 1.0f;
		}

		// 非表示状態
		else if (g_giraffe[nCntGiraffe].nCntCor % 5 == 0)
		{
			g_giraffe[nCntGiraffe].diffuse = 0.5f;
		}

		if (g_giraffe[nCntGiraffe].nCntCor == 50)
		{
			// カラーの状態カウント初期化
			g_giraffe[nCntGiraffe].nCntCor = 0;

			// キリンの状態をノーマルへ
			g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;
		}

		break;

	case GIRAFFESTATUS_DIE:
		// 目標回転量から現在の回転量の差分
		g_giraffe[nCntGiraffe].rotbetween.z =
			(g_giraffe[nCntGiraffe].rotLast.z - g_giraffe[nCntGiraffe].rot.z) / 10;

		// 回転
		g_giraffe[nCntGiraffe].rot.z += g_giraffe[nCntGiraffe].rotbetween.z;

		// 重力
		g_giraffe[nCntGiraffe].move.y -= 0.5f;	// 移動量
		g_giraffe[nCntGiraffe].pos.y += g_giraffe[nCntGiraffe].move.y;

		// ステータスのカウントアップ
		g_giraffe[nCntGiraffe].status.nCntStatus++;

		DeleteBillboard(BILLBOARDTYPE_ENTER);
		// ステータスのカウントが規定値以上になったら
		if (g_giraffe[nCntGiraffe].status.nCntStatus == 70)
		{
			if (GetMode() == MODE_TUTORIAL)
			{
				// バッファロー出現
				SetBuffalo(
					g_giraffe[nCntGiraffe].pos,
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
						g_giraffe[nCntGiraffe].pos.x,
						g_giraffe[nCntGiraffe].pos.y,
						g_giraffe[nCntGiraffe].pos.z),

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
			g_giraffe[nCntGiraffe].bUse = false;
			g_giraffe[nCntGiraffe].bDisp = false;

			// 影の消去
			DeleteShadow(g_giraffe[nCntGiraffe].nIdxShadow);

			// 予測線の消去
			DeletePrediction(g_giraffe[nCntGiraffe].nIdxPrediction);

			// 使用中の軌跡の消去
			DeleteTrajectory(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Orbit_nIdTrajectory);

			// ステータスのカウントの初期化
			g_giraffe[nCntGiraffe].status.nCntStatus = 0;

			// ステータス状況
			g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_NEUTRAL;

			// アイテム出現
			SetItem(
				g_giraffe[nCntGiraffe].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// 移動量
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// サイズ
				20.0f,								// 半径
				1,									// テクスチャー
				100);								// 出現時間

			// 撃破数
			g_giraffeTotal++;

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
void Update_RotGiraffe(int nCntGiraffe)
{
	// 方向転換番号
	int nRandNum = 0;

	// 移動状態ではないとき
	if (g_giraffe[nCntGiraffe].status.bRot == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 500;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// 目的の回転量
			g_giraffe[nCntGiraffe].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// 回転量が上限値を超えたら
			if (g_giraffe[nCntGiraffe].rotLast.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotLast.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += D3DX_PI * 2;
			}

			// 回転状態
			g_giraffe[nCntGiraffe].status.bRot = true;
		}
	}

	// 目的回転量から現在回転量の差分
	g_giraffe[nCntGiraffe].rotbetween.y = (g_giraffe[nCntGiraffe].rotLast.y - g_giraffe[nCntGiraffe].rot.y) * 0.01f;
	g_giraffe[nCntGiraffe].rot.y += g_giraffe[nCntGiraffe].rotbetween.y;

	// 回転状態終了
	if (g_giraffe[nCntGiraffe].rot.y >= -0.1f ||
		g_giraffe[nCntGiraffe].rot.y <= 0.1f)
	{
		g_giraffe[nCntGiraffe].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// 移動処理
// ------------------------------------------------------------------------------------------
void Update_MoveGiraffe(int nCntGiraffe)
{
	// 方向転換番号
	int nRandNum = 0;
	int nRotNum = 0;
	float fRandMove;

	// 移動状態ではないとき
	if (g_giraffe[nCntGiraffe].status.bMove == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 200;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// ランダムで移動量をきめる
			fRandMove = float(rand() % 100);

			// 目的移動量
			g_giraffe[nCntGiraffe].posLast.x = 
				sinf(g_giraffe[nCntGiraffe].rot.y - D3DX_PI) * fRandMove + g_giraffe[nCntGiraffe].pos.x;

			g_giraffe[nCntGiraffe].posLast.z = 
				cosf(g_giraffe[nCntGiraffe].rot.y - D3DX_PI) * fRandMove + g_giraffe[nCntGiraffe].pos.z;
		
			// 移動状態
			g_giraffe[nCntGiraffe].status.bMove = true;
		}
	}

	// 移動状態
	if (g_giraffe[nCntGiraffe].status.bMove == true)
	{
		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_MOVE;
		// 目的移動量から現在移動量の差分
		g_giraffe[nCntGiraffe].move.x = (g_giraffe[nCntGiraffe].posLast.x - g_giraffe[nCntGiraffe].pos.x) * 0.05f;
		g_giraffe[nCntGiraffe].move.z = (g_giraffe[nCntGiraffe].posLast.z - g_giraffe[nCntGiraffe].pos.z) * 0.05f;
	}

	// 位置情報の更新
	// 重力
	g_giraffe[nCntGiraffe].move.y -= GIRAFFE_G;	// 移動量
	g_giraffe[nCntGiraffe].pos += g_giraffe[nCntGiraffe].move;

	// 移動状態終了
	if (g_giraffe[nCntGiraffe].move.x >= -0.1f && g_giraffe[nCntGiraffe].move.x <= 0.1f &&
		g_giraffe[nCntGiraffe].move.z >= -0.1f && g_giraffe[nCntGiraffe].move.z <= 0.1f)
	{
		g_giraffe[nCntGiraffe].status.bMove = false;

		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// モーション処理
// ------------------------------------------------------------------------------------------
void Update_MosionGiraffe(int nCntGiraffe)
{
	// 変数宣言
	int	nCntModel;			// カウントキリン

		// 現在のモーションと前回のモーションを比較
	if (g_giraffe[nCntGiraffe].nMotionType != g_giraffe[nCntGiraffe].nMotionTypeOld)
	{
		g_giraffe[nCntGiraffe].nFram = 0;
		g_giraffe[nCntGiraffe].nCntKeySet = 0;
		g_giraffe[nCntGiraffe].nCntDamage = 0;
	}

	for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		// フレーム数が０の場合
		if (g_giraffe[nCntGiraffe].nFram == 0)
		{
			if (g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram != 0)
			{
				// 最終地点と現在地点の差分分割
				g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween =
					(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].rot -
						g_giraffe[nCntGiraffe].model[nCntModel].rot) /
						(float)g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram;

				// 最終地点と現在地点の差分
				g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween =
					(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].pos -
						g_giraffe[nCntGiraffe].model[nCntModel].pos) /
						(float)g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram;
			}

			else
			{
				// 最終地点と現在地点の差分分割
				g_giraffe[nCntGiraffe].model[nCntModel].rot =
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].rot;

				// 最終地点と現在地点の差分
				g_giraffe[nCntGiraffe].model[nCntModel].pos =
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].key[nCntModel].pos;

				// 差分の初期化
				g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 差分の初期化
				g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// 回転する
		g_giraffe[nCntGiraffe].model[nCntModel].rot +=
			g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween;

		// 移動する
		g_giraffe[nCntGiraffe].model[nCntModel].pos +=
			g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween;
	}

	// フレーム数が規定値と同じになったら
	if (g_giraffe[nCntGiraffe].nFram ==
		g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[g_giraffe[nCntGiraffe].nCntKeySet].nFram)
	{
		// キーセットのカウントアップ
		g_giraffe[nCntGiraffe].nCntKeySet++;

		// フレーム数初期化
		g_giraffe[nCntGiraffe].nFram = 0;

		// キーセット数が規定値と同じになったら
		if (g_giraffe[nCntGiraffe].nCntKeySet == g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].nNumKey)
		{
			// ループしないとき
			if (g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].nLoop == 0)
			{

				// キーセット数初期化
				g_giraffe[nCntGiraffe].nCntKeySet = 0;

				g_giraffe[nCntGiraffe].nMotionType = 0;
			}

			// ループする時
			else
			{
				// キーセット数初期化
				g_giraffe[nCntGiraffe].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// フレーム数カウントアップ
		g_giraffe[nCntGiraffe].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// 普通攻撃の処理
// ------------------------------------------------------------------------------------------
void Update_NormalAttackGiraffe(int nCntGiraffe)
{
	// 変数宣言
	int	nAttackNum;				// 攻撃番号
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーとキリンとの距離
	diffpos = pPlayerPos - g_giraffe[nCntGiraffe].pos;

	// 攻撃行動範囲
	if ((GIRAFFE_NORMALATTACK_DECISION * GIRAFFE_NORMALATTACK_DECISION) >=	// 攻撃行動の円範囲
		diffpos.x * diffpos.x +												// x軸の円
		diffpos.y * diffpos.y +												// y軸の円
		diffpos.z * diffpos.z)												// z軸の円
	{
		// 攻撃の確立
		nAttackNum = rand() % GIRAFFE_FREQUENCY;

		// 攻撃
		if (nAttackNum == 0 &&
			g_giraffe[nCntGiraffe].status.bAttack == false)
		{
			PlaySound(SOUND_LABEL_SE_ATTACK);

			// エフェクト発生

			// 普通攻撃モーション
			g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NORMALATACK;

			// 軌跡の発生
			OccurrenceTrajectory(
				g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Orbit_nIdTrajectory,
				nCntGiraffe);

			// 攻撃状態ON
			g_giraffe[nCntGiraffe].status.bAttack = true;

			// バッファローから見てプレイヤーがいる角度
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// 目的の回転量
			g_giraffe[nCntGiraffe].rotLast.y = fAngle - D3DX_PI;

			// 回転量が上限値を超えたら
			if (g_giraffe[nCntGiraffe].rotLast.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotLast.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += D3DX_PI * 2;
			}

			// 目的回転量から現在回転量の差分
			g_giraffe[nCntGiraffe].rotbetween.y =
				(g_giraffe[nCntGiraffe].rotLast.y - g_giraffe[nCntGiraffe].rot.y) * 0.1f;

			g_giraffe[nCntGiraffe].rot.y += g_giraffe[nCntGiraffe].rotbetween.y;

			// 回転量が上限値を超えたら
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

	// モーションが終わったら
	if (g_giraffe[nCntGiraffe].status.bAttack == true &&
		g_giraffe[nCntGiraffe].nMotionType != GIRAFFEMOSIONSET_NORMALATACK)
	{
		// 攻撃状態OFF
		g_giraffe[nCntGiraffe].status.bAttack = false;
	}
}

// ------------------------------------------------------------------------------------------
// 弾処理
// ------------------------------------------------------------------------------------------
void Update_ShotGiraffe(int nCntGiraffe)
{
	// 変数宣言
	int	nAttackNum;				// 攻撃番号
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーとキリンとの距離
	diffpos = pPlayerPos - g_giraffe[nCntGiraffe].pos;

	// 攻撃行動範囲
	if ((GIRAFFE_SHOT_DECISION * GIRAFFE_SHOT_DECISION) >=	// 攻撃行動の円範囲
		diffpos.x * diffpos.x +								// x軸の円
		diffpos.y * diffpos.y +								// y軸の円
		diffpos.z * diffpos.z)								// z軸の円
	{
		// 攻撃の確立
		nAttackNum = rand() % GIRAFFE_FREQUENCY;

		// 攻撃
		if (nAttackNum == 0 &&
			g_giraffe[nCntGiraffe].status.bShot == false)
		{
			// プレイヤーの位置を取得
			g_giraffe[nCntGiraffe].posAttack = GetPlayer()->pos;
			g_giraffe[nCntGiraffe].EffectCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_giraffe[nCntGiraffe].nCntAnim = 0;
			// 予測線の発生
			g_giraffe[nCntGiraffe].nIdxPrediction =
				SetPrediction(
					g_giraffe[nCntGiraffe].posAttack + D3DXVECTOR3(0.0f,20.0f,0.0f),
					D3DXVECTOR3(20.0f, 20.0f, 20.0f),
					D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

			// 普通攻撃モーション
			g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_SHOT;

			// 攻撃状態ON
			g_giraffe[nCntGiraffe].status.bShot = true;
		}
	}

	// 弾処理
	if (g_giraffe[nCntGiraffe].nMotionType == GIRAFFEMOSIONSET_SHOT)
	{
		// 発射する前
		if (g_giraffe[nCntGiraffe].nCntKeySet >= 0 &&
			g_giraffe[nCntGiraffe].nCntKeySet <= 2)
		{
			// 緑のいろの減少
			g_giraffe[nCntGiraffe].EffectCol.g -= 1.0f /
				(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[0].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[1].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[2].nFram);
			// 青のいろの減少
			g_giraffe[nCntGiraffe].EffectCol.b -= 1.0f /
				(g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[0].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[1].nFram +
					g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].KeyInfo[2].nFram);

			g_giraffe[nCntGiraffe].nCntAnim++;
			// キリンから見てプレイヤーがいる角度
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// 目的の回転量
			g_giraffe[nCntGiraffe].rotLast.y = fAngle - D3DX_PI;

			// 回転量が上限値を超えたら
			if (g_giraffe[nCntGiraffe].rotLast.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotLast.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotLast.y += D3DX_PI * 2;
			}

			// 目的回転量から現在回転量の差分
			g_giraffe[nCntGiraffe].rotbetween.y =
				(g_giraffe[nCntGiraffe].rotLast.y - g_giraffe[nCntGiraffe].rot.y);

			// 回転量が上限値を超えたら
			if (g_giraffe[nCntGiraffe].rotbetween.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rotbetween.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rotbetween.y += D3DX_PI * 2;
			}

			g_giraffe[nCntGiraffe].rot.y += g_giraffe[nCntGiraffe].rotbetween.y * 0.1f;

			// 回転量が上限値を超えたら
			if (g_giraffe[nCntGiraffe].rot.y > D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rot.y += -D3DX_PI * 2;
			}
			else if (g_giraffe[nCntGiraffe].rot.y < -D3DX_PI)
			{
				g_giraffe[nCntGiraffe].rot.y += D3DX_PI * 2;
			}
			// プレイヤーの位置を取得
			g_giraffe[nCntGiraffe].posAttack = GetPlayer()->pos;
			// 予測線の位置情報設定
			SetPositionPrediction(g_giraffe[nCntGiraffe].nIdxPrediction, g_giraffe[nCntGiraffe].posAttack + D3DXVECTOR3(0.0f, 20.0f, 0.0f));

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
						g_giraffe[nCntGiraffe].EffectCol.g,
						g_giraffe[nCntGiraffe].EffectCol.b,
						1.0f);

				// エフェクトの設定
				SetEffect(
					D3DXVECTOR3(													// 位置
						g_giraffe[nCntGiraffe].pos.x + sinf(fXZAngle) * 20.0f,
						0.0f,
						g_giraffe[nCntGiraffe].pos.z + cosf(fXZAngle) * 20.0f),
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
		else if (g_giraffe[nCntGiraffe].nCntKeySet == 3)
		{
			// 発射状態ではない場合
			if (g_giraffe[nCntGiraffe].status.bShot == true)
			{
				g_giraffe[nCntGiraffe].nCntDamage++;
				// 技の発動時間
				if (g_giraffe[nCntGiraffe].nCntDamage >= 20)
				{
					PlaySound(SOUND_LABEL_SE_BEAM);

					// 爆発設定
					SetExplosion(
						g_giraffe[nCntGiraffe].posAttack,
						EXPLOSIONTYPE_THUNDER,
						50.0f,
						g_giraffe[nCntGiraffe].status.nAttack);
					// 発射状態
					g_giraffe[nCntGiraffe].status.bShot = false;
					g_giraffe[nCntGiraffe].nCntDamage = 0;
					// 予測線の削除
					DeletePrediction(g_giraffe[nCntGiraffe].nIdxPrediction);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 攻撃の当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_DecisionGiraffe(int nCntGiraffe)
{
	// プレイヤー情報取得
	PLAYER * pPlayer = GetPlayer();

	// 条件判定
	if (g_giraffe[nCntGiraffe].status.nStatus != GIRAFFESTATUS_DIE &&			// 敵が生きているかどうか
		g_giraffe[nCntGiraffe].nMotionType == GIRAFFEMOSIONSET_NORMALATACK &&	// 攻撃状態かどうか
		pPlayer->status.bColition == false)										// プレイヤーの攻撃を受けているかどうか
	{
		// プレイヤーの攻撃時のキリンとプレイヤーの当たり判定
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
			// キリンのダメージ処理
			HitPlayer(
				g_giraffe[nCntGiraffe].status.nAttack +
				g_giraffeKey[g_giraffe[nCntGiraffe].nMotionType].Collision_Damage);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionGiraffe(int nCntGiraffe)
{
	// 床の当たり判定 //
	if (ColisionFLOOR(&g_giraffe[nCntGiraffe].pos,
		&g_giraffe[nCntGiraffe].posold,
		&g_giraffe[nCntGiraffe].move,
		&g_giraffe[nCntGiraffe].model[0].size,
		&g_giraffe[nCntGiraffe].pMeshField))
	{

	}

	// 素材の当たり判定 //
	if (ColisionMaterials(&g_giraffe[nCntGiraffe].pos,
		&g_giraffe[nCntGiraffe].posold,
		&g_giraffe[nCntGiraffe].move,
		&g_giraffe[nCntGiraffe].model[0].size,
		&g_giraffe[nCntGiraffe].pMaterials) != -1)
	{
		// 目標地点を現在地店へ
		g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;

		// モーションを自然体へ戻す
		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NEUTRAL;
	}

	// 壁の当たり判定
	if (ColisionWall(&g_giraffe[nCntGiraffe].pos,
		&g_giraffe[nCntGiraffe].posold,
		&g_giraffe[nCntGiraffe].move,
		&g_giraffe[nCntGiraffe].model[0].size,
		&g_giraffe[nCntGiraffe].pWall))
	{
		// 目標地点を現在地店へ
		g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;

		// モーションを自然体へ戻す
		g_giraffe[nCntGiraffe].nMotionType = GIRAFFEMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawGiraffe(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ
	int nCntGiraffe;
	int nCntModel;
	int nCnt = 0;

	// カウントキリン
	for (nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_giraffe[nCntGiraffe].mtxWorldGiraffe);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_giraffe[nCntGiraffe].rot.y, 
				g_giraffe[nCntGiraffe].rot.x, 
				g_giraffe[nCntGiraffe].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].mtxWorldGiraffe,
				&g_giraffe[nCntGiraffe].mtxWorldGiraffe, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_giraffe[nCntGiraffe].pos.x, 
				g_giraffe[nCntGiraffe].pos.y, 
				g_giraffe[nCntGiraffe].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].mtxWorldGiraffe,	// 1
				&g_giraffe[nCntGiraffe].mtxWorldGiraffe,					// 2
				&mtxTrans);									// 3

			// カウントキリン
			for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_giraffe[nCntGiraffe].model[nCntModel].rot.y,
					g_giraffe[nCntGiraffe].model[nCntModel].rot.x, 
					g_giraffe[nCntGiraffe].model[nCntModel].rot.z);

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
				D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
					&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel, &mtxRot);

				// 位置を反映 //
				// 平行移動行列作成(オフセット)
				D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
					g_giraffe[nCntGiraffe].model[nCntModel].pos.x,
					g_giraffe[nCntGiraffe].model[nCntModel].pos.y, 
					g_giraffe[nCntGiraffe].model[nCntModel].pos.z);	// それぞれの移動量

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
				D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,	// 1
					&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [すべての親]じゃない場合
				if (g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent != -1)
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].mtxWorldGiraffe);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_giraffeModel[nCntModel].paBuffMat->GetBufferPointer();

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_giraffeModel[nCntModel].nNumMat; nCntMat++, pMat++,nCnt++)
				{
					// 本体のカラー情報更新
					pMat->MatD3D.Diffuse.a = g_giraffe[nCntGiraffe].diffuse;

					// テクスチャーあり
					if (pMat->pTextureFilename != NULL)
					{
						// テクスチャー設定
						pDevice->SetTexture(0, g_paTextureGiraffe[0]);
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
					g_giraffeModel[nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitGiraffe(int nCntGiraffe, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 攻撃が当たっている
	g_giraffe[nCntGiraffe].status.bColition = true;

	// カラーの状態カウント初期化
	g_giraffe[nCntGiraffe].nCntCor = 0;

	// HPが減少
	g_giraffe[nCntGiraffe].status.nLife -= nDamage;

	// HPが0になったら
	if (g_giraffe[nCntGiraffe].status.nLife <= 0)
	{
		// 死亡状態
		g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_DIE;

		// 目標回転量
		g_giraffe[nCntGiraffe].rotLast.z = D3DX_PI / 2;

		// 移動量
		g_giraffe[nCntGiraffe].move.y = 5.0f;

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
					g_giraffe[nCntGiraffe].pos.x,
					g_giraffe[nCntGiraffe].pos.y,
					g_giraffe[nCntGiraffe].pos.z),

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
		g_giraffe[nCntGiraffe].move.y = 5.0f;

		// ダメージ状態
		g_giraffe[nCntGiraffe].status.nStatus = GIRAFFESTATUS_DAMAGE;

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
					g_giraffe[nCntGiraffe].pos.x,
					g_giraffe[nCntGiraffe].pos.y,
					g_giraffe[nCntGiraffe].pos.z),
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
void ColisionGiraffe(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// キリンキリンの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadGiraffeModel(void)
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

				// キリンセットが来たら
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// エンドキリンセットが来るまでループ
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeModel[nCntModel].nIdxModelParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_giraffeModel[nCntModel].pos.x,
								&g_giraffeModel[nCntModel].pos.y, 
								&g_giraffeModel[nCntModel].pos.z);

							// 位置情報保存
							g_giraffeModel[nCntModel].posOrigin = g_giraffeModel[nCntModel].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffeModel[nCntModel].rot.x, 
								&g_giraffeModel[nCntModel].rot.y,
								&g_giraffeModel[nCntModel].rot.z);
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
		MessageBox(NULL, "キリンのパーツデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// キリン情報の読み込み処理
// ------------------------------------------------------------------------------------------
void LoadGiraffe(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntGiraffe = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		// ファイル開
		pFile = fopen("data/SAVE/TUTORIAL/GIRAFFE/giraffe.txt", "r");
	}

	// ボス用
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/GIRAFFE/giraffe.txt", "r");
	}

	// それ以外
	else
	{
		// ファイル開
		pFile = fopen("data/SAVE/GIRAFFE/giraffe.txt", "r");
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

				// キリンセットが来たら
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// エンドキリンセットが来るまでループ
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_giraffe[nCntGiraffe].pos.x,
								&g_giraffe[nCntGiraffe].pos.y,
								&g_giraffe[nCntGiraffe].pos.z);

							// 目的位置に保存
							g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffe[nCntGiraffe].rot.x,
								&g_giraffe[nCntGiraffe].rot.y,
								&g_giraffe[nCntGiraffe].rot.z);

							// 影の作成
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

					// 使用状態
					g_giraffe[nCntGiraffe].bUse = true;

					// 表示状態
					g_giraffe[nCntGiraffe].bDisp = true;

					// キリンの更新
					nCntGiraffe++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "キリンのデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// キリンモーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadGiraffeMosion(void)
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
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeKey[nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeKey[nCntMotion].nNumKey);
						}

						// 当たり判定情報読み込み
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

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_giraffeKey[nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
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

							// キャラクタータイプ
							g_giraffeKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_GIRAFFE;

							// 使用状態
							g_giraffeKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_giraffeKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_giraffe[0].model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
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
		MessageBox(NULL, "キリンのモーションデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// キリン情報処理
// ------------------------------------------------------------------------------------------
GIRAFFE *GetGiraffe(void)
{
	return &g_giraffe[0];
}


// ------------------------------------------------------------------------------------------
// キリン撃破数処理
// ------------------------------------------------------------------------------------------
int GetGiraffeTotal(void)
{
	return g_giraffeTotal;
}

// ------------------------------------------------------------------------------------------
// キリン設置処理
// ------------------------------------------------------------------------------------------
void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// カウントキリン
	for (int nCntGiraffe = 0; nCntGiraffe < MAX_BUFFALO; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false)
		{
			// 位置情報
			g_giraffe[nCntGiraffe].pos = pos;


			// 目的位置に保存
			g_giraffe[nCntGiraffe].posLast = g_giraffe[nCntGiraffe].pos;

			// 回転情報
			g_giraffe[nCntGiraffe].rot = rot;

			// 使用状態
			g_giraffe[nCntGiraffe].bUse = true;

			// 表示状態
			g_giraffe[nCntGiraffe].bDisp = true;

			// 影の作成
			g_giraffe[nCntGiraffe].nIdxShadow = SetShadow(
				g_giraffe[nCntGiraffe].pos,
				D3DXVECTOR3(20.0f, 0.0f, 20.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ステータス情報
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
