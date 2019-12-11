// ------------------------------------------------------------------------------------------
//
// タイム処理 [time.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "time.h"

#include "game.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_TIME		"data/TEXTURE/TimeNumber.png"	// 読み込むテクスチャファイル名
#define TIME_SIZE (25)								// タイムボードの大きさ
#define TIME_MAX (3)								// 数値

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// グローバル変数宣言
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;				// 頂点バッファへのポインタ
D3DXVECTOR3				g_TimePos;							// タイムボードのポジション
D3DXCOLOR				g_TimeCol;							// タイムカラー
int						g_nTime;							// タイム
int						g_nTimer;							// タイマー
int						g_nCntTimeState;					// ゲームオーバーへのカウント

// ------------------------------------------------------------------------------------------
//
// 初期化処理
//
// ------------------------------------------------------------------------------------------
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_TimePos = D3DXVECTOR3(1220.0f, 50.0f, 0.0f);
	g_TimeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_nTime = 300;
	g_nTimer = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_TIME,		// ファイルの名前
		&g_pTextureTime);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexTime(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// 終了処理
//
// ------------------------------------------------------------------------------------------
void UninitTime(void)
{
	// テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// 更新処理
//
// ------------------------------------------------------------------------------------------
void UpdateTime(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTime;		// スコアカウント
	int nTime;			// 桁スコア
	float fTime;		// テクスチャ設定

	// タイムアップ
	g_nTimer++;

	// 1秒カウント
	if (g_nTimer % 60 == 0)
	{
		g_nTime--;	// スコア加算
	}

	// 赤色
	if (g_nTime <= 100)
	{
		g_TimeCol.g = 0.0f;
	}

	// ゲームオーバー
	if (g_nTime <= 0)
	{
		g_nCntTimeState++;

		if (g_nCntTimeState >= 30)
		{
			SetGameState(GAMESTATE_GAMEOVER);
		}
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		// 桁の数字
		nTime = g_nTime % (int)powf(10.0f, (float)nCntTime + 1.0f) / (int)powf(10.0f, (float)nCntTime);
		fTime = (float)nTime / 10.0f;

		pVtx[0].col = g_TimeCol;	// 一つ目の色設定
		pVtx[1].col = g_TimeCol;	// 二つ目の色設定
		pVtx[2].col = g_TimeCol;	// 三つ目の色設定
		pVtx[3].col = g_TimeCol;	// 四つ目の色設定

		pVtx[0].tex = D3DXVECTOR2(fTime, 0.0f);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(fTime + 0.1f, 0.0f);	// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(fTime, 1.0f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(fTime + 0.1f, 1.0f);	// 四つ目のテクスチャ

		// ポイント合わせ
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTime->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// 描画処理
//
// ------------------------------------------------------------------------------------------
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTime;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTime, 2);
	}
}

// ------------------------------------------------------------------------------------------
//
// 頂点の作成
//
// ------------------------------------------------------------------------------------------
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTime;		// タイムカウント

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTime, NULL);

	// 頂点情報の設定
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_TimePos.x + (-TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime, g_TimePos.y + (-TIME_SIZE / 2), 0.0f);				// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_TimePos.x + (TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime + TIME_SIZE, g_TimePos.y + (-TIME_SIZE / 2), 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(g_TimePos.x + (-TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime, g_TimePos.y + TIME_SIZE / 2, 0.0f);					// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_TimePos.x + (TIME_SIZE / 2) - (TIME_SIZE + TIME_SIZE) * nCntTime + TIME_SIZE, g_TimePos.y + TIME_SIZE / 2, 0.0f);		// 四つ目の頂点

		pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

		pVtx[0].col = g_TimeCol;	// 一つ目の色設定
		pVtx[1].col = g_TimeCol;	// 二つ目の色設定
		pVtx[2].col = g_TimeCol;	// 三つ目の色設定
		pVtx[3].col = g_TimeCol;	// 四つ目の色設定

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffTime->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// タイムの取得
//
// ------------------------------------------------------------------------------------------

int GetTime(void)
{
	return g_nTime;
}
