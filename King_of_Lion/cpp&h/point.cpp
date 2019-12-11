// ------------------------------------------------------------------------------------------
//
// スコア処理 [pointscore.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "point.h"

#include "camera.h"

#include "Player.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define	TEXTURE_POINTSCORE		"data/TEXTURE/PointNumber.png"	// 読み込むテクスチャファイル名
#define POINTSCORE_SIZE (25)									// スコアボードの大きさ
#define POINTSCORE_MAX (128)									// 数値マックス
#define POINTSCORE_TEXTURE (4)								// テクスチャー

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
void MakeVertexPointScore(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
//
// グローバル変数宣言
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTexturePointScore = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPointScore = NULL;	// 頂点バッファへのポインタ
POINTSCORE				g_PointScore[POINTSCORE_MAX * POINTSCORE_TEXTURE];				// ポイント変数
int						g_nPointScore;				// ポイント補助

// ------------------------------------------------------------------------------------------
//
// 初期化処理
//
// ------------------------------------------------------------------------------------------
void InitPointScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntPointScore;

	// 初期化
	for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX * POINTSCORE_TEXTURE; nCntPointScore++)
	{
		g_PointScore[nCntPointScore].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_PointScore[nCntPointScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_PointScore[nCntPointScore].nPointScore = 0;
		g_PointScore[nCntPointScore].nCntAnim = 0;
		g_PointScore[nCntPointScore].bUse = false;
	}

	// 初期化
	g_nPointScore = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_POINTSCORE,		// ファイルの名前
		&g_pTexturePointScore);	// 読み込むメモリー

	// 頂点情報の作成
	MakeVertexPointScore(pDevice);
}

// ------------------------------------------------------------------------------------------
//
// 終了処理
//
// ------------------------------------------------------------------------------------------
void UninitPointScore(void)
{
	// テクスチャの開放
	if (g_pTexturePointScore != NULL)
	{
		g_pTexturePointScore->Release();
		g_pTexturePointScore = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffPointScore != NULL)
	{
		g_pVtxBuffPointScore->Release();
		g_pVtxBuffPointScore = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// 更新処理
//
// ------------------------------------------------------------------------------------------
void UpdatePointScore(void)
{
	//VERTEX_2D *pVtx;	// 頂点情報へのポイント
	//int nCntPointScore;		// スコアカウント
	//int nCntTexture;	// テクストカウント
	//PLAYER *pPlayer;	// プレイヤ情報

	//// 頂点データの範囲をロックし、頂点バッファへのポインタ
	//g_pVtxBuffPointScore->Lock(0, 0, (void **)&pVtx, 0);

	//// カウントポイントスコア
	//for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX; nCntPointScore++)
	//{
	//	// カウントテクスチャー
	//	for (nCntTexture = 0; nCntTexture < POINTSCORE_TEXTURE; nCntTexture++)
	//	{
	//		// 使われているかどうか
	//		if (g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].bUse == true)
	//		{
	//			// プレイヤー取得
	//			pPlayer = GetPlayer();
	//			
	//			// カウントアニムアップ
	//			g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].nCntAnim++;

	//			// 未使用状態へ
	//			if (g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].nCntAnim >= 50)
	//			{
	//				g_PointScore[nCntPointScore *  POINTSCORE_TEXTURE + nCntTexture].bUse = false;
	//			}

	//			// 頂点座標更新
	//			pVtx[0].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);				// 一つ目の頂点
	//			pVtx[1].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);	// 二つ目の頂点
	//			pVtx[2].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);					// 三つ目の頂点
	//			pVtx[3].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//				(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);		// 四つ目の頂点

	//		}
	//		// ポイント合わせ
	//		pVtx += 4;
	//	}
	//}
	//// アンロック
	//g_pVtxBuffPointScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// 描画処理
//
// ------------------------------------------------------------------------------------------
void DrawPointScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntPointScore;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPointScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePointScore);

	for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX * POINTSCORE_TEXTURE; nCntPointScore++)
	{
		if (g_PointScore[nCntPointScore].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPointScore, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// 頂点の作成
//
// ------------------------------------------------------------------------------------------
void MakeVertexPointScore(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntPointScore;		// スコアカウント

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * POINTSCORE_MAX * POINTSCORE_TEXTURE, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPointScore, NULL);

	// 頂点情報の設定
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPointScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX * POINTSCORE_TEXTURE; nCntPointScore++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore,
			g_PointScore[nCntPointScore].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);				// 一つ目の頂点

		pVtx[1].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore + POINTSCORE_SIZE, 
			g_PointScore[nCntPointScore].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);	// 二つ目の頂点

		pVtx[2].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore,
			g_PointScore[nCntPointScore].pos.y + POINTSCORE_SIZE / 2, 0.0f);					// 三つ目の頂点

		pVtx[3].pos = D3DXVECTOR3(g_PointScore[nCntPointScore].pos.x + (POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntPointScore + POINTSCORE_SIZE, 
			g_PointScore[nCntPointScore].pos.y + POINTSCORE_SIZE / 2, 0.0f);		// 四つ目の頂点

		pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

		pVtx[0].col = g_PointScore[nCntPointScore].col;	// 一つ目の色設定
		pVtx[1].col = g_PointScore[nCntPointScore].col;	// 二つ目の色設定
		pVtx[2].col = g_PointScore[nCntPointScore].col;	// 三つ目の色設定
		pVtx[3].col = g_PointScore[nCntPointScore].col;	// 四つ目の色設定

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// 四つ目のテクスチャ

		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffPointScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// スコアの変更
//
// ------------------------------------------------------------------------------------------
void AddPointScore(D3DXVECTOR3 pos,int nValue)
{
	//VERTEX_2D *pVtx;	// 頂点情報へのポイント
	//int nCntPointScore;		// スコアカウント
	//int nCntTexture;	// テクストカウント
	//int nPointScore;			// 桁スコア
	//float fPointScore;		// テクスチャ設定
	//g_nPointScore = nValue;	// スコア加算

	//// 頂点データの範囲をロックし、頂点バッファへのポインタ
	//g_pVtxBuffPointScore->Lock(0, 0, (void **)&pVtx, 0);

	//// 位置指定のためのループ
	//for (nCntPointScore = 0; nCntPointScore < POINTSCORE_MAX; nCntPointScore++)
	//{
	//	// 数字指定のためのループ
	//	for (nCntTexture = 0; nCntTexture < POINTSCORE_TEXTURE; nCntTexture++)
	//	{
	//		if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].bUse == false)
	//		{
	//			// 位置
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos = pos;

	//			// スコア
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore = nValue;

	//			// カウントアニメ
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nCntAnim = 0;

	//			// 使用状態
	//			g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].bUse = true;

	//			// カラー
	//			if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 200)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	//			}

	//			// カラー
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 300)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//			}

	//			// カラー
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 400)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	//			}

	//			// カラー
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 500)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//			}

	//			// カラー
	//			else if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore <= 1000)
	//			{
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//			}

	//			// 桁の数字
	//			nPointScore = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore %
	//				(int)powf(10.0f, (float)nCntTexture + 1.0f) / 
	//				(int)powf(10.0f, (float)nCntTexture);

	//				// テクスチャ座標
	//				fPointScore = (float)nPointScore / 10.0f;

	//				// 位置
	//				g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos += D3DXVECTOR3(POINTSCORE_SIZE * 5, 0.0f, 0.0f);

	//				// 頂点座標更新
	//				pVtx[0].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);				// 一つ目の頂点
	//				pVtx[1].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + (-POINTSCORE_SIZE / 2), 0.0f);	// 二つ目の頂点
	//				pVtx[2].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(-POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);					// 三つ目の頂点
	//				pVtx[3].pos = D3DXVECTOR3(g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.x +
	//					(POINTSCORE_SIZE / 2) - (POINTSCORE_SIZE + POINTSCORE_SIZE) * nCntTexture + POINTSCORE_SIZE - GetCamera()->difference.x,
	//					g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].pos.y + POINTSCORE_SIZE / 2, 0.0f);		// 四つ目の頂点

	//				// 頂点カラー更新
	//				pVtx[0].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// 一つ目の色設定
	//				pVtx[1].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// 二つ目の色設定
	//				pVtx[2].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// 三つ目の色設定
	//				pVtx[3].col = g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].col;	// 四つ目の色設定

	//				// 頂点カラー(レインボー)更新
	//				if (g_PointScore[nCntPointScore * POINTSCORE_TEXTURE + nCntTexture].nPointScore == 5000)
	//				{
	//					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 一つ目の色設定
	//					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 二つ目の色設定
	//					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);	// 三つ目の色設定
	//					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定
	//				}

	//				// 最後の数値が０の場合スキップ
	//				if (nCntTexture == POINTSCORE_TEXTURE - 1 &&
	//					nPointScore == 0)
	//				{
	//					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);	// 一つ目の色設定
	//					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);	// 二つ目の色設定
	//					pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);	// 三つ目の色設定
	//					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 四つ目の色設定
	//				}

	//				// 頂点テクスチャー更新
	//				pVtx[0].tex = D3DXVECTOR2(fPointScore, 0.0f);		// 一つ目のテクスチャ
	//				pVtx[1].tex = D3DXVECTOR2(fPointScore + 0.1f, 0.0f);	// 二つ目のテクスチャ
	//				pVtx[2].tex = D3DXVECTOR2(fPointScore, 1.0f);		// 三つ目のテクスチャ
	//				pVtx[3].tex = D3DXVECTOR2(fPointScore + 0.1f, 1.0f);	// 四つ目のテクスチャ
	//		}
	//		// ポイント合わせ
	//		pVtx += 4;
	//	}

	//	// 
	//	if (nCntTexture == POINTSCORE_TEXTURE &&								// カウントが最大値になった場合
	//		g_PointScore[nCntPointScore * POINTSCORE_TEXTURE].bUse == true &&	// 
	//		g_PointScore[nCntPointScore * POINTSCORE_TEXTURE].nCntAnim == 0)
	//	{
	//		break;
	//	}

	//}
	//// アンロック
	//g_pVtxBuffPointScore->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// スコアの取得
//
// ------------------------------------------------------------------------------------------

POINTSCORE *GetPointScore(void)
{
	return &g_PointScore[0];
}