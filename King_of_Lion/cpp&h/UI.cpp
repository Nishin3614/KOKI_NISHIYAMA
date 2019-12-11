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
#define UI_TIME_SIZE_X (50)		// タイムサイズ（ｘ）
#define UI_TIME_SIZE_Y (50)		// タイムサイズ（ｙ）

#define UI_SCORE_SIZE_X (150)	// スコアサイズ（ｘ）
#define UI_SCORE_SIZE_Y (50)	// スコアサイズ（ｙ）

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------アイテムの種類---------- //
typedef enum
{
	UITYPE_PLAYER = 0,
	UITYPE_TIMERFRAME,
	UITYPE_TIMER,
	UITYPE_TOTAL,
	UITYPE_MAX
} UITYPE;

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntUi;

	// 読み込むテクスチャ
	char *paUITexture[UITYPE_MAX] =
	{
		{ "data/TEXTURE/ui/00_rionson.png" },		// プレイヤーアイコン
		{ "data/TEXTURE/ui/00_TimeFrame.png" },		// タイム枠
		{ "data/TEXTURE/ui/00_TimeName.png" },		// タイム名
		{ "data/TEXTURE/ui/scoreUI.png" }			// トータル
	};

	// 初期化
	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		// プレイヤーアイコン
		if (nCntUi == UITYPE_PLAYER)
		{
			g_aUi[nCntUi].fXSize = 30;
			g_aUi[nCntUi].fYSize = 30;
			g_aUi[nCntUi].pos = D3DXVECTOR3(25.0f, 45.0f, 0.0f);

		}

		// タイムの枠
		else if (nCntUi == UITYPE_TIMERFRAME)
		{
			g_aUi[nCntUi].fXSize = 100;
			g_aUi[nCntUi].fYSize = 40;
			g_aUi[nCntUi].pos = D3DXVECTOR3(1180.0f, 35.0f, 0.0f);

		}
		// タイム名
		else if (nCntUi == UITYPE_TIMER)
		{
			g_aUi[nCntUi].fXSize = 40;
			g_aUi[nCntUi].fYSize = 40;
			g_aUi[nCntUi].pos = D3DXVECTOR3(1140.0f, 20.0f, 0.0f);

		}
		// トータル
		else if (nCntUi == UITYPE_TOTAL)
		{
			g_aUi[nCntUi].fXSize = 120;
			g_aUi[nCntUi].fYSize = 50;
			g_aUi[nCntUi].pos = D3DXVECTOR3(1150.0f, 120.0f, 0.0f);

		}

		g_aUi[nCntUi].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUi[nCntUi].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aUi[nCntUi].bUse = true;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			paUITexture[nCntUi],
			&g_pTextureUi[nCntUi]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * UITYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITYPE_MAX; nCntUi++)
	{
		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(
			g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize,
			g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(
			g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize,
			g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(
			g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize,
			g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(
			g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize,
			g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);	// 四つ目の頂点
		
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
// UI終了処理
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
			pDevice->SetTexture(0, g_pTextureUi[nCntUi]);

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
void SetUi(D3DXVECTOR3 pos, D3DXVECTOR3 move ,float fXSize,float fYSize)
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
			g_aUi[nCntUi].pos = pos;
			g_aUi[nCntUi].move = move;
			g_aUi[nCntUi].bUse = true;
			g_aUi[nCntUi].fXSize = fXSize;
			g_aUi[nCntUi].fYSize = fYSize;

			// データのポインタを合わせる
			pVtx += nCntUi * 4;

			pVtx[0].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);											// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y - g_aUi[nCntUi].fYSize, 0.0f);						// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x - g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);						// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_aUi[nCntUi].pos.x + g_aUi[nCntUi].fXSize, g_aUi[nCntUi].pos.y + g_aUi[nCntUi].fYSize, 0.0f);	// 四つ目の頂点

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