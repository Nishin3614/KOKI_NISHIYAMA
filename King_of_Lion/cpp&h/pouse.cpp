// ------------------------------------------------------------------------------------------
//
// ポーズ処理の説明[pouse.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "pouse.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define POUSE1 "data/TEXTURE/POUSE001.png"	// ポーズ
#define POUSE_LEFT_X (0)					// 左
#define POUSE_ON_Y (0)						// 上
#define POUSE_SIZE_X (1280)					// サイズ（ｘ）
#define POUSE_SIZE_Y (720)					// サイズ（ｙ)

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePouse = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPouse = NULL;	// テクスチャへのポインタ

// ------------------------------------------------------------------------------------------
// ポーズ初期化処理
// ------------------------------------------------------------------------------------------
void InitPouse(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイス
	VERTEX_2D *pVtx;			// 頂点情報へのポイント

	// テクスチャの読み込み
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, POUSE1, &g_pTexturePouse);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPouse, NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPouse->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点の設定
	pVtx[0].pos = D3DXVECTOR3(POUSE_LEFT_X, POUSE_ON_Y, 0.0f);									// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(POUSE_LEFT_X + POUSE_SIZE_X, POUSE_ON_Y, 0.0f);					// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(POUSE_LEFT_X, POUSE_ON_Y + POUSE_SIZE_Y, 0.0f);					// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(POUSE_LEFT_X + POUSE_SIZE_X, POUSE_ON_Y + POUSE_SIZE_Y, 0.0f);	// 四つ目の頂点

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
	g_pVtxBuffPouse->Unlock();
}

// ------------------------------------------------------------------------------------------
// ポーズ終了処理
// ------------------------------------------------------------------------------------------
void UninitPouse(void)
{
	// テクスチャの開放
	if (g_pTexturePouse != NULL)
	{
		g_pTexturePouse->Release();
		g_pTexturePouse = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPouse != NULL)
	{
		g_pVtxBuffPouse->Release();
		g_pVtxBuffPouse = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// ポーズ更新処理
// ------------------------------------------------------------------------------------------
void UpdatePouse(void)
{
}

// ------------------------------------------------------------------------------------------
// ポーズ描画処理
// ------------------------------------------------------------------------------------------
void DrawPouse(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPouse, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePouse);

	// ポーズ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}