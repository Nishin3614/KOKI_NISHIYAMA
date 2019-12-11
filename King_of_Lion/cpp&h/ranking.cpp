// ------------------------------------------------------------------------------------------
// ランキング処理 [ranking.cpp]
// Author : Koki Nishiyama
// ------------------------------------------------------------------------------------------
#include "ranking.h"			// ランキングヘッダー

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "score.h"

#include "Load_Save.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define	TEXTURE_RANKING		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define RANKING_SIZE (30)								// ランキングボードの大きさ
#define RANKING_BETWEEN (30)
#define POINT_MAX (6)									// 数値
#define RANKING_DATE (5)								// ランキング数
#define SAVE_FILE_NAME ("text.txt") // セーブファイル

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数宣言
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureRanking = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				// 頂点バッファへのポインタ
D3DXVECTOR3				g_RankingPos;							// ランキングボードのポジション
RANKING					g_aRanking[RANKING_DATE];	// ランキング変数
int						g_nRankScore;
int						g_nCntRanking;
float					g_fColRanking;
int						g_Num;

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitRanking(void)
{
	// 変数宣言
	int nCntRank;										// カウントランキング
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイス取得
	g_RankingPos = D3DXVECTOR3(850.0f, 150.0f, 0.0f);	// ポジション
	LOAD_SAVE * pLoad_Save = GetLoad_Save();

	// スコアゲット
	g_nRankScore = 0;

	for (nCntRank = 0; nCntRank < RANKING_DATE; nCntRank++,pLoad_Save++)
	{
		g_aRanking[nCntRank].nPoint = pLoad_Save->nPoint;
		g_aRanking[nCntRank].nId = nCntRank;
	}
	

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_RANKING,		// ファイルの名前
		&g_pTextureRanking);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexRanking(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitRanking(void)
{
	// テクスチャの開放
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		// 頂点バッファの開放
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateRanking(void)
{
	int nCntRanking;
	int nCntCol;

	// 頂点バッファへのポインタ
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < RANKING_DATE; nCntRanking++)
	{
		if (g_aRanking[nCntRanking].nPoint == g_nRankScore)
		{
			g_nCntRanking++;

			if (g_nCntRanking % 14 == 0)
			{
				g_fColRanking = 0.0f;
			}

			else if (g_nCntRanking % 7 == 0)
			{
				g_fColRanking = 1.0f;
			}

			// ポイント合わせ
			pVtx += nCntRanking * POINT_MAX * 4;

			for (nCntCol = 0; nCntCol < POINT_MAX; nCntCol++)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fColRanking);	// 四つ目の色設定

				pVtx += 4;
			}

			// ポイント戻し
			pVtx -= nCntRanking * POINT_MAX * 4 + POINT_MAX * 4;

		}
	}
	// アンロック
	g_pVtxBuffRanking->Unlock();
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntRanking;

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking);

	for (nCntRanking = 0; nCntRanking < POINT_MAX * RANKING_DATE; nCntRanking++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

// ------------------------------------------------------------------------------------------
// 頂点の作成
// ------------------------------------------------------------------------------------------
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntRanking;		// ランキングカウント
	int nCntRankingDate;		// ランキングカウント

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POINT_MAX * RANKING_DATE, 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// 頂点情報の設定
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRankingDate = 0; nCntRankingDate < RANKING_DATE; nCntRankingDate++)
	{
		for (nCntRanking = 0; nCntRanking < POINT_MAX; nCntRanking++)
		{
			pVtx[0].pos = D3DXVECTOR3(g_RankingPos.x + (-RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking, 
				g_RankingPos.y + (-RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate, 0.0f);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_RankingPos.x + (RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking + RANKING_SIZE, 
				g_RankingPos.y + (-RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate, 0.0f);		// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(g_RankingPos.x + (-RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking, 
				g_RankingPos.y + (RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate + RANKING_SIZE, 0.0f);		// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_RankingPos.x + (RANKING_SIZE / 2) - (RANKING_SIZE + 30) * nCntRanking + RANKING_SIZE, 
				g_RankingPos.y + (RANKING_SIZE / 2) + (RANKING_SIZE + 60) * nCntRankingDate + RANKING_SIZE, 0.0f);		// 四つ目の頂点

			pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
			pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
			pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
			pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

			if (nCntRankingDate == 0)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f,0.0f, 1.0f);	// 四つ目の色設定
			}
			else if (nCntRankingDate == 1)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定
			}

			else if (nCntRankingDate == 2)
			{
				pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);	// 四つ目の色設定
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(0.7f,0.7f,0.7f, 1.0f);	// 四つ目の色設定
			}

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目のテクスチャ
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// 二つ目のテクスチャ
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// 三つ目のテクスチャ
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// 四つ目のテクスチャ

			pVtx += 4;
		}
	}
	// アンロック
	g_pVtxBuffRanking->Unlock();
}

// ------------------------------------------------------------------------------------------
// ランキングの変更
// ------------------------------------------------------------------------------------------
void AddRanking(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntPoint;		// 数値カウント
	int nCntPointDate;	// ランキングカウント
	int nRanking;			// 桁ランキング
	float fRanking;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPointDate = 0; nCntPointDate < RANKING_DATE; nCntPointDate++)
	{
		for (nCntPoint = 0; nCntPoint < POINT_MAX; nCntPoint++)
		{
			nRanking = g_aRanking[nCntPointDate].nPoint % (int)powf(10.0f, (float)nCntPoint + 1.0f) / (int)powf(10.0f, (float)nCntPoint);
			fRanking = (float)nRanking / 10.0f;

			pVtx[0].tex = D3DXVECTOR2(fRanking, 0.0f);		// 一つ目のテクスチャ
			pVtx[1].tex = D3DXVECTOR2(fRanking + 0.1f, 0.0f);		// 二つ目のテクスチャ
			pVtx[2].tex = D3DXVECTOR2(fRanking, 1.0f);		// 三つ目のテクスチャ
			pVtx[3].tex = D3DXVECTOR2(fRanking + 0.1f, 1.0f);		// 四つ目のテクスチャ

			pVtx += 4;
		}
	}
	// アンロック
	g_pVtxBuffRanking->Unlock();
}