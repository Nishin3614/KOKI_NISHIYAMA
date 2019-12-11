// ------------------------------------------------------------------------------------------
//
// 壁処理 [wall.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "wall.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define WALL_PHONT "data/TEXTURE/woods.png"
#define WALL_SIZE (10)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------モーションセット---------- //
typedef enum
{
	WALLTYPE_00_ROAD1 = 0,
	WALLTYPE_MAX
} WALLTYPE;
// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet);

void Update_ChangeWall(void);
void Update_DispWall(void);
void Update_DestroyWall(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	// 頂点テクスチャーのポインタ

WALL g_awall[ALL_MAX];							// 壁
WALLSET g_WallSet;							// 配置情報

// 3D用のセーブファイル
const char g_cWallFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/wall.txt",
	"data/SAVE/3D/Stage_2/wall.txt",
	"data/SAVE/3D/Stage_3/wall.txt",
	"data/SAVE/3D/Stage_4/wall.txt",
	"data/SAVE/3D/Stage_5/wall.txt"
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		g_awall[nCntWall].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_awall[nCntWall].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_awall[nCntWall].nBlock_Depth	= 2;
		g_awall[nCntWall].nBlock_Width	= 2;

		// 原点の計算
		g_awall[nCntWall].OriginBlock	= D3DXVECTOR3(
			g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
			g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
			0.0f);

		// 総頂点の計算
		g_awall[nCntWall].nNumberVertexMeshField = 
			(g_awall[nCntWall].nBlock_Depth + 1) *
			(g_awall[nCntWall].nBlock_Width + 1);

		// インデックスの計算
		g_awall[nCntWall].nNumIndex = 
			(g_awall[nCntWall].nBlock_Width + 1) * 2 *
			g_awall[nCntWall].nBlock_Depth + 
			2 * (g_awall[nCntWall].nBlock_Depth - 1);

		// ポリゴンの計算
		g_awall[nCntWall].nNumPolygon = 
			g_awall[nCntWall].nBlock_Depth * 
			g_awall[nCntWall].nBlock_Width * 2 + 
			4 * (g_awall[nCntWall].nBlock_Depth - 1);

		g_awall[nCntWall].nNumber = 0;

		// 使用・表示状態の初期化
		g_awall[nCntWall].bUse = false;
		g_awall[nCntWall].bDisp = false;
	}

	// 配置情報の初期化
	g_WallSet.nBlock_Depth = 1;
	g_WallSet.nBlock_Width = 1;
	g_WallSet.nType = 0;
	g_WallSet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_WallSet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, WALL_PHONT, &g_pTextureWall);

	// 壁のデータ読込
	LoadWall();

	// 頂点情報の作成
	MakeVertexWall(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitWall(void)
{
	// テクスチャの開放
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// 頂点バッファの開放
		if (g_awall[nCntWall].pVtxBuff != NULL)
		{
			g_awall[nCntWall].pVtxBuff->Release();
			g_awall[nCntWall].pVtxBuff = NULL;
		}

		// 頂点インデックスの開放
		if (g_awall[nCntWall].pIndex != NULL)
		{
			g_awall[nCntWall].pIndex->Release();
			g_awall[nCntWall].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateWall(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 削除処理
	Update_DestroyWall();

	// 床の変化処理
	Update_ChangeWall();

	// 配置時の場所把握
	Update_DispWall();

	// 画面内を左クリックで、カーソルの位置に敵配置
	if (GetMousePressTrigger(0))
	{
		SetWall(
			pArrangement->pos,
			pArrangement->rot,
			g_WallSet.size,
			g_WallSet.nBlock_Depth,
			g_WallSet.nBlock_Width,
			g_WallSet.nType);
	}
}

// ------------------------------------------------------------------------------------------
// 床の変化処理
// ------------------------------------------------------------------------------------------
void Update_ChangeWall(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 縦のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_WallSet.nBlock_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_WallSet.nBlock_Depth--;
	}

	// 横のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_WallSet.nBlock_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_WallSet.nBlock_Width--;
	}

	// 縦のサイズを増減
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_WallSet.size.y += WALL_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_WallSet.size.y -= WALL_SIZE;
	}

	// 横のサイズを増減
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_WallSet.size.x += WALL_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_WallSet.size.x -= WALL_SIZE;
	}

	// 回転量を増減
	if (GetKeyboardSpeacePress(DIK_T))
	{
		g_WallSet.rot.y += 1.57f;

		// -3.14～3.14に回転量を固定させる
		if (g_WallSet.rot.y < -D3DX_PI) g_WallSet.rot.y += D3DX_PI * 2;		// -3.14未満

		else if (g_WallSet.rot.y > D3DX_PI) g_WallSet.rot.y += -D3DX_PI * 2;	// 3.14超過

	}

	else if (GetKeyboardSpeacePress(DIK_G))
	{
		g_WallSet.rot.y -= 1.57f;

		// -3.14～3.14に回転量を固定させる
		if (g_WallSet.rot.y < -D3DX_PI) g_WallSet.rot.y += D3DX_PI * 2;		// -3.14未満

		else if (g_WallSet.rot.y > D3DX_PI) g_WallSet.rot.y += -D3DX_PI * 2;	// 3.14超過
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_WallSet.nType++;

		// 限界値まで行ったら
		if (g_WallSet.nType >= WALLTYPE_MAX)
		{
			// 初期化
			g_WallSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 配置の時の場所把握処理
// ------------------------------------------------------------------------------------------
void Update_DispWall(void)
{
	// 変数宣言												
	int nCountDirect;
	int nCountWidth;
	int nCntWall;
	// 情報取得
	ARRANGEMENT *pArrangement = GetArrangement();	// 配列
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイス

	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == false)
		{
			g_awall[nCntWall].nType = g_WallSet.nType;

			// 使用状態・表示状態
			g_awall[nCntWall].bDisp = true;

			// 配置の位置情報を取得
			g_awall[nCntWall].pos = pArrangement->pos;

			// 回転情報を取得
			g_awall[nCntWall].rot = pArrangement->rot;

			if (g_awall[nCntWall].nBlock_Depth != g_WallSet.nBlock_Depth ||
				g_awall[nCntWall].nBlock_Width != g_WallSet.nBlock_Width ||
				g_awall[nCntWall].size != g_WallSet.size)
			{
				g_awall[nCntWall].nBlock_Depth = g_WallSet.nBlock_Depth;
				g_awall[nCntWall].nBlock_Width = g_WallSet.nBlock_Width;
				g_awall[nCntWall].size = g_WallSet.size;

				// 原点の計算
				g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
					g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
					g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
					0.0f);

				// 総頂点数・インデックス・ポリゴンの設定(計算)
				g_awall[nCntWall].nNumberVertexMeshField =
					(g_awall[nCntWall].nBlock_Depth + 1) *
					(g_awall[nCntWall].nBlock_Width + 1);

				g_awall[nCntWall].nNumIndex =
					(g_awall[nCntWall].nBlock_Width + 1) * 2 *
					g_awall[nCntWall].nBlock_Depth + 2 *
					(g_awall[nCntWall].nBlock_Depth - 1);

				g_awall[nCntWall].nNumPolygon =
					g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 +
					4 * (g_awall[nCntWall].nBlock_Depth - 1);

				// 頂点バッファの生成
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_awall[nCntWall].pVtxBuff, NULL);

				// インデックスバッファの生成
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_awall[nCntWall].nNumIndex,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_awall[nCntWall].pIndex,
					NULL);

				// 頂点情報の設定
				VERTEX_3D *pVtx;	// 頂点情報へのポイント

									// インデックス情報の設定
				WORD * pIdx;		// インデックスデータへのポインタを取得

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_awall[nCntWall].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

				//頂点設定 //
				//行ループ
				for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth + 1; nCountDirect++)
				{
					// 列ループ
					for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
					{
						// 頂点座標の設定
						pVtx[0].pos =
							D3DXVECTOR3(
								g_awall[nCntWall].OriginBlock.x + 
								(g_awall[nCntWall].size.x * nCountWidth),
								g_awall[nCntWall].OriginBlock.y - 
								(g_awall[nCntWall].size.y * nCountDirect),
								0.0f);

						// 法線ベクトルの設定
						pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

						// 色の設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						// テクスチャーの設定
						pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

						// ポイント合わせ
						pVtx++;
					}
				}
				// アンロック
				g_awall[nCntWall].pVtxBuff->Unlock();

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_awall[nCntWall].pIndex->Lock(0, 0, (void **)&pIdx, 0);

				// 縦ブロック個数
				for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth; nCountDirect++)
				{
					// ２回目のループ以降
					if (nCountDirect >= 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = 
							nCountDirect * 
							(g_awall[nCntWall].nBlock_Width + 1) +
							g_awall[nCntWall].nBlock_Width + 1;

						// インデックスのポイント合わせ
						pIdx++;
					}

					// 横ブロックの頂点数
					for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
					{
						// 描画順番のインデックス
						pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth + g_awall[nCntWall].nBlock_Width + 1;
						pIdx[1] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth;

						// インデックスのポイント合わせ
						pIdx += 2;
					}

					// 縮退ポリゴンを作る必要がある場合
					if (nCountDirect < g_awall[nCntWall].nBlock_Depth - 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width;

						// インデックスのポイント合わせ
						pIdx++;
					}
				}

				// アンロック
				g_awall[nCntWall].pIndex->Unlock();
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 削除処理
// ------------------------------------------------------------------------------------------
void Update_DestroyWall(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true && g_awall[nCntWall].bDisp == true)
		{
			// ブロックのX範囲
			if (pArrangement->pos.x >
				g_awall[nCntWall].pos.x - sinf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width&&
				pArrangement->pos.x <
				g_awall[nCntWall].pos.x + sinf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width&&
				pArrangement->pos.z >
				g_awall[nCntWall].pos.z - cosf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.z * 0.5f * g_awall[nCntWall].nBlock_Depth&&
				pArrangement->pos.z <
				g_awall[nCntWall].pos.z + cosf(g_awall[nCntWall].rot.y) * g_awall[nCntWall].size.z * 0.5f * g_awall[nCntWall].nBlock_Depth)
			{
				// 削除処理
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_awall[nCntWall].bUse = false;
					g_awall[nCntWall].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// カウント壁
	for (int nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bDisp == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_awall[nCntWall].mtxWorldWall);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// 総合の入れ物
				g_awall[nCntWall].rot.y, g_awall[nCntWall].rot.x, g_awall[nCntWall].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorldWall,	// 1
				&g_awall[nCntWall].mtxWorldWall,				// 2
				&mtxRot);							// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,												// 総合の入れ物
				g_awall[nCntWall].pos.x, g_awall[nCntWall].pos.y, g_awall[nCntWall].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorldWall,	// 1
				&g_awall[nCntWall].mtxWorldWall,				// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_awall[nCntWall].mtxWorldWall);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_awall[nCntWall].pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(g_awall[nCntWall].pIndex);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_awall[nCntWall].nNumberVertexMeshField,
				0,
				g_awall[nCntWall].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	if (GetGameSelect() == GAMESELECT_WALL)
	{
		// 床の情報表示
		Debug_Wall();
	}
#endif

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCntWall;
	int nCountDirect;
	int nCountWidth;

	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true)
		{
			// 原点の計算
			g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
				g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
				g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
				0.0f);

			// 総頂点数・インデックス・ポリゴンの設定(計算)
			g_awall[nCntWall].nNumberVertexMeshField =
				(g_awall[nCntWall].nBlock_Depth + 1) *
				(g_awall[nCntWall].nBlock_Width + 1);

			g_awall[nCntWall].nNumIndex =
				(g_awall[nCntWall].nBlock_Width + 1) * 2 *
				g_awall[nCntWall].nBlock_Depth + 2 *
				(g_awall[nCntWall].nBlock_Depth - 1);

			g_awall[nCntWall].nNumPolygon =
				g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 +
				4 * (g_awall[nCntWall].nBlock_Depth - 1);

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(
				sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(
				sizeof(WORD) * g_awall[nCntWall].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

								// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

								// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_awall[nCntWall].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_awall[nCntWall].OriginBlock.x +
							(g_awall[nCntWall].size.x * nCountWidth),
							g_awall[nCntWall].OriginBlock.y -
							(g_awall[nCntWall].size.y * nCountDirect),
							0.0f);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャーの設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// ポイント合わせ
					pVtx++;
				}
			}
			// アンロック
			g_awall[nCntWall].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_awall[nCntWall].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] =
						nCountDirect *
						(g_awall[nCntWall].nBlock_Width + 1) +
						g_awall[nCntWall].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth + g_awall[nCntWall].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_awall[nCntWall].nBlock_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_awall[nCntWall].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 壁の当たり判定情報
// ------------------------------------------------------------------------------------------
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntWall;			// カウント床

	// 番地情報があったら
	if (ppWallRet != NULL)
	{
		*ppWallRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// 使用状態
		if (g_awall[nCntWall].bUse == true)
		{
			
			// ブロックのY範囲
			if (pos->y - size->y < g_awall[nCntWall].pos.y + g_awall[nCntWall].size.y&&
				pos->y + size->y > g_awall[nCntWall].pos.y)
			{
				// ブロックのZ範囲
				if (pos->z + size->z > g_awall[nCntWall].pos.z - (g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < g_awall[nCntWall].pos.z + (g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(左)
					if (pos->x + size->z >
						g_awall[nCntWall].pos.x&&
						posOld->x + size->z <=
						g_awall[nCntWall].pos.x)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_awall[nCntWall].pos.x - size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

					// ブロックのZ範囲
				if (pos->z + size->z > g_awall[nCntWall].pos.z + (g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < g_awall[nCntWall].pos.z -(g_awall[nCntWall].size.x * sinf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(右)
					if (pos->x - size->z <
						g_awall[nCntWall].pos.x&&
						posOld->x - size->z >=
						g_awall[nCntWall].pos.x)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_awall[nCntWall].pos.x + size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

				// ブロックのX範囲
				if (pos->x + size->z > g_awall[nCntWall].pos.x - (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < g_awall[nCntWall].pos.x + (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(手前)
					if (pos->z + size->z > g_awall[nCntWall].pos.z&&
						posOld->z + size->z <= g_awall[nCntWall].pos.z)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_awall[nCntWall].pos.z - size->z;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}

					// ブロックのX範囲
				if (pos->x + size->z > g_awall[nCntWall].pos.x + (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < g_awall[nCntWall].pos.x - (g_awall[nCntWall].size.x * cosf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(奥)
					if (pos->z - size->z < g_awall[nCntWall].pos.z&&
						posOld->z - size->z >= g_awall[nCntWall].pos.z)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_awall[nCntWall].pos.z + size->z;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}
			}
		}
	}

	return bLand;
}

// ------------------------------------------------------------------------------------------
// 配置場所がわかるために描画していたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDispWall(void)
{
	int nCntWall;

	// 床のカウント
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// 配置用の床
		if (g_awall[nCntWall].bUse == false &&
			g_awall[nCntWall].bDisp == true)
		{
			g_awall[nCntWall].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// セーブ処理
// ------------------------------------------------------------------------------------------
void SaveWall(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntWall = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// バグ対策カウント
	char	cWriteText[512];		// 文字として書き込み用
	char	cHeadText[128];			// 文字の格納
	char	cEqual[8] = { "=" };	// イコール用

	char	cSharpEnter[8];			// コメント１
	char	cComment[64];			// コメント２
	char	cEnter[8];				// コメント３

									// よく使われる文字
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// 床の種類種類
	char *cWallType[WALLTYPE_MAX] =
	{
		{ "    ----------[00]:壁1----------" },				// 00_道１
	};

	// 床データの種類
	char *cWallData[] =
	{
		{ "TYPE" },			// タイプ
		{ "POS" },			// 位置
		{ "ROT" },			// 回転
		{ "BLOCK_DEPTH" },	// 縦ブロック
		{ "BLOCK_WIDTH" },	// 横ブロック
		{ "XSIZE" },		// Ｘサイズ
		{ "YSIZE" },		// Zサイズ
		{ "END_WALLSET" }	// エンドフロアー
	};

	// ファイル開
	pFile = fopen(g_cWallFIleName_3D[GetGameStage()], "w");

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
				if (nCntWall < ALL_MAX - 1)
				{
					if (g_awall[nCntWall].bUse == true)
					{
						fputs(cEnter, pFile);

						// モデルのパーツの名前を代入
						strcpy(cWriteText, cWallType[g_awall[nCntWall].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    WALLSET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_WALLSET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cWallData[nCntDataLoad]);

								// タイプ情報保存
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_awall[nCntWall].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 位置情報保存
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].pos.x,
										g_awall[nCntWall].pos.y,
										g_awall[nCntWall].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].rot.x,
										g_awall[nCntWall].rot.y,
										g_awall[nCntWall].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 縦ブロック数情報保存
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_awall[nCntWall].nBlock_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 横ブロック数情報保存
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_awall[nCntWall].nBlock_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Ｘサイズ情報保存
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Zサイズ情報保存
								else if (strcmp(cWriteText, "YSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_awall[nCntWall].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// エンド床セット
								else if (strcmp(cWriteText, "END_WALLSET") == 0)
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
					nCntWall++;
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
		MessageBox(NULL, "モデルデータの書き込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 壁読み込み処理
// ------------------------------------------------------------------------------------------
void LoadWall(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntWall = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// もし表示しているものがあったら
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true)
		{
			g_awall[nCntWall].bUse = false;
			g_awall[nCntWall].bDisp = false;
		}
	}

	// カウント初期化
	nCntWall = 0;

	// ファイル開
	pFile = fopen(g_cWallFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "WALLSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_WALLSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_awall[nCntWall].pos.x, &g_awall[nCntWall].pos.y, &g_awall[nCntWall].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_awall[nCntWall].rot.x, &g_awall[nCntWall].rot.y, &g_awall[nCntWall].rot.z);
						}

						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nBlock_Depth);
						}

						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nBlock_Width);
						}

						// xサイズ情報読み込み
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_awall[nCntWall].size.x);
						}

						// yサイズ情報読み込み
						else if (strcmp(cHeadText, "YSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_awall[nCntWall].size.y);
						}

					}

					// 原点の計算
					g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
						g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
						g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
						0.0f);

					// 総頂点数・インデックス・ポリゴンの設定(計算)
					g_awall[nCntWall].nNumberVertexMeshField =
						(g_awall[nCntWall].nBlock_Depth + 1) * 
						(g_awall[nCntWall].nBlock_Width + 1);

					g_awall[nCntWall].nNumIndex = 
						(g_awall[nCntWall].nBlock_Width + 1) * 2 * 
						g_awall[nCntWall].nBlock_Depth + 2 *
						(g_awall[nCntWall].nBlock_Depth - 1);

					g_awall[nCntWall].nNumPolygon =
						g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 + 
						4 * (g_awall[nCntWall].nBlock_Depth - 1);

					// 使用状態・表示状態
					g_awall[nCntWall].bUse = true;
					g_awall[nCntWall].bDisp = true;

					// 床カウントの更新
					nCntWall++;
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
// 床の設置
// ------------------------------------------------------------------------------------------
void SetWall(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	int nCntWall;		// カウント床
	int nCountDirect;	// 縦ブロックのカウント
	int nCountWidth;	// 横ブロックのカウント

						// 床の設置をする
	for (nCntWall = 0; nCntWall < ALL_MAX; nCntWall++)
	{
		// 未使用の床
		if (g_awall[nCntWall].bUse == false)
		{
			// 情報を変数に代入
			g_awall[nCntWall].pos = pos;
			g_awall[nCntWall].rot = rot;
			g_awall[nCntWall].size = size;
			g_awall[nCntWall].nBlock_Depth = nBlock_Depth;
			g_awall[nCntWall].nBlock_Width = nBlock_Width;
			g_awall[nCntWall].nType = nType;

			// 原点の計算
			g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
				g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
				g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
				0.0f);

			// 総頂点の計算
			g_awall[nCntWall].nNumberVertexMeshField =
				(g_awall[nCntWall].nBlock_Depth + 1) *
				(g_awall[nCntWall].nBlock_Width + 1);

			// インデックスの計算
			g_awall[nCntWall].nNumIndex =
				(g_awall[nCntWall].nBlock_Width + 1) * 2 *
				g_awall[nCntWall].nBlock_Depth +
				2 * (g_awall[nCntWall].nBlock_Depth - 1);

			// ポリゴンの計算
			g_awall[nCntWall].nNumPolygon =
				g_awall[nCntWall].nBlock_Depth *
				g_awall[nCntWall].nBlock_Width * 2 +
				4 * (g_awall[nCntWall].nBlock_Depth - 1);

			g_awall[nCntWall].nNumber = 0;

			// 使用状態・表示状態
			g_awall[nCntWall].bUse = true;
			g_awall[nCntWall].bDisp = true;

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_awall[nCntWall].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

								// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

								// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_awall[nCntWall].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_awall[nCntWall].OriginBlock.x + (g_awall[nCntWall].size.x * nCountWidth),
							g_awall[nCntWall].OriginBlock.y - (g_awall[nCntWall].size.y * nCountDirect),
							0.0f);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャーの設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// ポイント合わせ
					pVtx++;
				}
			}
			// アンロック
			g_awall[nCntWall].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_awall[nCntWall].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth + g_awall[nCntWall].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_awall[nCntWall].nBlock_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_awall[nCntWall].pIndex->Unlock();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 壁情報
// ------------------------------------------------------------------------------------------
WALL *GetWall(void)
{
	return &g_awall[0];
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// デバッグ表示
// ------------------------------------------------------------------------------------------
void Debug_Wall(void)
{
	PrintDebug("/*---------<壁情報>----------*/\n");
	PrintDebug("サイズ変更(x:[I][K],z:[O][L])\n");
	PrintDebug("ブロック変更(横:[Y][H],縦:[U][J])\n");
	PrintDebug("タイプ変更[4]\n");
	PrintDebug("サイズ:(%0.3f,%0.3f,%0.3f)\n",
		g_WallSet.size.x,
		g_WallSet.size.y,
		g_WallSet.size.z);
	PrintDebug("ブロック(横:%d,縦:%d)\n",
		g_WallSet.nBlock_Width,
		g_WallSet.nBlock_Depth
	);
	PrintDebug("タイプ(%d)\n",
		g_WallSet.nType
	);
	PrintDebug("");
}
#endif // _DEBUG