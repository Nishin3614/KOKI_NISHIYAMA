// ------------------------------------------------------------------------------------------
//
// ポリゴン処理 [polygon.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "polygon.h"

#include "input.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define POLYGON_SIZE (100)
#define POLYGON_PHONT "data/TEXTURE/soil.jpg"
// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;	// 頂点テクスチャーのポインタ
POLYGON g_polygon;				// ポリゴン

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	g_polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_polygon.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	g_polygon.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, POLYGON_PHONT, &g_pTexturePolygon);

	// 頂点情報の作成
	MakeVertexPolygon(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitPolygon(void)
{
	// テクスチャの開放
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdatePolygon(void)
{
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&g_polygon.mtxWorldPolygon);

	// 回転を反映 //
	// ヨーピッチロールによる回転行列作成(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
		g_polygon.rot.y, g_polygon.rot.x, g_polygon.rot.z);	// それぞれの回転量
	
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&g_polygon.mtxWorldPolygon,	// 1
		&g_polygon.mtxWorldPolygon,					// 2
		&mtxRot);							// 3


	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
		g_polygon.pos.x, g_polygon.pos.y, g_polygon.pos.z);	// それぞれの移動量

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&g_polygon.mtxWorldPolygon,	// 1
		&g_polygon.mtxWorldPolygon,					// 2
		&mtxTrans);							// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_polygon.mtxWorldPolygon);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPolygon->Lock(0, 0, (void **)&pVtx, 0);
									
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-POLYGON_SIZE, 0, POLYGON_SIZE);	// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(POLYGON_SIZE, 0, POLYGON_SIZE);	// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(-POLYGON_SIZE, 0, -POLYGON_SIZE);	// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(POLYGON_SIZE, 0, -POLYGON_SIZE);	// 四つ目の頂点


	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); 	// 一つ目の頂点
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); 		// 二つ目の頂点
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // 三つ目の頂点
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // 四つ目の頂点

	// カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 一つ目の色設定
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 二つ目の色設定
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 三つ目の色設定
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定

	// テクスチャー
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目の色設定
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目の色設定
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目の色設定
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目の色設定

	// アンロック
	g_pVtxBuffPolygon->Unlock();
}

// ------------------------------------------------------------------------------------------
// ポリゴン情報
// ------------------------------------------------------------------------------------------
POLYGON *GetPolygon(void)
{
	return &g_polygon;
}