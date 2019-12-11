// ------------------------------------------------------------------------------------------
//
// 半球処理 [hemisphere.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "hemisphere.h"

#include "input.h"

#include "player.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define HEMISPHERE_PHONT "data/TEXTURE/mesh/00_sky.jpg"	// 床の写真

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexHemisphere(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
HEMISPHERE g_hemisphere;				// 半球

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitHemisphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転・移動量・サイズの初期設定
	g_hemisphere.pos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
	g_hemisphere.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	g_hemisphere.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_hemisphere.size = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	// ブロックの初期化
	g_hemisphere.nBlock_Depth = 20;
	g_hemisphere.nBlock_Width = 20;

	// ブロック描画の原点
	g_hemisphere.OriginBlock = D3DXVECTOR3(
		sinf(D3DX_PI * 0.0f) * g_hemisphere.size.x * -0.5f,
		0.0f,
		cosf(D3DX_PI * 0.0f) * g_hemisphere.size.z * 0.5f);

	// テクスチャ・インデックス・バッファの初期化
	g_hemisphere.pTextureHemisphere = NULL;
	g_hemisphere.pIndexMeshField = NULL;
	g_hemisphere.pVtxBuffHemisphere = NULL;

	// 総頂点数・インデックス・半球の初期化(計算)
	g_hemisphere.nNumberVertexMeshField = (g_hemisphere.nBlock_Depth + 1) * (g_hemisphere.nBlock_Width + 1);
	g_hemisphere.nNumIndex = (g_hemisphere.nBlock_Width + 1) * 2 * g_hemisphere.nBlock_Depth + 2 * (g_hemisphere.nBlock_Depth - 1);
	g_hemisphere.nNumPolygon = g_hemisphere.nBlock_Depth * g_hemisphere.nBlock_Width * 2 + 4 * (g_hemisphere.nBlock_Depth - 1);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, HEMISPHERE_PHONT, &g_hemisphere.pTextureHemisphere);

	// 頂点情報の作成
	MakeVertexHemisphere(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitHemisphere(void)
{
	// テクスチャの開放
	if (g_hemisphere.pTextureHemisphere != NULL)
	{
		g_hemisphere.pTextureHemisphere->Release();
		g_hemisphere.pTextureHemisphere = NULL;
	}

	// 頂点バッファの開放
	if (g_hemisphere.pVtxBuffHemisphere != NULL)
	{
		g_hemisphere.pVtxBuffHemisphere->Release();
		g_hemisphere.pVtxBuffHemisphere = NULL;
	}

	// 頂点インデックスの開放
	if (g_hemisphere.pIndexMeshField != NULL)
	{
		g_hemisphere.pIndexMeshField->Release();
		g_hemisphere.pIndexMeshField = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateHemisphere(void)
{
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawHemisphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&g_hemisphere.mtxWorldHemisphere);

	// 回転を反映 //
	// ヨーピッチロールによる回転行列作成(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
		g_hemisphere.rot.y, g_hemisphere.rot.x, g_hemisphere.rot.z);	// それぞれの回転量
	
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&g_hemisphere.mtxWorldHemisphere,	// 1
		&g_hemisphere.mtxWorldHemisphere,					// 2
		&mtxRot);							// 3

	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
		g_hemisphere.pos.x, g_hemisphere.pos.y, g_hemisphere.pos.z);	// それぞれの移動量

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&g_hemisphere.mtxWorldHemisphere,	// 1
		&g_hemisphere.mtxWorldHemisphere,					// 2
		&mtxTrans);							// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_hemisphere.mtxWorldHemisphere);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_hemisphere.pVtxBuffHemisphere, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームを設定
	pDevice->SetIndices(g_hemisphere.pIndexMeshField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_hemisphere.pTextureHemisphere);

	// 表面(右回り)でカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 半球の描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_hemisphere.nNumberVertexMeshField, 0, g_hemisphere.nNumPolygon);

	// 裏面(左回り)でカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexHemisphere(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCountDirect;
	int nCountWidth;
	float fXZAngel;
	float fXZRadian;
	float fYAngle;
	float fYRadian;

	// 角度の計算
	fXZAngel = D3DX_PI * 2 / (g_hemisphere.nBlock_Width);
	fYAngle = D3DX_PI / g_hemisphere.nBlock_Depth;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_hemisphere.nNumberVertexMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_hemisphere.pVtxBuffHemisphere,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_hemisphere.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_hemisphere.pIndexMeshField,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// インデックス情報の設定
	WORD * pIdx;		// インデックスデータへのポインタを取得

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_hemisphere.pVtxBuffHemisphere->Lock(0, 0, (void **)&pVtx, 0);

	 //頂点設定 //
	 //行ループ
	for (nCountDirect = 0; nCountDirect < g_hemisphere.nBlock_Depth + 1; nCountDirect++)
	{
		// 列ループ
		for (nCountWidth = 0; nCountWidth < g_hemisphere.nBlock_Width + 1; nCountWidth++)
		{
			// ラジアン値(XZ)
			fXZRadian = fXZAngel * nCountWidth;

			// ラジアン値(Y)
			fYRadian = fYAngle * nCountDirect;

			// 
			float kake = (nCountDirect) / 1.0f;

			// -3.14未満
			if (fXZRadian  < -D3DX_PI)
			{
				fXZRadian += D3DX_PI * 2;
			}

			// 3.14超過
			else if (fXZRadian > D3DX_PI)
			{
				fXZRadian -= D3DX_PI * 2;
			}

			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(
				g_hemisphere.OriginBlock.x + sinf(fXZRadian) * g_hemisphere.size.x * kake,
				g_hemisphere.OriginBlock.y + cosf(fYRadian) * g_hemisphere.size.y, 
				g_hemisphere.OriginBlock.z - cosf(fXZRadian) * g_hemisphere.size.z * kake);

			// 法線ベクトルの設定
			pVtx[0].nor = pVtx[0].pos;
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			// 色の設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャーの設定
			pVtx[0].tex = D3DXVECTOR2(
				1.0f / g_hemisphere.nBlock_Width * nCountWidth,
				1.0f / g_hemisphere.nBlock_Depth * nCountDirect);

			// ポイント合わせ
			pVtx++;
		}
	}

	// アンロック
	g_hemisphere.pVtxBuffHemisphere->Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_hemisphere.pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

	// 縦ブロック個数
	for (nCountDirect = 0; nCountDirect < g_hemisphere.nBlock_Depth; nCountDirect++)
	{
		// ２回目のループ以降
		if (nCountDirect >= 1)
		{
			// 縮退半球分の頂点追加
			pIdx[0] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + g_hemisphere.nBlock_Width + 1;
 
			// インデックスのポイント合わせ
			pIdx++;
		}

		// 横ブロックの頂点数
		for (nCountWidth = 0; nCountWidth < g_hemisphere.nBlock_Width + 1; nCountWidth++)
		{
			// 描画順番のインデックス
			pIdx[0] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + nCountWidth + g_hemisphere.nBlock_Width + 1;
			pIdx[1] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + nCountWidth ;

			// インデックスのポイント合わせ
			pIdx += 2;
		}

		// 縮退半球を作る必要がある場合
		if (nCountDirect < g_hemisphere.nBlock_Depth -1)
		{
			// 縮退半球分の頂点追加
			pIdx[0] = nCountDirect * (g_hemisphere.nBlock_Width + 1) + g_hemisphere.nBlock_Width;

			// インデックスのポイント合わせ
			pIdx++;
		}
	}

	// アンロック
	g_hemisphere.pIndexMeshField->Unlock();
}

// ------------------------------------------------------------------------------------------
// 半球情報
// ------------------------------------------------------------------------------------------
HEMISPHERE *GetHemisphere(void)
{
	return &g_hemisphere;
}