      // ------------------------------------------------------------------------------------------
//
// 水処理 [water.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "water.h"

#include "input.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------モーションセット---------- //
typedef enum
{
	WATERTYPE_02_WATER = 0,
	WATERTYPE_MAX
} WATERTYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexWATER(LPDIRECT3DDEVICE9 pDevice);
void LoadWater(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
WATER				g_water[WATER_MAX];						// 水
LPDIRECT3DTEXTURE9	g_apTextureWater[WATERTYPE_MAX] = {};	// 頂点テクスチャーのポインタ

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitWATER(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntWater;

	// テクスチャーファイル名定義
	char *paTextureFile[WATERTYPE_MAX] =
	{
		{ "data/TEXTURE/mesh/02_water3.jpg" },		// 02_湖
	};

	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		// 位置・回転・移動量・サイズの初期設定
		g_water[nCntWater].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_water[nCntWater].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_water[nCntWater].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_water[nCntWater].size = D3DXVECTOR3(500.0f, 0.0f, 500.0f);

		// 使用状態・表示状態の初期設定
		g_water[nCntWater].bUse = false;
		g_water[nCntWater].bDisp = false;

		// ブロックの初期設定
		g_water[nCntWater].nBlock_Depth = 2;
		g_water[nCntWater].nBlock_Width = 2;

		// ブロック描画の原点の初期設定
		g_water[nCntWater].OriginBlock = D3DXVECTOR3(
			g_water[nCntWater].size.x * -0.5f * g_water[nCntWater].nBlock_Width,
			0.0f,
			g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth);

		// インデックス・バッファの初期設定
		g_water[nCntWater].pIndexMeshField = NULL;
		g_water[nCntWater].pVtxBuffWATER = NULL;

		// 総頂点数・インデックス・水の初期設定(計算)
		g_water[nCntWater].nNumberVertexMeshField = 
			(g_water[nCntWater].nBlock_Depth + 1) * (g_water[nCntWater].nBlock_Width + 1);
		g_water[nCntWater].nNumIndex = 
			(g_water[nCntWater].nBlock_Width + 1) * 2 * g_water[nCntWater].nBlock_Depth +
			2 * (g_water[nCntWater].nBlock_Depth - 1);
			g_water[nCntWater].nNumPolygon = 
				g_water[nCntWater].nBlock_Depth * g_water[nCntWater].nBlock_Width * 2 + 
			4 * (g_water[nCntWater].nBlock_Depth - 1);
	}

	// テクスチャー
	for (int nCntTex = 0; nCntTex < WATERTYPE_MAX; nCntTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntTex],
			&g_apTextureWater[nCntTex]);
	}

	// 水情報を読み込み
	LoadWater();

	// 頂点情報の作成
	MakeVertexWATER(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitWATER(void)
{
	for (int nCntTex = 0; nCntTex < WATERTYPE_MAX; nCntTex++)
	{
		// テクスチャの開放
		if (g_apTextureWater[nCntTex] != NULL)
		{
			g_apTextureWater[nCntTex]->Release();
			g_apTextureWater[nCntTex] = NULL;
		}
	}

	for (int nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		// 頂点バッファの開放
		if (g_water[nCntWater].pVtxBuffWATER != NULL)
		{
			g_water[nCntWater].pVtxBuffWATER->Release();
			g_water[nCntWater].pVtxBuffWATER = NULL;
		}

		// 頂点インデックスの開放
		if (g_water[nCntWater].pIndexMeshField != NULL)
		{
			g_water[nCntWater].pIndexMeshField->Release();
			g_water[nCntWater].pIndexMeshField = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateWATER(void)
{
	for (int nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		if (g_water[nCntWater].bUse == true)
		{
			if (g_water[nCntWater].nType == WATERTYPE_02_WATER)
			{
				// 頂点情報の設定
				VERTEX_3D *pVtx;	// 頂点情報へのポイント

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_water[nCntWater].pVtxBuffWATER->Lock(0, 0, (void **)&pVtx, 0);

				//頂点設定 //
				//行ループ
				for (int nCountDirect = 0; nCountDirect < g_water[nCntWater].nBlock_Depth + 1; nCountDirect++)
				{
					// 列ループ
					for (int nCountWidth = 0; nCountWidth < g_water[nCntWater].nBlock_Width + 1; nCountWidth++)
					{
						// テクスチャーの設定
						pVtx[0].tex += D3DXVECTOR2(0.0f, 0.001f);

						// ポイント合わせ
						pVtx++;
					}
				}

				// アンロック
				g_water[nCntWater].pIndexMeshField->Unlock();
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawWATER(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntWater;

	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		if (g_water[nCntWater].bUse == true)
		{

			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_water[nCntWater].mtxWorldWATER);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				g_water[nCntWater].rot.y, g_water[nCntWater].rot.x, g_water[nCntWater].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_water[nCntWater].mtxWorldWATER,	// 1
				&g_water[nCntWater].mtxWorldWATER,					// 2
				&mtxRot);							// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				g_water[nCntWater].pos.x, g_water[nCntWater].pos.y, g_water[nCntWater].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_water[nCntWater].mtxWorldWATER,	// 1
				&g_water[nCntWater].mtxWorldWATER,					// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_water[nCntWater].mtxWorldWATER);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_water[nCntWater].pVtxBuffWATER, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(
				g_water[nCntWater].pIndexMeshField);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(
				0, 
				g_apTextureWater[g_water[nCntWater].nType]);

			if (g_water[nCntWater].bDisp == true)
			{
				// 水の描画
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLESTRIP,
					0,
					0, 
					g_water[nCntWater].nNumberVertexMeshField,
					0, 
					g_water[nCntWater].nNumPolygon);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexWATER(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCountDirect;
	int nCountWidth;
	int nCntWater;

	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		if (g_water[nCntWater].bUse == true)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *
				g_water[nCntWater].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_water[nCntWater].pVtxBuffWATER,
				NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * 
				g_water[nCntWater].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_water[nCntWater].pIndexMeshField,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

			// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_water[nCntWater].pVtxBuffWATER->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_water[nCntWater].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_water[nCntWater].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_water[nCntWater].OriginBlock.x +
							(g_water[nCntWater].size.x * nCountWidth),
							0.0f,
							g_water[nCntWater].OriginBlock.z -
							(g_water[nCntWater].size.z * nCountDirect));

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

					// カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,0.5f);

					// テクスチャーの設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// ポイント合わせ
					pVtx++;
				}
			}

			// アンロック
			g_water[nCntWater].pVtxBuffWATER->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_water[nCntWater].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_water[nCntWater].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退水分の頂点追加
					pIdx[0] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + g_water[nCntWater].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_water[nCntWater].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + nCountWidth + g_water[nCntWater].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退水を作る必要がある場合
				if (nCountDirect < g_water[nCntWater].nBlock_Depth - 1)
				{
					// 縮退水分の頂点追加
					pIdx[0] = nCountDirect * (g_water[nCntWater].nBlock_Width + 1) + g_water[nCntWater].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_water[nCntWater].pIndexMeshField->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 水読み込み処理
// ------------------------------------------------------------------------------------------
void LoadWater(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntWater = 0;		// カウント水
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// チュートリアルの場合
	if (GetMode() == MODE_TUTORIAL)
	{
		return;
	}

	pFile = fopen("data/SAVE/water.txt", "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// モデルセットが来たら
				if (strcmp(cHeadText, "WATERSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_WATERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_water[nCntWater].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_water[nCntWater].pos.x, &g_water[nCntWater].pos.y, &g_water[nCntWater].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_water[nCntWater].rot.x, &g_water[nCntWater].rot.y, &g_water[nCntWater].rot.z);
						}

						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_water[nCntWater].nBlock_Depth);
						}

						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_water[nCntWater].nBlock_Width);
						}

						// xサイズ情報読み込み
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_water[nCntWater].size.x);
						}

						// yサイズ情報読み込み
						else if (strcmp(cHeadText, "ZSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_water[nCntWater].size.z);
						}

					}

					// ブロック描画の原点の設定
					g_water[nCntWater].OriginBlock = D3DXVECTOR3(
						g_water[nCntWater].size.x * -0.5f * g_water[nCntWater].nBlock_Width,
						0.0f,
						g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth);

					// 総頂点数・インデックス・水の設定(計算)
					g_water[nCntWater].nNumberVertexMeshField = (g_water[nCntWater].nBlock_Depth + 1) * (g_water[nCntWater].nBlock_Width + 1);
					g_water[nCntWater].nNumIndex = (g_water[nCntWater].nBlock_Width + 1) * 2 * g_water[nCntWater].nBlock_Depth + 2 * (g_water[nCntWater].nBlock_Depth - 1);
					g_water[nCntWater].nNumPolygon = g_water[nCntWater].nBlock_Depth * g_water[nCntWater].nBlock_Width * 2 + 4 * (g_water[nCntWater].nBlock_Depth - 1);

					// 使用状態・表示状態
					g_water[nCntWater].bUse = true;
					g_water[nCntWater].bDisp = true;

					// 水カウントの更新
					nCntWater++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "水データの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
bool ColisionWATER(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, WATER ** ppWaterRet)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntWater;			// カウント水

	// 番地情報があったら
	if (ppWaterRet != NULL)
	{
		*ppWaterRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntWater = 0; nCntWater < WATER_MAX; nCntWater++)
	{
		// 使用状態
		if (g_water[nCntWater].bUse == true)
		{
			// ブロックのX範囲
			if (pos->x + size->x * 0.5f >
				g_water[nCntWater].pos.x - g_water[nCntWater].size.x * 0.5f * g_water[nCntWater].nBlock_Width&&
				pos->x - size->x * 0.5f <
				g_water[nCntWater].pos.x + g_water[nCntWater].size.x * 0.5f * g_water[nCntWater].nBlock_Width&&
				pos->z + size->z * 0.5f >
				g_water[nCntWater].pos.z - g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth&&
				pos->z - size->z * 0.5f <
				g_water[nCntWater].pos.z + g_water[nCntWater].size.z * 0.5f * g_water[nCntWater].nBlock_Depth)
			{
				// 水
				if (g_water[nCntWater].nType == WATERTYPE_02_WATER)
				{
					// 当たり判定(上)
					if (pos->y < g_water[nCntWater].pos.y)
					{
						move->y = 0;
						//move->x = 0;
						move->z += 1.1f;
						bLand = true;
					}

				}
			}
		}
	}
	return bLand;
}

// ------------------------------------------------------------------------------------------
// 水の情報
// ------------------------------------------------------------------------------------------
WATER *GetWATER(void)
{
	return &g_water[0];
}