// ------------------------------------------------------------------------------------------
//
// 配置処理 [gridshape.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "gridshape.h"
#include "input.h"
#include "camera.h"
#include "mouse.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_GRIDSHAPE (1)
#define GRIDSHAPE_MOVE (2.0f)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexGridshape(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 pVtxBuffGridshape = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	pIndexGridshape = NULL;		// インデックスバッファのポインタ
D3DXMATRIX				mtxWorldGridshape;			// ワールドマトリックス

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitGridshape(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
												// 頂点情報の作成
	MakeVertexGridshape(pDevice);

}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitGridshape(void)
{
	if (pVtxBuffGridshape != NULL)
	{
		pVtxBuffGridshape->Release();
		pVtxBuffGridshape = NULL;
	}

	// 頂点インデックスの開放
	if (pIndexGridshape != NULL)
	{
		pIndexGridshape->Release();
		pIndexGridshape = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateGridshape(void)
{

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawGridshape(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//											// 回転・位置の計算用変数
	//D3DXMATRIX mtxRot, mtxTrans;

	//int nCntGridshape;
			//// ワールドマトリックスの初期化(単位行列にするため)
			//D3DXMatrixIdentity(&g_floor[nCntGridshape].mtxWorldFLOOR);

			//// 回転を反映 //
			//// ヨーピッチロールによる回転行列作成(y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
			//	g_floor[nCntGridshape].rot.y, g_floor[nCntGridshape].rot.x, g_floor[nCntGridshape].rot.z);	// それぞれの回転量

			//																					// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			//D3DXMatrixMultiply(&g_floor[nCntGridshape].mtxWorldFLOOR,	// 1
			//	&g_floor[nCntGridshape].mtxWorldFLOOR,					// 2
			//	&mtxRot);							// 3


			//										// 位置を反映 //
			//										// 平行移動行列作成(オフセット)
			//D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
			//	g_floor[nCntGridshape].pos.x, g_floor[nCntGridshape].pos.y, g_floor[nCntGridshape].pos.z);	// それぞれの移動量

			//																					// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			//D3DXMatrixMultiply(&g_floor[nCntGridshape].mtxWorldFLOOR,	// 1
			//	&g_floor[nCntGridshape].mtxWorldFLOOR,					// 2
			//	&mtxTrans);							// 3

			//										// ワールドマトリックスの設定
			//pDevice->SetTransform(D3DTS_WORLD, &g_floor[nCntGridshape].mtxWorldFLOOR);



			//// 頂点バッファをデータストリームに設定
			//pDevice->SetStreamSource(0,pVtxBuffGridshape, 0, sizeof(VERTEX_3D));

			//// インデックスバッファをデータストリームを設定
			//pDevice->SetIndices(pIndexGridshape);

			//// 頂点フォーマットの設定
			//pDevice->SetFVF(FVF_VERTEX_3D);

			//// ポリゴンの描画
			//pDevice->DrawIndexedPrimitive(
			//	D3DPT_TRIANGLESTRIP,
			//	0,
			//	0,
			//	g_floor[nCntGridshape].nNumberVertexMeshField,
			//	0,
			//	g_floor[nCntGridshape].nNumPolygon);
}


// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexGridshape(LPDIRECT3DDEVICE9 pDevice)
{
	//// 変数宣言
	//int nCountDirect;
	//int nCountWidth;
	//int nCntGridshape;

	//for (nCntGridshape = 0; nCntGridshape < ALL_MAX; nCntGridshape++)
	//{
	//	// 頂点バッファの生成
	//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 1000,
	//		D3DUSAGE_WRITEONLY,
	//		FVF_VERTEX_3D,
	//		D3DPOOL_MANAGED,
	//		&pVtxBuffGridshape,
	//		NULL);

	//	// 頂点情報の設定
	//	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	//						// 頂点データの範囲をロックし、頂点バッファへのポインタ
	//	pVtxBuffGridshape->Lock(0, 0, (void **)&pVtx, 0);

	//		// 頂点座標の設定
	//		pVtx[0].pos =
	//			D3DXVECTOR3(
	//				g_floor[nCntGridshape].OriginBlock.x + (g_floor[nCntGridshape].size.x * nCountWidth),
	//				0.0f,
	//				g_floor[nCntGridshape].OriginBlock.z - (g_floor[nCntGridshape].size.z * nCountDirect));

	//		// 法線ベクトルの設定
	//		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//		if (g_floor[nCntGridshape].bUse == true)
	//		{
	//			// 色の設定
	//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		}

	//		else
	//		{
	//			// 色の設定
	//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//		}

	//		// テクスチャーの設定
	//		pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

	//		// ポイント合わせ
	//		pVtx++;

	//	// アンロック
	//	g_floor[nCntGridshape].pVtxBuffFLOOR->Unlock();

	//	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	//	g_floor[nCntGridshape].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

	//	// 縦ブロック個数
	//	for (nCountDirect = 0; nCountDirect < g_floor[nCntGridshape].nBlock_Depth; nCountDirect++)
	//	{
	//		// ２回目のループ以降
	//		if (nCountDirect >= 1)
	//		{
	//			// 縮退ポリゴン分の頂点追加
	//			pIdx[0] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + g_floor[nCntGridshape].nBlock_Width + 1;

	//			// インデックスのポイント合わせ
	//			pIdx++;
	//		}

	//		// 横ブロックの頂点数
	//		for (nCountWidth = 0; nCountWidth < g_floor[nCntGridshape].nBlock_Width + 1; nCountWidth++)
	//		{
	//			// 描画順番のインデックス
	//			pIdx[0] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + nCountWidth + g_floor[nCntGridshape].nBlock_Width + 1;
	//			pIdx[1] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + nCountWidth;

	//			// インデックスのポイント合わせ
	//			pIdx += 2;
	//		}

	//		// 縮退ポリゴンを作る必要がある場合
	//		if (nCountDirect < g_floor[nCntGridshape].nBlock_Depth - 1)
	//		{
	//			// 縮退ポリゴン分の頂点追加
	//			pIdx[0] = nCountDirect * (g_floor[nCntGridshape].nBlock_Width + 1) + g_floor[nCntGridshape].nBlock_Width;

	//			// インデックスのポイント合わせ
	//			pIdx++;
	//		}
	//	}

	//	// アンロック
	//	g_floor[nCntGridshape].pIndexMeshField->Unlock();
	//}
}