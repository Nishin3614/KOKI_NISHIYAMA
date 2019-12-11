// ------------------------------------------------------------------------------------------
//
// 予測線処理 [prediction.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "prediction.h"

//#include "polygon.h"

#include "floor.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define PRIDICTION_MAX (128)								// 予測線の最大数 
#define PRIDICTION_SIZE (20)							// サイズ
#define PRIDICTION_PHONT "data/TEXTURE/shadow000.jpg"	// 予測線の写真

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexPrediction(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPrediction = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTexturePrediction = NULL;	// 頂点テクスチャーのポインタ
PRIDICTION				g_prediction[PRIDICTION_MAX];	// 予測線情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitPrediction(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntPrediction;								// カウント予測線
	
	// 初期設定
	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++)
	{
		g_prediction[nCntPrediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_prediction[nCntPrediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_prediction[nCntPrediction].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_prediction[nCntPrediction].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PRIDICTION_PHONT, &g_pTexturePrediction);

	// 頂点情報の作成
	MakeVertexPrediction(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitPrediction(void)
{
	// テクスチャの開放
	if (g_pTexturePrediction != NULL)
	{
		g_pTexturePrediction->Release();
		g_pTexturePrediction = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPrediction != NULL)
	{
		g_pVtxBuffPrediction->Release();
		g_pVtxBuffPrediction = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdatePrediction(void)
{

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawPrediction(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	PRIDICTION *pPrediction = &g_prediction[0];				// 予測線情報
	D3DXMATRIX mtxRot, mtxTrans;				// 回転・位置の計算用変数
	int nCntPrediction;								// カウント予測線

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++,pPrediction++)
	{
		if (pPrediction->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pPrediction->mtxWorldPrediction);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pPrediction->rot.y, pPrediction->rot.x, pPrediction->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pPrediction->mtxWorldPrediction,	// 1
				&pPrediction->mtxWorldPrediction,					// 2
				&mtxRot);									// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pPrediction->pos.x, pPrediction->pos.y, pPrediction->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pPrediction->mtxWorldPrediction,	// 1
				&pPrediction->mtxWorldPrediction,					// 2
				&mtxTrans);									// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pPrediction->mtxWorldPrediction);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPrediction, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePrediction);

			// 予測線の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPrediction * 4, 2);
		}
	}
	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ------------------------------------------------------------------------------------------
// 予測線の設定
// ------------------------------------------------------------------------------------------
int SetPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	FLOOR *pFloor = GetFLOOR();	// ポリゴン情報
	int nCntPrediction;		// カウントバレット

	// ブロックループ
	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++)
	{
		// 使用していない状態
		if (!g_prediction[nCntPrediction].bUse)
		{
			// 情報更新
			g_prediction[nCntPrediction].pos = pos;
			g_prediction[nCntPrediction].size = size;
			g_prediction[nCntPrediction].col = col;
			g_prediction[nCntPrediction].bUse = true;

			// 予測線のy座標
			g_prediction[nCntPrediction].pos.y = pFloor->pos.y;

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_pVtxBuffPrediction->Lock(0, 0, (void **)&pVtx, 0);

			// ポイント合わせ
			pVtx += 4 * nCntPrediction;

			// 頂点座標の設定					
			pVtx[0].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, g_prediction[nCntPrediction].size.z);		// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x, 0.1f, g_prediction[nCntPrediction].size.z);		// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, -g_prediction[nCntPrediction].size.z);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x, 0.1f, -g_prediction[nCntPrediction].size.z);		// 四つ目の頂点

			// カラー
			pVtx[0].col = g_prediction[nCntPrediction].col;	// 一つ目の色設定
			pVtx[1].col = g_prediction[nCntPrediction].col;	// 二つ目の色設定
			pVtx[2].col = g_prediction[nCntPrediction].col;	// 三つ目の色設定
			pVtx[3].col = g_prediction[nCntPrediction].col;	// 四つ目の色設定

			// アンロック
			g_pVtxBuffPrediction->Unlock();

			break;
		}
	}
	return nCntPrediction;
}

// ------------------------------------------------------------------------------------------
// 予測線の位置更新
// ------------------------------------------------------------------------------------------
void SetPositionPrediction(int nIdxPrediction, D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	FLOOR *pFloor = GetFLOOR();	// ポリゴン情報
	float fWhile;						// 地面とモデルの距離

	// 位置の更新
	g_prediction[nIdxPrediction].pos = pos;

	// 距離
	fWhile = g_prediction[nIdxPrediction].pos.y - pFloor->pos.y;

	g_prediction[nIdxPrediction].col.a = 1.0f - fWhile / 200.0f;

	// 予測線を地面に
	for (int nCntFloor = 0; nCntFloor < 128; nCntFloor++, pFloor++)
	{
		// ブロックのX範囲
		if (g_prediction[nIdxPrediction].pos.x + g_prediction[nIdxPrediction].size.x * 0.5f >
			pFloor->pos.x - pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_prediction[nIdxPrediction].pos.x - g_prediction[nIdxPrediction].size.x * 0.5f <
			pFloor->pos.x + pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_prediction[nIdxPrediction].pos.z + g_prediction[nIdxPrediction].size.z * 0.5f >
			pFloor->pos.z - pFloor->size.z * 0.5f * pFloor->nBlock_Depth&&
			g_prediction[nIdxPrediction].pos.z - g_prediction[nIdxPrediction].size.z * 0.5f <
			pFloor->pos.z + pFloor->size.z * 0.5f * pFloor->nBlock_Depth)
		{

			g_prediction[nIdxPrediction].pos.y = pFloor->pos.y;
		}
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPrediction->Lock(0, 0, (void **)&pVtx, 0);

	// データのポインタを合わせる
	pVtx += nIdxPrediction * 4;

	// カラー
	pVtx[0].col = g_prediction[nIdxPrediction].col;	// 一つ目の色設定
	pVtx[1].col = g_prediction[nIdxPrediction].col;	// 二つ目の色設定
	pVtx[2].col = g_prediction[nIdxPrediction].col;	// 三つ目の色設定
	pVtx[3].col = g_prediction[nIdxPrediction].col;	// 四つ目の色設定

	// アンロック
	g_pVtxBuffPrediction->Unlock();
}

// ------------------------------------------------------------------------------------------
// 予測線消滅処理
// ------------------------------------------------------------------------------------------
void DeletePrediction(int nIdxPrediction)
{
	g_prediction[nIdxPrediction].bUse = false;
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexPrediction(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntPrediction;		// カウント予測線

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * PRIDICTION_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPrediction, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPrediction->Lock(0, 0, (void **)&pVtx, 0);
	
	// 初期設定
	for (nCntPrediction = 0; nCntPrediction < PRIDICTION_MAX; nCntPrediction++)
	{
		// 頂点座標の設定					
		pVtx[0].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, g_prediction[nCntPrediction].size.z);		// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x,  0.1f, g_prediction[nCntPrediction].size.z);		// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(-g_prediction[nCntPrediction].size.x, 0.1f, -g_prediction[nCntPrediction].size.z);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_prediction[nCntPrediction].size.x,  0.1f, -g_prediction[nCntPrediction].size.z);		// 四つ目の頂点

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 四つ目の頂点

		// カラー
		pVtx[0].col = g_prediction[nCntPrediction].col;	// 一つ目の色設定
		pVtx[1].col = g_prediction[nCntPrediction].col;	// 二つ目の色設定
		pVtx[2].col = g_prediction[nCntPrediction].col;	// 三つ目の色設定
		pVtx[3].col = g_prediction[nCntPrediction].col;	// 四つ目の色設定

		// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目の色設定

		// ポイント合わせ
		pVtx += 4;
	}
	// アンロック

	g_pVtxBuffPrediction->Unlock();
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
PRIDICTION *GetPrediction(void)
{
	return &g_prediction[0];
}