// ------------------------------------------------------------------------------------------
//
// STATISTICSUI処理の説明[statisticsUI.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "statisticsUI.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------アイテムの種類---------- //
typedef enum
{
	STATISTICSUITYPE_BG = 0,	// 背景
	STATISTICSUITYPE_ALL,		// 統計用のUI
	STATISTICSUITYPE_MAX
} STATISTICSUITYPE;

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureStatisticsUI[STATISTICSUITYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStatisticsUI = NULL;					// テクスチャへのポインタ

// 読み込むテクスチャ
char *g_paSTATISTICSUITexture[STATISTICSUITYPE_MAX] =
{
	{"data/TEXTURE/STATISTICS/NULL" },			// 背景
	{"data/TEXTURE/STATISTICS/スコア統計.png"}	// 統計用のUI
};

// ------------------------------------------------------------------------------------------
//
// タイトル初期化処理
//
// ------------------------------------------------------------------------------------------

void InitStatisticsUI(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntStatisticsUI;

	// 初期化
	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			g_paSTATISTICSUITexture[nCntStatisticsUI],
			&g_pTextureStatisticsUI[nCntStatisticsUI]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * STATISTICSUITYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStatisticsUI,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffStatisticsUI->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		if (nCntStatisticsUI == STATISTICSUITYPE_BG)
		{
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(
				0.0f,
				0.0f,
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(
				SCREEN_WIDTH,
				0.0f,
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(
				0.0f,
				SCREEN_HEIGHT,
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				0.0f);
		}

		else
		{
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(
				0.0f,
				0.0f,
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(
				1100,
				0.0f,
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(
				0.0f,
				SCREEN_HEIGHT,
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(
				1100,
				SCREEN_HEIGHT,
				0.0f);

		}

		// 頂点作成
		pVtx[0].rhw = 1.0f;									
		pVtx[1].rhw = 1.0f;									
		pVtx[2].rhw = 1.0f;									
		pVtx[3].rhw = 1.0f;									

		// 背景
		if (nCntStatisticsUI == STATISTICSUITYPE_BG)
		{
			// カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		// 統計用のUI
		else
		{
			// カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffStatisticsUI->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// STATISTICSUI終了処理
//
// ------------------------------------------------------------------------------------------

void UninitStatisticsUI(void)
{
	int nCntStatisticsUI;

	// テクスチャの開放
	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		if (g_pTextureStatisticsUI[nCntStatisticsUI] != NULL)
		{
			g_pTextureStatisticsUI[nCntStatisticsUI]->Release();
			g_pTextureStatisticsUI[nCntStatisticsUI] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffStatisticsUI != NULL)
	{
		g_pVtxBuffStatisticsUI->Release();
		g_pVtxBuffStatisticsUI = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateStatisticsUI(void)
{
}

// ------------------------------------------------------------------------------------------
//
// タイトル描画処理
//
// ------------------------------------------------------------------------------------------
void DrawStatisticsUI(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStatisticsUI;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffStatisticsUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	for (nCntStatisticsUI = 0; nCntStatisticsUI < STATISTICSUITYPE_MAX; nCntStatisticsUI++)
	{
		pDevice->SetTexture(0, g_pTextureStatisticsUI[nCntStatisticsUI]);

		// タイトル描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStatisticsUI * 4, 2);
	}
}