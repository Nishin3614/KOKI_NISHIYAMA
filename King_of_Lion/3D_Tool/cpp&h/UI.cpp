// ------------------------------------------------------------------------------------------
//
// UI処理の説明[ui.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "UI.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

#define TIMERWAKU "data/TEXTURE/timerwaku.png"		// タイムの枠
#define TIMENAME "data/TEXTURE/タイム名.png"		// タイム名
#define FRAME_SCORE "data/TEXTURE/Frame_Score2.png"	// スコア枠
#define SCORECAT1 "data/TEXTURE/スコア右.png"		// スコア右
#define SCORECAT2 "data/TEXTURE/スコア横.png"		// スコア左
#define HPNAME "data/TEXTURE/HP名.png"				// HP名

#define UI_TIME_SIZE_X (80)		// タイムサイズ（ｘ）
#define UI_TIME_SIZE_Y (50)		// タイムサイズ（ｙ）

#define UI_SCORE_SIZE_X (150)	// スコアサイズ（ｘ）
#define UI_SCORE_SIZE_Y (50)	// スコアサイズ（ｙ）

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureUi[UITYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;	// テクスチャへのポインタ
UI g_aUi[UITYPE_MAX];								// UI

// ------------------------------------------------------------------------------------------
//
// タイトル初期化処理
//
// ------------------------------------------------------------------------------------------

void InitUi(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntUi;

	// 初期化
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		g_aUi[nCntUi].Type = UITYPE_TIMEFRAME;
		g_aUi[nCntUi].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aUi[nCntUi].fXSize = 0;
		g_aUi[nCntUi].bUse = false;
	}

	// テクスチャの読み込み
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, TIMERWAKU, &g_pTextureUi[0]);
	D3DXCreateTextureFromFile(pDevice, TIMENAME, &g_pTextureUi[1]);
	D3DXCreateTextureFromFile(pDevice, FRAME_SCORE, &g_pTextureUi[2]);
	D3DXCreateTextureFromFile(pDevice, SCORECAT1, &g_pTextureUi[3]);
	D3DXCreateTextureFromFile(pDevice, SCORECAT2, &g_pTextureUi[4]);
	D3DXCreateTextureFromFile(pDevice, HPNAME, &g_pTextureUi[5]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffUi, NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y - UI_TIME_SIZE_Y, 0.0f);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y - UI_TIME_SIZE_Y, 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y + UI_TIME_SIZE_Y, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + UI_TIME_SIZE_X, g_aUi[nCntUi].pos.y + UI_TIME_SIZE_Y, 0.0f);	// 四つ目の頂点
		
		// 頂点作成
		pVtx[0].rhw = 1.0f;									// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;									// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;									// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;									// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = g_aUi[nCntUi].col;	// 一つ目の色設定
		pVtx[1].col = g_aUi[nCntUi].col;	// 二つ目の色設定
		pVtx[2].col = g_aUi[nCntUi].col;	// 三つ目の色設定
		pVtx[3].col = g_aUi[nCntUi].col;	// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// 四つ目のテクスチャ

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffUi->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// タイトル終了処理
//
// ------------------------------------------------------------------------------------------

void UninitUi(void)
{
	int nCntUi;

	// テクスチャの開放
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		if (g_pTextureUi[nCntUi] != NULL)
		{
			g_pTextureUi[nCntUi]->Release();
			g_pTextureUi[nCntUi] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateUi(void)
{
	//ENEMY *pEnemy;		// エネミー情報

	////エネミー取得
	//pEnemy = GetEnemy();

	//// 変数宣言
	//VERTEX_2D *pVtx;	// 頂点情報へのポイント
	//int nCntUi;

	//// 頂点データの範囲をロックし、頂点バッファへのポインタ
	//g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	//for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	//{
	//	if (g_aUi[nCntUi].bUse == true)
	//	{
	//		if (g_aUi[nCntUi].Type == UITYPE_TIMEFRAME &&
	//			g_aUi[nCntUi].Type == UITYPE_TIMENAME)
	//		{
	//			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	//			{
	//				if (pEnemy->bDisp == true)
	//				{
	//					// 敵との範囲
	//					if (pEnemy->pos.x + pEnemy->fSize / 2 - GetCamera()->difference.x > g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize&&
	//						pEnemy->pos.x - pEnemy->fSize / 2 - GetCamera()->difference.x < g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize &&
	//						pEnemy->pos.y > g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize &&
	//						pEnemy->pos.y - pEnemy->fSize < g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize)
	//					{
	//						// 半透明に
	//						g_aUi[nCntUi].col.a = 0.5f;
	//					}
	//				}

	//				// 通常表示へ
	//				else
	//				{
	//					g_aUi[nCntUi].col.a = 1.0f;
	//				}
	//			}
	//		}

	//		// カラー設定
	//		pVtx[0].col = g_aUi[nCntUi].col;	// 一つ目の色設定
	//		pVtx[1].col = g_aUi[nCntUi].col;	// 二つ目の色設定
	//		pVtx[2].col = g_aUi[nCntUi].col;	// 三つ目の色設定
	//		pVtx[3].col = g_aUi[nCntUi].col;	// 四つ目の色設定
	//	}

	//	// ポイント合わせ
	//	pVtx += 4;
	//}
}

// ------------------------------------------------------------------------------------------
//
// タイトル描画処理
//
// ------------------------------------------------------------------------------------------
void DrawUi(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffUi, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// テクスチャの設定
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		if (g_aUi[nCntUi].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureUi[g_aUi[nCntUi].Type]);

			// タイトル描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル設定処理
//
// ------------------------------------------------------------------------------------------
void SetUi(D3DXVECTOR3 pos, D3DXVECTOR3 move,UITYPE Type,D3DXCOLOR col ,float fXSize,float fYSize)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntUi;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		// 使用していない時
		if (g_aUi[nCntUi].bUse == false)
		{
			g_aUi[nCntUi].Type = Type;
			g_aUi[nCntUi].pos = pos;
			g_aUi[nCntUi].move = move;
			g_aUi[nCntUi].col = col;
			g_aUi[nCntUi].bUse = true;
			g_aUi[nCntUi].fXSize = fXSize;
			g_aUi[nCntUi].fYSize = fYSize;

			// データのポインタを合わせる
			pVtx += nCntUi * 4;

			pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);											// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);						// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);						// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);	// 四つ目の頂点

			// カラー設定
			pVtx[0].col = g_aUi[nCntUi].col;	// 一つ目の色設定
			pVtx[1].col = g_aUi[nCntUi].col;	// 二つ目の色設定
			pVtx[2].col = g_aUi[nCntUi].col;	// 三つ目の色設定
			pVtx[3].col = g_aUi[nCntUi].col;	// 四つ目の色設定

			// for分を抜ける
			break;
		}
	}
	// アンロック
	g_pVtxBuffUi->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// タイトル取得処理
//
// ------------------------------------------------------------------------------------------

UI *GetUi(void)
{
	return &g_aUi[0];
}