// ------------------------------------------------------------------------------------------
//
// 床処理 [floor.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "floor.h"

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
	FLOORTYPE_00_ROAD1 = 0,
	FLOORTYPE_01_ROAD2,
	FLOORTYPE_MAX
} FLOORTYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexFLOOR(LPDIRECT3DDEVICE9 pDevice);
void LoadFloor(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
FLOOR				g_floor[FLOOR_MAX];						// 床
LPDIRECT3DTEXTURE9	g_apTextureFloor[FLOORTYPE_MAX] = {};	// 頂点テクスチャーのポインタ

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitFLOOR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntFloor;

	// テクスチャーファイル名定義
	char *paTextureFile[FLOORTYPE_MAX] =
	{
		{ "data/TEXTURE/mesh/00_road.jpg" },		// 00_道1
		{ "data/TEXTURE/mesh/00_greenfloor.jpg" },	// 01_道２
	};

	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		// 位置・回転・移動量・サイズの初期設定
		g_floor[nCntFloor].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].size = D3DXVECTOR3(500.0f, 0.0f, 500.0f);

		// 使用状態・表示状態の初期設定
		g_floor[nCntFloor].bUse = false;
		g_floor[nCntFloor].bDisp = false;

		// ブロックの初期設定
		g_floor[nCntFloor].nBlock_Depth = 2;
		g_floor[nCntFloor].nBlock_Width = 2;

		// ブロック描画の原点の初期設定
		g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
			g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
			0.0f,
			g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

		// インデックス・バッファの初期設定
		g_floor[nCntFloor].pIndexMeshField = NULL;
		g_floor[nCntFloor].pVtxBuffFLOOR = NULL;

		// 総頂点数・インデックス・床の初期設定(計算)
		g_floor[nCntFloor].nNumberVertexMeshField = 
			(g_floor[nCntFloor].nBlock_Depth + 1) * (g_floor[nCntFloor].nBlock_Width + 1);
		g_floor[nCntFloor].nNumIndex = 
			(g_floor[nCntFloor].nBlock_Width + 1) * 2 * g_floor[nCntFloor].nBlock_Depth +
			2 * (g_floor[nCntFloor].nBlock_Depth - 1);
			g_floor[nCntFloor].nNumPolygon = 
				g_floor[nCntFloor].nBlock_Depth * g_floor[nCntFloor].nBlock_Width * 2 + 
			4 * (g_floor[nCntFloor].nBlock_Depth - 1);
	}

	for (int nCntTex = 0; nCntTex < FLOORTYPE_MAX; nCntTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntTex],
			&g_apTextureFloor[nCntTex]);
	}

	// 床情報を読み込み
	LoadFloor();

	// 頂点情報の作成
	MakeVertexFLOOR(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitFLOOR(void)
{
	for (int nCntTex = 0; nCntTex < FLOORTYPE_MAX; nCntTex++)
	{
		// テクスチャの開放
		if (g_apTextureFloor[nCntTex] != NULL)
		{
			g_apTextureFloor[nCntTex]->Release();
			g_apTextureFloor[nCntTex] = NULL;
		}
	}

	for (int nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		// 頂点バッファの開放
		if (g_floor[nCntFloor].pVtxBuffFLOOR != NULL)
		{
			g_floor[nCntFloor].pVtxBuffFLOOR->Release();
			g_floor[nCntFloor].pVtxBuffFLOOR = NULL;
		}

		// 頂点インデックスの開放
		if (g_floor[nCntFloor].pIndexMeshField != NULL)
		{
			g_floor[nCntFloor].pIndexMeshField->Release();
			g_floor[nCntFloor].pIndexMeshField = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateFLOOR(void)
{

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawFLOOR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == true)
		{

			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_floor[nCntFloor].mtxWorldFLOOR);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				g_floor[nCntFloor].rot.y, g_floor[nCntFloor].rot.x, g_floor[nCntFloor].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_floor[nCntFloor].mtxWorldFLOOR,	// 1
				&g_floor[nCntFloor].mtxWorldFLOOR,					// 2
				&mtxRot);							// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				g_floor[nCntFloor].pos.x, g_floor[nCntFloor].pos.y, g_floor[nCntFloor].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_floor[nCntFloor].mtxWorldFLOOR,	// 1
				&g_floor[nCntFloor].mtxWorldFLOOR,					// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_floor[nCntFloor].mtxWorldFLOOR);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_floor[nCntFloor].pVtxBuffFLOOR, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(
				g_floor[nCntFloor].pIndexMeshField);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(
				0, 
				g_apTextureFloor[g_floor[nCntFloor].nType]);

			if (g_floor[nCntFloor].bDisp == true)
			{
				// 床の描画
				pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLESTRIP,
					0, 
					0, 
					g_floor[nCntFloor].nNumberVertexMeshField, 
					0, 
					g_floor[nCntFloor].nNumPolygon);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexFLOOR(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCountDirect;
	int nCountWidth;
	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == true)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *
				g_floor[nCntFloor].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_floor[nCntFloor].pVtxBuffFLOOR,
				NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * 
				g_floor[nCntFloor].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_floor[nCntFloor].pIndexMeshField,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

			// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_floor[nCntFloor].pVtxBuffFLOOR->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_floor[nCntFloor].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_floor[nCntFloor].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_floor[nCntFloor].OriginBlock.x +
							(g_floor[nCntFloor].size.x * nCountWidth),
							0.0f,
							g_floor[nCntFloor].OriginBlock.z -
							(g_floor[nCntFloor].size.z * nCountDirect));

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f, 0.0f);

					// カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);

					// テクスチャーの設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// ポイント合わせ
					pVtx++;
				}
			}

			// アンロック
			g_floor[nCntFloor].pVtxBuffFLOOR->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_floor[nCntFloor].pIndexMeshField->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_floor[nCntFloor].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退床分の頂点追加
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_floor[nCntFloor].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + nCountWidth + g_floor[nCntFloor].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退床を作る必要がある場合
				if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
				{
					// 縮退床分の頂点追加
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_floor[nCntFloor].pIndexMeshField->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 床読み込み処理
// ------------------------------------------------------------------------------------------
void LoadFloor(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntFloor = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data/SAVE/TUTORIAL/floor.txt", "r");
	}

	// ボス用
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/floor.txt", "r");
	}

	// それ以外
	else
	{
		pFile = fopen("data/SAVE/floor.txt", "r");
	}

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
				if (strcmp(cHeadText, "FLOORSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_FLOORSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_floor[nCntFloor].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_floor[nCntFloor].pos.x, &g_floor[nCntFloor].pos.y, &g_floor[nCntFloor].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_floor[nCntFloor].rot.x, &g_floor[nCntFloor].rot.y, &g_floor[nCntFloor].rot.z);
						}

						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_floor[nCntFloor].nBlock_Depth);
						}

						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_floor[nCntFloor].nBlock_Width);
						}

						// xサイズ情報読み込み
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_floor[nCntFloor].size.x);
						}

						// yサイズ情報読み込み
						else if (strcmp(cHeadText, "ZSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_floor[nCntFloor].size.z);
						}

					}

					// ブロック描画の原点の設定
					g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
						g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
						0.0f,
						g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

					// 総頂点数・インデックス・床の設定(計算)
					g_floor[nCntFloor].nNumberVertexMeshField = (g_floor[nCntFloor].nBlock_Depth + 1) * (g_floor[nCntFloor].nBlock_Width + 1);
					g_floor[nCntFloor].nNumIndex = (g_floor[nCntFloor].nBlock_Width + 1) * 2 * g_floor[nCntFloor].nBlock_Depth + 2 * (g_floor[nCntFloor].nBlock_Depth - 1);
					g_floor[nCntFloor].nNumPolygon = g_floor[nCntFloor].nBlock_Depth * g_floor[nCntFloor].nBlock_Width * 2 + 4 * (g_floor[nCntFloor].nBlock_Depth - 1);

					// 使用状態・表示状態
					g_floor[nCntFloor].bUse = true;
					g_floor[nCntFloor].bDisp = true;

					// 床カウントの更新
					nCntFloor++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "床データの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
bool ColisionFLOOR(
	D3DXVECTOR3 *pos,			// 現在のポジション
	D3DXVECTOR3 *posOld,		// 前回のポジション
	D3DXVECTOR3 *move,			// 移動量
	D3DXVECTOR3 *size,			// サイズ
	FLOOR		**ppFloorRet	// 床のアドレス情報のアドレス
)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntFloor;			// カウント床

	// 番地情報があったら
	if (ppFloorRet != NULL)
	{
		*ppFloorRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++)
	{
		// 使用状態
		if (g_floor[nCntFloor].bUse == true)
		{
			// ブロックのX範囲
			if (pos->x + size->x * 0.5f >
				g_floor[nCntFloor].pos.x - g_floor[nCntFloor].size.x * 0.5f * g_floor[nCntFloor].nBlock_Width&&
				pos->x - size->x * 0.5f <
				g_floor[nCntFloor].pos.x + g_floor[nCntFloor].size.x * 0.5f * g_floor[nCntFloor].nBlock_Width&&
				pos->z + size->z * 0.5f >
				g_floor[nCntFloor].pos.z - g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth&&
				pos->z - size->z * 0.5f <
				g_floor[nCntFloor].pos.z + g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth)
			{
				// 当たり判定(上)
				if (pos->y < g_floor[nCntFloor].pos.y &&
					posOld->y >= g_floor[nCntFloor].pos.y)
				{
					// 番地の中身の情報に代入
					*ppFloorRet = &g_floor[nCntFloor];

					// めり込んでいる
					bLand = true;

					// ブロック状の上に
					pos->y = g_floor[nCntFloor].pos.y;

					// 移動量の初期化
					move->y = 0.0f;
				}

				// 当たり判定(下)
				if (pos->y + size->y > g_floor[nCntFloor].pos.y &&
					posOld->y + size->y <= g_floor[nCntFloor].pos.y)
				{
					// ブロック状の下に
					pos->y = g_floor[nCntFloor].pos.y - size->y;

					// 移動量の初期化
					move->y = 0.0f;
				}

			}
		}
	}
	return bLand;
}

// ------------------------------------------------------------------------------------------
// 床の情報
// ------------------------------------------------------------------------------------------
FLOOR *GetFLOOR(void)
{
	return &g_floor[0];
}