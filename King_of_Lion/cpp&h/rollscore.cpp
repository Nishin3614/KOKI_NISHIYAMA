// ------------------------------------------------------------------------------------------
//
// スコア処理 [rollscorescore.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "rollscore.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_ROLLSCORE		"data/TEXTURE/ui/00_RollScore_Number.png"	// 読み込むテクスチャファイル名
#define ROLLSCORE_SIZE (40)								// スコアボードの大きさ
#define ROLLSCORE_BETWEEN (15)							// 数字と数字の間
#define ROLLSCORE_MAX (6)								// 数値

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
void MakeVertexRollScore(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// グローバル変数宣言
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureRollScore = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRollScore = NULL;				// 頂点バッファへのポインタ
D3DXVECTOR3				g_RollScorePos;							// スコアボードのポジション
float					g_fRollScore = 0.0;		// テクスチャ設定
int						g_nRollScore;							// スコア

// ------------------------------------------------------------------------------------------
//
// 初期化処理
//
// ------------------------------------------------------------------------------------------
void InitRollScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_RollScorePos = D3DXVECTOR3(1100.0f, 50.0f, 0.0f);
	g_nRollScore = 12345;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_ROLLSCORE,		// ファイルの名前
		&g_pTextureRollScore);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexRollScore(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// 終了処理
//
// ------------------------------------------------------------------------------------------
void UninitRollScore(void)
{
	// テクスチャの開放
	if (g_pTextureRollScore != NULL)
	{
		g_pTextureRollScore->Release();
		g_pTextureRollScore = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffRollScore != NULL)
	{
		g_pVtxBuffRollScore->Release();
		g_pVtxBuffRollScore = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// 更新処理
//
// ------------------------------------------------------------------------------------------
void UpdateRollScore(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntRollScore;		// スコアカウント

	// 縦に回転する
	g_fRollScore += 0.05f;

	// 上限を超えたら初期化
	if (g_fRollScore >= 1.0f)
	{
		g_fRollScore = 0.0f;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRollScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_fRollScore);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_fRollScore);	// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_fRollScore + 0.1f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_fRollScore + 0.1f);	// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffRollScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// 描画処理
//
// ------------------------------------------------------------------------------------------
void DrawRollScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntRollScore;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffRollScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRollScore);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRollScore, 2);
	}
}

// ------------------------------------------------------------------------------------------
//
// 頂点の作成
//
// ------------------------------------------------------------------------------------------
void MakeVertexRollScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntRollScore;		// スコアカウント

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ROLLSCORE_MAX,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffRollScore,
		NULL);

	// 頂点情報の設定
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRollScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(
			g_RollScorePos.x + (-ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN)  * nCntRollScore,
			g_RollScorePos.y + (-ROLLSCORE_SIZE / 2),
			0.0f);				// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(
			g_RollScorePos.x + (ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN) * nCntRollScore + ROLLSCORE_SIZE,
			g_RollScorePos.y + (-ROLLSCORE_SIZE / 2),
			0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(
			g_RollScorePos.x + (-ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN) * nCntRollScore,
			g_RollScorePos.y + ROLLSCORE_SIZE / 2,
			0.0f);					// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(
			g_RollScorePos.x + (ROLLSCORE_SIZE / 2) - (ROLLSCORE_SIZE + ROLLSCORE_BETWEEN) * nCntRollScore + ROLLSCORE_SIZE,
			g_RollScorePos.y + ROLLSCORE_SIZE / 2,
			0.0f);		// 四つ目の頂点

		pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// 一つ目の色設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// 二つ目の色設定
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// 四つ目の色設定

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);		// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.1f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.1f);		// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffRollScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// スコアの変更
//
// ------------------------------------------------------------------------------------------
void AddRollScore(int nValue)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntRollScore;		// スコアカウント
	int nRollScore;			// 桁スコア
	float fRollScore;		// テクスチャ設定
	g_nRollScore += nValue;	// スコア加算

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffRollScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntRollScore = 0; nCntRollScore < ROLLSCORE_MAX; nCntRollScore++)
	{
		// 桁の数字
   		nRollScore = g_nRollScore % (int)powf(10.0f, (float)nCntRollScore + 1.0f) / (int)powf(10.0f, (float)nCntRollScore);
 		fRollScore = (float)nRollScore / 10.0f;
		
		pVtx[0].tex = D3DXVECTOR2(0.0f, fRollScore);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, fRollScore);	// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, fRollScore + 0.1f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, fRollScore + 0.1f);	// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffRollScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// スコアの取得
//
// ------------------------------------------------------------------------------------------

int GetRollScore(void)
{
	return g_nRollScore;
}
