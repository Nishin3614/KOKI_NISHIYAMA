// ------------------------------------------------------------------------------------------
//
// 爆発処理 [range.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "range.h"

#include "player.h"

#include "game.h"

#include "fade.h"

#include "Calculation.h"

#include "effect.h"

#include "gamefade.h"

#include "tutorial.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define RANGE_MAX (128)								// 爆発の最大数 
#define RANGE_SIZE (50)									// サイズ
#define RANGE_PHONT "data/TEXTURE/range000.png"	// 爆発の写真
#define ANIMATION_EXP 1/8

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------モーションセット---------- //
typedef enum
{
	RANGETYPE_00_TUTORIAL = 0,
	RANGETYPE_01_BOSS,
	RANGETYPE_MAX
} RANGETYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexRange(LPDIRECT3DDEVICE9 pDevice);
void ColisionRange(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRange = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureRange = NULL;	// 頂点テクスチャーのポインタ
RANGE					g_aRange[RANGE_MAX];		// 爆発情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitRange(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntRange;								// カウント爆発

												// 初期設定
	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		g_aRange[nCntRange].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRange[nCntRange].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRange[nCntRange].size = D3DXVECTOR3(100.0f, 3.0f, 0.0f);
		g_aRange[nCntRange].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRange[nCntRange].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);
		g_aRange[nCntRange].nType = RANGETYPE_00_TUTORIAL;
		g_aRange[nCntRange].bUse = false;
		g_aRange[nCntRange].bRange = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RANGE_PHONT, &g_pTextureRange);

	// 頂点情報の作成
	MakeVertexRange(pDevice);

	// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		SetRange(
			D3DXVECTOR3(0.0f, 40.0f, -200.0f),
			D3DXVECTOR3(0.0f,0.0f,0.0f),
			RANGETYPE_00_TUTORIAL);
	}

	// ゲーム用
	else if(GetGameStete() != GAMESTATE_BOSS)
	{
		SetRange(D3DXVECTOR3(-3200.0f, 40.0f, -3400.0f),
			D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),
			RANGETYPE_01_BOSS);
	}
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitRange(void)
{
	// テクスチャの開放
	if (g_pTextureRange != NULL)
	{
		g_pTextureRange->Release();
		g_pTextureRange = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRange != NULL)
	{
		g_pVtxBuffRange->Release();
		g_pVtxBuffRange = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateRange(void)
{
	ColisionRange();
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawRange(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	RANGE *pRange = &g_aRange[0];	// 爆発情報
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 回転・位置の計算用変数
	int nCntRange;							// カウント爆発

	// レンダーステートパラメータの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 両面をカリングする

	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++, pRange++)
	{
		if (pRange->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pRange->mtxWorldRange);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pRange->rot.y, pRange->rot.x, pRange->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pRange->mtxWorldRange,	// 1
				&pRange->mtxWorldRange,					// 2
				&mtxRot);									// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pRange->pos.x, pRange->pos.y, pRange->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pRange->mtxWorldRange,	// 1
				&pRange->mtxWorldRange,					// 2
				&mtxTrans);									// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pRange->mtxWorldRange);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffRange, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRange);

			// 爆発の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRange * 4, 2);
		}
	}
	// レンダーステートパラメータの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
}

// ------------------------------------------------------------------------------------------
// 爆発の設定
// ------------------------------------------------------------------------------------------
void SetRange(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int			nType)
{
	// 変数宣言
	int nCntRange;		// カウントバレット

						// ブロックループ
	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// 使用していない状態
		if (!g_aRange[nCntRange].bUse)
		{
			// 情報更新
			g_aRange[nCntRange].pos = pos;
			g_aRange[nCntRange].rot = rot;
			g_aRange[nCntRange].nType = nType;
			g_aRange[nCntRange].bUse = true;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// プレイヤーを押し出す
// ------------------------------------------------------------------------------------------
void PlayerPos_Range(void)
{
	// プレイヤー情報取得
	PLAYER *pPlayer = GetPlayer();

	// カウント範囲
	for (int nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// 範囲に入っていた
		if (g_aRange[nCntRange].bRange == true)
		{
			// 位置情報更新
			pPlayer->pos = g_aRange[nCntRange].posOld;
			
			// 移動量初期化
			pPlayer->move.x = 0.0f;
			pPlayer->move.z = 0.0f;
			g_aRange[nCntRange].bRange = false;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexRange(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntRange;		// カウント爆発

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * RANGE_MAX, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRange, NULL);
	
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRange->Lock(0, 0, (void **)&pVtx, 0);

	// 初期設定
	for (nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// 頂点座標の設定					
		pVtx[0].pos = D3DXVECTOR3(-g_aRange[nCntRange].size.x, g_aRange[nCntRange].size.y, 0.0f);		// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_aRange[nCntRange].size.x, g_aRange[nCntRange].size.y, 0.0f);		// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(-g_aRange[nCntRange].size.x, -g_aRange[nCntRange].size.y, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_aRange[nCntRange].size.x, -g_aRange[nCntRange].size.y, 0.0f);		// 四つ目の頂点

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 四つ目の頂点

		// カラー
		pVtx[0].col = g_aRange[nCntRange].col;	// 一つ目の色設定
		pVtx[1].col = g_aRange[nCntRange].col;	// 二つ目の色設定
		pVtx[2].col = g_aRange[nCntRange].col;	// 三つ目の色設定
		pVtx[3].col = g_aRange[nCntRange].col;	// 四つ目の色設定

														// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);	// 四つ目の色設定

		// ポイント合わせ
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffRange->Unlock();
}

// ------------------------------------------------------------------------------------------
// 範囲の当たり判定情報
// ------------------------------------------------------------------------------------------
void ColisionRange(void)
{
	// ベクトルの当たり判定
	D3DXVECTOR3 RangeLine;	// 前回のプレイヤーのブロックの始点からのベクトル
	D3DXVECTOR3 PlayerLine;	// プレイヤーのブロックの始点からのベクトル
	D3DXVECTOR3 PlayerOldLine;	// 前回のプレイヤーのブロックの始点からのベクトル
	D3DXVECTOR3 RangeBegin;
	D3DXVECTOR3 RangeLast;

	// プレイヤー情報取得
	PLAYER		*pPlayer = GetPlayer();

	// ブロックループ
	for (int nCntRange = 0; nCntRange < RANGE_MAX; nCntRange++)
	{
		// 使用状態
		if (g_aRange[nCntRange].bUse == true)
		{
			RangeBegin =
				g_aRange[nCntRange].pos +
				D3DXVECTOR3(
					g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
					0.0f,
					g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y));

			RangeLast =
				g_aRange[nCntRange].pos +
				D3DXVECTOR3(
					-g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
					0.0f,
					-g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y));

			// プレイヤーの線
			PlayerLine =
				Difference_Between(
					pPlayer->pos,
					g_aRange[nCntRange].pos + D3DXVECTOR3(
						g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
						0.0f,
						g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y)));


			// 前回プレイヤーの線
			PlayerOldLine =
				Difference_Between(
					pPlayer->posold,
					g_aRange[nCntRange].pos + D3DXVECTOR3(
						g_aRange[nCntRange].size.x * sinf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y),
						0.0f,
						g_aRange[nCntRange].size.x * cosf(D3DX_PI / 2 - g_aRange[nCntRange].rot.y)));

			// 範囲の線
			RangeLine =
				Difference_Between(
					RangeLast,
					RangeBegin);

			float fCross;
			float fCrossOld;

			// X・Zの角度
			float fAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// Yの角度
			float fYAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			fCross = Cross_product(
				PlayerLine,
				RangeLine);

			fCrossOld = Cross_product(
				PlayerOldLine,
				RangeLine);

			// ゲーム
			if (GetMode() == MODE_GAME)
			{
				// 外積(現在-)
				if (fCross <= 0 &&
					fCrossOld > 0)		// 前回の外積が+
				{
					SetGameState(GAMESTATE_SELECT);
					g_aRange[nCntRange].bRange = true;
					g_aRange[nCntRange].posOld = pPlayer->posold;
				}
			}

			// チュートリアル
			else
			{
				// 外積(現在-)
				if (fCross >= 0 &&
					fCrossOld < 0)		// 前回の外積が+
				{
					SetTutorialState(TUTORIALSSTATE_SELECT);
					g_aRange[nCntRange].bRange = true;
					g_aRange[nCntRange].posOld = pPlayer->posold;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 爆発の情報
// ------------------------------------------------------------------------------------------
RANGE *GetRange(void)
{
	return &g_aRange[0];
}