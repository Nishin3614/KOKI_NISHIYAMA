// ------------------------------------------------------------------------------------------
//
// 矢印処理の説明[arrow.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "arrow.h"
#include "pousecoment.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define ARROWTEX "data/TEXTURE/99_arrow.png"	// 背景
#define POUSENAME_SIZE_X (100)					// タイトル名サイズ（ｘ）
#define POUSENAME_SIZE_Y (100)					// タイトル名サイズ（ｙ）

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureArrow = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffArrow = NULL;	// テクスチャへのポインタ
ARROW g_aArrow;									// 矢印

// ------------------------------------------------------------------------------------------
// タイトル初期化処理
// ------------------------------------------------------------------------------------------
void InitArrow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	// 頂点情報へのポイント

	// 初期化
	g_aArrow.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aArrow.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aArrow.bUse = false;

	// テクスチャの読み込み
	pDevice = GetDevice();
	D3DXCreateTextureFromFile(pDevice, ARROWTEX, &g_pTextureArrow);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffArrow, NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffArrow->Lock(0, 0, (void **)&pVtx, 0);
	// 頂点の設定
	pVtx[0].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 四つ目の頂点
	// 頂点作成
	pVtx[0].rhw = 1.0f;									// 一つ目の頂点作成
	pVtx[1].rhw = 1.0f;									// 二つ目の頂点作成
	pVtx[2].rhw = 1.0f;									// 三つ目の頂点作成
	pVtx[3].rhw = 1.0f;									// 四つ目の頂点作成
	// カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 一つ目の色設定
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 二つ目の色設定
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 三つ目の色設定
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 四つ目の色設定
	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// 一つ目のテクスチャ
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// 二つ目のテクスチャ
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// 三つ目のテクスチャ
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// 四つ目のテクスチャ
	// アンロック
	g_pVtxBuffArrow->Unlock();
}

// ------------------------------------------------------------------------------------------
// タイトル終了処理
// ------------------------------------------------------------------------------------------

void UninitArrow(void)
{
	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// タイトル更新処理
// ------------------------------------------------------------------------------------------
void UpdateArrow(void)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポイント

	// 徐々に移動
	g_aArrow.pos += (g_aArrow.posLast - g_aArrow.pos) * 0.5f;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffArrow->Lock(0, 0, (void **)&pVtx, 0);
	// 頂点の設定
	pVtx[0].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 四つ目の頂点
	// アンロック
	g_pVtxBuffArrow->Unlock();

}

// ------------------------------------------------------------------------------------------
// タイトル描画処理
// ------------------------------------------------------------------------------------------
void DrawArrow(void)
{
	// 使用状態の場合
	if (g_aArrow.bUse == true)
	{
		// 変数宣言
		LPDIRECT3DDEVICE9 pDevice;
		// デバイスを取得する
		pDevice = GetDevice();
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffArrow, 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャー設定
		pDevice->SetTexture(0, g_pTextureArrow);
		// タイトル描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

// ------------------------------------------------------------------------------------------
// 矢印の位置処理
// ------------------------------------------------------------------------------------------
void ArrowPosition(D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポイント

	// 非表示状態から表示状態になる前
	if (g_aArrow.bUse == false)
	{
		g_aArrow.pos = pos;
	}

	// 位置情報取得
	g_aArrow.posLast = pos;
	g_aArrow.posLast.y -= 100.0f;

	// 使用状態へ
	g_aArrow.bUse = true;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffArrow->Lock(0, 0, (void **)&pVtx, 0);
	// 頂点の設定
	pVtx[0].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y - POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(g_aArrow.pos.x - POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(g_aArrow.pos.x + POUSENAME_SIZE_X * 0.5f, g_aArrow.pos.y + POUSENAME_SIZE_Y * 0.5f, 0.0f);	// 四つ目の頂点
	// アンロック
	g_pVtxBuffArrow->Unlock();
}

// ------------------------------------------------------------------------------------------
// 矢印を未使用状態処理
// ------------------------------------------------------------------------------------------
void DeleteArrow(void)
{
	g_aArrow.bUse = false;
}

// ------------------------------------------------------------------------------------------
// タイトル取得処理
// ------------------------------------------------------------------------------------------
ARROW *GetArrow(void)
{
	return &g_aArrow;
}