      // ------------------------------------------------------------------------------------------
//
// ポリゴン処理 [floor.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "floor.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "arrangement.h"
#include "game.h"
#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG


// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define FLOOR_PHONT "data/TEXTURE/soil.jpg"	// 床の写真

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------モーションセット---------- //
typedef enum
{
	FLOORTYPE_00_ROAD1 = 0,
	FLOORTYPE_01_ROAD2,
	FLOORTYPE_02_WATER,
	FLOORTYPE_MAX
} FLOORTYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexFloor(LPDIRECT3DDEVICE9 pDevice);
void Update_ChangeFloor(void);
void Update_DispFloor(void);
#ifdef _DEBUG			
// 床の情報
void Debug_Floor(void);
#endif


// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
FLOOR g_floor[ALL_MAX];						// ポリゴン
FLOORSET g_FloorSet;											// 配置情報
LPDIRECT3DTEXTURE9		g_apTextureFloor[FLOORTYPE_MAX] = {};	// 頂点テクスチャーのポインタ

// 3D用のセーブファイル
const char g_cFloorFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/floor.txt",
	"data/SAVE/3D/Stage_2/floor.txt",
	"data/SAVE/3D/Stage_3/floor.txt",
	"data/SAVE/3D/Stage_4/floor.txt",
	"data/SAVE/3D/Stage_5/floor.txt"
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntFloor;

	// テクスチャーファイル名定義
	char *paTextureFile[FLOORTYPE_MAX] =
	{
		{ "data/TEXTURE/mesh/00_road.jpg" },			// 00_道1
		{ "data/TEXTURE/mesh/01_Savannah_field.jpg" },	// 01_道２
		{ "data/TEXTURE/soil.jpg" },					// 02_湖
	};

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// 位置・回転・移動量・サイズの初期設定
		g_floor[nCntFloor].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_floor[nCntFloor].size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

		// 使用状態・表示状態の初期設定
		g_floor[nCntFloor].bUse = false;
		g_floor[nCntFloor].bDisp = false;
		g_floor[nCntFloor].bSelect = false;

		// ブロックの初期設定
		g_floor[nCntFloor].nBlock_Depth = 1;
		g_floor[nCntFloor].nBlock_Width = 1;

		// ブロック描画の原点の設定
		g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
			g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
			0.0f,
			g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

		// 総頂点数・インデックス・ポリゴンの設定(計算)
		g_floor[nCntFloor].nNumberVertexMeshField =
			(g_floor[nCntFloor].nBlock_Depth + 1) *
			(g_floor[nCntFloor].nBlock_Width + 1);
		g_floor[nCntFloor].nNumIndex =
			(g_floor[nCntFloor].nBlock_Width + 1) * 2
			* g_floor[nCntFloor].nBlock_Depth +
			2 * (g_floor[nCntFloor].nBlock_Depth - 1);
		g_floor[nCntFloor].nNumPolygon =
			g_floor[nCntFloor].nBlock_Depth *
			g_floor[nCntFloor].nBlock_Width * 2 +
			4 * (g_floor[nCntFloor].nBlock_Depth - 1);

		// インデックス・バッファの初期設定
		g_floor[nCntFloor].pIndexMeshField = NULL;
		g_floor[nCntFloor].pVtxBuffFLOOR = NULL;
	}

	// 配置情報の初期化
	g_FloorSet.nBlock_Depth = 1;
	g_FloorSet.nBlock_Width = 1;
	g_FloorSet.nType = 0;
	g_FloorSet.size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

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
	MakeVertexFloor(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitFloor(void)
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

	for (int nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
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
void UpdateFloor(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 床の変化処理
	Update_ChangeFloor();

	// 配置時の場所把握
	Update_DispFloor();

	// 画面内を左クリックで、カーソルの位置に敵配置
	if (GetMousePressTrigger(0))
	{
		SetFloor(
			pArrangement->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			g_FloorSet.size,
			g_FloorSet.nBlock_Depth,
			g_FloorSet.nBlock_Width,
			g_FloorSet.nType);
	}

	// セーブ
	if (GetKeyboardSpeacePress(DIK_F6))
	{
		SaveFloor();
	}
#ifdef _DEBUG	
	if (GetGameSelect() == GAMESELECT_FLOOR)
	{
		// 床の情報表示
		Debug_Floor();
	}
#endif
}

// ------------------------------------------------------------------------------------------
// 床の変化処理
// ------------------------------------------------------------------------------------------
void Update_ChangeFloor(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 縦のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_FloorSet.nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_FloorSet.nBlock_Depth--;
	}

	// 横のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_FloorSet.nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_FloorSet.nBlock_Width--;
	}

	// 縦のサイズを増減
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_FloorSet.size.z += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_FloorSet.size.z -= 50.0f;
	}

	// 横のサイズを増減
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_FloorSet.size.x += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_FloorSet.size.x -= 50.0f;
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_FloorSet.nType++;

		// 限界値まで行ったら
		if (g_FloorSet.nType >= FLOORTYPE_MAX)
		{
			// 初期化
			g_FloorSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 配置の時の場所把握処理
// ------------------------------------------------------------------------------------------
void Update_DispFloor(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// 変数宣言
	int nCountDirect;
	int nCountWidth;

	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == false)
		{
			g_floor[nCntFloor].nType = g_FloorSet.nType;

			if (g_floor[nCntFloor].nBlock_Depth != g_FloorSet.nBlock_Depth ||
				g_floor[nCntFloor].nBlock_Width != g_FloorSet.nBlock_Width ||
				g_floor[nCntFloor].size != g_FloorSet.size)
			{
				g_floor[nCntFloor].nBlock_Depth = g_FloorSet.nBlock_Depth;
				g_floor[nCntFloor].nBlock_Width = g_FloorSet.nBlock_Width;
				g_floor[nCntFloor].size = g_FloorSet.size;

				// ブロック描画の原点の設定
				g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
					g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
					0.0f,
					g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

				// 総頂点数・インデックス・ポリゴンの設定(計算)
				g_floor[nCntFloor].nNumberVertexMeshField =
					(g_floor[nCntFloor].nBlock_Depth + 1) *
					(g_floor[nCntFloor].nBlock_Width + 1);
				g_floor[nCntFloor].nNumIndex =
					(g_floor[nCntFloor].nBlock_Width + 1) * 2
					* g_floor[nCntFloor].nBlock_Depth +
					2 * (g_floor[nCntFloor].nBlock_Depth - 1);
				g_floor[nCntFloor].nNumPolygon =
					g_floor[nCntFloor].nBlock_Depth *
					g_floor[nCntFloor].nBlock_Width * 2 +
					4 * (g_floor[nCntFloor].nBlock_Depth - 1);

				// 頂点バッファの生成
				pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_floor[nCntFloor].pVtxBuffFLOOR,
					NULL);

				// インデックスバッファの生成
				pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
								g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
								0.0f,
								g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

						// 法線ベクトルの設定
						pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

						if (g_floor[nCntFloor].bUse == false)
						{
							// 色の設定
							pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
						}

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
						// 縮退ポリゴン分の頂点追加
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

					// 縮退ポリゴンを作る必要がある場合
					if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

						// インデックスのポイント合わせ
						pIdx++;
					}
				}

				// アンロック
				g_floor[nCntFloor].pIndexMeshField->Unlock();
			}

			g_floor[nCntFloor].bDisp = true;
			// 配置の位置情報を取得
			g_floor[nCntFloor].pos = pArrangement->pos;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bDisp == true)
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
			pDevice->SetIndices(g_floor[nCntFloor].pIndexMeshField);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(
				0,
				g_apTextureFloor[g_floor[nCntFloor].nType]);

			// ポリゴンの描画
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

// ------------------------------------------------------------------------------------------
// 床の設置
// ------------------------------------------------------------------------------------------
void SetFloor(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	int nCntFloor;		// カウント床
	int nCountDirect;	// 縦ブロックのカウント
	int nCountWidth;	// 横ブロックのカウント

	// 床の設置をする
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// 未使用の床
		if (g_floor[nCntFloor].bUse == false)
		{
			// 情報を変数に代入
			g_floor[nCntFloor].pos = pos;
			g_floor[nCntFloor].rot = rot;
			g_floor[nCntFloor].size = size;
			g_floor[nCntFloor].nBlock_Depth = nBlock_Depth;
			g_floor[nCntFloor].nBlock_Width = nBlock_Width;
			g_floor[nCntFloor].nType = nType;

			// ブロック描画の原点の設定
			g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
				g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
				0.0f,
				g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

			// 総頂点数・インデックス・ポリゴンの設定(計算)
			g_floor[nCntFloor].nNumberVertexMeshField =
				(g_floor[nCntFloor].nBlock_Depth + 1) *
				(g_floor[nCntFloor].nBlock_Width + 1);
			g_floor[nCntFloor].nNumIndex =
				(g_floor[nCntFloor].nBlock_Width + 1) * 2
				* g_floor[nCntFloor].nBlock_Depth +
				2 * (g_floor[nCntFloor].nBlock_Depth - 1);
			g_floor[nCntFloor].nNumPolygon =
				g_floor[nCntFloor].nBlock_Depth *
				g_floor[nCntFloor].nBlock_Width * 2 +
				4 * (g_floor[nCntFloor].nBlock_Depth - 1);

			// 使用状態・表示状態
			g_floor[nCntFloor].bUse = true;
			g_floor[nCntFloor].bDisp = true;

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_floor[nCntFloor].pVtxBuffFLOOR,
				NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
							g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
							0.0f,
							g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
					// 縮退ポリゴン分の頂点追加
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

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_floor[nCntFloor].pIndexMeshField->Unlock();

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexFloor(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCountDirect;
	int nCountWidth;
	int nCntFloor;

	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_floor[nCntFloor].pVtxBuffFLOOR,
			NULL);

		// インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
						g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
						0.0f,
						g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				if (g_floor[nCntFloor].bUse == true)
				{
					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				else
				{
					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				}

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
				// 縮退ポリゴン分の頂点追加
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

			// 縮退ポリゴンを作る必要がある場合
			if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
			{
				// 縮退ポリゴン分の頂点追加
				pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

				// インデックスのポイント合わせ
				pIdx++;
			}
		}

		// アンロック
		g_floor[nCntFloor].pIndexMeshField->Unlock();
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

	// もし表示しているものがあったら
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		if (g_floor[nCntFloor].bUse == true)
		{
			g_floor[nCntFloor].bUse = false;
			g_floor[nCntFloor].bDisp = false;
		}
	}

	// カウント初期化
	nCntFloor = 0;

	// ファイル開
	pFile = fopen(g_cFloorFIleName_3D[GetGameStage()], "r");

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

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// モデルセットが来たら
				else if (strcmp(cHeadText, "FLOORSET") == 0)
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

					// 総頂点数・インデックス・ポリゴンの設定(計算)
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
		MessageBox(NULL, "モデルデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// 配置場所がわかるために描画していたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDispFLoor(void)
{
	int nCntFloor;

	// 床のカウント
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// 配置用の床
		if (g_floor[nCntFloor].bUse == false &&
			g_floor[nCntFloor].bDisp == true)
		{
			g_floor[nCntFloor].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// セーブ処理
// ------------------------------------------------------------------------------------------
void SaveFloor(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntFloor = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// バグ対策カウント
	char	cWriteText[512];		// 文字として書き込み用
	char	cHeadText[128];			// 文字の格納
	char	cEqual[8] = { "=" };	// イコール用

	char	cSharpEnter[8];			// コメント１
	char	cComment[64];			// コメント２
	char	cEnter[8];				// コメント３
	char	cFilename[128];			// ファイルネーム

	// よく使われる文字
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// 床の種類種類
	char *cFloorType[FLOORTYPE_MAX] = 
	{
		{ "    ----------[00]:道１----------" },				// 00_道１
		{ "    ----------[01]:道２----------" },				// 01_道２
		{ "    ----------[02]:湖１----------" },				// 00_湖１
	};

	// 床データの種類
	char *cFloorData[] =
	{
		{ "TYPE"},			// タイプ
		{ "POS" },			// 位置
		{ "ROT" },			// 回転
		{ "BLOCK_DEPTH" },	// 縦ブロック
		{ "BLOCK_WIDTH" },	// 横ブロック
		{ "XSIZE" },		// Ｘサイズ
		{ "ZSIZE" },		// Zサイズ
		{ "END_FLOORSET" }	// エンドフロアー
	};
	strcpy(cFilename, FILENAME_DATE);
	strcat(cFilename, FILENAME_FORMAT);

	// ファイル開
	pFile = fopen(g_cFloorFIleName_3D[GetGameStage()], "w");

	// 開けた
	if (pFile != NULL)
	{
		//------------------ コメント ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# セーブモデル\n");

		strcat(cWriteText, "# Author : koki nishiyama\n");

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, cComment);

		strcat(cWriteText, cEnter);

		fputs(cWriteText, pFile);

		//------------------ コメント終了 ------------------//

		strcpy(cWriteText, "SCRIPT");

		fputs(cWriteText, pFile);

		// スクリプトが通った
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{
				// 床の最大数ならセーブ
				if (nCntFloor < ALL_MAX - 1)
				{
					if (g_floor[nCntFloor].bUse == true)
					{
						fputs(cEnter, pFile);

						// モデルのパーツの名前を代入
						strcpy(cWriteText, cFloorType[g_floor[nCntFloor].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    FLOORSET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_FLOORSET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cFloorData[nCntDataLoad]);

								// タイプ情報保存
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_floor[nCntFloor].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 位置情報保存
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].pos.x,
										g_floor[nCntFloor].pos.y,
										g_floor[nCntFloor].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].rot.x,
										g_floor[nCntFloor].rot.y,
										g_floor[nCntFloor].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 縦ブロック数情報保存
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].nBlock_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 横ブロック数情報保存
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].nBlock_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Ｘサイズ情報保存
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Zサイズ情報保存
								else if (strcmp(cWriteText, "ZSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_floor[nCntFloor].size.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// エンド床セット
								else if (strcmp(cWriteText, "END_FLOORSET") == 0)
								{
									sprintf(cHeadText, "    %s", cWriteText);
									fputs(cHeadText, pFile);

									// カウントデータ初期化
									nCntDataLoad = 0;
								}
							}
						}
					}
					// モデルパーツ変更
					nCntFloor++;
				}

				// エンドスクリプト
				else
				{
					fputs(cEnter, pFile);

					strcpy(cWriteText, "END_SCRIPT");

					fputs(cWriteText, pFile);
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	else
	{
		MessageBox(NULL, "床の書き込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
bool ColisionFloor(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move,D3DXVECTOR3 * size, FLOOR ** ppFloorRet)
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
	for (nCntFloor = 0; nCntFloor < ALL_MAX; nCntFloor++)
	{
		// 使用状態
		if (g_floor[nCntFloor].bUse == true)
		{
			// ブロックのX範囲
			if (pos->x + size->x * 0.5f > g_floor[nCntFloor].pos.x - g_floor[nCntFloor].size.x&&
				pos->x - size->x * 0.5f < g_floor[nCntFloor].pos.x + g_floor[nCntFloor].size.x&&
				pos->z + size->z * 0.5f > g_floor[nCntFloor].pos.z - g_floor[nCntFloor].size.z&&
				pos->z - size->z * 0.5f < g_floor[nCntFloor].pos.z + g_floor[nCntFloor].size.z)
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
// 選択中の床
// ------------------------------------------------------------------------------------------
bool SeleFloor(int nCntFloor)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	ARRANGEMENT	*pArrangement = GetArrangement();
	int nCountDirect;	// 縦ブロックのカウント
	int nCountWidth;	// 横ブロックのカウント

	// 移動
	if (GetMousePress(0))
	{
		// 配置の位置情報を取得
		g_floor[nCntFloor].pos = pArrangement->pos;
	}

	// 縦のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_floor[nCntFloor].nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_floor[nCntFloor].nBlock_Depth--;
	}

	// 横のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_floor[nCntFloor].nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_floor[nCntFloor].nBlock_Width--;
	}

	// 縦のサイズを増減
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_floor[nCntFloor].size.z += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_floor[nCntFloor].size.z -= 50.0f;
	}

	// 横のサイズを増減
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_floor[nCntFloor].size.x += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_floor[nCntFloor].size.x -= 50.0f;
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_floor[nCntFloor].nType++;

		// 限界値まで行ったら
		if (g_floor[nCntFloor].nType >= FLOORTYPE_MAX)
		{
			// 初期化
			g_floor[nCntFloor].nType = 0;
		}
	}

	// 削除
	if (GetKeyboardSpeacePress(DIK_BACK))
	{
		g_floor[nCntFloor].bDisp = false;
		g_floor[nCntFloor].bUse = false;
	}

	// ブロック描画の原点の設定
	g_floor[nCntFloor].OriginBlock = D3DXVECTOR3(
		g_floor[nCntFloor].size.x * -0.5f * g_floor[nCntFloor].nBlock_Width,
		0.0f,
		g_floor[nCntFloor].size.z * 0.5f * g_floor[nCntFloor].nBlock_Depth);

	// 総頂点数・インデックス・ポリゴンの設定(計算)
	g_floor[nCntFloor].nNumberVertexMeshField =
		(g_floor[nCntFloor].nBlock_Depth + 1) *
		(g_floor[nCntFloor].nBlock_Width + 1);
	g_floor[nCntFloor].nNumIndex =
		(g_floor[nCntFloor].nBlock_Width + 1) * 2
		* g_floor[nCntFloor].nBlock_Depth +
		2 * (g_floor[nCntFloor].nBlock_Depth - 1);
	g_floor[nCntFloor].nNumPolygon =
		g_floor[nCntFloor].nBlock_Depth *
		g_floor[nCntFloor].nBlock_Width * 2 +
		4 * (g_floor[nCntFloor].nBlock_Depth - 1);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_floor[nCntFloor].nNumberVertexMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_floor[nCntFloor].pVtxBuffFLOOR,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_floor[nCntFloor].nNumIndex,
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
					g_floor[nCntFloor].OriginBlock.x + (g_floor[nCntFloor].size.x * nCountWidth),
					0.0f,
					g_floor[nCntFloor].OriginBlock.z - (g_floor[nCntFloor].size.z * nCountDirect));

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色の設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
			// 縮退ポリゴン分の頂点追加
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

		// 縮退ポリゴンを作る必要がある場合
		if (nCountDirect < g_floor[nCntFloor].nBlock_Depth - 1)
		{
			// 縮退ポリゴン分の頂点追加
			pIdx[0] = nCountDirect * (g_floor[nCntFloor].nBlock_Width + 1) + g_floor[nCntFloor].nBlock_Width;

			// インデックスのポイント合わせ
			pIdx++;
		}
	}

	// アンロック
	g_floor[nCntFloor].pIndexMeshField->Unlock();

	// あるかないか
	return g_floor[nCntFloor].bUse;
}

// ------------------------------------------------------------------------------------------
// ポリゴン情報
// ------------------------------------------------------------------------------------------
FLOOR *GetFloor(void)
{
	return &g_floor[0];
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// デバッグ表示
// ------------------------------------------------------------------------------------------
void Debug_Floor(void)
{
	PrintDebug("/*---------<床情報>----------*/\n");
	PrintDebug("サイズ変更(x:[I][K],z:[O][L])\n");
	PrintDebug("ブロック変更(横:[Y][H],縦:[U][J])\n");
	PrintDebug("タイプ変更[1]\n");
	PrintDebug("サイズ:(%0.3f,%0.3f,%0.3f)\n",
		g_FloorSet.size.x,
		g_FloorSet.size.y,
		g_FloorSet.size.z);
	PrintDebug("ブロック(横:%d,縦:%d)\n",
		g_FloorSet.nBlock_Width,
		g_FloorSet.nBlock_Depth
	);
	PrintDebug("タイプ(%d)\n",
		g_FloorSet.nType
	);
}
#endif // _DEBUG