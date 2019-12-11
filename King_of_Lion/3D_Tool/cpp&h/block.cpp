// ------------------------------------------------------------------------------------------
//
// ブロック処理 [block.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "block.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define BLOCK_PHONT "data/TEXTURE/woods.png"
#define BLOCK_SIZE (10)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------モーションセット---------- //
typedef enum
{
	BLOCKTYPE_00_ROAD1 = 0,
	BLOCKTYPE_MAX
} BLOCKTYPE;
// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
bool ColisionBlock(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, BLOCK ** ppBlockRet);

void Update_ChangeBlock(void);
void Update_DispBlock(void);
void Update_DestroyBlock(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// 頂点テクスチャーのポインタ

BLOCK g_ablock[ALL_MAX];							// ブロック
BLOCKSET g_BlockSet;							// 配置情報

// 3D用のセーブファイル
const char g_cBlockFIleName_3D[5][128] =
{
	"data/SAVE/2D/Stage_1/block.txt",
	"data/SAVE/2D/Stage_2/block.txt",
	"data/SAVE/2D/Stage_3/block.txt",
	"data/SAVE/2D/Stage_4/block.txt",
	"data/SAVE/2D/Stage_5/block.txt"
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		g_ablock[nCntBlock].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ablock[nCntBlock].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ablock[nCntBlock].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_ablock[nCntBlock].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_ablock[nCntBlock].nBlock_Depth	= 1;
		g_ablock[nCntBlock].nBlock_Width	= 1;

		// 原点の計算
		g_ablock[nCntBlock].OriginBlock	= D3DXVECTOR3(
			g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
			g_ablock[nCntBlock].size.y * 0.5f * g_ablock[nCntBlock].nBlock_Depth,
			0.0f);

		// 総頂点の計算
		g_ablock[nCntBlock].nNumberVertexMeshField = 
			(g_ablock[nCntBlock].nBlock_Depth + 1) *
			(g_ablock[nCntBlock].nBlock_Width + 1);

		// インデックスの計算
		g_ablock[nCntBlock].nNumIndex = 
			(g_ablock[nCntBlock].nBlock_Width + 1) * 2 *
			g_ablock[nCntBlock].nBlock_Depth + 
			2 * (g_ablock[nCntBlock].nBlock_Depth - 1);

		// ポリゴンの計算
		g_ablock[nCntBlock].nNumPolygon = 
			g_ablock[nCntBlock].nBlock_Depth * 
			g_ablock[nCntBlock].nBlock_Width * 2 + 
			4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

		g_ablock[nCntBlock].nNumber = 0;

		// 使用・表示状態の初期化
		g_ablock[nCntBlock].bUse = false;
		g_ablock[nCntBlock].bDisp = false;
	}

	// 配置情報の初期化
	g_BlockSet.nBlock_Depth = 1;
	g_BlockSet.nBlock_Width = 1;
	g_BlockSet.nType = 0;
	g_BlockSet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_BlockSet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BLOCK_PHONT, &g_pTextureBlock);

	// ブロックのデータ読込
	LoadBlock();

	// 頂点情報の作成
	MakeVertexBlock(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitBlock(void)
{
	// テクスチャの開放
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// 頂点バッファの開放
		if (g_ablock[nCntBlock].pVtxBuff != NULL)
		{
			g_ablock[nCntBlock].pVtxBuff->Release();
			g_ablock[nCntBlock].pVtxBuff = NULL;
		}

		// 頂点インデックスの開放
		if (g_ablock[nCntBlock].pIndex != NULL)
		{
			g_ablock[nCntBlock].pIndex->Release();
			g_ablock[nCntBlock].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateBlock(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// マウスの情報取得
	pMouseX = GetMouseX();

	// 削除処理
	Update_DestroyBlock();

	// 床の変化処理
	Update_ChangeBlock();

	// 配置時の場所把握
	Update_DispBlock();

	// 画面内を左クリックで、カーソルの位置に敵配置
	if (GetMousePressTrigger(0))
	{
		SetBlock(
			pArrangement->pos,
			pArrangement->rot,
			g_BlockSet.size,
			g_BlockSet.nType);
	}
}

// ------------------------------------------------------------------------------------------
// 床の変化処理
// ------------------------------------------------------------------------------------------
void Update_ChangeBlock(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 縦のサイズを増減
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_BlockSet.size.y += BLOCK_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_BlockSet.size.y -= BLOCK_SIZE;
	}

	// 横のサイズを増減
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_BlockSet.size.x += BLOCK_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_BlockSet.size.x -= BLOCK_SIZE;
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_BlockSet.nType++;

		// 限界値まで行ったら
		if (g_BlockSet.nType >= BLOCKTYPE_MAX)
		{
			// 初期化
			g_BlockSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 配置の時の場所把握処理
// ------------------------------------------------------------------------------------------
void Update_DispBlock(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
												// 変数宣言
	int nCountDirect;
	int nCountWidth;

	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == false)
		{
			g_ablock[nCntBlock].nType = g_BlockSet.nType;

			// 使用状態・表示状態
			g_ablock[nCntBlock].bDisp = true;

			// 配置の位置情報を取得
			g_ablock[nCntBlock].pos = pArrangement->pos;

			// 回転情報を取得
			g_ablock[nCntBlock].rot = pArrangement->rot;

			if (g_ablock[nCntBlock].nBlock_Depth != g_BlockSet.nBlock_Depth ||
				g_ablock[nCntBlock].nBlock_Width != g_BlockSet.nBlock_Width ||
				g_ablock[nCntBlock].size != g_BlockSet.size)
			{
				g_ablock[nCntBlock].nBlock_Depth = g_BlockSet.nBlock_Depth;
				g_ablock[nCntBlock].nBlock_Width = g_BlockSet.nBlock_Width;
				g_ablock[nCntBlock].size = g_BlockSet.size;

				// 原点の計算
				g_ablock[nCntBlock].OriginBlock = D3DXVECTOR3(
					g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
					g_ablock[nCntBlock].size.y * 0.5f * g_ablock[nCntBlock].nBlock_Depth,
					0.0f);

				// 総頂点数・インデックス・ポリゴンの設定(計算)
				g_ablock[nCntBlock].nNumberVertexMeshField =
					(g_ablock[nCntBlock].nBlock_Depth + 1) *
					(g_ablock[nCntBlock].nBlock_Width + 1);

				g_ablock[nCntBlock].nNumIndex =
					(g_ablock[nCntBlock].nBlock_Width + 1) * 2 *
					g_ablock[nCntBlock].nBlock_Depth + 2 *
					(g_ablock[nCntBlock].nBlock_Depth - 1);

				g_ablock[nCntBlock].nNumPolygon =
					g_ablock[nCntBlock].nBlock_Depth * g_ablock[nCntBlock].nBlock_Width * 2 +
					4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

				// 頂点バッファの生成
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_ablock[nCntBlock].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_ablock[nCntBlock].pVtxBuff, NULL);

				// インデックスバッファの生成
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_ablock[nCntBlock].nNumIndex,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_ablock[nCntBlock].pIndex,
					NULL);

				// 頂点情報の設定
				VERTEX_3D *pVtx;	// 頂点情報へのポイント

									// インデックス情報の設定
				WORD * pIdx;		// インデックスデータへのポインタを取得

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_ablock[nCntBlock].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

				//頂点設定 //
				//行ループ
				for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth + 1; nCountDirect++)
				{
					// 列ループ
					for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
					{
						// 頂点座標の設定
						pVtx[0].pos =
							D3DXVECTOR3(
								g_ablock[nCntBlock].OriginBlock.x + 
								(g_ablock[nCntBlock].size.x * nCountWidth),
								g_ablock[nCntBlock].OriginBlock.y - 
								(g_ablock[nCntBlock].size.y * nCountDirect),
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
				g_ablock[nCntBlock].pVtxBuff->Unlock();

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_ablock[nCntBlock].pIndex->Lock(0, 0, (void **)&pIdx, 0);

				// 縦ブロック個数
				for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth; nCountDirect++)
				{
					// ２回目のループ以降
					if (nCountDirect >= 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = 
							nCountDirect * 
							(g_ablock[nCntBlock].nBlock_Width + 1) +
							g_ablock[nCntBlock].nBlock_Width + 1;

						// インデックスのポイント合わせ
						pIdx++;
					}

					// 横ブロックの頂点数
					for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
					{
						// 描画順番のインデックス
						pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth + g_ablock[nCntBlock].nBlock_Width + 1;
						pIdx[1] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth;

						// インデックスのポイント合わせ
						pIdx += 2;
					}

					// 縮退ポリゴンを作る必要がある場合
					if (nCountDirect < g_ablock[nCntBlock].nBlock_Depth - 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width;

						// インデックスのポイント合わせ
						pIdx++;
					}
				}

				// アンロック
				g_ablock[nCntBlock].pIndex->Unlock();
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 削除処理
// ------------------------------------------------------------------------------------------
void Update_DestroyBlock(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == true && g_ablock[nCntBlock].bDisp == true)
		{
			// ブロックのX範囲
			if (pArrangement->pos.x >
				g_ablock[nCntBlock].pos.x - sinf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.x * 0.5f * g_ablock[nCntBlock].nBlock_Width&&
				pArrangement->pos.x <
				g_ablock[nCntBlock].pos.x + sinf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.x * 0.5f * g_ablock[nCntBlock].nBlock_Width&&
				pArrangement->pos.z >
				g_ablock[nCntBlock].pos.z - cosf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.z * 0.5f * g_ablock[nCntBlock].nBlock_Depth&&
				pArrangement->pos.z <
				g_ablock[nCntBlock].pos.z + cosf(g_ablock[nCntBlock].rot.y) * g_ablock[nCntBlock].size.z * 0.5f * g_ablock[nCntBlock].nBlock_Depth)
			{
				// 削除処理
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_ablock[nCntBlock].bUse = false;
					g_ablock[nCntBlock].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// カウントブロック
	for (int nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bDisp == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_ablock[nCntBlock].mtxWorldBlock);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// 総合の入れ物
				g_ablock[nCntBlock].rot.y, g_ablock[nCntBlock].rot.x, g_ablock[nCntBlock].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_ablock[nCntBlock].mtxWorldBlock,	// 1
				&g_ablock[nCntBlock].mtxWorldBlock,				// 2
				&mtxRot);							// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,												// 総合の入れ物
				g_ablock[nCntBlock].pos.x, g_ablock[nCntBlock].pos.y, g_ablock[nCntBlock].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_ablock[nCntBlock].mtxWorldBlock,	// 1
				&g_ablock[nCntBlock].mtxWorldBlock,				// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_ablock[nCntBlock].mtxWorldBlock);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_ablock[nCntBlock].pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(g_ablock[nCntBlock].pIndex);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_ablock[nCntBlock].nNumberVertexMeshField,
				0,
				g_ablock[nCntBlock].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	//if (GetGameSelect() == GAMESELECT_BLOCK)
	//{
	//	// 床の情報表示
	//	DrawBlockInfo(
	//		g_BlockSet.size,
	//		g_BlockSet.nBlock_Width,
	//		g_BlockSet.nBlock_Depth,
	//		g_BlockSet.nType);
	//}
#endif

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCntBlock;
	int nCountDirect;
	int nCountWidth;

	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == true)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ablock[nCntBlock].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_ablock[nCntBlock].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

			// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_ablock[nCntBlock].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_ablock[nCntBlock].OriginBlock.x + 
							(g_ablock[nCntBlock].size.x * nCountWidth),
							g_ablock[nCntBlock].OriginBlock.y -
							(g_ablock[nCntBlock].size.y * nCountDirect),
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
			g_ablock[nCntBlock].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_ablock[nCntBlock].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] =
						nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + 
						nCountWidth + g_ablock[nCntBlock].nBlock_Width + 1;

					pIdx[1] =
						nCountDirect *
						(g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_ablock[nCntBlock].nBlock_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_ablock[nCntBlock].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// ブロックの当たり判定情報
// ------------------------------------------------------------------------------------------
bool ColisionBlock(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, BLOCK ** ppBlockRet)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntBlock;			// カウント床

	// 番地情報があったら
	if (ppBlockRet != NULL)
	{
		*ppBlockRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// 使用状態
		if (g_ablock[nCntBlock].bUse == true)
		{
			
			// ブロックのY範囲
			if (pos->y - size->y < g_ablock[nCntBlock].pos.y + g_ablock[nCntBlock].size.y&&
				pos->y + size->y > g_ablock[nCntBlock].pos.y)
			{
				// ブロックのZ範囲
				if (pos->z + size->z > g_ablock[nCntBlock].pos.z - (g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < g_ablock[nCntBlock].pos.z + (g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(左)
					if (pos->x + size->z >
						g_ablock[nCntBlock].pos.x&&
						posOld->x + size->z <=
						g_ablock[nCntBlock].pos.x)
					{
						// 番地の中身の情報に代入
						*ppBlockRet = &g_ablock[nCntBlock];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_ablock[nCntBlock].pos.x - size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

					// ブロックのZ範囲
				if (pos->z + size->z > g_ablock[nCntBlock].pos.z + (g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < g_ablock[nCntBlock].pos.z -(g_ablock[nCntBlock].size.x * sinf(g_ablock[nCntBlock].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(右)
					if (pos->x - size->z <
						g_ablock[nCntBlock].pos.x&&
						posOld->x - size->z >=
						g_ablock[nCntBlock].pos.x)
					{
						// 番地の中身の情報に代入
						*ppBlockRet = &g_ablock[nCntBlock];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_ablock[nCntBlock].pos.x + size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

				// ブロックのX範囲
				if (pos->x + size->z > g_ablock[nCntBlock].pos.x - (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < g_ablock[nCntBlock].pos.x + (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(手前)
					if (pos->z + size->z > g_ablock[nCntBlock].pos.z&&
						posOld->z + size->z <= g_ablock[nCntBlock].pos.z)
					{
						// 番地の中身の情報に代入
						*ppBlockRet = &g_ablock[nCntBlock];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_ablock[nCntBlock].pos.z - size->z;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}

					// ブロックのX範囲
				if (pos->x + size->z > g_ablock[nCntBlock].pos.x + (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < g_ablock[nCntBlock].pos.x - (g_ablock[nCntBlock].size.x * cosf(g_ablock[nCntBlock].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(奥)
					if (pos->z - size->z < g_ablock[nCntBlock].pos.z&&
						posOld->z - size->z >= g_ablock[nCntBlock].pos.z)
					{
						// 番地の中身の情報に代入
						*ppBlockRet = &g_ablock[nCntBlock];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_ablock[nCntBlock].pos.z + size->z;

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
void NotDispBlock(void)
{
	int nCntBlock;

	// 床のカウント
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// 配置用の床
		if (g_ablock[nCntBlock].bUse == false &&
			g_ablock[nCntBlock].bDisp == true)
		{
			g_ablock[nCntBlock].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// セーブ処理
// ------------------------------------------------------------------------------------------
void SaveBlock(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntBlock = 0;
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
	char *cBlockType[BLOCKTYPE_MAX] =
	{
		{ "    ----------[00]:ブロック1----------" },				// 00_道１
	};

	// 床データの種類
	char *cBlockData[] =
	{
		{ "TYPE" },			// タイプ
		{ "POS" },			// 位置
		{ "SIZE" },			// 回転
		{ "END_BLOCKSET" }	// エンドフロアー
	};

	// ファイル開
	pFile = fopen(g_cBlockFIleName_3D[GetGameStage()], "w");

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
				if (nCntBlock < ALL_MAX - 1)
				{
					if (g_ablock[nCntBlock].bUse == true)
					{
						fputs(cEnter, pFile);

						// モデルのパーツの名前を代入
						strcpy(cWriteText, cBlockType[g_ablock[nCntBlock].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    BLOCKSET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_BLOCKSET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cBlockData[nCntDataLoad]);

								// タイプ情報保存
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_ablock[nCntBlock].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 位置情報保存
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_ablock[nCntBlock].pos.x,
										g_ablock[nCntBlock].pos.y,
										g_ablock[nCntBlock].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// サイズ情報保存
								else if (strcmp(cWriteText, "SIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f\n", 
										&cWriteText, &cEqual,
										g_ablock[nCntBlock].size.x,
										g_ablock[nCntBlock].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// エンド床セット
								else if (strcmp(cWriteText, "END_BLOCKSET") == 0)
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
					nCntBlock++;
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
		MessageBox(NULL, "ブロックデータの書き込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// ブロック読み込み処理
// ------------------------------------------------------------------------------------------
void LoadBlock(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntBlock = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// もし表示しているものがあったら
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		if (g_ablock[nCntBlock].bUse == true)
		{
			g_ablock[nCntBlock].bUse = false;
			g_ablock[nCntBlock].bDisp = false;
		}
	}

	// カウント初期化
	nCntBlock = 0;

	// ファイル開
	pFile = fopen(g_cBlockFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "BLOCKSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_BLOCKSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_ablock[nCntBlock].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_ablock[nCntBlock].pos.x, &g_ablock[nCntBlock].pos.y, &g_ablock[nCntBlock].pos.z);
						}

						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f", 
								&cDie, &cDie,
								&g_ablock[nCntBlock].size.x,
								&g_ablock[nCntBlock].size.y);
						}

					}

					// 原点の計算
					g_ablock[nCntBlock].OriginBlock = D3DXVECTOR3(
						g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
						g_ablock[nCntBlock].size.y * 0.5f * g_ablock[nCntBlock].nBlock_Depth,
						0.0f);

					// 総頂点数・インデックス・ポリゴンの設定(計算)
					g_ablock[nCntBlock].nNumberVertexMeshField =
						(g_ablock[nCntBlock].nBlock_Depth + 1) * 
						(g_ablock[nCntBlock].nBlock_Width + 1);

					g_ablock[nCntBlock].nNumIndex = 
						(g_ablock[nCntBlock].nBlock_Width + 1) * 2 * 
						g_ablock[nCntBlock].nBlock_Depth + 2 *
						(g_ablock[nCntBlock].nBlock_Depth - 1);

					g_ablock[nCntBlock].nNumPolygon =
						g_ablock[nCntBlock].nBlock_Depth * g_ablock[nCntBlock].nBlock_Width * 2 + 
						4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

					// 使用状態・表示状態
					g_ablock[nCntBlock].bUse = true;
					g_ablock[nCntBlock].bDisp = true;

					// 床カウントの更新
					nCntBlock++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "ブロックデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// 床の設置
// ------------------------------------------------------------------------------------------
void SetBlock(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	int nCntBlock;		// カウント床
	int nCountDirect;	// 縦ブロックのカウント
	int nCountWidth;	// 横ブロックのカウント

						// 床の設置をする
	for (nCntBlock = 0; nCntBlock < ALL_MAX; nCntBlock++)
	{
		// 未使用の床
		if (g_ablock[nCntBlock].bUse == false)
		{
			// 情報を変数に代入
			g_ablock[nCntBlock].pos = pos;
			g_ablock[nCntBlock].rot = rot;
			g_ablock[nCntBlock].size = size;
			g_ablock[nCntBlock].nType = nType;

			// 原点の計算
			g_ablock[nCntBlock].OriginBlock = D3DXVECTOR3(
				g_ablock[nCntBlock].size.x * -0.5f * g_ablock[nCntBlock].nBlock_Width,
				g_ablock[nCntBlock].size.y * 0.5f *g_ablock[nCntBlock].nBlock_Depth,
				0.0f);

			// 総頂点の計算
			g_ablock[nCntBlock].nNumberVertexMeshField =
				(g_ablock[nCntBlock].nBlock_Depth + 1) *
				(g_ablock[nCntBlock].nBlock_Width + 1);

			// インデックスの計算
			g_ablock[nCntBlock].nNumIndex =
				(g_ablock[nCntBlock].nBlock_Width + 1) * 2 *
				g_ablock[nCntBlock].nBlock_Depth +
				2 * (g_ablock[nCntBlock].nBlock_Depth - 1);

			// ポリゴンの計算
			g_ablock[nCntBlock].nNumPolygon =
				g_ablock[nCntBlock].nBlock_Depth *
				g_ablock[nCntBlock].nBlock_Width * 2 +
				4 * (g_ablock[nCntBlock].nBlock_Depth - 1);

			g_ablock[nCntBlock].nNumber = 0;

			// 使用状態・表示状態
			g_ablock[nCntBlock].bUse = true;
			g_ablock[nCntBlock].bDisp = true;

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ablock[nCntBlock].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_ablock[nCntBlock].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_ablock[nCntBlock].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

								// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

								// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_ablock[nCntBlock].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_ablock[nCntBlock].OriginBlock.x + (g_ablock[nCntBlock].size.x * nCountWidth),
							g_ablock[nCntBlock].OriginBlock.y - (g_ablock[nCntBlock].size.y * nCountDirect),
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
			g_ablock[nCntBlock].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_ablock[nCntBlock].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_ablock[nCntBlock].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_ablock[nCntBlock].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth + g_ablock[nCntBlock].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_ablock[nCntBlock].nBlock_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_ablock[nCntBlock].nBlock_Width + 1) + g_ablock[nCntBlock].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_ablock[nCntBlock].pIndex->Unlock();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ブロック情報
// ------------------------------------------------------------------------------------------
BLOCK *GetBlock(void)
{
	return &g_ablock[0];
}