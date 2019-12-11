// ------------------------------------------------------------------------------------------
//
// 敵処理 [enemy.cpp]
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
// マクロ定義
// ------------------------------------------------------------------------------------------
#define	MAX_ENEMY (128)		// 最大数
#define ENEMY_HP (30)
#define ENEMY_ATTACK (10)					// 攻撃力
#define ENEMY_FREQUENCY (100)				// 攻撃頻度
#define ENEMY_NORMALATTACK_DECISION (100)
#define ENEMY_SHOT_DECISION (500)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
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
// グローバル変数
// ------------------------------------------------------------------------------------------
ENEMY				g_enemy[MAX_ENEMY];						// 敵情報

ENEMYMODEL			g_enemyModel[ENEMY_TYPE][ENEMY_PARTS];			// 敵情報

MOSION_ENEMY		g_enemyKey[ENEMY_TYPE][ENEMY_MOTION];			// モーション情報

int					g_enemyTotal;								// 敵撃破数合計


// 敵専用変数
ENEMYMODELINFO		g_enemyModelInfo[ENEMY_TYPE];						// 敵の基本情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitEnemy(void)
{
	g_enemyTotal = 0;
	// 敵パーツ情報
	Init_PartsEnemy();

	// 敵情報
	Init_EnemyInfo();

	// 敵モーション情報
	Init_MotionEnemy();
}

// ------------------------------------------------------------------------------------------
// 敵パーツ情報処理
// ------------------------------------------------------------------------------------------
void Init_PartsEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	int					nCntModel;				// 敵のカウント
	int					nCntType;				// キャラクターの種類
	D3DXMATERIAL		*pMat;					// 現在のマテリアル保存

	// キャラクターの初期化 //
	for (nCntType = 0; nCntType < ENEMY_TYPE; nCntType++)
	{
		// パーツの初期化 //
		for (nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
		{
			// 初期化しなければならない
			g_enemyModel[nCntType][nCntModel].paBuffMat = NULL;
			g_enemyModel[nCntType][nCntModel].paMesh = NULL;
			g_enemyModel[nCntType][nCntModel].nNumMat = 0;
			g_enemyModel[nCntType][nCntModel].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_enemyModel[nCntType][nCntModel].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 基本的に同じ情報
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

		// モーションの初期化
		for (int nCntMotion = 0; nCntMotion < ENEMY_MOTION; nCntMotion++)
		{
			// モーションの初期設定
			g_enemyKey[nCntType][nCntMotion].nNumKey = 0;		// ナンバーキー
			g_enemyKey[nCntType][nCntMotion].nLoop = 0;		// ループの有無

															// 当たり判定用 //
			g_enemyKey[nCntType][nCntMotion].Collision_nParts = 0;		// パーツ番号
			g_enemyKey[nCntType][nCntMotion].Collision_Ofset =
				D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
			g_enemyKey[nCntType][nCntMotion].Collision_Radius = 0.0f;	// 円の半径
			g_enemyKey[nCntType][nCntMotion].Collision_StartFram = 0;	// 開始フレーム数
			g_enemyKey[nCntType][nCntMotion].Collision_EndFram = 0;		// 終了フレーム数
			g_enemyKey[nCntType][nCntMotion].Collision_Damage = 0;		// ダメージ数

																		// 軌跡用 //
			g_enemyKey[nCntType][nCntMotion].Orbit_nParts = 0;			// パーツ番号
			g_enemyKey[nCntType][nCntMotion].Orbit_Ofset =
				D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// オフセット
			g_enemyKey[nCntType][nCntMotion].Orbit_Mesh = 0;			// 軌跡の線
			g_enemyKey[nCntType][nCntMotion].Orbit_StartFram = 0;		// 開始フレーム数
			g_enemyKey[nCntType][nCntMotion].Orbit_EndFram = 0;			// 終了フレーム数
			g_enemyKey[nCntType][nCntMotion].Orbit_nType = 1;			// 軌跡のタイプ
			g_enemyKey[nCntType][nCntMotion].Orbit_nIdTrajectory = -1;	// 軌跡の番号
			g_enemyKey[nCntType][nCntMotion].Orbit_bUse = false;		// 軌跡の使用状態

			for (int nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
			{
				g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

				// キーごとに初期化
				for (int nCntKey = 0; nCntKey < ENEMY_PARTS; nCntKey++)
				{
					g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	// 敵・モーションの読み込み
	LoadEnemyMosion();

	// キャラクターの読み込み //
	for (nCntType = 0; nCntType < 1; nCntType++)
	{
		// パーツの読み込み //
		for (nCntModel = 0; nCntModel < g_enemyModelInfo[nCntType].Num_Model; nCntModel++)
		{
			if (strcmp(&g_enemyModelInfo[nCntType].aFileModel[nCntModel][0], "") != 0)
			{

			// Xファイルの読み込み
			D3DXLoadMeshFromX(
				&g_enemyModelInfo[nCntType].aFileModel[nCntModel][0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_enemyModel[nCntType][nCntModel].paBuffMat,
				NULL,
				&g_enemyModel[nCntType][nCntModel].nNumMat,
				&g_enemyModel[nCntType][nCntModel].paMesh);

				// マテリアル情報に対するポインタを取得
				pMat =
					(D3DXMATERIAL*)g_enemyModel[nCntType][nCntModel].paBuffMat->GetBufferPointer();

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_enemyModel[nCntType][nCntModel].nNumMat; nCntMat++, pMat++)
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(
						pDevice,
						pMat->pTextureFilename,
						&g_enemyModel[nCntType][nCntModel].paTextureEnemy[nCntMat]);
				}


				// 敵の頂点座標の最大値・最小値の設定
				int nNumVertex;			// 頂点数
				DWORD sizeFVF;			// 頂点フォーマットのサイズ
				BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

										// 頂点数を取得
				nNumVertex = g_enemyModel[nCntType][nCntModel].paMesh->GetNumVertices();

				// 頂点フォーマットのサイズを取得
				sizeFVF = D3DXGetFVFVertexSize(g_enemyModel[nCntType][nCntModel].paMesh->GetFVF());

				// 頂点バッファをロック
				g_enemyModel[nCntType][nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

				// 頂点数分繰り返す
				for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
				{
					// 番地情報のD3DXVE
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

					// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
					// x //
					// 最小値
					if (g_enemyModel[nCntType][nCntModel].vtxMinMaterials.x > vtx.x)
					{
						g_enemyModel[nCntType][nCntModel].vtxMinMaterials.x = vtx.x;
					}

					// 最大値
					if (g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.x < vtx.x)
					{
						g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.x = vtx.x;
					}

					// y //
					// 最小値
					if (g_enemyModel[nCntType][nCntModel].vtxMinMaterials.y > vtx.y)
					{
						g_enemyModel[nCntType][nCntModel].vtxMinMaterials.y = vtx.y;
					}

					// 最大値
					if (g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.y < vtx.y)
					{
						g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.y = vtx.y;
					}

					// z //
					// 最小値
					if (g_enemyModel[nCntType][nCntModel].vtxMinMaterials.z > vtx.z)
					{
						g_enemyModel[nCntType][nCntModel].vtxMinMaterials.z = vtx.z;
					}

					// 最大値
					if (g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.z < vtx.z)
					{
						g_enemyModel[nCntType][nCntModel].vtxMaxMaterials.z = vtx.z;
					}

					// サイズポインタを進める
					pVertexBuffer += sizeFVF;
				}

				// アンロック
				g_enemyModel[nCntType][nCntModel].paMesh->UnlockVertexBuffer();

				// 最大値と最小値の距離
				g_enemyModel[nCntType][nCntModel].size = g_enemyModel[nCntType][nCntModel].vtxMaxMaterials - g_enemyModel[nCntType][nCntModel].vtxMinMaterials;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 敵情報処理
// ------------------------------------------------------------------------------------------
void Init_EnemyInfo(void)
{
	// 変数宣言
	int					nCntEnemy;	// プレイヤーカウント
	int					nCntModel;	// 敵のカウント

	// 敵の初期設定 //
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
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

		// カラー情報
		g_enemy[nCntEnemy].nCntCor = 0;
		g_enemy[nCntEnemy].diffuse = 1.0f;

		// ステータス情報
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

	// ロード敵
	LoadEnemy();


	// 敵の初期設定 //
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
			if (g_enemy[nCntEnemy].bUse == true)
			{
				DeleteShadow(g_enemy[nCntEnemy].nIdxShadow);
			}
		}

		// パーツごとのカウント
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
// 敵モーション処理
// ------------------------------------------------------------------------------------------
void Init_MotionEnemy(void)
{
	// 変数宣言
	int		nCntEnemy;		// 敵カウント

	// モーション用変数
	int		nCntMotion;		// モーションカウント

	// 敵の初期設定 //
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true)
		{
			for (nCntMotion = 0; nCntMotion < ENEMY_MOTION; nCntMotion++)
			{
				// 使用状態
				if (g_enemyKey[g_enemy[nCntEnemy].nType][nCntMotion].Orbit_bUse == true)
				{
					// 軌跡の設定
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
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitEnemy(void)
{
	// 敵データの開放
	for (int nCntType = 0; nCntType < ENEMY_TYPE; nCntType++)
	{
		// 敵データの開放
		for (int nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
		{
			// メッシュの開放
			if (g_enemyModel[nCntType][nCntModel].paMesh != NULL)
			{
				g_enemyModel[nCntType][nCntModel].paMesh->Release();
				g_enemyModel[nCntType][nCntModel].paMesh = NULL;
			}

			// マテリアルの開放
			if (g_enemyModel[nCntType][nCntModel].paBuffMat != NULL)
			{
				g_enemyModel[nCntType][nCntModel].paBuffMat->Release();
				g_enemyModel[nCntType][nCntModel].paBuffMat = NULL;
			}

			// 敵テクスチャの開放
			for (int nCntMat = 0; nCntMat < ENEMY_MAT; nCntMat++)
			{
				// テクスチャーの開放
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
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	//// カウント敵
	//int nCntEnemy;
	//PLAYER *pPlayer = GetPlayer();

	//// 敵のカウント
	//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	if (g_enemy[nCntEnemy].bUse)
	//	{
	//		// 現在地保存
	//		g_enemy[nCntEnemy].posold = g_enemy[nCntEnemy].pos;

	//		// モーションを保存
	//		g_enemy[nCntEnemy].nMotionTypeOld = g_enemy[nCntEnemy].nMotionType;

	//		// 状態処理
	//		Update_StateEnemy(nCntEnemy);

	//		if (GetMode() != MODE_TUTORIAL)
	//		{
	//			// 死んでいない状態
	//			if (g_enemy[nCntEnemy].status.nStatus != ENEMYSTATUS_DIE)
	//			{
	//				// 攻撃状態でないとき
	//				if (g_enemy[nCntEnemy].status.bAttack == false)
	//				{
	//					// 発射状態でないとき
	//					if (g_enemy[nCntEnemy].status.bShot == false)
	//					{
	//						// 移動状態ではないとき
	//						if (g_enemy[nCntEnemy].status.bMove == false)
	//						{
	//							// 回転処理
	//							Update_RotEnemy(nCntEnemy);
	//						}

	//						// 移動状態ではないとき
	//						if (g_enemy[nCntEnemy].status.bRot == false)
	//						{
	//							// 移動処理
	//							Update_MoveEnemy(nCntEnemy);
	//						}
	//					}
	//				}

	//				// 移動・回転状態ではないとき
	//				if (g_enemy[nCntEnemy].status.bMove == false)
	//				{
	//					// 移動・回転状態ではないとき
	//					if (g_enemy[nCntEnemy].status.bRot == false)
	//					{
	//						if (g_enemy[nCntEnemy].status.bShot == false)
	//						{
	//							// 普通攻撃の処理
	//							Update_NormalAttackEnemy(nCntEnemy);
	//						}

	//						if (g_enemy[nCntEnemy].status.bAttack == false)
	//						{
	//							if (GetMode() == MODE_GAME)
	//							{
	//								// 弾の処理
	//								Update_ShotEnemy(nCntEnemy);
	//							}
	//						}
	//					}
	//				}

	//				// 攻撃の当たり判定
	//				Update_DecisionEnemy(nCntEnemy);
	//			}
	//		}

	//		// 当たり判定
	//		Update_ColisionEnemy(nCntEnemy);

	//		// 死んでいない状態
	//		if (g_enemy[nCntEnemy].status.nStatus != ENEMYSTATUS_DIE)
	//		{
	//			// モーション処理
	//			Update_MosionEnemy(nCntEnemy);
	//		}

	//		// 影の位置を設定
	//		SetPositionShadow(
	//			g_enemy[nCntEnemy].nIdxShadow, 
	//			g_enemy[nCntEnemy].pos +
	//			D3DXVECTOR3(
	//				sinf(-g_enemy[nCntEnemy].rot.y) * 10.0f,
	//				0.0f,
	//				cosf(D3DX_PI - g_enemy[nCntEnemy].rot.y) * 10.0f));

	//		// 攻撃が当たっている
	//		if (g_enemy[nCntEnemy].status.bColition == true)
	//		{
	//			// 攻撃が終わったら
	//			if (pPlayer->nMotionType != PLAYERMOSIONSET_NORMALATACK)
	//			{
	//				g_enemy[nCntEnemy].status.bColition = false;
	//			}
	//		}
	//	}
	//}
}

// ------------------------------------------------------------------------------------------
// 状態処理
// ------------------------------------------------------------------------------------------
void Update_StateEnemy(int nCntEnemy)
{
	switch (g_enemy[nCntEnemy].status.nStatus)
	{
	case ENEMYSTATUS_NEUTRAL:
		g_enemy[nCntEnemy].diffuse = 1.0f;
		break;

	case ENEMYSTATUS_DAMAGE:
		// カラーの状態カウントアップ
		g_enemy[nCntEnemy].nCntCor++;

		// 表示状態
		if (g_enemy[nCntEnemy].nCntCor % 10 == 0)
		{
			g_enemy[nCntEnemy].diffuse = 1.0f;
		}

		// 非表示状態
		else if (g_enemy[nCntEnemy].nCntCor % 5 == 0)
		{
			g_enemy[nCntEnemy].diffuse = 0.5f;
		}

		if (g_enemy[nCntEnemy].nCntCor == 50)
		{
			// カラーの状態カウント初期化
			g_enemy[nCntEnemy].nCntCor = 0;

			// 敵の状態をノーマルへ
			g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_NEUTRAL;
		}

		break;

	case ENEMYSTATUS_DIE:
		// 目標回転量から現在の回転量の差分
		g_enemy[nCntEnemy].rotbetween.z =
			(g_enemy[nCntEnemy].rotLast.z - g_enemy[nCntEnemy].rot.z) / 10;

		// 回転
		g_enemy[nCntEnemy].rot.z += g_enemy[nCntEnemy].rotbetween.z;

		// 重力
		g_enemy[nCntEnemy].move.y -= 0.5f;	// 移動量
		g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;

		// ステータスのカウントアップ
		g_enemy[nCntEnemy].status.nCntStatus++;

		// ステータスのカウントが規定値以上になったら
		if (g_enemy[nCntEnemy].status.nCntStatus == 70)
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
						g_enemy[nCntEnemy].pos.x,
						g_enemy[nCntEnemy].pos.y,
						g_enemy[nCntEnemy].pos.z),

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
			g_enemy[nCntEnemy].bUse = false;
			g_enemy[nCntEnemy].bDisp = false;

			// 影の消去
			DeleteShadow(g_enemy[nCntEnemy].nIdxShadow);

			//// 使用中の軌跡の消去
			//DeleteTrajectory(g_enemyKey[g_enemy[nCntEnemy].nMotionType].Orbit_nIdTrajectory);

			// ステータスのカウントの初期化
			g_enemy[nCntEnemy].status.nCntStatus = 0;

			// ステータス状況
			g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_NEUTRAL;

			// アイテム出現
			SetItem(
				g_enemy[nCntEnemy].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 回転
				D3DXVECTOR3(0.0f, 10.0f, 0.0f),		// 移動量
				D3DXVECTOR3(20.0f, 20.0f, 0.0f),	// サイズ
				20.0f,								// 半径
				1,									// テクスチャー
				100);								// 出現時間

			// 撃破数
			g_enemyTotal++;

		}

		break;
	default:
		break;
	}

}

// ------------------------------------------------------------------------------------------
// 回転処理
// ------------------------------------------------------------------------------------------
void Update_RotEnemy(int nCntEnemy)
{
	// 方向転換番号
	int nRandNum = 0;

	// 移動状態ではないとき
	if (g_enemy[nCntEnemy].status.bRot == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 500;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// 目的の回転量
			g_enemy[nCntEnemy].rotLast.y += float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// 回転量が上限値を超えたら
			if (g_enemy[nCntEnemy].rotLast.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotLast.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += D3DX_PI * 2;
			}

			// 回転状態
			g_enemy[nCntEnemy].status.bRot = true;
		}
	}

	// 目的回転量から現在回転量の差分
	g_enemy[nCntEnemy].rotbetween.y = (g_enemy[nCntEnemy].rotLast.y - g_enemy[nCntEnemy].rot.y) * 0.01f;
	g_enemy[nCntEnemy].rot.y += g_enemy[nCntEnemy].rotbetween.y;

	// 回転状態終了
	if (g_enemy[nCntEnemy].rot.y >= -0.1f ||
		g_enemy[nCntEnemy].rot.y <= 0.1f)
	{
		g_enemy[nCntEnemy].status.bRot = false;
	}
}

// ------------------------------------------------------------------------------------------
// 移動処理
// ------------------------------------------------------------------------------------------
void Update_MoveEnemy(int nCntEnemy)
{
	// 方向転換番号
	int nRandNum = 0;
	int nRotNum = 0;
	float fRandMove;

	// 移動状態ではないとき
	if (g_enemy[nCntEnemy].status.bMove == false)
	{
		// ランダムで数値を決める
		nRandNum = rand() % 200;

		// 方向転換番号が0なら
		if (nRandNum == 0)
		{
			// ランダムで移動量をきめる
			fRandMove = float(rand() % 100);

			// 目的移動量
			g_enemy[nCntEnemy].posLast.x = 
				sinf(g_enemy[nCntEnemy].rot.y - D3DX_PI) * fRandMove + g_enemy[nCntEnemy].pos.x;

			g_enemy[nCntEnemy].posLast.z = 
				cosf(g_enemy[nCntEnemy].rot.y - D3DX_PI) * fRandMove + g_enemy[nCntEnemy].pos.z;
		
			// 移動状態
			g_enemy[nCntEnemy].status.bMove = true;
		}
	}

	// 目的移動量から現在移動量の差分
	g_enemy[nCntEnemy].move.x = (g_enemy[nCntEnemy].posLast.x - g_enemy[nCntEnemy].pos.x) * 0.05f;
	g_enemy[nCntEnemy].move.z = (g_enemy[nCntEnemy].posLast.z - g_enemy[nCntEnemy].pos.z) * 0.05f;

	// 位置情報の更新
	g_enemy[nCntEnemy].pos += g_enemy[nCntEnemy].move;

	// 移動状態終了
	if (g_enemy[nCntEnemy].move.x >= -0.1f && g_enemy[nCntEnemy].move.x <= 0.1f &&
		g_enemy[nCntEnemy].move.z >= -0.1f && g_enemy[nCntEnemy].move.z <= 0.1f)
	{
		g_enemy[nCntEnemy].status.bMove = false;

		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NEUTRAL;
	}

	// 移動状態
	if (g_enemy[nCntEnemy].status.bMove == true)
	{
		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// モーション処理
// ------------------------------------------------------------------------------------------
void Update_MosionEnemy(int nCntEnemy)
{
	// 変数宣言
	int	nCntModel;			// カウント敵

		// 現在のモーションと前回のモーションを比較
	if (g_enemy[nCntEnemy].nMotionType != g_enemy[nCntEnemy].nMotionTypeOld)
	{

		g_enemy[nCntEnemy].nFram = 0;

		g_enemy[nCntEnemy].nCntKeySet = 0;
	}

	for (nCntModel = 0; nCntModel < ENEMY_PARTS; nCntModel++)
	{
		// フレーム数が０の場合
		if (g_enemy[nCntEnemy].nFram == 0)
		{
			if (g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram != 0)
			{
				// 最終地点と現在地点の差分分割
				g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween =
					(g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].rot -
						g_enemy[nCntEnemy].model[nCntModel].rot) /
						(float)g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram;

				// 最終地点と現在地点の差分
				g_enemy[nCntEnemy].model[nCntModel].posKeyBetween =
					(g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].pos -
						g_enemy[nCntEnemy].model[nCntModel].pos) /
						(float)g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram;
			}

			else
			{
				// 最終地点と現在地点の差分分割
				g_enemy[nCntEnemy].model[nCntModel].rot =
					g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].rot;

				// 最終地点と現在地点の差分
				g_enemy[nCntEnemy].model[nCntModel].pos =
					g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].key[nCntModel].pos;

				// 差分の初期化
				g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 差分の初期化
				g_enemy[nCntEnemy].model[nCntModel].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			}
		}

		// 回転する
		g_enemy[nCntEnemy].model[nCntModel].rot +=
			g_enemy[nCntEnemy].model[nCntModel].rotKeyBetween;

		// 移動する
		g_enemy[nCntEnemy].model[nCntModel].pos +=
			g_enemy[nCntEnemy].model[nCntModel].posKeyBetween;
	}

	// フレーム数が規定値と同じになったら
	if (g_enemy[nCntEnemy].nFram ==
		g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].KeyInfo[g_enemy[nCntEnemy].nCntKeySet].nFram)
	{
		// キーセットのカウントアップ
		g_enemy[nCntEnemy].nCntKeySet++;

		// フレーム数初期化
		g_enemy[nCntEnemy].nFram = 0;

		// キーセット数が規定値と同じになったら
		if (g_enemy[nCntEnemy].nCntKeySet == g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].nNumKey)
		{
			// ループしないとき
			if (g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].nLoop == 0)
			{

				// キーセット数初期化
				g_enemy[nCntEnemy].nCntKeySet = 0;

				g_enemy[nCntEnemy].nMotionType = 0;
			}

			// ループする時
			else
			{
				// キーセット数初期化
				g_enemy[nCntEnemy].nCntKeySet = 0;
			}
		}
	}

	else
	{
		// フレーム数カウントアップ
		g_enemy[nCntEnemy].nFram++;
	}
}

// ------------------------------------------------------------------------------------------
// 普通攻撃の処理
// ------------------------------------------------------------------------------------------
void Update_NormalAttackEnemy(int nCntEnemy)
{
	// 変数宣言
	int	nAttackNum;				// 攻撃番号
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーと敵との距離
	diffpos = pPlayerPos - g_enemy[nCntEnemy].pos;

	// 攻撃行動範囲
	if ((ENEMY_NORMALATTACK_DECISION * ENEMY_NORMALATTACK_DECISION) >=	// 攻撃行動の円範囲
		diffpos.x * diffpos.x +												// x軸の円
		diffpos.y * diffpos.y +												// y軸の円
		diffpos.z * diffpos.z)												// z軸の円
	{
		// 攻撃の確立
		nAttackNum = rand() % ENEMY_FREQUENCY;

		// 攻撃
		if (nAttackNum == 0 &&
			g_enemy[nCntEnemy].status.bAttack == false)
		{
			PlaySound(SOUND_LABEL_SE_ATTACK);
			// 普通攻撃モーション
			g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NORMALATACK;

			// 軌跡の発生
			OccurrenceTrajectory(
				g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Orbit_nIdTrajectory,
				nCntEnemy);

			// 攻撃状態ON
			g_enemy[nCntEnemy].status.bAttack = true;

			// バッファローから見てプレイヤーがいる角度
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// 目的の回転量
			g_enemy[nCntEnemy].rotLast.y = fAngle - D3DX_PI;

			// 回転量が上限値を超えたら
			if (g_enemy[nCntEnemy].rotLast.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotLast.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += D3DX_PI * 2;
			}

			// 目的回転量から現在回転量の差分
			g_enemy[nCntEnemy].rotbetween.y =
				(g_enemy[nCntEnemy].rotLast.y - g_enemy[nCntEnemy].rot.y) * 0.1f;

			g_enemy[nCntEnemy].rot.y += g_enemy[nCntEnemy].rotbetween.y;

			// 回転量が上限値を超えたら
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

	// モーションが終わったら
	if (g_enemy[nCntEnemy].status.bAttack == true &&
		g_enemy[nCntEnemy].nMotionType != ENEMYMOSIONSET_NORMALATACK)
	{
		// 攻撃状態OFF
		g_enemy[nCntEnemy].status.bAttack = false;
	}
}

// ------------------------------------------------------------------------------------------
// 弾処理
// ------------------------------------------------------------------------------------------
void Update_ShotEnemy(int nCntEnemy)
{
	// 変数宣言
	int	nAttackNum;				// 攻撃番号
	D3DXVECTOR3 diffpos;		// プレイヤーまでの距離
	D3DXVECTOR3 pPlayerPos =
		GetPlayer_Pos();		// プレイヤー情報取得

	// プレイヤーと敵との距離
	diffpos = pPlayerPos - g_enemy[nCntEnemy].pos;

	// 攻撃行動範囲
	if ((ENEMY_SHOT_DECISION * ENEMY_SHOT_DECISION) >=	// 攻撃行動の円範囲
		diffpos.x * diffpos.x +								// x軸の円
		diffpos.y * diffpos.y +								// y軸の円
		diffpos.z * diffpos.z)								// z軸の円
	{
		// 攻撃の確立
		nAttackNum = rand() % ENEMY_FREQUENCY;

		// 攻撃
		if (nAttackNum == 0 &&
			g_enemy[nCntEnemy].status.bShot == false)
		{
			// 普通攻撃モーション
			g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_SHOT;

			// 攻撃状態ON
			g_enemy[nCntEnemy].status.bShot = true;
		}
	}

	// 弾処理
	if (g_enemy[nCntEnemy].nMotionType == ENEMYMOSIONSET_SHOT)
	{
		// 発射する前
		if (g_enemy[nCntEnemy].nCntKeySet >= 0 &&
			g_enemy[nCntEnemy].nCntKeySet <= 2)
		{
			// 敵から見てプレイヤーがいる角度
			float	fAngle = atan2f(diffpos.x, diffpos.z);

			// 目的の回転量
			g_enemy[nCntEnemy].rotLast.y = fAngle - D3DX_PI;

			// 回転量が上限値を超えたら
			if (g_enemy[nCntEnemy].rotLast.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotLast.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotLast.y += D3DX_PI * 2;
			}

			// 目的回転量から現在回転量の差分
			g_enemy[nCntEnemy].rotbetween.y =
				(g_enemy[nCntEnemy].rotLast.y - g_enemy[nCntEnemy].rot.y);

			// 回転量が上限値を超えたら
			if (g_enemy[nCntEnemy].rotbetween.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rotbetween.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rotbetween.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotbetween.y += D3DX_PI * 2;
			}

			g_enemy[nCntEnemy].rot.y += g_enemy[nCntEnemy].rotbetween.y * 0.1f;

			// 回転量が上限値を超えたら
			if (g_enemy[nCntEnemy].rot.y > D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += -D3DX_PI * 2;
			}
			else if (g_enemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}
		}

		// 発射タイミング
		else if (g_enemy[nCntEnemy].nCntKeySet == 3)
		{
			// 発射状態ではない場合
			if (g_enemy[nCntEnemy].status.bShot == true)
			{
				PlaySound(SOUND_LABEL_SE_BEAM);

				SetBullet(D3DXVECTOR3(
					g_enemy[nCntEnemy].model[4].mtxWorldModel._41,
					g_enemy[nCntEnemy].model[4].mtxWorldModel._42,
					g_enemy[nCntEnemy].model[4].mtxWorldModel._43),	// 発射位置
					D3DXVECTOR3(4.0f, 0.0f, 4.0f),						// 移動
					g_enemy[nCntEnemy].rot,							// 回転
					D3DXVECTOR3(15.0f, 15.0f,15.0f),						// サイズ
					200,												// HP
					1,													// タイプ
					g_enemy[nCntEnemy].status.nAttack);				// 攻撃力

				// 発射状態
				g_enemy[nCntEnemy].status.bShot = false;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 攻撃の当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_DecisionEnemy(int nCntEnemy)
{
	// プレイヤー情報取得
	PLAYER * pPlayer = GetPlayer();

	// 敵が生きているかどうか
	if (g_enemy[nCntEnemy].status.nStatus != ENEMYSTATUS_DIE)
	{
		// 攻撃状態かどうか
		if (g_enemy[nCntEnemy].nMotionType == ENEMYMOSIONSET_NORMALATACK)
		{
			// プレイヤーの攻撃を受けているかどうか
			if (pPlayer->status.bColition == false)
			{
				// プレイヤーの攻撃時の敵とプレイヤーの当たり判定
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
					// 敵のダメージ処理
					HitPlayer(
						g_enemy[nCntEnemy].status.nAttack +
						g_enemyKey[g_enemy[nCntEnemy].nType][g_enemy[nCntEnemy].nMotionType].Collision_Damage);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionEnemy(int nCntEnemy)
{
	// 床の当たり判定 //
	if (ColisionFLOOR(&g_enemy[nCntEnemy].pos,
		&g_enemy[nCntEnemy].posold,
		&g_enemy[nCntEnemy].move,
		&g_enemy[nCntEnemy].model[0].size,
		&g_enemy[nCntEnemy].pMeshField))
	{

	}

	// 素材の当たり判定 //
	if (ColisionMaterials(&g_enemy[nCntEnemy].pos,
		&g_enemy[nCntEnemy].posold,
		&g_enemy[nCntEnemy].move,
		&g_enemy[nCntEnemy].model[0].size,
		&g_enemy[nCntEnemy].pMaterials) != -1)
	{
		// 目標地点を現在地店へ
		g_enemy[nCntEnemy].posLast = g_enemy[nCntEnemy].pos;

		// モーションを自然体へ戻す
		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NEUTRAL;
	}

	// 壁の当たり判定
	if (ColisionWall(&g_enemy[nCntEnemy].pos,
		&g_enemy[nCntEnemy].posold,
		&g_enemy[nCntEnemy].move,
		&g_enemy[nCntEnemy].model[0].size,
		&g_enemy[nCntEnemy].pWall))
	{
		// 目標地点を現在地店へ
		g_enemy[nCntEnemy].posLast = g_enemy[nCntEnemy].pos;

		// モーションを自然体へ戻す
		g_enemy[nCntEnemy].nMotionType = ENEMYMOSIONSET_NEUTRAL;
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ
	int nCntEnemy;
	int nCntModel;

	// カウント敵
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_enemy[nCntEnemy].mtxWorldEnemy);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_enemy[nCntEnemy].rot.y, 
				g_enemy[nCntEnemy].rot.x, 
				g_enemy[nCntEnemy].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].mtxWorldEnemy,
				&g_enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_enemy[nCntEnemy].pos.x, 
				g_enemy[nCntEnemy].pos.y, 
				g_enemy[nCntEnemy].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].mtxWorldEnemy,	// 1
				&g_enemy[nCntEnemy].mtxWorldEnemy,					// 2
				&mtxTrans);									// 3

			// カウント敵
			for (
				nCntModel = 0;
				nCntModel < g_enemyModelInfo[g_enemy[nCntEnemy].nType].Num_Model; 
				nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_enemy[nCntEnemy].model[nCntModel].rot.y,
					g_enemy[nCntEnemy].model[nCntModel].rot.x, 
					g_enemy[nCntEnemy].model[nCntModel].rot.z);

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
					&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel, &mtxRot);

				// 位置を反映 //
				// 平行移動行列作成(オフセット)
				D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
					g_enemy[nCntEnemy].model[nCntModel].pos.x,
					g_enemy[nCntEnemy].model[nCntModel].pos.y, 
					g_enemy[nCntEnemy].model[nCntModel].pos.z);	// それぞれの移動量

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,	// 1
					&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [すべての親]じゃない場合
				if (g_enemy[nCntEnemy].model[nCntModel].nIdxModelParent != -1)
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].model[g_enemy[nCntEnemy].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel,
						&g_enemy[nCntEnemy].mtxWorldEnemy);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_enemy[nCntEnemy].model[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);
				
				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].paBuffMat->GetBufferPointer();

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].nNumMat; nCntMat++, pMat++)
				{
					// 本体のカラー情報更新
					pMat->MatD3D.Diffuse.a = g_enemy[nCntEnemy].diffuse;

					// テクスチャー設定
					pDevice->SetTexture(
						0, 
						g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].paTextureEnemy[nCntMat]);

					// マテリアルの設定
					pDevice->SetMaterial(&pMat->MatD3D);

					// 描画
					g_enemyModel[g_enemy[nCntEnemy].nType][nCntModel].paMesh->DrawSubset(nCntMat);
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
void HitEnemy(int nCntEnemy, int nDamage)
{
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// 攻撃が当たっている
	g_enemy[nCntEnemy].status.bColition = true;

	// カラーの状態カウント初期化
	g_enemy[nCntEnemy].nCntCor = 0;

	// HPが減少
	g_enemy[nCntEnemy].status.nLife -= nDamage;

	// HPが0になったら
	if (g_enemy[nCntEnemy].status.nLife <= 0)
	{
		// 死亡状態
		g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_DIE;

		// 目標回転量
		g_enemy[nCntEnemy].rotLast.z = D3DX_PI / 2;

		// 移動量
		g_enemy[nCntEnemy].move.y = 5.0f;

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
					g_enemy[nCntEnemy].pos.x,
					g_enemy[nCntEnemy].pos.y,
					g_enemy[nCntEnemy].pos.z),

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
		g_enemy[nCntEnemy].status.nStatus = ENEMYSTATUS_DAMAGE;

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
					g_enemy[nCntEnemy].pos.x,
					g_enemy[nCntEnemy].pos.y,
					g_enemy[nCntEnemy].pos.z),
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
// 敵の当たり判定処理
// ------------------------------------------------------------------------------------------
void ColisionEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size)
{
}

// ------------------------------------------------------------------------------------------
// 敵情報の読み込み処理
// ------------------------------------------------------------------------------------------
void LoadEnemy(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntEnemy = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

							// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		// ファイル開
		pFile = fopen("data/SAVE/TUTORIAL/ENEMY/enemy.txt", "r");
	}

	//// ボス用
	//else if (GetGameStete() == GAMESTATE_BOSS)
	//{
	//	// ファイル開
	//	pFile = fopen("data/SAVE/GAME_BOSS/ENEMY/enemy.txt", "r");
	//}

	// それ以外
	else
	{
		// ファイル開
		pFile = fopen("data/SAVE/ENEMY/enemy.txt", "r");
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

				// 敵セットが来たら
				else if (strcmp(cHeadText, "ENEMYSET") == 0)
				{

					// エンド敵セットが来るまでループ
					while (strcmp(cHeadText, "END_ENEMYSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_enemy[nCntEnemy].pos.x,
								&g_enemy[nCntEnemy].pos.y,
								&g_enemy[nCntEnemy].pos.z);

							// 目的位置に保存
							g_enemy[nCntEnemy].posLast = g_enemy[nCntEnemy].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_enemy[nCntEnemy].rot.x,
								&g_enemy[nCntEnemy].rot.y,
								&g_enemy[nCntEnemy].rot.z);

							// 使用状態
							g_enemy[nCntEnemy].bUse = true;

							// 表示状態
							g_enemy[nCntEnemy].bDisp = true;

							// 影の作成
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


					// 敵の更新
					nCntEnemy++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "敵のデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 敵モーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadEnemyMosion(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int nCntType = 0;							// タイプカウント
	int nCntPartsFile = 0;						// xファイル読み込み用カウント
	int	nCntMotion = 0;							// プレイヤーカウント
	int	nCntModel = 0;							// 敵カウント
	int	nCntKeySet = 0;							// モーションカウント
	int	nCntKey = 0;							// キーカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/SAVE/ENEMY/enemy_motion.txt", "r");

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
			//----------SCRIPT～END_SCRIPTまで----------//
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// 敵数の読み込み
				if (strcmp(cHeadText, "NUM_MODEL") == 0)
				{
					sscanf(cRaedText, "%s %s %d",
						&cDie, &cDie, 
						&g_enemyModelInfo[nCntType].Num_Model);
				}

				// 敵ファイル名の読み込み
				else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
				{
					sscanf(cRaedText, "%s %s %s",
						&cDie, &cDie,
						&g_enemyModelInfo[nCntType].aFileModel[nCntPartsFile]);

					// カウントアップ
					nCntPartsFile++;
				}

					// スクリプトだったら
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					//----------CHARACTERSET～END_CHARACTERSETまで----------//
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 移動量読み込み
						if (strcmp(cHeadText, "MOVE") == 0)
						{
							sscanf(cRaedText, "%s %s %f",
								&cDie, &cDie,
								&g_enemyModelInfo[nCntType].move);
						}

						// じゃんぷ読み込み
						else if (strcmp(cHeadText, "JUMP") == 0)
						{
							sscanf(cRaedText, "%s %s %f",
								&cDie, &cDie,
								&g_enemyModelInfo[nCntType].fJump);
						}

						// 半径読み込み
						else if (strcmp(cHeadText, "RADIUS") == 0)
						{
							sscanf(cRaedText, "%s %s %f",
								&cDie, &cDie,
								&g_enemyModelInfo[nCntType].fRadius);
						}

						// 敵セットが来たら
						else if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							//----------PARTSSET～END_PARTSSETまで----------//
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// 親情報読み込み
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cRaedText, "%s %s %d",
										&cDie, &cDie,
										&g_enemyModel[nCntType][nCntModel].nIdxModelParent);
								}

								// 位置情報読み込み
								else if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f",
										&cDie, &cDie,
										&g_enemyModel[nCntType][nCntModel].pos.x,
										&g_enemyModel[nCntType][nCntModel].pos.y,
										&g_enemyModel[nCntType][nCntModel].pos.z);

									// 位置情報保存
									g_enemyModel[nCntType][nCntModel].posOrigin = 
										g_enemyModel[nCntType][nCntModel].pos;
								}

								// 回転情報読み込み
								else if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&g_enemyModel[nCntType][nCntModel].rot.x,
										&g_enemyModel[nCntType][nCntModel].rot.y,
										&g_enemyModel[nCntType][nCntModel].rot.z);
								}
							}
							//----------PARTSSET～END_PARTSSET終了----------//

							// 敵の更新
							nCntModel++;
						}
					}
					//----------CHARACTERSET～END_CHARACTERSET終了----------//
				}
				
				// モーションセットが来たら
				if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// 敵の初期化
					nCntModel = 0;

					//----------MOTIONSET～END_MOTIONSETまで----------//
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", 
								&cDie, &cDie, 
								&g_enemyKey[nCntType][nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_ENEMYKEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_enemyKey[nCntType][nCntMotion].nNumKey);
						}

						// 当たり判定情報読み込み
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

						// ダメージ情報読み込み
						else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_enemyKey[nCntType][nCntMotion].Collision_Damage);
						}

						// 軌跡情報読み込み
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

							// キャラクタータイプ
							g_enemyKey[nCntType][nCntMotion].Orbit_nType = TRAJECTORYTYPE_GIRAFFE;

							// 使用状態
							g_enemyKey[nCntType][nCntMotion].Orbit_bUse = true;
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
									&cDie, &cDie, &g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].nFram);
							}

							// キーの初期化
							nCntKey = 0;

							//----------KEYSET～END_KEYSETまで----------//
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// キー情報読み込み
								if (strcmp(cHeadText, "KEY") == 0)
								{

									//----------KEY～END_KEYまで----------//
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
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_enemy[0].model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_enemyKey[nCntType][nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}
									//----------KEY～END_KEY終了----------//

									// カウントキー
									nCntKey++;
								}
							}
							//----------KEYSET～END_KEYSET終了----------//

							// カウント敵
							nCntModel++;
						}
					}
					//----------MOTIONSET～END_MOTIONSET終了----------//

					// カウントプレイヤー
					nCntMotion++;
				}
			}
			//----------SCRIPT～END_SCRIPT終了----------//
		}

		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "敵のモーションデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 敵情報処理
// ------------------------------------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_enemy[0];
}


// ------------------------------------------------------------------------------------------
// 敵撃破数処理
// ------------------------------------------------------------------------------------------
int GetEnemyTotal(void)
{
	return g_enemyTotal;
}
