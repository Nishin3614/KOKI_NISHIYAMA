// ------------------------------------------------------------------------------------------
// 統計処理 [Statipoint.cpp]
// Author : Koki Nishiyama
// ------------------------------------------------------------------------------------------
#include "statipoint.h"			// 統計ヘッダー
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "giraffe.h"
#include "buffalo.h"
#include "king_rion.h"
#include "time.h"
#include "ranking.h"
#include "score.h"
#include "Load_Save.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define	TEXTURE_STATIPOINT		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define STATIPOINT_SIZE (25)										// 統計ボードの大きさ
#define STATIPOINT_BETWEEN (25)										// 間の間隔
#define POINT_MAX (6)												// 数値
#define TIME_BORNUS (10)											// タイムボーナス
#define ENEMY_BORNUS (200)											// 撃破ボーナス

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexStatipoint(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数宣言
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureStatipoint = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStatipoint = NULL;			// 頂点バッファへのポインタ
STATIPOINT				g_aStatipoint[STATIPOINTTYPE_MAX];		// 統計変数
STATIPOINTTYPE			g_StatiPointType = STATIPOINTTYPE_TIME;
int						g_nStatipointScore;
int						g_nCntStatipoint;
float					g_fColStatipoint;

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitStatipoint(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	int nCntStatipoint;							// カウント統計
	int nTime			= GetTime();			// タイム
	int nScore			= GetScore();			// 討伐数

	// スコアゲット
	g_nStatipointScore = 0;
	g_StatiPointType = STATIPOINTTYPE_TIME;

	// 統計開始
	for (nCntStatipoint = 0; nCntStatipoint < STATIPOINTTYPE_MAX; nCntStatipoint++)
	{
		switch (nCntStatipoint)
		{
		case STATIPOINTTYPE_TIME:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(300.0, 150.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint = nTime;
			break;
		case STATIPOINTTYPE_TIMEBORNUS:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(1200.0, 150.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint =
				g_aStatipoint[STATIPOINTTYPE_TIME].nPoint * TIME_BORNUS;
			break;
		case STATIPOINTTYPE_ENEMY:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(300.0, 400.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint = nScore;
			break;
		case STATIPOINTTYPE_ENEMYBORNUS:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(1200.0, 400.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint = 
				g_aStatipoint[STATIPOINTTYPE_ENEMY].nPoint * ENEMY_BORNUS;
			break;
		case STATIPOINTTYPE_SUM:
			g_aStatipoint[nCntStatipoint].pos = D3DXVECTOR3(1200.0, 650.0f, 0.0f);
			g_aStatipoint[nCntStatipoint].nPoint =
				g_aStatipoint[STATIPOINTTYPE_TIMEBORNUS].nPoint + 
				g_aStatipoint[STATIPOINTTYPE_ENEMYBORNUS].nPoint;
			break;
		default:
			break;
		}
		g_aStatipoint[nCntStatipoint].nId = nCntStatipoint;
	}
	
	SaveData(g_aStatipoint[STATIPOINTTYPE_SUM].nPoint);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_STATIPOINT,				// ファイルの名前
		&g_pTextureStatipoint);			// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexStatipoint(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitStatipoint(void)
{
	// テクスチャの開放
	if (g_pTextureStatipoint != NULL)
	{
		g_pTextureStatipoint->Release();
		g_pTextureStatipoint = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffStatipoint != NULL)
	{
		// 頂点バッファの開放
		g_pVtxBuffStatipoint->Release();
		g_pVtxBuffStatipoint = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateStatipoint(void)
{
	int nCntCol;

	// 頂点バッファへのポインタ
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffStatipoint->Lock(0, 0, (void **)&pVtx, 0);
	g_nCntStatipoint++;

	if (g_nCntStatipoint % 14 == 0)
	{
		g_fColStatipoint = 0.0f;
	}

	else if (g_nCntStatipoint % 7 == 0)
	{
		g_fColStatipoint = 1.0f;
	}

	// ポイント合わせ
	pVtx += STATIPOINTTYPE_SUM * POINT_MAX * 4;

	for (nCntCol = 0; nCntCol < POINT_MAX; nCntCol++)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// 一つ目の色設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// 二つ目の色設定
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// 三つ目の色設定
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColStatipoint);	// 四つ目の色設定

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffStatipoint->Unlock();
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawStatipoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStatipoint;

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffStatipoint, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureStatipoint);

	for (nCntStatipoint = 0; nCntStatipoint < POINT_MAX * STATIPOINTTYPE_MAX; nCntStatipoint++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntStatipoint, 2);
	}
}

// ------------------------------------------------------------------------------------------
// 頂点の作成
// ------------------------------------------------------------------------------------------
void MakeVertexStatipoint(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntStatipoint;		// 統計カウント
	int nCntStatipointDate;		// 統計カウント
	int nStatipoint;			// 桁統計
	float fStatipoint;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POINT_MAX * STATIPOINTTYPE_MAX, 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStatipoint,
		NULL);

	// 頂点情報の設定
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffStatipoint->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntStatipointDate = 0; nCntStatipointDate < STATIPOINTTYPE_MAX; nCntStatipointDate++)
	{
		for (nCntStatipoint = 0; nCntStatipoint < POINT_MAX; nCntStatipoint++)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (-STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint,
				g_aStatipoint[nCntStatipointDate].pos.y + (-STATIPOINT_SIZE / 2), 0.0f);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint + STATIPOINT_SIZE,
				g_aStatipoint[nCntStatipointDate].pos.y + (-STATIPOINT_SIZE / 2) , 0.0f);		// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (-STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint,
				g_aStatipoint[nCntStatipointDate].pos.y + (STATIPOINT_SIZE / 2), 0.0f);		// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_aStatipoint[nCntStatipointDate].pos.x + (STATIPOINT_SIZE / 2) - (STATIPOINT_SIZE + 30) * nCntStatipoint + STATIPOINT_SIZE,
				g_aStatipoint[nCntStatipointDate].pos.y + (STATIPOINT_SIZE / 2), 0.0f);		// 四つ目の頂点

			pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
			pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
			pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
			pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 一つ目の色設定
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 二つ目の色設定
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 三つ目の色設定
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定

			nStatipoint = g_aStatipoint[nCntStatipointDate].nPoint % (int)powf(10.0f, (float)nCntStatipoint + 1.0f) / (int)powf(10.0f, (float)nCntStatipoint);
			fStatipoint = (float)nStatipoint / 10.0f;

			pVtx[0].tex = D3DXVECTOR2(fStatipoint, 0.0f);		// 一つ目のテクスチャ
			pVtx[1].tex = D3DXVECTOR2(fStatipoint + 0.1f, 0.0f);		// 二つ目のテクスチャ
			pVtx[2].tex = D3DXVECTOR2(fStatipoint, 1.0f);		// 三つ目のテクスチャ
			pVtx[3].tex = D3DXVECTOR2(fStatipoint + 0.1f, 1.0f);		// 四つ目のテクスチャ

			pVtx += 4;
		}
	}
	// アンロック
	g_pVtxBuffStatipoint->Unlock();
}

// ------------------------------------------------------------------------------------------
// 統計の変更
// ------------------------------------------------------------------------------------------
void AddStatipoint(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntPoint;		// 数値カウント
	int nCntPointDate;	// 統計カウント
	int nStatipoint;			// 桁統計
	float fStatipoint;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffStatipoint->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPointDate = 0; nCntPointDate < STATIPOINTTYPE_MAX; nCntPointDate++)
	{
		for (nCntPoint = 0; nCntPoint < POINT_MAX; nCntPoint++)
		{
			nStatipoint = g_aStatipoint[nCntPointDate].nPoint % (int)powf(10.0f, (float)nCntPoint + 1.0f) / (int)powf(10.0f, (float)nCntPoint);
			fStatipoint = (float)nStatipoint / 10.0f;

			pVtx[0].tex = D3DXVECTOR2(fStatipoint, 0.0f);		// 一つ目のテクスチャ
			pVtx[1].tex = D3DXVECTOR2(fStatipoint + 0.1f, 0.0f);		// 二つ目のテクスチャ
			pVtx[2].tex = D3DXVECTOR2(fStatipoint, 1.0f);		// 三つ目のテクスチャ
			pVtx[3].tex = D3DXVECTOR2(fStatipoint + 0.1f, 1.0f);		// 四つ目のテクスチャ

			pVtx += 4;
		}
	}
	// アンロック
	g_pVtxBuffStatipoint->Unlock();
}