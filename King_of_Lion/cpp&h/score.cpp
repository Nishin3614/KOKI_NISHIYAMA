// ------------------------------------------------------------------------------------------
//
// スコア処理 [score.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "score.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_SCORE	"data/TEXTURE/PointNumber.png"	// 読み込むテクスチャファイル名
#define SCORE_SIZE (25)								// スコアボードの大きさ
#define SCORE_MAX (3)								// 数値

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// グローバル変数宣言
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// 頂点バッファへのポインタ
D3DXVECTOR3				g_ScorePos;				// スコアボードのポジション
int						g_nScore;				// スコア

// ------------------------------------------------------------------------------------------
//
// 初期化処理
//
// ------------------------------------------------------------------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_ScorePos = D3DXVECTOR3(1200, 140.0f, 0.0f);
	g_nScore = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_SCORE,		// ファイルの名前
		&g_pTextureScore);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexScore(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// 終了処理
//
// ------------------------------------------------------------------------------------------
void UninitScore(void)
{
	// テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// 更新処理
//
// ------------------------------------------------------------------------------------------
void UpdateScore(void)
{

}

// ------------------------------------------------------------------------------------------
//
// 描画処理
//
// ------------------------------------------------------------------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < SCORE_MAX; nCntScore++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

// ------------------------------------------------------------------------------------------
//
// 頂点の作成
//
// ------------------------------------------------------------------------------------------
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntScore;		// スコアカウント

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL);

	// 頂点情報の設定
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < SCORE_MAX; nCntScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_ScorePos.x + (-SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore, g_ScorePos.y + (-SCORE_SIZE / 2), 0.0f);				// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_ScorePos.x + (SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore + SCORE_SIZE, g_ScorePos.y + (-SCORE_SIZE / 2), 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(g_ScorePos.x + (-SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore, g_ScorePos.y + SCORE_SIZE / 2, 0.0f);					// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_ScorePos.x + (SCORE_SIZE / 2) - (SCORE_SIZE + SCORE_SIZE) * nCntScore + SCORE_SIZE, g_ScorePos.y + SCORE_SIZE / 2, 0.0f);		// 四つ目の頂点

		pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 一つ目の色設定
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 二つ目の色設定
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 四つ目の色設定

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// スコアの変更
//
// ------------------------------------------------------------------------------------------
void AddScore(int nValue)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntScore;		// スコアカウント
	int nScore;			// 桁スコア
	float fScore;		// テクスチャ設定
	g_nScore += nValue;	// スコア加算

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < SCORE_MAX; nCntScore++)
	{
		// 桁の数字
   		nScore = g_nScore % (int)powf(10.0f, (float)nCntScore + 1.0f) / (int)powf(10.0f, (float)nCntScore);
 		fScore = (float)nScore / 10.0f;
		
		pVtx[0].tex = D3DXVECTOR2(fScore, 0.0f);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(fScore + 0.1f, 0.0f);	// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(fScore, 1.0f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(fScore + 0.1f, 1.0f);	// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// スコアの取得
//
// ------------------------------------------------------------------------------------------

int GetScore(void)
{
	return g_nScore;
}
