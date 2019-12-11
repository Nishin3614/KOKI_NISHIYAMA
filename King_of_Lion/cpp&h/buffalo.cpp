// ------------------------------------------------------------------------------------------
//
// バッファロー処理 [buffalo.cpp]
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
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_BUFFALO (1)					// テクスチャーの数
#define BUFFALO_NORMALATTACK_DECISION (500)	// 攻撃範囲
#define BUFFALO_ADDMOVE (5.0f)				// スピード
#define BUFFALO_HP (50)						// HP
#define BUFFALO_ATTACK (15)					// 攻撃力
#define BUFFALO_FREQUENCY (100)				// 攻撃頻度

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
// 初期化
void Init_PartsBuffalo(void);					// パーツ
void Init_BuffaloInfo(void);					// バッファロー
void Init_MotionBuffalo(void);					// モーション
// 更新
void Update_StateBuffalo(int nCntBuffalo);		// 状態
void Update_RotBuffalo(int nCntBuffalo);		// 回転
void Update_MoveBuffalo(int nCntBuffalo);		// 移動
void Update_MosionBuffalo(int nCntBuffalo);		// モーション
void Update_ASSAULT(int nCntBuffalo);			// 攻撃
void Update_DecisionBuffalo(int nCntBuffalo);	// 攻撃当たり判定
void Update_ColisionBuffalo(int nCntBuffalo);	// 当たり判定
// 読み取り
void LoadBuffaloModel(void);					// モデル情報
void LoadBuffalo(void);							// バッファロー情報
void LoadBuffaloMosion(void);					// モーション情報

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureBuffalo[TEXTURE_BUFFALO] = {};	// テクスチャへのポインタ
BUFFALO				g_buffalo[MAX_BUFFALO];						// バッファロー情報
MODEL				g_buffaloModel[BUFFALOTYPE_MAX];			// バッファロー情報
MOSION_BUFFALO		g_buffaloKey[BUFFALOMOSIONSET_MAX];			// モーション情報
int					g_buffaloTotal;								// バッファローの撃破合計
// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitBuffalo(void)
{
	// 撃破数
	g_buffaloTotal = 0;
	// バッファローパーツ情報
	Init_PartsBuffalo();
	// バッファロー情報
	Init_BuffaloInfo();
	// バッファローモーション情報
	Init_MotionBuffalo();
}

// ------------------------------------------------------------------------------------------
// バッファローパーツ情報処理
// ------------------------------------------------------------------------------------------
void Init_PartsBuffalo(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	int		nCntModel;							// バッファローのカウント

	// バッファローファイル名定義
	const char *paBuffaloFile[BUFFALOTYPE_MAX] =
	{
		{ "data/MODEL/buffalo/00_Buffalo_body.x" },				// [0]体
		{ "data/MODEL/buffalo/01_Buffalo_head_joint.x" },		// [1]頭の関節
		{ "data/MODEL/buffalo/02_Buffalo_head.x" },				// [2]頭
		{ "data/MODEL/buffalo/03_Buffalo_LightHand_joint.x" },	// [3]右手の関節
		{ "data/MODEL/buffalo/04_Buffalo_LightHand.x" },		// [4]右手
		{ "data/MODEL/buffalo/05_Buffalo_LeftHand_joint.x" },	// [5]左手の関節
		{ "data/MODEL/buffalo/06_Buffalo_LeftHand.x" },			// [6]左手
		{ "data/MODEL/buffalo/07_Buffalo_LightLeg_joint.x" },	// [7]右足の関節
		{ "data/MODEL/buffalo/08_Buffalo_LightLeg.x" },			// [8]右足
		{ "data/MODEL/buffalo/09_Buffalo_LeftLeg_joint.x" },	// [9]左足の関節
		{ "data/MODEL/buffalo/10_Buffalo_LeftLeg.x" },			// [10]左足
		{ "data/MODEL/buffalo/11_Buffalo_Tail_joint.x" },		// [11]しっぽの関節
		{ "data/MODEL/buffalo/12_Buffalo_Tail.x" }				// [12]しっぽ
	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_BUFFALO] =
	{
		{ "" },
	};

	// パーツの読み込み //
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

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			paBuffaloFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_buffaloModel[nCntModel].paBuffMat,
			NULL,
			&g_buffaloModel[nCntModel].nNumMat,
			&g_buffaloModel[nCntModel].paMesh);

		// バッファローの頂点座標の最大値・最小値の設定
		int nNumVertex;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

								// 頂点数を取得
		nNumVertex = g_buffaloModel[nCntModel].paMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_buffaloModel[nCntModel].paMesh->GetFVF());

		// 頂点バッファをロック
		g_buffaloModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// 頂点数分繰り返す
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// 番地情報のD3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
			// x //
			// 最小値
			if (g_buffaloModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_buffaloModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// 最大値
			if (g_buffaloModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_buffaloModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// 最小値
			if (g_buffaloModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_buffaloModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// 最大値
			if (g_buffaloModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_buffaloModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// 最小値
			if (g_buffaloModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_buffaloModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// 最大値
			if (g_buffaloModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_buffaloModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// サイズポインタを進める
			pVertexBuffer += sizeFVF;
		}

		// アンロック
		g_buffaloModel[nCntModel].paMesh->UnlockVertexBuffer();

		// 最大値と最小値の距離
		g_buffaloModel[nCntModel].size = g_buffaloModel[nCntModel].vtxMaxMaterials - g_buffaloModel[nCntModel].vtxMinMaterials;
	}

	// パーツ読込
	LoadBuffaloModel();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureBuffalo[0]);
}

// ------------------------------------------------------------------------------------------
// バッファロー情報処理
// ------------------------------------------------------------------------------------------
void Init_BuffaloInfo(void)
{
	// 変数宣言
	int		nCntBuffalo;	// プレイヤーカウント
	int		nCntModel;		// バッファローのカウント

	// バッファローの初期設定 //
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
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

		// カラー情報
		g_buffalo[nCntBuffalo].nCntCor = 0;
		g_buffalo[nCntBuffalo].diffuse = 1.0f;

		// ステータス情報
		g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;
		g_buffalo[nCntBuffalo].status.nCntStatus = 0;
		g_buffalo[nCntBuffalo].status.nAttack = BUFFALO_ATTACK;
		g_buffalo[nCntBuffalo].status.nLife = BUFFALO_HP;
		g_buffalo[nCntBuffalo].status.bColition = false;
		g_buffalo[nCntBuffalo].status.bAttack = false;
		g_buffalo[nCntBuffalo].status.bRot = false;
		g_buffalo[nCntBuffalo].status.bMove = false;

		// パーツごとのカウント
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

	// ロードバッファロー
	LoadBuffalo();
}

// ------------------------------------------------------------------------------------------
// バッファローモーション処理
// ------------------------------------------------------------------------------------------
void Init_MotionBuffalo(void)
{
	// 変数宣言
	int		nCntBuffalo;						// バッファローカウント

												// モーション用変数
	int		nCntMotion;							// モーションカウント
	int		nCntKeySet;							// モーションカウント
	int		nCntKey;							// キーカウント

												// モーションカウント //
	for (nCntMotion = 0; nCntMotion < BUFFALOMOSIONSET_MAX; nCntMotion++)
	{
		// モーションの初期設定
		g_buffaloKey[nCntMotion].nNumKey = 0;		// ナンバーキー
		g_buffaloKey[nCntMotion].nLoop = 0;		// ループの有無

												// 当たり判定用 //
		g_buffaloKey[nCntMotion].Collision_nParts = 0;		// パーツ番号
		g_buffaloKey[nCntMotion].Collision_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
		g_buffaloKey[nCntMotion].Collision_Radius = 0.0f;	// 円の半径
		g_buffaloKey[nCntMotion].Collision_StartFram = 0;	// 開始フレーム数
		g_buffaloKey[nCntMotion].Collision_EndFram = 0;		// 終了フレーム数
		g_buffaloKey[nCntMotion].Collision_Damage = 0;		// ダメージ数

															// 軌跡用 //
		g_buffaloKey[nCntMotion].Orbit_nParts = 0;			// パーツ番号
		g_buffaloKey[nCntMotion].Orbit_Ofset =
			D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
		g_buffaloKey[nCntMotion].Orbit_Mesh = 0;			// 軌跡の線
		g_buffaloKey[nCntMotion].Orbit_StartFram = 0;		// 開始フレーム数
		g_buffaloKey[nCntMotion].Orbit_EndFram = 0;			// 終了フレーム数
		g_buffaloKey[nCntMotion].Orbit_nType = 1;			// 軌跡のタイプ
		g_buffaloKey[nCntMotion].Orbit_nIdTrajectory = -1;	// 軌跡の番号
		g_buffaloKey[nCntMotion].Orbit_bUse = false;		// 軌跡の使用状態

		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_buffaloKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// キーごとに初期化
			for (nCntKey = 0; nCntKey < BUFFALOTYPE_MAX; nCntKey++)
			{
				g_buffaloKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_buffaloKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// プレイヤーモーション読み込み
	LoadBuffaloMosion();

	// バッファローの初期設定 //
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse == true)
		{

			for (nCntMotion = 0; nCntMotion < BUFFALOMOSIONSET_MAX; nCntMotion++)
			{
				// 使用状態
				if (g_buffaloKey[nCntMotion].Orbit_bUse == true)
				{
					// 軌跡の設定
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
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitBuffalo(void)
{
	// バッファローデータの開放
	for (int nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_buffaloModel[nCntModel].paMesh != NULL)
		{
			g_buffaloModel[nCntModel].paMesh->Release();
			g_buffaloModel[nCntModel].paMesh = NULL;
		}

		// マテリアルの開放
		if (g_buffaloModel[nCntModel].paBuffMat != NULL)
		{
			g_buffaloModel[nCntModel].paBuffMat->Release();
			g_buffaloModel[nCntModel].paBuffMat = NULL;
		}
	}

	// バッファローテクスチャの開放
	for (int nCntTexture = 0; nCntTexture < TEXTURE_BUFFALO; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTextureBuffalo[nCntTexture] != NULL)
		{
			g_paTextureBuffalo[nCntTexture]->Release();
			g_paTextureBuffalo[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateBuffalo(void)
{
	// カウントバッファロー
	int nCntBuffalo;
	PLAYER *pPlayer = GetPlayer();

	// バッファローのカウント
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse)
		{
			// 現在地保存
			g_buffalo[nCntBuffalo].posold = g_buffalo[nCntBuffalo].pos;

			// モーションを保存
			g_buffalo[nCntBuffalo].nMotionTypeOld = g_buffalo[nCntBuffalo].nMotionType;

			// 状態処理
			Update_StateBuffalo(nCntBuffalo);

			// 死んでいない状態
			if (g_buffalo[nCntBuffalo].status.nStatus != BUFFALOSTATUS_DIE)
			{
				// 攻撃状態でないとき
				if (g_buffalo[nCntBuffalo].status.bAttack == false)
				{
					// 移動状態ではないとき
					if (g_buffalo[nCntBuffalo].status.bMove == false)
					{
						// 回転処理
						Update_RotBuffalo(nCntBuffalo);
					}

					// 移動状態ではないとき
					if (g_buffalo[nCntBuffalo].status.bRot == false)
					{
						// 移動処理
						Update_MoveBuffalo(nCntBuffalo);
					}

				}

				// 移動・回転状態ではないとき
				if (g_buffalo[nCntBuffalo].status.bMove == false)
				{
					// 移動・回転状態ではないとき
					if (g_buffalo[nCntBuffalo].status.bRot == false)
					{
						// 普通攻撃の処理
						Update_ASSAULT(nCntBuffalo);
					}
				}

				// 攻撃の当たり判定
				Update_DecisionBuffalo(nCntBuffalo);
			}
			g_buffalo[nCntBuffalo].move.y -= 0.5f;
			g_buffalo[nCntBuffalo].pos.y += g_buffalo[nCntBuffalo].move.y;

			// 当たり判定
			Update_ColisionBuffalo(nCntBuffalo);

			// 死んでいない状態
			if (g_buffalo[nCntBuffalo].status.nStatus != BUFFALOSTATUS_DIE)
			{
				// モーション処理
				Update_MosionBuffalo(nCntBuffalo);
			}
			// 影の位置を設定
			SetPositionShadow(
				g_buffalo[nCntBuffalo].nIdxShadow,
				g_buffalo[nCntBuffalo].pos +
				D3DXVECTOR3(
					sinf(-g_buffalo[nCntBuffalo].rot.y) * 10.0f,
					0.0f,
					cosf(D3DX_PI - g_buffalo[nCntBuffalo].rot.y) * 10.0f));

			// 攻撃が当たっている
			if (g_buffalo[nCntBuffalo].status.bColition == true)
			{
				// 攻撃が終わったら
				if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
				{
					g_buffalo[nCntBuffalo].status.bColition = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 状態処理
// ------------------------------------------------------------------------------------------
void Update_StateBuffalo(int nCntBuffalo)
{
	switch (g_buffalo[nCntBuffalo].status.nStatus)
	{
	case BUFFALOSTATUS_NEUTRAL:
		g_buffalo[nCntBuffalo].diffuse = 1.0f;
		break;

	case BUFFALOSTATUS_DAMAGE:
		// カラーの状態カウントアップ
		g_buffalo[nCntBuffalo].nCntCor++;

		// 表示状態
		if (g_buffalo[nCntBuffalo].nCntCor % 10 == 0)
		{
			g_buffalo[nCntBuffalo].diffuse = 1.0f;
		}

		// 非表示状態
		else if (g_buffalo[nCntBuffalo].nCntCor % 5 == 0)
		{
			g_buffalo[nCntBuffalo].diffuse = 0.5f;
		}

		if (g_buffalo[nCntBuffalo].nCntCor == 50)
		{
			// カラーの状態カウント初期化
			g_buffalo[nCntBuffalo].nCntCor = 0;

			// バッファローの状態をノーマルへ
			g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;
		}

		break;

	case BUFFALOSTATUS_DIE:
		// 目標回転量から現在の回転量の差分
		g_buffalo[nCntBuffalo].rotbetween.z =
			(g_buffalo[nCntBuffalo].rotLast.z - g_buffalo[nCntBuffalo].rot.z) / 10;

		// 回転の限界計算
		g_buffalo[nCntBuffalo].rotbetween.z =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rotbetween.z);

		// 回転
		g_buffalo[nCntBuffalo].rot.z += g_buffalo[nCntBuffalo].rotbetween.z;


		g_buffalo[nCntBuffalo].rot.z =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rot.z);

		// 重力
		g_buffalo[nCntBuffalo].move.y -= 0.5f;	// 移動量
		g_buffalo[nCntBuffalo].pos.y += g_buffalo[nCntBuffalo].move.y;

		// ステータスのカウントアップ
		g_buffalo[nCntBuffalo].status.nCntStatus++;

		// ステータスのカウントが規定値以上になったら
		if (g_buffalo[nCntBuffalo].status.nCntStatus == 70)
		{
			if (GetMode() == MODE_TUTORIAL)
			{
				// バッファロー出現
				SetGiraffe(
					g_buffalo[nCntBuffalo].pos,
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
						cosf(fAngle) * 5.0f,
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
						g_buffalo[nCntBuffalo].pos.x,
						g_buffalo[nCntBuffalo].pos.y,
						g_buffalo[nCntBuffalo].pos.z),

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
			g_buffalo[nCntBuffalo].bUse = false;
			g_buffalo[nCntBuffalo].bDisp = false;

			// 影の消去
			DeleteShadow(g_buffalo[nCntBuffalo].nIdxShadow);

			// 使用中の軌跡の消去
			DeleteTrajectory(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Orbit_nIdTrajectory);

			// ステータスのカウントの初期化
			g_buffalo[nCntBuffalo].status.nCntStatus = 0;

			// ステータス状況
			g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_NEUTRAL;

			// アイテム出現
			SetItem(
				g_buffalo[nCntBuffalo].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// 移動量
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// サイズ
				20.0f,								// 半径
				0,									// テクスチャー
				100);								// 出現時間

			// 撃破数
			g_buffaloTotal++;

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
void Update_RotBuffalo(int nCntBuffalo)
{
	// 方向転換番号
	int nRandNum = 0;

	// 移動状態ではないとき
	if (g_buffalo[nCntBuffalo].status.bRot == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 500;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// 目的の回転量
			g_buffalo[nCntBuffalo].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			g_buffalo[nCntBuffalo].rotLast.y =
				Rot_One_Limit(g_buffalo[nCntBuffalo].rotLast.y);

			// 回転状態
			g_buffalo[nCntBuffalo].status.bRot = true;
		}
	}

	// 目的回転量から現在回転量の差分
	g_buffalo[nCntBuffalo].rotbetween.y = (g_buffalo[nCntBuffalo].rotLast.y - g_buffalo[nCntBuffalo].rot.y) * 0.01f;

	g_buffalo[nCntBuffalo].rotbetween.y =
		Rot_One_Limit(g_buffalo[nCntBuffalo].rotbetween.y);

	g_buffalo[nCntBuffalo].rot.y += g_buffalo[nCntBuffalo].rotbetween.y;

	g_buffalo[nCntBuffalo].rot.y =
		Rot_One_Limit(g_buffalo[nCntBuffalo].rot.y);

	// 回転状態終了
	if (g_buffalo[nCntBuffalo].rot.y >= -0.1f ||
		g_buffalo[nCntBuffalo].rot.y <= 0.1f)
	{
		g_buffalo[nCntBuffalo].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// 移動処理
// ------------------------------------------------------------------------------------------
void Update_MoveBuffalo(int nCntBuffalo)
{
	// 方向転換番号
	int nRandNum = 0;
	int nRotNum = 0;
	float fRandMove;

	// 移動状態ではないとき
	if (g_buffalo[nCntBuffalo].status.bMove == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 200;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// ランダムで移動量をきめる
			fRandMove = float(rand() % 100);

			// 目的回転量
			g_buffalo[nCntBuffalo].posLast.x =
				sinf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * fRandMove + g_buffalo[nCntBuffalo].pos.x;

			g_buffalo[nCntBuffalo].posLast.z =
				cosf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * fRandMove + g_buffalo[nCntBuffalo].pos.z;

			// 移動状態
			g_buffalo[nCntBuffalo].status.bMove = true;
		}
	}

	// 目的回転量から現在回転量の差分
	g_buffalo[nCntBuffalo].move.x = (g_buffalo[nCntBuffalo].posLast.x - g_buffalo[nCntBuffalo].pos.x) * 0.05f;
	g_buffalo[nCntBuffalo].move.z = (g_buffalo[nCntBuffalo].posLast.z - g_buffalo[nCntBuffalo].pos.z) * 0.05f;

	// 位置情報の更新
	g_buffalo[nCntBuffalo].pos += g_buffalo[nCntBuffalo].move;

	// 移動状態終了
	if (g_buffalo[nCntBuffalo].move.x >= -0.1f && g_buffalo[nCntBuffalo].move.x <= 0.1f &&
		g_buffalo[nCntBuffalo].move.z >= -0.1f && g_buffalo[nCntBuffalo].move.z <= 0.1f)
	{
		g_buffalo[nCntBuffalo].status.bMove = false;

		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
	}

	// 移動状態
	if (g_buffalo[nCntBuffalo].status.bMove == true)
	{
		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// モーション処理
// ------------------------------------------------------------------------------------------
void Update_MosionBuffalo(int nCntBuffalo)
{
	// 変数宣言
	int	nCntModel;			// カウントバッファロー

	// 現在のモーションと前回のモーションを比較
	if (g_buffalo[nCntBuffalo].nMotionType != g_buffalo[nCntBuffalo].nMotionTypeOld)
	{
 		g_buffalo[nCntBuffalo].nFram = 0;
		g_buffalo[nCntBuffalo].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
	{
		// フレーム数が０の場合
		if (g_buffalo[nCntBuffalo].nFram == 0)
		{
			if (g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram != 0)
			{
				// 最終地点と現在地点の差分分割
				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween =
					(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].rot -
						g_buffalo[nCntBuffalo].model[nCntModel].rot) /
						(float)g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram;

				// 回転の限界計算
				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.x =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.x);

				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.y =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.y);

				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.z =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween.z);

				// 最終地点と現在地点の差分
				g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween =
					(g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].pos -
						g_buffalo[nCntBuffalo].model[nCntModel].pos) /
						(float)g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram;
			}

			else
			{
				// 最終地点と現在地点の差分分割
				g_buffalo[nCntBuffalo].model[nCntModel].rot =
					g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].rot;

				// 回転の限界計算
				g_buffalo[nCntBuffalo].model[nCntModel].rot.x =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.x);

				g_buffalo[nCntBuffalo].model[nCntModel].rot.y =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.y);

				g_buffalo[nCntBuffalo].model[nCntModel].rot.z =
					Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.z);

				// 最終地点と現在地点の差分
				g_buffalo[nCntBuffalo].model[nCntModel].pos =
					g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].key[nCntModel].pos;

				// 差分の初期化
				g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 差分の初期化
				g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}
		// 回転する
		g_buffalo[nCntBuffalo].model[nCntModel].rot +=
			g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween;

		// 回転の限界計算
		g_buffalo[nCntBuffalo].model[nCntModel].rot.x =
			Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.x);

		g_buffalo[nCntBuffalo].model[nCntModel].rot.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.y);

		g_buffalo[nCntBuffalo].model[nCntModel].rot.z =
			Rot_One_Limit(g_buffalo[nCntBuffalo].model[nCntModel].rot.z);


		// 移動する
		g_buffalo[nCntBuffalo].model[nCntModel].pos +=
			g_buffalo[nCntBuffalo].model[nCntModel].posKeyBetween;
	}

	// フレーム数が規定値と同じになったら
	if (g_buffalo[nCntBuffalo].nFram ==
		g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].KeyInfo[g_buffalo[nCntBuffalo].nCntKeySet].nFram)
	{
		// キーセットのカウントアップ
		g_buffalo[nCntBuffalo].nCntKeySet++;

		// フレーム数初期化
		g_buffalo[nCntBuffalo].nFram = 0;

		// キーセット数が規定値と同じになったら
		if (g_buffalo[nCntBuffalo].nCntKeySet == g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].nNumKey)
		{
			// ループしないとき
			if (g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].nLoop == 0)
			{
				// キーセット数初期化
				g_buffalo[nCntBuffalo].nCntKeySet = 0;

				// モーション切り替え
				// 突進待機時
				if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT_WAIT)
				{
					g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_ASSAULT;

					// 切り替えカウントの初期化
					g_buffalo[nCntBuffalo].nCntMotion = 0;

					for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
					{
						// 回転量の初期化
						g_buffalo[nCntBuffalo].model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_buffalo[nCntBuffalo].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_buffalo[nCntBuffalo].model[nCntModel].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}

				// それ以外
				else
				{
					g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
				}
			}
			// ループする時
			else
			{
				// キーセット数初期化
				g_buffalo[nCntBuffalo].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// フレーム数カウントアップ
		g_buffalo[nCntBuffalo].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// 突進攻撃の処理
// ------------------------------------------------------------------------------------------
void Update_ASSAULT(int nCntBuffalo)
{
	int	nAttackNum;	// 攻撃番号
	D3DXVECTOR3 diffpos;						// 距離
	D3DXVECTOR3 pPlayerPos = GetPlayer_Pos();	// プレイヤー情報取得

	// 差分
	diffpos = pPlayerPos - g_buffalo[nCntBuffalo].pos;

	// 攻撃行動範囲
	if ((BUFFALO_NORMALATTACK_DECISION * BUFFALO_NORMALATTACK_DECISION) >=	// 攻撃行動の円範囲
		diffpos.x * diffpos.x +												// x軸の円
		diffpos.y * diffpos.y +												// y軸の円
		diffpos.z * diffpos.z)												// z軸の円
	{
		// 攻撃の確立
		nAttackNum = rand() % BUFFALO_FREQUENCY;

		// 攻撃
		if (nAttackNum == 0 &&
			g_buffalo[nCntBuffalo].status.bAttack == false)
		{
			// エフェクト発生


			// 普通攻撃モーション
			g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_ASSAULT_WAIT;

			// 攻撃状態ON
			g_buffalo[nCntBuffalo].status.bAttack = true;
		}
	}

	// 突進待機モーションだったら
	if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT_WAIT)
	{
		// バッファローから見てプレイヤーがいる角度
		float	fAngle = atan2f(diffpos.x, diffpos.z);

		// 目的の回転量
		g_buffalo[nCntBuffalo].rotLast.y = fAngle - D3DX_PI;

		g_buffalo[nCntBuffalo].rotLast.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rotLast.y);

		// 目的回転量から現在回転量の差分
		g_buffalo[nCntBuffalo].rotbetween.y = 
			(g_buffalo[nCntBuffalo].rotLast.y - g_buffalo[nCntBuffalo].rot.y);

		g_buffalo[nCntBuffalo].rotbetween.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rotbetween.y);

		g_buffalo[nCntBuffalo].rot.y += g_buffalo[nCntBuffalo].rotbetween.y * 0.1f;
		
		g_buffalo[nCntBuffalo].rot.y =
			Rot_One_Limit(g_buffalo[nCntBuffalo].rot.y);
	}

	// 突進モーションだったら
	if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT)
	{
		// 目的移動量
		g_buffalo[nCntBuffalo].move.x =
			sinf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * BUFFALO_ADDMOVE;

		g_buffalo[nCntBuffalo].move.z =
			cosf(g_buffalo[nCntBuffalo].rot.y - D3DX_PI) * BUFFALO_ADDMOVE;

		
		// 位置情報の更新
		g_buffalo[nCntBuffalo].pos.x += g_buffalo[nCntBuffalo].move.x;

		// 位置情報の更新
		g_buffalo[nCntBuffalo].pos.z += g_buffalo[nCntBuffalo].move.z;

		// 目的位置情報の更新
		g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

		// 切り替えカウントアップ
		g_buffalo[nCntBuffalo].nCntMotion++;

		// ５秒たったら切り替え
		if (g_buffalo[nCntBuffalo].nCntMotion == 300)
		{
			// モーションを自然体へ戻す
			g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;

			// 切り替えカウントの初期化
 			g_buffalo[nCntBuffalo].nCntMotion = 0;
		}
	}

	// モーションが終わったら
	if (g_buffalo[nCntBuffalo].nMotionType != BUFFALOMOSIONSET_ASSAULT_WAIT &&
		g_buffalo[nCntBuffalo].nMotionType != BUFFALOMOSIONSET_ASSAULT)
	{
		// 攻撃状態OFF
		g_buffalo[nCntBuffalo].status.bAttack = false;
	}
}

// ------------------------------------------------------------------------------------------
// 攻撃の当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_DecisionBuffalo(int nCntBuffalo)
{
	// プレイヤー情報取得
	PLAYER * pPlayer = GetPlayer();

	// 敵が生きているかどうか
	if (g_buffalo[nCntBuffalo].status.nStatus != BUFFALOSTATUS_DIE)
	{
		// 攻撃状態かどうか
		if (g_buffalo[nCntBuffalo].nMotionType == BUFFALOMOSIONSET_ASSAULT)
		{
			// プレイヤーの攻撃を受けているかどうか
			if (pPlayer->status.bColition == false)
			{
				// プレイヤーの攻撃時のバッファローとプレイヤーの当たり判定
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
					// バッファローのダメージ処理
					HitPlayer(
						g_buffalo[nCntBuffalo].status.nAttack +
						g_buffaloKey[g_buffalo[nCntBuffalo].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionBuffalo(int nCntBuffalo)
{
	// 床の当たり判定 //
	if (ColisionFLOOR(&g_buffalo[nCntBuffalo].pos,
		&g_buffalo[nCntBuffalo].posold,
		&g_buffalo[nCntBuffalo].move,
		&g_buffalo[nCntBuffalo].model[0].size,
		&g_buffalo[nCntBuffalo].pMeshField))
	{

	}

	// 素材の当たり判定 //
	if (ColisionMaterials(&g_buffalo[nCntBuffalo].pos,
		&g_buffalo[nCntBuffalo].posold,
		&g_buffalo[nCntBuffalo].move,
		&g_buffalo[nCntBuffalo].model[0].size,
		&g_buffalo[nCntBuffalo].pMaterials) != -1)
	{
		// 目標地点を現在地店へ
		g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

		// モーションを自然体へ戻す
		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
	}

	// 壁の当たり判定
	if (ColisionWall(&g_buffalo[nCntBuffalo].pos,
		&g_buffalo[nCntBuffalo].posold,
		&g_buffalo[nCntBuffalo].move,
		&g_buffalo[nCntBuffalo].model[0].size,
		&g_buffalo[nCntBuffalo].pWall))
	{
		// 目標地点を現在地店へ
		g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

		// モーションを自然体へ戻す
		g_buffalo[nCntBuffalo].nMotionType = BUFFALOMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawBuffalo(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ
	int nCntBuffalo;
	int nCntModel;
	int nCnt = 0;

	// カウントバッファロー
	for (nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_buffalo[nCntBuffalo].mtxWorldBuffalo);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_buffalo[nCntBuffalo].rot.y,
				g_buffalo[nCntBuffalo].rot.x,
				g_buffalo[nCntBuffalo].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].mtxWorldBuffalo,
				&g_buffalo[nCntBuffalo].mtxWorldBuffalo, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_buffalo[nCntBuffalo].pos.x,
				g_buffalo[nCntBuffalo].pos.y,
				g_buffalo[nCntBuffalo].pos.z);	// それぞれの移動量

												// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].mtxWorldBuffalo,	// 1
				&g_buffalo[nCntBuffalo].mtxWorldBuffalo,					// 2
				&mtxTrans);									// 3

															// カウントバッファロー
			for (nCntModel = 0; nCntModel < BUFFALOTYPE_MAX; nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_buffalo[nCntBuffalo].model[nCntModel].rot.y,
					g_buffalo[nCntBuffalo].model[nCntModel].rot.x,
					g_buffalo[nCntBuffalo].model[nCntModel].rot.z);

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
				D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
					&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel, &mtxRot);

				// 位置を反映 //
				// 平行移動行列作成(オフセット)
				D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
					g_buffalo[nCntBuffalo].model[nCntModel].pos.x,
					g_buffalo[nCntBuffalo].model[nCntModel].pos.y,
					g_buffalo[nCntBuffalo].model[nCntModel].pos.z);	// それぞれの移動量

																	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
				D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,	// 1
					&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


																// [すべての親]じゃない場合
				if (g_buffalo[nCntBuffalo].model[nCntModel].nIdxModelParent != -1)
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].model[g_buffalo[nCntBuffalo].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel,
						&g_buffalo[nCntBuffalo].mtxWorldBuffalo);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_buffalo[nCntBuffalo].model[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_buffaloModel[nCntModel].paBuffMat->GetBufferPointer();

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_buffaloModel[nCntModel].nNumMat; nCntMat++, pMat++, nCnt++)
				{
					// 本体のカラー情報更新
					pMat->MatD3D.Diffuse.a = g_buffalo[nCntBuffalo].diffuse;

					// テクスチャーあり
					if (pMat->pTextureFilename != NULL)
					{
						// テクスチャー設定
						pDevice->SetTexture(0, g_paTextureBuffalo[0]);
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
					g_buffaloModel[nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitBuffalo(int nCntBuffalo, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 攻撃が当たっている
	g_buffalo[nCntBuffalo].status.bColition = true;

	// カラーの状態カウント初期化
	g_buffalo[nCntBuffalo].nCntCor = 0;

	// HPが減少
	g_buffalo[nCntBuffalo].status.nLife -= nDamage;
	
	PlaySound(SOUND_LABEL_SE_COLISION);

	// HPが0になったら
	if (g_buffalo[nCntBuffalo].status.nLife <= 0)
	{
		// 死亡状態
		g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_DIE;

		// 目標回転量
		g_buffalo[nCntBuffalo].rotLast.z = D3DX_PI / 2;

		// 移動量
		g_buffalo[nCntBuffalo].move.y = 5.0f;

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
					g_buffalo[nCntBuffalo].pos.x,
					g_buffalo[nCntBuffalo].pos.y,
					g_buffalo[nCntBuffalo].pos.z),

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
		g_buffalo[nCntBuffalo].status.nStatus = BUFFALOSTATUS_DAMAGE;

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
					g_buffalo[nCntBuffalo].pos.x,
					g_buffalo[nCntBuffalo].pos.y,
					g_buffalo[nCntBuffalo].pos.z),
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
// バッファローの当たり判定処理
// ------------------------------------------------------------------------------------------
void ColisionBuffalo(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// バッファロー設定処理
// ------------------------------------------------------------------------------------------
void SetBuffalo(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// カウントバッファロー
	for (int nCntBuffalo = 0; nCntBuffalo < MAX_BUFFALO; nCntBuffalo++)
	{
		if (g_buffalo[nCntBuffalo].bUse == false)
		{
			// 位置情報
			g_buffalo[nCntBuffalo].pos = pos;


			// 目的位置に保存
			g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;

			// 回転情報
			g_buffalo[nCntBuffalo].rot = rot;

			// 使用状態
			g_buffalo[nCntBuffalo].bUse = true;

			// 表示状態
			g_buffalo[nCntBuffalo].bDisp = true;

			// 影の作成
			g_buffalo[nCntBuffalo].nIdxShadow = SetShadow(
				g_buffalo[nCntBuffalo].pos,
				D3DXVECTOR3(20.0f, 0.0f, 20.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ステータス情報
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
// バッファローバッファローの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadBuffaloModel(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/BUFFALO/buffalo_model.txt", "r");

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

				// バッファローセットが来たら
				else if (strcmp(cHeadText, "PARTSSET") == 0)
				{

					// エンドバッファローセットが来るまでループ
					while (strcmp(cHeadText, "END_PARTSSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_buffaloModel[nCntModel].nIdxModelParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_buffaloModel[nCntModel].pos.x,
								&g_buffaloModel[nCntModel].pos.y,
								&g_buffaloModel[nCntModel].pos.z);

							// 位置情報保存
							g_buffaloModel[nCntModel].posOrigin = g_buffaloModel[nCntModel].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_buffaloModel[nCntModel].rot.x,
								&g_buffaloModel[nCntModel].rot.y,
								&g_buffaloModel[nCntModel].rot.z);
						}
					}
					// バッファローの更新
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
		MessageBox(NULL, "バッファローのパーツデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// バッファロー情報の読み込み処理
// ------------------------------------------------------------------------------------------
void LoadBuffalo(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntBuffalo = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ボス用
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/BUFFALO/buffalo.txt", "r");
	}

	// チュートリアル
	else if (GetMode() == MODE_TUTORIAL)
	{
		return;
	}

	// それ以外
	else
	{
		// ファイル開
		pFile = fopen("data/SAVE/BUFFALO/buffalo.txt", "r");
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

				// バッファローセットが来たら
				else if (strcmp(cHeadText, "BUFFALOSET") == 0)
				{

					// エンドバッファローセットが来るまでループ
					while (strcmp(cHeadText, "END_BUFFALOSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_buffalo[nCntBuffalo].pos.x,
								&g_buffalo[nCntBuffalo].pos.y,
								&g_buffalo[nCntBuffalo].pos.z);

							// 目的位置に保存
							g_buffalo[nCntBuffalo].posLast = g_buffalo[nCntBuffalo].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_buffalo[nCntBuffalo].rot.x,
								&g_buffalo[nCntBuffalo].rot.y,
								&g_buffalo[nCntBuffalo].rot.z);

							// 使用状態
							g_buffalo[nCntBuffalo].bUse = true;

							// 表示状態
							g_buffalo[nCntBuffalo].bDisp = true;

							// 影の作成
							g_buffalo[nCntBuffalo].nIdxShadow = SetShadow(
								g_buffalo[nCntBuffalo].pos,
								D3DXVECTOR3(20.0f, 0.0f, 20.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}
					// バッファローの更新
					nCntBuffalo++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "バッファローのデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// バッファローモーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadBuffaloMosion(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntMotion = 0;							// プレイヤーカウント
	int	nCntModel = 0;							// バッファローカウント
	int	nCntKeySet = 0;							// モーションカウント
	int	nCntKey = 0;							// キーカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

							// ファイル開
	pFile = fopen("data/SAVE/BUFFALO/buffalo_motion.txt", "r");

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
					// バッファローの初期化
					nCntModel = 0;

					// エンドバッファローセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_buffaloKey[nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_buffaloKey[nCntMotion].nNumKey);
						}

						// 当たり判定情報読み込み
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

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_buffaloKey[nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
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

							// キャラクタータイプ
							g_buffaloKey[nCntMotion].Orbit_nType = TRAJECTORYTYPE_BUFFALO;

							// 使用状態
							g_buffaloKey[nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_buffaloKey[nCntMotion].KeyInfo[nCntModel].nFram);
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
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_buffalo[0].model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_buffaloKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// カウントキー
									nCntKey++;
								}
							}

							// カウントバッファロー
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
		MessageBox(NULL, "バッファローのモーションデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// バッファロー情報処理
// ------------------------------------------------------------------------------------------
BUFFALO *GetBuffalo(void)
{
	return &g_buffalo[0];
}

// ------------------------------------------------------------------------------------------
// バッファロー撃破数処理
// ------------------------------------------------------------------------------------------
int GetBuffaloTotal(void)
{
	return g_buffaloTotal;
}
