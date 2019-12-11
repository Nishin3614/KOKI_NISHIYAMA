// ------------------------------------------------------------------------------------------
//
// 選択処理の説明[select.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "select.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define SELECT1 "data/TEXTURE/SELECT001.png"	// 選択
#define SELECT_LEFT_X (0)					// 左
#define SELECT_ON_Y (0)						// 上
#define SELECT_SIZE_X (1280)					// サイズ（ｘ）
#define SELECT_SIZE_Y (720)					// サイズ（ｙ)

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureSelect = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	// テクスチャへのポインタ

// ------------------------------------------------------------------------------------------
// 選択初期化処理
// ------------------------------------------------------------------------------------------
void InitSelect(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイス
	VERTEX_2D *pVtx;			// 頂点情報へのポイント

	// テクスチャの読み込み
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, SELECT1, &g_pTextureSelect);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelect, NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffSelect->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点の設定
	pVtx[0].pos = D3DXVECTOR3(SELECT_LEFT_X, SELECT_ON_Y, 0.0f);									// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(SELECT_LEFT_X + SELECT_SIZE_X, SELECT_ON_Y, 0.0f);					// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(SELECT_LEFT_X, SELECT_ON_Y + SELECT_SIZE_Y, 0.0f);					// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(SELECT_LEFT_X + SELECT_SIZE_X, SELECT_ON_Y + SELECT_SIZE_Y, 0.0f);	// 四つ目の頂点

	// 頂点作成
	pVtx[0].rhw = 1.0f;																// 一つ目の頂点作成
	pVtx[1].rhw = 1.0f;																// 二つ目の頂点作成
	pVtx[2].rhw = 1.0f;																// 三つ目の頂点作成
	pVtx[3].rhw = 1.0f;																// 四つ目の頂点作成

	// カラー設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// 一つ目の色設定
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// 二つ目の色設定
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// 三つ目の色設定
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);								// 四つ目の色設定

	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);								// 一つ目のテクスチャ
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);								// 二つ目のテクスチャ
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);								// 三つ目のテクスチャ
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);								// 四つ目のテクスチャ

	// アンロック
	g_pVtxBuffSelect->Unlock();
}

// ------------------------------------------------------------------------------------------
// 選択終了処理
// ------------------------------------------------------------------------------------------
void UninitSelect(void)
{
	// テクスチャの開放
	if (g_pTextureSelect != NULL)
	{
		g_pTextureSelect->Release();
		g_pTextureSelect = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 選択更新処理
// ------------------------------------------------------------------------------------------
void UpdateSelect(void)
{
}

// ------------------------------------------------------------------------------------------
// 選択描画処理
// ------------------------------------------------------------------------------------------
void DrawSelect(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSelect);

	// 選択描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}