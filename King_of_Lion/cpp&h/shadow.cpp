// ------------------------------------------------------------------------------------------
//
// 影処理 [shadow.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "shadow.h"

//#include "polygon.h"

#include "floor.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define SHADOW_MAX (128)								// 影の最大数 
#define SHADOW_SIZE (20)							// サイズ
#define SHADOW_PHONT "data/TEXTURE/shadow000.jpg"	// 影の写真

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// 頂点テクスチャーのポインタ
SHADOW					g_shadow[SHADOW_MAX];		// 影情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntShadow;								// カウント影
	
	// 初期設定
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		g_shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_shadow[nCntShadow].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SHADOW_PHONT, &g_pTextureShadow);

	// 頂点情報の作成
	MakeVertexShadow(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitShadow(void)
{
	// テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateShadow(void)
{

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawShadow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	SHADOW *pShadow = &g_shadow[0];				// 影情報
	D3DXMATRIX mtxRot, mtxTrans;				// 回転・位置の計算用変数
	int nCntShadow;								// カウント影

	// 減算 
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pShadow->mtxWorldShadow);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pShadow->mtxWorldShadow,	// 1
				&pShadow->mtxWorldShadow,					// 2
				&mtxRot);									// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pShadow->mtxWorldShadow,	// 1
				&pShadow->mtxWorldShadow,					// 2
				&mtxTrans);									// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorldShadow);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ------------------------------------------------------------------------------------------
// 影の設定
// ------------------------------------------------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	FLOOR *pFloor = GetFLOOR();	// ポリゴン情報
	int nCntShadow;		// カウントバレット

	// ブロックループ
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		// 使用していない状態
		if (!g_shadow[nCntShadow].bUse)
		{
			// 情報更新
			g_shadow[nCntShadow].pos = pos;
			g_shadow[nCntShadow].size = size;
			g_shadow[nCntShadow].col = col;
			g_shadow[nCntShadow].bUse = true;

			// 影のy座標
			g_shadow[nCntShadow].pos.y = pFloor->pos.y;

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);

			// ポイント合わせ
			pVtx += 4 * nCntShadow;

			// 頂点座標の設定					
			pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, g_shadow[nCntShadow].size.z);		// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x, 0.1f, g_shadow[nCntShadow].size.z);		// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, -g_shadow[nCntShadow].size.z);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x, 0.1f, -g_shadow[nCntShadow].size.z);		// 四つ目の頂点

			// カラー
			pVtx[0].col = g_shadow[nCntShadow].col;	// 一つ目の色設定
			pVtx[1].col = g_shadow[nCntShadow].col;	// 二つ目の色設定
			pVtx[2].col = g_shadow[nCntShadow].col;	// 三つ目の色設定
			pVtx[3].col = g_shadow[nCntShadow].col;	// 四つ目の色設定

			// アンロック
			g_pVtxBuffShadow->Unlock();

			break;
		}
	}
	return nCntShadow;
}

// ------------------------------------------------------------------------------------------
// 影の位置更新
// ------------------------------------------------------------------------------------------
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	FLOOR *pFloor = GetFLOOR();	// ポリゴン情報
	float fWhile;						// 地面とモデルの距離

	// 位置の更新
	g_shadow[nIdxShadow].pos = pos;

	// 距離
	fWhile = g_shadow[nIdxShadow].pos.y - pFloor->pos.y;

	g_shadow[nIdxShadow].col.a = 1.0f - fWhile / 200.0f;

	// 影を地面に
	for (int nCntFloor = 0; nCntFloor < 128; nCntFloor++, pFloor++)
	{
		// ブロックのX範囲
		if (g_shadow[nIdxShadow].pos.x + g_shadow[nIdxShadow].size.x * 0.5f >
			pFloor->pos.x - pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_shadow[nIdxShadow].pos.x - g_shadow[nIdxShadow].size.x * 0.5f <
			pFloor->pos.x + pFloor->size.x * 0.5f * pFloor->nBlock_Width&&
			g_shadow[nIdxShadow].pos.z + g_shadow[nIdxShadow].size.z * 0.5f >
			pFloor->pos.z - pFloor->size.z * 0.5f * pFloor->nBlock_Depth&&
			g_shadow[nIdxShadow].pos.z - g_shadow[nIdxShadow].size.z * 0.5f <
			pFloor->pos.z + pFloor->size.z * 0.5f * pFloor->nBlock_Depth)
		{

			g_shadow[nIdxShadow].pos.y = pFloor->pos.y;
		}
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);

	// データのポインタを合わせる
	pVtx += nIdxShadow * 4;

	// カラー
	pVtx[0].col = g_shadow[nIdxShadow].col;	// 一つ目の色設定
	pVtx[1].col = g_shadow[nIdxShadow].col;	// 二つ目の色設定
	pVtx[2].col = g_shadow[nIdxShadow].col;	// 三つ目の色設定
	pVtx[3].col = g_shadow[nIdxShadow].col;	// 四つ目の色設定

	// アンロック
	g_pVtxBuffShadow->Unlock();
}

// ------------------------------------------------------------------------------------------
// 影消滅処理
// ------------------------------------------------------------------------------------------
void DeleteShadow(int nIdxShadow)
{
	g_shadow[nIdxShadow].bUse = false;
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntShadow;		// カウント影

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * SHADOW_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);
	
	// 初期設定
	for (nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		// 頂点座標の設定					
		pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, g_shadow[nCntShadow].size.z);		// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x,  0.1f, g_shadow[nCntShadow].size.z);		// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].size.x, 0.1f, -g_shadow[nCntShadow].size.z);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_shadow[nCntShadow].size.x,  0.1f, -g_shadow[nCntShadow].size.z);		// 四つ目の頂点

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 四つ目の頂点

		// カラー
		pVtx[0].col = g_shadow[nCntShadow].col;	// 一つ目の色設定
		pVtx[1].col = g_shadow[nCntShadow].col;	// 二つ目の色設定
		pVtx[2].col = g_shadow[nCntShadow].col;	// 三つ目の色設定
		pVtx[3].col = g_shadow[nCntShadow].col;	// 四つ目の色設定

		// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目の色設定

		// ポイント合わせ
		pVtx += 4;
	}
	// アンロック

	g_pVtxBuffShadow->Unlock();
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
SHADOW *GetShadow(void)
{
	return &g_shadow[0];
}