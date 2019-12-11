// ------------------------------------------------------------------------------------------
//
// 順位処理の説明[rank5.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "rank.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "titlecomment.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

#define NO_1 "data/TEXTURE/ProvisionalTexture/1位.png"	// 1位
#define NO_2 "data/TEXTURE/ProvisionalTexture/2位.png"	// 2位
#define NO_3 "data/TEXTURE/ProvisionalTexture/3位.png"	// 3位
#define NO_4 "data/TEXTURE/ProvisionalTexture/4位.png"	// 4位
#define NO_5 "data/TEXTURE/ProvisionalTexture/5位.png"	// 5位

#define NO_MAX (5)					// 画像数

#define RANK5_LEFT_X (400)			// 左
#define RANK5_ON_Y (100)			// 上

#define RANK5_SIZE_X (100)			// サイズ（ｘ）
#define RANK5_SIZE_Y (100)			// サイズ（ｙ）


// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureRank5[NO_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank5 = NULL;		// テクスチャへのポインタ
float g_fRank5Y;									// yのポジション

// ------------------------------------------------------------------------------------------
//
// ランキング背景初期化処理
//
// ------------------------------------------------------------------------------------------

void InitRank5(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイス
	VERTEX_2D *pVtx;			// 頂点情報へのポイント
	int nCntNo;					// ナンバーカウント

	// 初期化
	g_fRank5Y = 90.0f;		// yのポジション

	// テクスチャの読み込み
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, NO_1, &g_pTextureRank5[0]);
	D3DXCreateTextureFromFile(pDevice, NO_2, &g_pTextureRank5[1]);
	D3DXCreateTextureFromFile(pDevice, NO_3, &g_pTextureRank5[2]);
	D3DXCreateTextureFromFile(pDevice, NO_4, &g_pTextureRank5[3]);
	D3DXCreateTextureFromFile(pDevice, NO_5, &g_pTextureRank5[4]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NO_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRank5, NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRank5->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntNo = 0; nCntNo < NO_MAX; nCntNo++)
	{
		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(RANK5_LEFT_X, RANK5_ON_Y + g_fRank5Y * nCntNo, 0.0f);									// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(RANK5_LEFT_X + RANK5_SIZE_X, RANK5_ON_Y + g_fRank5Y* nCntNo, 0.0f);					// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(RANK5_LEFT_X, RANK5_ON_Y + RANK5_SIZE_Y + g_fRank5Y* nCntNo, 0.0f);					// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(RANK5_LEFT_X + RANK5_SIZE_X, RANK5_ON_Y + RANK5_SIZE_Y + g_fRank5Y* nCntNo, 0.0f);	// 四つ目の頂点

		// 頂点作成
		pVtx[0].rhw = 1.0f;																// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;																// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;																// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;																// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// 一つ目の色設定
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// 二つ目の色設定
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// 三つ目の色設定
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);								// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);								// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);								// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);								// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);								// 四つ目のテクスチャ

		pVtx += 4;	// ポイント合わせ
	}
	// アンロック
	g_pVtxBuffRank5->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// ランキング背景終了処理
//
// ------------------------------------------------------------------------------------------

void UninitRank5(void)
{	
	for (int nCntNo = 0; nCntNo < NO_MAX; nCntNo++)
	{
		// テクスチャの開放
		if (g_pTextureRank5[nCntNo] != NULL)
		{
			g_pTextureRank5[nCntNo]->Release();
			g_pTextureRank5[nCntNo] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffRank5 != NULL)
	{
		g_pVtxBuffRank5->Release();
		g_pVtxBuffRank5 = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// ランキング背景更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateRank5(void)
{

}

// ------------------------------------------------------------------------------------------
//
// ランキング背景描画処理
//
// ------------------------------------------------------------------------------------------
void DrawRank5(void)
{
	// 変数宣言
	int nCntNo;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffRank5, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntNo = 0; nCntNo < NO_MAX; nCntNo++)
	{
		pDevice->SetTexture(0, g_pTextureRank5[nCntNo]);

		// ランキング背景描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntNo, 2);
	}
}