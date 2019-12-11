// ------------------------------------------------------------------------------------------
//
// ドーム処理 [dome.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "dome.h"

#include "input.h"

#include "player.h"

#include "camera.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define DOME_PHONT "data/TEXTURE/Water_Texture.png"	// 床の写真
#define DOME_MAX (128)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexDome(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
DOME g_dome[DOME_MAX];									// ドーム
LPDIRECT3DTEXTURE9	pTextureDome[DOMETYPE_MAX] = {};	// 頂点テクスチャーのポインタ

														// テクスチャーファイル名定義
const char *g_paTexDomeFile[DOMETYPE_MAX] =
{
	{ "data/TEXTURE/Line.png" },		// 00_線
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitDome(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// ドームの初期化
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// 位置・回転・移動量・サイズの初期設定
		g_dome[nCntDome].pos = D3DXVECTOR3(0.0f, 0.0, 0.0f);
		g_dome[nCntDome].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_dome[nCntDome].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_dome[nCntDome].size = D3DXVECTOR3(25.0f, 50.0f, 25.0f);
		g_dome[nCntDome].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ブロックの初期化
		g_dome[nCntDome].nBlock_Depth = 1;
		g_dome[nCntDome].nBlock_Width = 20;

		// テクスチャ・インデックス・バッファの初期化
		g_dome[nCntDome].pIndexMeshField = NULL;
		g_dome[nCntDome].pVtxBuffDome = NULL;

		// 総頂点数・インデックス・ドームの初期化(計算)
		g_dome[nCntDome].nNumberVertexMeshField =
			(g_dome[nCntDome].nBlock_Depth + 1) *
			(g_dome[nCntDome].nBlock_Width + 1);

		g_dome[nCntDome].nNumIndex =
			(g_dome[nCntDome].nBlock_Width + 1) * 2 *
			g_dome[nCntDome].nBlock_Depth + 2 *
			(g_dome[nCntDome].nBlock_Depth - 1);

		g_dome[nCntDome].nNumPolygon =
			g_dome[nCntDome].nBlock_Depth *
			g_dome[nCntDome].nBlock_Width * 2 +
			4 * (g_dome[nCntDome].nBlock_Depth - 1);

		// 使用状態
		g_dome[nCntDome].bUse = false;
	}

	// 頂点情報の作成
	MakeVertexDome(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitDome(void)
{
	// ドームの描画の終了処理
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// テクスチャの開放
		if (pTextureDome[g_dome[nCntDome].nType] != NULL)
		{
			pTextureDome[g_dome[nCntDome].nType]->Release();
			pTextureDome[g_dome[nCntDome].nType] = NULL;
		}

		// 頂点バッファの開放
		if (g_dome[nCntDome].pVtxBuffDome != NULL)
		{
			g_dome[nCntDome].pVtxBuffDome->Release();
			g_dome[nCntDome].pVtxBuffDome = NULL;
		}

		// 頂点インデックスの開放
		if (g_dome[nCntDome].pIndexMeshField != NULL)
		{
			g_dome[nCntDome].pIndexMeshField->Release();
			g_dome[nCntDome].pIndexMeshField = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateDome(void)
{
	// 変数宣言
	int nCountDirect;
	int nCountWidth;

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// ドームの更新
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// 使用状態のもの
		if (g_dome[nCntDome].bUse == true)
		{
			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_dome[nCntDome].pVtxBuffDome->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
				{
					// テクスチャーの設定
					pVtx[0].tex = pVtx[0].tex +
						D3DXVECTOR2(
							1.0f / g_dome[nCntDome].nBlock_Width * 0.1f,
							0.0f);

					// ポイント合わせ
					pVtx++;
				}
			}

			// アンロック
			g_dome[nCntDome].pVtxBuffDome->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF, int(255 * 0.1f));				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)

																// 両面を描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ドームの描画
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// 使用状態のもの
		if (g_dome[nCntDome].bUse == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_dome[nCntDome].mtxWorldDome);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				g_dome[nCntDome].rot.y, g_dome[nCntDome].rot.x, g_dome[nCntDome].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_dome[nCntDome].mtxWorldDome,	// 1
				&g_dome[nCntDome].mtxWorldDome,					// 2
				&mtxRot);										// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				g_dome[nCntDome].pos.x, g_dome[nCntDome].pos.y, g_dome[nCntDome].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_dome[nCntDome].mtxWorldDome,	// 1
				&g_dome[nCntDome].mtxWorldDome,					// 2
				&mtxTrans);							// 3

													// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_dome[nCntDome].mtxWorldDome);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_dome[nCntDome].pVtxBuffDome, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(g_dome[nCntDome].pIndexMeshField);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTextureDome[g_dome[nCntDome].nType]);

			// ドームの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_dome[nCntDome].nNumberVertexMeshField, 0, g_dome[nCntDome].nNumPolygon);
		}
	}
	// 裏面(左回り)でカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// アルファテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexDome(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCountDirect;
	int nCountWidth;
	float fAngle;
	float fRadian;

	for (int nCntDomeType = 0; nCntDomeType < DOMETYPE_MAX; nCntDomeType++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			g_paTexDomeFile[nCntDomeType],
			&pTextureDome[nCntDomeType]);
	}

	// ドームの頂点情報作成
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// 角度の計算
		fAngle = D3DX_PI * 2 / g_dome[nCntDome].nBlock_Width;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_dome[nCntDome].nNumberVertexMeshField,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_dome[nCntDome].pVtxBuffDome,
			NULL);

		// インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_dome[nCntDome].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_dome[nCntDome].pIndexMeshField,
			NULL);

		// 頂点情報の設定
		VERTEX_3D *pVtx;	// 頂点情報へのポイント

							// インデックス情報の設定
		WORD * pIdx;		// インデックスデータへのポインタを取得

							// 頂点データの範囲をロックし、頂点バッファへのポインタ
		g_dome[nCntDome].pVtxBuffDome->Lock(0, 0, (void **)&pVtx, 0);

		//頂点設定 //
		//行ループ
		for (nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth + 1; nCountDirect++)
		{
			// 列ループ
			for (nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
			{
				// ラジアン値
				fRadian = fAngle * nCountWidth;

				// -3.14未満
				if (fRadian < -D3DX_PI)
				{
					fRadian += D3DX_PI * 2;
				}

				// 3.14超過
				else if (fRadian > D3DX_PI)
				{
					fRadian -= D3DX_PI * 2;
				}

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(
					sinf(fRadian) * g_dome[nCntDome].size.x,
					g_dome[nCntDome].size.y * nCountDirect,
					cosf(fRadian) * g_dome[nCntDome].size.z);

				// 法線ベクトルの設定
				pVtx[0].nor = pVtx[0].pos;
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				// 色の設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

				// テクスチャーの設定
				pVtx[0].tex = D3DXVECTOR2(
					1.0f / g_dome[nCntDome].nBlock_Width * nCountWidth,
					1.0f / g_dome[nCntDome].nBlock_Depth * (g_dome[nCntDome].nBlock_Depth - nCountDirect));

				// ポイント合わせ
				pVtx++;
			}
		}

		// アンロック
		g_dome[nCntDome].pVtxBuffDome->Unlock();

		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		g_dome[nCntDome].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

		// 縦ブロック個数
		for (nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth; nCountDirect++)
		{
			// ２回目のループ以降
			if (nCountDirect >= 1)
			{
				// 縮退ドーム分の頂点追加
				pIdx[0] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + g_dome[nCntDome].nBlock_Width + 1;

				// インデックスのポイント合わせ
				pIdx++;
			}

			// 横ブロックの頂点数
			for (nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
			{
				// 描画順番のインデックス
				pIdx[0] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + nCountWidth + g_dome[nCntDome].nBlock_Width + 1;
				pIdx[1] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + nCountWidth;

				// インデックスのポイント合わせ
				pIdx += 2;
			}

			// 縮退ドームを作る必要がある場合
			if (nCountDirect < g_dome[nCntDome].nBlock_Depth - 1)
			{
				// 縮退ドーム分の頂点追加
				pIdx[0] = nCountDirect * (g_dome[nCntDome].nBlock_Width + 1) + g_dome[nCntDome].nBlock_Width;

				// インデックスのポイント合わせ
				pIdx++;
			}
		}

		// アンロック
		g_dome[nCntDome].pIndexMeshField->Unlock();
	}
}

// ------------------------------------------------------------------------------------------
// 使用中のものを初期化
// ------------------------------------------------------------------------------------------
void UseInitDome(void)
{
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		if (g_dome[nCntDome].bUse == true)
		{
			g_dome[nCntDome].bUse = false;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ドームの設置
// ------------------------------------------------------------------------------------------
int SetDome(
	D3DXVECTOR3 pos,
	D3DXCOLOR	col,
	int nType)
{
	// 変数宣言
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// ドームの設置
	for (int nCntDome = 0; nCntDome < DOME_MAX; nCntDome++)
	{
		// 未使用のものを選択
		if (g_dome[nCntDome].bUse == false)
		{
			g_dome[nCntDome].pos = pos;
			g_dome[nCntDome].nType = nType;
			g_dome[nCntDome].col = col;
			g_dome[nCntDome].bUse = true;

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_dome[nCntDome].pVtxBuffDome->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (int nCountDirect = 0; nCountDirect < g_dome[nCntDome].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (int nCountWidth = 0; nCountWidth < g_dome[nCntDome].nBlock_Width + 1; nCountWidth++)
				{
					// テクスチャーの設定
					pVtx[0].col = g_dome[nCntDome].col;

					// ポイント合わせ
					pVtx++;
				}
			}

			// アンロック
			g_dome[nCntDome].pVtxBuffDome->Unlock();

			return nCntDome;
		}
	}
	return 0;
}

// ------------------------------------------------------------------------------------------
// 削除
// ------------------------------------------------------------------------------------------
void DeleteDome(int nIdx)
{
	g_dome[nIdx].bUse = false;
}

// ------------------------------------------------------------------------------------------
// ドーム情報
// ------------------------------------------------------------------------------------------
DOME *GetDome(void)
{
	return &g_dome[0];
}